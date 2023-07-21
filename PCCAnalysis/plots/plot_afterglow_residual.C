#include <iostream>
#include <vector>
#include <dirent.h>
#include "globals.h"


TString pcctitle="Scale Factor"; 
float pccmin=0.9,pccmax=1.02; //SBIL
int bcid=3041;//selected bcd for iov dependence plot 
int iov=-1;//selected IOV for bcid dependence plot

//Run 366800: Randoms last train [3040,3075],  ZB ?

float collidingcountMin=1000;
TString objname="Type1Fraction"; TString objtitle="Type1 fraction"; float resmin=0,resmax=0.05;
//TString objname="Type1Res"; TString objtitle="Type1: Non-colliding residual / Colliding PCC"; float resmin=-0.0001,resmax=0.0001;
//TString objname="Type2Res"; TString objtitle="Type2: Non-colliding residual / Colliding PCC"; float resmin=-0.02,resmax=0.02;



float getPCCAvg(TFile*F=NULL, int Run=-1, int I=-1){
  if(F==NULL || Run<0 || I<0) return 0.;
  
  float bincontent_avg=0.;
  TIter next(F->GetListOfKeys());
  TObject* key;
  while ((key = next())) {
    TString kname(key->GetName());
    if(!kname.Contains("RawLumiAvg_")) continue;
    
    TObjArray * a = kname.Tokenize("_");
    long r=atoi(((TObjString*)(*a)[1])->GetName());
    long l=atoi(((TObjString*)(*a)[2])->GetName());
    long ls1=atoi(((TObjString*)(*a)[3])->GetName());
    long ls2=atoi(((TObjString*)(*a)[4])->GetName());
  
    if(r!=Run || l!=I) continue;
    
    //TH1F* Lumi = (TH1F*) Afterglow.Get(TString("RawLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    TH1F* Lumi = (TH1F*) F->Get(kname.Data());
    if(!Lumi)      continue;
    
    int bxcounter=0;
    float bincontent_total=0;
    for(int j = 1; j <= Lumi->GetNbinsX(); j++) {
      float bincontent = Lumi->GetBinContent(j);
      if (bincontent > collidingcountMin){
	bincontent_total += bincontent;
	bxcounter++;
      }
    }
    
    if(bxcounter>0)
      bincontent_avg=bincontent_total/bxcounter;      
  }    

  return bincontent_avg;
}

void plot_afterglow_residual(TString inpath, TString outpath, std::string runlist) {

  TGraph* gResidVsPCC=new TGraph();
  TGraph* gResidVsIOV=new TGraph();

  ////////////////////
  std::stringstream ss(runlist.c_str());
  int Run;
  while (ss >> Run) {
    cout<<Run<<endl;  
    TFile InputFile(inpath+"/"+Run+".root");
    if(InputFile.IsZombie()) continue;

    TGraphErrors* gRes = (TGraphErrors*)InputFile.Get(objname.Data());
    if(!gRes){cout<<objname<<" not found"<<endl; continue;}

    double *Y = gRes->GetY();
    for (int l = 0; l < gRes->GetN(); l++) {
      float pccavg= getPCCAvg(&InputFile,Run,l);
      if(pccavg>collidingcountMin){
	gResidVsPCC->SetPoint(gResidVsPCC->GetN(),pccavg, Y[l]); 
	gResidVsIOV->SetPoint(gResidVsIOV->GetN(),gResidVsIOV->GetN(), Y[l]); 
      }
    }

    ss.ignore(1);   
  }


  ////////////////////
  TCanvas Canvas("Canvas"," ", 0,67,1264,678);

  Canvas.Clear();
  gResidVsPCC->GetYaxis()->SetTitle(objtitle.Data());
  gResidVsPCC->GetXaxis()->SetTitle("Avg. PCC");
  gResidVsPCC->GetYaxis()->SetRangeUser(resmin,resmax);
  gResidVsPCC->SetMarkerStyle(8);
  gResidVsPCC->SetMarkerSize(0.6);
  gResidVsPCC->Draw("ap");
  Canvas.Print(outpath+"/afterglow_"+objname+"_vsinstlumi.png");

  Canvas.Clear();
  gResidVsIOV->GetXaxis()->SetTitle("LS block id");
  gResidVsIOV->GetYaxis()->SetTitle(objtitle.Data());
  gResidVsIOV->GetYaxis()->SetRangeUser(resmin,resmax);
  gResidVsIOV->SetMarkerStyle(8);
  gResidVsIOV->SetMarkerSize(0.6);
  gResidVsIOV->Draw("ap");
  Canvas.Print(outpath+"/afterglow_"+objname+"_vsLSBlock.png");
}



//    //*****************************************
//    float  bincontent_avg_array[gRes->GetN()];
//    TIter next(InputFile.GetListOfKeys());
//    TObject* key;
//    while ((key = next())) {
//      TString kname(key->GetName());
//      if(!kname.Contains("CorrectedLumiAvg")) continue;
//      TObjArray * a = kname.Tokenize("_");
//      long r=atoi(((TObjString*)(*a)[1])->GetName());
//      long l=atoi(((TObjString*)(*a)[2])->GetName());
//      long ls1=atoi(((TObjString*)(*a)[3])->GetName());
//      long ls2=atoi(((TObjString*)(*a)[4])->GetName());
//      if(r!=Run) continue;
//      if(iov!=-1 && l!=iov) continue;
//
//      TH1F* Lumi = (TH1F*) InputFile.Get(TString("RawLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
//      if(!Lumi)      continue;
//      
//      int bxcounter=0;
//      float bincontent_total=0;
//      for(int j = 1; j <= Lumi->GetNbinsX(); j++) {
//	float bincontent = Lumi->GetBinContent(j);
//	if (bincontent > collidingcountMin) {   // select only colliding bunches
//	  bincontent_total+=bincontent;
//	  bxcounter++;
//	}
//      }
//      float bincontent_avg=0.;
//      if(bxcounter>0)
//	bincontent_avg=bincontent_total/bxcounter;      
//      bincontent_avg_array[l] = bincontent_avg;
//    }                                      
//    double *Y = gRes->GetY();
//    for (int l = 0; l < gRes->GetN(); l++) {
//      if(bincontent_avg_array[l]>collidingcountMin){
//	gResidVsPCC->SetPoint(gResidVsPCC->GetN(), bincontent_avg_array[l], Y[l]); 
//	gResidVsIOV->SetPoint(gResidVsIOV->GetN(),gResidVsIOV->GetN(), Y[l]); 
//      }
//    }
