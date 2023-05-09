#include <iostream>
#include <vector>
#include <dirent.h>
#include "globals.h"


TString pcctitle="Scale Factor"; float pccmin=0.9,pccmax=1.02;//SBIL
int bcid=3041;//selected bcd for iov dependence plot 
int iov=-1;//selected IOV for bcid dependence plot

//Run 366800: Randoms last train [3040,3075],  ZB ?

float collidingcountMin=0.6;
//TString objname="Type1Fraction"; TString objtitle="Type1 fraction"; float resmin=0,resmax=0.05;
//TString objname="Type1Res"; TString objtitle="Type1: Non-colliding residual / Colliding PCC"; float resmin=-0.0001,resmax=0.0001;
TString objname="Type2Res"; TString objtitle="Type2: Non-colliding residual / Colliding PCC"; float resmin=-0.02,resmax=0.02;

void afterglow_residual_luminosity_v2(TString inpath, TString outpath, std::string runlist) {
  
  TGraph* lumiVsIOV=new TGraph();
  TGraph* lumiVsBX=new TGraph();
  TGraph* residualvsinstlumi=new TGraph();
  TGraph* residualvsLSBlock=new TGraph();

  std::stringstream ss(runlist.c_str());
  int Run;
  while (ss >> Run) {
    cout<<Run<<endl;  
    TFile Afterglow(inpath+"/"+Run+".root");
    
    TGraphErrors* residual_type1 = (TGraphErrors*)Afterglow.Get(objname.Data());
    if(!residual_type1){cout<<objname<<" not found"<<endl; continue;}

    //*****************************************
    float  bincontent_avg_array[residual_type1->GetN()];
    TIter next(Afterglow.GetListOfKeys());
    TObject* key;
    while ((key = next())) {
      TString kname(key->GetName());
      if(!kname.Contains("CorrectedLumiAvg")) continue;
      TObjArray * a = kname.Tokenize("_");
      long r=atoi(((TObjString*)(*a)[1])->GetName());
      long l=atoi(((TObjString*)(*a)[2])->GetName());
      long ls1=atoi(((TObjString*)(*a)[3])->GetName());
      long ls2=atoi(((TObjString*)(*a)[4])->GetName());
      if(r!=Run) continue;
      if(iov!=-1 && l!=iov) continue;
      //TH1F* Lumi = (TH1F*) Afterglow.Get(TString("CorrectedLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
      TH1F* Lumi = (TH1F*) Afterglow.Get(TString("ScaleFactorsAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
      if(!Lumi)      continue;
      
      int bxcounter=0;
      float bincontent_total=0;
      for(int j = 1; j <= Lumi->GetNbinsX(); j++) {
	float bincontent = Lumi->GetBinContent(j);
	lumiVsBX->SetPoint(lumiVsBX->GetN(),j, bincontent);  
	if(bcid!=-1 && j!=bcid) continue;       // use only one bcid, if disabled will compute orbit average
	if (bincontent > collidingcountMin) {   // select only colliding bunches
	  bincontent_total+=bincontent;
	  bxcounter++;
	  //std::cout<<j<<" "<<bincontent<<std::endl;
	}
      }
      //std::cout<<Run<<" "<<l<<" "<<bxcounter<<" "<<bincontent_total<<" "<<bincontent_total/bxcounter<<" "<<std::endl;

      float bincontent_avg=0.;
      if(bxcounter>0)
	bincontent_avg=bincontent_total/bxcounter;      
      bincontent_avg_array[l] = bincontent_avg;
    }  
                                    
    double *Y = residual_type1->GetY();
    for (int l = 0; l < residual_type1->GetN(); l++) {
      if(bincontent_avg_array[l]>collidingcountMin){
	lumiVsIOV->SetPoint(lumiVsIOV->GetN(),l, bincontent_avg_array[l]); 
	residualvsinstlumi->SetPoint(residualvsinstlumi->GetN(), bincontent_avg_array[l], Y[l]); 
	residualvsLSBlock->SetPoint(residualvsLSBlock->GetN(),residualvsLSBlock->GetN(), Y[l]); 
      }
    }

    ss.ignore(1);   
  }


  TCanvas *cf = new TCanvas("cf"," ", 0,67,1264,678);
  cf->GetFrame()->SetFillColor(21);
  cf->GetFrame()->SetBorderSize(12); 
  


  ///Lumi vs iov block
  cf->Clear();
  lumiVsIOV->GetXaxis()->SetTitle("LS block id");
  lumiVsIOV->GetYaxis()->SetTitle(pcctitle);
  lumiVsIOV->GetYaxis()->SetRangeUser(pccmin,pccmax);  
  lumiVsIOV->SetMarkerStyle(8);
  lumiVsIOV->SetMarkerSize(1.0);
  lumiVsIOV->Draw("ap");
  cf->Print(outpath+"/afterglow_lumi_iov.png");

  //lumi vs bx
  cf->Clear();
  lumiVsBX->GetXaxis()->SetTitle("bcid");
  lumiVsBX->GetYaxis()->SetTitle(pcctitle);
  lumiVsBX->GetYaxis()->SetRangeUser(pccmin,pccmax);  
  lumiVsBX->SetMarkerStyle(8);
  lumiVsBX->SetMarkerSize(1.0);
  lumiVsBX->Draw("ap");
  cf->Print(outpath+"/afterglow_lumi_bx.png");


  /////////////////////////////
  /// Residuals vs inst lumi
  TH2F *fhist = new TH2F("fhist","",1,pccmin,pccmax,1,resmin,resmax);
  fhist->GetXaxis()->SetTitle("PCC count (orbit avg)");
  fhist->GetYaxis()->SetTitle(objtitle.Data());
  fhist->GetXaxis()->CenterTitle();
  fhist->GetYaxis()->CenterTitle();
  fhist->SetStats(0);
  cf->Clear();
  fhist->Draw("");
  residualvsinstlumi->SetMarkerStyle(8);
  residualvsinstlumi->SetMarkerSize(1.0);
  residualvsinstlumi->Draw("psame");
  cf->Print(outpath+"/afterglow_"+objname+"_vsinstlumi.png");

  /////////////////////////////
  /// Residuals vs LS block
  cf->Clear();
  residualvsLSBlock->GetXaxis()->SetTitle("LS block id");
  residualvsLSBlock->GetYaxis()->SetTitle(objtitle.Data());
  residualvsLSBlock->GetYaxis()->SetRangeUser(resmin,resmax);
  residualvsLSBlock->SetMarkerStyle(8);
  residualvsLSBlock->SetMarkerSize(1.0);
  residualvsLSBlock->Draw("ap");
  cf->Print(outpath+"/afterglow_"+objname+"_vsLSBlock.png");
}

