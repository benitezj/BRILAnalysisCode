#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"
#include "rootlogon.C"

float maxLumi=30000;
float ratiomin=0.95;
float ratiomax=1.15;
float refLumi[NLS];
//char refname[100];

bool resumBXLumi=1;

bool perBXRatioPlots=0;
float refLumiBX[NLS][NBX];
float maxLumiBX=12;

int linMinLS=100;
int linMaxLS=200;

TString refname="BCM1FuTCA";//"HFET";

void getRefLumi(TString refpath, int Run){

  for(int i=0;i<NLS;i++){
    refLumi[i]=0.;
    for(int j=0;j<NBX;j++)
      refLumiBX[i][j]=0.;
  }

  ifstream myfile((refpath+"/"+Run+"."+refname).Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<refpath.Data()<<" "<<Run<<std::endl;
    return;
  }

  std::string line;
  int run=0;
  int ls=0;
  int bx=0;
  string tmp;
  float lumiBX;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);
    //370093 9029 1 1 07/03/23 23 52 21 STABLE BEAMS 6800 245021.818361167 200655.900251765 31.9 HFET  1 -0.020900944 -0.017116426 2 -0.022822392 -0.018689959
    //iss>>run>>tmp>>ls>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>refLumi[ls]>>tmp>>tmp>>tmp;
    //370095 9029 1 1 1688439259 STABLE BEAMS 6800 19654.256852600 18307.908974516 59.6 BCM1FUTCA
    iss>>run>>tmp>>ls>>tmp>>tmp>>tmp>>tmp>>tmp>>refLumi[ls]>>tmp>>tmp>>tmp;
    
    if(run!=Run)continue;
    
    //refLumi[ls]/=23.31;
    //cout<<"Ref: "<<run<<" "<<ls<<" "<<refLumi[ls]<<" "<<refname<<endl;
    if(perBXRatioPlots){
      for(int j=0;j<NBX;j++){
	iss>>bx>>lumiBX>>tmp;
	if(bx!=j+1) continue;
	//lumiBX/=23.31;
	//cout<<run<<" "<<ls<<" "<<j<<" "<<lumiBX<<endl;
	refLumiBX[ls][j]=lumiBX;
	//HRefLumiBXvsLS.SetBinContent(ls,j+1,lumiBX);
      }
    }
  }

  cout<<"Done reading the ref lumi."<<endl;
}



