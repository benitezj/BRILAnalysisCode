#include"globals.h" 
#include <iostream>
#include <vector>
#include <dirent.h>
void compareZBfiles_oldnewveto(char run_period='B') {
  
  TCanvas*C = new TCanvas("Luminosity ratio (new old veto)","");
  C->cd();

  TString Path1="/eos/user/a/asehrawa/BRIL-new/";
  TString Path, Path2; 
  
  std::vector<int> run_number;
  if(run_period=='A'){
    run_number = {315252, 315255, 315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315533, 315543, 315555, 315556, 315557, 315587, 315640, 315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785, 315786, 315787, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062, 316082, 316095, 316109, 316110, 316111, 316112, 316113, 316114, 316151, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219, 316239, 316240, 316241, 316260, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316456, 316457, 316469, 316470, 316472, 316477, 316478, 316505, 316569, 316590, 316613, 316615, 316664, 316665, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723, 316758, 316766, 316797, 316798, 316799, 316808, 316820, 316839, 316851, 316876,316877, 316878, 316879, 316928, 316944, 316985, 316993, 316994, 316995};

    //run_number ={316187};
  }
  
  
  if(run_period=='B'){
    run_number = {317080, 317087, 317088, 317089, 317170, 317182, 317212, 317213, 317239, 317279,317291, 317292, 317295, 317296, 317297, 317319, 317320,317338, 317339, 317340, 317382, 317383, 317391, 317392, 317434, 317435, 317438, 317475, 317478, 317479, 317480, 317481, 317482, 317484, 317488, 317509, 317510, 317511, 317512, 317527, 317580, 317591, 317626, 317640, 317641, 317648,317649, 317650, 317661, 317663, 317683, 317696, 318070, 318622, 318653, 318661, 318662, 318663, 318667, 318669, 318670, 318675, 318712,318714, 318733, 318734, 318785,318816, 318817, 318819, 318820, 318828, 318834, 318836, 318837, 318872, 318874, 318876, 318877, 318939, 318944, 318945, 318953, 318980, 318981, 318982, 318983, 318984, 318992, 319006, 319011, 319015, 319016, 319018, 319019, 319077, 319097, 319098, 319099, 319100, 319101, 319103, 319104, 319124, 319125, 319159, 319160, 319173, 319174, 319175, 319176, 319177, 319190, 319222, 319223, 319254, 319255, 319256, 319260, 319262, 319263, 319264, 319265, 319266, 319267, 319268, 319270, 319273, 319274, 319300, 319310, 319311};
    
  }
  
  if(run_period=='C'){
    run_number = {319337, 319347, 319348, 319349, 319449, 319450, 319452, 319456, 319459, 319460, 319462, 319463, 319464, 319466, 319467, 319468, 319469, 319470, 319471, 319472, 319486, 319488, 319503, 319506, 319524, 319525, 319526, 319528, 319579, 319606, 319625, 319639, 319656, 319657, 319658, 319659, 319678, 319687, 319697, 319698, 319756, 319840, 319841, 319847, 319848, 319849, 319851, 319852, 319853, 319854, 319908, 319909, 319910, 319912, 319913, 319914, 319915, 319928, 319941, 319942, 319950, 319991, 319992, 319993, 320002, 320003, 320006, 320007, 320008, 320009, 320010, 320011, 320012, 320023, 320024, 320025, 320026, 320038, 320039, 320040, 320058, 320059, 320060, 320061, 320062, 320063, 320064, 320065};
  }
  
  //D1=D, D2=E, D3=F,D4=G
  if(run_period=='D'){
    run_number= {320500, 320569, 320570, 320571, 320612, 320617, 320654, 320673, 320674, 320688, 320712, 320757, 320804, 320807, 320809, 320821, 320822, 320823, 320824, 320838, 320840, 320841, 320853, 320854, 320855, 320856, 320857, 320858, 320859, 320887, 320888, 320917, 320920, 320933, 320934, 320936, 320941, 320980, 320995, 320996, 321004, 321005, 321006, 321007, 321009, 321010, 321011, 321012, 321051, 321055, 321067, 321068, 321069, 321078, 321119, 321121, 321122, 321123, 321124, 321126, 321134, 321138, 321140, 321149, 321162, 321164, 321165, 321166, 321167, 321177, 321178, 321218, 321219, 321221, 321230, 321231, 321232, 321233, 321261, 321262, 321283, 321294, 321295, 321296, 321305, 321310, 321311, 321312, 321313, 321383, 321386, 321388, 321393, 321396, 321397, 321414, 321415, 321431, 321432, 321433, 321434, 321436, 321457, 321461, 321475, 321528, 321536, 321586, 321607, 321636, 321640, 321650, 321652, 321653, 321665};
    
  }

  if(run_period=='E'){  
    run_number= {321710, 321712, 321730, 321731, 321732, 321735, 321755, 321758, 321759, 321760, 321773, 321774, 321775, 321776, 321777, 321778, 321780, 321781, 321794, 321796, 321813, 321815, 321817, 321818, 321819, 321820, 321831, 321832, 321833, 321834, 321879, 321880, 321887, 321908, 321909, 321917, 321919, 321933, 321949, 321960, 321961, 321973, 321975, 321988, 321990, 322013, 322014, 322022, 322040, 322049, 322057, 322068, 322079, 322088, 322106, 322113, 322118, 322165, 322167, 322179, 322201, 322204, 322222, 322252, 322317, 322319, 322322, 322324, 322332, 322348, 322355, 322356, 322381, 322407, 322430, 322431, 322480, 322483, 322484, 322485, 322487, 322492, 322510, 322599, 322602, 322603, 322605, 322616, 322617, 322625, 322633, 322958, 322959, 322961, 322963, 322964}; 
  }
  
  if(run_period=='F'){
    run_number={323363, 323364, 323365, 323367, 323368, //323369, 
		323370, 323371, 323373, 323374, 323375, 323376, 323377, 323378, 323388, 323391, 323393, 323394, 323395, 323396, 323397, 323398, 323399, 323414, 323415, 323416, 323417, 323418, 323419, 323420, 323421, 323422, 323423, 323470, 323471, 323472, 323473, 323474, 323475, 323487, 323488, 323492, 323493, 323495, 323524, 323525, 323526, 323693, 323696, 323698, 323699, 323700, 323702, 323725, 323726, 323727, 323755, 323775, 323778, 323790, 323794, 323841, 323857, 323940, 323954, 323976, 323978, 323980, 323983, 323997, 324021, 324022, 324077, 324078, 324201, 324202, 324205, 324206, 324207, 324209, 324237, 324245, 324293, 324315, 324318, 324420};
  }
  
  if(run_period=='G'){
    run_number={324564, 324571, 324612, 324729, 324747, 324764, 324765, 324769, 324772, 324785, 324791, 324835, 324840, 324841, 324846, 324878, 324897, 324970, 324980, 324997, 324998, 324999, 325000, 325001, 325022, 325057, 325097, 325098, 325099, 325100, 325101, 325110, 325111, 325113, 325114, 325117, 325159, 325168, 325169, 325170, 325172, 325175};  
  }
  
  
  //std::vector<int> run_number = {316186};
  std::cout<< run_number.size()<<std::endl;
  
  TH1F *LumiperLS;
  LumiperLS=new TH1F("h", "PCC_per_ls_oldveto", 59708, 0.0, 59708);
  
  TGraph *LumiLSratio;
  LumiLSratio=new TGraph();
  
  TH1F *LumiperLS1;
  LumiperLS1=new TH1F("h1", "PCC_per_ls_newveto", 59708, 0.0, 59708); 
  
  int LS=0;
  int previousrunlumisec_count=0;
  int LS1=0;
  int previousrunlumisec_count1=0;
  int lumisec_count=0;
  int lumisec_count1=0;
  
  std::map<int, float> cluster_count; 
  
  if(run_period=='A'){
    Path = "/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018A";
  }  
  if(run_period=='B'){
    Path = "/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018B";
  }    
  if(run_period=='C'){
    Path = "/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018C";
  }
  if(run_period=='D' || run_period=='E'){
    Path = "/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018D";
  }  
  if(run_period=='F'){
    Path ="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018D_mergeLateRunD/";
  }
  
  if(run_period=='G'){
    Path ="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_Dec21_LateRunD/Run2018D";
  }
  
  
  if(run_period=='A'){
    Path2 = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018A";
  }
  if(run_period=='B'){
    Path2 = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018B";
  }
  if(run_period=='C'){
    Path2 = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018C";
  } 
  if(run_period=='D'){
    Path2 = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018D1";
  }
  if(run_period=='E'){   
    Path2 = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018D2";
  }
  if(run_period=='F'){
    Path2 = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018D3";
  }
  if(run_period=='G'){    
    Path2 = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018D4";
  }
  
  
  for (unsigned int j=0;j<run_number.size();j++){
    
    TString infile=Path+"/"+run_number.at(j)+".csv"; 
    //std::cout<< run_number.at(j)<<std::endl;  
    
    ifstream myfile (infile.Data());    
    if (!myfile.is_open()){
      cout << "Unable to open old file: "<<infile.Data()<<endl; 
      return;
    }    
    
  
    std::string line;
    int run=0;
    int ls=0;
    float LumiLS=0;
    //int lumisec_count_perrun=0;
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
      //LS = previousrunlumisec_count + ls;      
      
      std::getline(iss,token, ',');
      std::stringstream lumiiss(token);
      lumiiss>>LumiLS;
      cluster_count[ls]=LumiLS;    // map contain old ls and PCC values
      //std::cout<<"oldveto "<<run<< "  "<<ls<<"  "<<cluster_count[ls]<<std::endl;       
      LumiperLS->Fill(lumisec_count, LumiLS);
      //std::cout<<"old veto "<<run<< "  "<<ls<<"  "<<LumiLS<<std::endl;
      //std::cout<<"old "<<run<<"    "<< lumisec_count<<std::endl;
    }
    
    //previousrunlumisec_count+=lumisec_count; 
    myfile.close();     
    
    //checking map content
    //for(std::map<int,float>::iterator it = cluster_count.begin(); it != cluster_count.end(); ++it) {
    //std::cout <<run<< " Key: " << it->first << " Value: " << it->second << std::endl;
    //}  
    
    
    TString infile1=Path2+"/"+run_number.at(j)+".csv";
    //std::cout<< run_number.at(j)<<std::endl;                                                                                               
    ifstream myfile1 (infile1.Data());
    if (!myfile1.is_open()){
      cout << "Unable to open new file: "<<infile1.Data()<<endl;
      return;
    }
    
    std::string line1;  
    int ls1=0;
    float LumiLS1=0;
    //int lumisec_count_perrun1=0;
    while (std::getline(myfile1, line1)){
      std::stringstream iss1(line1);
      std::string token1;
      
      std::getline(iss1,token1, ',');
      std::stringstream runiss1(token1);
      runiss1>>run;
      
      std::getline(iss1,token1, ',');
      std::stringstream lsiss1(token1);
      lsiss1>>ls1;
      
      lumisec_count1++;
      //LS1 = previousrunlumisec_count1 + ls1;
      
      std::getline(iss1,token1, ',');
      std::stringstream lumiiss1(token1);
      lumiiss1>>LumiLS1;
      
      LumiperLS1->Fill(lumisec_count1, LumiLS1);
      //std::cout<<"newveto "<<run<< "  "<<ls<<"  "<<LumiLS1<<std::endl;
      
      if (run_period=='A' && LumiLS>=2200000 && LumiLS1>=1900000){
	if(cluster_count[ls]!=0){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), lumisec_count1, LumiLS1/cluster_count[ls1]);
	  std::cout<<lumisec_count1<<"  "<< LumiLS1<<" "<<cluster_count[ls1]<<"  "<<LumiLS1/cluster_count[ls1] <<std::endl;
	}
      }
      
      
      if (run_period=='B' && LumiLS>=3000000 && LumiLS1>=4000000){
	if(LumiLS!=0){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), LS, LumiLS1/LumiLS);
	  //std::cout<< "ratio "<<LS1<<"  "<< LumiLS1/LumiLS <<std::endl;                                                                  
	}
      }

      if (run_period=='C' && LumiLS>=1000000 && LumiLS1>=1000000){
	if(LumiLS!=0){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), LS, LumiLS1/LumiLS);
	  //std::cout<< "ratio "<<LS1<<"  "<< LumiLS1/LumiLS <<std::endl;                                                                  
	}
      }
      
      if (run_period=='D' && LumiLS>=2000000 && LumiLS1>=2000000){
	if(LumiLS!=0){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), LS, LumiLS1/LumiLS);
	  //std::cout<< "ratio "<<LS1<<"  "<< LumiLS1/LumiLS <<std::endl;                                                                  
	}
      }

      if (run_period=='E' && LumiLS>=3000000 && LumiLS1>=2000000){
	if(LumiLS!=0){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), LS, LumiLS1/LumiLS);
	  //std::cout<< "ratio "<<LS1<<"  "<< LumiLS1/LumiLS <<std::endl;                                                                  
	}
      }

      if (run_period=='F' &&  LumiLS>=2000000 && LumiLS1>=2000000){
	if(LumiLS!=0){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), LS, LumiLS1/LumiLS);
	  //std::cout<< "ratio "<<LS1<<"  "<< LumiLS1/LumiLS <<std::endl;                                                                  
	}
      }

      if (run_period=='G' &&  LumiLS>=1000000 && LumiLS1>=3000000){ 
	if(LumiLS!=0){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), LS, LumiLS1/LumiLS);
	  //std::cout<< "ratio "<<LS1<<"  "<< LumiLS1/LumiLS <<std::endl;
	}
      }
      
    }
    
    //previousrunlumisec_count1+=lumisec_count1;    
    myfile1.close();
    
    //for(std::map<int,float>::iterator it = cluster_count.begin(); it != cluster_count.end(); ++it) {
      //std::cout <<run<< " Key: " << it->first << " Value: " << it->second << std::endl;
    //}
    
    
    //std::cout<<"new "<<run<<"    "<< lumisec_count1<<std::endl;
  }
  
  
  LumiperLS->GetXaxis()->SetTitle("Lumi section");
  LumiperLS->GetYaxis()->SetTitle("PCC");  
  LumiperLS->Draw("histp");
  if(run_period=='A'){
    C->Print(Path1+"Lumiperls_oldveto_Run2018A"+".root");
  }
  
  if(run_period=='B'){
    C->Print(Path1+"Lumiperls_oldveto_Run2018B"+".root");
  }
  if(run_period=='C'){
    C->Print(Path1+"Lumiperls_oldveto_Run2018C"+".root");
  }
  if(run_period=='D'){
    C->Print(Path1+"Lumiperls_oldveto_Run2018D1"+".root");
  }
  if(run_period=='E'){
    C->Print(Path1+"Lumiperls_oldveto_Run2018D2"+".root");
  }
  if(run_period=='F'){
    C->Print(Path1+"Lumiperls_oldveto_Run2018D3"+".root");
  }
  if(run_period=='G'){
    C->Print(Path1+"Lumiperls_oldveto_Run2018D4"+".root");
  }
  
  
  LumiperLS1->GetXaxis()->SetTitle("Lumi section");
  LumiperLS1->GetYaxis()->SetTitle("PCC");
  LumiperLS1->Draw("histp");
  
  if(run_period=='A'){
    C->Print(Path1+"Lumiperls_newveto_Run2018A"+".root");
  }
  
  if(run_period=='B'){
    C->Print(Path1+"Lumiperls_newveto_Run2018B"+".root");
  }
  if(run_period=='C'){
    C->Print(Path1+"Lumiperls_newveto_Run2018C"+".root");
  }
  if(run_period=='D'){
    C->Print(Path1+"Lumiperls_newveto_Run2018D1"+".root");
  }
  if(run_period=='E'){
    C->Print(Path1+"Lumiperls_newveto_Run2018D2"+".root");
  }
  if(run_period=='F'){
    C->Print(Path1+"Lumiperls_newveto_Run2018D3"+".root");
  }
  if(run_period=='G'){
    C->Print(Path1+"Lumiperls_newveto_Run2018D4"+".root");
  }
  
  if(run_period=='A'){
    LumiLSratio ->SetTitle("Run2018A (315252-316995)");
    LumiLSratio ->GetYaxis()->SetRangeUser(0.6, 0.8); 
  }
  if(run_period=='B'){
    LumiLSratio->SetTitle("Run2018B (317080-319311)");
    LumiLSratio ->GetYaxis()->SetRangeUser(1.5, 1.6);
  }
  if(run_period=='C'){
    LumiLSratio->SetTitle("Run2018C (319337-320065)");
    LumiLSratio ->GetYaxis()->SetRangeUser(1, 1.15);  
  }
  if(run_period=='D'){
    LumiLSratio->SetTitle("Run2018D (320500-321665)");
    LumiLSratio ->GetYaxis()->SetRangeUser(0.8, 0.9); 
  }
  if(run_period=='E'){
    LumiLSratio->SetTitle("Run2018D (321710-322964)");
    LumiLSratio ->GetYaxis()->SetRangeUser(0.75, 0.95); 
  }
  if(run_period=='F'){
    LumiLSratio->SetTitle("Run2018D (323363-324420)");
  }
  if(run_period=='G'){   
    LumiLSratio->SetTitle("Run2018D (324564-325175)");
    LumiLSratio ->GetYaxis()->SetRangeUser(2.05, 2.2); 
  }
  
  LumiLSratio->GetXaxis()->SetTitle("Lumi section");                                                                                 
  LumiLSratio->GetYaxis()->SetTitle("new data/old data");
  //LumiLSratio ->GetXaxis()->SetRangeUser(0,2200);  
  LumiLSratio ->Draw("AP"); 
  
  if(run_period=='A'){
    C->Print(Path1+"Lumiratio_Run2018A"+".png");
  }
  
  if(run_period=='B'){
    C->Print(Path1+"Lumiratio_Run2018B"+".root");
  }
  if(run_period=='C'){
    C->Print(Path1+"Lumiratio_Run2018C"+".root");
  }
  if(run_period=='D'){
    C->Print(Path1+"Lumiratio_Run2018D1"+".root");
  }
  if(run_period=='E'){
    C->Print(Path1+"Lumiratio_Run2018D2"+".root");
  }
  if(run_period=='F'){
    C->Print(Path1+"Lumiratio_Run2018D3"+".root");
  }
  if(run_period=='G'){
    C->Print(Path1+"Lumiratio_Run2018D4"+".root");
  }
  
 
}
