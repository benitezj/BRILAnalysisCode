#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"

float minL=0;
float plotYrangeMin=0.01;
float plotYrangeMax;//code will find max lumi below
float plotYrangeMaxLog=10;//log plot
float plotYrangeZoomMin=0.0;
float plotYrangeZoomMax=0.2;//zoomed plot

///background model
float bkgSS1=0.04;
float timeSS1=550;
float bkgSS2=0.08;
float timeSS2=2340;


void plotPCCvdMlumi(TString infile, int plotXrangeMin=0, int plotXrangeMax=100){

  ifstream myfile((infile).Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
  }
  cout<<"opened "<<(infile)<<std::endl;


  TH1F Lumi("HLumi","",plotXrangeMax-plotXrangeMin,plotXrangeMin,plotXrangeMax);
  TH1F HBkg("HBkg","",plotXrangeMax-plotXrangeMin,plotXrangeMin,plotXrangeMax);
  int counterLumi=0;


  TF1 FBkg("FBkg","[0]+[1]*x",plotXrangeMin,plotXrangeMax);
  FBkg.SetParameter(0,bkgSS1);
  FBkg.SetParameter(1,(bkgSS2-bkgSS1)/(timeSS2-timeSS1));
  
  
  std::map<int,int> runlist;
  int firstrun=1000000000;
  int lastrun=0;
  int counter_ls_run=0;
  int counter_ls_run_tot=0;
  int prevrun=0;

  
  std::string line;
  int run=0;
  int ls=0;
  double totL=0;   //lumi for given LS
  double lumiBCID[5];   //lumi for bcids
  while (std::getline(myfile, line)){

    std::stringstream iss(line);
    std::string token;//comma separated
    std::getline(iss,token, ',');run=atoi(token.c_str());
    std::getline(iss,token, ',');ls=atoi(token.c_str());
    std::getline(iss,token, ',');totL=strtof(token.c_str(),NULL);
    std::getline(iss,token, ',');lumiBCID[0]=strtof(token.c_str(),NULL);
    std::getline(iss,token, ',');lumiBCID[1]=strtof(token.c_str(),NULL);
    std::getline(iss,token, ',');lumiBCID[2]=strtof(token.c_str(),NULL);
    std::getline(iss,token, ',');lumiBCID[3]=strtof(token.c_str(),NULL);
    std::getline(iss,token, ',');lumiBCID[4]=strtof(token.c_str(),NULL);

    totL=(lumiBCID[0]+lumiBCID[1]+lumiBCID[2]+lumiBCID[3]+lumiBCID[4]);

    ///Run and LS counters
    if(run!=prevrun){
      counter_ls_run_tot+=counter_ls_run;
      counter_ls_run=0;
      prevrun=run;
    }
    counterLumi = counter_ls_run_tot + ls;

    if(run>lastrun){
      runlist[counterLumi]=run;
      lastrun=run;
    }
    if(run<firstrun){
       firstrun=run;
    }

    counter_ls_run++;

    ///Lumi cut
    if(totL<minL) continue;
    if(totL>plotYrangeMax) plotYrangeMax=totL;
    

    float totL_bkgSS1=totL-bkgSS1;
    float totL_bkgSS2=totL-bkgSS2;
    float totL_bkgAvg=totL-(bkgSS1+bkgSS2)/2;
    float totL_bkgInterpol=totL-FBkg.Eval(counterLumi);
    //cout<<run<<" "<<ls<<" "<<totL<<endl;
    
    ///lumi vs time plot   
    Lumi.SetBinContent(counterLumi-plotXrangeMin,totL);
    HBkg.SetBinContent(counterLumi-plotXrangeMin,FBkg.Eval(counterLumi));
    
  }
  cout<<"TotalLS: "<<counterLumi<<"  MaxLumi: "<<plotYrangeMax<<" FirstRun:"<<firstrun<<" LastRun:"<<lastrun<<endl;
  myfile.close();


  /////////////////////////////
  TCanvas C("C","",1000,600);
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1);
  C.Clear();
  Lumi.SetTitle(TString("Runs: ")+firstrun+" - "+lastrun);
  Lumi.SetStats(0);
  Lumi.SetMarkerStyle(8);
  Lumi.SetMarkerSize(0.3);
  Lumi.GetXaxis()->SetTitle("lumi section");
  Lumi.GetYaxis()->SetTitle(" rate ");
  Lumi.GetYaxis()->SetRangeUser(plotYrangeMin,1.5*plotYrangeMax);
  Lumi.Draw("histp");
  HBkg.SetMarkerColor(2);
  HBkg.Draw("histpsame");
  C.Print("ls_lumi.png");

  C.SetLogy(1);
  Lumi.GetYaxis()->SetRangeUser(plotYrangeMin,plotYrangeMaxLog*plotYrangeMax);
  C.Update();
  C.Print("ls_lumi_log.png");
  C.SetLogy(0);


  ///fit the rate in a certain time interval
  TF1 fitF("bkg","[0]",plotXrangeMin,plotXrangeMax);
  fitF.SetLineColor(2);
  Lumi.GetYaxis()->SetRangeUser(plotYrangeZoomMin,plotYrangeZoomMax);
  Lumi.Fit(&fitF,"","",plotXrangeMin,plotXrangeMax);
  fitF.Draw("lsame");
  C.Update();
  C.Print("ls_lumi_zoom.png");


  

  gROOT->ProcessLine(".q");
}

