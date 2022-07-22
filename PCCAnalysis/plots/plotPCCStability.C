#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"

float minratio=0.7;
float maxratio=1.3;
float plotYrangeMin=0.1;
float plotYrange=2500;
float plotYrangeLog=1e4;


void plotPCCStability(TString inpath, int plotXrange=100){

  //gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  ifstream myfile((inpath+"/ls.dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
  }
  cout<<"opened "<<(inpath+"/ls.dat")<<std::endl;


  ///create histogram
  TH1F HistoLumiRatio("HistoLumiRatio","",500,minratio,maxratio);
  TH1F LumiRatio("HLumiRatio","",plotXrange,0,plotXrange);
  TH1F Lumi("HLumi","",plotXrange,0,plotXrange);
  TH1F LumiRef("HLumiRef","",plotXrange,0,plotXrange);
  int counterLumi=0;
  
  std::map<int,int> runlist;
  int lastrun=0;

  std::string line;
  int run=0;
  int ls=0;
  double totL=0;   //lumi for given LS
  double totLRef=0;//lumi for given LS
  while (std::getline(myfile, line)){
    //cout<<line;
    std::stringstream iss(line);
    iss>>run>>ls>>totL>>totLRef;

    counterLumi++;
    Lumi.SetBinContent(counterLumi,totL);


    float ratio=0;
    if(totLRef>0 ){
      ratio=totL/totLRef;
      LumiRef.SetBinContent(counterLumi,totLRef);
      HistoLumiRatio.Fill(ratio);
      LumiRatio.SetBinContent(counterLumi,ratio);
    }

    if(run>lastrun){
      runlist[counterLumi]=run;
      lastrun=run;
    }

  }
    
  myfile.close();


  TCanvas C("C","",1000,600);
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1);



  /////////////////////////////
  TLegend leg(0.75,0.8,0.90,0.90);
  
  C.Clear();
  Lumi.SetStats(0);
  Lumi.SetMarkerStyle(8);
  Lumi.SetMarkerSize(0.3);
  Lumi.GetXaxis()->SetTitle("lumi section");
  Lumi.GetYaxis()->SetTitle(" integrated lumi [#mub^{-1}]");
  Lumi.GetYaxis()->SetRangeUser(plotYrangeMin,plotYrange);
  Lumi.Draw("histp");
  leg.AddEntry(&Lumi,"PCC","p");

  TLine li;
  li.SetLineStyle(2);
  TLatex ltxt;
  ltxt.SetTextAngle(90);
  ltxt.SetTextSize(0.03);
  TString rtxt("");
  for ( std::map<int,int>::iterator it = runlist.begin(); it != runlist.end(); it++){
    li.DrawLine(it->first,plotYrangeMin,it->first,plotYrange);
    ltxt.DrawLatex(it->first,plotYrangeMin,rtxt+it->second);
  }


  if(RefLumi.CompareTo("")!=0){
    LumiRef.SetMarkerStyle(8);
    LumiRef.SetMarkerSize(0.3);
    LumiRef.SetMarkerColor(4);
    LumiRef.SetLineColor(4);
    LumiRef.Draw("histpsame");
    TLegendEntry*  legLumiRef =   leg.AddEntry(&LumiRef,RefLumi,"p");
    legLumiRef->SetTextColor(4);
  }

  leg.Draw();
  C.Print(inpath+"/ls_lumi.png");

  C.SetLogy(1);
  Lumi.GetYaxis()->SetRangeUser(plotYrangeMin,plotYrangeLog);
  C.Update();
  C.Print(inpath+"/ls_lumi_log.png");
  C.SetLogy(0);




  ///Ratio graphs
  if(RefLumi.CompareTo("")!=0){
        
    C.Clear();
    LumiRatio.SetStats(0);
    LumiRatio.GetYaxis()->SetRangeUser(minratio,maxratio);
    LumiRatio.SetMarkerStyle(8);
    LumiRatio.SetMarkerSize(0.4);
    LumiRatio.GetXaxis()->SetTitle("lumi section");
    //LumiRatio.GetYaxis()->SetTitle(TString("ratio"));
    LumiRatio.GetYaxis()->SetTitle(TString("PCC / ")+RefLumi);
    LumiRatio.SetMarkerColor(2);
    LumiRatio.Draw("histp");
    li.SetLineWidth(2);
    li.SetLineColor(1);
    li.SetLineStyle(1);
    li.DrawLine(0,1,plotXrange,1);//y=1 line
    li.SetLineColor(4);
    li.SetLineStyle(2);
    li.SetLineWidth(3);
    li.DrawLine(0,HistoLumiRatio.GetBinCenter(HistoLumiRatio.GetMaximumBin()),plotXrange,HistoLumiRatio.GetBinCenter(HistoLumiRatio.GetMaximumBin()));
    TLatex Meantxt;
    Meantxt.SetTextSize(0.03);
    Meantxt.SetTextColor(4);
    char meantxt[100];
    sprintf(meantxt,"r=%.3f",HistoLumiRatio.GetBinCenter(HistoLumiRatio.GetMaximumBin()));
    Meantxt.DrawLatex(plotXrange,HistoLumiRatio.GetBinCenter(HistoLumiRatio.GetMaximumBin()),meantxt);
    C.Print(inpath+"/ls_ratio.png");

    C.Clear();
    HistoLumiRatio.GetYaxis()->SetTitle(" # of lumi sections ");
    HistoLumiRatio.GetXaxis()->SetTitle(TString("ratio"));
    //HistoLumiRatio.SetMarkerStyle(8);
    //HistoLumiRatio.SetMarkerSize(0.4);
    //HistoLumiRatio.Draw("hist");
    HistoLumiRatio.Fit("gaus");//,"","same",0.9,1.1);
    C.Print(inpath+"/ls_ratio_histo.png");

  }
  

  gROOT->ProcessLine(".q");
}

