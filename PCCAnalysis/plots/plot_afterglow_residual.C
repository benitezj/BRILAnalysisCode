#include <iostream>
#include <vector>
#include <dirent.h>
#include "globals.h"


TString pcctitle="Scale Factor"; 
float collidingcountMin=3; //select fills with this number of bunches or larger

int NAfterGlowBX=15;//needed for background estimation from non-colliding bx


std::pair<float,float> getPCCAvg(TFile*F=NULL, int Run=-1, int I=-1, int type=1){
  if(F==NULL || Run<0 || I<0) return std::pair<float,float>();
  
  std::pair<float,float> bincontent_avg;
  
  TIter next(F->GetListOfKeys());
  TObject* key;
  while ((key = next())) {
    TString kname(key->GetName());    //TString("RawLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    if(!kname.Contains("RawLumiAvg_")) continue;
    
    TObjArray * a = kname.Tokenize("_");
    long r=atoi(((TObjString*)(*a)[1])->GetName());
    long l=atoi(((TObjString*)(*a)[2])->GetName());
    long ls1=atoi(((TObjString*)(*a)[3])->GetName());
    long ls2=atoi(((TObjString*)(*a)[4])->GetName());
  
    if(r!=Run || l!=I) continue; //select the desired block
    
    
    TH1F* Lumi = (TH1F*) F->Get(kname.Data());
    if(!Lumi)      continue;

    if(type==1){///Average colliding rate
      int nbincounter=0;
      float bincontent_total=0.;
      for(int j = 1; j <= Lumi->GetNbinsX(); j++) {
	float bincontent = Lumi->GetBinContent(j);
	if (bincontent > collidingcountMin){
	  bincontent_total += bincontent;
	  nbincounter++;
	}
      }
      if(nbincounter>0)
	bincontent_avg.first =bincontent_total/nbincounter;
    }

    if(type==2){///background from non-colliding bx
      int afterglowcounter=0;
      int nbincounter=0;
      float bincontent_total=0;
      //calculate the mean
      for(int j = 1; j <= Lumi->GetNbinsX(); j++) {
	float bincontent = Lumi->GetBinContent(j);
	afterglowcounter++;
	if(bincontent > collidingcountMin)
	  afterglowcounter=0;
	if(afterglowcounter>NAfterGlowBX && bincontent>0){
	  bincontent_total += bincontent;
	  nbincounter++;
	}
      }
      
      if(nbincounter>0){
	bincontent_avg.first = bincontent_total / nbincounter;

	//calculate the RMS
	afterglowcounter=0;
	bincontent_total=0.;
	for(int j = 1; j <= Lumi->GetNbinsX(); j++) {
	  float bincontent = Lumi->GetBinContent(j);
	  afterglowcounter++;
	  if(bincontent > collidingcountMin)
	    afterglowcounter=0;
	  if(afterglowcounter>NAfterGlowBX && bincontent>0)
	    bincontent_total += (bincontent-bincontent_avg.first)*(bincontent-bincontent_avg.first);
	}
	//bincontent_avg.second = sqrt(bincontent_total / nbincounter); // RMS
	bincontent_avg.second = sqrt(bincontent_total) / nbincounter; //SEM
      }


      
    }
    
  }    

  return bincontent_avg;
}

