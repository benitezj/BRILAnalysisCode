#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"

float minratio=0.97;
float maxratio=1.03;
float plotYrangeMin=0.01;
float plotYrange=0;
float plotYrangeLog=1e5;
int plotXrange=5000; //total number of LS's
float plotXrangeSBIL=12;

/* int SelectedRunMin=360458; */
/* int SelectedRunMinLSMin=1; */
/* int SelectedRunMax=360460; */
/* int SelectedRunMaxLSMax=2000; */
/* int nColliding=2450;*/

int SelectedRunMin=360428;
int SelectedRunMinLSMin=1;
int SelectedRunMax=360491;
int SelectedRunMaxLSMax=2000;
int nColliding=2450;



void plotPCCLinearity(TString inpath){
  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  
  TString REF="DT";
  if(REF.CompareTo("")!=0)RefLumi=REF;

  ifstream myfile((inpath+"/ls.dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
  }
  cout<<"opened "<<(inpath+"/ls.dat")<<std::endl;


  ///create histogram
  TH1F HistoLumiRatio("HistoLumiRatio","",200,minratio,maxratio);
  TH1F LumiRatio("HLumiRatio","",plotXrange,0,plotXrange);
  TH1F Lumi("HLumi","",plotXrange,0,plotXrange);
  TH2F H2LumiRatio("H2LumiRatio","",plotXrange/50,0,plotXrange,100,minratio,maxratio);//ratio vs time
  TH1F LumiRef("HLumiRef","",plotXrange,0,plotXrange);
  TH2F H2LumiRatioVsLumi("H2LumiRatioVsLumi","",100,0,plotXrangeSBIL,100,minratio,maxratio);//ratio vs sbil
  int counterLumi=0;
  
  std::map<int,int> runlist;
  int firstrun=1000000000;
  int lastrun=0;

  std::string line;
  int run=0, ls=0;
  double totL=0, totLRef=0;

  ////////////////////////////////////////////////////////
  //make a first pass to determine first and last LS values of all selected runs
  int SelectedRunMinLSMax=0;
  int SelectedRunMaxLSMin=10000000;
  std::list<int> lslist;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);
    iss>>run>>ls>>totL>>totLRef;
    if(! ( (run==SelectedRunMin && ls>SelectedRunMinLSMin)
	   || (run==SelectedRunMax && ls<SelectedRunMaxLSMax)
	   || (run>SelectedRunMin && run<SelectedRunMax) ) ) continue;
    //cout<<"selected :"<<run<<" "<<ls<<endl;
    lslist.push_back(run*2000+ls);
  }
  myfile.clear();
  myfile.seekg(0);
  cout<<"Total number of selected LS's : "<<lslist.size()<<endl;

  lslist.sort();//time order the LS's

  std::map<int,int> lsmap; //map containing the time ordered indexes of each LS
  int lslistindex=1;
  for(std::list<int>::iterator it=lslist.begin();it!=lslist.end();it++){
    lsmap[*it]=lslistindex;
    lslistindex++;
  }


  
  ///Now read again the data and fill plots with time ordering
  while (std::getline(myfile, line)){
    //cout<<line;
    std::stringstream iss(line);
    iss>>run>>ls>>totL>>totLRef;
    

    if(! ( (run==SelectedRunMin && ls>SelectedRunMinLSMin)
	   || (run==SelectedRunMax && ls<SelectedRunMaxLSMax)
	   || (run>SelectedRunMin && run<SelectedRunMax) ) ) continue;
    
    counterLumi++;

    ///lumi profile
    Lumi.SetBinContent(lsmap[run*2000+ls],totL);
    LumiRef.SetBinContent(lsmap[run*2000+ls],totLRef);

    if(totL>plotYrange) plotYrange=totL;

    
    ///ratio plots
    float ratio=0;
    if(totLRef>0 ) ratio=totL/totLRef;

    HistoLumiRatio.Fill(ratio);
    LumiRatio.SetBinContent(lsmap[run*2000+ls],ratio);
    H2LumiRatio.Fill(lsmap[run*2000+ls],ratio);
  
    if(totL>7000)
      H2LumiRatioVsLumi.Fill(totL/nColliding,ratio);

    //find run range
    if(run>lastrun){
      runlist[counterLumi]=run;
      lastrun=run;
    }
    if(run<firstrun){
       firstrun=run;
    }

  }
    
  myfile.close();




  
  TCanvas C("C","",1000,600);
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1);


  /////////////////////////
  ///Lumi vs time
  /////////////////////////////
  TLegend leg(0.75,0.9,0.95,0.99);
  leg.SetLineWidth(0);
  leg.SetFillStyle(0);
  leg.SetNColumns(2);

  C.Clear();
  Lumi.SetTitle(TString("Runs: ")+firstrun+" - "+lastrun);
  Lumi.SetStats(0);
  Lumi.SetMarkerStyle(8);
  Lumi.SetMarkerSize(0.3);
  Lumi.GetXaxis()->SetTitle("lumi section");
  Lumi.GetYaxis()->SetTitle(" Inst. Lumi [Hz/ub]");
  Lumi.GetYaxis()->SetRangeUser(plotYrangeMin,1.1*plotYrange);
  Lumi.Draw("histp");
  leg.AddEntry(&Lumi,"PCC","p");

  LumiRef.SetMarkerStyle(8);
  LumiRef.SetMarkerSize(0.3);
  LumiRef.SetMarkerColor(4);
  LumiRef.SetLineColor(4);
  LumiRef.Draw("histpsame");
  TLegendEntry*  legLumiRef =   leg.AddEntry(&LumiRef,RefLumi,"p");
  legLumiRef->SetTextColor(4);
  
  leg.Draw();
  C.Print(inpath+"/ls_lumi.png");


  /////////////////////////
  ///Lumi ratio vs time 
  /////////////////////////////
  C.Clear();
  LumiRatio.SetTitle(TString("Runs: ")+firstrun+" - "+lastrun);
  LumiRatio.SetStats(0);
  LumiRatio.GetYaxis()->SetRangeUser(minratio,maxratio);
  LumiRatio.SetMarkerStyle(8);
  LumiRatio.SetMarkerSize(0.4);
  LumiRatio.GetXaxis()->SetTitle("lumi section");
  LumiRatio.GetYaxis()->SetTitle(TString("PCC / ")+RefLumi);
  LumiRatio.SetMarkerColor(2);
  LumiRatio.Draw("histp");
  TLine li;
  li.SetLineWidth(2);
  li.SetLineColor(1);
  li.SetLineStyle(1);
  li.DrawLine(0,1,plotXrange,1);//y=1 line
  C.Print(inpath+"/ls_ratio.png");


  TProfile* H2LumiRatioProfX=H2LumiRatio.ProfileX();
  H2LumiRatioProfX->SetTitle(TString("Runs: ")+firstrun+" - "+lastrun);
  H2LumiRatioProfX->GetYaxis()->SetTitle(TString("PCC / ")+RefLumi + " per 50 LS");
  H2LumiRatioProfX->GetXaxis()->SetTitle("lumi section");
  H2LumiRatioProfX->GetYaxis()->SetRangeUser(minratio,maxratio);
  H2LumiRatioProfX->GetYaxis()->SetNdivisions(8);
  H2LumiRatioProfX->GetXaxis()->SetNdivisions(8);
  H2LumiRatioProfX->SetStats(0);
  C.Clear();
  H2LumiRatioProfX->Draw("histpe");
  li.DrawLine(0,1,plotXrange,1);//y=1 line
  C.Print(inpath+"/lumiratio_vs_ls_profile.png");


  
  /////////////////////////
  ///Lumi ratio vs inst lumi,  2D histo
  /////////////////////////////
  H2LumiRatioVsLumi.SetTitle(TString("Runs: ")+firstrun+" - "+lastrun);
  H2LumiRatioVsLumi.SetStats(0);
  H2LumiRatioVsLumi.GetYaxis()->SetTitle(TString("PCC / ")+RefLumi);
  H2LumiRatioVsLumi.GetXaxis()->SetTitle("SBIL [Hz/ub]");
  C.Clear();
  H2LumiRatioVsLumi.Draw("colz");
  li.DrawLine(0,1,plotXrangeSBIL,1);//y=1 line
  C.Print(inpath+"/lumiratio_vs_instlumi.png");

  /////////////////////////
  ///Lumi ratio vs inst lumi, profile
  /////////////////////////////
  TProfile* H2LumiRatioVsLumiProfX=H2LumiRatioVsLumi.ProfileX();
  H2LumiRatioVsLumiProfX->SetTitle(TString("Runs: ")+firstrun+" - "+lastrun);
  H2LumiRatioVsLumiProfX->GetYaxis()->SetTitle(TString("PCC / ")+RefLumi);
  H2LumiRatioVsLumiProfX->GetXaxis()->SetTitle("SBIL [Hz/ub]");
  H2LumiRatioVsLumiProfX->GetYaxis()->SetRangeUser(minratio,maxratio);
  H2LumiRatioVsLumiProfX->GetYaxis()->SetNdivisions(5);
  H2LumiRatioVsLumiProfX->GetXaxis()->SetNdivisions(5);
  H2LumiRatioVsLumiProfX->SetStats(0);
  C.Clear();
  H2LumiRatioVsLumiProfX->Draw("histpe");
  li.DrawLine(0,1,plotXrangeSBIL,1);//y=1 line
  C.Print(inpath+"/lumiratio_vs_instlumi_profile.png");

  

  gROOT->ProcessLine(".q");
}

