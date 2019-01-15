#include <iostream>
#include <fstream>
#include <string>

//#define scale 1.0    ///Ratios w.r.t. hfoc offline
// #define scale 9.97891e-01  // 2018Vdm Veto
// #define scale 0.5443       // CondDB Veto Dec 17
// #define scale 0.3243*0.965*1.005 //SAM Hi test
// #define scale 0.3429*0.965*0.9968 //SAM Lo test
//#define scale 0.546*0.975*0.9973  //Sam Tight cut
#define scale 0.546*0.9863 //Sam tight cut relative to pltzero

void plotPCCStability(TString inpath, TString ref){

  ifstream myfile((inpath+"/ls.dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
  }
  cout<<"opened "<<(inpath+"/ls.dat")<<std::endl;


  ///create histogram
  TH1F HLumiLS("HLumiLS","",100,0.95,1.05);
  //TH1F HLumiLS("HLumiLS","",100,0.9,1.1);
  //TH1F HLumiLS("HLumiLS","",100,0.4,0.8);

  TGraph LumiLS;
  int counter=0;

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
      counter++;

      if(counter<17000 || counter>30000) continue;

      LumiLS.SetPoint(counter,counter,ratio);
      HLumiLS.Fill(ratio);

      //if(ratio>1.008)cout<<run<<" "<<ls<<" "<<ratio<<endl;
    }
  }
    
  myfile.close();



  TCanvas C;


  C.Clear();
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1);

  HLumiLS.GetYaxis()->SetTitle(" # of lumi sections ");
  HLumiLS.GetXaxis()->SetTitle(TString(" PCC / ")+ref);
  HLumiLS.SetMarkerStyle(8);
  HLumiLS.SetMarkerSize(0.5);
  //HLumiLS.Draw("hist");
  HLumiLS.Fit("gaus","","same",0.98,1.02);
  C.Print(inpath+"/ls_ratio_histo.png");


  C.Clear();
  LumiLS.GetYaxis()->SetRangeUser(0.9,1.1);
  LumiLS.SetMarkerStyle(8);
  LumiLS.SetMarkerSize(0.5);
  LumiLS.GetXaxis()->SetTitle("lumi section");
  LumiLS.GetYaxis()->SetTitle(TString(" PCC / ")+ref);
  LumiLS.Draw("ap");
  C.Print(inpath+"/ls_ratio.png");



  gROOT->ProcessLine(".q");
}

