#include <iostream>
#include <vector>
#include <dirent.h>
#include "globals.h"


TString pcctitle="Scale Factor"; 
float collidingcountMin=3; //select fills with this number of bunches or larger

int NAfterGlowBX=15;//needed for background estimation from non-colliding bx


std::string runlist="360390,360391,360392,360393,360400,360411,360413,360414,360428,360429,360430,360431,360432,360433,360434,360435,360436,360437,360438,360439,360440,360441,360442,360444,360445,360458,360459,360460,360472,360486,360490,360491,360519,360520,360523,360525,360526,360528,360530,360533,360538,360542,360590,360591,360594,360659,360660,360666,360681,360683,360684,360685,360688,360696,360697,360698,360703,360736,360737,360761,360787,360788,360794,360795,360796,360797,360813,360815,360816,360817,360819,360820,360821,360822,360823,360825,360826,360850,360856,360874,360882,360886,360887,360888,360889,360890,360891,360892,360893,360894,360895,360912,360919,360920,360921,360927,360940,360941,360942,360943,360944,360945,360946,360948,360949,360950,360951,360968,360989,360991,360992,361020,361032,361038,361044,361045,361046,361047,361048,361050,361051,361052,361053,361054,361059,361074,361082,361083,361088,361089,361090,361091,361092,361101,361105,361106,361107,361108,361110,361177,361183,361188,361193,361195,361196,361197,361198,361208,361223,361239,361240,361267,361269,361270,361271,361272,361279,361280,361283,361284,361292,361294,361297,361303,361318,361319,361320,361333,361361,361362,361363,361364,361365,361366,361370,361398,361400,361417,361424,361442,361443,361464,361467,361468,361475,361503,361512,361562,361564,361569,361570,361573,361579,361580,361604,361606,361607,361609,361614,361618,361677,361685,361686,361722,361723,361724,361740,361790,361791,361807,361808,361810,361848,361849,361852,361859,361860,361872,361881,361885,361887,361889,361890,361893,361894,361895,361896,361898,361905,361906,361948,361954,361957,361968,361971,361974,361989,361990,361992,361993,361994,362009,362058,362059,362060,362061,362062,362063,362064,362085,362086,362087,362091,362095,362104,362105,362106,362107,362108,362148,362152,362153,362154,362159,362160,362161,362163,362166,362167";




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

void plot_afterglow_residual(TString inpath, TString outpath) {
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
    ss.ignore(1);   

    TString filename=inpath+"/"+Run+".root";
    if(gSystem->AccessPathName(filename.Data())){ cout<<"bad file: "<<filename<<endl; continue;}

    TFile InputFile(filename.Data(),"read");
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
  gT1ResidVsPCC->GetYaxis()->SetRangeUser(0,0.0001);
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
  gT1ResidVsIOV->GetYaxis()->SetRangeUser(0,0.0001);
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


 
