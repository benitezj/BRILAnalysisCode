#include <iostream>
#include <fstream>
#include <string>
#include <TLegend.h>
void comparelsdotdatfiles() {
  
  TString Path="/eos/user/a/asehrawa/BRIL-new";

  TString inpath="/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC/EXPRESS_datasets/Run2018_ZB_test/Run2018A";  
  //TString inpath="/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_newveto_2018A/Run2018A_ZB";
  TString inpath1="/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_oldveto_Run2018A/Run2018A";
  
  //TString inpath="/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_newveto_2018B/Run2018B_ZB";
  //TString inpath1="/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_oldveto_Run2018B/Run2018B";
  
  //TString inpath="/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_newveto_2018C/Run2018C_ZB";
  //TString inpath1="/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_oldveto_Run2018C/Run2018C";
  
  gROOT->ProcessLine(".x /afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/rootlogon.C");
  
  ifstream myfile((inpath+"/ls.dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file";
    return;
    
  }
  cout<<"opened "<<(inpath+"/ls.dat")<<std::endl;
  
  ifstream myfile1((inpath1+"/ls.dat").Data());
  if (!myfile1.is_open()){
    cout << "Unable to open file"; 
    return;
    
  }
  cout<<"opened "<<(inpath1+"/ls.dat")<<std::endl;

  ///create histogram
  TH2F H("H (New veto)","", 200,0,25000,700,0,35000);
  TH2F H1("H1 (Old veto)","", 200,0,25000,700,0,35000);
  
  std::string line;
  std::string line1;
  
  int run=0;
  int run1=0;
  int ls=0;
  int ls1=0;
  
  double totL=0;   //lumi for given LS
  double totL1=0;
  
  //int Lumicounter=0;
  //int Lumicounter1=0;
  while (std::getline(myfile, line) || std::getline(myfile1, line1)){
    
    //cout<<"new "<<line<<endl;
    
    std::getline(myfile, line);
    std::stringstream iss(line);
    iss>>run>>ls>>totL;
    
    std::getline(myfile1, line1);
    std::stringstream iss1(line1);
    iss1>>run1>>ls1>>totL1;
    
    //cout<<"old "<<line1<<endl;
    
    float count=0;
    float count1=0;
    
    count=totL;
    count1=totL1;
    
    H.Fill(ls, count);
    cout<<"new "<<run<<"  "<<ls<<"  "<<count<<endl;
    H1.Fill(ls1, count1);
    cout<<"old "<<run1<<"  "<<ls1<<"  "<<count1<<endl;
    
    //Lumicounter+=ls;
    //Lumicounter1+=ls1;
    
  }
  
  myfile.close(); 
  myfile1.close();
  
  TCanvas C;
  
  H.GetXaxis()->SetTitle(" lumi sections ");
  H.GetYaxis()->SetTitle(" PCC count ");
  H.SetTitle("Run2018A (315255 - 316995)");
  //H.SetTitle("Run2018B (317080 - 319311)");
  //H.SetTitle("Run2018C (319337 - 320065)");
  H.Draw("colz");
  C.Print(Path+"/compare_lsdotdatfiles_H_RunA.png");
  
  TCanvas C1;
  
  H1.GetXaxis()->SetTitle(" lumi sections ");
  H1.GetYaxis()->SetTitle(" PCC count ");
  H1.SetTitle("Run2018A (315255 - 316995)");
  //H1.SetTitle("Run2018B (317080 - 319311)");                                                                                               
  //H1.SetTitle("Run2018C (319337 - 320065)");    
  H1.Draw("colz");
  C1.Print(Path+"/compare_lsdotdatfiles_H1_RunA.png");  
  
  gStyle->SetOptStat(0);
  
  TProfile* P = H.ProfileX();
  TProfile* P1 = H1.ProfileX();
  
  //P->GetYaxis()->SetRangeUser(0, 35000);
  P->SetMarkerColor(1);
  P->SetLineColor(1);
  
  //P1->GetYaxis()->SetRangeUser(0, 35000);
  P1->SetLineColor(2);
  P1->SetLineColor(2);
  
  TGraph *gr;
  gr=new TGraph();
  
  TGraph *gr1;
  gr1=new TGraph();
  
  //TGraph *gr2;
  //gr2=new TGraph();
  
  
  int x_value;
  float bincontent;
  for(int j = 1; j <= P->GetNbinsX(); j++) {
    x_value =  P->GetBinCenter(j);
    bincontent = P->GetBinContent(j); 
    if(bincontent > 0){
      gr ->SetPoint(gr->GetN(), x_value, bincontent);
    }
  }
  
  
  int x_value1;
  float bincontent1;
  for(int k = 1; k <= P1->GetNbinsX(); k++) {
    x_value1 =  P1->GetBinCenter(k);
    bincontent1 = P1->GetBinContent(k);
    if(bincontent1 > 0){
      gr1 ->SetPoint(gr1->GetN(), x_value1, bincontent1);
    }
  }
  
  
  TH1* h = new TH1D("h", "PCC count per ls (new)", 200, 0.0, 30000); 
  auto nPoints = gr->GetN(); 
  for(int i=0; i < nPoints; ++i) {
    double x,y;
    gr->GetPoint(i, x, y);
    h->Fill(x,y); 
  }
  
  
  TH1* h1 = new TH1D("h1", "PCC count per ls (old)", 200, 0.0, 30000); 
  auto nPoints1 = gr1->GetN();                                                                        
  for(int j=0; j < nPoints1; ++j) {
    double x1,y1;
    gr1->GetPoint(j, x1, y1);
    h1->Fill(x1,y1);                                                                                                               
  }
  
  h->GetXaxis()->SetTitle(" lumi sections ");
  h->GetYaxis()->SetTitle(" new/old ");
  h->SetMarkerStyle(8);
  h->SetLineColor(1);
  h->SetMarkerColor(1);
  h->SetMarkerSize(0.5);
  
  h1->GetXaxis()->SetTitle(" lumi sections ");
  h1->GetYaxis()->SetTitle(" new/old ");
  h1->SetMarkerStyle(8);
  h1->SetLineColor(1);
  h1->SetMarkerColor(1);
  h1->SetMarkerSize(0.5);
  
  TCanvas C10;
  
  //C10.Print(Path+"/histo_old_RunB.png");
  
  //gStyle->SetErrorX(0);
  if(ls==ls1){
    h->Divide(h1);
  }
  h->Draw("*histp");
  
  //TCanvas C11;
  //h1->Draw("histpsame");
  C10.Print(Path+"/histo_old_new_RunA_divide.png");

  TCanvas C2;
  P->Draw();
  C2.Print(Path+"/compare_lsdotdatfiles_profileX_RunA.png");
  
  TCanvas C3;
  P1->Draw();
  C3.Print(Path+"/compare_lsdotdatfiles_profileX_RunA1.png");

  gr->SetMarkerStyle(8);
  gr->SetLineColor(1);
  gr->SetMarkerColor(1);
  gr->SetMarkerSize(0.5);
  gr->GetXaxis()->SetTitle("lumi section");
  gr->GetYaxis()->SetTitle("PCC");
  gr->SetTitle("Run2018A (315255 - 316995)");                                                                                              
  //gr->SetTitle("Run2018B (317080 - 319311)");                                                                                              
  //gr->SetTitle("Run2018C (319337 - 320065)");
  
  //TCanvas C4;
  //gr->Draw("AP");
  //C4.Print(Path+"/compare_tgraph_countsperls_P.png");
  
  gr1->SetMarkerStyle(8);
  gr1->SetMarkerColor(2);
  gr1->SetLineColor(2);
  gr1->SetMarkerSize(0.5);
  gr1->GetXaxis()->SetTitle("lumi section");
  gr1->GetYaxis()->SetTitle("PCC");
  gr1->SetTitle("Run2018A (315255 - 316995)");                                                                                             
  //gr1->SetTitle("Run2018B (317080 - 319311)");                                                                                             
  //gr1->SetTitle("Run2018C (319337 - 320065)");
  
  //gr2->SetMarkerStyle(8);
  //gr2->SetMarkerColor(2);
  //gr2->SetLineColor(2);
  //gr2->SetMarkerSize(0.5);
  //gr2->GetXaxis()->SetTitle("lumi section");
  //gr2->GetYaxis()->SetTitle("PCC count (Zero Bias)");
  //gr2->SetTitle("Run2018A (315255 - 316995)");                                                                                             
  //gr2->SetTitle("Run2018B (317080 - 319311)");                                                                                             
  //gr2->SetTitle("Run2018C (319337 - 320065)");
  
  TCanvas C5;
  
  gr->Draw("AP"); 
  //gr->Divide(gr1); 
  gr1->Draw("Psame");
  //gr2->Draw("AP");
  
  auto* legend = new TLegend(0.77,0.77,0.89,0.89);                                                                                           
  legend->AddEntry(gr,"new veto","l");
  legend->AddEntry(gr1,"old veto","l");  
  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  legend->Draw("same");       
  
  C5.Print(Path+"/compare_tgraph_countsperls_tgraph_RunA_ratio.png");
  
  gROOT->ProcessLine(".q");

}
