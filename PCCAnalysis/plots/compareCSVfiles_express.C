#include"globals.h" 
#include <iostream>
#include <vector>
#include <dirent.h>
void compareCSVfiles_express() {
  
  TCanvas*C = new TCanvas("Luminosity ratio (new old veto)","");
  C->cd();
  TString Path1="/eos/user/a/asehrawa/BRIL-new/";
  
  std::vector<int> run_number = {315252, 315255, 315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315533, 315543, 315555, 315556, 315557, 315587, 315640, 315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785, 315786, 315787, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062, 316082, 316095, 316109, 316110, 316111, 316112, 316113, 316114, 316151, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219, 316239, 316240, 316241, 316260, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316456, 316457, 316469, 316470, 316472, 316477, 316478, 316505, 316569, 316590, 316613, 316615, 316664, 316665, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723, 316758, 316766, 316797, 316798, 316799, 316808, 316820, 316839, 316851, 316876,316877, 316878, 316879, 316928, 316944, 316985, 316993, 316994, 316995};
  
  //std::vector<int> run_number = {316186};
  //std::cout<< run_number.size()<<std::endl;
  
  TH1F *LumiperLS;
  LumiperLS=new TH1F("h", "histogram_per_ls", 59708, 0.0, 59708);
  
  TGraph *LumiLSratio;
  LumiLSratio=new TGraph();
  
  TH1F *LumiperLS1;
  LumiperLS1=new TH1F("h1", "histogram_per_ls1", 59708, 0.0, 59708); 
  
  int LS=0;
  int previousrunlumisec_count=0;

  int LS1=0;
  int previousrunlumisec_count1=0;

  std::map<int, float> cluster_count;

  for (unsigned int j=0;j<run_number.size();j++){
    TString Path = "/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018A";
    TString infile=Path+"/"+run_number.at(j)+".csv"; 
    //std::cout<< run_number.at(j)<<std::endl;  
    
    ifstream myfile (infile.Data());    
    if (!myfile.is_open()){
      cout << "Unable to open new file: "<<infile.Data()<<endl; 
      return;
    }    
        
    TString Path1 = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018A";
    TString infile1=Path1+"/"+run_number.at(j)+".csv";
    //std::cout<< run_number1.at(j)<<std::endl;                                                                                               
    ifstream myfile1 (infile1.Data());
    if (!myfile1.is_open()){
      cout << "Unable to open old file: "<<infile1.Data()<<endl;
      return;
    }
    
    std::string line;
    int run=0;
    int ls=0;
    float LumiLS=0;
    int lumisec_count=0;
    while (std::getline(myfile, line)){
      std::stringstream iss(line);
      std::string token;
      
      std::getline(iss,token, ',');
      std::stringstream runiss(token);
      runiss>>run;
      
      std::getline(iss,token, ',');
      std::stringstream lsiss(token);
      lsiss>>ls;

      lumisec_count++;      
      LS = previousrunlumisec_count + ls;      

      std::getline(iss,token, ',');
      std::stringstream lumiiss(token);
      lumiiss>>LumiLS;
      cluster_count[ls]=LumiLS;
      //std::cout<<"oldveto "<<run<< "  "<<ls<<"  "<<cluster_count[ls]<<std::endl;   

      LumiperLS->Fill(LS, LumiLS);
      std::cout<<"old veto "<<run<< "  "<<LS<<"  "<<LumiLS<<std::endl;
      
    }
    
    previousrunlumisec_count+=lumisec_count; 
    myfile.close();     

    std::string line1;  
    int run1=0;
    int ls1=0;
    float LumiLS1=0;
    int lumisec_count1=0;
    while (std::getline(myfile1, line1)){
      std::stringstream iss1(line1);
      std::string token1;
      
      std::getline(iss1,token1, ',');
      std::stringstream runiss1(token1);
      runiss1>>run1;
      
      std::getline(iss1,token1, ',');
      std::stringstream lsiss1(token1);
      lsiss1>>ls1;
      
      lumisec_count1++;
      LS1 = previousrunlumisec_count1 + ls1;

      std::getline(iss1,token1, ',');
      std::stringstream lumiiss1(token1);
      lumiiss1>>LumiLS1;
      
      LumiperLS1->Fill(LS1, LumiLS1);
      std::cout<<"newveto "<<run1<< "  "<<LS1<<"  "<<LumiLS1<<std::endl;
      
      if((run==run1) && (ls==ls1)){
	if (LumiLS>=2200000 && LumiLS1>=1900000){
	  if(LumiLS!=0){
	    LumiLSratio->SetPoint(LumiLSratio->GetN(), ls, LumiLS1/LumiLS);
	    std::cout<< "ratio "<<LS1<<"  "<< LumiLS1/LumiLS <<std::endl;
	  }
	}
      }
    }
    
    previousrunlumisec_count1+=lumisec_count1;    
    myfile1.close();
    
  }
  

    LumiperLS->Draw("histp");
    C->Print(Path1+"Lumiperls_oldveto"+".png");

    LumiperLS1->Draw("histp");
    C->Print(Path1+"Lumiperls_newveto"+".png");

    LumiLSratio ->SetTitle("Run2018A (315252-316995)");
    //LumiLSratio ->SetTitle("run number 316186");
    LumiLSratio ->GetXaxis()->SetTitle("Lumi section");                                                                                 
    LumiLSratio ->GetYaxis()->SetTitle("Lumiratio");
    LumiLSratio ->GetXaxis()->SetRangeUser(0,2200);
    //LumiLSratio ->GetYaxis()->SetRangeUser(0.6, 0.8);
    LumiLSratio ->Draw("AP"); 
    C->Print(Path1+"Lumiratio"+".png");
    

}
  
