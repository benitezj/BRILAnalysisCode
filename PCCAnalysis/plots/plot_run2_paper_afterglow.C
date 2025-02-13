#include <iostream>
#include <vector>
#include <dirent.h>
#include "globals.h"

void plotlabels(){
  //drawCMSPrelim();
  //drawFillYear(8395,2022);
  drawFillYear(0,2017);
  //drawPCCLuminometer();
}


//TString input="./AfterglowModel_2022Data_SummerVeto/data_v2/Run2022F/362064.root";  TString lsblock="362064_19_968_1018";
TString input="2017ReReco/Random_v4/297219.root";
TString lsblock="297219_3_991_1039";


void plot_run2_paper_afterglow() {

  cout<<"input path: "<<input<<endl;
  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");

  TFile InputFile(input.Data(),"read");
  if(InputFile.IsZombie()) return;
  
  TH1F* HRaw=(TH1F*)InputFile.Get("RawLumiAvg_"+lsblock);
  TH1F* HCorr=(TH1F*)InputFile.Get("CorrectedLumiAvg_"+lsblock);
  if(!HRaw || !HCorr) return;

  
  ////////////////////
  TCanvas Canvas("Canvas"," ", 0,67,1264,678);

  /* TLegend leg(.66,.84,.72,.90); */
  /* leg.SetFillColor(0); */
  /* leg.SetFillStyle(0); */
  /* leg.SetLineColor(0); */
  /* leg.SetTextColor(1); */
  /* leg.AddEntry(HRaw,"Raw","p"); */
  /* TLegend leg2(.72,.84,.88,.90); */
  /* leg2.SetFillColor(0); */
  /* leg2.SetFillStyle(0); */
  /* leg2.SetLineColor(0); */
  /* leg2.SetTextColor(4); */
  /* leg2.AddEntry(HCorr,"Corrected","p"); */
  
  Canvas.Clear();
  Canvas.SetLogy(1);
  HRaw->GetYaxis()->SetRangeUser(1,5000);
  HRaw->SetStats(0);
  HRaw->GetYaxis()->SetTitle("PCC");
  HRaw->GetXaxis()->SetTitle("BCID        ");
  HRaw->Draw("histp");
  HCorr->SetMarkerColor(4);
  HCorr->Draw("histpsame");
  plotlabels();
  // leg.Draw();
  //leg2.Draw();

  TLatex text;
  text.SetTextSize(0.04);
  text.SetTextColor(1);
  text.DrawLatexNDC(0.2,0.93,"Raw rate, ");
  text.SetTextColor(4);
  text.DrawLatexNDC(0.4,0.93,"Corrected rate");
  
  Canvas.Print("./plot_afterglow_RawCorr.png");
  Canvas.Print("./plot_afterglow_RawCorr.pdf");


  Canvas.Clear();
  Canvas.SetLogy(0);
  HRaw->GetYaxis()->SetRangeUser(-100,400);
  HRaw->Draw("histp");
  HCorr->Draw("histpsame");
  plotlabels();
  text.SetTextColor(1);
  text.DrawLatexNDC(0.2,0.93,"Raw rate, ");
  text.SetTextColor(4);
  text.DrawLatexNDC(0.4,0.93,"Corrected rate");
  Canvas.Print("./plot_afterglow_RawCorr_zoom.png");
  Canvas.Print("./plot_afterglow_RawCorr_zoom.pdf");

  
}


