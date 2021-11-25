#include"globals.h" 
#include <iostream>
#include <vector>
#include <dirent.h>
void modcount_2018B_copy_new() {
  
  TString Path1 = "/eos/user/a/asehrawa/modcountgraphs_goodmodules/B/";  
  TCanvas C("C");
  C.cd();
  //C.SetLogy();
  //C.SetCanvasSize(2000,600); 
  //gROOT->ProcessLine(".x /afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  //gStyle->SetOptStat(1111111);
  
  //std::vector<int> run_number = {315252, 315255, 315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315533, 315543, 315555, 315556, 315557, 315587, 315640, 315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785, 315786, 315787, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062, 316082, 316095, 316109, 316110, 316111, 316112, 316113, 316114, 316151, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219, 316239, 316240, 316241, 316260, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316456, 316457, 316469, 316470, 316472, 316477, 316478, 316505, 316569, 316590, 316613, 316615, 316664, 316665, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723, 316758, 316766, 316797, 316798, 316799, 316808, 316820, 316839, 316851, 316876,316877, 316878, 316879, 316928, 316944, 316985, 316993, 316994, 316995};
  
  std::vector<int> run_number = {317080, 317087, 317088, 317089, 317170, 317182, 317212, 317213, 317239, 317279,317291, 317292, 317295, 317296, 317297, 317319, 317320,317338, 317339, 317340, 317382, 317383, 317391, 317392, 317434, 317435, 317438, 317475, 317478, 317479, 317480, 317481, 317482, 317484, 317488, 317509, 317510, 317511, 317512, 317527, 317580, 317591, 317626, 317640, 317641, 317648,317649, 317650, 317661, 317663, 317683, 317696, 318070, 318622, 318653, 318661, 318662, 318663, 318667, 318669, 318670, 318675, 318712,318714, 318733, 318734, 318785,318816, 318817, 318819, 318820, 318828, 318834, 318836, 318837, 318872, 318874, 318876, 318877, 318939, 318944, 318945, 318953, 318980, 318981, 318982, 318983, 318984, 318992, 319006, 319011, 319015, 319016, 319018, 319019, 319077, 319097, 319098, 319099, 319100, 319101, 319103, 319104, 319124, 319125, 319159, 319160, 319173, 319174, 319175, 319176, 319177, 319190, 319222, 319223, 319254, 319255, 319256, 319260, 319262, 319263, 319264, 319265, 319266, 319267, 319268, 319270, 319273, 319274, 319300, 319310, 319311};
  
  //std::vector<int> run_number_vdM2018 = {318976, 318977, 318978, 318979, 318980, 318981, 318982, 318983, 318984, 318992, 319006, 319010, 319011, 319015, 319016, 319018, 319019, 319021};  
  
  //std::vector<int> run_number = {319337, 319347, 319348, 319349, 319449, 319450, 319452, 319456, 319459, 319460, 319462, 319463, 319464, 319466, 319467, 319468, 319469, 319470, 319471, 319472, 319486, 319488, 319503, 319506, 319524, 319525, 319526, 319528, 319579, 319606, 319625, 319639, 319656, 319657, 319658, 319659, 319678, 319687, 319697, 319698, 319756, 319840, 319841, 319847, 319848, 319849, 319851, 319852, 319853, 319854, 319908, 319909, 319910, 319912, 319913, 319914, 319915, 319928, 319941, 319942, 319950, 319991, 319992, 319993, 320002, 320003, 320006, 320007, 320008, 320009, 320010, 320011, 320012, 320023, 320024, 320025, 320026, 320038, 320039, 320040, 320058, 320059, 320060, 320061, 320062, 320063, 320064, 320065};
  
  //std::vector<int> run_number= {320500, 320569, 320570, 320571, 320612, 320617, 320654, 320673, 320674, 320688, 320712, 320757, 320804, 320807, 320809, 320821, 320822, 320823, 320824, 320838, 320840, 320841, 320853, 320854, 320855, 320856, 320857, 320858, 320859, 320887, 320888, 320917, 320920, 320933, 320934, 320936, 320941, 320980, 320995, 320996, 321004, 321005, 321006, 321007, 321009, 321010, 321011, 321012, 321051, 321055, 321067, 321068, 321069, 321078, 321119, 321121, 321122, 321123, 321124, 321126, 321134, 321138, 321140, 321149, 321162, 321164, 321165, 321166, 321167, 321177, 321178, 321218, 321219, 321221, 321230, 321231, 321232, 321233, 321261, 321262, 321283, 321294, 321295, 321296, 321305, 321310, 321311, 321312, 321313, 321383, 321386, 321388, 321393, 321396, 321397, 321414, 321415, 321431, 321432, 321433, 321434, 321436, 321457, 321461, 321475, 321528, 321536, 321586, 321607, 321636, 321640, 321650, 321652, 321653, 321665, 321710, 321712, 321730, 321731, 321732, 321735, 321755, 321758, 321759, 321760, 321773, 321774, 321775, 321776, 321777, 321778, 321780, 321781, 321794, 321796, 321813, 321815, 321817, 321818, 321819, 321820, 321831, 321832, 321833, 321834, 321879, 321880, 321887, 321908, 321909, 321917, 321919, 321933, 321949, 321960, 321961, 321973, 321975, 321988, 321990, 322013, 322014, 322022, 322040, 322049, 322057, 322068, 322079, 322088, 322106, 322113, 322118, 322165, 322167, 322179, 322201, 322204, 322222, 322252, 322317, 322319, 322322, 322324, 322332, 322348, 322355, 322356, 322381, 322407, 322430, 322431, 322480, 322483, 322484, 322485, 322487, 322492, 322510, 322599, 322602, 322603, 322605, 322616, 322617, 322625, 322633, 322958, 322959, 322961, 322963, 322964, 323363, 323364, 323365, 323367, 323368, 323369, 323370, 323371, 323373, 323374, 323375, 323376, 323377, 323378, 323388, 323391, 323393, 323394, 323395, 323396, 323397, 323398, 323399, 323414, 323415, 323416, 323417, 323418, 323419, 323420, 323421, 323422, 323423, 323470, 323471, 323472, 323473, 323474, 323475, 323487, 323488, 323492, 323493, 323495, 323524, 323525, 323526, 323693, 323696, 323698, 323699, 323700, 323702, 323725, 323726, 323727, 323755, 323775, 323778, 323790, 323794, 323841, 323857, 323940, 323954, 323976, 323978, 323980, 323983, 323997, 324021, 324022, 324077, 324078, 324201, 324202, 324205, 324206, 324207, 324209, 324237, 324245, 324293, 324315, 324318, 324420, 324564, 324571, 324612, 324729, 324747, 324764, 324765, 324769, 324772, 324785, 324791, 324835, 324840, 324841, 324846, 324878, 324897, 324970, 324980, 324997, 324998, 324999, 325000, 325001, 325022, 325057, 325097, 325098, 325099, 325100, 325101, 325110, 325111, 325113, 325114, 325117, 325159, 325168, 325169, 325170, 325172, 325175};
  
  //std::vector<int> run_number = {317080};  
  std::cout<<run_number.size()<<std::endl;
  readModRPhiZCoordinates();
  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/module_L1_rms_mean_gr_0.07_totcount10^8_first_iteration.txt");
  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/module_L1_rms_mean_gr_0.07_totcount10^8_second_iteration.txt");
  readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/module_L1_rms_mean_gr_0.07_totcount10^8_third_iteration.txt");
  
  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/test/veto_lateRunD_lowcut_tight_Run2018B.txt");
  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/test/veto_lateRunD_lowcut_tight1.txt");
  
  int LS=0;
  int run=0;
  int ls=0;
  int previousrunlumisec_count=0;
  
  TGraph *gr[1856];
  
  TGraph *gr1;
  TGraph *gr2;
  TGraph *gr3;
  TGraph *gr4;
  TGraph *gr5;
  TGraph *gr6;
  TGraph *gr7;
  TGraph *gr8;
  TGraph *gr9;  
  TH1D *h;
  TGraph *gr_modweight;
  
  TH2F *histo_counts[1856];
  
  TH2F *histo_L2;
  TH2F *histo_L3;
  TH2F *histo_L4;
  TH2F *histo_D1S1;
  TH2F *histo_D2S1;
  TH2F *histo_D3S1;
  TH2F *histo_D1S2;
  TH2F *histo_D2S2;
  TH2F *histo_D3S2;
  
  TProfile *P_L2;
  TProfile *P_L3;
  TProfile *P_L4;
  
  TProfile *P_D1S1;
  TProfile *P_D2S1;
  TProfile *P_D3S1;
  TProfile *P_D1S2;
  TProfile *P_D2S2;
  TProfile *P_D3S2;
  
  TH1D *ProjY1[1856];
  TH1D *ProjY2[1856];
  TH1D *ProjY[1856];  

  TH1D *ProjY_L2;
  TH1D *ProjY_L3;
  TH1D *ProjY_L4;
  
  TH1D *ProjY_D1S1;
  TH1D *ProjY_D2S1;
  TH1D *ProjY_D3S1;
  TH1D *ProjY_D1S2;
  TH1D *ProjY_D2S2;
  TH1D *ProjY_D3S2;
  
  gr1=new TGraph();
  gr2=new TGraph();
  gr3=new TGraph();
  gr4=new TGraph();
  gr5=new TGraph();
  gr6=new TGraph();
  gr7=new TGraph();
  gr8=new TGraph();
  gr9=new TGraph();
  h=new TH1D("histo1D", "rms/mean",100,0,0.5);
  gr_modweight=new TGraph();
  
  histo_L2=new TH2F("Histo_Layer2","",200,0,35000,300,0,0.35);
  histo_L3=new TH2F("Histo_Layer3","",200,0,35000,300,0,0.35);
  histo_L4=new TH2F("Histo_Layer4","",200,0,35000,300,0,0.35);
  histo_D1S1=new TH2F("Histo_Disk1S1","",200,0,35000,300,0,0.35);
  histo_D2S1=new TH2F("Histo_Disk2S1","",200,0,35000,300,0,0.35);
  histo_D3S1=new TH2F("Histo_Disk3S1","",200,0,35000,300,0,0.35);
  histo_D1S2=new TH2F("Histo_Disk1S2","",200,0,35000,300,0,0.35);
  histo_D2S2=new TH2F("Histo_Disk2S2","",200,0,35000,300,0,0.35);
  histo_D3S2=new TH2F("Histo_Disk3S2","",200,0,35000,300,0,0.35);
  
  for (unsigned int i=0;i<1856;i++){
    gr[i]=new TGraph();
    histo_counts[i]=new TH2F(TString("Histo_counts")+i,"", 700,0,35000,300,0,0.016);
    ProjY[i]=new TH1D(TString("Projection_")+i, "rms/mean",100,0,0.0015);
  }
  
  int modid[1856];
  for (unsigned int j=0;j<run_number.size();j++){
    TString Path = "/eos/user/a/asehrawa/BRIL-PCC_25Sep2021/ZeroBias/Run2018B"; 
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
      int count_L2=0;
      int count_L3=0;
      int count_L4=0;
      int count_D1S1=0;
      int count_D2S1=0;
      int count_D3S1=0;
      int count_D1S2=0;
      int count_D2S2=0;
      int count_D3S2=0;      
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
	  if(BPIXorFPIX[modid[i]]==1){	
	    if(LY[modid[i]]==2){
	      count_L2+=count;
	    }	    
	    if(LY[modid[i]]==3){
	      count_L3+=count;
	    }
	    if(LY[modid[i]]==4){
	      count_L4+=count;
	    }
	  }

	  if(BPIXorFPIX[modid[i]]==2){
	    if(DISK[modid[i]]==2){
	      count_D1S1+=count;
	    }	    
	    if(DISK[modid[i]]==1){
	      count_D2S1+=count;
	    } 
	    if(DISK[modid[i]]==0){
	      count_D3S1+=count;
	    } 	    
	    if(DISK[modid[i]]==3){
	      count_D1S2+=count;
	    }	    
	    if(DISK[modid[i]]==4){
	      count_D2S2+=count;
	    }    
	    if(DISK[modid[i]]==5){
	      count_D3S2+=count;
	      
	    }
	  }  
	}
      }
      
      
      
      //loop over modules
      for (unsigned int i=0;i<1856;i++){
	if(totcount > 100000000){
	if(MODVETO[modid[i]]==0){
	    histo_counts[i]->Fill(LS, m_count[i]/totcount);
	    //std::cout<<i<<" "<<countsum[i]<<" "<<totcountsum<<" "<<countsum[i]/totcountsum<<std::endl; 
	    //std::cout<<i<<" "<<m_count[i]/totcount<<std::endl;  
	    }
	   }
      }
      
      
      
      if(totcount > 100000000){
	histo_L2->Fill(LS, count_L2/totcount);
	histo_L3->Fill(LS, count_L3/totcount);
	histo_L4->Fill(LS, count_L4/totcount);
	histo_D1S1->Fill(LS, count_D1S1/totcount);
	histo_D2S1->Fill(LS, count_D2S1/totcount);
	histo_D3S1->Fill(LS, count_D3S1/totcount);
	histo_D1S2->Fill(LS, count_D1S2/totcount);
	histo_D2S2->Fill(LS, count_D2S2/totcount);
	histo_D3S2->Fill(LS, count_D3S2/totcount);
	gr9->SetPoint(gr9->GetN(), LS, totcount);
	//std::cout<<LS<<" "<< totcount<<std::endl;
	//std::cout<<run_number.at(j)<<std::endl;	
	}
      
      //if(totcount > 10000000) {
      //std::cout<<run_number.at(j)<<std::endl;
      //}
      
    }
     
    previousrunlumisec_count+=lumisec_count;  
    myfile.close();    
    
  }
  
  
  TProfile* P[1856];
  //TH1D* ProjY[1856];
  C.Clear();   
  bool var=0;
  int g_count=0;
  int g_plot=0;
  //auto legend = new TLegend(0.7,0.15,0.85,0.85);            
  auto legend = new TLegend(0.91,0.1,1,0.9); 
  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  for (unsigned int i=0;i<1856;i++){
    if(MODVETO[modid[i]]==0){      
      //if(LY[modid[i]]==0){
      //histo_counts[i]->SetTitle(modid[i] + TString(" Side ") + SD[modid[i]] + " FPIX Disk " + DISK[modid[i]] + " Ring " + RING[modid[i]]);
      //} 
      //if(LY[modid[i]]!=0){
      //histo_counts[i]->SetTitle(modid[i]+ TString(" BPIX layer ") + LY[modid[i]] +" ladder " + LD[modid[i]] + " MD " + MD[modid[i]]);
      // }
      histo_counts[i]->SetTitle(TString(" TH2F Histogram of cluster counts per lumi section "));      
      //histo_counts[i]->GetXaxis()->SetRangeUser(0, 70000);  
      //histo_counts[i]->GetYaxis()->SetRangeUser(0, 0.016);      
      histo_counts[i]->GetXaxis()->SetTitle("Lumi section");
      histo_counts[i]->GetYaxis()->SetTitle("Normalized count");
      //histo_counts[i]->Draw("COLZ");                                                                                                   
      //char *hist_name1 = new char[20];                                                     
      //if(modid[i]==344253444){                                                
      //sprintf(hist_name1,"histo2D_%d.png",i);                                                                                              
      //C.Print(Path1 + "Graph1D_test1/"+hist_name1);    
      //C.Print(Path1+"Graphs_cut_totcount/"+hist_name1);      
      //}
      
      //if(MODVETO[modid[i]]==0){
      P[i] = histo_counts[i]->ProfileX();
      ProjY[i] = histo_counts[i]->ProjectionY(TString("Projection_")+i,0,700);
      //if(BPIXorFPIX[modid[i]]==2){
	//if(LY[modid[i]]==2 || LY[modid[i]]==3 || LY[modid[i]]==4){
	gr_modweight->SetPoint(gr_modweight->GetN(), i, ProjY[i]->GetMean());
	//}
      //ProjY1[i] = histo_counts[i]->ProjectionY(TString("Projection1_")+i,0,250);
      //ProjY2[i] = histo_counts[i]->ProjectionY(TString("Projection2_")+i,330,430);
      //} 
      
      //P[i]->GetXaxis()->SetRangeUser(0, 70000);
      //copy content of TProfile into TGraph by looping over bin content of TProfile
      int x_value;
      float bincontent;
      float binerror;
      for(int j = 1; j <= P[i]->GetNbinsX(); j++) {
	x_value =  P[i]->GetBinCenter(j);
	bincontent = P[i]->GetBinContent(j);
	binerror = P[i]->GetBinError(j);
	if(bincontent > 0 && (binerror/bincontent) < 0.05) {
	    gr[i] ->SetPoint(gr[i]->GetN(), x_value, bincontent/(ProjY[i]->GetMean()));    
	    //std::cout<< P[i]->GetBinContent(j)<<" "<<std::endl;
	}
      }
      
      gr[i]->GetXaxis()->SetRangeUser(0, 35000);
      gr[i]->GetXaxis()->SetNdivisions(10);                                                                                                
      gr[i]->GetYaxis()->SetRangeUser(0.5, 1.5);
      gr[i]->SetMarkerStyle(8);  
      gr[i]->SetMarkerSize(0.5); 
      gr[i]->SetLineStyle(1);
      gr[i]->GetXaxis()->SetTitle("Lumi section");
      gr[i]->GetYaxis()->SetTitle("Normalized weight");
      //if(LY[modid[i]]==2){
	//if(DISK[modid[i]]==2 || DISK[modid[i]]==3){      
      //if(g_count==0){
      //C.Clear();
      //legend->Clear();
      //gr[i]->SetLineColor(g_count+1);
      //gr[i]->Draw("AL");
      //legend->AddEntry(gr[i], TString("modid ")+modid[i], "l");
      //g_count++;
      //} else if(g_count>0&&g_count<19) {
      //if(g_count<9){
      //gr[i]->SetLineColor(g_count+1);
      // }
      //if(g_count>=9 && g_count<19){
      //gr[i]->SetLineColor(g_count+20);
      //}
      //gr[i]->Draw("LSAME");
      //legend->AddEntry(gr[i], TString("modid ")+modid[i], "l");
      //g_count++;
      //} else if(g_count==19) { 
      //gr[i]->SetLineColor(g_count+27);
      //gr[i]->Draw("LSAME"); 
      //legend->AddEntry(gr[i], TString("modid ")+modid[i], "l");
      //legend->Draw("SAME");
      //char* histname2 = new char[100];
      //sprintf(histname2, "graph_%d", g_plot++);
      //legend->Draw("SAME");
      //C.Print(Path1+"Graphs_cut_totcount/"+histname2+".png");
      //C.Print(Path1+"Graph1D_test1/"+histname2+".png");
      //g_count=0;      
      // }
      // }

      //if(LY[modid[i]]==0){
      //gr[i]->SetTitle(modid[i]+ TString(" Side ") + SD[modid[i]] +" FPIX Disk " + DISK[modid[i]] + " Ring " + RING[modid[i]]);
      // }
      //if(LY[modid[i]]!=0){
      //gr[i]->SetTitle(modid[i]+ TString(" BPIX layer ") + LY[modid[i]] +" ladder " + LD[modid[i]] + " MD " + MD[modid[i]]);
      // }
      gr[i]->SetTitle(" Module stability profile ");
      gr[i]->Draw("AP");
      char *hist_name = new char[100];
      //if(LY[modid[i]]==0){                                                                                                                 
      //sprintf(hist_name, TString("Graph1D_") + modid[i]+ "_Side_" + SD[modid[i]] +"_Disk_" + DISK[modid[i]] + "_Ring_" + RING[modid[i]]+".png", i);                                                                                                               
      //}                                                                                                                                    
      //if(LY[modid[i]]!=0){                                                                                                                 
      //sprintf(hist_name, TString("Graph1D_") + modid[i]+ "_layer_" + LY[modid[i]] +"_ladder_" + LD[modid[i]] + "_MDnumber_" + MD[modid[i]]+".png", i);                                                                                                            
      //}   
      //C.Print(Path1+"Graphs_cut_totcount/"+hist_name+".png"); 

      //if(modid[i]==344253444){
      if(i==1845 || i==1293){
      sprintf(hist_name,"Graph1D_%d.png",i);
      //sprintf(hist_name,"Graph1D_%d.root",i);
      C.Print(Path1+"Graphs_cut_totcount/"+hist_name);
      //}
      //C.Print(Path1 + "Graph1D_test1/"+hist_name);
      }
      //float p1=ProjY1[i]->GetMean();
      //float p2= ((ProjY2[i]->GetMean())-(ProjY1[i]->GetMean()))/p1;
      //gr1->SetPoint(gr1->GetN(), i, p1);
      //gr2->SetPoint(gr2->GetN(), i, p2);      
      //gr1->GetXaxis()->SetTitle("module number index");
      //gr1->GetYaxis()->SetTitle("Mean");
      //gr1->GetXaxis()->SetRangeUser(0, 1900);
      //gr1->GetYaxis()->SetRangeUser(0, 0.012);
      //gr1->SetMarkerStyle(8);
      //gr1->SetMarkerSize(0.5);
      //gr2->GetXaxis()->SetTitle("module number index");
      //gr2->GetYaxis()->SetTitle("Normalized mean difference");
      //gr2->SetMarkerStyle(8);
      //gr2->SetMarkerSize(0.5);
      //gr2->GetXaxis()->SetRangeUser(0, 1900);
      //gr2->GetYaxis()->SetRangeUser(-0.25, 0.25);
      
      //if(LY[modid[i]]==0){
      //P[i]->SetTitle(modid[i]+ TString(" Side ") + SD[modid[i]] +" FPIX Disk " + DISK[modid[i]] + " Ring " + RING[modid[i]]);
      //}
      //if(LY[modid[i]]!=0){
      //P[i]->SetTitle(modid[i]+ TString(" BPIX layer ") + LY[modid[i]] +" ladder " + LD[modid[i]] + " MD " + MD[modid[i]]);
      //}   
      //P[i]->SetTitle(" Module count distribution ");
      //P[i]->Draw();
      //char* hist_name1 = new char[100];
      //sprintf(hist_name1,"ProfileX_%d.png",i);                                                                                            
      //C.Print(Path1 + "Graph1D_test1/"+hist_name1);
      
      //if(LY[modid[i]]==0){
      //sprintf(histname1, TString("ProfileX_Forward_Disk_Modid_") + modid[i]+ "_Side_" + SD[modid[i]] +"_Disk_" + DISK[modid[i]] + "_Ring_" + RING[modid[i]]+".png", i);
      // }
      //if(LY[modid[i]]!=0){
      //sprintf(histname1, TString("ProfileX_Barrel_layer_Modid_") + modid[i]+ "_layer_" + LY[modid[i]] +"_ladder_" + LD[modid[i]] + "_MDnumber_" + MD[modid[i]]+".png", i);
      //}
      
      //gr[i]->SetMarkerStyle(8);
      //gr[i]->SetMarkerSize(0.5);
      //gr[i]->SetLineStyle(1);
      //gr[i]->SetLineSize(0.5);
      //gr[i]->GetXaxis()->SetRangeUser(0, 80000);
      //gr[i]->GetXaxis()->SetNdivisions(10);
      //gr[i]->GetYaxis()->SetRangeUser(0.5, 1.5);
      //gr[i]->GetXaxis()->SetTitle("Lumi section");
      //gr[i]->GetYaxis()->SetTitle("Normalized count");  
      //if(LY[modid[i]]==0){
      //gr[i]->SetTitle(modid[i]+ TString(" Side ") + SD[modid[i]] +" FPIX Disk " + DISK[modid[i]] + " Ring " + RING[modid[i]]);
      //}
      //if(LY[modid[i]]!=0){
      //gr[i]->SetTitle(modid[i]+ TString(" BPIX layer ") + LY[modid[i]] +" ladder " + LD[modid[i]] + " MD " + MD[modid[i]]);
      //}
      
      //if(LY[modid[i]]==0){
      //gr3->SetPoint(gr3->GetN(), i, FPIX_Z[modid[i]]);
      //}
      //if(LY[modid[i]]!=0){
      //gr3->SetPoint(gr3->GetN(), i, BPIX_Z[modid[i]]);
      //}
      //gr4->SetPoint(gr4->GetN(), FPIX_Z[modid[i]],  DISK[modid[i]]);
      
      //gr3->SetMarkerStyle(8);
      //gr3->SetMarkerSize(0.5);
      //gr3->GetYaxis()->SetRangeUser(-100, 100);
      //gr3->GetXaxis()->SetRangeUser(0, 1900);
      //gr3->GetYaxis()->SetTitle("z coordinate");
      //gr3->GetXaxis()->SetTitle("module number index");
      
      //gr4->SetMarkerStyle(8);
      //gr4->SetMarkerSize(0.5);
      //gr4->GetXaxis()->SetRangeUser(-80, 80);
      //gr4->GetYaxis()->SetRangeUser(0, 5);
      //gr4->GetXaxis()->SetTitle("z coordinate");
      //gr4->GetYaxis()->SetTitle("Disk");
      
      //gr5->SetPoint(gr5->GetN(), i, LY[modid[i]]);   
      //if(SD[modid[i]]==1){
      //gr6->SetPoint(gr6->GetN(), i, abs(DISK[modid[i]]-3));
      //}
      //if(SD[modid[i]]==2){
      //gr6->SetPoint(gr6->GetN(), i, abs(DISK[modid[i]]-2));
      //}
      //if(SD[modid[i]]==1){
      //gr7->SetPoint(gr7->GetN(), SD[modid[i]], DISK[modid[i]]); 
      //}
      //if(SD[modid[i]]==2){
      //gr7->SetPoint(gr7->GetN(), FPIX_Z[modid[i]], abs(DISK[modid[i]]-2));
      //}
      
      gr8->SetMarkerStyle(8);
      gr8->SetMarkerSize(0.4);
      //gr8->GetYaxis()->SetNdivisions(80);
      gr8->GetYaxis()->SetLabelSize(0.02);
      gr8->GetXaxis()->SetRangeUser(0, 1900);
      //gr8->GetYaxis()->SetRangeUser(0, 0.1);
      gr8->SetTitle(" Run2018B (317080-319311)");
      gr8->GetYaxis()->SetTitle("RMS/Mean");
      gr8->GetXaxis()->SetTitle("module number index");
  
      h->SetMarkerStyle(8);
      h->SetMarkerSize(0.5);
      //h->GetYaxis()->SetRangeUser(0, 200);
      //gr10->GetYaxis()->SetNdivisions(80);                                                                                                 
      //gr10->GetYaxis()->SetLabelSize(0.02);              
      //h->GetXaxis()->SetRangeUser(0, 1);
      h->GetYaxis()->SetTitle("Entries");                                                                                             
      h->GetXaxis()->SetTitle("rms/mean");
      h->SetTitle("              Run2018B (317080-319311)");                                                                              
      
      float mean=ProjY[i]->GetMean();
      float rms=ProjY[i]->GetRMS();    
      if(mean!=0){
	//if(BPIXorFPIX[modid[i]]==2){
	  //if(LY[modid[i]]==2 || LY[modid[i]]==3 || LY[modid[i]]==4){
	gr8->SetPoint(gr8->GetN(), i, rms/mean);
	h->Fill(rms/mean);	
	//std::cout<<i<<"  "<<rms<<"  "<<mean<<"  "<<rms/mean<<std::endl;
	//std::cout<<modid[i]<<" "<<rms/mean<<std::endl;
	//}
    }
      //}
      
      //if(LY[modid[i]]==1 || rms/mean>=0.02){
      //std::cout<<modid[i]<<" "<<rms/mean<<std::endl;
      //}
      
      //gr11->SetPoint(gr11->GetN(), i, summodcount/totmodcount); 
      //std::cout<<summodcount<<" "<<totmodcount<<"  "<< summodcount/totmodcount<<std::endl;    
      //std::cout<<(ProjY[i]->GetRMS())/(ProjY[i]->GetMean())<<std::endl;                                                                   
      //if((rms/mean)==0 || (rms/mean)>=0.015){
      //std::cout<<i<<" "<<(ProjY[i]->GetRMS())/(ProjY[i]->GetMean())<<std::endl;     
      //std::cout<<modid[i]<<std::endl; 
      //}
      
      //float h_mean=h->GetMean();
      //float h_rms=h->GetStdDev();
      //float rms_mean=rms/mean;

      //if((rms_mean==0) || (0.4*(rms_mean-h_mean)>= h_rms)) {
      //std::cout<<modid[i]<<std::endl;
	//std::cout<<i<<" "<<modid[i]<<std::endl;
      //}

      ProjY[i]->GetXaxis()->SetRangeUser(0, 0.0015);
      //if(LY[modid[i]]==0){
      //ProjY[i]->SetTitle(modid[i]+ TString(" Side ") + SD[modid[i]] +" FPIX Disk " + DISK[modid[i]] + " Ring " + RING[modid[i]]);
      //}
      //if(LY[modid[i]]!=0){
      //ProjY[i]->SetTitle(modid[i]+ TString(" BPIX layer ") + LY[modid[i]] +" ladder " + LD[modid[i]] + " MD " + MD[modid[i]]);
      //}
      //ProjY[i]->SetTitle(TString(" ProjectionY of TH2F histogram "));
      
      ProjY[i]->GetYaxis()->SetTitle(" ");
      ProjY[i]->GetXaxis()->SetTitle("module weight");
      ProjY[i]->SetTitle("Module count distribution");
      ProjY[i]->Draw();
      char* hist_name3 = new char[100];
      //if(modid[i]==344253444){
      if(i==1293 || i==1845){
      sprintf(hist_name3,"ProjectionY_%d.root",i);
      //sprintf(hist_name3,"ProjectionY_%d.root",i);
      C.Print(Path1 + "Graphs_cut_totcount/"+hist_name3);
      }
      //}      
      //if(LY[modid[i]]==0){
      //sprintf(histname3, TString("ProjectionY_Forward_Disk_Modid_") + modid[i]+ "_Side_" + SD[modid[i]] +"_Disk_" + DISK[modid[i]] + "_Ring_" + RING[modid[i]]+".png", i);
      //}
      //if(LY[modid[i]]!=0){
      //sprintf(histname3, TString("ProjectionY_Barrel_layer_Modid_") + modid[i]+ "_layer_" + LY[modid[i]] +"_ladder_" + LD[modid[i]] + "_MDnumber_" + MD[modid[i]]+".png", i);
      
      //} 
      //}
      }
  }
  
  //if(g_count!=0){ 
  //legend->Draw("SAME"); 
  //char* histname2 = new char[100];
  //sprintf(histname2, "graph_%d", g_plot++);
  //C.Print(Path1+"Graphs_cut_totcount/"+histname2+".png");
  //}
  
  //gr1->Draw("AP");  
  //C.Print(Path1+"Graphs_cut_totcount/"+"graphprojy1"+".png");
  //C.Print(Path1+"Graph1D_test1/"+"graphprojy1"+".png");
  
  //gr2->Draw("AP");
  //C.Print(Path1+"Graphs_cut_totcount/"+"graphprojy2"+".png");
  //C.Print(Path1+"Graph1D_test1/"+"graphprojy2"+".png");
  
  //TLine *l = new TLine(1180,-100,1180,100);
  //l->SetLineStyle(2);
  //gr3->Draw("AP");
  //l->Draw("SAME");  
  //C.Print(Path1+"Graph1D_test1/"+"graph_modindex_z"+".png");
  
  //gr4->Draw("AP");
  //C.Print(Path1+"Graph1D_test1/"+"graph_disk_z"+".png");
  
  //gr8->SetTitle("                Run2018A (315252-316995)");
  gr8->SetTitle("              Run2018B (317080-319311)");                                                                            
  //gr8->SetTitle("              Run2018C (319337-320065)");                                                                            
  //gr8->SetTitle("              Run2018D (320500-325175)");
  //gr8->GetYaxis()->SetRangeUser(0, 0.018);
  gr8->GetYaxis()->SetRangeUser(0, 0.2);    
  gr8->Draw("AP");
  //TLine *l = new TLine(0,0.03,2050,0.03);                                                                                                
  //l->SetLineStyle(2);                                                                                                                    
  //l->Draw("SAME"); 
  //C.Print(Path1+"Graphs_cut_totcount/"+"RMS_Mean_Run2018A_(B+A)veto"+".png");
  //C.Print(Path1+"Graphs_cut_totcount/"+"RMS_Mean_2p5sigma"+".png"); 
  C.Print(Path1+"Graphs_cut_totcount/"+"RMS_Mean_testing"+".png");
  
  //gr9->GetXaxis()->SetTitle("Lumi section");
  //gr9->GetYaxis()->SetTitle("totcount");
  //gr9->SetTitle("              Run2018A (315252-316995)");
  //gr9->SetTitle("            Run2018B (317080-319311)");                                                                            
  //gr9->SetTitle("            Run2018C (319337-320065)");
  //gr9->SetTitle("            Run2018D (320500-325175)");
  
  gr9->GetXaxis()->SetNdivisions(10);
  gr9->GetXaxis()->SetLabelSize(0.03);
  gr9->SetTitle(" Run2018B (317080-319311)");
  gr9->GetYaxis()->SetTitle("Total PCC / LS");
  gr9->GetXaxis()->SetTitle("Lumi section");
  gr9->Draw("AP");                                                                                                                   
  //C.Print(Path1 + "Graph1D_test1/"+"LStot_RunA"+".root"); 
  //C.Print(Path1 + "Graph1D_test1/"+"LStot_RunB_lowlumi"+".root");
  //C.Print(Path1 + "Graph1D_test1/"+"LStot_RunC"+".root");
  C.Print(Path1 + "Graphs_cut_totcount/"+"LStot_RunB_testing"+".root");
  
  h->GetXaxis()->SetRangeUser(0, 0.5); 
  //h->GetXaxis()->SetRangeUser(0, 0.018);
  h->GetYaxis()->SetRangeUser(0, 1200);
  h->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"RMS_Mean_histo_2p5sigma"+".png");
  C.Print(Path1+"Graphs_cut_totcount/"+"RMS_Mean_histo_testing"+".root");
  
  gr_modweight->SetTitle("              Run2018B (317080-319311)");
  gr_modweight->GetXaxis()->SetTitle("module number index");                                                                                 
  gr_modweight->GetYaxis()->SetTitle("module weight");
  gr_modweight->GetXaxis()->SetRangeUser(0,2000);
  gr_modweight->GetYaxis()->SetRangeUser(0, 0.0045);
  gr_modweight->Draw("AP"); 
  C.Print(Path1+"Graphs_cut_totcount/"+"mod_weight_testing"+".png");
  
  histo_L2->GetXaxis()->SetTitle("Lumi section");
  histo_L2->GetYaxis()->SetTitle("Lumi ratio");
  histo_L2->GetXaxis()->SetRangeUser(0,70000);
  //histo_L2->SetTitle("              Run2018A (315252-316995)");
  histo_L2->SetTitle("              Run2018B (317080-319311)");                                                                          
  //histo_L2->SetTitle("              Run2018C (319337-320065)"); 
  //histo_L2->SetTitle("            Run2018D (320500-325175)");
  histo_L3->GetXaxis()->SetTitle("Lumi section");
  histo_L3->GetYaxis()->SetTitle("Lumi ratio");
  histo_L4->GetXaxis()->SetTitle("Lumi section");
  histo_L4->GetYaxis()->SetTitle("Lumi ratio");
  histo_D1S1->GetXaxis()->SetTitle("Lumi section");
  histo_D1S1->GetYaxis()->SetTitle("Lumi ratio");
  histo_D2S1->GetXaxis()->SetTitle("Lumi section");
  histo_D2S1->GetYaxis()->SetTitle("Lumi ratio");
  histo_D3S1->GetXaxis()->SetTitle("Lumi section");
  histo_D3S1->GetYaxis()->SetTitle("Lumi ratio");
  histo_D1S2->GetXaxis()->SetTitle("Lumi section");
  histo_D1S2->GetYaxis()->SetTitle("Lumi ratio");
  histo_D2S2->GetXaxis()->SetTitle("Lumi section");
  histo_D2S2->GetYaxis()->SetTitle("Lumi ratio");
  histo_D3S2->GetXaxis()->SetTitle("Lumi section");
  histo_D3S2->GetYaxis()->SetTitle("Lumi ratio");
  
  histo_L2->Draw("COLZ"); 
  histo_L3->Draw("COLZSAME");
  histo_L4->Draw("COLZSAME");
  histo_D1S1->Draw("COLZSAME");
  histo_D2S1->Draw("COLZSAME");
  histo_D3S1->Draw("COLZSAME");
  histo_D1S2->Draw("COLZSAME");
  histo_D2S2->Draw("COLZSAME");
  histo_D3S2->Draw("COLZSAME");
  //C.Print(Path1+"Graphs_cut_totcount/"+"Histo_stability_combined_2p5sigma"+".png"); 
  //C.Print(Path1+"Graphs_cut_totcount/"+"Histo_stability_combined_newveto(B+A)_Run2018A_update"+".png");  
  C.Print(Path1+"Graphs_cut_totcount/"+"Histo_stability_combined_testing"+".png");
  
  //histo_L2->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityL2"+".png"); 
  //histo_L3->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityL3"+".png");
  //histo_L4->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityL4"+".png");
  //histo_D1S1->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityD1S1"+".png");
  //histo_D2S1->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityD2S1"+".png");
  //histo_D3S1->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityD3S1"+".png");
  //histo_D1S2->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityD1S2"+".png");
  //histo_D2S2->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityD2S2"+".png");
  //histo_D3S2->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityD3S2"+".png");
  
  P_L2 = histo_L2->ProfileX();
  //P_L2->SetTitle("BPIX Layer 2");
  ProjY_L2 = histo_L2->ProjectionY(TString("Projection_L2"),0,200);
  P_L2->GetXaxis()->SetRangeUser(0, 35000);
  P_L2->GetXaxis()->SetNdivisions(50);
  P_L2->GetYaxis()->SetRangeUser(0, 0.35);
  //P_L2->GetXaxis()->SetNdivisions(7);
  
  P_L2->GetXaxis()->SetTitle("Lumi section");
  P_L2->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  //P_L2->SetTitle("              Run2018A (315252-316995)");
  P_L2->SetTitle("              Run2018B (317080-319311)");
  //P_L2->SetTitle("              Run2018C (319337-320065)");
  //P_L2->SetTitle("            Run2018D (320500-325175)");
  P_L3 = histo_L3->ProfileX();
  P_L3->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_L3 = histo_L3->ProjectionY(TString("Projection_L3"),0,200);
  //P_L3->GetYaxis()->SetRangeUser(0, 0.3);
  P_L3->SetTitle("BPIX Layer 3");
  P_L3->GetXaxis()->SetTitle("Lumi section");
  P_L3->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  P_L4 = histo_L4->ProfileX();
  P_L4->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_L4 = histo_L4->ProjectionY(TString("Projection_L4"),0,200);
  //P_L4->GetYaxis()->SetRangeUser(0, 0.3);
  P_L4->SetTitle("BPIX Layer 4");
  P_L4->GetXaxis()->SetTitle("Lumi section");
  P_L4->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  P_D1S1 = histo_D1S1->ProfileX();
  P_D1S1->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_D1S1 = histo_D1S1->ProjectionY(TString("Projection_D1S1"),0,200);
  //P_D1S1->GetYaxis()->SetRangeUser(0, 0.3);
  P_D1S1->SetTitle("FPIX Disk 1 Side 1");
  P_D1S1->GetXaxis()->SetTitle("Lumi section");
  P_D1S1->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  P_D2S1 = histo_D2S1->ProfileX();
  P_D2S1->SetTitle("FPIX Disk 2 Side 1");
  P_D2S1->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_D2S1 = histo_D2S1->ProjectionY(TString("Projection_D2S1"),0,200);
  //P_D2S1->GetYaxis()->SetRangeUser(0, 0.3);
  P_D2S1->GetXaxis()->SetTitle("Lumi section");
  P_D2S1->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  P_D3S1 = histo_L2->ProfileX();
  // P_D3S1->SetTitle("FPIX Disk 3 Side 1");
  P_D3S1->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_D3S1 = histo_D3S1->ProjectionY(TString("Projection_D3S1"),0,200);
  //P_D3S1->GetYaxis()->SetRangeUser(0, 0.3);
  P_D3S1->GetXaxis()->SetTitle("Lumi section");
  P_D3S1->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  ProjY_D3S1->SetTitle(" ProjectionY_D3S1");
  ProjY_D3S1->GetXaxis()->SetRangeUser(0, 0.15);
  P_D1S2 = histo_D1S2->ProfileX();
  P_D1S2->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_D1S2 = histo_D1S2->ProjectionY(TString("Projection_D1S2"),0,200);
  //P_D1S2->GetYaxis()->SetRangeUser(0, 0.3);
  P_D1S2->SetTitle("FPIX Disk 1 Side 2");
  P_D1S2->GetXaxis()->SetTitle("Lumi section");
  P_D1S2->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  P_D2S2 = histo_D2S2->ProfileX();
  P_D2S2->SetTitle("FPIX Disk 2 Side 2");
  
  P_D2S2->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_D2S2 = histo_D2S2->ProjectionY(TString("Projection_D2S2"),0,200);
  ProjY_D2S2->SetTitle(" ProjectionY_D2S2");
  ProjY_D2S2->GetXaxis()->SetRangeUser(0, 0.15);
  //P_D2S2->GetYaxis()->SetRangeUser(0, 0.3);
  P_D2S2->GetXaxis()->SetTitle("Lumi section");
  P_D2S2->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  P_D3S2 = histo_D3S2->ProfileX();
  P_D3S2->SetTitle("FPIX Disk 3 Side 2");
  P_D3S2->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_D3S2 = histo_D3S2->ProjectionY(TString("Projection_D3S2"),0,200);
  //P_D3S2->GetYaxis()->SetRangeUser(0, 0.3);
  P_D3S2->GetXaxis()->SetTitle("Lumi section");
  P_D3S2->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
    
  std::cout<<ProjY_L2->GetMean()<<std::endl;
  std::cout<<ProjY_L3->GetMean()<<std::endl;
  std::cout<<ProjY_L4->GetMean()<<std::endl;
  std::cout<<ProjY_D1S1->GetMean()<<std::endl;
  std::cout<<ProjY_D2S1->GetMean()<<std::endl;
  std::cout<<ProjY_D3S1->GetMean()<<std::endl;
  std::cout<<ProjY_D1S2->GetMean()<<std::endl;
  std::cout<<ProjY_D2S2->GetMean()<<std::endl;
  std::cout<<ProjY_D3S2->GetMean()<<std::endl;
  
  P_L2->SetLineColor(1);
  P_L2->SetMarkerColor(1);
  P_L3->SetLineColor(2);
  P_L3->SetMarkerColor(2);
  P_L4->SetLineColor(3);
  P_L4->SetMarkerColor(3);    
  P_D1S1->SetLineColor(4);
  P_D1S1->SetMarkerColor(4);
  P_D2S1->SetLineColor(5);
  P_D2S1->SetMarkerColor(5);
  P_D3S1->SetLineColor(6);
  P_D3S1->SetMarkerColor(6);
  P_D1S2->SetLineColor(7);
  P_D1S2->SetMarkerColor(7);
  P_D2S2->SetLineColor(8);
  P_D2S2->SetMarkerColor(8);
  P_D3S2->SetLineColor(9);
  P_D3S2->SetMarkerColor(9);
  
  P_L2->Draw();
  P_L3->Draw("SAME");
  P_L4->Draw("SAME");
  P_D1S1->Draw("SAME");
  P_D2S1->Draw("SAME");
  P_D3S1->Draw("SAME");
  P_D1S2->Draw("SAME");
  P_D2S2->Draw("SAME");
  P_D3S2->Draw("SAME");
    
  auto legend1 = new TLegend(0.91,0.15,1,0.9);
  //auto legend1 = new TLegend(0.7,0.35,0.85,0.8);
  legend1->AddEntry(P_L2,"B1","l");
  legend1->AddEntry(P_L3,"B2","l");
  legend1->AddEntry(P_L4,"B3","l");
  legend1->AddEntry(P_D1S1,"F1S1","l");
  legend1->AddEntry(P_D2S1,"F2S1","l");
  legend1->AddEntry(P_D3S1,"F3S1","l");
  legend1->AddEntry(P_D1S2,"F1S2","l");
  legend1->AddEntry(P_D2S2,"F2S2","l");
  legend1->AddEntry(P_D3S2,"F3S2","l");
  legend1->SetFillColor(0);
  legend1->SetLineColor(0);
  legend1->SetFillColor(0);
  legend1->Draw("SAME");
  
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_combined_2p5sigma"+".png");
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_combined_newveto(B+A)_Run2018A_update"+".png");
  C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_combined_testing"+".png");
  
  //P_L2->Draw();  
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_L2"+".png"); 
  //P_L3->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_L3"+".png");
  //P_L4->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_L4"+".png");
  //P_D1S1->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_D1S1"+".png");
  //P_D2S1->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_D2S1"+".png");
  //P_D3S1->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_D3S1"+".png");
  //P_D1S2->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_D1S2"+".png");
  //P_D2S2->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_D2S2"+".png");
  //P_D3S2->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_D3S2"+".png");
  
  ProjY_L2->Draw();                                                                                                                        
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_L2"+".png");                                                                                     
  ProjY_L3->Draw();                                                                                                                        
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_L3"+".png");                                                                                     
  ProjY_L4->Draw();                                                                                                                        
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_L4"+".png");                                                                                     
  ProjY_D1S1->Draw();                                                                                                                      
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_D1S1"+".png");                                                                                   
  ProjY_D2S1->Draw();                                                                                                                      
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_D2S1"+".png");                                                                                   
  ProjY_D3S1->Draw();                                                                                                                      
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_D3S1"+".png");                                                                                   
  ProjY_D1S2->Draw();                                                                                                                      
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_D1S2"+".png");                                                                                   
  ProjY_D2S2->Draw();                                                                                                                      
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_D2S2"+".png");                                                                                   
  ProjY_D3S2->Draw();                                                                                                                      
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_D3S2"+".png"); 
  //gROOT->ProcessLine(".q");
  
}


//see how modcount looks for entire Run 2018A, B and C without normalization
//sum counts and normalize individual module count graphs with respect to total counts 
//when we plot many runs, we should be careful about order of lumi sections (time ordering of lumi section values)
//next steps:- make loop over runs , create a vector with run list and iterate over this. Need to add a counter for the total number of lumi sections, then use this counter as x-axis value in the Graph.- need to fix order of the lumi sections, they are not time ordered in the csv file. for now maybe can define a variable like LS = run_index * 2500 + ls , where ls is the lumi section read from the file. 3000 is like a maximum number of lumi sections per run. run_index is just a counter for the runs, the run vector should be ordered.
//make ProfileX and ProjectionY graphs.
//make two more projections. First from 0 to 25000 lumi sections and second from 33000 to 43000 lumi sections
//graph of weights/mean values with module number on x axis and weight/mean value from ProjectionY on y axis.
//using second projection, plot module number vs difference (difference between mean values from first projection from 0 to 25000 and second projection from 33000 to 43000) in mean values.
//draw projections in a separate module loop.

