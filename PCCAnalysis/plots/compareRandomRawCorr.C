#include "globals.h"

//TString inpath="./May2022Checks/May2022Data/Random/ALCAPROMPT/Run2022A";
//TString inpath="./May2022Checks/May2022Data_Events/Random/ALCAPROMPT/Run2022A";
TString inpath="./May2022Checks/Run2022Bdata/Random/ALCAPROMPT/Run2022B/";
TString outpath("./May2022Checks/");


int calculateNcolliding(TH1F*H){
  if(!H){
    cout<<"calculateNcolliding  bad histo"<<endl;
    return 0;
  }
  
  float max=H->GetBinContent(H->GetMaximumBin());
  unsigned ncol=0;
  for(int b=0;b<NBX;b++){
    if(H->GetBinContent(b)>0.5*max)
      ncol++;
  }

  return ncol;
}


void compareRandomRawCorr(long run){


  system(TString("mkdir -p ")+outpath);

  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  TCanvas C("C","",1200,900);
  C.Divide(1,3);  
  C.Print(outpath+"/compareRandomRawCorr_"+run+".pdf[");

  cout<<"Reading from: "<<inpath<<endl;

  TLatex text;

  
  //iterate over the TFile keys
  int counter=0;
  TFile FCORR((inpath+"/"+run+".root").Data(),"read");
  if(FCORR.IsZombie()){
    cout<<"Run "<<run<<"  not found in "<<inpath<<endl;
    return;
  }
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

    
    TH1F* HRAW = (TH1F*) FCORR.Get(TString("RawLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    if(!HRAW)      continue;
    HRAW->SetTitle(TString("Run: ")+run+" , LS: "+ls1+" - "+ls2);
    HRAW->GetYaxis()->SetTitle("Raw and Corrected counts");
    HRAW->GetYaxis()->SetNdivisions(4);
    HRAW->GetXaxis()->SetTitle("bcid");
    HRAW->SetMarkerColor(1);
    HRAW->SetLineColor(1);   

    unsigned nRaw=calculateNcolliding(HRAW);
    
    TH1F* HCORR = (TH1F*) FCORR.Get(TString("CorrectedLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    if(!HCORR)      continue;
    HCORR->SetTitle(TString("Run: ")+run+" , LS: "+ls1+" - "+ls2);
    HCORR->GetYaxis()->SetTitle("RAW and Corrected counts");
    HCORR->GetYaxis()->SetNdivisions(4);
    HCORR->GetXaxis()->SetTitle("bcid");
    HCORR->SetMarkerColor(4);

    unsigned nCorr=calculateNcolliding(HCORR);
    
    TH1F* HSCALE = (TH1F*) FCORR.Get(TString("ScaleFactorsAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    if(!HSCALE)      continue;
    HSCALE->SetTitle(TString("Run: ")+run+" , LS: "+ls1+" - "+ls2);
    HSCALE->GetYaxis()->SetTitle("scale factor");
    HSCALE->GetYaxis()->SetNdivisions(4);
    HSCALE->GetXaxis()->SetTitle("bcid");
    HSCALE->SetMarkerColor(2);

    unsigned nScale=calculateNcolliding(HSCALE);
    
    float maxy=1.1*HRAW->GetMaximum();
    TH1F*HRAWFrame=(TH1F*)HRAW->Clone("HRAWFrame");
    HRAWFrame->Reset();
    HRAWFrame->Clear();
    HRAWFrame->GetYaxis()->SetRangeUser(0,maxy);
    C.cd(1);
    HRAWFrame->Draw("hist");
    HRAW->Draw("histsame");
    HCORR->Draw("histpsame");

    
    TVirtualPad* p2=C.cd(2);
    p2->SetLogy(1);
    HRAW->GetYaxis()->SetRangeUser(0.1,5*maxy);
    HRAW->GetYaxis()->SetNdivisions(3);
    HRAW->Draw("hist");
    HCORR->Draw("histpsame");
    text.SetTextColor(1);
    text.DrawTextNDC(0.78,0.85,TString("# colliding = ")+nRaw);
    text.SetTextColor(4);
    text.DrawTextNDC(0.78,0.80,TString("# colliding = ")+nCorr);
    
    C.cd(3);
    HSCALE->GetYaxis()->SetRangeUser(-0.2,1.1);
    HSCALE->Draw("histp");
    text.SetTextColor(2);
    text.DrawTextNDC(0.78,0.85,TString("# colliding = ")+nScale);
    
    C.Print(outpath+"/compareRandomRawCorr_"+run+".pdf");
    counter++;
  }
  cout<<"Number of histograms: "<<counter<<endl;
  
  
  C.Print(outpath+"/compareRandomRawCorr_"+run+".pdf]");
  
  gROOT->ProcessLine(".q");
}
