#include <iostream>
#include <fstream>
#include <string>

//#define MAXPCCRUN 5e10
//#define PCCZEROLUMI 1e3
//#define PCCRUNLUMIRatioMin 18
//#define PCCRUNLUMIRatioMax 22

#define MAXPCCRUN 1e9
#define PCCZEROLUMI 0
#define PCCRUNLUMIRatioMin 0.9
#define PCCRUNLUMIRatioMax 1.1

long RUNOFFSET=355862;
//TString ref="bestlumi";


// float getRunRefLumi(TString inputfile){
  
//   ifstream myfile(inputfile.Data());
//   if (!myfile.is_open()){
//     std::cout << "Unable to open ref lumi file: "<<inputfile.Data()<<std::endl;
//     return 0.;
//   }
  
//   std::string line;
//   int run=0;
//   int ls=0;
//   float totL=0.;
//   float totLRun=0.;
//   while (std::getline(myfile, line)){
//     std::stringstream iss(line);
//     std::string token;
    
//     std::getline(iss,token, ',');
//     std::stringstream runiss(token);
//     runiss>>run;
    
//     std::getline(iss,token, ',');
//     std::stringstream lsiss(token);
//     lsiss>>ls;
    
//     std::getline(iss,token, ',');
//     std::stringstream totLiss(token);
//     totLiss>>totL;
    
//     totLRun+=totL;
//     //if(run==316766) cout<<run<<" "<<ls<<" "<<totL<<endl;
//   }
//   //if(run==316766) cout<<totLRun<<endl;

//   return totLRun;
// }


// bool getPIXReadyState(TString inputfile, int RUN){
  
//   ifstream myfile(inputfile.Data());
//   if (!myfile.is_open()){
//     std::cout << "Unable to open ref lumi file: "<<inputfile.Data()<<std::endl;
//     return 0.;
//   }
  
//   std::string line;
//   int run=0;
//   bool state=0;
//   while (std::getline(myfile,line)){
//     std::stringstream iss(line);
//     iss>>run;
//     if(run==RUN){
//       iss>>state;
//       break;
//     }
//   }
//   myfile.close();

//   return state;//actual values in input file are 1=OFF, 0=ON
// }


void plotPCCruns(TString Path){

  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");
  
  TString infile=Path+"/runs.dat";
  ifstream myfile(infile.Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
  }
  cout<<"opened "<<infile<<std::endl;


  ///create histogram
  TGraph LumiRun;
  TGraph LumiRunZero;
  TGraph LumiRunRatio;
  
  TH1F HRatio("HRatio","",40,PCCRUNLUMIRatioMin,PCCRUNLUMIRatioMax);

  std::string line;
  int run=0;
  float totLRun=0.;
  float totRefLRun=0.;
  int counter=0;
  int counterRatio=0;
  int counterZero=0;
  int lastrun=0;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);

    iss>>run;
    iss>>totLRun;
    iss>>totRefLRun;

    LumiRun.SetPoint(counter,run-RUNOFFSET,totLRun);


    float runratio=0.;
    if(totRefLRun>0){
      runratio=totLRun/totRefLRun;
    }
    LumiRunRatio.SetPoint(counter,counter,runratio);
    HRatio.Fill(runratio);

    counter++;
    cout<<run<<" "<<runratio<<endl;

    if(run>lastrun) lastrun=run-RUNOFFSET;    
  }
  
  
  myfile.close();

  gStyle->SetOptStat(0);
    

  TCanvas C;

  C.SetLogy(1);

  //LumiRun.GetXaxis()->SetLabelSize(0.07);
  //LumiRun.GetXaxis()->SetTitleSize(0.1);
  //LumiRun.GetXaxis()->SetTitleOffset(0.6);
  //LumiRun.GetYaxis()->SetLabelSize(0.07);
  //LumiRun.GetYaxis()->SetTitleSize(0.08);
  //LumiRun.GetYaxis()->SetTitleOffset(0.6);

  LumiRun.GetYaxis()->SetRangeUser(PCCZEROLUMI,MAXPCCRUN);
  LumiRun.SetMarkerStyle(8);
  LumiRun.SetMarkerSize(0.5);
  LumiRun.Draw("ap");

  LumiRun.GetXaxis()->SetTitle("run");//TString("run number ") + " - " + RUNOFFSET);
  LumiRun.GetYaxis()->SetTitle("integrated lumi [ #mub^{-1} ]");

  LumiRunZero.SetMarkerStyle(8);
  LumiRunZero.SetMarkerSize(0.5); 
  LumiRunZero.SetMarkerColor(4);
  LumiRunZero.Draw("psame");
  C.Print(Path+"/runs.png");


    C.Clear();
    C.SetLogy(0);

    LumiRunRatio.Draw("ap");
    LumiRunRatio.GetYaxis()->SetRangeUser(PCCRUNLUMIRatioMin,PCCRUNLUMIRatioMax);      
    LumiRunRatio.SetMarkerStyle(8);
    LumiRunRatio.SetMarkerSize(0.5); 
    LumiRunRatio.SetMarkerColor(2);
    LumiRunRatio.GetYaxis()->SetTitle(TString("ratio"));
    LumiRunRatio.GetXaxis()->SetTitle(LumiRun.GetXaxis()->GetTitle());
    LumiRunRatio.Draw("ap");
    
    // TGaxis *axis = new TGaxis(LumiRun.GetXaxis()->GetXmax(),PCCZEROLUMI,LumiRun.GetXaxis()->GetXmax(),MAXPCCRUN,PCCRUNLUMIRatioMin,PCCRUNLUMIRatioMax,510,"+L");
    // axis->SetLineColor(kRed);
    // axis->SetTextColor(kRed);
    // //axis->SetLabelSize(0.07);
    // axis->SetLabelColor(2);
    // //axis->SetTitleSize(0.08);
    // //axis->SetTitleOffset(0.6);
    // axis->SetTitle(TString("PCC / ")+ref);
    // axis->Draw();

    C.Print(Path+"/runs_ratio.png");

    gStyle->SetOptStat(111111);
    C.Clear();
    C.SetLogy(0);
    HRatio.GetYaxis()->SetTitle("# of runs");
    HRatio.GetXaxis()->SetTitle(LumiRunRatio.GetYaxis()->GetTitle());
    HRatio.Draw("hist");
    C.Print(Path+"/runs_ratio_hist.png");


  gROOT->ProcessLine(".q");
}

