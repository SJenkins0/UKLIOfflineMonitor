/* ---------------- main.cc ---------------- *
 * This will extract a variety of variables  *
 * from the top diffuser root files for the  *
 * monitoring system.                        *
 *                                           *
 * SOFTWARE REQUIRED:                        *
 * ---> ROOT                                 *
 * ---> position.h                           *
 * ---> rndFunc.h                            *
 *                                           *
 *               s.j.jenkins@liverpool.ac.uk *
 * ------------------------- S. Jenkins ---- */

#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>
#include <fstream>
#include <numeric>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <typeinfo>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TVector3.h"



int main(int argc, char *argv[]){


  std::string rFile = "";

  int fnameSwitch = 0;
  std::string filename = "";
  int runNum = 99999;


  int opt;
  while ((opt = getopt(argc, argv, ":h:f:o:r:")) != -1){
    switch (opt)
      {
      case 'f':
	rFile = optarg;
	break;
      case 'r':
	runNum = std::stoi(optarg);
	break;
      case 'o':
	fnameSwitch = 1;
	filename = optarg;
	break;
      case ':':
	printf("\033[1;31m[ERROR]\033[0m -%c requires an argument.\n",optopt);
	return 0;
      case '?':
	printf("\033[1;33m[ERROR]\033[0m -%c is an unknown argument... just ignoring it.\n",optopt);
	break;
      }
  }
  
  TVector3 inj(-35.3, 777.7, 1795);
  TVector3 tar(-35.3, 777.7, -1795);
  TVector3 tarVec = tar - inj;

  if (rFile == ""){
    std::cout << "\033[1;31m[ERROR]\033[0m What am I supposed to do without a file?" << std::endl;
    return 0;
  }
  
  std::ifstream fIsAlive(rFile);
  if (!fIsAlive) {
    std::cout << "\033[1;31m[ERROR]\033[0m " << rFile
	      << " does not exist. Please supply real file." << std::endl;
    return 0;
  }
  
  TFile infile(rFile.c_str(),"READ");
  TTree *intree = 0;
  infile.GetObject("tqtree", intree);
  infile.ls();
  std::cout << "\033[1;34m[INFO]\033[0m Example event:" << std::endl;
  intree->Show(0);
  
  Int_t year = 0;
  Int_t month = 0;
  Int_t day = 0;
  Int_t hour = 0;
  Int_t minute = 0;
  Int_t second = 0;
  Int_t run = 0;
  Int_t subrun = 0;
  Int_t nev = 0;
  std::vector<int> *ihit_vec = 0;
  std::vector<int> *cable_vec = 0;
  std::vector<float> *charge_vec = 0;
  std::vector<double> *time_vec = 0;
  std::vector<double> *pmtx_vec = 0;
  std::vector<double> *pmty_vec = 0;
  std::vector<double> *pmtz_vec = 0;
  std::vector<int> *mon_ihit_vec = 0;
  std::vector<int> *mon_cable_vec = 0;
  std::vector<float> *mon_charge_vec = 0;
  std::vector<double> *mon_time_vec = 0;
  
  intree->SetBranchAddress("year", &year);
  intree->SetBranchAddress("month", &month);
  intree->SetBranchAddress("day", &day);
  intree->SetBranchAddress("hour", &hour);
  intree->SetBranchAddress("minute", &minute);
  intree->SetBranchAddress("second", &second);
  intree->SetBranchAddress("run", &run);
  intree->SetBranchAddress("subrun", &subrun);
  intree->SetBranchAddress("nev", &nev);
  intree->SetBranchAddress("ihit_vec", &ihit_vec);
  intree->SetBranchAddress("cable_vec", &cable_vec);
  intree->SetBranchAddress("charge_vec", &charge_vec);
  intree->SetBranchAddress("time_vec", &time_vec);
  intree->SetBranchAddress("pmtx_vec", &pmtx_vec);
  intree->SetBranchAddress("pmty_vec", &pmty_vec);
  intree->SetBranchAddress("pmtz_vec", &pmtz_vec);
  intree->SetBranchAddress("mon_ihit_vec", &mon_ihit_vec);
  intree->SetBranchAddress("mon_cable_vec", &mon_cable_vec);
  intree->SetBranchAddress("mon_charge_vec", &mon_charge_vec);
  intree->SetBranchAddress("mon_time_vec", &mon_time_vec);
  
  
  // CREATE FILE FOR PUSHING DATA
  std::ofstream dataFile;
  // Check if output file name given
  if (fnameSwitch == 0) filename = "top_diffuser_" + std::to_string(runNum) + ".dat";
  dataFile.open(filename);
  std::cout << "\033[1;34m[INFO]\033[0m Creating file " << filename << std::endl;

  //Set up the time cuts
  //Has to change based on the run number to account for the replacement of the top diffuser laser
  //New source starts from run 87073
  //
  double timeLowTot, timeHighTot;
  if(run >= 87073){
    timeLowTot = 650;
    timeHighTot = 1650;
    //timeLowSpt = 700;
    //timeHighSpt = 950;
  }
  else{
    timeLowTot = 700;
    timeHighTot = 1700;
  }
   

  int nEvnt = intree->GetEntries();
  for (Int_t evnt =0; evnt < nEvnt; ++evnt){
    
    intree->GetEntry(evnt);
    if ((evnt + 1) % 500 == 0){
      std::cout << "\033[1;34m[INFO]\033[0m Processing event number: " << evnt + 1 << std::endl;
    }
    
    
    // SK CHARGE AND TIME VECTOR LOOP
    float bottomQ = 0;
    float barrelQ = 0;
    float bz0y0 = 0;
    float bzlt0y0 = 0;
    float bzylt0 = 0;
    float barzylt0 = 0;
    float noiseQ = 0;
    float totQ = 0;
    TVector3 pmt;
    for (int count = 0; count < ihit_vec->size(); ++count){

      //total Q window
      if(time_vec->at(count) > timeLowTot && time_vec->at(count) < timeHighTot){
	totQ += charge_vec->at(count);

	// Original spatial cut
      
	pmt.SetXYZ(pmtx_vec->at(count),pmty_vec->at(count),pmtz_vec->at(count));
	TVector3 a = pmt - inj;
	float theta = a.Angle(tarVec) * 180 / M_PI;
	if (theta < 49){
	  
	  if (pmtz_vec->at(count) < -1805){
	    bottomQ += charge_vec->at(count);
	  }
	  else {
	    barrelQ += charge_vec->at(count);
	  }
	  if (pmtz_vec->at(count) > 0 && pmty_vec->at(count) > 0){
	    bz0y0 += charge_vec->at(count);
	  }
	  if (pmtz_vec->at(count) < 0 && pmtz_vec->at(count) > -1795 && pmty_vec->at(count) < 0){
	    barzylt0 += charge_vec->at(count);
	  }
	}//end of angle cut
	
      }//end of time cut     
            
      
    }
    
    dataFile << run << " " << subrun << " " << year << " " << month << " " << day << " "
	     << hour << " " << minute << " " << second << " " << totQ << " "
	     << barrelQ << " " << bz0y0 << " " << bottomQ << " " << barzylt0 << "\n";    
  }

  dataFile.close();
      
  return 0;
}
