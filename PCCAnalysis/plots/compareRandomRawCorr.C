#include "globals.h"

TString inpath="./May2022Checks/May2022Data/Random/ALCAPROMPT/Run2022A";
//TString inpath="./May2022Checks/May2022Data_Events/Random/ALCAPROMPT/Run2022A";
TString outpath("./May2022Checks/");


void compareRandomRawCorr(long run){

  system(TString("mkdir -p ")+outpath);

  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  TCanvas C("C","",1200,900);
  C.Divide(1,3);
  
  C.Print(outpath+"/compareRandomRawCorr_"+run+".pdf[");

  
  //iterate over the TFile keys
  TFile FCORR((inpath+"/"+run+".root").Data(),"read");
  TIter next(FCORR.GetListOfKeys());
  TObject* key;
  while ((key = next())) {
    TString kname(key->GetName());
    if(!kname.Contains("RawLumiAvg")) continue;
    TObjArray * a = kname.Tokenize("_");
    long r=atoi(((TObjString*)(*a)[1])->GetName());
    long l=atoi(((TObjString*)(*a)[2])->GetName());
    long ls1=atoi(((TObjString*)(*a)[3])->GetName());
    long ls2=atoi(((TObjString*)(*a)[4])->GetName());
    
    if(r!=run) continue;
    //if(l>0) continue;//only look a the first ls block

    TH1F* HRAW = (TH1F*) FCORR.Get(TString("RawLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    if(!HRAW)      continue;
    HRAW->SetTitle(TString("Run: ")+run+" , LS: "+ls1+" - "+ls2);
    HRAW->GetYaxis()->SetTitle("counts");
    HRAW->GetYaxis()->SetNdivisions(4);
    HRAW->GetXaxis()->SetTitle("bcid");
    HRAW->SetMarkerColor(1);
    
    TH1F* HCORR = (TH1F*) FCORR.Get(TString("CorrectedLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    if(!HCORR)      continue;
    HCORR->SetTitle(TString("Run: ")+run+" , LS: "+ls1+" - "+ls2);
    HCORR->GetYaxis()->SetTitle("counts");
    HCORR->GetYaxis()->SetNdivisions(4);
    HCORR->GetXaxis()->SetTitle("bcid");
    HCORR->SetMarkerColor(4);

    TH1F* HSCALE = (TH1F*) FCORR.Get(TString("ScaleFactorsAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    if(!HSCALE)      continue;
    HSCALE->SetTitle(TString("Run: ")+run+" , LS: "+ls1+" - "+ls2);
    HSCALE->GetYaxis()->SetTitle("scale factor");
    HSCALE->GetYaxis()->SetNdivisions(4);
    HSCALE->GetXaxis()->SetTitle("bcid");
    HSCALE->SetMarkerColor(2);


    float maxy=1.1*HRAW->GetMaximum();
    C.cd(1);
    HRAW->GetYaxis()->SetRangeUser(0,maxy);
    HRAW->Draw("histp");

    C.cd(2);
    HCORR->GetYaxis()->SetRangeUser(0,maxy);
    HCORR->Draw("histp");

    C.cd(3);
    HSCALE->GetYaxis()->SetRangeUser(-0.2,1.1);
    HSCALE->Draw("histp");

    //C.Print(outpath+"/compareRandomRawCorr_"+run+"_"+l+".png");
    C.Print(outpath+"/compareRandomRawCorr_"+run+".pdf");
    
    l++;
  }

  C.Print(outpath+"/compareRandomRawCorr_"+run+".pdf]");
  
  gROOT->ProcessLine(".q");
}
