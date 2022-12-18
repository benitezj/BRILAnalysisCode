#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"

bool perBXRatioPlots=0;
float ratiomin=0.7;
float ratiomax=1.3;


float refLumi[NLS];
TH2F HRefLumiBXvsLS("HRefLumiBXvsLS","",NLS,0.5,NLS+0.5,NBX,0.5,NBX+0.5);
float modfrac[NLS];//correction to visible crossection for applied Pixel Quality flag

void getRefLumi(TString inputfile){

  for(int i=0;i<NLS;i++)  refLumi[i]=0.;

  ifstream myfile(inputfile.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<inputfile.Data()<<std::endl;
    return;
  }

  std::string line;
  int run=0;
  int ls=0;
  string tmp;
  float lumiBX;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);
    //325310 7358 2 2 10/26/18 07 27 01 STABLE BEAMS 6500 8368.161 6041.397 98.2 HFOC 1 0.0760 0.0549 ...
    iss>>run>>tmp>>ls>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp;//>>tmp;
    iss>>refLumi[ls];
    iss>>tmp>>tmp;
    for(int j=0;j<NBX;j++){
      iss>>tmp>>tmp>>lumiBX;
      HRefLumiBXvsLS.SetBinContent(ls,j+1,lumiBX/23.31);
    }
  }

  cout<<"Done reading the ref lumi."<<endl;
}

void getModFrac(TString inputfile){

  for(int i=0;i<NLS;i++) modfrac[i]=1.;

  ifstream myfile(inputfile.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open fraction file: "<<inputfile.Data()<<std::endl;
    return;
  }

  std::string line;
  int run=0;
  int ls=0;
  string tmp;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);
    std::string token;

    std::getline(iss,token, ',');
    std::stringstream runiss(token);
    runiss>>run;

    std::getline(iss,token, ',');
    std::stringstream lsiss(token);
    lsiss>>ls;
    if(ls>NLS){
      cout<<"LS larger than maximum"<<endl;
      return 0.;
    }
    
    std::getline(iss,token, ',');
    std::stringstream totLiss(token);
    totLiss>>modfrac[ls];

  }

  cout<<"Done reading the sigma_vis fraction."<<endl;
}



void plotCSVList(TString inpath, TString outpath=".", std::string runlist=""){


  ///create output file for lumisections
  TString lsoutfile=outpath+"/ls.dat";
  ofstream lsfile(lsoutfile.Data(),std::ofstream::app);
  if (!lsfile.is_open()){
    cout << "Unable to open output run file"; 
    return;
  }


  ////create output file for runs
  TString runoutfile=outpath+"/runs.dat";
  ofstream runfile(runoutfile.Data(),std::ofstream::app);
  if (!runfile.is_open()){
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
    getRefLumi(inpath+"/"+Run+".ref");

    
    //get the module fraction corrections
    bool corrModFrac=0;
    if(corrModFrac)  
      getModFrac(inpath+"/"+Run+".frac");
    
    
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
    double rawL=0.;
    double lsL=0; //lumi for given LS
    double bxL=0.;
    int maxLS=0;  //find last LS with lumi
    float maxL=0.;
    float runL=0.;
    float runLRef=0.;
    float sigmavis=getSigmaVis(Run);
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
      
      //cout<<","<<ls;
      
      ///read the  Lumi per ls
      std::getline(iss,token, ',');
      std::stringstream totLiss(token);
      totLiss>>rawL;
      
      //std::cout<<Run<<" "<<left<<setw(3)<<ls<<" "<<setw(10)<<rawL<<" "<<setw(10)<<refLumi[ls]<<" "<<sigmavis<<" "<<modfrac[ls]<<" "<<rawL/(sigmavis*modfrac[ls])<<std::endl;
      
      lsL=0.;
      if(sigmavis>0)
	lsL = rawL/sigmavis;
      if(corrModFrac && modfrac[ls]>0)
	lsL /= modfrac[ls];
      
      runL+=lsL;
      
      Nls++;
      
      ////fill lumi per LS plots
      if(HLumiLS.GetBinContent(ls)>0)
	cout<<run<<","<<ls<<"  duplicated"<<endl;    
      HLumiLS.SetBinContent(ls,lsL);
      
      //ratio to ref luminometer
      if(refLumi[ls]>0){
	//HLumiLSRatio.SetBinContent(ls,(lsL/refLumi[ls]-ratiomin)/(ratiomax-ratiomin));
	HLumiLSRatio.SetBinContent(ls,lsL/refLumi[ls]);
	HLumiLSRef.SetBinContent(ls,refLumi[ls]);
	runLRef+=refLumi[ls];
      }
      
      ///fill lumi per BX plots
      unsigned ncoll=0;
      if(perBXRatioPlots) { 
	//cout<<"bx colliding: ";
	for(int bx=0;bx<NBX;bx++){
	  std::getline(iss,token, ',');
	  std::stringstream bxLiss(token);
	  bxLiss>>rawL;
	  bxL = rawL/sigmavis;
	  if(corrModFrac && modfrac[ls]>0 )
	    bxL /= modfrac[ls];
	  HLumiBXvsLS.SetBinContent(ls,bx+1,bxL);
	  HLumiBX.AddBinContent(bx+1,bxL);
	  
	  if(rawL>0.5){
	    ncoll++;
	    //cout<<","<<bx+1;
	  }
	  
	}
	//cout<<endl;
      }
      
      if(ls > maxLS) maxLS=ls;
      if(lsL > maxL) maxL=lsL;
      
      lsfile<<Run<<" "<<left<<setw(3)<<ls<<" "<<setw(10)<<lsL<<" "<<setw(10)<<refLumi[ls]<<" "<<ncoll<<std::endl;
      Ncoll+=ncoll;
    }
    cout<<endl;
    cout<<"Done processing input file"<<endl;
    
    ///close files
    myfile.close();
    

    //write run lumi
    runfile<<Run<<" "<<runL<<" "<<runLRef<<std::endl;
    
  
  
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
    leg.AddEntry(&HLumiLS,"PCC","p");
    if(HLumiLSRef.GetEntries()>0){
      HLumiLSRef.SetMarkerStyle(8);
      HLumiLSRef.SetMarkerSize(0.3);
      HLumiLSRef.SetMarkerColor(4);
      HLumiLSRef.Draw("histpsame");
      leg.AddEntry(&HLumiLSRef,RefLumi,"p");
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
      HLumiLSRatio.GetYaxis()->SetRangeUser(0.7,1.3);
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
      HLumiBX.GetYaxis()->SetTitle("Avg. PCC");
      HLumiBX.GetXaxis()->SetTitle("bcid");
      HLumiBX.GetYaxis()->SetRangeUser(0,HLumiBX.GetMaximum()*1.3);
      HLumiBX.Draw("histp");
      text.DrawLatexNDC(0.75,0.85,TString("# colliding ")+(long)(Ncoll/Nls));
      C2.Print(outpath+"/"+(long)Run+"_AvgLumiBX.png");
      
      
    }




    
    ss.ignore(1);
  }
  
  lsfile.close();
  runfile.close();
  gROOT->ProcessLine(".q");
}




