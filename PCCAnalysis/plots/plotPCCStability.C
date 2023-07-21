#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"


float minratio=0.97;
float maxratio=1.03;
float plotYrangeMin=0.01;
float plotYrange=0;
float plotYrangeLog=1e5;


void plotPCCStability(TString inpath, int plotXrange=100, TString REF="HFET",TString runselection=""){

  RefLumi=REF;

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
  TH1F LumiRef("HLumiRef","",plotXrange,0,plotXrange);
  TH2F H2LumiRatioVsLumi("H2LumiRatioVsLumi","",100,0,22e3,100,minratio,maxratio);
  int counterLumi=0;
  
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
  double totLRef=0;//lumi for given LS
  while (std::getline(myfile, line)){
    //cout<<line;
    std::stringstream iss(line);
    iss>>run>>ls>>totL>>totLRef;

    if( !(runselection.Contains(TString("")+run)) ) continue;

    //counterLumi++; //total number of lumi sections

    if(run!=prevrun){
      counter_ls_run_tot+=counter_ls_run;
      counter_ls_run=0;
      prevrun=run;
    }
    counter_ls_run++;
    counterLumi = counter_ls_run_tot + ls;


    Lumi.SetBinContent(counterLumi,totL);
    if(totL>plotYrange) plotYrange=totL;


    float ratio=0;
    if(totLRef>0 ){
      ratio=totL/totLRef;
    }
    LumiRef.SetBinContent(counterLumi,totLRef);
    HistoLumiRatio.Fill(ratio);
    LumiRatio.SetBinContent(counterLumi,ratio);
    if(totL>7000)
      H2LumiRatioVsLumi.Fill(totL,ratio);

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
  Lumi.GetYaxis()->SetTitle(" lumi ");
  Lumi.GetYaxis()->SetRangeUser(plotYrangeMin,1.5*plotYrange);
  Lumi.Draw("histp");
  leg.AddEntry(&Lumi,"pcc","p");

  TLine li;
  li.SetLineStyle(2);
  TLatex ltxt;
  ltxt.SetTextAngle(90);
  ltxt.SetTextSize(0.03);
  TString rtxt("");
  for ( std::map<int,int>::iterator it = runlist.begin(); it != runlist.end(); it++){
    li.DrawLine(it->first,plotYrangeMin,it->first,plotYrange);
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
    LumiRatio.GetYaxis()->SetTitle(TString("pcc / ")+RefLumi);
    LumiRatio.SetMarkerColor(2);
    LumiRatio.Draw("histp");
    li.SetLineWidth(2);
    li.SetLineColor(1);
    li.SetLineStyle(1);
    li.DrawLine(0,1,plotXrange,1);//y=1 line
    li.SetLineColor(4);
    li.SetLineStyle(2);
    li.SetLineWidth(3);
    //li.DrawLine(0,HistoLumiRatio.GetBinCenter(HistoLumiRatio.GetMaximumBin()),plotXrange,HistoLumiRatio.GetBinCenter(HistoLumiRatio.GetMaximumBin()));
    TLatex Meantxt;
    Meantxt.SetTextSize(0.03);
    Meantxt.SetTextColor(4);
    char meantxt[100];
    sprintf(meantxt,"r=%.3f",HistoLumiRatio.GetBinCenter(HistoLumiRatio.GetMaximumBin()));
    //Meantxt.DrawLatex(plotXrange,HistoLumiRatio.GetBinCenter(HistoLumiRatio.GetMaximumBin()),meantxt);
    C.Print(inpath+"/ls_ratio.png");

    C.Clear();
    HistoLumiRatio.GetYaxis()->SetTitle(" # of lumi sections ");
    HistoLumiRatio.GetXaxis()->SetTitle(TString("ratio"));
    HistoLumiRatio.SetMarkerStyle(8);
    HistoLumiRatio.SetMarkerSize(0.4);
    HistoLumiRatio.Draw("hist");
    //HistoLumiRatio.Fit("gaus");//,"","same",0.9,1.1);
    C.Print(inpath+"/ls_ratio_histo.png");


    C.Clear();
    H2LumiRatioVsLumi.SetStats(0);
    H2LumiRatioVsLumi.GetYaxis()->SetTitle("ratio");
    H2LumiRatioVsLumi.GetXaxis()->SetTitle("Inst. Lumi [Hz/ub]");
    H2LumiRatioVsLumi.Draw("colz");
    C.Print(inpath+"/lumiratio_vs_instlumi.png");
        
    TProfile* H2LumiRatioVsLumiProfX=H2LumiRatioVsLumi.ProfileX();
    H2LumiRatioVsLumiProfX->GetYaxis()->SetTitle("ratio");
    H2LumiRatioVsLumiProfX->GetXaxis()->SetTitle("Inst. Lumi [Hz/ub]");
    H2LumiRatioVsLumiProfX->GetYaxis()->SetRangeUser(minratio,maxratio);
    H2LumiRatioVsLumiProfX->GetYaxis()->SetNdivisions(5);
    H2LumiRatioVsLumiProfX->GetXaxis()->SetNdivisions(5);
    H2LumiRatioVsLumiProfX->SetStats(0);
    C.Clear();
    H2LumiRatioVsLumiProfX->Draw("histpe");
    C.Print(inpath+"/lumiratio_vs_instlumi_profile.png");

  }
  

  gROOT->ProcessLine(".q");
}

