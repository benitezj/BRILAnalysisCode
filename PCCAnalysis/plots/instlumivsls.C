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
#include <set>

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


//void instlumivsls(TString inputFile = "/afs/cern.ch/user/a/asehrawa/reprocessing_PCC_ZB_data_27May2023/CMSSW_10_6_30/src/PCC_newafterglowparameters_27May2023/Run2018_27May2023/ls.dat" ) {

//void instlumivsls(TString inputFile = "/afs/cern.ch/user/a/asehrawa/reprocessing_PCC_ZB_data_27May2023/CMSSW_10_6_30/src/ls_latest.dat" ) {

  void instlumivsls(TString inputFile = "/afs/cern.ch/user/a/asehrawa/reprocessing_PCC_ZB_data_27May2023/CMSSW_10_6_30/src/ls_JB.dat" ) {


  std::set<int> excludedRuns = { 359280,359281,359283,359284,359285,359286,359287,359288,359291,359293,359294,359297,
                                 361881,361882,361883,361884,361885,361886,361887,361889,361890,361893,
                                 361906,361909,361910,361912,361913,361915,361916,361917,361919,361921,361922,361923,
                                 361925,361926,361927,361929,361932,361933, 357842, 359268, 359270, 359278, 359279, 
                                 359282,359289,359296,359585,359813,359869,359901,359904,359905,359998,360121,
                                 360129,360430,360433,361198,361992,361993,362656,362726};

  TString path = "/eos/user/a/asehrawa/";

  if (!fillLastLSmap(inputFile)) return;

  TGraph PCCGraph, HFOCGraph;

  std::ifstream infile(inputFile.Data());
  if (!infile.is_open()) {
    std::cout << "Unable to open file: " << inputFile.Data() << std::endl;
    return;
  }

  float totalPCC = 0., totalHFOC = 0.;
  float totalPCCFiltered = 0., totalHFOCFiltered = 0.;
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

    //totalPCC += pcc;
    //totalHFOC += hfoc;

    if (excludedRuns.find(run) != excludedRuns.end()) {
     continue; 
    }
    
    totalPCCFiltered += pcc * timePerLumiSection;
    totalHFOCFiltered += hfoc * timePerLumiSection;

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
                     
  std::cout << "Filtered Integrated PCC Lumi [Hz/ub]: " << totalPCCFiltered << std::endl;
  std::cout << "Filtered Integrated HFOC Lumi [Hz/ub]: " << totalHFOCFiltered << std::endl;
                                                                                                      
  float totalPCC_fb = totalPCC * 1e-9;
  float totalHFOC_fb = totalHFOC * 1e-9;

  float totalPCCFiltered_fb = totalPCCFiltered * 1e-9;
  float totalHFOCFiltered_fb = totalHFOCFiltered * 1e-9;

  std::cout << "Integrated PCC Lumi: " << totalPCC_fb << " fb^-1" << std::endl;
  std::cout << "Integrated HFOC Lumi: " << totalHFOC_fb << " fb^-1" << std::endl;

  std::cout << "Filtered Integrated PCC Lumi: " << totalPCCFiltered_fb << " fb^-1" << std::endl;
  std::cout << "Filtered Integrated HFOC Lumi: " << totalHFOCFiltered_fb << " fb^-1" << std::endl;

  TCanvas C;

  C.Clear();
  C.SetLogy(1);
  PCCGraph.GetYaxis()->SetTitle("PCC Lumi [Hz/ub]");
  PCCGraph.GetXaxis()->SetTitle("Lumi Section");
  PCCGraph.GetYaxis()->SetRangeUser(0.1, 100000);
  PCCGraph.Draw("AP");
  C.Print(path + "PCC_vs_LumiSection_2022.png");

  C.Clear();
  C.SetLogy(1);
  HFOCGraph.GetYaxis()->SetTitle("HFOC Lumi [Hz/ub]");
  HFOCGraph.GetXaxis()->SetTitle("Lumi Section");
  HFOCGraph.GetYaxis()->SetRangeUser(0.1, 100000);
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
