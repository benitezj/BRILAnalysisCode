#include "globals.h"

void compareRandomRawCorr(TString inpath, long run, TString outpath){

  system(TString("mkdir -p ")+outpath);

  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");
  TCanvas C("C","",1200,500);

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
    if(l>0) continue;//only look a the first ls block

    TH1F* HRAW = (TH1F*) FCORR.Get(TString("RawLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    if(!HRAW)      continue;
    float scale=1./HRAW->GetMaximum();
    HRAW->Scale(scale);
    
    TH1F* HCORR = (TH1F*) FCORR.Get(TString("CorrectedLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    if(!HCORR)      continue;
    HCORR->Scale(scale);
    

    C.Clear();
    HRAW->SetTitle(TString("Run: ")+run+" , LS: "+ls1+" - "+ls2);
    HRAW->GetYaxis()->SetTitle("counts");
    HRAW->GetXaxis()->SetTitle("bcid");
    HRAW->GetYaxis()->SetRangeUser(-0.1,1.1*HRAW->GetMaximum());
    HRAW->SetMarkerColor(1);
    HRAW->Draw("histp");
    HCORR->SetMarkerColor(4);
    HCORR->Draw("histpsame");
    C.Print(outpath+"/compareRandomRawCorr_"+run+"_"+l+".png");
    
    l++;
  }


  gROOT->ProcessLine(".q");
}
