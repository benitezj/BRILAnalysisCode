#include"globals.h" 
#include <iostream>
#include <vector>
#include <dirent.h>
void weights_2018A_B() {
  
  TString Path1 = "/eos/user/a/asehrawa/modcountgraphs_goodmodules/A/"; 
  TCanvas C("C");
  C.cd();
  gStyle->SetOptStat(1111111);
  
  std::vector<int> run_number = {315252, 315255, 315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315533, 315543, 315555, 315556, 315557, 315587, 315640, 315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785, 315786, 315787, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062, 316082, 316095, 316109, 316110, 316111, 316112, 316113, 316114, 316151, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219, 316239, 316240, 316241, 316260, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316456, 316457, 316469, 316470, 316472, 316477, 316478, 316505, 316569, 316590, 316613, 316615, 316664, 316665, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723, 316758, 316766, 316797, 316798, 316799, 316808, 316820, 316839, 316851, 316876,316877, 316878, 316879, 316928, 316944, 316985, 316993, 316994, 316995};
  
  std::cout<<run_number.size()<<std::endl;
  
  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/Run2018A_vetolist_second_iteration.txt");
  readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/veto_RunA_0p02_3sigma_weight_cut.txt");


  int LS=0;
  int run=0;
  int ls=0;
  int previousrunlumisec_count=0;
  
  TGraph *g1;
  g1=new TGraph();

  TH2F *histo_counts[1856];
  for (unsigned int i=0;i<1856;i++){
    histo_counts[i]=new TH2F(TString("Histo_counts")+i,"", 1400,0,70000,300,0,0.016);
  }
  
  int modid[1856];
  for (unsigned int j=0;j<run_number.size();j++){
    TString Path = "/eos/user/a/asehrawa/BRIL-PCC_25Sep2021/ZeroBias/Run2018A"; 
    TString infile=Path+"/"+run_number.at(j)+".csv"; 
    std::cout<< run_number.at(j)<<std::endl;  
    
    ifstream myfile (infile.Data());    
    if (!myfile.is_open()){
      cout << "Unable to open file: "<<infile.Data()<<endl; 
      return;
    }
    
    std::string line;    
    std::getline(myfile, line);
    std::stringstream iss(line);
    std::string token;
    
    for (unsigned int i=0;i<1856;i++){
      std::getline(iss,token, ',');
      std::stringstream modidiss(token);
      modidiss>>modid[i];
      
    }
    
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
      //std::cout<<run <<" ";
      //std::cout<< ls;
      lumisec_count++;      
      LS = previousrunlumisec_count + ls; 
      
      int m_count[1856];
      int count=0;
      float totcount=0;      
      for (unsigned int i=0;i<1856;i++){
	std::getline(iss,token, ',');
	std::stringstream modidiss(token);
        modidiss>>count;
	m_count[i]=count;        
	if(MODVETO[modid[i]]==0){                                                                               
	  totcount+=count;
	}
      }
      
      for (unsigned int i=0;i<1856;i++){
	if(totcount > 50000000){
	  if(MODVETO[modid[i]]==0){
	    histo_counts[i]->Fill(LS, m_count[i]/totcount);
	  }
	}
      }
    }
    
    previousrunlumisec_count+=lumisec_count;  
    myfile.close();    
    
  }
  
  
  TH1D* ProjY[1856];
  for (unsigned int i=0;i<1856;i++){
    if(MODVETO[modid[i]]==0){ 
      ProjY[i] = histo_counts[i]->ProjectionY(TString("Projection_")+i,0,1400);
      g1->SetPoint(g1->GetN(), i, ProjY[i]->GetMean());
      //std::cout<<g1->GetN()<<" "<<i<<" "<<ProjY[i]->GetMean()<<std::endl;
    }
  }
  
  
  std::vector<int> run_number1 = {317080, 317087, 317088, 317089, 317170, 317182, 317212, 317213, 317239, 317279,317291, 317292, 317295, 317296, 317297, 317319, 317320,317338, 317339, 317340, 317382, 317383, 317391, 317392, 317434, 317435, 317438, 317475, 317478, 317479, 317480, 317481, 317482, 317484, 317488, 317509, 317510, 317511, 317512, 317527, 317580, 317591, 317626, 317640, 317641, 317648,317649, 317650, 317661, 317663, 317683, 317696, 318070, 318622, 318653, 318661, 318662, 318663, 318667, 318669, 318670, 318675, 318712,318714, 318733, 318734, 318785,318816, 318817, 318819, 318820, 318828, 318834, 318836, 318837, 318872, 318874, 318876, 318877, 318939, 318944, 318945, 318953, 318980, 318981, 318982, 318983, 318984, 318992, 319006, 319011, 319015, 319016, 319018, 319019, 319077, 319097, 319098, 319099, 319100, 319101, 319103, 319104, 319124, 319125, 319159, 319160, 319173, 319174, 319175, 319176, 319177, 319190, 319222, 319223, 319254, 319255, 319256, 319260, 319262, 319263, 319264, 319265, 319266, 319267, 319268, 319270, 319273, 319274, 319300, 319310, 319311};
  
  std::cout<<run_number1.size()<<std::endl;

  int LS1=0;
  int run1=0;
  int ls1=0;
  int previousrunlumisec_count1=0;
  
  TGraph *g2;
  g2=new TGraph();

  TH2F *RunB_histo[1856];
  for (unsigned int i=0;i<1856;i++){
    RunB_histo[i]=new TH2F(TString("RunB_histogram")+i,"", 700,0,35000,300,0,0.016);
  }
  
  int modid1[1856];
  for (unsigned int j=0;j<run_number1.size();j++){
    TString Path1 = "/eos/user/a/asehrawa/BRIL-PCC_25Sep2021/ZeroBias/Run2018B"; 
    TString infile1=Path1+"/"+run_number1.at(j)+".csv"; 
    std::cout<< run_number1.at(j)<<std::endl;  
    
    ifstream myfile1 (infile1.Data());    
    if (!myfile1.is_open()){
      cout << "Unable to open file: "<<infile1.Data()<<endl; 
      return;
    }
    
    std::string line1;    
    std::getline(myfile1, line1);
    std::stringstream iss1(line1);
    std::string token1;
    
    for (unsigned int i=0;i<1856;i++){
      std::getline(iss1,token1, ',');
      std::stringstream modidiss1(token1);
      modidiss1>>modid1[i];
      
    }
    
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
      //std::cout<<run <<" ";
      //std::cout<< ls;
      lumisec_count1++;      
      LS1 = previousrunlumisec_count1 + ls1; 
      
      int m_count1[1856];
      int count1=0;
      float totcount1=0;      
      for (unsigned int i=0;i<1856;i++){
	std::getline(iss1,token1, ',');
	std::stringstream modidiss1(token1);
        modidiss1>>count1;
	m_count1[i]=count1;        
	if(MODVETO[modid1[i]]==0){                                                                               
	  totcount1+=count1;
	  
	}
      }
      
      
      for (unsigned int i=0;i<1856;i++){
	if(totcount1 > 100000000){
	  if(MODVETO[modid1[i]]==0){
	    RunB_histo[i]->Fill(LS1, m_count1[i]/totcount1);
	  }
	}
      }
    }
    
    previousrunlumisec_count1+=lumisec_count1;  
    myfile1.close();    
    
  }
  
  
  TH1D* ProjY1[1856];
  for (unsigned int i=0;i<1856;i++){
    if(MODVETO[modid1[i]]==0){ 
      ProjY1[i] = RunB_histo[i]->ProjectionY(TString("RunB_Projection_")+i,0,700);
      g2->SetPoint(g2->GetN(), i, ProjY1[i]->GetMean());
      //std::cout<<g2->GetN()<<" "<<i<<" "<<ProjY1[i]->GetMean()<<std::endl;
    }
  }
  
  TGraph *g3;
  g3=new TGraph();
  TH1D *h;
  h=new TH1D("histo1D", "weights (A-B)/B",100,-0.05,0.05);
  for (unsigned int i=0;i<1856;i++){
    if(MODVETO[modid1[i]]==0){
      g3->SetPoint(g3->GetN(), i, ((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean()));
      //std::cout<<g3->GetN()<<" "<<i<<" "<<((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean())<<std::endl;
      h->Fill(((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean()));
    }
  }
  
  for (unsigned int i=0;i<1856;i++){
    if(MODVETO[modid1[i]]==0){
      if(((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean())<-0.01 || ((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean())>0.01){
  	//std::cout<<modid1[i]<<std::endl;
      }
    }
  }


  for (unsigned int i=0;i<1856;i++){
    if(MODVETO[modid1[i]]==0){
      if(((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean())<6.97110e-04-3*2.70604e-03 || ((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean())>6.97110e-04+3*2.70604e-03){
        std::cout<<modid1[i]<<std::endl;                                                                                                    
      }
    }
  }


  
  g1->SetMarkerStyle(8);
  g1->SetMarkerSize(0.4);
  g1->SetMarkerColor(1);
  g1->GetXaxis()->SetTitle("module number index");
  g1->GetYaxis()->SetTitle("module weights");
  g1->SetTitle(TString("Run2018A (Black)") +" & "+ TString("Run2018B (Red)"));
  g2->SetMarkerStyle(8);
  g2->SetMarkerSize(0.4);
  g2->SetMarkerColor(2);
  g3->SetMarkerStyle(8);
  g3->SetMarkerSize(0.4);
  g3->SetMarkerColor(4);
  g3->GetYaxis()->SetRangeUser(-0.05, 0.05);
  g3->GetXaxis()->SetTitle("module number index");
  g3->GetYaxis()->SetTitle("module weights (A-B)/B");
  h->GetXaxis()->SetTitle("weights (A-B)/B");
  h->GetYaxis()->SetTitle("entries");
  g1->Draw("AP");
  g2->Draw("PSAME");
  C.Print(Path1+"Graphs_cut_totcount/"+"module_weight_A_B_testing"+".png");

  g3->Draw("AP");
  C.Print(Path1+"Graphs_cut_totcount/"+"module_weight_(A-B)_B_testing"+".png");
  
  h->Draw();
  C.Print(Path1+"Graphs_cut_totcount/"+"module_weight_(A-B)_B_histogram_testing"+".png");
  
}
