#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"

void plotLHCFill_LumiVsTime(TString Path,long RUN){
  //provide: path, fill number, fill starting date
 
  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  ///Open the lumi csv file
  ifstream myfile ((Path+"/"+RUN+"_PHYSICS.txt").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<endl; 
    return;
  }

  TGraphErrors G;

  std::string line;
  int run=0;
  int ls=0;
  long FILL=0;
  long year=0,month=0,day=0;
  double L=0;//lumi for given LS
  int h,m,s;
  int hstart=-1;
  int dcounter=-1;
  int counter=0;
  char date[100];
  char tmp[100];
  while (std::getline(myfile, line)){
    std::stringstream iss(line);

    ///ling format: 324293 7270 2347 2347 10/09/18 14 04 56 STABLE BEAMS 6500 89792.454689641 66744.437627449 10.8 HFOC

    iss>>run>>FILL>>ls>>ls>>date>>h>>m>>s>>tmp>>tmp>>tmp>>L;

    if(dcounter==-1||h<hstart){
      hstart=h;
      dcounter++;
      cout<<"update day counter:"<<ls<<" "<<date<<" "<<h<<" -> "<<dcounter<<", t="<<dcounter*24*3600+h*3600+m*60+s<<endl;
    }

    G.SetPoint(counter,dcounter*24*3600+h*3600+m*60+s,L*InstLumiScale);
    counter++;

    if(year==0){///determine the initial date
      std::stringstream dss(date);
      std::string token;
      std::getline(dss,token,'/');
      std::stringstream ss1(token);
      ss1>>month;
      std::getline(dss,token,'/');
      std::stringstream ss2(token);
      ss2>>day;
      std::getline(dss,token,'/');
      std::stringstream ss3(token);
      ss3>>year; year+=2000;
    }

  }
   

  ///close files
  myfile.close();
  
  /////////////////////////////////////////////////////
  ///   make the plot
  ///////////////////////////////////////////////////
  TCanvas C("C","",800,600);

  G.GetXaxis()->SetTitle("Date Time      ");
  G.GetXaxis()->SetLabelSize(0.04);
  G.GetXaxis()->SetTitleSize(0.05);
  G.GetXaxis()->SetTitleOffset(0.9);
  G.GetXaxis()->SetNdivisions(4);
  G.GetYaxis()->SetTitle(InstLumiAxisTitle.Data());
  G.GetYaxis()->SetLabelSize(0.04);
  G.GetYaxis()->SetTitleSize(0.05);
  G.GetYaxis()->SetTitleOffset(1.0);
  G.GetYaxis()->SetRangeUser(0,2);
  G.SetMarkerStyle(8);
  G.SetMarkerSize(0.5);
  G.SetMarkerColor(4);

  cout<<FILL<<" "<<year<<" "<<month<<" "<<day<<endl;
  TDatime dh(year,month,day,00,00,00);
  G.GetXaxis()->SetTimeDisplay(1);
  G.GetXaxis()->SetTimeFormat("%m/%d %H:%M");
  G.GetXaxis()->SetTimeOffset(dh.Convert()); 

  G.Draw("ap");
  drawLumiTitle(FILL);
  drawCMSPrelim(year,0.55);
 
  C.Print(Path+"/LumiVsTime_fill"+FILL+".png");
  C.Print(Path+"/LumiVsTime_fill"+FILL+".pdf");

  gROOT->ProcessLine(".q");
}

