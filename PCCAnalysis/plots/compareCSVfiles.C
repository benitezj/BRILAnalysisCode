#include "globals.h"

#include "../test_Run3/verifyT0ReplayDBUploads.C"


void compareCSVfiles(){

  //TString path="/Users/josebenitez/analysis/verify-T0Replay-ZB";
  
  //TString refF="rawPCC-step5_onlyRawPCCProducer_ALCAPRODUCER-correctGT-fromT0ReplayALCARECO-UnCorr.csv";
  //TString refF="rawPCC-step5_onlyRawPCCProducer_ALCAPRODUCER-correctGT-fromT0ReplayALCARECO.csv";
  //TString newF="rawPCC-fromT0ReplayRawPCCProducer-UnCorr.csv";
  //TString newF="rawPCC-fromT0ReplayRawPCCProducer.csv";
  //TString newF="rawPCC_Tier0_REPLAY_2022-RawPCCProducer-PromptReco-v19-UnCorr.csv";
  //TString newF="rawPCC_Tier0_REPLAY_2022-RawPCCProducer-PromptReco-v19.csv";

  TString path="/Users/josebenitez/analysis/verify-PCCIntegrator_Concurrent/";
  TString refF="rawPCC-step5_onlyRawPCCProducer_ALCAPRODUCER-correctGT-fromT0ReplayALCARECO-UnCorr.csv";
  TString newF="rawPCC_step5_ALCAPRODUCER-4Thread.csv";


  cout<<"Ref file: "<<refF<<endl;
  cout<<"New file: "<<newF<<endl;
  
  int selectLS=600; //select one LS to compare lumi vs bcid

  bool applySF=0; //apply Afterglow correction factors before comparison
  
  //////////////////////////////
  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");

  gStyle->SetOptStat(0);


  TGraph LumiNew;
  TGraph LumiRef;
  TGraph LumiRatio;
  TGraph LumiRefCorr;
  TGraph LumiRatioCorr;

  
  TGraph LumiNewBX;
  TGraph LumiRefBX;
  TGraph LumiRatioBX;
  TGraph LumiRefBXCorr;
  TGraph LumiRatioBXCorr;

  ifstream newfile((path+"/"+newF).Data());
  if (!newfile.is_open()){
    std::cout << "Unable to open  "<<newF.Data()<<std::endl;
    return;
  }


  std::string line;
  int run=0;
  int ls=0;
  int bx=0;
  float LumiLS = 0.;
  float LumiBX = 0.;
  float maxLumiLS=0.;
  float maxLumiBX=0;


  while (std::getline(newfile,line)){
    std::stringstream iss(line);
    std::string token;

    std::getline(iss,token, ',');
    std::stringstream runiss(token);
    runiss>>run;

    std::getline(iss,token, ',');
    std::stringstream lsiss(token);
    lsiss>>ls;

    std::getline(iss,token, ',');
    std::stringstream lumiiss(token);
    lumiiss>>LumiLS;


    
    ///////////// read the   reference
    ifstream reffile((path+"/"+refF).Data());
    if (!reffile.is_open()){
      std::cout << "Unable to open "<<refF.Data()<<std::endl;
      return;
    }

    std::string refline;
    int refrun=0;
    int refls=0;
    int refbx=0;
    float refLumiLS = 0.;
    float refLumiBX = 0.;
    while (std::getline(reffile,refline)){

      std::getline(reffile,refline);
      std::stringstream refiss(refline);
      std::string reftoken;

      std::getline(refiss,reftoken, ',');
      std::stringstream refruniss(reftoken);
      refruniss>>refrun;

      std::getline(refiss,reftoken, ',');
      std::stringstream reflsiss(reftoken);
      reflsiss>>refls;

      std::getline(refiss,reftoken, ',');
      std::stringstream reflumiiss(reftoken);
      reflumiiss>>refLumiLS;
    
 
      
      if(run==refrun && ls==refls){
	//cout<<run<<" "<<ls<<" "<<LumiLS<<" "<<refrun<<" "<<refls<<" "<<refLumiLS<<endl;
      
	//////////Fill per LS plots
	LumiNew.SetPoint(LumiNew.GetN(),ls,LumiLS);
	LumiRef.SetPoint(LumiRef.GetN(),ls,refLumiLS);
      
	if(refLumiLS!=0.)   LumiRatio.SetPoint(LumiRatio.GetN(),ls,LumiLS/refLumiLS);
	else if(LumiLS==0.) LumiRatio.SetPoint(LumiRatio.GetN(),ls,1);
	else                LumiRatio.SetPoint(LumiRatio.GetN(),ls,-0.5);
      
	if(maxLumiLS<LumiLS) maxLumiLS = LumiLS;
	if(maxLumiLS<refLumiLS) maxLumiLS = refLumiLS;


	//////////////////////////////////
	/// Read per bx lumi
      
	//read the Afterglow factors from CONDDB payload
	float SF[NBX];
	for(int b=1;b<=NBX;b++) SF[b-1]=1;
	if(applySF) readPayload_csv(inputPath,ls,SF);
	float refLumiBX_corr=0.;
	float refLumiLS_corr=0.;

      
	for(int b=1;b<=NBX;b++){
	  std::getline(iss,token, ',');
	  std::stringstream biss(token);
	  biss>>LumiBX;
	
	  std::getline(refiss,reftoken, ',');
	  std::stringstream refbiss(reftoken);
	  refbiss>>refLumiBX;

	
	  //apply Payload correction factor
	  refLumiBX_corr = refLumiBX * SF[b-1];
	  refLumiLS_corr += refLumiBX_corr;
	
	  //per bx comparisons only for selected LS
	  if(ls==selectLS){
	    LumiNewBX.SetPoint(LumiNewBX.GetN(),b,LumiBX);
	    LumiRefBX.SetPoint(LumiRefBX.GetN(),b,refLumiBX);

	    if(refLumiBX!=0.)       LumiRatioBX.SetPoint(LumiRatioBX.GetN(),b,LumiBX/refLumiBX);
	    else if(refLumiBX==0.)  LumiRatioBX.SetPoint(LumiRatioBX.GetN(),b,1);
	    else                    LumiRatioBX.SetPoint(LumiRatioBX.GetN(),b,-0.5);
	    
	    if(LumiBX>maxLumiBX) maxLumiBX=LumiBX;
	    if(refLumiBX>maxLumiBX) maxLumiBX=refLumiBX;


	    LumiRefBXCorr.SetPoint(LumiRefBXCorr.GetN(),b,refLumiBX_corr);	    	    
	    if(refLumiBX_corr!=0.) LumiRatioBXCorr.SetPoint(LumiRatioBXCorr.GetN(),b,LumiBX/refLumiBX_corr);
	    else if(refLumiBX==0.) LumiRatioBXCorr.SetPoint(LumiRatioBXCorr.GetN(),b,1);
	    else                   LumiRatioBXCorr.SetPoint(LumiRatioBXCorr.GetN(),b,-0.5);

	  }
	}


      
	LumiRefCorr.SetPoint(LumiRefCorr.GetN(),ls,refLumiLS_corr);
	if(refLumiLS_corr!=0.)	LumiRatioCorr.SetPoint(LumiRatioCorr.GetN(),ls,LumiLS/refLumiLS_corr);
	else if(LumiLS==0.) 	LumiRatioCorr.SetPoint(LumiRatioCorr.GetN(),ls,1);
	else               	LumiRatioCorr.SetPoint(LumiRatioCorr.GetN(),ls,-0.5);


	break;
      } else cout<<" LS = "<<ls<<" not found"<<endl;
      
      
    }
    reffile.close();
  }
  newfile.close();
  

  TCanvas C;
  TLatex text;
  text.SetTextSize(0.025);

  ///per LS
  C.Clear();
  LumiNew.GetYaxis()->SetRangeUser(-0.1*maxLumiLS,1.1*maxLumiLS);
  LumiNew.SetMarkerStyle(8);
  LumiNew.SetMarkerSize(0.5);
  LumiNew.SetMarkerColor(4);
  LumiNew.GetXaxis()->SetTitle("lumi section");
  LumiNew.GetYaxis()->SetTitle(TString("Run ")+run+"   PCC");
  LumiNew.Draw("ap");

  LumiRef.SetMarkerStyle(8);
  LumiRef.SetMarkerSize(0.4);
  LumiRef.SetMarkerColor(2);
  LumiRef.Draw("psame");
  
  text.SetTextColor(4);
  text.DrawLatexNDC(0.05,0.97,newF.Data());
  text.SetTextColor(2);
  text.DrawLatexNDC(0.05,0.94,refF.Data());
  C.Print(path+"/compareCSVfiles_LS.png");
  
  C.Clear();
  LumiRatio.GetYaxis()->SetRangeUser(-0.51,2);
  LumiRatio.GetXaxis()->SetTitle("lumi section");
  LumiRatio.GetYaxis()->SetTitle(TString("Run ")+run+"   PCC ratio");
  LumiRatio.SetMarkerStyle(8);
  LumiRatio.SetMarkerSize(0.4);
  LumiRatio.SetMarkerColor(1);
  LumiRatio.Draw("ap");
  C.Print(path+"/compareCSVfiles_LS_ratio.png");

  
  ////per bcid for one LS
  C.Clear();
  LumiNewBX.SetTitle(TString("PCC for LS = ") + selectLS);
  LumiNewBX.GetYaxis()->SetRangeUser(-0.1*maxLumiBX,1.1*maxLumiBX);
  LumiNewBX.GetXaxis()->SetTitle("bx");
  LumiNewBX.GetYaxis()->SetTitle(TString("Run ")+run+"   PCC");
  LumiNewBX.SetMarkerStyle(8);
  LumiNewBX.SetMarkerSize(0.5);
  LumiNewBX.SetMarkerColor(4);
  LumiNewBX.Draw("ap");
  LumiRefBX.SetMarkerStyle(8);
  LumiRefBX.SetMarkerSize(0.4);
  LumiRefBX.SetMarkerColor(2);
  LumiRefBX.Draw("psame");
  C.Print(path+"/compareCSVfiles_BX.png");
 
  
  C.Clear();
  LumiRatioBX.SetTitle(TString("PCC ratio  for LS = ") + selectLS);
  LumiRatioBX.GetYaxis()->SetRangeUser(-0.5,2);
  LumiRatioBX.GetXaxis()->SetTitle("bx");
  LumiRatioBX.GetYaxis()->SetTitle(TString("Run ")+run+"   PCC ratio");
  LumiRatioBX.SetMarkerStyle(8);
  LumiRatioBX.SetMarkerSize(0.4);
  LumiRatioBX.SetMarkerColor(1);
  LumiRatioBX.Draw("ap");
  C.Print(path+"/compareCSVfiles_BX_ratio.png");



  if(applySF){
    //////////////////////////////////////////////
    ////corrected per LS
    C.Clear();
    LumiNew.Draw("ap");
    LumiRefCorr.SetMarkerStyle(8);
    LumiRefCorr.SetMarkerSize(0.4);
    LumiRefCorr.SetMarkerColor(2);
    LumiRefCorr.Draw("psame");
    text.SetTextColor(4);
    text.DrawLatexNDC(0.05,0.97,newF.Data());
    text.SetTextColor(2);
    text.DrawLatexNDC(0.05,0.94,refF.Data());
    C.Print(path+"/compareCSVfiles_LS_corr.png");
  
    C.Clear();
    LumiRatioCorr.GetYaxis()->SetRangeUser(-0.5,2);
    LumiRatioCorr.GetXaxis()->SetTitle("lumi section");
    LumiRatioCorr.GetYaxis()->SetTitle(TString("Run ")+run+"   PCC ratio");
    LumiRatioCorr.SetMarkerStyle(8);
    LumiRatioCorr.SetMarkerSize(0.4);
    LumiRatioCorr.SetMarkerColor(1);
    LumiRatioCorr.Draw("ap");
    C.Print(path+"/compareCSVfiles_LS_ratio_corr.png");
  

    //per bcid
    C.Clear();
    LumiNewBX.Draw("ap");
    LumiRefBXCorr.SetMarkerStyle(8);
    LumiRefBXCorr.SetMarkerSize(0.4);
    LumiRefBXCorr.SetMarkerColor(2);
    LumiRefBXCorr.Draw("psame");
    C.Print(path+"/compareCSVfiles_BX_corr.png");
 
  
    C.Clear();
    LumiRatioBXCorr.SetTitle(TString("PCC ratio  for LS = ") + selectLS);
    LumiRatioBXCorr.GetYaxis()->SetRangeUser(-0.5,2);
    LumiRatioBXCorr.GetXaxis()->SetTitle("bx");
    LumiRatioBXCorr.GetYaxis()->SetTitle(TString("Run ")+run+"   PCC ratio");
    LumiRatioBXCorr.SetMarkerStyle(8);
    LumiRatioBXCorr.SetMarkerSize(0.4);
    LumiRatioBXCorr.SetMarkerColor(1);
    LumiRatioBXCorr.Draw("ap");
    C.Print(path+"/compareCSVfiles_BX_ratio_corr.png");

  }
  
  gROOT->ProcessLine(".q");
}
