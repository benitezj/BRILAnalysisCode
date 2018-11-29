#include <iostream>
#include <fstream>
#include <string>

#define NBX 3564
#define NLS 3000

//#define SigmaPCC 5.8e6/(23.31*11245.6) // old veto list
//#define SigmaPCC 6.704e6/(23.31*11245.6) // new veto list, but with double counting pixels
#define SigmaPCC 5.91e6/(23.31*11245.6) // new veto list, Georgios fixed pixel double counting


//#define MAXPCC 10e6  // un-normalized
//#define MAXPCC 0.5e6 // normalized
//#define MAXPCC 5e3 // un-normalized low mu
//#define MAXPCC 300 // normalized low mu

float refLumi[NLS];
TH2F HRefLumiBXvsLS("HRefLumiBXvsLS","",NLS,0.5,NLS+0.5,NBX,0.5,NBX+0.5);

float ratiomin=0.8;
float ratiomax=1.2;


void getRefLumi(TString inputfile){

  for(int i=0;i<NLS;i++){
    refLumi[i]=0.;
  }
    

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
    
    // std::string token;

    // std::getline(iss,token, ',');
    // std::stringstream runiss(token);
    // runiss>>run;

    // std::getline(iss,token, ',');
    // std::stringstream lsiss(token);
    // lsiss>>ls;
    // if(ls>NLS){
    //   cout<<"LS larger than maximum"<<endl;
    //   return;
    // }
    
    // std::getline(iss,token, ',');
    // std::stringstream totLiss(token);
    // totLiss>>refLumi[ls];
    // //cout<<run<<" "<<ls<endl;

    
    //325310 7358 2 2 10/26/18 07 27 01 STABLE BEAMS 6500 8368.161 6041.397 98.2 HFOC 1 0.0760 0.0549 ...
    iss>>run>>tmp>>ls>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp;
    iss>>refLumi[ls];
    iss>>tmp>>tmp;
    for(int j=0;j<NBX;j++){
      iss>>tmp>>tmp>>lumiBX;
      HRefLumiBXvsLS.SetBinContent(ls,j+1,lumiBX);
    }


  }

}


float getMaxLumi(TString inputfile){
    
  ifstream myfile(inputfile.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<inputfile.Data()<<std::endl;
    return 0.;
  }

  std::string line;
  int run=0;
  int ls=0;
  float L=0.;
  float maxL=0.;
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
    totLiss>>L;
    L/=SigmaPCC;
    if(L>maxL)maxL=L;
  }
  myfile.close();

  return maxL;
}


