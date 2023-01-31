#include <iostream>
#include <vector>
#include <dirent.h>
#include "globals.h"

float xmin=0,xmax=12;//SBIL
//TString objname="Type1Fraction"; float ymin=0,ymax=0.05;
//TString objname="Type1Res"; float ymin=-0.0001,ymax=0.0001;
TString objname="Type2Res"; float ymin=-0.005,ymax=0.005;


//float sigmavisible=1.00895*5.91e6/ORBITF;//2018 ABCveto
float sigmavisible=4.1694e6/ORBITF;//2022 veto

void afterglow_residual_luminosity_v2(TString inpath, TString outpath, std::string runlist) {
  
  
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
      TH1F* Lumi = (TH1F*) Afterglow.Get(TString("CorrectedLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
      if(!Lumi)      continue;
      
      int bxcounter=0;
      float bincontent_total=0;
      for(int j = 1; j <= Lumi->GetNbinsX(); j++) { 
	float bincontent = Lumi->GetBinContent(j); 
	if (bincontent > 100) {   // only colliding bunches
	  bincontent_total+=bincontent/sigmavisible;
	  bxcounter++;
	}
      }
      std::cout<<Run<<" "<<l<<" "<<bxcounter<<" "<<bincontent_total<<" "<<bincontent_total/bxcounter<<" "<<std::endl;

      float bincontent_avg=0.;
      if(bxcounter>1000){
	bincontent_avg=bincontent_total/bxcounter;      
      }
      bincontent_avg_array[l] = bincontent_avg;
    }  
                                    
    double *Y = residual_type1->GetY();
    for (int l = 0; l < residual_type1->GetN(); l++) {
      if(bincontent_avg_array[l]>1){
	residualvsinstlumi->SetPoint(residualvsinstlumi->GetN(), bincontent_avg_array[l], Y[l]); 
	residualvsLSBlock->SetPoint(residualvsLSBlock->GetN(),residualvsLSBlock->GetN(), Y[l]); 
      }
    }

    ss.ignore(1);   
  }



  TCanvas *cf = new TCanvas("cf"," ", 0,67,1264,678);
  cf->GetFrame()->SetFillColor(21);
  cf->GetFrame()->SetBorderSize(12); 
  
  TH2F *fhist = new TH2F("fhist","",1,xmin,xmax,1,ymin,ymax);
  fhist->GetXaxis()->SetTitle("SBIL [Hz/ub]");
  fhist->GetYaxis()->SetTitle(objname.Data());
  fhist->GetXaxis()->CenterTitle();
  fhist->GetYaxis()->CenterTitle();
  fhist->SetStats(0);
  cf->Clear();
  fhist->Draw("");
  residualvsinstlumi->SetMarkerStyle(8);
  residualvsinstlumi->SetMarkerSize(1.0);
  residualvsinstlumi->Draw("psame");
  cf->Print(outpath+"/afterglow_"+objname+"_vsinstlumi.png");


  cf->Clear();
  residualvsLSBlock->GetXaxis()->SetTitle("50 LS block id");
  residualvsLSBlock->GetYaxis()->SetTitle(objname.Data());
  residualvsLSBlock->GetYaxis()->SetRangeUser(ymin,ymax);
  residualvsLSBlock->SetMarkerStyle(8);
  residualvsLSBlock->SetMarkerSize(1.0);
  residualvsLSBlock->Draw("ap");
  cf->Print(outpath+"/afterglow_"+objname+"_vsLSBlock.png");
}

