#include <iostream>
#include <fstream>
#include <string>
//#include "globals.h"
//#include "rootlogon.C"

void plotEmmitScanEff(TString DetName="HFET"){
  //rootlogon();
  
  ifstream myfile ((DetName+".dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<endl; 
    return;
  }

  TGraphErrors G;

  std::string line;
  float Lint;
  char tmpstr[100];
  float tmp;
  float eff;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);

    ///ling format: 324293 7270 2347 2347 10/09/18 14 04 56 STABLE BEAMS 6500 89792.454689641 66744.437627449 10.8 HFOC
    //iss>>run>>FILL>>ls>>ls>>date>>h>>m>>s>>tmp>>tmp>>tmp>>L;

    //41.536  HFET    1.025042196     1.033048942
    iss>>Lint>>tmpstr>>tmp>>eff;
    cout<<Lint<<" "<<tmpstr<<" "<<eff<<endl;
    
    G.SetPoint(G.GetN(),Lint,eff);
  }
   

  ///close files
  myfile.close();
  
  /////////////////////////////////////////////////////
  ///   make the plot
  ///////////////////////////////////////////////////
  TCanvas C("C","",800,600);

  G.GetXaxis()->SetTitle("");
  G.GetXaxis()->SetLabelSize(0.04);
  G.GetXaxis()->SetTitleSize(0.05);
  G.GetXaxis()->SetTitleOffset(0.9);
  G.GetXaxis()->SetNdivisions(4);
  G.GetYaxis()->SetTitle("");
  G.GetYaxis()->SetLabelSize(0.04);
  G.GetYaxis()->SetTitleSize(0.05);
  G.GetYaxis()->SetTitleOffset(1.0);
  //G.GetYaxis()->SetRangeUser(0,2);
  G.SetMarkerStyle(8);
  G.SetMarkerSize(0.5);
  G.SetMarkerColor(4);


  G.Draw("ape");
  //drawLumiTitle(FILL);
  //drawCMSPrelim(year,0.55);
 
  C.Print(DetName+"_EmmittScanEff.png");
  //C.Print(DetName+"_EmmittScanEff.pdf");

  gROOT->ProcessLine(".q");
}

