#include <iostream>
#include <vector>
#include <dirent.h>
#include "globals.h"
#include "rootlogon.C"

TString outpath=".";
float MinPCCColliding=100;

void plotlabels(){
  //drawCMSPrelim();
  drawFillYear(0,2017);
  //drawPCCLuminometer();
}


float getNColl(TFile*F=NULL, int Run=-1, int I=-1,int type=0){
  if(F==NULL || Run<0 || I<0) return 0;
  float ncoll=0.;
  float sf=0.;
  TIter next(F->GetListOfKeys());
  TObject* key;
  while ((key = next())) {
    TString kname(key->GetName());    
    if(!kname.Contains("ScaleFactorsAvg_")) continue; //ScaleFactorsAvg_370093_7_351_400-
    TObjArray * a = kname.Tokenize("_");
    long r=atoi(((TObjString*)(*a)[1])->GetName());
    long l=atoi(((TObjString*)(*a)[2])->GetName());
    if(r!=Run || l!=I) continue; //select the desired block
    TH1F* HIST = (TH1F*) F->Get(kname.Data());
    if(!HIST) continue;
    for(int j = 1; j <= HIST->GetNbinsX(); j++){ 
      if(HIST->GetBinContent(j)>0.7){
	ncoll++;
	if(type==1)
	  sf+=HIST->GetBinContent(j);
      }
    }

    ///if(ncoll<2000) cout<<"ncoll="<<ncoll<<" : "<<kname<<endl;
  }
  
  if(type==1) return sf/ncoll; 
  return ncoll;
}



