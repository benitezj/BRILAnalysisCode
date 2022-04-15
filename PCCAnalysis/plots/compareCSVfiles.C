#include "globals.h"


void compareCSVfiles(TString newF, TString refF){
  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  bool plotbx=0;
  

  TGraph LumiNew;
  TGraph LumiRef;
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

    //cout<<newF<<" "<<ls<<" "<<Lumi<<": "<<endl;
    LumiNew.SetPoint(LumiNew.GetN(),LumiNew.GetN(),Lumi);

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

    //cout<<refF<<" "<<refls<<" "<<refLumi<<": "<<endl;
    LumiRef.SetPoint(LumiRef.GetN(),LumiRef.GetN(),refLumi);

    //fill the ratio
    if(run==refrun && ls==refls){
      if(refLumi>0)
	LumiRatio.SetPoint(LumiRatio.GetN(),LumiRatio.GetN(),Lumi/refLumi);
      else if(Lumi==0.)
	LumiRatio.SetPoint(LumiRatio.GetN(),LumiRatio.GetN(),1);
      else LumiRatio.SetPoint(LumiRatio.GetN(),LumiRatio.GetN(),-1);
      
    }


    //per bx comparisons
    if(plotbx){
      for(int b=1;b<=3564;b++){
	std::getline(iss,token, ',');
	std::stringstream biss(token);
	biss>>Lumi;
	
	std::getline(refiss,reftoken, ',');
	std::stringstream refbiss(reftoken);
	refbiss>>refLumi;
	
	if(refLumi>0) 
	  //cout<<b<<":"<<Lumi<<"/"<<refLumi<<"="<<Lumi/refLumi<<endl;
      }
      //cout<<endl;
    }

  }
  reffile.close();
  newfile.close();
  


  gStyle->SetOptStat(0);

  TCanvas C;
  LumiRatio.GetYaxis()->SetRangeUser(0,2);
  LumiRatio.SetMarkerStyle(8);
  LumiRatio.SetMarkerSize(0.5);
  LumiRatio.Draw("ap");
  C.Print("compareCSVfiles_ratio.png");

  gROOT->ProcessLine(".q");
}
