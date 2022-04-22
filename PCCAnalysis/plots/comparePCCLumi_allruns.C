#include "globals.h"
void comparePCCLumi_allruns(){
  
  TString Path="/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018A_newveto/";
  
  //std::vector<int> run_number = {315252, 315255, 315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315533, 315543, 315555, 315556, 315557, 315587, 315640,315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785, 315786, 315787, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062, 316082, 316095, 316109, 316110, 316111, 316112, 316113, 316114, 316151, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219, 316239, 316240, 316241, 316260, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316456, 316457, 316469, 316470, 316472, 316477, 316478, 316505, 316569, 316590, 316613, 316615, 316664, 316665, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723, 316758,316766, 316797, 316798, 316799, 316808, 316820, 316839, 316851, 316876,316877, 316878, 316879, 316928, 316944, 316985, 316993, 316994, 316995}; 

  std::vector<int> run_number = {315690};

  //std::vector<int> run_number = {317080, 317087, 317088, 317089, 317170, 317182, 317212, 317213, 317239, 317279,317291, 317292, 317295, 317296, 317297, 317319, 317320,317338, 317339, 317340, 317382, 317383, 317391, 317392, 317434, 317435, 317438, 317475, 317478, 317479, 317480, 317481, 317482, 317484, 317488, 317509, 317510, 317511, 317512, 317527, 317580, 317591, 317626, 317640, 317641, 317648,317649, 317650, 317661, 317663, 317683, 317696, 318070, 318622, 318653, 318661, 318662, 318663, 318667, 318669, 318670, 318675, 318712,318714, 318733, 318734, 318785, 318816, 318817, 318819, 318820, 318828, 318834, 318836, 318837, 318872, 318874, 318876, 318877, 318939, 318944, 318945, 318953, 318980, 318981, 318982, 318983, 318984, 318992, 319006, 319011, 319015, 319016, 319018, 319019, 319077, 319097, 319098, 319099, 319100, 319101, 319103, 319104, 319124, 319125, 319159, 319160, 319173, 319174, 319175, 319176, 319177, 319190, 319222, 319223, 319254, 319255, 319256, 319260, 319262, 319263, 319264, 319265, 319266, 319267, 319268, 319270, 319273, 319274, 319300, 319310, 319311};


  //std::vector<int> run_number = {319337, 319347, 319348, 319349, 319449, 319450, 319452, 319456, 319459, 319460, 319462, 319463, 319464, 319466, 319467, 319468, 319469, 319470, 319471, 319472, 319486, 319488, 319503, 319506, 319524, 319525, 319526, 319528, 319579, 319606, 319625, 319639, 319656, 319657, 319658, 319659, 319678, 319687, 319697, 319698, 319756, 319840, 319841, 319847, 319848, 319849, 319851, 319852, 319853, 319854, 319908, 319909, 319910, 319912, 319913, 319914, 319915, 319928, 319941, 319942, 319950, 319991, 319992, 319993, 320002, 320003, 320006, 320007, 320008, 320009, 320010, 320011, 320012, 320023, 320024, 320025, 320026, 320038, 320039, 320040, 320058, 320059, 320060, 320061 , 320062, 320063, 320064, 320065}; 

  std::cout<<run_number.size()<<std::endl;
  
  TH1F*HCSV;
  TH1F*HCSV1;

  for (unsigned int j=0;j<run_number.size();j++){
  
    std::cout<<run_number.at(j)<<std::endl;
  
    TString Path1="/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018A";
    //TString Path1="/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018B";
    //TString Path1="/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018C";
 
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
    //TString Path2="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018B";
    //TString Path2="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018C";
  
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
    //HCSV->GetYaxis()->SetRangeUser(0,4000000);
    //HCSV->SetTitle("Run2018A (315255 - 316995)");
    HCSV->SetTitle("run number 315690");
    //HCSV->SetTitle("Run2018B (317080 - 319311)");                                                                                         
    //HCSV->SetTitle("Run2018C (319337 - 320065)"); 
    HCSV->GetXaxis()->SetTitle("bx");
    //HCSV->GetYaxis()->SetTitle("PCC");
    HCSV->GetYaxis()->SetTitle("New/Old");
    //HCSV->GetYaxis()->SetRangeUser(0, 70000);
    //HCSV->GetYaxis()->SetRangeUser(0.716, 0.73);
    //HCSV->SetTitle("Zero Bias data with afterglow corrections (run number 316110)");
    //HCSV->SetTitle("Run Number 317696");
    //HCSV->SetTitle("Run Number 319696");
    
    HCSV1->SetMarkerStyle(8);
    HCSV1->SetLineColor(2);
    HCSV1->SetMarkerColor(2);
    HCSV1->SetMarkerSize(0.5);
    HCSV1->GetXaxis()->SetTitle("bx");
    //HCSV1->GetYaxis()->SetTitle("PCC");
    gStyle->SetOptStat(0);    
    
    HCSV->Divide(HCSV1);
    HCSV->Draw("p");
    //HCSV1->Draw("psame");
    
    auto legend = new TLegend(0.75, 0.75, 0.88, 0.88);    
    legend->AddEntry(HCSV, "New veto", "l");
    legend->AddEntry(HCSV1, "Old veto", "l");
    legend->SetFillColor(0);
    legend->SetLineColor(0);
    legend->SetFillColor(0);
    legend->Draw("same");
  
    C.Print(Path+TString("csv_file_lumi_comparison_RunA_ratio_315690")+".root");
    
    //gROOT->ProcessLine(".q");
    
}
