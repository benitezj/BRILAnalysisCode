#include "globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <TString.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TStyle.h>


std::map<int, int> lastLSmap;

bool fillLastLSmap(TString file) {
  std::ifstream reffile(file.Data());
  if (!reffile.is_open()) {
    std::cout << "Unable to open lumi file: " << file.Data() << std::endl;
    return false;
  }

  std::string line;
  int run = 0, ls = 0;
  float pcc = 0., hfoc = 0.;
  while (std::getline(reffile, line)) {
    std::stringstream iss(line);
    iss >> run >> ls >> pcc >> hfoc;
    if (ls > lastLSmap[run]) {
      lastLSmap[run] = ls;
    }
  }
  return true;
}


void instlumivsls(TString inputFile = "/afs/cern.ch/user/a/asehrawa/reprocessing_PCC_ZB_data_27May2023/CMSSW_10_6_30/src/PCC_newafterglowparameters_27May2023/Run2018_27May2023/ls.dat" ) {

  //void instlumivsls(TString inputFile = "/afs/cern.ch/user/a/asehrawa/reprocessing_PCC_ZB_data_27May2023/CMSSW_10_6_30/src/ls_JB.dat" ) {

  TString path = "/eos/user/a/asehrawa/";

  if (!fillLastLSmap(inputFile)) return;

  TGraph PCCGraph, HFOCGraph;

  std::ifstream infile(inputFile.Data());
  if (!infile.is_open()) {
    std::cout << "Unable to open file: " << inputFile.Data() << std::endl;
    return;
  }

  float totalPCC = 0., totalHFOC = 0.;
  const float timePerLumiSection = 23.36;

  std::string line;
  int run = 0, ls = 0;
  float pcc = 0., hfoc = 0.;
  int lastRun = 0, lastLS = 0, counter = 0;
  while (std::getline(infile, line)) {
    std::stringstream iss(line);
    iss >> run >> ls >> pcc >> hfoc;

    totalPCC += pcc * timePerLumiSection;
    totalHFOC += hfoc * timePerLumiSection;

    if (run != lastRun) {
      lastRun = run;
      lastLS += lastLSmap[run];
    }

    int x = (lastLS - lastLSmap[run]) + ls;

    PCCGraph.SetPoint(counter, x, pcc);
    HFOCGraph.SetPoint(counter, x, hfoc);

    counter++;
  }
  infile.close();

  std::cout << "Integrated PCC Lumi [Hz/ub]: " << totalPCC << std::endl;
  std::cout << "Integrated HFOC Lumi [Hz/ub] " << totalHFOC << std::endl;
                                                                                                                           
  float totalPCC_fb = totalPCC * 1e-9;
  float totalHFOC_fb = totalHFOC * 1e-9;

  std::cout << "Integrated PCC Lumi: " << totalPCC_fb << " fb^-1" << std::endl;
  std::cout << "Integrated HFOC Lumi: " << totalHFOC_fb << " fb^-1" << std::endl;

  TCanvas C;

  C.Clear();
  C.SetLogy(1);
  PCCGraph.GetYaxis()->SetTitle("PCC Lumi [Hz/ub]");
  PCCGraph.GetXaxis()->SetTitle("Lumi Section");
  PCCGraph.GetYaxis()->SetRangeUser(0.00001, 100000);
  PCCGraph.Draw("AP");
  C.Print(path + "PCC_vs_LumiSection_2022.png");

  C.Clear();
  C.SetLogy(1);
  HFOCGraph.GetYaxis()->SetTitle("HFOC Lumi [Hz/ub]");
  HFOCGraph.GetXaxis()->SetTitle("Lumi Section");
  HFOCGraph.GetYaxis()->SetRangeUser(0.00001, 100000);
  HFOCGraph.Draw("AP");
  C.Print(path + "HFOC_vs_LumiSection_2022.png");

  gROOT->ProcessLine(".q");
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: ./program <input_file>" << std::endl;
    return 1;
  }
  instlumivsls(argv[1]);
  return 0;
}