/*
    C.Clear();
    TPad can_1("can_1", "can_1", 0.0, 0.4, 1.0, 1.0);
    can_1.SetTopMargin(0.05);
    can_1.SetBottomMargin(0.02);
    can_1.SetLeftMargin(0.1);
    can_1.SetRightMargin(0.15);
    can_1.SetFrameBorderMode(0);
    can_1.cd();
    HLumiBXvsLS.GetXaxis()->SetTitle("lumi section");
    HLumiBXvsLS.GetYaxis()->SetTitle("bcid");
    HLumiBXvsLS.GetZaxis()->SetTitle("PCC Lumi [1/#mub]");
    HLumiBXvsLS.GetXaxis()->SetNdivisions(0);
    HLumiBXvsLS.GetXaxis()->SetRangeUser(0,maxLS+50);
    HLumiBXvsLS.GetYaxis()->SetLabelSize(0.05);
    HLumiBXvsLS.GetYaxis()->SetTitleSize(0.07);
    HLumiBXvsLS.GetYaxis()->SetTitleOffset(0.6);
    HLumiBXvsLS.GetZaxis()->SetLabelSize(0.05);
    HLumiBXvsLS.GetZaxis()->SetTitleSize(0.06);
    HLumiBXvsLS.GetZaxis()->SetTitleOffset(0.7);
    HLumiBXvsLS.Draw("colz");



    int nLSmerge=100;
    HRefLumiBXvsLS.RebinX(nLSmerge); HRefLumiBXvsLS.Scale(1./nLSmerge);
    HLumiBXvsLS.RebinX(nLSmerge); HLumiBXvsLS.Scale(1./nLSmerge);

    TH2F*HLumiBXvsLS_ratio=(TH2F*)HLumiBXvsLS.Clone("HLumiBXvsLS_ratio");
    HLumiBXvsLS_ratio->Divide(&HRefLumiBXvsLS);
    
    TGraphErrors G;
    int counterG=0;
    float fitmin=2,fitmax=7.5;
    TF1 P1("P1","[0]+[1]*x",fitmin,fitmax);
    P1.SetLineColor(2);

    TH2F HRatio("HRatio","",100,0,10,100,ratiomin,ratiomax);
    TH1F HR(TString("HR"),"",100,0,10);
    TH1F HN(TString("HN"),"",100,0,10);

    int nLSavg=20;
    TH1F HSlope(TString("HSlope"),"",nLSavg,0.5,nLSavg+0.5);
    TH1F HSlopeN(TString("HSlopeN"),"",nLSavg,0.5,nLSavg+0.5);

    TGraphErrors GRvsL;
    int counterGRvsL=0;

    float sbil=0.;
    float ratio=0.;
    
    int javg=0;

    for(int j=0;j<NBX;j++){
      if(j!=63) continue;
      
      //if(j==63)//||j==197||j==386||j==575||j==768||j==902||j==1091||j==1280||j==1469||j==1662||j==1796||j==1985||j==2174||j==2363||j==2556||j==2690||j==2879||j==3068||j==3257) javg=0;//reset the bcid index for averaging trains
      javg++;
      
      HR.Reset();
      HN.Reset();
      
      for(int l=1;l<HRefLumiBXvsLS.GetXaxis()->GetNbins();l++){
	sbil=HRefLumiBXvsLS.GetBinContent(l,j+1);
	ratio=HLumiBXvsLS_ratio->GetBinContent(l,j+1);
	if(sbil<1.5) continue;
	if(ratio<ratiomin) continue;//remove outliers
	if(ratio>ratiomax) continue;
	HR.Fill(sbil,ratio);
	HN.Fill(sbil,1);

	GRvsL.SetPoint(++counterGRvsL,l,ratio);
      }
      HR.Divide(&HN);
      //cout<<j+1<<" "<<HN.Integral()<<endl;

      if(HN.Integral()>1){
	HR.Fit(&P1,"Q","",0,20);
	G.SetPoint(counterG,j,P1.GetParameter(1));
	//G.SetPointError(counterG,0,P1.GetParError(1));
	counterG++;

	HSlope.Fill(javg,P1.GetParameter(1));
	HSlopeN.Fill(javg,1);
      }

      //fill the bx integrated histogram
      for(int b=1;b<=HR.GetNbinsX();b++)
	HRatio.Fill(HR.GetBinCenter(b),HR.GetBinContent(b));
     
    }
    

    C.Clear();
    //C.SetLeftMargin(0.15);
    GRvsL.GetYaxis()->SetRangeUser(0.85,1.15);
    GRvsL.GetYaxis()->SetTitle(TString(" slope"));
    GRvsL.GetXaxis()->SetTitle("lumi section");
    GRvsL.SetMarkerStyle(8);
    GRvsL.SetMarkerSize(0.4);
    GRvsL.Draw("ap");
    C.Print(outpath+"/"+(long)Run+"_sbilVsLS.png");

    

    C.Clear();
    HRatio.GetYaxis()->SetTitle(TString("ratio"));
    HRatio.GetYaxis()->SetRangeUser(ratiomin,ratiomax);
    HRatio.GetXaxis()->SetTitle("sbil");
    //HRatio.SetMarkerStyle(8);
    //HRatio.SetMarkerSize(0.4);
    HRatio.Fit(&P1,"Q","",fitmin,fitmax);
    HRatio.Draw("histp");
    P1.Draw("lsame");
    char text[100];
    sprintf(text,"slope = %.4f +/- %.4f ",P1.GetParameter(1),P1.GetParError(1));
    TText labeltext;
    labeltext.SetTextColor(2);
    labeltext.DrawTextNDC(0.4,0.2, TString(text));
    C.Print(outpath+"/"+(long)Run+"_linearity.png");    

    C.Clear();
    TProfile*P=HRatio.ProfileX("HRatioProf");
    P->GetYaxis()->SetTitle(TString("ratio"));
    P->GetYaxis()->SetRangeUser(ratiomin,ratiomax);
    P->Draw("pe");
    P1.Draw("lsame");
    labeltext.DrawTextNDC(0.4,0.2, TString(text));
    C.Print(outpath+"/"+(long)Run+"_linearityProf.png");    


    C.Clear();
    C.SetLeftMargin(0.15);
    G.GetYaxis()->SetTitle(TString(" ratio")+" slope");
    G.GetYaxis()->SetRangeUser(-0.02,0.02);
    G.GetXaxis()->SetTitle("bcid");
    G.SetMarkerStyle(8);
    G.Draw("ap");
    C.Print(outpath+"/"+(long)Run+"_perBXslopes.png");

    C.Clear();
    HSlope.Divide(&HSlopeN);
    HSlope.GetYaxis()->SetTitle(TString(" ratio")+" slope");
    HSlope.GetYaxis()->SetRangeUser(-0.02,0.02);
    HSlope.GetXaxis()->SetTitle("bcid");
    HSlope.Draw("histp");
    C.Print(outpath+"/"+(long)Run+"_perBXslopesAvg.png");
     

    ///create output file for slopes
    TString slopeoutfile=outpath+"/slope.dat";
    ofstream slopefile(slopeoutfile.Data(),std::ofstream::app);
    if (!slopefile.is_open()){
      cout << "Unable to open output slope.data file"; 
      return;
    }
    slopefile<<Run<<" "<<P1.GetParameter(1)<<" "<<P1.GetParError(1)<<endl;
    

*/
