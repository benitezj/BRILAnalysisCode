#include "globals.h"

void comparePCCLumi_allruns(){
  
  TString Path="/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018A_newveto/";
  
  std::vector<int> run_number = {315252, 315255, 315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315533, 315543, 315555, 315556, 315557, 315587, 315640,315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, //315690, 
				 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785, 315786, 315787, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062, 316082, 316095, 316109, 316110, 316111, 316112, 316113, 316114, 316151, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219, 316239, 316240, 316241, 316260, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316456, 316457, 316469, 316470, 316472, 316477, 316478, 316505, //316569, 316590, 316613, 316615, 316664, 316665, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723, 316758, 
316766, 316797, 316798, 316799, 316808, 316820, 316839, 316851, 316876,316877, 316878, 316879, 316928, //316944, 
316985, 316993, 316994, 316995}; 

  //std::vector<int> run_number = {315252, 315255, 315257};

  std::cout<<run_number.size()<<std::endl;
  
  
  TH1F*HCSV;
  TH1F*HCSV1;

  for (unsigned int j=0;j<run_number.size();j++){
  
    std::cout<<run_number.at(j)<<std::endl;  
    TString Path1="/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018A_newveto/ZeroBias/Run2018A_ZB";
 
    TString CSV=Path1+"/"+run_number.at(j)+".csv";
        
    HCSV = new TH1F(TString("HCSV_")+j,"",NBX,0.5,NBX+0.5);
    
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
      
      //cout<<"new "<<run<<" "<<ls<<" "<<Lumi<<": "<<endl;
      
      for(int b=1;b<=NBX;b++){
	std::getline(iss,token, ',');
	std::stringstream biss(token);
	biss>>Lumi;
	HCSV->AddBinContent(b,Lumi);
      }
      
    }
    
    csvfile.close();
    

    TString Path2="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018A";  
    TString CSV1=Path2+"/"+run_number.at(j)+".csv";
    
    HCSV1 = new TH1F(TString("HCSV1_")+j,"",NBX,0.5,NBX+0.5);
    
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
      
      //cout<<"old "<<run1<<" "<<ls1<<" "<<Lumi1<<": "<<endl;
      
      for(int b1=1;b1<=NBX;b1++){
	std::getline(iss1,token1, ',');
	std::stringstream biss1(token1);
	biss1>>Lumi1;
	HCSV1->AddBinContent(b1,Lumi1);
      }
      
    }
    
    csvfile1.close();
    
  }

    gStyle->SetOptStat(0);
    
    //gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");
    TCanvas C("C","",1200,500);
    
    //C.Clear();
    HCSV->SetMarkerStyle(8);
    HCSV->SetLineColor(1);
    HCSV->SetMarkerColor(1);
    HCSV->SetMarkerSize(0.5);
    HCSV->GetXaxis()->SetTitle("bx");
    HCSV->GetYaxis()->SetTitle("PCC Count");
    //HCSV->GetYaxis()->SetTitle("New/Old");
    //HCSV->GetYaxis()->SetRangeUser(0, 1400000);
    //HCSV->GetYaxis()->SetRangeUser(0.716, 0.73);
    //HCSV->SetTitle("Zero Bias data with afterglow corrections (run number 316110)");
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
    legend->AddEntry(HCSV, "New veto", "l");
    legend->AddEntry(HCSV1, "Old veto", "l");
    legend->SetFillColor(0);
    legend->SetLineColor(0);
    legend->SetFillColor(0);
    legend->Draw("same");
  
    C.Print(Path+TString("csv_file_lumi_comparison_RunA")+".png");
    
    //gROOT->ProcessLine(".q");
    
}
