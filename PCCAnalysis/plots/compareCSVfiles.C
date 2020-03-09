#include "globals.h"


void compareCSVfiles(TString newF, TString refF){
  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  TGraph LumiFrame;
  TGraph LumiRatio;


  ifstream newfile((newF).Data());
  if (!newfile.is_open()){
    std::cout << "Unable to open runs.dat in "<<newF.Data()<<std::endl;
    return;
  }

  ifstream reffile((refF).Data());
  if (!reffile.is_open()){
    std::cout << "Unable to open runs.dat in "<<refF.Data()<<std::endl;
    return;
  }

  std::string line;
  std::string refline;
  int run=0;
  int ls=0;
  int refrun=0;
  int refls=0;
  int bx=0;
  int refbx=0;
  float Lumi = 0.;
  float refLumi = 0.;
  int counter=0;
  while (std::getline(newfile,line)){
    std::stringstream iss(line);
    std::string token;

    std::getline(iss,token, ',');
    std::stringstream runiss(token);
    runiss>>run;

    std::getline(iss,token, ',');
    std::stringstream lsiss(token);
    lsiss>>ls;

    std::getline(iss,token, ',');
    std::stringstream lumiiss(token);
    lumiiss>>Lumi;

    cout<<run<<" "<<ls<<" "<<Lumi<<": "<<endl;


    //read the   reference
    std::getline(reffile,refline);
    std::stringstream refiss(refline);
    std::string reftoken;

    std::getline(refiss,reftoken, ',');
    std::stringstream refruniss(reftoken);
    refruniss>>refrun;

    std::getline(refiss,reftoken, ',');
    std::stringstream reflsiss(reftoken);
    reflsiss>>refls;

    std::getline(refiss,reftoken, ',');
    std::stringstream reflumiiss(reftoken);
    reflumiiss>>refLumi;

    cout<<refrun<<" "<<refls<<" "<<refLumi<<": "<<endl;

    for(int b=1;b<=3564;b++){
      std::getline(iss,token, ',');
      std::stringstream biss(token);
      biss>>Lumi;

      std::getline(refiss,reftoken, ',');
      std::stringstream refbiss(reftoken);
      refbiss>>refLumi;

      if(Lumi>0)cout<<b<<":"<<Lumi<<"/"<<refLumi<<"="<<Lumi/refLumi<<endl;
    }
    cout<<endl;

  }
  newfile.close();

  // //fix boundaries
  // LumiFrame.SetPoint(0,firstrun-RUNOFFSET,0);
  // LumiFrame.SetPoint(1,lastrun-RUNOFFSET,0);
  // LumiFrame.GetXaxis()->SetTitle(TString("run - ")+RUNOFFSET);
  // LumiFrame.GetYaxis()->SetTitle("new lumi / old lumi");


  gStyle->SetOptStat(0);

  TCanvas C;

  //LumiRatio.GetYaxis()->SetRangeUser(PCCZEROLUMI,MAXPCCRUN);

  // LumiFrame.GetYaxis()->SetRangeUser(0.9,1.1);
  // LumiFrame.GetXaxis()->SetRangeUser(firstrun,lastrun);
  // LumiFrame.SetMarkerSize(0.1);
  // LumiFrame.Draw("ap");


  //LumiRatio.GetYaxis()->SetRangeUser(0,2);
  //LumiRatio.GetXaxis()->SetRangeUser(firstrun,lastrun);
  LumiRatio.SetMarkerStyle(8);
  LumiRatio.SetMarkerSize(0.5);
  LumiRatio.Draw("ape");

  
  C.Print("compareCSVfiles.png");

  gROOT->ProcessLine(".q");
}
