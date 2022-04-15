#include "globals.h"


void compareCSVfiles(TString path,TString newF, TString refF){
  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");

  bool plotbx=0;

  TGraph LumiNew;
  TGraph LumiRef;
  TGraph LumiRatio;


  ifstream newfile((path+"/"+newF).Data());
  if (!newfile.is_open()){
    std::cout << "Unable to open  "<<newF.Data()<<std::endl;
    return;
  }

  ifstream reffile((path+"/"+refF).Data());
  if (!reffile.is_open()){
    std::cout << "Unable to open "<<refF.Data()<<std::endl;
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
  float maxLumi=0.;
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

    //cout<<newF<<" "<<ls<<" "<<Lumi<<endl;
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

    //cout<<refF<<" "<<refls<<" "<<refLumi<<endl;
    LumiRef.SetPoint(LumiRef.GetN(),LumiRef.GetN(),refLumi);
    
    if(maxLumi<Lumi) maxLumi = Lumi;
    if(maxLumi<refLumi) maxLumi = refLumi;
    
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
	
      }
    }

  }
  reffile.close();
  newfile.close();
  


  gStyle->SetOptStat(0);

  TCanvas C;

  C.Clear();
  LumiNew.GetYaxis()->SetRangeUser(-0.1*maxLumi,1.1*maxLumi);
  LumiNew.SetMarkerStyle(3);
  LumiNew.SetMarkerSize(0.2);
  LumiNew.SetMarkerColor(4);
  LumiNew.GetXaxis()->SetTitle("lumi section");
  LumiNew.GetYaxis()->SetTitle("PCC");
  LumiNew.Draw("ap");

  LumiRef.SetMarkerStyle(3);
  LumiRef.SetMarkerSize(0.2);
  LumiRef.SetMarkerColor(2);
  LumiRef.Draw("psame");
  
  TLatex text;
  text.SetTextSize(0.025);
  text.SetTextColor(4);
  text.DrawLatexNDC(0.05,0.97,newF.Data());
  text.SetTextColor(2);
  text.DrawLatexNDC(0.05,0.94,refF.Data());
  C.Print(path+"/compareCSVfiles.png");
  
  C.Clear();
  LumiRatio.GetYaxis()->SetRangeUser(-0.01,2);
  LumiRatio.GetXaxis()->SetTitle("lumi section");
  LumiRatio.GetYaxis()->SetTitle("ratio");
  LumiRatio.SetMarkerStyle(8);
  LumiRatio.SetMarkerSize(0.5);
  LumiRatio.Draw("ap");
  C.Print(path+"/compareCSVfiles_ratio.png");

  gROOT->ProcessLine(".q");
}
