#include <iostream>
#include <fstream>
#include <string>
#define NBX 3564
#define NLS 1200
 
void plotPCCcsv(TString Path,long Run,TString outpath="."){

  ifstream myfile ((Path+"/"+Run+".csv").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
  }


  ///create histogram
  TH2F HLumiBXvsLS("HLumiBXvsLS","",NLS,0.5,NLS+0.5,NBX,0.5,NBX+0.5);

  TH1F HLumiLS("HLumiLS","",NLS,0.5,NLS+0.5);


  std::string line;
  int run=0;
  int ls=0;
  double totL=0;
  double bxL[NBX];
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


    ///read the total Lumi
    std::getline(iss,token, ',');
    std::stringstream totLiss(token);
    totLiss>>totL;

    HLumiLS.SetBinContent(ls,totL);
    
    ///read lumi per BX
    for(int bx=0;bx<NBX;bx++)
      bxL[bx]=0.;

    double totbxL=0.;
    for(int bx=0;bx<NBX;bx++){
      std::getline(iss,token, ',');
      std::stringstream bxLiss(token);
      bxLiss>>bxL[bx];
      totbxL+=bxL[bx];

      HLumiBXvsLS.SetBinContent(ls,bx+1,bxL[bx]);

    }

    //std::cout<<run<<" "<<ls<<" "<<totL<<" "<<totbxL<<std::endl;
  }
    
  myfile.close();



  gStyle->SetOptStat(0);




  HLumiBXvsLS.GetXaxis()->SetTitle("lumi section");
  HLumiBXvsLS.GetYaxis()->SetTitle("bunch crossing");
  HLumiBXvsLS.GetZaxis()->SetTitle("PCC Lumi");
  
  HLumiLS.GetXaxis()->SetTitle("lumi section");
  HLumiLS.GetYaxis()->SetTitle("PCC Lumi");


  TCanvas C;
  ///only 2D plot
  //C.Clear();
  //HLumiBXvsLS.Draw("colz");
  
  //2D plot on top and 1D on bottom
  TPad can_1("can_1", "can_1", 0.0, 0.4, 1.0, 1.0);
  can_1.SetTopMargin(0.05);
  can_1.SetBottomMargin(0.02);
  can_1.SetLeftMargin(0.1);
  can_1.SetRightMargin(0.15);
  can_1.SetFrameBorderMode(0);
  can_1.cd();
  HLumiBXvsLS.GetXaxis()->SetNdivisions(0);
  HLumiBXvsLS.GetYaxis()->SetLabelSize(0.05);
  HLumiBXvsLS.GetYaxis()->SetTitleSize(0.07);
  HLumiBXvsLS.GetYaxis()->SetTitleOffset(0.6);
  HLumiBXvsLS.GetZaxis()->SetLabelSize(0.05);
  HLumiBXvsLS.GetZaxis()->SetTitleSize(0.06);
  HLumiBXvsLS.GetZaxis()->SetTitleOffset(0.7);
  HLumiBXvsLS.Draw("colz");

  TPad can_2("can_2", "can_2",0.0, 0.0, 1.0, 0.4);
  can_2.SetTopMargin(0.08);
  can_2.SetBottomMargin(0.15);
  can_2.SetLeftMargin(0.1);
  can_2.SetRightMargin(0.15);
  can_2.SetFrameBorderMode(0);
  can_2.cd();
  HLumiLS.GetYaxis()->SetLabelSize(0.07);
  HLumiLS.GetYaxis()->SetTitleSize(0.08);
  HLumiLS.GetYaxis()->SetTitleOffset(0.6);
  HLumiLS.GetYaxis()->SetRangeUser(0,10e6);
  HLumiLS.GetXaxis()->SetLabelSize(0.07);
  HLumiLS.GetXaxis()->SetTitleSize(0.1);
  HLumiLS.GetXaxis()->SetTitleOffset(0.6);
  HLumiLS.SetMarkerStyle(8);
  HLumiLS.SetMarkerSize(0.5);
  HLumiLS.Draw("histp");
  
  C.Clear();
  can_1.Draw();
  can_2.Draw();

  C.Print(outpath+"/"+(long)Run+".png");

  gROOT->ProcessLine(".q");
}