void plotPCCcsv(TString Path,long Run,TString outpath=".",TString ref=""){

  bool perBXRatioPlots=0;


  ///Open the lumi csv file
  TString infile=Path+"/"+Run+".csv";
  ifstream myfile (infile.Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<infile.Data()<<endl; 
    return;
  }

  float maxL=getMaxLumi(infile);
  if(maxL<2) return; //don't create the graph for empty runs
  float MAXPCC=1.3*maxL;
  cout<<"Max lumi: "<<maxL<<endl;


  ////create output file for runs
  TString runoutfile=outpath+"/runs.dat";
  ofstream runfile(runoutfile.Data(),std::ofstream::app);
  if (!runfile.is_open()){
    cout << "Unable to open output run file"; 
    return;
  }

  ///create output file for lumisections
  TString lsoutfile=outpath+"/ls.dat";
  ofstream lsfile(lsoutfile.Data(),std::ofstream::app);
  if (!lsfile.is_open()){
    cout << "Unable to open output run file"; 
    return;
  }


  ///create histograms
  TH2F HLumiBXvsLS("HLumiBXvsLS","",NLS,0.5,NLS+0.5,NBX,0.5,NBX+0.5);

  TH1F HLumiLS("HLumiLS","",NLS,0.5,NLS+0.5);
  TH1F HLumiLSRatio("HLumiLSRatio","",NLS,0.5,NLS+0.5);

  TH1F HLumiBX("HLumiBX","",NBX,0.5,NBX+0.5);
  TH1F HLumiBXRatio("HLumiBXRatio","",NBX,0.5,NBX+0.5);


  ///read the reference lumi
  if(ref.CompareTo("")!=0) getRefLumi(outpath+"/"+Run+"."+ref);
  

  std::string line;
  int run=0;
  int ls=0;
  double lsL=0;//lumi for given LS
  int maxLS=0;//find last LS with lumi
  int nLS=0;
  float runL=0.;
  float runLRef=0.;
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

    ///read the total Lumi
    std::getline(iss,token, ',');
    std::stringstream totLiss(token);
    totLiss>>lsL;
    lsL/=SigmaPCC;
    runL+=lsL;

    ////fill lumi per LS plots
    if(HLumiLS.GetBinContent(ls)>0)
      cout<<run<<","<<ls<<"  duplicated"<<endl;
    else HLumiLS.SetBinContent(ls,lsL);

    if(ref.CompareTo("")!=0&&refLumi[ls]>0){
      HLumiLSRatio.SetBinContent(ls,MAXPCC*(lsL/refLumi[ls]-ratiomin)/(ratiomax-ratiomin));
      runLRef+=refLumi[ls];
    }
    

    ///fill lumi per BX plots
    double bxL=0.;
    for(int bx=0;bx<NBX;bx++){
      std::getline(iss,token, ',');
      std::stringstream bxLiss(token);
      bxLiss>>bxL;
      bxL/=SigmaPCC;
      HLumiBXvsLS.SetBinContent(ls,bx+1,bxL);
      HLumiBX.AddBinContent(bx+1,bxL);
    }
    
    
    if(lsL>10){
      nLS++;
      if(ls>maxLS) maxLS=ls;
    }

    lsfile<<Run<<" "<<ls<<" "<<lsL<<" "<<refLumi[ls]<<std::endl;
  }
    
  ///close files
  myfile.close();
  lsfile.close();
  runfile<<Run<<" "<<runL<<" "<<runLRef<<std::endl;
  runfile.close();

  
  /////////////////////////////////////////////////////
  ///   make the plots
  ///////////////////////////////////////////////////
  gStyle->SetOptStat(0);

  HLumiBXvsLS.GetXaxis()->SetTitle("lumi section");
  HLumiBXvsLS.GetYaxis()->SetTitle("bcid");
  HLumiBXvsLS.GetZaxis()->SetTitle("PCC Lumi [1/#mub]");
  
  HLumiLS.GetXaxis()->SetTitle("lumi section");
  HLumiLS.GetYaxis()->SetTitle("PCC Lumi [1/#mub]");

  HLumiLSRatio.GetXaxis()->SetTitle("lumi section");
  HLumiLSRatio.GetYaxis()->SetTitle(TString("PCC/")+ref);


  TCanvas C;

  ///only 2D plot
  //C.Clear();
  //HLumiBXvsLS.Draw("colz");
  
  //2D plot on top and 1D on bottom
  TPad can_1("can_1", "can_1", 0.0, 0.4, 1.0, 1.0);
  can_1.SetTopMargin(0.05);
  can_1.SetBottomMargin(0.02);
  can_1.SetLeftMargin(0.1);
  can_1.SetRightMargin(0.15);
  can_1.SetFrameBorderMode(0);
  can_1.cd();
  HLumiBXvsLS.GetXaxis()->SetNdivisions(0);
  HLumiBXvsLS.GetXaxis()->SetRangeUser(0,maxLS+50);
  HLumiBXvsLS.GetYaxis()->SetLabelSize(0.05);
  HLumiBXvsLS.GetYaxis()->SetTitleSize(0.07);
  HLumiBXvsLS.GetYaxis()->SetTitleOffset(0.6);
  HLumiBXvsLS.GetZaxis()->SetLabelSize(0.05);
  HLumiBXvsLS.GetZaxis()->SetTitleSize(0.06);
  HLumiBXvsLS.GetZaxis()->SetTitleOffset(0.7);
  HLumiBXvsLS.Draw("colz");


  ////// Lumi vs ls 
  TPad can_2("can_2", "can_2",0.0, 0.0, 1.0, 0.4);
  can_2.SetTopMargin(0.08);
  can_2.SetBottomMargin(0.15);
  can_2.SetLeftMargin(0.1);
  can_2.SetRightMargin(0.15);
  can_2.SetFrameBorderMode(0);
  can_2.cd();

  HLumiLS.GetXaxis()->SetRangeUser(0,maxLS+50);
  HLumiLS.GetXaxis()->SetLabelSize(0.07);
  HLumiLS.GetXaxis()->SetTitleSize(0.1);
  HLumiLS.GetXaxis()->SetTitleOffset(0.6);
  HLumiLS.GetYaxis()->SetLabelSize(0.07);
  HLumiLS.GetYaxis()->SetTitleSize(0.08);
  HLumiLS.GetYaxis()->SetTitleOffset(0.6);
  HLumiLS.GetYaxis()->SetRangeUser(0,MAXPCC);
  HLumiLS.SetMarkerStyle(8);
  HLumiLS.SetMarkerSize(0.5);
  HLumiLS.Draw("histp");

  
  TLatex text;
  text.SetTextSize(0.13);
  text.DrawLatexNDC(0.6,0.8,TString("Run ")+(long)Run);
 
  TLine tline;
  tline.SetLineColor(2);


  ///////Ratio plot
  HLumiLSRatio.SetMarkerStyle(8);
  HLumiLSRatio.SetMarkerSize(0.5);
  HLumiLSRatio.SetMarkerColor(2);
  if(ref.CompareTo("")!=0) 
    HLumiLSRatio.Draw("histpsame");

  if(ref.CompareTo("")!=0)tline.DrawLine(0,MAXPCC*(1-ratiomin)/(ratiomax-ratiomin),maxLS+50,MAXPCC*(1-ratiomin)/(ratiomax-ratiomin));

  TGaxis *axis = new TGaxis(maxLS+50,0,maxLS+50,MAXPCC,ratiomin,ratiomax,510,"+L");
  axis->SetLineColor(kRed);
  axis->SetTextColor(kRed);
  axis->SetLabelSize(0.07);
  axis->SetLabelColor(2);
  axis->SetTitleSize(0.08);
  axis->SetTitleOffset(0.6);
  axis->SetTitle(TString("PCC/")+ref);
  if(ref.CompareTo("")!=0)axis->Draw();

 

  C.Clear();
  can_1.Draw();
  can_2.Draw();
  C.Print(outpath+"/"+(long)Run+".png");


  
  //// per BX ratio plots
  if(perBXRatioPlots&&ref.CompareTo("")!=0) {
    TH2F*HLumiBXvsLS_ratio=(TH2F*)HLumiBXvsLS.Clone("HLumiBXvsLS_ratio");
    HLumiBXvsLS_ratio->Divide(&HRefLumiBXvsLS);
    
    TGraph G;
    int counterG=0;
    TF1 P1("P1","[0]+[1]*x",0,500);

    bool firstplot=0;
    
    C.Clear();
    for(int j=0;j<NBX;j++){
      float maxl=0.;
      float lumi=0.;
      
      TH1F*HR=new TH1F(TString("HR")+(long)j,"",50,0,500);
      TH1F*HN=new TH1F(TString("HN")+(long)j,"",50,0,500);
      
      for(int l=1;l<NLS;l++){
	lumi=HLumiBXvsLS.GetBinContent(l,j+1);
	if(lumi>maxl) maxl=lumi;
	HR->Fill(lumi,HLumiBXvsLS_ratio->GetBinContent(l,j+1));
	HN->Fill(lumi,1);
      }
      
      if(maxl>1){
	HR->Divide(HN);
	if(!firstplot){ 
	  HR->GetYaxis()->SetRangeUser(ratiomin,ratiomax);
	  HR->GetYaxis()->SetTitle(TString("PCC / ")+ref);
	  HR->GetXaxis()->SetTitle("lumi/LS/bx [1/#mub]");
	  HR->Draw("histp");
	  firstplot=1;
	} else HR->Draw("histpsame");
	
	HR->Fit(&P1);
	G.SetPoint(counterG,counterG+1,P1.GetParameter(1));
	counterG++;
      }
    }
    C.Print(outpath+"/"+(long)Run+"_perBX.png");


    C.Clear();
    C.SetLeftMargin(0.15);
    G.GetYaxis()->SetTitle(TString(" PCC / ")+ref+" slope");
    G.GetYaxis()->SetRangeUser(-0.0005,0.0005);
    G.GetXaxis()->SetTitle("bcid");
    G.SetMarkerStyle(8);
    G.Draw("ap");
    C.Print(outpath+"/"+(long)Run+"_perBXslopes.png");

  }

  gROOT->ProcessLine(".q");
}

