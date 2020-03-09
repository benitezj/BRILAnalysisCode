#include <iostream>
#include <fstream>
#include <string>

#include "BRILAnalysisCode/PCCAnalysis/plots/plotPCCcsv.C"

void plotModuleFrac(TString Path, TString Runs, TString outpath="."){

  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");

float ratiomin=0.95;
float ratiomax=1.0;
short nmodmax=770;
short nmodmin=700;


  TGraph Frac;
  TGraph NMod;

  long run1=0;
  long run2=0;


  std::string line;
  int run=0;
  int ls=0;
  double frac=0.;
  unsigned nmod=0;
  unsigned cntr=0;


  TObjArray* RunList=Runs.Tokenize(",");
  cout<<RunList->GetEntries()<<endl;
  for(unsigned r=0;r<RunList->GetEntries();r++){
    long Run=atoi(((TObjString*)RunList->At(r))->GetString());    
    //cout<<Run<<endl;
    
    if(run1==0) run1=Run; // keep the first one
    run2=Run;//will keep the last one

    getRefLumi(Path+"/"+Run+".hfoc");    
    
    ///Open the lumi csv file
    TString infile=Path+"/"+Run+"_frac.csv";
    ifstream myfile (infile.Data());
    if (!myfile.is_open()){
      cout << "Unable to open file: "<<infile.Data()<<endl; 
      return;
    }  
    while (std::getline(myfile, line)){
      //cout<<line;
      
      std::stringstream iss(line);
      std::string token;
      
      ///read the run number
      std::getline(iss,token, ',');
      std::stringstream runiss(token);
      runiss>>run;
      
      ///read the LS
      std::getline(iss,token, ',');
      std::stringstream lsiss(token);
      lsiss>>ls;
      
      if(refLumi[ls]<1) continue;

      
      ///read the fraction
      std::getline(iss,token, ',');
      std::stringstream fraciss(token);
      fraciss>>frac;
      
      ///read the number of modules
      std::getline(iss,token, ',');
      std::stringstream nmodiss(token);
      nmodiss>>nmod;
      
      Frac.SetPoint(cntr,cntr,frac);
      NMod.SetPoint(cntr,cntr,nmod);
      cntr++;
      //std:cout<<Run<<" "<<ls<<" "<<frac<<" "<<nmod<<std::endl;
    }
    
    ///close files
    myfile.close();
  }
  
  /////////////////////////////////////////////////////
  ///   make the plots
  ///////////////////////////////////////////////////
  gStyle->SetOptStat(0);
  TString title=TString("run: ")+(long)run1+" - "+(long)run2;

  TCanvas C("C","",1500,500);
  C.Clear();
  NMod.SetTitle(title);
  NMod.GetYaxis()->SetTitleOffset(-0.);
  NMod.SetMarkerSize(0.5);
  NMod.GetYaxis()->SetRangeUser(nmodmin,nmodmax);
  NMod.GetYaxis()->SetTitle("Number of good modules");
  NMod.GetXaxis()->SetTitle("lumi section");
  NMod.Draw("al");
  C.Print(outpath+"/plotModuleFrac_nmod.png");

  C.Clear();
  Frac.SetTitle(title);
  Frac.GetYaxis()->SetTitleOffset(-0.);
  Frac.SetMarkerSize(0.5);
  Frac.GetYaxis()->SetRangeUser(ratiomin,ratiomax);
  Frac.GetYaxis()->SetTitle("visible crossection fraction"); 
  Frac.GetXaxis()->SetTitle("lumi section");
  Frac.Draw("al");
  C.Print(outpath+"/plotModuleFrac_frac.png");

  gROOT->ProcessLine(".q");
}

