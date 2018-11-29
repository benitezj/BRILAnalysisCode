#include <iostream>
#include <fstream>
#include <string>

void plotPCCStability(TString inpath, TString ref){

  ifstream myfile((inpath+"/ls.dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
  }
  cout<<"opened "<<(inpath+"/ls.dat")<<std::endl;


  ///create histogram
  TH1F HLumiLS("HLumiLS","",100,0.9,1.1);

  TGraph LumiLS;
  int counter=0;

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
      LumiLS.SetPoint(counter++,counter,ratio);
    }

    HLumiLS.Fill(ratio);
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

  HLumiLS.Fit("gaus","","same",0.98,1.01);
  //HLumiLS.Draw("hist");
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

