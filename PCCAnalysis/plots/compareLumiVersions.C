#include "globals.h"

float getRefLumi(TString file, unsigned int RUN, unsigned int LS){
  
  ifstream reffile(file.Data());
  if (!reffile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<file.Data()<<std::endl;
    return 0.;
  }
  
  std::string line;
  int run=0;
  int ls=0;
  float L=0.;
  while (std::getline(reffile, line)){
    std::stringstream iss(line);
    iss>>run>>ls>>L;
    if(run==RUN&&ls==LS) return L;
  }
  
  return 0.;
}


void compareLumiVersions(TString refDir, TString newDir){
  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  TGraph LumiFrame;
  TGraph LumiRatio;
  TGraph LumiGainedRuns;
  TGraph LumiLostRuns;


  ifstream myfile((newDir+"/ls.dat").Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open runs.dat in "<<newDir.Data()<<std::endl;
    return;
  }

  std::string line;
  int run=0;
  int ls=0;
  int firstrun=9999999;
  int lastrun=0;
  float newLumi = 0.;
  float refLumi = 0.;
  int counter=0;
  int counter2=0;
  while (std::getline(myfile,line)){
    std::stringstream iss(line);
    iss>>run>>ls>>newLumi;
    //cout<<run<<" "<<newLumi<<endl;

    if(run<firstrun) firstrun=ls;
    if(run>lastrun) lastrun=ls;

    refLumi = getRefLumi(refDir+"/ls.dat",run,ls);

    if(refLumi>0.){
      if(newLumi/refLumi>2)
	LumiRatio.SetPoint(counter++,ls,1.1);
      else LumiRatio.SetPoint(counter++,ls,newLumi/refLumi);
    }else if(newLumi>0.) {
      LumiGainedRuns.SetPoint(counter2++,ls,1);
    }

  }
  myfile.close();

  //fix boundaries
  LumiFrame.SetPoint(0,firstrun,0);
  LumiFrame.SetPoint(1,lastrun,0);
  LumiFrame.GetXaxis()->SetTitle("lumi section");//TString("run - ")+RUNOFFSET);
  LumiFrame.GetYaxis()->SetTitle("new lumi / old lumi");

  //now loop over old run list and check if any where lost:
  ifstream reffile((refDir+"/runs.dat").Data());
  if (!reffile.is_open()){
    std::cout << "Unable to open runs in "<<newDir<<std::endl;
    return;
  }
  counter=0;
  while (std::getline(reffile,line)){
    std::stringstream iss(line);
    iss>>run>>refLumi;
    newLumi = getRefLumi(newDir+"/runs.dat",run,ls);

    if(refLumi>0.&&newLumi<=0.){ 
      cout<<"Lost: "<<run<<endl;
      LumiLostRuns.SetPoint(counter++,run,0.9);
    }
  }
  reffile.close();


  gStyle->SetOptStat(0);

  TCanvas C;

  //LumiRatio.GetYaxis()->SetRangeUser(PCCZEROLUMI,MAXPCCRUN);

  //LumiFrame.GetYaxis()->SetRangeUser(0.9,1.1);
  LumiFrame.GetXaxis()->SetRangeUser(firstrun,lastrun);
  LumiFrame.SetMarkerSize(0.1);
  LumiFrame.Draw("ap");


  //LumiRatio.GetYaxis()->SetRangeUser(0,2);
  //LumiRatio.GetXaxis()->SetRangeUser(firstrun,lastrun);
  LumiRatio.SetMarkerStyle(8);
  LumiRatio.SetMarkerSize(0.5);
  LumiRatio.Draw("psame");

  LumiGainedRuns.SetMarkerStyle(8);
  LumiGainedRuns.SetMarkerSize(0.5);
  LumiGainedRuns.SetMarkerColor(3);
  LumiGainedRuns.Draw("psame");

  LumiLostRuns.SetMarkerStyle(8);
  LumiLostRuns.SetMarkerSize(0.5);
  LumiLostRuns.SetMarkerColor(2);
  LumiLostRuns.Draw("psame");
  
  C.Print(newDir+"/comparePCCLumiVersions.png");

  gROOT->ProcessLine(".q");
}
