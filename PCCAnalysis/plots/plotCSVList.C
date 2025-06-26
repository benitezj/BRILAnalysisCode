#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"

bool perBXRatioPlots=1;
float ratiomin=0.5;
float ratiomax=1.5;

//set to 0 to retrieve nominal value
float sigmavis=0;
//sigmavis=1.24*4.1533e6/ORBITF; 
//sigmavis=0.9807*0.38*4.1533e6/ORBITF;  

float refLumi[NLS];
TH2F HRefLumiBXvsLS("HRefLumiBXvsLS","",NLS,0.5,NLS+0.5,NBX,0.5,NBX+0.5);

void getRefLumi(TString inputfile){

  for(int i=0;i<NLS;i++)  refLumi[i]=0.;

  ifstream myfile(inputfile.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<inputfile.Data()<<std::endl;
    return;
  }

  cout<<"Opened Ref lumi: "<<inputfile<<endl;

  std::string line;
  int run=0;
  int ls=0;
  string tmp;

  while (std::getline(myfile, line)){
    std::stringstream iss(line);

    //369802:8999,3791:3791,06/29/23 22:19:35,STABLE BEAMS,6800,1.075448824,1.067225496,0.1,HFET
    string field;
    int pos=0;
    while (getline(iss, field, ',' )){
	stringstream fs( field );
	fs >> tmp;
	if(pos==0){ string tmprun; stringstream tmpfs(tmp); getline(tmpfs,tmprun,':'); run=atoi(tmprun.c_str()); } //get run number
	if(pos==1){ string tmpls; stringstream tmpfs(tmp); getline(tmpfs,tmpls,':'); ls=atoi(tmpls.c_str()); } //get ls number
	if(pos==5)refLumi[ls]=atof(tmp.c_str());
	pos++;
    }


    ///old data
    //325310 7358 2 2 10/26/18 07 27 01 STABLE BEAMS 6500 8368.161 6041.397 98.2 HFOC 1 0.0760 0.0549 ...
    //iss>>run>>tmp>>ls>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp;//>>tmp;
    //iss>>refLumi[ls];
    //iss>>tmp>>tmp;

    //cout<<run<<" "<<ls<<" "<<refLumi[ls]<<endl;
  }


}


