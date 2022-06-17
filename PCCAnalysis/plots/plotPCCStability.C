#include <iostream>
#include <fstream>
#include <string>

float minratio=0.95;
float maxratio=1.05;
float plotYrange=0.2;


void plotPCCStability(TString inpath, TString ref, int plotXrange=100){

  //gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  ifstream myfile((inpath+"/ls.dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
  }
  cout<<"opened "<<(inpath+"/ls.dat")<<std::endl;


  ///create histogram
  TH1F HLumiRatio("HLumiRatio","",100,minratio,maxratio);


  TGraph LumiRatio;
  int counterRatio=0;

  TH1F Lumi("HLumi","",plotXrange,0,plotXrange);
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
      HLumiRatio.Fill(ratio);

      LumiRatio.SetPoint(counterRatio,counterRatio,ratio);
      counterRatio++; 
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


  C.Clear();
  Lumi.SetStats(0);
  Lumi.SetMarkerStyle(8);
  Lumi.SetMarkerSize(0.5);
  Lumi.GetXaxis()->SetTitle("lumi section");
  Lumi.GetYaxis()->SetTitle(" PCC Lumi per LS [#mub^{-1}]");
  Lumi.GetYaxis()->SetRangeUser(0,plotYrange);
  Lumi.Draw("histp");

   //draw run separators
   TLine li;
   li.SetLineStyle(2);
   TLatex ltxt;
   ltxt.SetTextAngle(90);
   ltxt.SetTextSize(0.03);
   TString rtxt("");
   for ( std::map<int,int>::iterator it = runlist.begin(); it != runlist.end(); it++){
     li.DrawLine(it->first,0,it->first,plotYrange);
     ltxt.DrawLatex(it->first,0.02,rtxt+it->second);
   }

  C.Print(inpath+"/ls_lumi.png");


  if(ref.CompareTo("")!=0){
    C.Clear();
    HLumiRatio.GetYaxis()->SetTitle(" # of lumi sections ");
    HLumiRatio.GetXaxis()->SetTitle(TString(" PCC / ")+ref);
    HLumiRatio.SetMarkerStyle(8);
    HLumiRatio.SetMarkerSize(0.5);
    //HLumiRatio.Draw("hist");
    HLumiRatio.Fit("gaus","","same",0.98,1.02);
    C.Print(inpath+"/ls_ratio_histo.png");
    
    
    C.Clear();
    LumiRatio.GetYaxis()->SetRangeUser(minratio,maxratio);
    LumiRatio.SetMarkerStyle(8);
    LumiRatio.SetMarkerSize(0.5);
    LumiRatio.GetXaxis()->SetTitle("lumi section");
    LumiRatio.GetYaxis()->SetTitle(TString(" PCC / ")+ref);
    LumiRatio.Draw("ap");
    C.Print(inpath+"/ls_ratio.png");
  }
  

  gROOT->ProcessLine(".q");
}