std::pair<float,float> getPCCAvg(TFile*F=NULL, int Run=-1, int I=-1, int type=1){
  if(F==NULL || Run<0 || I<0) return std::pair<float,float>();
  
  std::pair<float,float> bincontent_avg;
  bincontent_avg.first=0.;
  bincontent_avg.second=0.;
 

  TIter next(F->GetListOfKeys());
  TObject* key;
  while ((key = next())) {
    TString kname(key->GetName());    //TString("RawHISTAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    if(!kname.Contains("RawLumiAvg_")) continue;
    
    TObjArray * a = kname.Tokenize("_");
    long r=atoi(((TObjString*)(*a)[1])->GetName());
    long l=atoi(((TObjString*)(*a)[2])->GetName());
    long ls1=atoi(((TObjString*)(*a)[3])->GetName());
    long ls2=atoi(((TObjString*)(*a)[4])->GetName());
  
    if(r!=Run || l!=I) continue; //select the desired block
    
    TH1F* Lumi = (TH1F*) F->Get(kname.Data());
    if(!Lumi)      continue;

    ///Average PCC from colliding bx
    if(type==1){
      int nbincounter=0;
      float bincontent_total=0.;
      for(int j = 1; j <= Lumi->GetNbinsX(); j++) {
	float bincontent = Lumi->GetBinContent(j);
	if (bincontent > MinPCCColliding){
	  bincontent_total += bincontent;
	  nbincounter++;
	}
      }
      if(nbincounter>0)
	bincontent_avg.first =bincontent_total/nbincounter;
    }


    ///background from non-colliding bx after train
    if(type==2){  
      int afterglowcounter=0;
      int nbincounter=0;
      float bincontent_total=0;
      int NAfterGlowBX=15;  
      
      for(int j = 1; j <= Lumi->GetNbinsX(); j++) {
	float bincontent = Lumi->GetBinContent(j);
	afterglowcounter++;
	if(bincontent > MinPCCColliding)
	  afterglowcounter=0;
	if(afterglowcounter>NAfterGlowBX && bincontent>0){
	  bincontent_total += bincontent;
	  nbincounter++;
	}
      }
      
      if(nbincounter>0){//calcuate average bkg and unc.
	bincontent_avg.first = bincontent_total / nbincounter;

	afterglowcounter=0;
	bincontent_total=0.;
	for(int j = 1; j <= Lumi->GetNbinsX(); j++) {
	  float bincontent = Lumi->GetBinContent(j);
	  afterglowcounter++;
	  if(bincontent > MinPCCColliding)
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

void plot_AfterglowResidual(TString inpath, std::string inputfiles="CorrectionHisto_1.root") {

  rootlogon();
    
  cout<<"input path: "<<inpath<<endl;
  cout<<"file list: "<<inputfiles<<endl;
  
  TGraph* gAvgVsIOV=new TGraph();
  TGraph* gT1fracVsPCC=new TGraph();
  TGraph* gT1fracVsIOV=new TGraph();
  TGraph* gT1ResidVsPCC=new TGraph();
  TGraph* gT1ResidVsIOV=new TGraph();
  TGraph* gT2ResidVsPCC=new TGraph();
  TGraph* gT2ResidVsIOV=new TGraph();
  TGraph* gPedVsIOV=new TGraph();
  TGraphErrors* gBkgVsIOV=new TGraphErrors();
  TGraph* gNCollVsIOV=new TGraph();
  TGraph* gAvgSFVsIOV=new TGraph();

  ////////////////////
  string file;
  istringstream iss(inputfiles.c_str());
  while (getline(iss, file, ',')) {
    cout<<"Processing: "<<file.c_str()<<endl;

    if(!TString(file).Contains(".root")) continue;
    
    TString filename=inpath+"/"+file.c_str();
    if(gSystem->AccessPathName(filename.Data())){ cout<<"bad file: "<<filename<<endl; continue;}


    TFile InputFile(filename.Data(),"read");
    if(InputFile.IsZombie()) continue;
    //InputFile.ls();


    TGraphErrors* gT1resid = (TGraphErrors*)InputFile.Get("Type1Res");
    if(!gT1resid) continue;
    double *YT1resid = gT1resid->GetY();	
    for (int l=0; l<gT1resid->GetN(); l++) {
      if(YT1resid[l]>0)
	gT1ResidVsIOV->SetPoint(gT1ResidVsIOV->GetN(),gT1ResidVsIOV->GetN(), YT1resid[l]);
    }

    TGraphErrors* gT2resid = (TGraphErrors*)InputFile.Get("Type2Res");
    if(!gT2resid) continue;
    double *YT2resid = gT2resid->GetY();	
    for (int l=0; l<gT2resid->GetN(); l++) {
      if(YT2resid[l]>0)
	gT2ResidVsIOV->SetPoint(gT2ResidVsIOV->GetN(),gT2ResidVsIOV->GetN(), YT2resid[l]);
    }

    TGraphErrors* gT1frac = (TGraphErrors*)InputFile.Get("Type1Fraction");
    if(!gT1frac) continue;
    double *YT1frac = gT1frac->GetY();
    for (int l=0; l<gT1frac->GetN(); l++) {
      if(YT1frac[l]>0)
	gT1fracVsIOV->SetPoint(gT1fracVsIOV->GetN(),gT1fracVsIOV->GetN(), YT1frac[l]); 
    }

    
//    TIter next(InputFile.GetListOfKeys());
//    TObject* key;
//    while ((key = next())) {
//      TString kname(key->GetName());    
//
//      if(kname.Contains("RawLumiAvg_")){
//	 TH1F*h=(TH1F*)InputFile.Get(kname);
//	 cout<<kname<<" : "<<h->Integral()<<endl;
//      }
//    }

    
    
    
//    TGraphErrors* gT1frac = (TGraphErrors*)InputFile.Get("Type1Fraction");
//    TGraphErrors* gT1resid = (TGraphErrors*)InputFile.Get("Type1Res");
//    TGraphErrors* gT2resid = (TGraphErrors*)InputFile.Get("Type2Res"); 
//    TGraphErrors* gPed = (TGraphErrors*)InputFile.Get("Pedestal");
//    if(!gT1frac || !gT2resid || !gT2resid){cout<<" objects  not found. run="<<Run<<endl; continue;}
//
//    double *YT1frac = gT1frac->GetY();
//    double *YT1resid = gT1resid->GetY();
//    double *YT2resid = gT2resid->GetY();
//    double *YPed = gPed->GetY();
//    for (int l = 0; l < gT1frac->GetN(); l++) {
//      cout<<gAvgVsIOV->GetN()<<",";  
//      std::pair<float,float> pccavg=getPCCAvg(&InputFile,Run,l);
//      if(pccavg.first>MinPCCColliding){
//	gAvgVsIOV->SetPoint(gAvgVsIOV->GetN(),gAvgVsIOV->GetN(),pccavg.first);
//	gT1fracVsPCC->SetPoint(gT1fracVsPCC->GetN(),pccavg.first, YT1frac[l]); 
//	gT1fracVsIOV->SetPoint(gT1fracVsIOV->GetN(),gT1fracVsIOV->GetN(), YT1frac[l]); 
//	gT1ResidVsPCC->SetPoint(gT1ResidVsPCC->GetN(),pccavg.first, YT1resid[l]); 
//	gT1ResidVsIOV->SetPoint(gT1ResidVsIOV->GetN(),gT1ResidVsIOV->GetN(), YT1resid[l]); 
//	gT2ResidVsPCC->SetPoint(gT2ResidVsPCC->GetN(),pccavg.first, YT2resid[l]); 
//	gT2ResidVsIOV->SetPoint(gT2ResidVsIOV->GetN(),gT2ResidVsIOV->GetN(), YT2resid[l]);
//	gPedVsIOV->SetPoint(gPedVsIOV->GetN(),gPedVsIOV->GetN(), YPed[l]/pccavg.first); 
//	
//      }
//      
//      gNCollVsIOV->SetPoint(gNCollVsIOV->GetN(),gNCollVsIOV->GetN(),getNColl(&InputFile,Run,l));
//      gAvgSFVsIOV->SetPoint(gAvgSFVsIOV->GetN(),gAvgSFVsIOV->GetN(),getNColl(&InputFile,Run,l,1));
//    }

    
  }


  ////////////////////
  TCanvas Canvas;//("Canvas"," ", 0,67,1264,678);
  TLine line;

  ///////////////////////////////////
  ///type 1 fraction
//  Canvas.Clear();
//  gT1fracVsPCC->GetYaxis()->SetTitle("Type 1 Afterglow Fraction");
//  gT1fracVsPCC->GetXaxis()->SetTitle("Average Raw PCC per BCID");
//  gT1fracVsPCC->GetYaxis()->SetRangeUser(0.,0.04);
//  gT1fracVsPCC->SetMarkerStyle(8);
//  gT1fracVsPCC->SetMarkerSize(0.6);
//  gT1fracVsPCC->Draw("ap");
//  plotlabels();
//  line.DrawLine(gT1fracVsPCC->GetXaxis()->GetXmin(),0,gT1fracVsPCC->GetXaxis()->GetXmax(),0);
//  Canvas.Print(outpath+"/afterglow_t1f_vsinstlumi.png");

  Canvas.Clear();
  gT1fracVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gT1fracVsIOV->GetYaxis()->SetTitle("Type 1 Afterglow Fraction");
  gT1fracVsIOV->GetYaxis()->SetRangeUser(0,0.05);
  gT1fracVsIOV->SetMarkerStyle(8);
  gT1fracVsIOV->SetMarkerSize(0.6);
  gT1fracVsIOV->Draw("ap");
  plotlabels();
  line.DrawLine(gT1fracVsIOV->GetXaxis()->GetXmin(),0,gT1fracVsIOV->GetXaxis()->GetXmax(),0);
  Canvas.Print(outpath+"/afterglow_t1f_vsLSBlock.png");

  ///////////////////////////////////////
//  ///Type 1 residual
//  Canvas.Clear();
//  gT1ResidVsPCC->GetYaxis()->SetTitle("Type 1 Afterglow Residual");
//  gT1ResidVsPCC->GetXaxis()->SetTitle("Average Raw PCC per BCID");
//  gT1ResidVsPCC->GetYaxis()->SetRangeUser(-0.0001,0.0001);
//  gT1ResidVsPCC->SetMarkerStyle(8);
//  gT1ResidVsPCC->SetMarkerSize(0.6);
//  gT1ResidVsPCC->Draw("ap");
//  plotlabels();
//  line.DrawLine(gT1ResidVsPCC->GetXaxis()->GetXmin(),0,gT1ResidVsPCC->GetXaxis()->GetXmax(),0);
//  Canvas.Print(outpath+"/afterglow_t1res_vsinstlumi.png");

  Canvas.Clear();
  gT1ResidVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gT1ResidVsIOV->GetYaxis()->SetTitle("Type 1 Afterglow Residual");
  gT1ResidVsIOV->GetYaxis()->SetRangeUser(-0.0001,0.0001);
  gT1ResidVsIOV->SetMarkerStyle(8);
  gT1ResidVsIOV->SetMarkerSize(0.6);
  gT1ResidVsIOV->Draw("ap");
  plotlabels();
  line.DrawLine(gT1ResidVsIOV->GetXaxis()->GetXmin(),0,gT1ResidVsIOV->GetXaxis()->GetXmax(),0);
  Canvas.Print(outpath+"/afterglow_t1res_vsLSBlock.png");


  ////////////////////////
  ///Type 2 residual
//  Canvas.Clear();
//  gT2ResidVsPCC->GetYaxis()->SetTitle("Type 2 Afterglow Residual");
//  gT2ResidVsPCC->GetXaxis()->SetTitle("Average Raw PCC per BCID");
//  gT2ResidVsPCC->GetYaxis()->SetRangeUser(-0.005,0.005);
//  gT2ResidVsPCC->SetMarkerStyle(8);
//  gT2ResidVsPCC->SetMarkerSize(0.6);
//  gT2ResidVsPCC->Draw("ap");
//  plotlabels();
//  line.DrawLine(gT2ResidVsPCC->GetXaxis()->GetXmin(),0,gT2ResidVsPCC->GetXaxis()->GetXmax(),0);
//  Canvas.Print(outpath+"/afterglow_t2res_vsinstlumi.png");

  Canvas.Clear();
  gT2ResidVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gT2ResidVsIOV->GetYaxis()->SetTitle("Type 2 Afterglow Residual");
  gT2ResidVsIOV->GetYaxis()->SetRangeUser(0,0.03);
  gT2ResidVsIOV->SetMarkerStyle(8);
  gT2ResidVsIOV->SetMarkerSize(0.6);
  gT2ResidVsIOV->Draw("ap");
  plotlabels();
  line.DrawLine(gT2ResidVsIOV->GetXaxis()->GetXmin(),0,gT2ResidVsIOV->GetXaxis()->GetXmax(),0);
  Canvas.Print(outpath+"/afterglow_t2res_vsLSBlock.png");


//  /////////Pedestal
//  Canvas.Clear();
//  gPedVsIOV->GetXaxis()->SetTitle("50 LS Block");
//  gPedVsIOV->GetYaxis()->SetTitle("Pedestal fraction [%]");
//  gPedVsIOV->GetYaxis()->SetRangeUser(-0.01,0.01);
//  gPedVsIOV->SetMarkerStyle(8);
//  gPedVsIOV->SetMarkerSize(0.6);
//  gPedVsIOV->Draw("ap");
//  plotlabels();
//  line.DrawLine(gPedVsIOV->GetXaxis()->GetXmin(),0,gPedVsIOV->GetXaxis()->GetXmax(),0);
//  Canvas.Print(outpath+"/afterglow_pedestal_vsLSBlock.png");
//
//
//  /////////Avg Colliding 
//  Canvas.Clear();
//  gAvgVsIOV->GetXaxis()->SetTitle("50 LS Block");
//  gAvgVsIOV->GetYaxis()->SetTitle("Avg. PCC");
//  //gAvgVsIOV->GetYaxis()->SetRangeUser(0,0.005);
//  gAvgVsIOV->SetMarkerStyle(8);
//  gAvgVsIOV->SetMarkerSize(0.6);
//  gAvgVsIOV->Draw("ap");
//  plotlabels();
//  Canvas.Print(outpath+"/afterglow_avgpcc_vsLSBlock.png");
//
//
//  Canvas.Clear();
//  gNCollVsIOV->GetXaxis()->SetTitle("50 LS Block");
//  gNCollVsIOV->GetYaxis()->SetTitle("N colliding");
//  gNCollVsIOV->SetMarkerStyle(8);
//  gNCollVsIOV->SetMarkerSize(0.6);
//  gNCollVsIOV->Draw("ap");
//  plotlabels();
//  Canvas.Print(outpath+"/afterglow_ncoll_vsLSBlock.png");
//
//
//
//
//  Canvas.Clear();
//  gAvgSFVsIOV->GetXaxis()->SetTitle("50 LS Block");
//  gAvgSFVsIOV->GetYaxis()->SetTitle("Avg. Scale Factor");
//  gAvgSFVsIOV->SetMarkerStyle(8);
//  gAvgSFVsIOV->SetMarkerSize(0.6);
//  gAvgSFVsIOV->Draw("ap");
//  plotlabels();
//  Canvas.Print(outpath+"/afterglow_avgsf_vsLSBlock.png");

  
}





 