void plotPCCcsv(TString inpath, long Run, TString outpath="."){

  rootlogon();

  ///Open the lumi csv file
  TString infile=inpath+"/"+Run+".csv";
  ifstream myfile (infile.Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<infile.Data()<<endl; 
    return;
  }

  ///read the reference lumi
  getRefLumi(inpath,Run);


  float sigmavis=getSigmaVis(Run);  if(sigmavis==0) return;
  

  ///create histograms
  TH2F HLumiBXvsLS("HLumiBXvsLS","",NLS,0.5,NLS+0.5,NBX,0.5,NBX+0.5);
  TH1F HLumiLS("HLumiLS","",NLS,0.5,NLS+0.5);
  TH1F HLumiLSRef("HLumiLSRef","",NLS,0.5,NLS+0.5);
  TH1F HLumiLSRatio("HLumiLSRatio","",NLS,0.5,NLS+0.5);
  TH1F HLumiBX("HLumiBX","",NBX,0.5,NBX+0.5);
  TH1F HLumiBXRatio("HLumiBXRatio","",NBX,0.5,NBX+0.5);

  TH1F HRefLumiBX("HRefLumiBX","",NBX,0.5,NBX+0.5);
  //TH2F HRefLumiBXvsLS("HRefLumiBXvsLS","",NLS,0.5,NLS+0.5,NBX,0.5,NBX+0.5);

  //for linearity 
  //TH2F HLinearityRatio("HLinearityRatio","",50,0,10,50,0.95,1.05);
  TGraphErrors GLinearityRatio;
  
  std::string line;
  int run=0;
  int ls=0;
  double rawL=0.;
  double lsL=0; //lumi for given LS
  double bxL=0.;
  int maxLS=0;  //find last LS with lumi
  float maxL=0.;
  float runL=0.;
  float runLRef=0.;
  int Nls=0;
  int Ncoll=0;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);
    std::string token;

    ///read the run number
    std::getline(iss,token, ',');
    std::stringstream runiss(token);
    runiss>>run;

    ///read the LS
    std::getline(iss,token, ',');
    std::stringstream lsiss(token);
    lsiss>>ls;
    if(ls<0 || ls>=NLS){
      std::cout<<"lumi section out of bounds"<<std::endl;
      return;
    }

    ///read the  Lumi per ls
    std::getline(iss,token, ',');
    std::stringstream totLiss(token);
    totLiss>>rawL;
    lsL = rawL/sigmavis;
    
    ///fill lumi per BX plots
    unsigned ncoll=0;
    float totLBX=0.;
    if(perBXRatioPlots || resumBXLumi) { 
      for(int bx=0;bx<NBX;bx++){
	std::getline(iss,token, ',');
	std::stringstream bxLiss(token);
	bxLiss>>rawL;
	bxL = rawL/sigmavis;
	totLBX+=bxL;
	if(perBXRatioPlots){
	  HLumiBX.AddBinContent(bx+1,bxL);
	  HRefLumiBX.AddBinContent(bx+1,refLumiBX[ls][bx]);
	}
	if(rawL>0.5) ncoll++;
      }
    }

    if(resumBXLumi) lsL=totLBX;

    
    ////fill lumi per LS plots
    if(HLumiLS.GetBinContent(ls)>0)
      cout<<run<<","<<ls<<"  duplicated"<<endl;    
    HLumiLS.SetBinContent(ls,lsL);
        
    HLumiLSRef.SetBinContent(ls,refLumi[ls]);

    if(linMinLS<ls && ls<linMaxLS){
      //HLinearityRatio.Fill(lsL/ncoll,lsL/refLumi[ls]);
      GLinearityRatio.SetPoint(GLinearityRatio.GetN(),lsL/ncoll,lsL/refLumi[ls]);
    }
    
    if(ls > maxLS) maxLS=ls;
    if(lsL > maxL) maxL=lsL;

    Nls++;
    Ncoll+=ncoll;
  }
  cout<<endl;
  
  cout<<"Done processing input file: "<<Run<<endl;
  cout<<"NLS : "<<Nls<<endl;
  cout<<"max lumi : "<<maxL<<endl;
  cout<<"n colliding : "<<Ncoll/Nls<<endl;
  
  ///close files
  myfile.close();


  /////////////////////////////////////////////////////
  ///   make the plots
  ///////////////////////////////////////////////////
  gStyle->SetOptStat(0);
  TCanvas C;


  ////// Lumi vs ls 
  TLegend leg(0.8,0.8,0.95,0.98);
  TPad can_1("can_1", "can_1", 0.0, 0.5, 1.0, 1.0);
  can_1.SetTopMargin(0.02);
  can_1.SetBottomMargin(0.15);
  can_1.SetLeftMargin(0.1);
  can_1.SetRightMargin(0.05);
  can_1.SetFrameBorderMode(0);
  can_1.cd();
  HLumiLS.GetXaxis()->SetTitle("lumi section");
  HLumiLS.GetYaxis()->SetTitle("Lumi");
  HLumiLS.GetXaxis()->SetRangeUser(0,maxLS);
  HLumiLS.GetXaxis()->SetLabelSize(0.05);
  HLumiLS.GetXaxis()->SetTitleSize(0.07);
  HLumiLS.GetXaxis()->SetTitleOffset(0.9);
  HLumiLS.GetYaxis()->SetLabelSize(0.04);
  HLumiLS.GetYaxis()->SetTitleSize(0.06);
  HLumiLS.GetYaxis()->SetTitleOffset(0.6);
  HLumiLS.GetYaxis()->SetRangeUser(0,maxLumi);
  HLumiLS.GetYaxis()->SetNdivisions(5);
  HLumiLS.SetMarkerStyle(8);
  HLumiLS.SetMarkerSize(0.3);
  HLumiLS.Draw("histp");
  leg.AddEntry(&HLumiLS,"PCC","p");
  if(HLumiLSRef.GetEntries()>0){
    HLumiLSRef.SetMarkerStyle(8);
    HLumiLSRef.SetMarkerSize(0.3);
    HLumiLSRef.SetMarkerColor(4);
    HLumiLSRef.Draw("histpsame");
    leg.AddEntry(&HLumiLSRef,refname,"p");
  }
  leg.Draw();
  TLatex text;
  text.SetTextSize(0.06);
  text.DrawLatexNDC(0.12,0.9,TString("Run ")+(long)Run);


 
  ///////Ratio plot
  TLine tline;  
  TPad can_2("can_2", "can_2",0.0, 0.0, 1.0, 0.5);
  can_2.SetTopMargin(0.02);
  can_2.SetBottomMargin(0.15);
  can_2.SetLeftMargin(0.1);
  can_2.SetRightMargin(0.05);
  can_2.SetFrameBorderMode(0);
  can_2.cd();
  if(HLumiLSRef.GetMaximum()>0){
    HLumiLSRatio.Add(&HLumiLS);
    HLumiLSRatio.Divide(&HLumiLSRef);
    HLumiLSRatio.GetXaxis()->SetTitle(HLumiLS.GetXaxis()->GetTitle());
    HLumiLSRatio.GetYaxis()->SetTitle(TString(" PCC / ")+refname);
    HLumiLSRatio.GetXaxis()->SetRangeUser(0,maxLS);
    HLumiLSRatio.GetYaxis()->SetRangeUser(ratiomin,ratiomax);
    HLumiLSRatio.GetXaxis()->SetLabelSize(0.05);
    HLumiLSRatio.GetXaxis()->SetTitleSize(0.07);
    HLumiLSRatio.GetXaxis()->SetTitleOffset(0.9);
    HLumiLSRatio.GetYaxis()->SetLabelSize(0.04);
    HLumiLSRatio.GetYaxis()->SetTitleSize(0.06);
    HLumiLSRatio.GetYaxis()->SetTitleOffset(0.6);
    HLumiLSRatio.GetYaxis()->SetNdivisions(5);
    HLumiLSRatio.SetMarkerStyle(8);
    HLumiLSRatio.SetMarkerSize(0.3);
    HLumiLSRatio.SetMarkerColor(2);
    HLumiLSRatio.Draw("histpsame");
    text.DrawLatexNDC(0.12,0.9,TString("Run ")+(long)Run);
    tline.SetLineColor(1);
    tline.SetLineStyle(4);
    tline.DrawLine(0,1,maxLS,1);
  }

  
  C.Clear();
  can_1.Draw();
  can_2.Draw();
  C.Print(outpath+"/"+(long)Run+".png");




  //////////////////////////////////////////
  //// per BX ratio plots, and linearity plots
  //////////////////////////////////////////
  if(perBXRatioPlots) {
    can_1.cd();
    can_1.Clear();
    if(Nls>0) HLumiBX.Scale(1./Nls);
    HLumiBX.SetMarkerStyle(8);
    HLumiBX.SetMarkerSize(0.5);
    HLumiBX.GetYaxis()->SetTitle("Avg. Lumi");
    HLumiBX.GetXaxis()->SetTitle("BCID");
    HLumiBX.GetYaxis()->SetRangeUser(0,maxLumiBX);
    HLumiBX.Draw("histp");

    TLegend legbx(0.8,0.8,0.95,0.98);
    legbx.AddEntry(&HLumiBX,"PCC","p");
    
    if(HRefLumiBX.GetMaximum()>0){
      if(Nls>0) HRefLumiBX.Scale(1./Nls);
      HRefLumiBX.SetMarkerStyle(8);
      HRefLumiBX.SetMarkerSize(0.3);
      HRefLumiBX.SetMarkerColor(4);
      HRefLumiBX.Draw("histpsame");
      legbx.AddEntry(&HRefLumiBX,refname,"p");
    }

    legbx.Draw();
    //text.DrawLatexNDC(0.75,0.85,TString("# colliding ")+(long)(Ncoll/Nls));

    can_2.cd();
    can_2.Clear();
    if(HRefLumiBX.GetMaximum()>0){
      TH1F* HLumiBXRatio=(TH1F*)HLumiBX.Clone("HLumiBXRatio");
      HLumiBXRatio->GetXaxis()->SetTitle("BCID");
      HLumiBXRatio->SetMarkerColor(2);

//     ///ratio
//     HLumiBXRatio->Divide(&HRefLumiBX);
//     HLumiBXRatio->GetYaxis()->SetTitle(TString(" PCC / ")+refname);
//     HLumiBXRatio->GetYaxis()->SetRangeUser(ratiomin,ratiomax);
//     HLumiBXRatio->Draw("histp");
//     tline.DrawLine(0,1,NBX,1);

      //difference
      HLumiBXRatio->Add(&HRefLumiBX,-1);
      HLumiBXRatio->GetYaxis()->SetTitle(TString(" PCC - ")+refname);
      //HLumiBXRatio->GetYaxis()->SetRangeUser(-0.1*maxLumiBX,0.1*maxLumiBX);
      HLumiBXRatio->Draw("histp");
      tline.DrawLine(0,0,NBX,0);

      
    }

    
    TCanvas C2("C2","",1200,800);
    C2.Clear();
    can_1.Draw();
    can_2.Draw();
    C2.Print(outpath+"/"+(long)Run+"_bx.png");
  }


  ////linearity analysis
