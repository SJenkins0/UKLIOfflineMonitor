#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "TGraph.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TDatime.h"
#include "TLegend.h"

int main(){

  std::ifstream file("darkRate.dat");
  std::string line;

  std::vector<double> time;
  std::vector<double> rate, rate_100;

  while(getline(file, line)){
    std::stringstream line_stream(line);
    std::string entry;
    std::vector<std::string> entries;
    char delim = ' ';
    
    while(getline(line_stream, entry, delim)){
      entries.push_back(entry);
    }
    time.push_back(std::stod(entries.at(1)));
    rate.push_back(std::stod(entries.at(2)));
    rate_100.push_back(std::stod(entries.at(3)));
  }

  
  TCanvas c1("c1", "", 850, 600);
  c1.cd();
  c1.SetGrid();
  
  TGraph *dark_all = new TGraph(time.size(), &time[0], &rate[0]);
  TGraph *dark_100 = new TGraph(time.size(), &time[0], &rate_100[0]);

  dark_all->SetTitle("Since Gd;Date;Avg. Dark Noise Rate [kHz]");
  dark_all->SetMarkerStyle(20);
  dark_all->SetMarkerColor(kRed);
  dark_all->SetMarkerSize(1.);

  dark_all->GetXaxis()->SetTimeDisplay(1);
  dark_all->GetXaxis()->SetTimeFormat("%y-%m-%d %F 1970-01-01 00:00:00");
  dark_all->GetXaxis()->SetNdivisions(8,kTRUE);
  dark_all->GetXaxis()->CenterTitle(kTRUE);
  dark_all->SetMinimum(5.5);
  dark_all->SetMaximum(8.5);

  dark_all->Draw("AP");
  c1.Update();
  c1.SaveAs("darkrates_all_100cut_full.png");


  dark_100->SetTitle("Since Gd;Date;Avg. Dark Noise Rate [kHz]");
  dark_100->SetMarkerStyle(20);
  dark_100->SetMarkerColor(kBlue);
  dark_100->SetMarkerSize(1.);
  dark_100->GetXaxis()->SetTimeDisplay(1);
  dark_100->GetXaxis()->SetTimeFormat("%y-%m-%d %F 1970-01-01 00:00:00");
  dark_100->GetXaxis()->SetNdivisions(8,kTRUE);
  dark_100->GetXaxis()->CenterTitle(kTRUE);
  dark_100->SetMinimum(5.5);
  dark_100->SetMaximum(8.5);
  
  dark_100->Draw("AP");
  TLegend *leg1 = new TLegend(0.1, 0.75, 0.45, 0.9);
  leg1->AddEntry(dark_100, "UKLI events - 100 kHz Cut", "p");
  leg1->Draw();
  c1.Update();
  c1.SaveAs("darkrates_100_100cut_full.png");


  dark_all->Draw("AP");
  dark_100->Draw("psame");

  TLegend *leg = new TLegend(0.1, 0.75, 0.45, 0.9);
  leg->AddEntry(dark_all, "All PMTs", "p");
  leg->AddEntry(dark_100, "100 kHz Cut", "p");
  leg->Draw();
  
  c1.Update();
  c1.SaveAs("darkrates_stacked_100cut_full.png");

  //restricted range since new top diff laser
  dark_100->SetTitle("Since New Top Diff Source");
  dark_100->GetXaxis()->SetLimits(1648504800, time.back()+604800*3);
  dark_100->SetMaximum(8.5);
  dark_100->SetMinimum(6.5);
  dark_100->Draw("AP");
  leg1->SetX1NDC(0.55);
  leg1->SetX2NDC(0.9);
  leg1->SetY1NDC(0.75);
  leg1->SetY2NDC(0.9);
  leg1->Draw();
  c1.Update();
  c1.Print("darkrates_100_100cut_newLaser.png");
	   
}
