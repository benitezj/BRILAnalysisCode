#include <iostream>
#include <fstream>
#include <string>

float minratio=0.95;
float maxratio=1.05;

///Ratios w.r.t. hfoc offline
//#define scale 1.0        
//#define scale 1.00895  // 2018Vdm Veto
// #define scale 0.3243*0.965*1.005 //SAM Hi test
// #define scale 0.3429*0.965*0.9968 //SAM Lo test
// #define scale 0.546*0.975*0.9973  //Sam Tight cut
// #define scale 0.546*0.9863 //Sam tight cut relative to pltzero
//#define scale 1.024974  // 2018Vdm Veto, with dynamic module veto but uncorrected
//#define scale 1.00892  // 2018Vdm Veto, with dynamic module veto corrected

//#define scale 0.97  //2017G after pedestal subtraction
#define scale 0.8414  //2017G after pedestal subtraction


void plotPCCStability(TString inpath, TString ref){

  // gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");


  ifstream myfile((inpath+"/ls.dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
  }
  cout<<"opened "<<(inpath+"/ls.dat")<<std::endl;


  ///create histogram
  TH1F HLumiRatio("HLumiRatio","",100,minratio,maxratio);

  TGraph LumiRatio;
  int counterRatio=0;

  TGraph Lumi;
  int counterLumi=0;
  

  std::string line;
  int run=0;
  int ls=0;
  double totL=0;   //lumi for given LS
  double totLRef=0;//lumi for given LS
  while (std::getline(myfile, line)){
    //cout<<line;

    std::stringstream iss(line);
    iss>>run>>ls>>totL>>totLRef;
   
    totL/=scale;

    float ratio=0;
    if(totLRef>0 ){
      ratio=totL/totLRef;

      //if(counterRatio<17000 || counterRatio>30000) continue;

      //if(counterRatio>25000) continue;//for PCC/hfoc RunB

      //if( ratio<1.01 || ratio>1.025 ) continue;

      LumiRatio.SetPoint(counterRatio,counterRatio,ratio);
      counterRatio++;
      HLumiRatio.Fill(ratio);

      Lumi.SetPoint(counterLumi,counterLumi,totL);
      counterLumi++;

    }
  }
    
  myfile.close();



  TCanvas C;

  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1);


  C.Clear();
  HLumiRatio.GetYaxis()->SetTitle(" # of lumi sections ");
  HLumiRatio.GetXaxis()->SetTitle(TString(" PCC / ")+ref);
  HLumiRatio.SetMarkerStyle(8);
  HLumiRatio.SetMarkerSize(0.5);
  //HLumiRatio.Draw("hist");
  HLumiRatio.Fit("gaus","","same",0.98,1.02);
  C.Print(inpath+"/ls_ratio_histo.png");


  C.Clear();
  LumiRatio.GetYaxis()->SetRangeUser(minratio,maxratio);
  LumiRatio.SetMarkerStyle(8);
  LumiRatio.SetMarkerSize(0.5);
  LumiRatio.GetXaxis()->SetTitle("lumi section");
  LumiRatio.GetYaxis()->SetTitle(TString(" PCC / ")+ref);
  LumiRatio.Draw("ap");
  C.Print(inpath+"/ls_ratio.png");


  C.Clear();
  Lumi.SetMarkerStyle(8);
  Lumi.SetMarkerSize(0.5);
  Lumi.GetXaxis()->SetTitle("lumi section");
  Lumi.GetYaxis()->SetTitle(" PCC ");
  Lumi.Draw("ap");
  C.Print(inpath+"/ls_lumi.png");



  gROOT->ProcessLine(".q");
}

