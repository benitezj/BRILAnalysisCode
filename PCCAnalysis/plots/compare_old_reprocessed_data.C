#include "globals.h"

TH1F * readCSV(TString CSV){

  TH1F*HCSV = new TH1F(TString("HCSV_"),"",NBX,0.5,NBX+0.5);

  ifstream csvfile(CSV.Data());
  if (!csvfile.is_open()){
    std::cout << "Unable to open csvfile "<<CSV.Data()<<std::endl;
    return 0;
  }

  std::string line;
  int run=0;
  int ls=0;
  float Lumi = 0.;
  while (std::getline(csvfile,line)){
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
    lumiiss>>Lumi;

    //if(ls<ls1 || ls>ls2) continue;

    cout<<"new "<<run<<" "<<ls<<" "<<Lumi<<": "<<endl;

    for(int b=1;b<=NBX;b++){
      std::getline(iss,token, ',');
      std::stringstream biss(token);
      biss>>Lumi;
      HCSV->AddBinContent(b,Lumi);
    }

  }
  csvfile.close();

  return HCSV;
}


TH1F * readCSV1(TString CSV1){

  TH1F*HCSV1 = new TH1F(TString("HCSV1_"),"",NBX,0.5,NBX+0.5);

  ifstream csvfile1(CSV1.Data());
  if (!csvfile1.is_open()){
    std::cout << "Unable to open csvfile1 "<<CSV1.Data()<<std::endl;
    return 0;
  }

  std::string line1;
  int run1=0;
  int ls1=0;
  float Lumi1 = 0.;
  while (std::getline(csvfile1,line1)){
    std::stringstream iss1(line1);
    std::string token1;
    std::getline(iss1,token1, ',');
    std::stringstream runiss1(token1);
    runiss1>>run1;
    std::getline(iss1,token1, ',');
    std::stringstream lsiss1(token1);
    lsiss1>>ls1;
    std::getline(iss1,token1, ',');
    std::stringstream lumiiss1(token1);
    lumiiss1>>Lumi1;

    //if(ls<ls1 || ls>ls2) continue;

    cout<<"old "<<run1<<" "<<ls1<<" "<<Lumi1<<": "<<endl;

    for(int b1=1;b1<=NBX;b1++){
      std::getline(iss1,token1, ',');
      std::stringstream biss1(token1);
      biss1>>Lumi1;
      HCSV1->AddBinContent(b1,Lumi1);
    }

  }
  csvfile1.close();

  return HCSV1;
}

void compare_old_reprocessed_data(){

  TString Path="/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018A_newveto/";

  TString CSV="/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018A_newveto/ZeroBias/Run2018A_ZB/316110.csv";
  TString CSV1="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018A/316110.csv";

  //TString CSV="/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018B_newveto/ZeroBias/Run2018B_ZB/317696.csv";
  //TString CSV1="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018B/317696.csv";

  //TString CSV="/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018C_newveto/ZeroBias/Run2018C_ZB/319639.csv";
  //TString CSV1="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018C/319639.csv";

  gStyle->SetOptStat(0);

  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");
  TCanvas C("C","",1200,500);
    
    TH1F* HCSV=readCSV(CSV);
    //HCSV->Scale(1./HCSV->GetMaximum());

    TH1F* HCSV1=readCSV1(CSV1);
    //HCSV1->Scale(1./HCSV1->GetMaximum());  
  
    C.Clear();
    HCSV->SetMarkerStyle(8);
    HCSV->SetLineColor(1);
    HCSV->SetMarkerColor(1);
    HCSV->SetMarkerSize(0.5);
    HCSV->GetXaxis()->SetTitle("bx");
    HCSV->GetYaxis()->SetTitle("PCC Count");
    //HCSV->GetYaxis()->SetTitle("New/Old");
    //HCSV->GetYaxis()->SetRangeUser(0, 1400000);
    
    //HCSV->GetYaxis()->SetRangeUser(0.5, 1);
    HCSV->SetTitle("Zero Bias data with afterglow corrections (run number 316110)");
    //HCSV->SetTitle("Run Number 317696");
    //HCSV->SetTitle("Run Number 319696");

    
    HCSV1->SetMarkerStyle(8);
    HCSV1->SetLineColor(2);
    HCSV1->SetMarkerColor(2);
    HCSV1->SetMarkerSize(0.5);
    HCSV1->GetXaxis()->SetTitle("bx");
    HCSV1->GetYaxis()->SetTitle("PCC Count");
    gStyle->SetOptStat(0);    
    
    //HCSV->Divide(HCSV1);
    HCSV->Draw("p");
    HCSV1->Draw("psame");

    auto legend = new TLegend(0.75, 0.75, 0.88, 0.88);
    //legend->AddEntry(HCSV, "New PCC Count", "l");
    //legend->AddEntry(HCSV1, "Old PCC Count", "l");

    legend->AddEntry(HCSV, "New veto", "l");
    legend->AddEntry(HCSV1, "Old veto", "l");

    legend->SetFillColor(0);
    legend->SetLineColor(0);
    legend->SetFillColor(0);
    legend->Draw("same");

    C.Print(Path+TString("csv_file_lumi_comparison_RunA")+".root");
    
  gROOT->ProcessLine(".q");
}