void plotCSVList(TString inpath, TString outpath=".", std::string runlist="",TString REF=""){

  ///create output file for lumisections
  TString lsoutfile=outpath+"/ls.dat";
  ofstream lsfile(lsoutfile.Data());
  if (!lsfile.is_open()){
    cout << "Unable to open output run file"; 
    return;
  }


  std::stringstream ss(runlist.c_str());
  int Run;
  while (ss >> Run) {
    cout<<Run<<endl;
    
    ///Open the lumi csv file
    TString infile=inpath+"/"+Run+".csv";
    ifstream myfile (infile.Data());
    if (!myfile.is_open()){
      cout << "Unable to open file: "<<infile.Data()<<endl; 
      ss.ignore(1); 
      continue;
    }
    

    ///read the reference lumi
    if(REF.CompareTo("")!=0) getRefLumi(inpath+"/"+Run+"."+REF);
    
    ///create histograms
    TH2F HLumiBXvsLS("HLumiBXvsLS","",NLS,0.5,NLS+0.5,NBX,0.5,NBX+0.5);
    TH1F HLumiLS("HLumiLS","",NLS,0.5,NLS+0.5);
    TH1F HLumiLSRef("HLumiLSRef","",NLS,0.5,NLS+0.5);
    TH1F HLumiLSRatio("HLumiLSRatio","",NLS,0.5,NLS+0.5);
    TH1F HLumiBX("HLumiBX","",NBX,0.5,NBX+0.5);
    TH1F HLumiBXRatio("HLumiBXRatio","",NBX,0.5,NBX+0.5);
    
    
    std::string line;
    int run=0;
    int ls=0;
    float rawL=0.;
    float lsL=0; //lumi for given LS
    float bxL=0.;
    int maxLS=0;  //find last LS with lumi
    float maxL=0.;
    float runL=0.;
    float runLRef=0.;
    if(sigmavis==0.) sigmavis=getSigmaVis(Run);
    int Nls=0;
    int Ncoll=0;
    while (std::getline(myfile, line)){
      //cout<<line;
      
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

      if(HLumiLS.GetBinContent(ls)>0)
	cout<<run<<","<<ls<<"  duplicated"<<endl;    

  
      ///read the  Lumi per ls
      std::getline(iss,token, ',');
      std::stringstream totLiss(token);
      totLiss>>rawL;
      
      lsL=0.;
      if(sigmavis>0)
	lsL = rawL/sigmavis;
           
      runL+=lsL;
      Nls++;
      
   
      
      ///Lumi per BX
      unsigned ncoll=0;
      if(perBXRatioPlots) { 
	float bxlumi=0.;
	for(int bx=0;bx<NBX;bx++){
	  std::getline(iss,token, ',');
	  std::stringstream bxLiss(token);
	  bxLiss>>rawL;
	  bxL = rawL/sigmavis;
	  bxlumi+=bxL;
	  HLumiBXvsLS.SetBinContent(ls,bx+1,bxL);
	  HLumiBX.AddBinContent(bx+1,bxL);
	  if(rawL>0.5)
	    ncoll++;
	}
	lsL=bxlumi;//temp fix
      }



      //Lumi per LS
      HLumiLS.SetBinContent(ls,lsL);
      if(refLumi[ls]>0){
	HLumiLSRatio.SetBinContent(ls,lsL/refLumi[ls]);
	HLumiLSRef.SetBinContent(ls,refLumi[ls]);
	runLRef+=refLumi[ls];
      }
      
      
      //// write ouput
      lsfile<<Run<<" "<<ls<<" "<<lsL<<" "<<refLumi[ls]<<" "<<ncoll<<std::endl;
      //lsfile<<Run<<" "<<ls<<" "<<lsL<<" "<<bxlumi<<" "<<ncoll<<std::endl;


      
      if(ls > maxLS) maxLS=ls;
      if(lsL > maxL) maxL=lsL;
      Ncoll+=ncoll;
    }
    cout<<endl;
    myfile.close();


    
    /////////////////////////////////////////////////////
    ///   make the plots
    ///////////////////////////////////////////////////
    gStyle->SetOptStat(0);
    TCanvas C;
    
    float MAXPCC=1.4*maxL;
    
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
    HLumiLS.GetYaxis()->SetTitle("integrated lumi [ #mub^{-1} ]");
    HLumiLS.GetXaxis()->SetRangeUser(0,maxLS);
    HLumiLS.GetXaxis()->SetLabelSize(0.05);
    HLumiLS.GetXaxis()->SetTitleSize(0.07);
    HLumiLS.GetXaxis()->SetTitleOffset(0.9);
    HLumiLS.GetYaxis()->SetLabelSize(0.04);
    HLumiLS.GetYaxis()->SetTitleSize(0.06);
    HLumiLS.GetYaxis()->SetTitleOffset(0.6);
    HLumiLS.GetYaxis()->SetRangeUser(0,MAXPCC);
    HLumiLS.GetYaxis()->SetNdivisions(5);
    HLumiLS.SetMarkerStyle(8);
    HLumiLS.SetMarkerSize(0.3);
    HLumiLS.Draw("histp");
    leg.AddEntry(&HLumiLS,"pcc","p");
    if(HLumiLSRef.GetEntries()>0){
      HLumiLSRef.SetMarkerStyle(8);
      HLumiLSRef.SetMarkerSize(0.3);
      HLumiLSRef.SetMarkerColor(4);
      HLumiLSRef.Draw("histpsame");
      leg.AddEntry(&HLumiLSRef,REF,"p");
    }
    leg.Draw();
    TLatex text;
    text.SetTextSize(0.06);
    text.DrawLatexNDC(0.12,0.9,TString("Run ")+(long)Run);
    
    
    
    ///////Ratio plot
    
    TPad can_2("can_2", "can_2",0.0, 0.0, 1.0, 0.5);
    can_2.SetTopMargin(0.02);
    can_2.SetBottomMargin(0.15);
    can_2.SetLeftMargin(0.1);
    can_2.SetRightMargin(0.05);
    can_2.SetFrameBorderMode(0);
    can_2.cd();
    if(HLumiLSRef.GetEntries()>0){
      HLumiLSRatio.GetXaxis()->SetTitle("lumi section");
      HLumiLSRatio.GetYaxis()->SetTitle(TString("ratio"));
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
      
      TLine tline;  
      tline.SetLineColor(2);
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
    
      
      TCanvas C2("C2","",1200,400);
      C2.Clear();
      if(Nls>0) HLumiBX.Scale(1./Nls);
      HLumiBX.SetMarkerStyle(8);
      HLumiBX.SetMarkerSize(0.5);
      HLumiBX.GetYaxis()->SetTitle("Avg. pcc");
      HLumiBX.GetXaxis()->SetTitle("bcid");
      HLumiBX.GetYaxis()->SetRangeUser(0,HLumiBX.GetMaximum()*1.3);
      HLumiBX.Draw("histp");
      text.DrawLatexNDC(0.75,0.85,TString("# colliding ")+(long)(Ncoll/Nls));
      C2.Print(outpath+"/"+(long)Run+"_AvgLumiBX.png");
      
      
    }


    
    ss.ignore(1);
  }
  
  lsfile.close();
  gROOT->ProcessLine(".q");
}
