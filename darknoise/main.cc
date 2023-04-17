/* ---------------- main.cc ---------------- *
 * Extract PMT-by-PMT dark noise rates in    *
 * order to calculate the average per run    *
 *                                           *
 * SOFTWARE REQUIRED:                        *
 * ---> ROOT                                 *
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
#include <algorithm>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TVector3.h"
#include "TDatime.h"


int main(int argc, char *argv[]){


  // ORGANISING ARGUMENT FLAGS
  // Set default values 
  std::string rFile = "";
  std::string nrun = "";
  int timeLowCut = 500;
  int timeHighCut = 1000;
  int fnameSwitch = 0;
  std::string filename = "";
  
  // Search command options for flags
  int opt;
  while ((opt = getopt(argc, argv, "o:r:")) != -1){
    switch (opt)
      {
      case 'o':
	fnameSwitch = 1;
	filename = optarg;
	break;
      case 'r':
	nrun = optarg;
	break;
      case ':':
	printf("\033[1;31m[ERROR]\033[0m -%c requires an argument.\n",optopt);
	return 0;
      case '?':
	printf("\033[1;33m[ERROR]\033[0m -%c is an unknown argument... just ignoring it.\n",optopt);
	break;
      }
  }

  //check we have a run number
  if (nrun == ""){
    std::cout << "\033[1;31m[ERROR]\033[0m Cannot run without a run number! Exiting.." << std::endl;
    return 0;
  }

  // CREATE FILE FOR PUSHING DATA
  std::ofstream dataFile;
  // Check if output file name given
  if (fnameSwitch == 0) filename = "darkRate_recent.dat";  
  dataFile.open(filename, std::ios::app);
  std::cout << "\033[1;34m[INFO]\033[0m Creating file " << filename << std::endl;

  double timewindow = 500e-9;
  double scale = 1000;
  
  //Root file path for the col/dif backlog data
  //Will need to edit how this works for integration into the online monitoring,
  //but better to just check it works for now
  std::string filepath = "/disk1/disk02/calib3/usr/ukli/backLog/";
  std::vector<double> darkrate_avg_vec_100, darkrate_avg_vec;
  std::vector<UInt_t> Utime_vec;
  std::vector<int> nevnt_vec;
  std::vector<int> hot_PMTs;
  
  //loop through injector options
  std::string inj_labels[] = {"dif", "col"};
  std::string inj_char[] = {"d", "c"};
  std::string infile = "";
  std::string slash = "/";
  int ninjpos = 6;//max number of injector positions
  
  for(int inj=0; inj<2; inj++){//switch between diffuser and collimator
    //Change max number of injectors to account for where we remove B3-5C from autocalib scheduler
    if(std::stoi(nrun) >= 90588 && inj==1) ninjpos = 3;
    for(int i=1; i<ninjpos; i++){
      infile = filepath+inj_labels[inj]+slash+"b"+std::to_string(i)+slash+"ukl"+inj_char[inj]+std::to_string(i)+".0"+nrun+".root";//Should put this in a separate function later
      std::cout << "Running on file " << infile << std::endl;
      std::ifstream fIsAlive(infile);
      if (!fIsAlive) {
	std::cout << "\033[1;31m[ERROR]\033[0m " << infile
		  << " does not exist. Please supply real file." << std::endl;
	//check whether this is because none of the files exist, or if its after the point where diffusers 3-5 were removed from autocalib
	return 0;
      }

      // READ INPUT FILE AND SETUP VARIABLES
      TFile inTfile(infile.c_str(),"READ");
      TTree *intree = 0;
      inTfile.GetObject("tqtree", intree);
      inTfile.ls();

      //Access the relevant tree branches
      Int_t year = 0;
      Int_t month = 0;
      Int_t day = 0;
      Int_t hour = 0;
      Int_t minute = 0;
      Int_t second = 0;
      std::vector<int> *ihit_vec = 0;
      std::vector<int> *cable_vec = 0;
      std::vector<float> *charge_vec = 0;
      std::vector<double> *time_vec = 0;

      intree->SetBranchAddress("year", &year);
      intree->SetBranchAddress("month", &month);
      intree->SetBranchAddress("day", &day);
      intree->SetBranchAddress("hour", &hour);
      intree->SetBranchAddress("minute", &minute);
      intree->SetBranchAddress("second", &second);
      intree->SetBranchAddress("ihit_vec", &ihit_vec);
      intree->SetBranchAddress("cable_vec", &cable_vec);
      intree->SetBranchAddress("charge_vec", &charge_vec);
      intree->SetBranchAddress("time_vec", &time_vec);

      int yearConv;
      TDatime date;
      UInt_t dateU;
      //vector to hold PMT hit info
      std::vector<int> hit_vec(11150, 0);
      // MAIN EVENT LOOP
      int nEvnt = intree->GetEntries();
      if(nEvnt == 0) continue;
      nevnt_vec.push_back(nEvnt);//store the number of events
      std::cout << "Number of events: " << nEvnt << std::endl;
      for (Int_t evnt =0; evnt < nEvnt; ++evnt){
	
	intree->GetEntry(evnt);
	if ((evnt + 1) % 500 == 0){
	  std::cout << "\033[1;34m[INFO]\033[0m Processing event number: " << evnt + 1 << std::endl;
	}

	//Get time info for event
	yearConv = year+1900;
	date.Set(year, month, day, hour, minute, second);
	dateU = date.Convert(); //date in unix time

	//store the time of the final event
	if(evnt==nEvnt-1) Utime_vec.push_back(dateU);

	for (int count = 0; count < ihit_vec->size(); ++count){
	  //Early time cut to select noise
	  if(time_vec->at(count) >= timeLowCut && time_vec->at(count) < timeHighCut){
	    //std::cout << "Passing time cut" << std::endl;
	    hit_vec.at(cable_vec->at(count)) += 1; //incrememnt PMT cable num for hit
	  }
	}
      }//end loop over all events
      std::vector<float> darkrate_vec_100;//vector to hold rates for hit PMTs, after cuts
      std::vector<float> darkrate_vec;
      double darkrate;

      for(int j=0; j<hit_vec.size(); j++){
	darkrate = (double)hit_vec.at(j)/(nEvnt*timewindow*scale);
	if(darkrate >= 0){
	  darkrate_vec.push_back(darkrate); //check without cuts too
	  if(darkrate <= 100) darkrate_vec_100.push_back(darkrate);
	  //make a list of hot PMTs
	  if(darkrate > 100){
	    if(std::find(hot_PMTs.begin(), hot_PMTs.end(), j) == hot_PMTs.end()){//check the cable_vector hasn't already been added to the hot list
	      hot_PMTs.push_back(j);
	    }
	  } 
	}
      }//end loop over PMT IDs

      //now calculate the average dark rate of all hit PMTs
      double dark_avg_100, dark_avg;
      int npmts=11129-90; //number of working PMTs //need to have this update but this is okay for now
      double sum=0;
      for(int j=0; j<darkrate_vec.size(); j++) sum+=darkrate_vec.at(j);
      dark_avg = sum/npmts;
      sum=0;
      for(int j=0; j<darkrate_vec_100.size(); j++) sum+=darkrate_vec_100.at(j);
      dark_avg_100 = sum/npmts;      

      darkrate_avg_vec.push_back(dark_avg);
      darkrate_avg_vec_100.push_back(dark_avg_100);
     
    }
  }

  double sum = 0;
  double sum100 = 0;
  double sum_nev = 0;

  for(int i=0; i<darkrate_avg_vec_100.size(); i++){
    sum100+=darkrate_avg_vec_100.at(i);
    std::cout << darkrate_avg_vec_100.at(i) << std::endl;
    sum+=darkrate_avg_vec.at(i);
    sum_nev+=nevnt_vec.at(i);
  }
  sum/=darkrate_avg_vec.size();
  sum100/=darkrate_avg_vec_100.size();
  sum_nev/=nevnt_vec.size();//average number of events - will cut on this

  
  std::cout << "Average (all) of all injectors = " << sum << std::endl;
  std::cout << "Average (100) of all injectors = " << sum100 << std::endl;
  std::cout << "Average num. events = " << sum_nev << std::endl;
  
  if(sum_nev > 100){
    
    //Sort the time vector
    std::sort(Utime_vec.begin(), Utime_vec.end());
    
    dataFile << nrun << " " << Utime_vec.back() << " " << sum << " " << sum100 << " " << sum_nev;
    for(int i=0; i<hot_PMTs.size(); i++){ //write hot PMT cable vectors to the dat file
      dataFile << " " << hot_PMTs.at(i);
    }
    dataFile << "\n";
  }
  
  dataFile.close();
  
  return 0;
  
    
}
