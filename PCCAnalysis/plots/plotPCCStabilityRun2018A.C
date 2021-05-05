#include <iostream>
#include <fstream>
#include <string>
#include <TLegend.h>
void plotPCCStabilityRun2018A() {
    
  TString inpath40 = "/afs/cern.ch/user/a/asehrawa/www/BRIL-PCC/veto_lateRunD_lowcut_tight/Run2018A";

  TString inpath="/eos/user/a/asehrawa/B1/Run2018A";
  TString inpath3="/eos/user/a/asehrawa/B2/Run2018A";
  TString inpath6="/eos/user/a/asehrawa/B3/Run2018A";
  TString inpath9="/eos/user/a/asehrawa/F1/Run2018A";
  TString inpath12="/eos/user/a/asehrawa/F2/Run2018A";
  TString inpath15="/eos/user/a/asehrawa/F3/Run2018A";
  TString ref="";
  
  gROOT->ProcessLine(".x /afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/rootlogon.C");
  


  ifstream myfile40((inpath40+"/ls.dat").Data());
  if (!myfile40.is_open()){
    cout << "Unable to open file";
    return;

  }
  cout<<"opened "<<(inpath40+"/ls.dat")<<std::endl;



  ifstream myfile((inpath+"/ls.dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;
    
  }
  cout<<"opened "<<(inpath+"/ls.dat")<<std::endl;



  ifstream myfile3((inpath3+"/ls.dat").Data());
  if (!myfile3.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath3+"/ls.dat")<<std::endl;



  ifstream myfile6((inpath6+"/ls.dat").Data());
  if (!myfile6.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath6+"/ls.dat")<<std::endl;



  ifstream myfile9((inpath9+"/ls.dat").Data());
  if (!myfile9.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath9+"/ls.dat")<<std::endl;



  ifstream myfile12((inpath12+"/ls.dat").Data());
  if (!myfile12.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath12+"/ls.dat")<<std::endl;



  ifstream myfile15((inpath15+"/ls.dat").Data());
  if (!myfile15.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath15+"/ls.dat")<<std::endl;



  ///create histogram
  int Lumicounter=0;

  TH2F H40("H40","", 200,0,80000,300,0,0.35);

  TH2F H("H","", 200,0,80000,300,0,0.35);
  TH2F H3("H3","", 200,0,80000,300,0,0.35);
  TH2F H6("H6","", 200,0,80000,300,0,0.35);
  TH2F H9("H9","", 200,0,80000,300,0,0.35);
  TH2F H12("H12","", 200,0,80000,300,0,0.35);
  TH2F H15("H15","", 200,0,80000,300,0,0.35);
  
  std::string line;
  int run=0;
  int ls=0;

  double totL40=0;

  double totL=0;   //lumi for given LS
  double totL3=0;
  double totL6=0;
  double totL9=0;
  double totL12=0;
  double totL15=0;
  

  double totLRef=0;//lumi for given LS

  while (std::getline(myfile40, line)){


    //cout<<line;

    std::stringstream iss40(line);
    iss40>>run>>ls>>totL40>>totLRef;


    std::getline(myfile, line);
    std::stringstream iss(line);
    iss>>run>>ls>>totL>>totLRef;

    std::getline(myfile3, line);
    std::stringstream iss3(line);
    iss3>>run>>ls>>totL3>>totLRef;
   
    std::getline(myfile6, line);
    std::stringstream iss6(line);
    iss6>>run>>ls>>totL6>>totLRef;
    
    std::getline(myfile9, line);
    std::stringstream iss9(line);
    iss9>>run>>ls>>totL9>>totLRef;
   
    std::getline(myfile12, line);
    std::stringstream iss12(line);
    iss12>>run>>ls>>totL12>>totLRef;
    
    std::getline(myfile15, line);
    std::stringstream iss15(line);
    iss15>>run>>ls>>totL15>>totLRef;

    float ratio40=0;

    float ratio=0;
    float ratio3=0;
    float ratio6=0;
    float ratio9=0;
    float ratio12=0;
    float ratio15=0;
    

    if(totLRef>0){
   
      ratio=totL/totL40;
      ratio3=totL3/totL40;
      ratio6=totL6/totL40;
      ratio9=totL9/totL40;
      ratio12=totL12/totL40;
      ratio15=totL15/totL40;

      H.Fill(Lumicounter,ratio);
      H3.Fill(Lumicounter,ratio3);
      H6.Fill(Lumicounter,ratio6);
      H9.Fill(Lumicounter,ratio9);
      H12.Fill(Lumicounter,ratio12);
      H15.Fill(Lumicounter,ratio15);
      
      Lumicounter++;

    }
  }
    
  myfile.close(); 
  myfile3.close();
  myfile6.close();
  myfile9.close();
  myfile12.close();
  myfile15.close();
  
  TCanvas C;
 
  H.GetXaxis()->SetTitle(" lumi sections ");
  H.GetYaxis()->SetTitle(" lumi ratio ");
  H.SetTitle("                                                                               Run2018A (315255 - 316995)");
  H.Draw("colz");
  H3.Draw("colzsame");
  H6.Draw("colzsame");
  H9.Draw("colzsame");
  H12.Draw("colzsame");
  H15.Draw("colzsame");
  
  
  C.Print(inpath+"/combined.png"); 

  TCanvas C1;

  TProfile* P = H.ProfileX();
 
  P->GetYaxis()->SetRangeUser(0,0.35);
  P->GetYaxis()->SetTitle(" lumi fraction (per 400 lumi sections) ");

  TProfile* P3 = H3.ProfileX();
  TProfile* P6 = H6.ProfileX();
  TProfile* P9 = H9.ProfileX();
  TProfile* P12 = H12.ProfileX();
  TProfile* P15 = H15.ProfileX();
 
  P->SetMarkerColor(1);
  P3->SetMarkerColor(2);
  P6->SetMarkerColor(3);
  P9->SetMarkerColor(4);
  P12->SetMarkerColor(5);
  P15->SetMarkerColor(6);

  P->SetLineColor(1);
  P3->SetLineColor(2);
  P6->SetLineColor(3);
  P9->SetLineColor(4);
  P12->SetLineColor(5);
  P15->SetLineColor(6);


  P->Draw();
  P3->Draw("same");
  P6->Draw("same");
  P9->Draw("same");
  P12->Draw("same");
  P15->Draw("same");

  auto* legend = new TLegend(0.8,0.35,0.93,0.8);
  //legend->SetHeader("Histograms","C");                                                                                                      
  legend->AddEntry(P,"B1 ","l");
  legend->AddEntry(P3,"B2","l");
  legend->AddEntry(P6,"B3 ","l");
  legend->AddEntry(P9,"F1","l");
  legend->AddEntry(P12,"F2 ","l");
  legend->AddEntry(P15,"F3","l");
  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  legend->Draw("same");       


  C1.Print(inpath+"/ProfileXcombined.png");


  TCanvas C2;

  TProfile* P30 = H.ProfileY();
  P30->SetLineColor(1);
  TProfile* P31 = H3.ProfileY();
  P31->SetLineColor(2);
  TProfile* P32 = H6.ProfileY();
  P32->SetLineColor(3);
  TProfile* P33 = H9.ProfileY();
  P33->SetLineColor(4);
  TProfile* P34 = H12.ProfileY();
  P34->SetLineColor(5);
  TProfile* P35 = H15.ProfileY();
  P35->SetLineColor(6);


  P30->Draw();
  P31->Draw("same");
  P32->Draw("same");
  P33->Draw("same");
  P34->Draw("same");
  P35->Draw("same");


  C2.Print(inpath+"/ProfileYcombined.png");



 
  gROOT->ProcessLine(".q");
}
