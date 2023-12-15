#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"

float minL=0;
float minratio=0.95;
float maxratio=1.05;
float plotYrangeMin=0.01;
float plotYrangeMax;//code will find max lumi below
float plotYrangeMaxLog=10;//scale factor for log plot
float plotYrangeZoomMin=0.0;
float plotYrangeZoomMax=0.2;//zoomed plot


void plotPCCStability(TString inpath, int plotXrangeMin=0, int plotXrangeMax=100, TString REF="HFET"){

  RefLumi=REF;

  ifstream myfile((inpath+"/ls.dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
  }
  cout<<"opened "<<(inpath+"/ls.dat")<<std::endl;


  ///create histogram
  TH1F HistoLumiRatio("HistoLumiRatio","",200,minratio,maxratio);
  TH1F LumiRatio("HLumiRatio","",(plotXrangeMax-plotXrangeMin),plotXrangeMin,plotXrangeMax);
  TH1F Lumi("HLumi","",plotXrangeMax-plotXrangeMin,plotXrangeMin,plotXrangeMax);
  TH1F LumiRef("HLumiRef","",plotXrangeMax-plotXrangeMin,plotXrangeMin,plotXrangeMax);
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
  double totL=0;    //lumi for given LS
  double totLRef=0; //lumi for given LS
  while (std::getline(myfile, line)){
    std::stringstream iss(line);    
    iss>>run>>ls>>totL; //space separated

    if(RefLumi.CompareTo("")!=0) iss>>totLRef; //>>ncollb;
    //cout<<run<<" "<<ls<<" "<<totL<<" "<<totLRef<<endl;

    if(totL<minL) continue;

    if(run!=prevrun){
      counter_ls_run_tot += counter_ls_run;
      counter_ls_run=0;
      prevrun=run;
    }

    counter_ls_run++;
    counterLumi = counter_ls_run_tot + ls;


    Lumi.SetBinContent(counterLumi-plotXrangeMin,totL);
    if(totL>plotYrangeMax) plotYrangeMax=totL;


    float ratio=0;
    if(totLRef>0 ){
      ratio=totL/totLRef;
    }
    LumiRef.SetBinContent(counterLumi-plotXrangeMin,totLRef);
    HistoLumiRatio.Fill(ratio);
    LumiRatio.SetBinContent(counterLumi-plotXrangeMin,ratio);
    H2LumiRatioVsLumi.Fill(totL,ratio);

    if(run>lastrun){
      runlist[counterLumi]=run;
      lastrun=run;
    }
    if(run<firstrun){
       firstrun=run;
    }

  }
  cout<<"TotalLS: "<<counterLumi<<"  MaxLumi: "<<plotYrangeMax<<" FirstRun:"<<firstrun<<" LastRun:"<<lastrun<<endl;
  myfile.close();


  TCanvas C("C","",1000,600);
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1);



  /////////////////////////////
  TLegend leg(0.70,0.92,0.90,0.99);
  leg.SetLineWidth(0);
  leg.SetFillStyle(0);
  leg.SetNColumns(2);

  C.Clear();
  Lumi.SetTitle(TString("Runs: ")+firstrun+" - "+lastrun);
  Lumi.SetStats(0);
  Lumi.SetMarkerStyle(8);
  Lumi.SetMarkerSize(0.3);
  Lumi.GetXaxis()->SetTitle("Lumi Section");
  Lumi.GetYaxis()->SetTitle("Inst. Lumi [Hz/ub]");
  Lumi.GetYaxis()->SetRangeUser(plotYrangeMin,1.5*plotYrangeMax);
  Lumi.Draw("histp");
  leg.AddEntry(&Lumi,"PCC","p");

  TLine li;
  li.SetLineStyle(2);
  TLatex ltxt;
  ltxt.SetTextAngle(90);
  ltxt.SetTextSize(0.03);
  TString rtxt("");
  //for ( std::map<int,int>::iterator it = runlist.begin(); it != runlist.end(); it++){
  //li.DrawLine(it->first,plotYrangeMin,it->first,plotYrangeMax);
  //}


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
  Lumi.GetYaxis()->SetRangeUser(plotYrangeMin,plotYrangeMaxLog*plotYrangeMax);
  C.Update();
  C.Print(inpath+"/ls_lumi_log.png");
  C.SetLogy(0);


  TF1 fitF("bkg","[0]",plotXrangeMin,plotXrangeMax);
  fitF.SetLineColor(2);
  Lumi.GetYaxis()->SetRangeUser(plotYrangeZoomMin,plotYrangeZoomMax);
  Lumi.Fit(&fitF,"","",plotXrangeMin,plotXrangeMax);
  fitF.Draw("lsame");
  C.Update();
  C.Print(inpath+"/ls_lumi_zoom.png");




  ///Ratio graphs
  if(RefLumi.CompareTo("")!=0){
        
    C.Clear();
    LumiRatio.SetStats(0);
    LumiRatio.GetYaxis()->SetRangeUser(minratio,maxratio);
    LumiRatio.SetMarkerStyle(8);
    LumiRatio.SetMarkerSize(0.4);
    LumiRatio.GetXaxis()->SetTitle("Lumi Section");
    LumiRatio.GetYaxis()->SetTitle(TString("PCC / ")+RefLumi);
    LumiRatio.SetMarkerColor(2);
    LumiRatio.Draw("histp");
    li.SetLineWidth(2);
    li.SetLineColor(1);
    li.SetLineStyle(1);
    li.DrawLine(plotXrangeMin,1,plotXrangeMax,1);//y=1 line
    li.SetLineColor(4);
    li.SetLineStyle(2);
    li.SetLineWidth(3);

    TLatex Meantxt;
    Meantxt.SetTextSize(0.03);
    Meantxt.SetTextColor(4);
    char meantxt[100];
    sprintf(meantxt,"r=%.3f",HistoLumiRatio.GetBinCenter(HistoLumiRatio.GetMaximumBin()));
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

