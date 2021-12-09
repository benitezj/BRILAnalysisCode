#include"globals.h" 
#include <iostream>
#include <vector>
#include <dirent.h>
void weights_2018D_early_B() {
  
  TString Path1 = "/eos/user/a/asehrawa/modcountgraphs_goodmodules/D/"; 
  TCanvas C("C");
  C.cd();
  gStyle->SetOptStat(1111111);
  
  std::vector<int> run_number= {320500, 320569, 320570, 320571, 320612, 320617, 320654, 320673, 320674, 320688, 320712, 320757, 320804, 320807, 320809, 320821, 320822, 320823, 320824, 320838, 320840, 320841, 320853, 320854, 320855, 320856, 320857, 320858, 320859, 320887, 320888, 320917, 320920, 320933, 320934, 320936, 320941, 320980, 320995, 320996, 321004, 321005, 321006, 321007, 321009, 321010, 321011, 321012, 321051,321055, 321067, 321068, 321069, 321078, 321119, 321121, 321122, 321123, 321124, 321126, 321134, 321138, 321140, 321149, 321162, 321164, 321165, 321166, 321167, 321177, 321178, 321218, 321219, 321221, 321230, 321231, 321232, 321233, 321261, 321262, 321283, 321294, 321295, 321296, 321305, 321310, 321311, 321312, 321313, 321383, 321386, 321388, 321393, 321396, 321397, 321414, 321415, 321431, 321432, 321433, 321434, 321436 , 321457, 321461, 321475, 321528, 321536, 321586, 321607, 321636, 321640, 321650, 321652, 321653, 321665, 321710, 321712, 321730, 321731, 321732, 321735, 321755, 321758, 321759, 321760, 321773, 321774, 321775, 321776, 321777, 321778, 321780, 321781, 321794, 321796, 321813, 321815, 321817, 321818, 321819, 321820, 321831, 321832, 321833, 321834, 321879, 321880, 321887, 321908, 321909, 321917, 321919, 321933, 321949, 321960, 321961, 321973, 321975, 321988, 321990, 322013, 322014, 322022, 322040, 322049, 322057, 322068, 322079, 322088, 322106, 322113, 322118, 322165, 322167, 322179, 322201, 322204, 322222, 322252, 322317, 322319, 322322, 322324, 322332, 322348, 322355, 322356, 322381, 322407, 322430,322431, 322480, 322483, 322484, 322485, 322487, 322492, 322510, 322599, 322602, 322603, 322605, 322616, 322617, 322625, 322633, 322958, 322959, 322961, 322963, 322964, 323363, 323364, 323365, 323367, 323368, 323369, 323370, 323371, 323373, 323374, 323375, 323376, 323377, 323378, 323388, 323391, 323393, 323394, 323395, 323396, 323397, 323398, 323399, 323414, 323415, 323416, 323417, 323418, 323419, 323420, 323421, 323422 , 323423, 323470, 323471, 323472, 323473, 323474, 323475, 323487, 323488, 323492, 323493, 323495, 323524, 323525, 323526, 323693, 323696, 323698, 323699};
 
  std::cout<<run_number.size()<<std::endl;
  
  readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/Run2018D_early_vetolist.txt");
 
  int LS=0;
  int run=0;
  int ls=0;
  int previousrunlumisec_count=0;
  
  TGraph *g1;
  g1=new TGraph();

  TH2F *histo_counts[1856];
  for (unsigned int i=0;i<1856;i++){
    histo_counts[i]=new TH2F(TString("Histo_counts")+i,"", 1400,0,140000,300,0,0.016);
  }
  
  int modid[1856];
  for (unsigned int j=0;j<run_number.size();j++){
    TString Path = "/eos/user/a/asehrawa/BRIL-PCC_25Sep2021/ZeroBias/Run2018D"; 
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
	if(totcount > 70000000){
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
      std::cout<<g1->GetN()<<" "<<i<<" "<<ProjY[i]->GetMean()<<std::endl;
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
      std::cout<<g2->GetN()<<" "<<i<<" "<<ProjY1[i]->GetMean()<<std::endl;
    }
  }
  
  TGraph *g3;
  g3=new TGraph();
  TH1D *h;
  h=new TH1D("histo1D", "weights (D-B)/B",100,-0.05,0.05);
  for (unsigned int i=0;i<1856;i++){
    if(MODVETO[modid1[i]]==0){
      g3->SetPoint(g3->GetN(), i, ((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean()));
      std::cout<<g3->GetN()<<" "<<i<<" "<<((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean())<<std::endl;
      h->Fill(((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean()));
    }
  }
  
  //for (unsigned int i=0;i<1856;i++){
  //if(MODVETO[modid1[i]]==0){
  //  if(((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean())<-0.01 || ((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean())>0.01){
  //	std::cout<<modid1[i]<<std::endl;
  //  }
  // }
  //}
  
  g1->SetMarkerStyle(8);
  g1->SetMarkerSize(0.4);
  g1->SetMarkerColor(1);
  g1->GetXaxis()->SetTitle("module number index");
  g1->GetYaxis()->SetTitle("module weights");
  g1->SetTitle(TString("Run2018D_early (Black)") +" & "+ TString("Run2018B (Red)"));
  g2->SetMarkerStyle(8);
  g2->SetMarkerSize(0.4);
  g2->SetMarkerColor(2);
  g3->SetMarkerStyle(8);
  g3->SetMarkerSize(0.4);
  g3->SetMarkerColor(4);
  g3->GetYaxis()->SetRangeUser(-0.05, 0.05);
  g3->GetXaxis()->SetTitle("module number index");
  g3->GetYaxis()->SetTitle("module weights (D-B)/B");
  h->GetXaxis()->SetTitle("weights (D-B)/B");
  h->GetYaxis()->SetTitle("entries");
  g1->Draw("AP");
  g2->Draw("PSAME");
  C.Print(Path1+"Graphs_cut_totcount/"+"module_weight_D_early_B_testing"+".png");

  g3->Draw("AP");
  C.Print(Path1+"Graphs_cut_totcount/"+"module_weight_(D-B)_early_B_testing"+".png");
  
  h->Draw();
  C.Print(Path1+"Graphs_cut_totcount/"+"module_weight_(D-B)_early_B_histogram_testing"+".png");
  
}
