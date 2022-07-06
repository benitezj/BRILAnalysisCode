#include"globals.h" 
#include <iostream>
#include <vector>
#include <dirent.h>
void compareZBfiles_PCC_hfoc(char run_period='H') {
  
  TCanvas*C = new TCanvas("Luminosity (PCC hfoc data)");
  C->cd();
  //C->SetLogy();
  //gStyle->SetOptStat(111111);
  //gStyle->SetStatX(0.93);
  //gStyle->SetStatY(0.93);  
  TString Path1="/eos/user/a/asehrawa/BRIL-new/";
  TString Path; 
  
  std::vector<int> run_number;
  
  if(run_period=='A'){
    run_number = {315252, 315255, 315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315533, 315543, 315555, 315556, 315557, 315587, 315640, 315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785, 315786, 315787, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062, 316082, 316095, 316109, 316110, 316111, 316112, 316113, 316114, 316151, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219, 316239, 316240, 316241, 316260, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316456, 316457, 316469, 316470, 316472, 316477, 316478, 316505, 316569, 316590, 316613, 316615, 316664, 316665, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723, 316758, 316766, 316797, 316798, 316799, 316808, 316820, 316839, 316851, 316876,316877, 316878, 316879, 316928, 316944, 316985, 316993, 316994, 316995};
    
    //run_number = {316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723};
    
  }
  
  if(run_period=='B'){
    run_number = {317080, 317087, 317088, 317089, 317170, 317182, 317212, 317213, 317239, 317279,317291, 317292, 317295, 317296, 317297, 317319, 317320,317338, 317339, 317340, 317382, 317383, 317391, 317392, 317434, 317435, 317438, 317475, 317478, 317479, 317480, 317481, 317482, 317484, 317488, 317509, 317510, 317511, 317512, 317527, 317580, 317591, 317626, 317640, 317641, 317648,317649, 317650, 317661, 317663, 317683, 317696, 318070, 318622, 318653, 318661, 318662, 318663, 318667, 318669, 318670, 318675, 318712,318714, 318733, 318734, 318785,318816, 318817, 318819, 318820, 318828, 318834, 318836, 318837, 318872, 318874, 318876, 318877, 318939, 318944, 318945, 318953, 318980, 318981, 318982, 318983, 318984, 318992, 319006, 319011, 319015, 319016, 319018, 319019, 319077, 319097, 319098, 319099, 319100, 319101, 319103, 319104, 319124, 319125, 319159, 319160, 319173, 319174, 319175, 319176, 319177, 319190, 319222, 319223, 319254, 319255, 319256, 319260, 319262, 319263, 319264, 319265, 319266, 319267, 319268, 319270, 319273, 319274, 319300, 319310, 319311};
  }
  
  if(run_period=='C'){
    //run_number = {319337, 319347, 319348, 319349, 319449, 319450, 319452, 319456, 319459, 319460, 319462, 319463, 319464, 319466, 319467, 319468, 319469, 319470, 319471, 319472, 319486, 319488, 319503, 319506, 319524, 319525, 319526, 319528, 319579, 319606, 319625, 319639, 319656, 319657, 319658, 319659, 319678, 319687, 319697, 319698, 319756, 319840, 319841, 319847, 319848, 319849, 319851, 319852, 319853, 319854, 319908, 319909, 319910, 319912, 319913, 319914, 319915, 319928, 319941, 319942, 319950, 319991, 319992, 319993, 320002, 320003, 320006, 320007, 320008, 320009, 320010, 320011, 320012, 320023, 320024, 320025, 320026, 320038, 320039, 320040, 320058, 320059, 320060, 320061, 320062, 320063, 320064, 320065};

    run_number = {320058, 320059, 320060, 320061, 320062, 320063, 320064, 320065}; 
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
 
  if(run_period=='H'){
    run_number={315252, 315255, 315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315533, 315543, 315555, 315556, 315557, 315587, 315640, 315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785,315786, 315787, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062, 316082, 316095, 316109, 316110, 316111, 316112, 316113, 316114, 316151, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219,316239, 316240, 316241, 316260, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316456, 316457, 316469, 316470, 316472,316477, 316478, 316505, 316569, 316590, 316613, 316615, 316664, 316665, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723, 316758, 316766, 316797, 316798, 316799, 316808, 316820, 316839, 316851, 316876,316877, 316878,316879, 316928, 316944, 316985, 316993, 316994, 316995, 317080, 317087, 317088, 317089, 317170, 317182, 317212, 317213, 317239, 317279,317291, 317292, 317295, 317296, 317297, 317319, 317320,317338, 317339, 317340, 317382, 317383, 317391, 317392, 317434, 317435, 317438, 317475, 317478, 317479, 317480, 317481, 317482, 317484, 317488, 317509, 317510, 317511, 317512, 317527, 317580, 317591, 317626, 317640, 317641, 317648,317649, 317650, 317661, 317663, 317683, 317696, 318070, 318622, 318653, 318661, 318662, 318663, 318667, 318669, 318670, 318675, 318712,318714, 318733, 318734, 318785,318816, 318817, 318819, 318820, 318828, 318834, 318836, 318837, 318872, 318874, 318876, 318877, 318939, 318944, 318945, 318953, 318980, 318981, 318982, 318983, 318984, 318992, 319006, 319011, 319015, 319016, 319018, 319019, 319077, 319097, 319098, 319099, 319100, 319101, 319103, 319104, 319124, 319125, 319159, 319160, 319173, 319174, 319175, 319176, 319177, 319190, 319222, 319223, 319254, 319255, 319256, 319260, 319262, 319263, 319264, 319265, 319266, 319267, 319268, 319270, 319273, 319274, 319300, 319310, 319311, 319337, 319347, 319348, 319349, 319449, 319450, 319452, 319456, 319459, 319460, 319462, 319463, 319464, 319466, 319467, 319468, 319469, 319470, 319471, 319472, 319486, 319488, 319503, 319506, 319524, 319525, 319526, 319528, 319579, 319606, 319625, 319639, 319656, 319657, 319658, 319659, 319678, 319687, 319697, 319698, 319756, 319840, 319841, 319847, 319848, 319849, 319851, 319852, 319853, 319854, 319908, 319909, 319910, 319912, 319913, 319914, 319915, 319928, 319941, 319942, 319950, 319991, 319992, 319993, 320002, 320003, 320006, 320007, 320008, 320009, 320010, 320011, 320012, 320023, 320024, 320025, 320026, 320038, 320039, 320040, 320058, 320059, 320060, 320061, 320062, 320063, 320064, 320065, 320500, 320569, 320570, 320571, 320612, 320617, 320654, 320673, 320674, 320688, 320712, 320757, 320804, 320807, 320809, 320821, 320822, 320823, 320824, 320838, 320840, 320841, 320853, 320854, 320855, 320856, 320857, 320858, 320859, 320887, 320888, 320917, 320920, 320933, 320934, 320936, 320941, 320980, 320995, 320996, 321004, 321005, 321006, 321007, 321009, 321010, 321011, 321012, 321051, 321055, 321067, 321068, 321069, 321078, 321119, 321121, 321122, 321123, 321124, 321126, 321134, 321138, 321140, 321149, 321162, 321164, 321165, 321166, 321167, 321177, 321178, 321218, 321219, 321221, 321230, 321231, 321232, 321233, 321261, 321262, 321283, 321294, 321295, 321296, 321305, 321310, 321311, 321312, 321313, 321383, 321386, 321388, 321393, 321396, 321397, 321414, 321415, 321431, 321432, 321433, 321434, 321436, 321457, 321461, 321475, 321528, 321536, 321586, 321607, 321636, 321640, 321650, 321652, 321653, 321665,321710, 321712, 321730, 321731, 321732, 321735, 321755, 321758, 321759, 321760, 321773, 321774, 321775, 321776, 321777, 321778, 321780, 321781, 321794, 321796, 321813, 321815, 321817, 321818, 321819, 321820, 321831, 321832, 321833, 321834, 321879, 321880, 321887, 321908, 321909, 321917, 321919, 321933, 321949, 321960, 321961, 321973, 321975, 321988, 321990, 322013, 322014, 322022, 322040, 322049, 322057, 322068, 322079, 322088, 322106, 322113, 322118, 322165, 322167, 322179, 322201, 322204, 322222, 322252, 322317, 322319, 322322, 322324, 322332, 322348, 322355, 322356, 322381, 322407, 322430, 322431, 322480, 322483, 322484, 322485, 322487, 322492, 322510, 322599, 322602, 322603, 322605, 322616, 322617, 322625, 322633, 322958, 322959, 322961, 322963, 322964, 323363, 323364, 323365, 323367, 323368, 323370, 323371, 323373, 323374, 323375, 323376, 323377, 323378, 323388, 323391, 323393, 323394, 323395, 323396, 323397, 323398, 323399, 323414, 323415, 323416, 323417, 323418, 323419, 323420, 323421, 323422, 323423, 323470, 323471, 323472, 323473, 323474, 323475, 323487, 323488, 323492, 323493, 323495, 323524, 323525, 323526, 323693, 323696, 323698, 323699, 323700, 323702, 323725, 323726, 323727, 323755, 323775, 323778, 323790, 323794, 323841, 323857, 323940, 323954, 323976, 323978, 323980, 323983, 323997, 324021, 324022, 324077, 324078, 324201, 324202, 324205, 324206, 324207, 324209, 324237, 324245, 324293, 324315, 324318, 324420, 324564, 324571, 324612, 324729, 324747, 324764, 324765, 324769, 324772, 324785, 324791, 324835, 324840, 324841, 324846, 324878, 324897, 324970, 324980, 324997, 324998, 324999, 325000, 325001, 325022, 325057, 325097, 325098, 325099, 325100, 325101, 325110, 325111, 325113, 325114, 325117, 325159, 325168, 325169, 325170, 325172, 325175};
  }
 
  //std::cout<< run_number.size()<<std::endl;
  
  TH1F *LumiperLS;
  LumiperLS=new TH1F("h", "PCC_per_ls", 70000, 0.0, 70000);
  //LumiperLS=new TH1F("h", "PCC_per_ls", 2000, 0.0, 2000);
  
  TGraph *LumiLSratio;
  LumiLSratio=new TGraph();
  
  TH1F *LumiperLS1;
  LumiperLS1=new TH1F("h1", "hfoc_per_ls", 70000, 0.0, 70000); 
  //LumiperLS1=new TH1F("h1", "hfoc_per_ls", 2000, 0.0, 2000);
  
  TH1F *Lumisection_perrun;
  Lumisection_perrun=new TH1F("h_lr", "# of Lumi sections vs run number", 150, 0, 150);
  
  TH1F *Lumisection_perrun1;
  Lumisection_perrun1=new TH1F("h_lr1", "Lumi sections1 vs run number", 150, 0.0, 150);
  
  TGraph *Lumisectionratio;
  Lumisectionratio=new TGraph();
  
  TGraph *LumiratiovsHF;
  LumiratiovsHF=new TGraph();
  
  TGraph *PCCvsHF;
  PCCvsHF=new TGraph();

  TGraph *PCCvsHF_residual;
  PCCvsHF_residual=new TGraph();

  TGraph *PCC_HFvsHF_residual;
  PCC_HFvsHF_residual=new TGraph();

  //TH1D *h_PCCvsHF_residual;
  //h_PCCvsHF_residual=new TH1D("PCC vs HF residual histogram", "Residuals",500,-20,20);
  
  //TH1D *h_PCC_HFvsHF_residual;
  //h_PCC_HFvsHF_residual=new TH1D("PCC/HF vs HF residual histogram", "Residuals",500,-0.02,0.02);
  
  TH1* h = new TH1D("h_spread", "Spread of ratio plot", 59070, 0.0, 59070); 
  
  TH2F* h_ratio;
  TH2F* h_ratiovsHF;
  TH2F* PCCvsHF_linearity;
  
  //h_ratio = new TH2F("h_ratio", "PCC/HFOC vs lumi section histogram", 28000, 0.0, 70000, 28000, 0.0, 1.4);
  h_ratio = new TH2F("h_ratio", "PCC/HFOC vs lumi section histogram ", 200, 0.0, 70000, 300, 0.4, 1.6);
  //h_ratio = new TH2F("h_ratio", "Fill 6961 ", 2100, 0.0, 2100, 500, 0.4, 1.6);
  //h_ratiovsHF = new TH2F("h_ratiovsHF", "lumi ratio vs HF histogram", 28000, 0.0, 23000, 28000, 0.0, 1.4);
  h_ratiovsHF = new TH2F("h_ratiovsHF", " ", 500, 0.0, 23000, 300, 0.3, 1.3);
  PCCvsHF_linearity = new TH2F("h_PCCvsHF_linearity", "Linearity PCC vs HF histogram", 100, 0.0, 25000, 100, 0.0, 25000);
    
  TH1* h_PCCvsHF_residual = new TH1F("residual histogram PCC vs HF", "Residuals", 300, -0.1, 0.1);
  TH1* h_PCC_HFvsHF_residual = new TH1F("residual histogram PCC/HF vs HF", "Residuals", 300, -0.1, 0.1); 
  
  TFile*f;
  
  int LS=0;
  int previousrunlumisec_count=0;
  int LS1=0;
  int previousrunlumisec_count1=0;
  int run_counter=0;
  float sigmavis=0;
  int run=0;
  int ls=0;
  float LumiLS=0;
  int ls1=0;
  int run1=0;
  string tmp;
  float LumiLS1=0;

  std::map<int, float> cluster_count; 
  
  float refLumi[NLS];
  for(int i=0;i<NLS;i++)  refLumi[i]=0.;
  
  if(run_period=='A'){
    Path = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018A";
  }
  if(run_period=='B'){
    Path = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018B";
  }
  if(run_period=='C'){
    Path = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018C";
  } 
  if(run_period=='D'){
    Path = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018D1";
  }
  if(run_period=='E'){   
    Path = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018D2";
  }
  if(run_period=='F'){
    Path = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018D3";
  }
  if(run_period=='G'){    
    Path = "/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018D4";
  }
  if(run_period=='H'){
    Path = "/eos/user/a/asehrawa/zerobias_allrun";
  }
  

  for (unsigned int j=0;j<run_number.size();j++){  
    TString infile=Path+"/"+run_number.at(j)+".csv"; 
    std::cout<< run_number.at(j)<<std::endl;    
    ifstream myfile (infile.Data());    
    if (!myfile.is_open()){
      cout << "Unable to open PCC file: "<<infile.Data()<<endl; 
      return;
    }    
    
    std::string line;
    int lumisec_count=0;
    float lumisec_count_perrun=0;
    sigmavis=getSigmaVis(run_number.at(j));
    //sigmavis=0.7275* getSigmaVis(run_number.at(j));
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
      //run_counter++;         
      lumisec_count_perrun++;
      
      std::getline(iss,token, ',');
      std::stringstream lumiiss(token);
      lumiiss>>LumiLS;
      cluster_count[ls]=LumiLS;    // map contain old ls and PCC values
      //std::cout<<"oldveto "<<run<< "  "<<ls<<"  "<<cluster_count[ls]<<std::endl;  
      //Lumisection_perrun->Fill(run_counter, lumisec_count_perrun);      
      LumiperLS->Fill(LS, LumiLS/sigmavis);
      //LumiperLS->Fill(ls, LumiLS/sigmavis);
      //std::cout<<"old veto "<<run<< "  "<<ls<<"  "<<LumiLS<<std::endl;
      //std::cout<<"old "<<run<<"    "<< lumisec_count<<std::endl;    
    }

    previousrunlumisec_count+=lumisec_count;
    myfile.close();     
    
    TString infile1=Path+"/"+run_number.at(j)+".hfoc";
    //std::cout<< run_number.at(j)<<std::endl;                                                                                               
    ifstream myfile1 (infile1.Data());
    if (!myfile1.is_open()){
      cout << "Unable to open hfoc file: "<<infile1.Data()<<endl;
      return;
    }
    
    std::string line1;
    string tmp;
    int lumisec_count1=0;
    float lumisec_count_perrun1=0;
    while (std::getline(myfile1, line1)){
      std::stringstream iss(line1);
      //325310 7358 2 2 10/26/18 07 27 01 STABLE BEAMS 6500 8368.161 6041.397 98.2 HFOC 1 0.0760 0.0549 ...
      iss>>run1>>tmp>>ls1>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp;//>>tmp;
      iss>>refLumi[ls1];
      iss>>tmp>>tmp;
      lumisec_count1++;
      LS1 = previousrunlumisec_count1 + ls1;
      lumisec_count_perrun1++;      
      //std::getline(iss1,token1, ',');
      //std::stringstream lumiiss1(token1);
      //lumiiss1>>LumiLS1;
      //Lumisection_perrun1->Fill(run_counter++, lumisec_count_perrun1);
      LumiperLS1->Fill(LS1, refLumi[ls1]);
      //LumiperLS1->Fill(ls1, refLumi[ls1]);
      //std::cout<<"hfoc "<<run<< "  "<<ls<<"  "<<LumiLS1<<std::endl;
      //std::cout<<lumiBX<<std::endl; 
      
      if (run_period=='A'){// && cluster_count[ls1]/sigmavis>=5000 && refLumi[ls1]>=5000){
	if(sigmavis>0 && refLumi[ls1]!=0){

	  LumiLSratio->SetPoint(LumiLSratio->GetN(), LS1, cluster_count[ls1]/(sigmavis*refLumi[ls1]));
	  LumiratiovsHF->SetPoint(LumiratiovsHF->GetN(), refLumi[ls1], cluster_count[ls1]/(sigmavis*refLumi[ls1]));	  
	  PCCvsHF->SetPoint(PCCvsHF->GetN(), refLumi[ls1], cluster_count[ls1]/sigmavis);
	  h_ratio->Fill(LS1, cluster_count[ls1]/(sigmavis*refLumi[ls1]));
	  
	  //std::cout<<run1<<"  "<<LS1<<"  "<<cluster_count[ls1]<<" "<<refLumi[ls1]<<"  "<<sigmavis<<"  "<<cluster_count[ls1]/(sigmavis*refLumi[ls1])<<std::endl;
	  //h_ratio->Fill(ls1, cluster_count[ls1]/(sigmavis*refLumi[ls1]));
	  h_ratiovsHF->Fill(refLumi[ls1], cluster_count[ls1]/(sigmavis*refLumi[ls1]));
	  //std::cout<<h_ratiovsHF->ProjectionY()->GetMean()<<std::endl;
	  PCCvsHF_linearity->Fill(cluster_count[ls1]/sigmavis, refLumi[ls1]);
	  //std::cout<<run<<"  "<<lumisec_count1<<" "<<cluster_count[ls1]/refLumi[ls1] <<std::endl;	  
	}
      }
      
      if (run_period=='B' && LumiLS>=3000000 && LumiLS1>=4000000){
	if(refLumi[ls1]!=0){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), lumisec_count1, cluster_count[ls1]/refLumi[ls1]);
	  //std::cout<<lumisec_count1<<" "<<cluster_count[ls1]/refLumi[ls1] <<std::endl; 
	}
      }
      
      if (run_period=='C'){//&& LumiLS>=1000000 && LumiLS1>=1000000){
	if(sigmavis>0 && refLumi[ls1]!=0){
	  //if(cluster_count[ls1]/(sigmavis*refLumi[ls1])>1.05 && cluster_count[ls1]/(sigmavis*refLumi[ls1])<1.10){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), LS1, cluster_count[ls1]/(sigmavis*refLumi[ls1]));
	  std::cout<<run1<<"  "<<LS1<<"  "<<cluster_count[ls1]<<" "<<refLumi[ls1]<<"  "<<sigmavis<<"  "<<cluster_count[ls1]/(sigmavis*refLumi[ls1])<<std::endl;	  
	  LumiratiovsHF->SetPoint(LumiratiovsHF->GetN(), refLumi[ls1], cluster_count[ls1]/(sigmavis*refLumi[ls1]));
          PCCvsHF->SetPoint(PCCvsHF->GetN(), refLumi[ls1], cluster_count[ls1]/sigmavis);
	  //}          
	  h_ratio->Fill(LS1, cluster_count[ls1]/(sigmavis*refLumi[ls1]));
	  
          //h_ratio->Fill(ls1, cluster_count[ls1]/(sigmavis*refLumi[ls1]));                                                                  
          h_ratiovsHF->Fill(refLumi[ls1], cluster_count[ls1]/(sigmavis*refLumi[ls1]));
          //std::cout<<h_ratiovsHF->ProjectionY()->GetMean()<<std::endl;                                                                     
          PCCvsHF_linearity->Fill(cluster_count[ls1]/sigmavis, refLumi[ls1]);
          //std::cout<<run<<"  "<<lumisec_count1<<" "<<cluster_count[ls1]/refLumi[ls1] <<std::endl;     
	  //std::cout<<lumisec_count1<<" "<<cluster_count[ls1]/refLumi[ls1] <<std::endl; 
	}
      }
      
      if (run_period=='D' && LumiLS>=2000000 && LumiLS1>=2000000){
	if(refLumi[ls1]!=0){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), lumisec_count1, cluster_count[ls1]/refLumi[ls1]);
	  //std::cout<<lumisec_count1<<" "<<cluster_count[ls1]/refLumi[ls1] <<std::endl; 
	}
      }
      
      if (run_period=='E' && LumiLS>=3000000 && LumiLS1>=2000000){
	if(refLumi[ls1]!=0){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), lumisec_count1, cluster_count[ls1]/refLumi[ls1]);
	  //std::cout<<lumisec_count1<<" "<<cluster_count[ls1]/refLumi[ls1] <<std::endl; 
	}
      }
      
      if (run_period=='F' &&  LumiLS>=2000000 && LumiLS1>=2000000){
	if(refLumi[ls1]!=0){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), lumisec_count1, cluster_count[ls1]/refLumi[ls1]);
	  //std::cout<<lumisec_count1<<" "<<cluster_count[ls1]/refLumi[ls1] <<std::endl; 
	}
      }
      
      if (run_period=='G' &&  LumiLS>=1000000 && LumiLS1>=3000000){ 
	if(refLumi[ls1]!=0){
	  LumiLSratio->SetPoint(LumiLSratio->GetN(), lumisec_count1, cluster_count[ls1]/refLumi[ls1]);
	  //std::cout<<lumisec_count1<<" "<<cluster_count[ls1]/refLumi[ls1] <<std::endl; 
	}
      }
      
      if (run_period=='H'){// && cluster_count[ls1]/sigmavis>=5000 && refLumi[ls1]>=5000){                                                   
        if(sigmavis>0 && refLumi[ls1]!=0){
          LumiLSratio->SetPoint(LumiLSratio->GetN(), LS1, cluster_count[ls1]/(sigmavis*refLumi[ls1]));
          LumiratiovsHF->SetPoint(LumiratiovsHF->GetN(), refLumi[ls1], cluster_count[ls1]/(sigmavis*refLumi[ls1]));
          PCCvsHF->SetPoint(PCCvsHF->GetN(), refLumi[ls1], cluster_count[ls1]/sigmavis);
          h_ratio->Fill(LS1, cluster_count[ls1]/(sigmavis*refLumi[ls1]));
          //std::cout<<run1<<"  "<<LS1<<"  "<<cluster_count[ls1]<<" "<<refLumi[ls1]<<"  "<<sigmavis<<"  "<<cluster_count[ls1]/(sigmavis*refLu\mi[ls1])<<std::endl;                                                                                                                          
	//h_ratio->Fill(ls1, cluster_count[ls1]/(sigmavis*refLumi[ls1]));                                                                   
	h_ratiovsHF->Fill(refLumi[ls1], cluster_count[ls1]/(sigmavis*refLumi[ls1]));
	//std::cout<<h_ratiovsHF->ProjectionY()->GetMean()<<std::endl;                                                                      
	PCCvsHF_linearity->Fill(cluster_count[ls1]/sigmavis, refLumi[ls1]);
	//std::cout<<run<<"  "<<lumisec_count1<<" "<<cluster_count[ls1]/refLumi[ls1] <<std::endl;                                           
      }
    }



    }
    previousrunlumisec_count1+=lumisec_count1;
    myfile1.close();
    
    run_counter++; 
    Lumisection_perrun->Fill(run_counter, lumisec_count_perrun);
    //std::cout<<"old "<<run_counter<<"  "<<run<<"  "<<lumisec_count_perrun<<std::endl;      
    
    Lumisection_perrun1->Fill(run_counter, lumisec_count_perrun1);
    //std::cout<<"new "<<run_counter<<"  "<<run<<"  "<<lumisec_count_perrun1<<std::endl;
    
    Lumisectionratio->SetPoint(Lumisectionratio->GetN(), run_counter, lumisec_count_perrun1/lumisec_count_perrun);
    //std::cout <<run_counter<<"  "<<run<< "  " << lumisec_count_perrun1/lumisec_count_perrun << std::endl; 
    
    if(lumisec_count_perrun1/lumisec_count_perrun!=1){
      //std::cout <<run<< "  " << lumisec_count_perrun<<"       "<<lumisec_count_perrun1<<"         "<<lumisec_count_perrun1/lumisec_count_perrun << std::endl;
      //std::cout <<run<<".log ";
    }
    //std::cout<<"new "<<run<<"    "<< lumisec_count1<<std::endl;
    
  }
  
  //std::cout<<h_ratiovsHF->ProjectionY()->GetMean()<<std::endl;

  TF1 *fitfunc = new TF1("fitfunc","[0]*x+[1]",0,22000);
  LumiratiovsHF->Fit("fitfunc");

  TProfile* P = PCCvsHF_linearity->ProfileX();
  //P->SetTitle("ProfileX of PCC vs HFOC linearity histogram");
  P->GetXaxis()->SetTitle("HF Inst. Lumi");
  P->GetYaxis()->SetTitle("PCC");

  //TF1 *fitfn = new TF1("fitfn","[0]*x+[1]",0,25000);
  //P->SetMarkerStyle(8);
  //P->SetMarkerColor(2);
  //P->Fit("fitfn");

  TProfile* P1 = h_ratiovsHF->ProfileX();
  P1->SetTitle("ProfileX of PCC/HFOC vs HFOC histogram");
  P1->GetXaxis()->SetTitle("HFOC");
  P1->GetYaxis()->SetTitle("PCC/HFOC");
  P1->GetYaxis()->SetRangeUser(0.8, 1.2);  

  int x_value;
  float bincontent;
  for(int a = 1; a <= P1->GetNbinsX(); a++) {
    x_value =  P1->GetBinCenter(a);
    bincontent = P1->GetBinContent(a);   
    //std::cout<<x_value<<" "<<bincontent<<std::endl;
  }
  
  
  TF1 *fitfn = new TF1("fitfn","[0]*x+[1]",0,25000);
  P1->SetMarkerStyle(8);
  P1->SetMarkerColor(2);
  P1->Fit("fitfn");

  for (unsigned int j=0;j<run_number.size();j++){
    TString infile1=Path+"/"+run_number.at(j)+".hfoc";
    //std::cout<< run_number.at(j)<<std::endl;                                                                                               
    ifstream myfile1 (infile1.Data());
    if (!myfile1.is_open()){
      cout << "Unable to open new file: "<<infile1.Data()<<endl;
      return;
    }
    
    std::string line1;
    int ls1=0;
    int run1=0;
    string tmp;
    while (std::getline(myfile1, line1)){
      std::stringstream iss(line1);
      //325310 7358 2 2 10/26/18 07 27 01 STABLE BEAMS 6500 8368.161 6041.397 98.2 HFOC 1 0.0760 0.0549 ...                                  
      iss>>run1>>tmp>>ls1>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp;//>>tmp;                                                                   
      iss>>refLumi[ls1];
      iss>>tmp>>tmp;                                                                           
      if(fitfn->Eval(P->GetBinCenter(ls1))!=0){
	PCCvsHF_residual->SetPoint(PCCvsHF_residual->GetN(), P->GetBinCenter(ls1), ((P->GetBinContent(ls1))-fitfn->Eval(P->GetBinCenter(ls1)))/fitfn->Eval(P->GetBinCenter(ls1)));
	h_PCCvsHF_residual->Fill(((P->GetBinContent(ls1))-fitfn->Eval(P->GetBinCenter(ls1)))/fitfn->Eval(P->GetBinCenter(ls1)));
	//PCCvsHF_residual->SetPointError(PCCvsHF_residual->GetN(), 0, (P->GetBinError(ls1))/(fitfn->Eval(P->GetBinCenter(ls1))));
	//std::cout<<(P->GetBinError(ls1))/(fitfn->Eval(P->GetBinCenter(ls1)))<<std::endl;
	//std::cout<<PCCvsHF_residual->GetN()<<"  "<< P->GetBinCenter(ls1)<<"  "<<P->GetBinContent(ls1)<<"  "<<fitfn->Eval(P->GetBinCenter(ls1))<<"  "<<((P->GetBinContent(ls1))-fitfn->Eval(P->GetBinCenter(ls1)))/fitfn->Eval(P->GetBinCenter(ls1))<<std::endl;
	PCC_HFvsHF_residual->SetPoint(PCC_HFvsHF_residual->GetN(), P1->GetBinCenter(ls1), ((P1->GetBinContent(ls1))-fitfn->Eval(P1->GetBinCenter(ls1)))/fitfn->Eval(P1->GetBinCenter(ls1)));
	//h_PCC_HFvsHF_residual->Fill(((P1->GetBinContent(ls1))-fitfn->Eval(P1->GetBinCenter(ls1)))/fitfn->Eval(P1->GetBinCenter(ls1)));
	//std::cout<<PCC_HFvsHF_residual->GetN()<<"  "<< P1->GetBinCenter(ls1)<<"  "<<P1->GetBinContent(ls1)<<"  "<<fitfn->Eval(P1->GetBinCenter(ls1))<<"  "<<((P1->GetBinContent(ls1))-fitfn->Eval(P1->GetBinCenter(ls1)))/fitfn->Eval(P1->GetBinCenter(ls1))<<std::endl;
      }
    }
  }

  h_ratio->GetXaxis()->SetTitle("Lumi section");
  h_ratio->GetYaxis()->SetTitle("PCC/HFOC");
  h_ratiovsHF->GetXaxis()->SetTitle("HF Inst. Lumi");
  h_ratiovsHF->GetYaxis()->SetTitle("PCC/HFOC");
  
  TH1D* ProjY_h_ratio;
  ProjY_h_ratio=h_ratio->ProjectionY("ProjY_ratio_ls", 0, 70000);
  //ProjY_h_ratio=h_ratio->ProjectionY();
  
  TH1D* ProjY_h_ratiovsHF;
  //ProjY_h_ratiovsHF=h_ratiovsHF->ProjectionY();
  ProjY_h_ratiovsHF=h_ratiovsHF->ProjectionY("ProjY_ratio_HF",0, 70000);
  
  LumiperLS->GetXaxis()->SetTitle("Lumi section");
  LumiperLS->GetYaxis()->SetTitle("PCC");
  //LumiperLS->GetYaxis()->SetRangeUser(1, 1000000);
  //LumiperLS->GetYaxis()->SetRangeUser(1, 200000);

  LumiperLS->SetMarkerStyle(8);
  LumiperLS->SetMarkerColor(2);  
  LumiperLS->SetMarkerSize(0.4);   
  LumiperLS->Draw("histp");
  
  if(run_period=='A'){
    C->Print(Path1+"Lumiperls_PCC_Run2018A"+".png");
  }
  if(run_period=='B'){
    C->Print(Path1+"Lumiperls_PCC_Run2018B"+".png");
  }
  if(run_period=='C'){
    C->Print(Path1+"Lumiperls_PCC_Run2018C"+".png");
  }
  if(run_period=='D'){
    C->Print(Path1+"Lumiperls_PCC_Run2018D1"+".png");
  }
  if(run_period=='E'){
    C->Print(Path1+"Lumiperls_PCC_Run2018D2"+".png");
  }
  if(run_period=='F'){
    C->Print(Path1+"Lumiperls_PCC_Run2018D3"+".png");
  }
  if(run_period=='G'){
    C->Print(Path1+"Lumiperls_PCC_Run2018D4"+".png");
  }
  if(run_period=='H'){
    C->Print(Path1+"Lumiperls_PCC_Run2018_all"+".png");
  }
 
 
  LumiperLS1->GetXaxis()->SetTitle("Lumi section");
  LumiperLS1->GetYaxis()->SetTitle("HFOC");
  LumiperLS1->SetMarkerStyle(8);
  LumiperLS1->SetMarkerColor(1);
  LumiperLS1->SetMarkerSize(0.4);
  LumiperLS1->Draw("histp");
  //LumiperLS1->GetYaxis()->SetRangeUser(1, 1000000);
  
  if(run_period=='A'){    
    C->Print(Path1+"Lumiperls_hfoc_Run2018A"+".png");
  }
  if(run_period=='B'){
    C->Print(Path1+"Lumiperls_hfoc_Run2018B"+".png");
  }
  if(run_period=='C'){
    C->Print(Path1+"Lumiperls_hfoc_Run2018C"+".png");
  }
  if(run_period=='D'){
    C->Print(Path1+"Lumiperls_hfoc_Run2018D1"+".png");
  }
  if(run_period=='E'){
    C->Print(Path1+"Lumiperls_hfoc_Run2018D2"+".png");
  }
  if(run_period=='F'){
    C->Print(Path1+"Lumiperls_hfoc_Run2018D3"+".png");
  }
  if(run_period=='G'){
    C->Print(Path1+"Lumiperls_hfoc_Run2018D4"+".png");
  }
  if(run_period=='H'){
    C->Print(Path1+"Lumiperls_hfoc_Run2018_all"+".png");
  }

  
  TCanvas*C1 = new TCanvas("Luminosity PCC HFOC on same canvas");
  C1->cd();
  C1->SetLogy();
  
  LumiperLS->GetXaxis()->SetTitle("Lumi section");
  LumiperLS->GetYaxis()->SetTitle("PCC or HFOC");
  //LumiperLS->GetYaxis()->SetRangeUser(1, 1000000);
  //LumiperLS->GetYaxis()->SetRangeUser(1, 120000);
  LumiperLS->SetMarkerStyle(8);
  LumiperLS->SetLineColor(2);
  LumiperLS->SetMarkerColor(2);
  LumiperLS->SetMarkerSize(0.4);
  LumiperLS1->GetXaxis()->SetTitle("Lumi section");
  LumiperLS1->GetYaxis()->SetTitle("PCC or HFOC");
  //LumiperLS1->GetYaxis()->SetRangeUser(1, 1000000);
  //LumiperLS1->GetYaxis()->SetRangeUser(1, 120000);

  LumiperLS1->SetMarkerStyle(8);
  LumiperLS1->SetLineColor(1);
  LumiperLS1->SetMarkerColor(1);
  LumiperLS1->SetMarkerSize(0.4);
  LumiperLS->Draw("histp");
  LumiperLS1->Draw("histpsame");
  
  auto legend1 = new TLegend(0.75, 0.75, 0.88, 0.88);
  legend1->AddEntry(LumiperLS, "PCC", "l");
  legend1->AddEntry(LumiperLS1, "HFOC", "l");
  //legend1->AddEntry(LumiperLS, "PCC", "l");
  legend1->SetFillColor(0);
  legend1->SetLineColor(0);
  legend1->SetFillColor(0);
  legend1->Draw("same");
  
  if(run_period=='A'){
    LumiperLS->GetXaxis()->SetRangeUser(0, 70000);
    LumiperLS1->GetXaxis()->SetRangeUser(0, 70000);
    //C1->Print(Path1+"Lumiperls_PCC_hfoc_Run2018A"+".png");
  }
  if(run_period=='B'){
    C1->Print(Path1+"Lumiperls_PCC_hfoc_Run2018B"+".png");
  }
  if(run_period=='C'){
    LumiperLS->GetXaxis()->SetRangeUser(0, 35000);
    LumiperLS1->GetXaxis()->SetRangeUser(0, 35000);
    C1->Print(Path1+"Lumiperls_PCC_hfoc_Run2018C"+".png");
  }
  if(run_period=='D'){
    LumiperLS->GetXaxis()->SetRangeUser(0, 50000);
    LumiperLS1->GetXaxis()->SetRangeUser(0, 50000);
    C1->Print(Path1+"Lumiperls_PCC_hfoc_Run2018D1"+".png");
  }
  if(run_period=='E'){
    LumiperLS->GetXaxis()->SetRangeUser(0, 45000);
    LumiperLS1->GetXaxis()->SetRangeUser(0, 45000);    
    C1->Print(Path1+"Lumiperls_PCC_hfoc_Run2018D2"+".png");
  }
  if(run_period=='F'){
    LumiperLS->GetXaxis()->SetRangeUser(0, 30000);
    LumiperLS1->GetXaxis()->SetRangeUser(0, 30000);
    C1->Print(Path1+"Lumiperls_PCC_hfoc_Run2018D3"+".png");
  }
  if(run_period=='G'){
    LumiperLS->GetXaxis()->SetRangeUser(0, 25000);
    LumiperLS1->GetXaxis()->SetRangeUser(0, 25000);
    C1->Print(Path1+"Lumiperls_PCC_hfoc_Run2018D4"+".png");
  }
  if(run_period=='H'){
    //LumiperLS->GetXaxis()->SetRangeUser(0, 25000);
    //LumiperLS1->GetXaxis()->SetRangeUser(0, 25000);
    C1->Print(Path1+"Lumiperls_PCC_hfoc_Run2018_all"+".png");
  }

  
  TCanvas*C2 = new TCanvas("Luminosity ratio (new old veto)");
  C2->cd();
  
  if(run_period=='A'){
    LumiLSratio ->SetTitle("Run2018A (315252-316995)");
    LumiLSratio->GetXaxis()->SetRangeUser(0, 70000);
    LumiLSratio ->GetYaxis()->SetRangeUser(0, 2);
  }
  if(run_period=='B'){
    LumiLSratio->SetTitle("Run2018B (317080-319311)");
    LumiLSratio ->GetYaxis()->SetRangeUser(0, 50);
  }
  if(run_period=='C'){
    //LumiLSratio->SetTitle("Run2018C (319337-320065)");
    LumiLSratio->SetTitle("Fill 6961");
    //LumiLSratio ->GetYaxis()->SetRangeUser(1, 1.15);  
  }
  if(run_period=='D'){
    LumiLSratio->SetTitle("Run2018D (320500-321665)");
    //LumiLSratio ->GetYaxis()->SetRangeUser(0.8, 0.9); 
  }
  if(run_period=='E'){
    LumiLSratio->SetTitle("Run2018D (321710-322964)");
    //LumiLSratio ->GetYaxis()->SetRangeUser(0.75, 0.95); 
  }
  if(run_period=='F'){
    LumiLSratio->SetTitle("Run2018D (323363-324420)");
    //LumiLSratio ->GetYaxis()->SetRangeUser(1.85, 2);
  }
  if(run_period=='G'){   
    LumiLSratio->SetTitle("Run2018D (324564-325175)");
    //LumiLSratio ->GetYaxis()->SetRangeUser(2.05, 2.2); 
  }

  
  LumiLSratio->GetXaxis()->SetTitle("Lumi section");                                                                                 
  LumiLSratio->GetYaxis()->SetTitle("PCC/HFOC");
  LumiLSratio ->GetXaxis()->SetRangeUser(0,70000);  
  LumiLSratio->Draw("AP"); 
  
  if(run_period=='A'){
    LumiLSratio->Draw("AP");
    C2->Print(Path1+"Lumiratio_PCC_hfoc_Run2018A"+".png");
  }  
  if(run_period=='B'){
    C2->Print(Path1+"Lumiratio_PCC_hfoc_Run2018B"+".png");
  }
  if(run_period=='C'){
    C2->Print(Path1+"Lumiratio_PCC_hfoc_Run2018C"+".root");
  }
  if(run_period=='D'){
    C2->Print(Path1+"Lumiratio_PCC_hfoc_Run2018D1"+".png");
  }
  if(run_period=='E'){
    C2->Print(Path1+"Lumiratio_PCC_hfoc_Run2018D2"+".png");
  }
  if(run_period=='F'){
    C2->Print(Path1+"Lumiratio_PCC_hfoc_Run2018D3"+".png");
  }
  if(run_period=='G'){
    C2->Print(Path1+"Lumiratio_PCC_hfoc_Run2018D4"+".png");
  }
  
  if(run_period=='H'){
    C2->Print(Path1+"Lumiratio_PCC_hfoc_Run2018_all"+".png");
  }
  
  TCanvas*C3 = new TCanvas("Lumi section ratio (new old veto)");
  C3->cd();
  
  Lumisectionratio->GetXaxis()->SetTitle("run number");
  Lumisectionratio->GetYaxis()->SetTitle("new ls/old ls");
  Lumisectionratio->SetTitle("Lumi section ratio per run");
  //Lumisectionratio->GetYaxis()->SetRangeUser(0,2500);                                                                                      
  Lumisectionratio->Draw("AP");
  
  if(run_period=='A'){
    Lumisectionratio->SetTitle("Run2018A (315252-316995)");
    //C3->Print(Path1+"Lumisectionratio_PCC_hfoc_Run2018A"+".png");
  }
  if(run_period=='B'){
    Lumisectionratio->SetTitle("Run2018B (317080-319311)");
    C3->Print(Path1+"Lumisectionratio_PCC_hfoc_Run2018B"+".png");
  }
  if(run_period=='C'){
    Lumisectionratio->SetTitle("Run2018C (319337-320065)");
    C3->Print(Path1+"Lumisectionratio_PCC_hfoc_Run2018C"+".png");
  }
  if(run_period=='D'){
    Lumisectionratio->SetTitle("Run2018D (320500-321665)");
    C3->Print(Path1+"Lumisectionratio_PCC_hfoc_Run2018D1"+".png");
  }
  if(run_period=='E'){
    Lumisectionratio->SetTitle("Run2018D (321710-322964)");
    C3->Print(Path1+"Lumisectionratio_PCC_hfoc_Run2018D2"+".png");
  }
  if(run_period=='F'){
    Lumisectionratio->SetTitle("Run2018D (323363-324420)");
    C3->Print(Path1+"Lumisectionratio_PCC_hfoc_Run2018D3"+".png");
  }
  if(run_period=='G'){
    Lumisectionratio->SetTitle("Run2018D (324564-325175)");
    Lumisectionratio->GetYaxis()->SetRangeUser(0, 1.1);
    C3->Print(Path1+"Lumisectionratio_PCC_hfoc_Run2018D4"+".png");
  }
  
  
  TCanvas*C4 = new TCanvas("Lumisection vs run number new old veto on same canvas");
  C4->cd();
  
  Lumisection_perrun->GetXaxis()->SetTitle("run");
  Lumisection_perrun->GetYaxis()->SetTitle("# of lumi section");
  Lumisection_perrun->GetYaxis()->SetRangeUser(0, 2500);
  Lumisection_perrun->SetMarkerStyle(8);
  Lumisection_perrun->SetLineColor(2);
  Lumisection_perrun->SetMarkerColor(2);
  Lumisection_perrun->SetMarkerSize(0.5);
  Lumisection_perrun1->GetXaxis()->SetTitle("run");
  Lumisection_perrun1->GetYaxis()->SetTitle("# of lumi section");
  Lumisection_perrun1->GetYaxis()->SetRangeUser(0, 2500);
  Lumisection_perrun1->SetMarkerStyle(8);
  Lumisection_perrun1->SetLineColor(1);
  Lumisection_perrun1->SetMarkerColor(1);
  Lumisection_perrun1->SetMarkerSize(0.4);
  Lumisection_perrun->Draw("histp");
  Lumisection_perrun1->Draw("histpsame");
  
  auto legend2 = new TLegend(0.75, 0.75, 0.88, 0.88);
  legend2->AddEntry(Lumisection_perrun1, "New veto", "l");
  legend2->AddEntry(Lumisection_perrun, "Old veto", "l");
  legend2->SetFillColor(0);
  legend2->SetLineColor(0);
  legend2->SetFillColor(0);
  legend2->Draw("same");
  
  if(run_period=='A'){
    //C4->Print(Path1+"Lumisection_run_PCC_hfoc_Run2018A"+".png");
  }
  if(run_period=='B'){
    C4->Print(Path1+"Lumisection_run_PCC_hfoc_Run2018B"+".png");
  }
  if(run_period=='C'){
    //LumiperLS->GetXaxis()->SetRangeUser(0, 35000);
    //LumiperLS1->GetXaxis()->SetRangeUser(0, 35000);
    C4->Print(Path1+"Lumisection_run_PCC_hfoc_Run2018C"+".png");
  }
  if(run_period=='D'){
    //LumiperLS->GetXaxis()->SetRangeUser(0, 50000);
    //LumiperLS1->GetXaxis()->SetRangeUser(0, 50000);
    C4->Print(Path1+"Lumisection_run_PCC_hfoc_Run2018D1"+".png");
  }
  if(run_period=='E'){
    //LumiperLS->GetXaxis()->SetRangeUser(0, 45000);
    //LumiperLS1->GetXaxis()->SetRangeUser(0, 45000);    
    C4->Print(Path1+"Lumisection_run_PCC_hfoc_Run2018D2"+".png");
  }
  if(run_period=='F'){
    //LumiperLS->GetXaxis()->SetRangeUser(0, 30000);
    //LumiperLS1->GetXaxis()->SetRangeUser(0, 30000);
    C4->Print(Path1+"Lumisection_run_PCC_hfoc_Run2018D3"+".png");
  }
  if(run_period=='G'){
    Lumisection_perrun->GetXaxis()->SetRangeUser(0, 60);
    Lumisection_perrun1->GetXaxis()->SetRangeUser(0, 60);
    C4->Print(Path1+"Lumisection_run_PCC_hfoc_Run2018D4"+".png");
  }
  
 
  TCanvas*C5 = new TCanvas("Luminosity ratio PCC/HFOC vs HF");
  C5->cd();
  
  if(run_period=='A'){
    LumiratiovsHF ->SetTitle("Run2018A (315252-316995)");
    //LumiratiovsHF->GetXaxis()->SetRangeUser(0, 70000);
    //LumiLSratiovsHF ->GetYaxis()->SetRangeUser(350, 450);
  }
  
  LumiratiovsHF->GetXaxis()->SetTitle("HFOC");                                                                                 
  LumiratiovsHF->GetYaxis()->SetTitle("PCC/HFOC");
  LumiratiovsHF->SetMarkerStyle(8);
  LumiratiovsHF->SetLineColor(1);
  LumiratiovsHF->SetMarkerColor(1);
  LumiratiovsHF->SetMarkerSize(0.5);
  LumiratiovsHF->GetYaxis()->SetRangeUser(0, 2);
  //LumiLSratiovsHF ->GetXaxis()->SetRangeUser(0,70000);  
  LumiratiovsHF->Draw("AP"); 
  
  PCCvsHF->GetXaxis()->SetTitle("HFOC");
  PCCvsHF->GetYaxis()->SetTitle("PCC");
  PCCvsHF->SetMarkerStyle(8);
  PCCvsHF->SetLineColor(2);
  PCCvsHF->SetMarkerColor(2);
  PCCvsHF->SetMarkerSize(0.5);
  //PCCvsHF->GetYaxis()->SetRangeUser(0,2);
  //LumiLSratiovsHF ->GetXaxis()->SetRangeUser(0,70000);                                                                                     
  //LumiratiovsHF->Draw("AP");
  
  PCCvsHF_linearity->GetXaxis()->SetTitle("HFOC");
  PCCvsHF_linearity->GetYaxis()->SetTitle("PCC");
  PCCvsHF_linearity->SetMarkerStyle(8);
  PCCvsHF_linearity->SetLineColor(2);
  PCCvsHF_linearity->SetMarkerColor(2);
  PCCvsHF_linearity->SetMarkerSize(0.5);

  if(run_period=='A'){
    LumiratiovsHF->Draw("AP");
    C5->Print(Path1+"Lumiratio_PCC_hfoc_vsHF_Run2018A"+".png");
    //PCCvsHF->SetTitle("Run2018A (315252-316995)");
    PCCvsHF->SetTitle("run number 316380");
    PCCvsHF->Draw("AP");
    //C5->Print(Path1+"PCCvsHF_Run2018A"+".png");
  }  
  
  PCCvsHF_residual->GetXaxis()->SetTitle("HFOC");
  PCCvsHF_residual->GetYaxis()->SetTitle("(PCC-fit)/fit");
  PCCvsHF_residual->GetXaxis()->SetRangeUser(0, 25000);
  PCCvsHF_residual->GetYaxis()->SetRangeUser(-0.1, 0.1);
  PCCvsHF_residual->SetMarkerStyle(8);
  PCCvsHF_residual->SetLineColor(1);
  PCCvsHF_residual->SetMarkerColor(1);
  PCCvsHF_residual->SetMarkerSize(0.3);


  PCC_HFvsHF_residual->GetXaxis()->SetTitle("HFOC");
  PCC_HFvsHF_residual->GetYaxis()->SetTitle("(PCC/HFOC-fit)/fit");
  PCC_HFvsHF_residual->GetXaxis()->SetRangeUser(0, 25000);
  PCC_HFvsHF_residual->GetYaxis()->SetRangeUser(-0.1, 0.1);
  PCC_HFvsHF_residual->SetMarkerStyle(8);
  PCC_HFvsHF_residual->SetLineColor(1);
  PCC_HFvsHF_residual->SetMarkerColor(1);
  PCC_HFvsHF_residual->SetMarkerSize(0.3);
  

  h_PCC_HFvsHF_residual->GetXaxis()->SetTitle("residuals");
  h_PCC_HFvsHF_residual->GetYaxis()->SetTitle("Entries");
  //h_PCC_HFvsHF_residual->GetYaxis()->SetRangeUser(0, 500);
  //h_PCC_HFvsHF_residual->GetXaxis()->SetRangeUser(-0.02, 0.02);
  h_PCC_HFvsHF_residual->SetMarkerStyle(8);
  h_PCC_HFvsHF_residual->SetLineColor(1);
  h_PCC_HFvsHF_residual->SetMarkerColor(1);
  h_PCC_HFvsHF_residual->SetMarkerSize(0.3);


  ProjY_h_ratio->GetXaxis()->SetTitle("lumi section");
  ProjY_h_ratio->GetYaxis()->SetTitle("PCC/HFOC");
  ProjY_h_ratiovsHF->GetXaxis()->SetTitle("HFOC");
  ProjY_h_ratiovsHF->GetYaxis()->SetTitle("PCC/HFOC");
  ProjY_h_ratio->GetYaxis()->SetRangeUser(0, 400);
  ProjY_h_ratiovsHF->GetYaxis()->SetRangeUser(0, 400);
  ProjY_h_ratio->GetXaxis()->SetRangeUser(0.8, 1.2);
  ProjY_h_ratiovsHF->GetXaxis()->SetRangeUser(0.8, 1.2);

  ProjY_h_ratio->SetMarkerStyle(8);
  ProjY_h_ratio->SetLineColor(1);
  ProjY_h_ratio->SetMarkerColor(1);
  ProjY_h_ratio->SetMarkerSize(0.7);
  ProjY_h_ratiovsHF->SetMarkerStyle(8);
  ProjY_h_ratiovsHF->SetLineColor(1);
  ProjY_h_ratiovsHF->SetMarkerColor(1);
  ProjY_h_ratiovsHF->SetMarkerSize(0.7);

  if(run_period=='A'){
    PCCvsHF_residual->SetTitle("Run2018A (315252-316995)");
    PCCvsHF_residual->SetTitle("Residuals");
    TLine* line = new TLine(0, 0, 25000, 0);
    line->SetLineColor(kBlack);
    line->SetLineStyle(2);

    TLine* line1 = new TLine(0, -0.01, 25000, -0.01);
    line1->SetLineColor(kBlack);
    line1->SetLineStyle(2);

    TLine* line2 = new TLine(0, 0.01, 25000, 0.01);
    line2->SetLineColor(kBlack);
    line2->SetLineStyle(2);

    PCCvsHF_residual->Draw("AP");
    line->Draw("same");
    line1->Draw("same");
    line2->Draw("same");
    C5->Print(Path1+"PCCvsHF_residual_Run2018A"+".png");

    TLine* line3 = new TLine(0, 0, 25000, 0);
    line3->SetLineColor(kBlack);
    line3->SetLineStyle(2);
    TLine* line4 = new TLine(0, -0.01, 25000, -0.01);
    line4->SetLineColor(kBlack);
    line4->SetLineStyle(2);
    TLine* line5 = new TLine(0, 0.01, 25000, 0.01);
    line5->SetLineColor(kBlack);
    line5->SetLineStyle(2);
    PCC_HFvsHF_residual->SetTitle("Residuals");
    PCC_HFvsHF_residual->Draw("AP");
    line3->Draw("same");                                                                                                                    
    line4->Draw("same");                                                                                                                   
    line5->Draw("same"); 
    C5->Print(Path1+"PCC_HFvsHF_residual_Run2018A"+".png");

    h_PCCvsHF_residual->Draw("histp");
    C5->Print(Path1+"PCCvsHF_residual_histo_Run2018A"+".png");
    //h_ratio->Draw("histp");
    //C5->Print(Path1+"Lumiratio_PCC_hfoc_vs_ls_histo_Run2018A"+".png");
    //h_ratiovsHF->Draw("histp");
    //C5->Print(Path1+"Lumiratio_PCC_hfoc_vs_HFOC_histo_Run2018A"+".png");
    h_ratio->Draw("colz");
    C5->Print(Path1+"Lumiratio_PCC_hfoc_vs_ls_histo_Run2018A"+".png");
    h_ratiovsHF->Draw("colz");
    C5->Print(Path1+"Lumiratio_PCC_hfoc_vs_HFOC_histo_Run2018A"+".png");
    //ProjY_h_ratio->Draw("histp");
    //C5->Print(Path1+"Lumiratio_PCC_hfoc_vs_ls_histo_projectionY_Run2018A"+".png");
    //ProjY_h_ratiovsHF->Draw("histp");
    //C5->Print(Path1+"Lumiratio_PCC_hfoc_vs_HFOC_histo_projectionY_Run2018A"+".png");
    PCCvsHF_linearity->Draw("colz");
    C5->Print(Path1+"PCC_vs_HFOC_histo_linearity_Run2018A"+".png");
    P->Draw("P");   
    C5->Print(Path1+"PCC_vs_HFOC_histo_linearity_tprofile_Run2018A"+".png");
    P1->Draw("histp");
    fitfn->Draw("same");
    C5->Print(Path1+"PCC_hfoc_vs_HFOC_histo_linearity_tprofile_Run2018A"+".png");
  }

    
}
