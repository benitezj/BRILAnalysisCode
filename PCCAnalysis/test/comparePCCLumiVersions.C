#include "globals.h"

float getRefLumi(TString file, unsigned int RUN){
  
  ifstream myfile(file.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<file.Data()<<std::endl;
    return 0.;
  }
  
  std::string line;
  int run=0;
  float L=0.;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);
    iss>>run>>L;
    if(run==RUN) return L;
  }
  
  return 0.;
}


void comparePCCLumiVersions(TString refDir, TString newDir){

  TGraph LumiFrame;
  TGraph LumiRatio;
  TGraph LumiGainedRuns;
  TGraph LumiLostRuns;
  

  ifstream myfile((newDir+"/runs.txt").Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open runs in "<<newDir<<std::endl;
    return;
  }

  std::string line;
  int run=0;
  int firstrun=9999999;
  int lastrun=0;
  float newLumi = 0.;
  float refLumi = 0.;
  int counter=0;
  int counter2=0;
  while (std::getline(myfile,line)){
    std::stringstream iss(line);
    iss>>run>>newLumi;
    //cout<<run<<" "<<newLumi<<endl;

    if(run<firstrun) firstrun=run;
    if(run>lastrun) lastrun=run;

    refLumi = getRefLumi(refDir+"/runs.txt",run);

    if(refLumi>0.){
      if(newLumi/refLumi>2)
	LumiRatio.SetPoint(counter++,run-RUNOFFSET,2);
      else LumiRatio.SetPoint(counter++,run-RUNOFFSET,newLumi/refLumi);
    }else if(newLumi>0.) {
      LumiGainedRuns.SetPoint(counter2++,run-RUNOFFSET,1);
    }

  }
  myfile.close();

  //fix boundaries
  LumiFrame.SetPoint(0,firstrun-RUNOFFSET,0);
  LumiFrame.SetPoint(1,lastrun-RUNOFFSET,0);
  LumiFrame.GetXaxis()->SetTitle(TString("run - ")+RUNOFFSET);
  LumiFrame.GetYaxis()->SetTitle("new lumi / old lumi");

  //now loop over old run list and check if any where lost:
  ifstream reffile((refDir+"/runs.txt").Data());
  if (!reffile.is_open()){
    std::cout << "Unable to open runs in "<<newDir<<std::endl;
    return;
  }
  counter=0;
  while (std::getline(reffile,line)){
    std::stringstream iss(line);
    iss>>run>>refLumi;
    newLumi = getRefLumi(newDir+"/runs.txt",run);

    if(refLumi>0.&&newLumi<=0.){ 
      cout<<"Lost: "<<run<<endl;
      LumiLostRuns.SetPoint(counter++,run-RUNOFFSET,0);
    }
  }
  reffile.close();


  gStyle->SetOptStat(0);

  TCanvas C;

  //LumiRatio.GetYaxis()->SetRangeUser(PCCZEROLUMI,MAXPCCRUN);

  LumiFrame.GetYaxis()->SetRangeUser(0,2);
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
