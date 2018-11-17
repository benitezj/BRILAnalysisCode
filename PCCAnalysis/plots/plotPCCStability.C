#include <iostream>
#include <fstream>
#include <string>

void plotPCCStability(TString inpath){

  ifstream myfile((inpath+"/ls.dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
  }
  cout<<"opened "<<(inpath+"/ls.dat")<<std::endl;


  ///create histogram
  TH1F HLumiLS("HLumiLS","",100,0.8,1.2);


  std::string line;
  int run=0;
  int ls=0;
  double totL=0;//lumi for given LS
  double totLRef=0;//lumi for given LS
  while (std::getline(myfile, line)){
    //cout<<line;

    std::stringstream iss(line);
    iss>>run>>ls>>totL>>totLRef;

    float ratio=0;
    if(totLRef>0){
      ratio=totL/totLRef;
    }

    HLumiLS.Fill(ratio);
  }
    
  myfile.close();


  gStyle->SetOptStat(111111);
  
  HLumiLS.GetYaxis()->SetTitle(" # of lumi sections ");
  HLumiLS.GetXaxis()->SetTitle(" normtag_BRIL / PCC ");

  TCanvas C;

  //HLumiLS.GetXaxis()->SetRangeUser(0,maxLS+50);
  // HLumiLS.GetXaxis()->SetLabelSize(0.07);
  // HLumiLS.GetXaxis()->SetTitleSize(0.1);
  // HLumiLS.GetXaxis()->SetTitleOffset(0.6);
  // HLumiLS.GetYaxis()->SetLabelSize(0.07);
  // HLumiLS.GetYaxis()->SetTitleSize(0.08);
  // HLumiLS.GetYaxis()->SetTitleOffset(0.6);
  ///HLumiLS.GetYaxis()->SetRangeUser(0,MAXPCC);
  HLumiLS.SetMarkerStyle(8);
  HLumiLS.SetMarkerSize(0.5);
  HLumiLS.Draw("hist");
  
  C.Print(inpath+"/PCCStabilityPerLS.png");

  gROOT->ProcessLine(".q");
}