//  C.Clear();
//  HLinearityRatio.GetYaxis()->SetTitle(TString(" PCC / ")+refname);
//  HLinearityRatio.GetXaxis()->SetTitle("SBIL");
//  HLinearityRatio.Draw("colz");
//  C.Print(outpath+"/"+(long)Run+"_linearity.png");
//
//
//  C.Clear();
//  TProfile* PLinearity = HLinearityRatio.ProfileX();
//  PLinearity->GetYaxis()->SetRangeUser(0.95,1.05);
//  PLinearity->Draw("histpe");
//  tline.DrawLine(0,1,10,1);
//  C.Print(outpath+"/"+(long)Run+"_linearity_profile.png");

  C.Clear();
  GLinearityRatio.GetYaxis()->SetTitle(TString(" PCC / ")+refname);
  GLinearityRatio.GetXaxis()->SetTitle("SBIL");
  GLinearityRatio.GetYaxis()->SetRangeUser(0.97,1.03);
  GLinearityRatio.SetMarkerStyle(8);
  GLinearityRatio.SetMarkerSize(0.5);
  GLinearityRatio.Draw("ap");
  tline.DrawLine(0,1,10,1);
  TF1 FLin("FLin","[0]+[1]*x",4,10);
  FLin.SetLineColor(2);
  GLinearityRatio.Fit(&FLin,"","Q",4.5,10);
  text.SetTextSize(0.04);
  text.SetTextColor(1);
  text.DrawLatexNDC(0.2,0.93,TString("Run ")+(long)Run);
  char linresults[100];
  sprintf(linresults,"p0 = %.4f  ,  p1 = %.5f",FLin.GetParameter(0),FLin.GetParameter(1));
  text.SetTextColor(2);
  text.DrawLatexNDC(0.5,0.93,linresults);
  C.Print(outpath+"/"+(long)Run+"_linearity_tgraph.png");

  
  
  gROOT->ProcessLine(".q");
}