void plot_afterglow_residual(TString inpath, TString outpath, std::string runlist) {
  cout<<"input path: "<<inpath<<endl;
  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");

  TGraph* gAvgVsIOV=new TGraph();
    
  TGraph* gT1fracVsPCC=new TGraph();
  TGraph* gT1fracVsIOV=new TGraph();
  TGraph* gT1ResidVsPCC=new TGraph();
  TGraph* gT1ResidVsIOV=new TGraph();
  TGraph* gT2ResidVsPCC=new TGraph();
  TGraph* gT2ResidVsIOV=new TGraph();

  TGraph* gPedVsIOV=new TGraph();
  TGraphErrors* gBkgVsIOV=new TGraphErrors();


  ////////////////////
  std::stringstream ss(runlist.c_str());
  int Run;
  while (ss >> Run) {
    cout<<Run<<endl;  
    TFile InputFile(inpath+"/"+Run+".root");
    if(InputFile.IsZombie()) continue;
    //InputFile.ls();

    TGraphErrors* gT1frac = (TGraphErrors*)InputFile.Get("Type1Fraction");
    TGraphErrors* gT1resid = (TGraphErrors*)InputFile.Get("Type1Res");
    TGraphErrors* gT2resid = (TGraphErrors*)InputFile.Get("Type2Res"); 
    TGraphErrors* gPed = (TGraphErrors*)InputFile.Get("Pedestal");
    if(!gT1frac || !gT2resid || !gT2resid){cout<<" objects  not found. run="<<Run<<endl; continue;}

    double *YT1frac = gT1frac->GetY();
    double *YT1resid = gT1resid->GetY();
    double *YT2resid = gT2resid->GetY();
    double *YPed = gPed->GetY();
    for (int l = 0; l < gT1frac->GetN(); l++) {
      std::pair<float,float> pccavg=getPCCAvg(&InputFile,Run,l);
      gAvgVsIOV->SetPoint(gAvgVsIOV->GetN(),gAvgVsIOV->GetN(),pccavg.first);
      if(pccavg.first>collidingcountMin){
	gT1fracVsPCC->SetPoint(gT1fracVsPCC->GetN(),pccavg.first, YT1frac[l]); 
	gT1fracVsIOV->SetPoint(gT1fracVsIOV->GetN(),gT1fracVsIOV->GetN(), YT1frac[l]); 
	gT1ResidVsPCC->SetPoint(gT1ResidVsPCC->GetN(),pccavg.first, YT1resid[l]); 
	gT1ResidVsIOV->SetPoint(gT1ResidVsIOV->GetN(),gT1ResidVsIOV->GetN(), YT1resid[l]); 
	gT2ResidVsPCC->SetPoint(gT2ResidVsPCC->GetN(),pccavg.first, YT2resid[l]); 
	gT2ResidVsIOV->SetPoint(gT2ResidVsIOV->GetN(),gT2ResidVsIOV->GetN(), YT2resid[l]);
	gPedVsIOV->SetPoint(gPedVsIOV->GetN(),gPedVsIOV->GetN(), YPed[l]); 
      }

      std::pair<float,float> pccbkg = getPCCAvg(&InputFile,Run,l,2);
      gBkgVsIOV->SetPoint(gBkgVsIOV->GetN(),gBkgVsIOV->GetN(),pccbkg.first);
      gBkgVsIOV->SetPointError(gBkgVsIOV->GetN()-1,0,pccbkg.second); 
      
    }

    ss.ignore(1);   
  }


  ////////////////////
  TCanvas Canvas;//("Canvas"," ", 0,67,1264,678);

  ///////////////////////////////////
  ///type 1 fraction
  Canvas.Clear();
  gT1fracVsPCC->GetYaxis()->SetTitle("Type 1 Afterglow Fraction");
  gT1fracVsPCC->GetXaxis()->SetTitle("Average Raw PCC per BCID");
  gT1fracVsPCC->GetYaxis()->SetRangeUser(0.,0.04);
  gT1fracVsPCC->SetMarkerStyle(8);
  gT1fracVsPCC->SetMarkerSize(0.6);
  gT1fracVsPCC->Draw("ap");
  drawCMSPrelim();
  drawFillYear(0,2022);
  drawPCCLuminometer();
  Canvas.Print(outpath+"/afterglow_t1f_vsinstlumi.png");

  Canvas.Clear();
  gT1fracVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gT1fracVsIOV->GetYaxis()->SetTitle("Type 1 Afterglow Fraction");
  gT1fracVsIOV->GetYaxis()->SetRangeUser(0,0.04);
  gT1fracVsIOV->SetMarkerStyle(8);
  gT1fracVsIOV->SetMarkerSize(0.6);
  gT1fracVsIOV->Draw("ap");
  drawCMSPrelim();
  drawFillYear(0,2022);
  drawPCCLuminometer();
  Canvas.Print(outpath+"/afterglow_t1f_vsLSBlock.png");

  ///////////////////////////////////////
  ///Type 1 residual
  Canvas.Clear();
  gT1ResidVsPCC->GetYaxis()->SetTitle("Type 1 Afterglow Residual");
  gT1ResidVsPCC->GetXaxis()->SetTitle("Average Raw PCC per BCID");
  gT1ResidVsPCC->GetYaxis()->SetRangeUser(-0.0001,0.0001);
  gT1ResidVsPCC->SetMarkerStyle(8);
  gT1ResidVsPCC->SetMarkerSize(0.6);
  gT1ResidVsPCC->Draw("ap");
  drawCMSPrelim();
  drawFillYear(0,2022);
  drawPCCLuminometer();
  Canvas.Print(outpath+"/afterglow_t1res_vsinstlumi.png");

  Canvas.Clear();
  gT1ResidVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gT1ResidVsIOV->GetYaxis()->SetTitle("Type 1 Afterglow Residual");
  gT1ResidVsIOV->GetYaxis()->SetRangeUser(-0.0001,0.0001);
  gT1ResidVsIOV->SetMarkerStyle(8);
  gT1ResidVsIOV->SetMarkerSize(0.6);
  gT1ResidVsIOV->Draw("ap");
  drawCMSPrelim();
  drawFillYear(0,2022);
  drawPCCLuminometer();
  Canvas.Print(outpath+"/afterglow_t1res_vsLSBlock.png");


  ////////////////////////
  ///Type 2 residual
  Canvas.Clear();
  gT2ResidVsPCC->GetYaxis()->SetTitle("Type 2 Afterglow Residual");
  gT2ResidVsPCC->GetXaxis()->SetTitle("Average Raw PCC per BCID");
  gT2ResidVsPCC->GetYaxis()->SetRangeUser(0,0.005);
  gT2ResidVsPCC->SetMarkerStyle(8);
  gT2ResidVsPCC->SetMarkerSize(0.6);
  gT2ResidVsPCC->Draw("ap");
  drawCMSPrelim();
  drawFillYear(0,2022);
  drawPCCLuminometer();
  Canvas.Print(outpath+"/afterglow_t2res_vsinstlumi.png");

  Canvas.Clear();
  gT2ResidVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gT2ResidVsIOV->GetYaxis()->SetTitle("Type 2 Afterglow Residual");
  gT2ResidVsIOV->GetYaxis()->SetRangeUser(0,0.005);
  gT2ResidVsIOV->SetMarkerStyle(8);
  gT2ResidVsIOV->SetMarkerSize(0.6);
  gT2ResidVsIOV->Draw("ap");
  drawCMSPrelim();
  drawFillYear(0,2022);
  drawPCCLuminometer();
  Canvas.Print(outpath+"/afterglow_t2res_vsLSBlock.png");




  /////////Pedestal
  Canvas.Clear();
  gPedVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gPedVsIOV->GetYaxis()->SetTitle("Pedestal");
  //gPedVsIOV->GetYaxis()->SetRangeUser(0,0.005);
  gPedVsIOV->SetMarkerStyle(8);
  gPedVsIOV->SetMarkerSize(0.6);
  gPedVsIOV->Draw("ap");
  drawCMSPrelim();
  drawFillYear(0,2022);
  drawPCCLuminometer();
  Canvas.Print(outpath+"/afterglow_pedestal_vsLSBlock.png");

  /////////Avg Colliding 
  Canvas.Clear();
  gAvgVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gAvgVsIOV->GetYaxis()->SetTitle("Avg. PCC");
  //gAvgVsIOV->GetYaxis()->SetRangeUser(0,0.005);
  gAvgVsIOV->SetMarkerStyle(8);
  gAvgVsIOV->SetMarkerSize(0.6);
  gAvgVsIOV->Draw("ap");
  drawCMSPrelim();
  drawFillYear(0,2022);
  drawPCCLuminometer();
  Canvas.Print(outpath+"/afterglow_avgpcc_vsLSBlock.png");

  /////////calculated Bkg from non-colliding bunches
  Canvas.Clear();
  gBkgVsIOV->GetXaxis()->SetTitle("LS Block");
  gBkgVsIOV->GetYaxis()->SetTitle("Background value");
  //gBkgVsIOV->GetYaxis()->SetRangeUser(0,0.005);
  gBkgVsIOV->SetMarkerStyle(8);
  gBkgVsIOV->SetMarkerSize(0.6);
  gBkgVsIOV->Draw("ape");
  Canvas.Print(outpath+"/afterglow_background_vsLSBlock.png");

  
}


 
