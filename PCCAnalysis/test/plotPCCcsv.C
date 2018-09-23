#include <iostream>
#include <fstream>
#include <string>
#define NBX 3564
#define NLS 3000
#define MAXPCC 10e6
//#define MAXPCC 5e3
//#define MAXPCC 1.5e3
float refLumi[NLS];
float ratiomin=18;
float ratiomax=22;

void getTotLumiFromCSV(TString inputfile){

  for(int i=0;i<NLS;i++) 
    refLumi[i]=0.;
    
  ifstream myfile(inputfile.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<inputfile.Data()<<std::endl;
    return;
  }

  std::string line;
  int run=0;
  int ls=0;
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
      return;
    }
    
    std::getline(iss,token, ',');
    std::stringstream totLiss(token);
    totLiss>>refLumi[ls];
    //cout<<run<<" "<<ls<endl;
  }

}

void plotPCCcsv(TString Path,long Run,TString outpath=".",TString ref=""){

  TString infile=Path+"/"+Run+".csv";
  ifstream myfile (infile.Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
  }
  cout<<"opened "<<infile<<std::endl;

  
  TString runoutfile=outpath+"/runs.dat";
  ofstream runfile(runoutfile.Data(),std::ofstream::app);
  if (!runfile.is_open()){
    cout << "Unable to open output run file"; 
    return;
  }
  cout<<"opened "<<runoutfile<<std::endl;


  ///create histogram
  TH2F HLumiBXvsLS("HLumiBXvsLS","",NLS,0.5,NLS+0.5,NBX,0.5,NBX+0.5);
  TH1F HLumiLS("HLumiLS","",NLS,0.5,NLS+0.5);
  TH1F HLumiLSRatio("HLumiLSRatio","",NLS,0.5,NLS+0.5);


  ///read the reference lumi
  if(ref.CompareTo("")!=0) getTotLumiFromCSV(outpath+"/"+Run+"."+ref);
  

  std::string line;
  int run=0;
  int ls=0;
  double totL=0;//lumi for given LS
  double bxL[NBX];//lumi for bunch crossing in LS
  float maxL=0.;//find maximum totL 
  int maxLS=0;//find last LS with lumi
  float totLRun=0.;
  float totRefLRun=0.;
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
    totLiss>>totL;
    totLRun+=totL;
    
    if(HLumiLS.GetBinContent(ls)>0)
      cout<<run<<","<<ls<<"  duplicated"<<endl;
    else HLumiLS.SetBinContent(ls,totL);


    if(ref.CompareTo("")!=0){
      HLumiLSRatio.SetBinContent(ls,MAXPCC*(totL/refLumi[ls]-ratiomin)/(ratiomax-ratiomin));
      totRefLRun+=refLumi[ls];
    }

    ///read lumi per BX
    for(int bx=0;bx<NBX;bx++)
      bxL[bx]=0.;

    double totbxL=0.;
    for(int bx=0;bx<NBX;bx++){
      std::getline(iss,token, ',');
      std::stringstream bxLiss(token);
      bxLiss>>bxL[bx];
      totbxL+=bxL[bx];

      HLumiBXvsLS.SetBinContent(ls,bx+1,bxL[bx]);

    }

    
    if(totL>maxL) maxL=totL;
    if(totL>10 && ls > maxLS) maxLS=ls;

    //std::cout<<run<<" "<<ls<<" "<<totL<<" "<<refLumi[ls]<<std::endl;
  }
    
  myfile.close();

  //std::cout<<run<<" "<<totLRun<<" "<<totRefLRun<<std::endl;

  //write run info:
  runfile<<Run<<" "<<totLRun<<std::endl;
  runfile.close();

 
  

  if(maxL<2) return; //don't create the graph for empty runs


  gStyle->SetOptStat(0);


  HLumiBXvsLS.GetXaxis()->SetTitle("lumi section");
  HLumiBXvsLS.GetYaxis()->SetTitle("bunch crossing");
  HLumiBXvsLS.GetZaxis()->SetTitle("PCC Lumi");
  
  HLumiLS.GetXaxis()->SetTitle("lumi section");
  HLumiLS.GetYaxis()->SetTitle("PCC Lumi");

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
  
  // HLumiLSRatio.GetXaxis()->SetRangeUser(0,maxLS+50);
  // HLumiLSRatio.GetXaxis()->SetLabelSize(0.07);
  // HLumiLSRatio.GetXaxis()->SetTitleSize(0.1);
  // HLumiLSRatio.GetXaxis()->SetTitleOffset(0.6);
  // HLumiLSRatio.GetYaxis()->SetLabelSize(0.07);
  // HLumiLSRatio.GetYaxis()->SetTitleSize(0.08);
  // HLumiLSRatio.GetYaxis()->SetTitleOffset(0.6);
  // HLumiLSRatio.GetYaxis()->SetRangeUser(ratiomin,ratiomax);
  HLumiLSRatio.SetMarkerStyle(8);
  HLumiLSRatio.SetMarkerSize(0.5);
  HLumiLSRatio.SetMarkerColor(2);
  if(ref.CompareTo("")!=0) HLumiLSRatio.Draw("histpsame");

  TGaxis *axis = new TGaxis(maxLS+50,0,maxLS+50,MAXPCC,ratiomin,ratiomax,510,"+L");
  axis->SetLineColor(kRed);
  axis->SetTextColor(kRed);
  axis->SetLabelSize(0.07);
  axis->SetLabelColor(2);
  axis->SetTitleSize(0.08);
  axis->SetTitleOffset(0.6);
  axis->SetTitle(TString("PCC/")+ref);
  if(ref.CompareTo("")!=0)axis->Draw();

  TLatex text;
  text.SetTextSize(0.13);
  text.DrawLatexNDC(0.6,0.8,TString("Run ")+(long)Run);
  


  C.Clear();
  can_1.Draw();
  can_2.Draw();

  C.Print(outpath+"/"+(long)Run+".png");

  gROOT->ProcessLine(".q");
}

