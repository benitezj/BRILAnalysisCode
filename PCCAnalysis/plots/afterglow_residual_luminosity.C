#include "globals.h"
#include <iostream>
#include <vector>
#include <dirent.h>
const Double_t dmarker[10] = {8, 21, 22,29,33, 34, 21, 45,8, 21};
const Color_t dcol[10] = {kRed,kBlue,kGreen+1,kAzure+7,kViolet-5,kRed-3,kRed-5,kBlue-5,kGreen,kYellow};
const char *legname[10]={"run1", "run2","run3", "run1", "run1", "run1", "run1", "run1","run1", "run1"};

void setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // Canvas parameters
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600);
  tdrStyle->SetCanvasDefW(600);
  tdrStyle->SetCanvasDefX(0);
  tdrStyle->SetCanvasDefY(0);

  // Pad parameters
  tdrStyle->SetPadBorderMode(0);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);
 

  // Frame parameters
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

  // Margins
  tdrStyle->SetPadTopMargin(0.1);
  tdrStyle->SetPadBottomMargin(0.1);
  tdrStyle->SetPadLeftMargin(0.12);
  tdrStyle->SetPadRightMargin(0.12);

  // Global title parameters
  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);

  // Set the style
  gROOT->SetStyle("tdrStyle");
  gROOT->ForceStyle();
}


void addCMSLabels(TPad* pad) {
  pad->cd();  // Switch to the pad to add labels

  // Create TLatex object
  TLatex latex;
  latex.SetNDC();  // Use normalized coordinates
  latex.SetTextFont(42); // Helvetica
  latex.SetTextSize(0.04);

  // Add the CMS Preliminary label
  //latex.DrawLatex(0.15, 0.91, "#font[62]{CMS} #font[62]{Preliminary}");
  latex.DrawLatex(0.15, 0.91, "#font[62]{Afterglow Background Residuals}");

  // Add the center-of-mass energy on the right
  latex.DrawLatex(0.75, 0.91, "#sqrt{s} = 13 TeV");
}

void afterglow_residual_luminosity() {

  setTDRStyle();

  //TString inpath="/eos/user/a/asehrawa/PCC/EXPRESS_datasets/AlCaPCCRandom/Run2018_RD_test/Run2018";  
  //TString inpath="/eos/user/a/asehrawa/PCC_test_newtype2afterglowparameters/AlCaPCCRandom/Run2018_RD_test/Run2018";
  //TString inpath="/afs/cern.ch/user/a/asehrawa/13March2023_PCC_processing/CMSSW_10_6_30/src";
  //TString inpath="/eos/user/a/asehrawa/PCC_RD_315252_316062/AlCaPCCRandom/Run2018A_RD_test_new";

  TString inpath="/eos/user/a/asehrawa/PCC_newafterglowparameters_23May2023/AlCaPCCRandom/Run2018_27May2023";

  TString outpath="/eos/user/a/asehrawa/BRIL-new/";
  
  //TCanvas*C = new TCanvas("residual vs inst lumi");
  TCanvas *C = new TCanvas("C","C",0,0,1400,1200);
  C->cd();
  
  std::vector<int> run;
  TF1 *linear = new TF1("linear", "pol1");
  
  //float sigmavisible=9.229e6/ORBITF;
  float sigmavisible=0.960601e6/ORBITF;
  //float sigmavisible=1;

  //run ={316114, 319851};

  run = {315257,315258,315259,315264,315265,315267,315270,315322,315339,315357,315361,315363,315365,315366,315420,315488,315489,315490,315506,315509,315510,315512,315543,315555,315556,315557,315640,315641,//315642, 
  315644,315645,315646,315647,315648,315689,315690,315702,315703,315704,315705,315713,315721,315741,315764,315770,315784,315785,315786,315788,315789,315790,315800,315801,315840,315973,315974,316058,316059,316060, 316061,316062,316082,316095,316109,316110,316111,316112,316113,316114,316151,316153,316186,316187,316199,316200,316201,316202,316216,316217,316218,316219,316239,316240,316241,316260,316271,316361,316362,316363,316377,316378,316379,316380,316455,316456,316457,316469,316470,316472,316477,316478,316505,316524,316525,316559,316569,316590,316613,316615,316664,316665,316666,316667,316700,316701,316702,316715,316716,316717,316718,316719,316720,316721,316722,316723,316758,316766,316797,316798,316799,316808,316839,316876,316877,316878,316879,316928,316944,316985,316993,316994,316995,317080,317087,317088,317089,317170,317182,317212,317213,317239,317279,317291,317292,317295,317296,317297,317319,317320,317338,317339,317340,317382,317383,317391,317392,317434,317435,317438,317475,317478,317479,317480,317481,317482,317484,317488,317509,317510,317511,317512,317527,317580,317591,317626,317640,317641,317648,317649,317650,317661,317663,317683,317696,318070,318518,318622,318653,318661,318662,318663,318667,318669,318670,318675,318712,318714,318733,318734,318785,318816,318817,318819,318820,318828,318834,318836,318837,318872,318874,318876,318877,318939,318944,318945,318953,318980,318981,318982,318983,318984,318992,319006,319011,319015,319016,319018,319019,319077,319097,319098,319099,319100,319101,319103,319104,319124,319125,319159,319160,319173,319174,319175,319176,319177,319190,319222,319223,319254,319255,319256,319260,319262,319263,319264,319265,319266,319267,319268,319270,319273,319274,319300,319310,319311,319337,319347,319348,319349,319449,319450,319452,319456,319459,319460,319462,319463,319464,319466,319467,319468,319469,319470,319471,319472,319486,319488,319503,319506,319524,319525,319526,319528,319557,319559,319579,319606,319625,319639,319656,319657,319658,319659,319678,319687,319697,319698,319756,319840,319841,319847,319848,319849,319851,319852,319853,319854,319908,319909,319910,319912,319913,319914,319915,319928,319941,319942,319950,319991,319992,319993,320002,320003,320006,320007,320008,320009,320010,320011,320012,320023,320024,320025,320026,320038,320039,320040,320058,320059,320060,320061,320062,320063,320064,320065,320500,320569,320570,320571,320612,320617,320654,320673,320674,320688,320712,320757,320804,320807,320809,320821,320822,320823,320824,320838,320840,320841,320853,320854,320855,320856,320857,320858,320859,320887,320888,320917,320920,320933,320934,320936,320941,320980,320995,320996,321004,321005,321006,321007,321009,321010,321011,321012,321051,321055,321067,321068,321069,321078,321119,321121,321122,321123,321124,321126,321134,321138,321140,321149,321162,321164,321165,321166,321167,321177,321178,321218,321219,321221,321230,321231,321232,321233,321261,321262,321283,321294,321295,321296,321305,321310,321311,321312,321313,321383,321386,321388,321393,321396,321397,321414,321415,321431,321432,321433,321434,321436,321457,321461,321475,321528,321536,321586,321607,321636,321640,321650,321652,321653,321665,321710,321712,321730,321732,321735,321755,321758,321759,321760,321773,321774,321775,321776,321777,321778,321780,321781,321794,321796,321813,321815,321817,321818,321819,321820,321831,321832,321833,321834,321879,321880,321887,321908,321909,321917,321919,321933,321949,321960,321961,321973,321975,321988,321990,322013,322014,322022,322040,322049,322057,322068,322079,322088,322106,322113,322118,322165,322167,322179,322201,322204,322222,322252,322317,322319,322322,322324,322332,322348,322355,322356,322381,322407,322430,322431,322480,322483,322484,322485,322487,322492,322510,322599,322602,322603,322605,322616,322617,322625,322633,322672,322958,322959,322961,322963,322964,323220,323221,323224,323270,323279,323363,323364,323365,323367,323368,323370,323371,323373,323374,323375,323376,323377,323378,323388,323391,323393,323394,323395,323396,323397,323398,323399,323414,323415,323416,323417,323418,323419,323420,323421,323422,323423,323470,323471,323472,323473,323474,323475,323488,323492,323493,323495,323513,323524,323525,323526,323558,323560,323693,323696,323698,323699,323700,323702,323725,323726,323727,323755,323775,323778,323790,323794,323825,323841,323857,323940,323954,323976,323978,323980,323983,323997,324021,324022,324077,324078,324131,324161,324162,324163,324164,324167,324201,324202,324205,324206,324207,324209,324237,324245,324293,324315,324318,324393,324404,324408,324418,324420,324564,324571,324612,324645,324729,324747,324764,324765,324769,324772,324785,324791,324835,324840,324841,324846,324878,324897,324903,324968,324970,324980,324997,324998,324999,325000,325001,325022,325057,325065,325097,325098,325099,325100,325101,325110,325111,325113,325114,325117,325159,325168,325169,325170,325172,325175};

  //run= {317212,317212,317213,317213,317239,317239,317279,317279,317291,317291,317292,317292,317295,317295,317296,317296,317297,317297,317319,317319,317320,317320,317338,317338,317339,317339,317340,317340,317382,317382,317383,317383,317391,317391,317392,317392,317434,317434,317435,317435,317438,317438,317475,317475,317478,317478,317479,317479,317480,317480,317481,317481,317482,317482,317484,317484,317488,317488,317509,317509,317510,317510,317511,317511,317512,317512,317527,317527,317580,317580,317591,317591,317626,317626,317640,317640,317641,317641,317648,317648,317649,317649,317650,317650,317661,317661,317663,317663,317683,317683,317696,317696,318070,318070,318518,318518,318622,318622,318653,318653,318661,318661,318662,318662,318663,318663,318667,318667,318669,318669,318670,318670,318675,318675,318712,318712,318714,318714,318733,318733,318734,318734,318785,318785,318816,318816,318817,318817,318819,318819,318820,318820,318828,318828,318834,318834,318836,318836,318837,318837,318872,318872,318874,318874,318876,318876,318877,318877,318939,318939,318944,318944,318945,318945,318953,318953,318980,318980,318981,318981,318982,318982,318984,318984,319006,319006,319011,319011,319015,319015,319016,319016,319018,319018,319019,319019,319077,319077,319097,319097,319098,319098,319099,319099,319100,319100,319101,319101,319103,319103,319104,319104,319124,319124,319125,319125,319159,319159,319160,319160,319173,319173,319174,319174,319175,319175,319176,319176,319177,319177,319190,319190,319222,319222,319223,319223,319254,319254,319255,319255,319256,319256,319262,319262,319263,319263,319264,319264,319265,319265,319266,319266,319267,319267,319268,319268,319270,319270,319273,319273,319274,319274,319300,319300,319310,319310,319311,319311};

  //run={315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315543, 315555, 315556, 315557, 315640, 315641, //315642, 
  //315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785, 315786, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062};

  //run = {//315252, 315255, 315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315533, 315543, 315555, 315556, 315557, 315587, 315640, 315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785, 315786, 315787, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 
  //316061, 316062, 316082, 316095, 316109, 316110, 316111, 316112, 316113, 316114, 316151, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219,316239, 316240, 316241, 316260, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316456, 316457, 316469, 316470, 316472,316477, 316478, 316505, 316569, 316590, 316613, 316615, 316664, 316665, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, //316722,
      //316723, 316758, 316766, 316797, 316798, 316799, 316808, //316820, 
	    //316876,316877, 316878,316879, 316928, 316944, 316985, 316993, 316994, 316995, 317080, 317087, 317088, 317089, 317170, 317182, 317212, 317213, 317239, 317279,317291, 317292, 317295, 317296, 317297, 317319, 317320,317338, 317339, 317340, 317382, 317383, 317391, 317392, 317434, 317435, 317438, 317475, 317478, 317479, 317480, 317481, 317482, 317484, 317488, 317509, 317510, 317511, 317512, 317527, 317580, 317591, 317626, 317640, 317641, 317648,317649, 317650, 317661, 317663, 317683, 317696, 318070, 318622, 318653, 318661, 318662, 318663, 318667, 318669, 318670, 318675, 318712,318714, 318733, 318734, 318785,318816, 318817, 318819, 318820, 318828, 318834, 318836, 318837, 318872, 318874, 318876, 318877, 318939, 318944, 318945, 318953, 318980, 318981, 318982, 318983, 318984, 318992, 319006, 319011, 319015, 319016, 319018, 319019, 319077, 319097, 319098, 319099, 319100, 319101, 319103, 319104, 319124, 319125, 319159, 319160,319173, 319174, 319175, 319176, 319177, 319190, 319222, 319223, 319254, 319255, 319256, 319260, 319262, 319263, 319264, 319265, 319266, 319267, 319268, 319270, 319273, 319274, 319300, 319310, 319311, 319337, 319347, 319348, 319349, 319449, 319450, 319452, 319456, 319459, 319460, 319462, 319463, 319464, 319466, 319467, 319468, 319469, 319470, 319471, 319472, 319486, 319488, 319503, 319506, 319524, 319525, 319526, 319528, 319579, 319606, 319625, 319639, 319656, 319657, 319658, 319659, 319678, 319687, 319697, 319698, 319756, 319840, 319841, 319847, 319848, 319849, 319851, 319852, 319853, 319854, 319908, 319909, 319910, 319912, 319913, 319914, 319915, 319928, 319941, 319942, 319950, 319991, 319992, 319993, 320002, 320003, 320006, 320007, 320008, 320009, 320010, 320011, 320012, 320023, 320024, 320025, 320026, 320038, 320039, 320040, 320058, 320059, 320060, 320061, 320062, 320063, 320064, 320065, 320500, 320569, 320570, 320571, 320612, 320617, 320654, 320673, 320674, 320688, 320712, 320757, 320804, 320807, 320809, 320821, 320822, 320823, 320824, 320838, 320840, 320841, 320853, 320854, 320855, 320856, 320857, 320858, 320859, 320887, 320888, 320917, 320920, 320933, 320934, 320936, 320941, 320980, 320995, 320996, 321004, 321005, 321006, 321007, 321009, 321010, 321011, 321012, 321051, 321055, 321067, 321068, 321069, 321078, 321119, 321121, 321122, 321123, 321124, 321126, 321134, 321138, 321140, 321149, 321162, 321164, 321165, 321166, 321167, 321177, 321178, 321218, 321219, 321221, 321230, 321231, 321232, 321233, 321261, 321262, 321283, 321294, 321295, 321296, 321305, 321310, 321311, 321312, 321313, 321383, 321386, 321388, 321393, 321396, 321397, 321414, 321415, 321431, 321432, 321433, 321434, 321436, 321457, 321461, 321475, 321528, 321536, 321586, 321607, 321636, 321640, 321650, 321652, 321653, 321665, 321710, 321712, 321730, 321731, 321732, 321735, 321755, 321758, 321759, 321760, 321773, 321774, 321775, 321776, 321777, 321778, 321780, 321781, 321794, 321796, 321813, 321815, 321817, 321818, 321819, 321820, 321831, 321832, 321833, 321834, 321879, 321880, 321887, 321908, 321909, 321917, 321919, 321933, 321949, 321960, 321961, 321973, 321975, 321988, 321990, 322013, 322014, 322022, 322040, 322049, 322057, 322068, 322079, 322088, 322106, 322113, 322118, 322165, 322167, 322179, 322201, 322204, 322222, 322252, 322317, 322319, 322322, 322324, 322332, 322348, 322355, 322356, 322381, 322407, 322430, 322431, 322480, 322483, 322484, 322485, 322487, 322492, 322510, 322599, 322602, 322603, 322605, 322616, 322617, 322625, 322633, 322958, 322959, 322961, 322963, 322964, 323363, 323364, 323365, 323367, 323368, 323370,323371, 323373, 323374, 323375, 323376, 323377, 323378, 323388, 323391, 323393, 323394, 323395, 323396, 323397, 323398, 323399, 323414, 323415, 323416, 323417, 323418, 323419, 323420, 323421, 323422, 323423, 323470, 323471, 323472, 323473, 323474, 323475, 323487, 323488, 323492, 323493, 323495, 323524, 323525, 323526, 323693, 323696, 323698, 323699, 323700, 323702, 323725, 323726, 323727, 323755, 323775, 323778, 323790, 323794, 323841, 323857, 323940, 323954, 323976, 323978, 323980, 323983, 323997, 324021, 324022, 324077, 324078, 324201, 324202, 324205, 324206, 324207, 324209, 324237, 324245, 324293, 324315, 324318, 324420, 324564, 324571, 324612, 324729, 324747, 324764, 324765, 324769, 324772, 324785, 324791, 324835, 324840, 324841, 324846, 324878, 324897, 324970, 324980, 324997, 324998, 324999, 325000, 325001, 325022, 325057, 325097, 325098, 325099, 325100, 325101, 325110, 325111, 325113, 325114, 325117, 325159, 325168, 325169, 325170, 325172, 325175};

  //run = {315252, 315255, 315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315533, 315543, 315555, 315556, 315557, 315587, 315640, 315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785, 315786, 315787, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062, 316082, 316095, 316109, 316110, 316111, 316112, 316113, 316114, 316151, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219,316239, 316240, 316241, 316260, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316456, 316457, 316469, 316470, 316472, 316477, 316478, 316505, 316569, 316590, 316613, 316615, 316664, 316665, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723, 316758, 316766, 316797, 316798, 316799, 316808, 316820, 316839, 316851, 316876,316877, 316878,316879, 316928, 316944, 316985, 316993, 316994, 316995};
  
  //run = {316766};
 
  int e = run.size();
  TGraphErrors* residual_type1;
  TGraph* residualvsinstlumi[e];
  TH1F *hist = new TH1F("hist", "Type2 residual vs sbil", 1000, 0, 10);
  TH2F *hist2D = new TH2F("hist2D", "2D Histogram;X-Axis;Y-Axis", 100, -4, 4, 100, -20, 20);

  for (unsigned int i=0;i<run.size();i++){
    //TFile Afterglow(inpath+"/"+run.at(i)+".root");
    TFile Afterglow(inpath + "/" + TString::Itoa(run.at(i), 10) + ".root");
    //cout<<"Run "<<run.at(i)<<"  "<<inpath<<endl;
    //std::cout<<run.at(i)<<std::endl;
    residualvsinstlumi[i]=new TGraph();
    //hist = new TH1F();
    residual_type1 = (TGraphErrors*)Afterglow.Get("Type1Fraction");
    //residual_type1 = (TGraphErrors*)Afterglow.Get("Type1Res");
    //residual_type1 = (TGraphErrors*)Afterglow.Get("Type2Res");
    
    //*****************************************
    int N = residual_type1->GetN();
    cout<<N<<endl;
    float  bincontent_avg_array[N];
    int m = 0;
    
    //**********************************************
    
    TIter next(Afterglow.GetListOfKeys());
    TObject* key;
    while ((key = next())) {
      TString kname(key->GetName());
      if(!kname.Contains("CorrectedLumiAvg")) continue;
      TObjArray * a = kname.Tokenize("_");
      long r=atoi(((TObjString*)(*a)[1])->GetName());
      long l=atoi(((TObjString*)(*a)[2])->GetName());
      long ls1=atoi(((TObjString*)(*a)[3])->GetName());
      long ls2=atoi(((TObjString*)(*a)[4])->GetName());
      
      //std::cout<<r<<" "<<l<<" "<<ls1<<" "<<ls2<<std::endl;
      if(r!=run.at(i)) continue;
      TH1F* Lumi = (TH1F*) Afterglow.Get(TString("CorrectedLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
      if(!Lumi)      continue;
      
      float bincontent_total=0;
      int bxcounter=0;      
      for(int j = 1; j <= Lumi->GetNbinsX(); j++) { 
	int x_value =  Lumi->GetBinCenter(j);
	//std::cout<<x_value<<std::endl;
	float bincontent = Lumi->GetBinContent(j); 
	if (bincontent > 100) {   // only colliding bunches
	  bincontent_total+=bincontent;
	  bxcounter++;
	  //std::cout<<x_value<<std::endl;
	}
      }
      
      float bincontent_avg=0.;
      if(bxcounter>1000){
	bincontent_avg=bincontent_total/(sigmavisible*bxcounter);      
	//std::cout<<run.at(i)<<" "<<x_value<<" "<<bincontent_total<<" "<<bincontent_avg<<" "<<std::endl;
      }
      //****************************
      if (bincontent_avg > 0){
	bincontent_avg_array[m] = bincontent_avg;
	//****************************
	//std::cout<<run.at(i)<<" =============" <<bincontent_avg<<" ============= "<<bincontent_avg_array[m]<<std::endl;      
      } m++;
      //residualvsinstlumi->SetPoint(residualvsinstlumi->GetN(), bincontent_avg, Y[k]); 
      //std::cout<<residualvsinstlumi->GetN()<<" "<<bincontent_avg<<" "<<Y[k]<<std::endl;
      
      //Lumi->SetTitle(TString("Run: ")+run.at(i)+" , LS: "+ls1+" - "+ls2);
      Lumi->SetTitle(TString("Run: ") + TString::Itoa(run.at(i), 10) + " , LS: " + ls1 + " - " + ls2);     
      Lumi->GetYaxis()->SetTitle("Corrected counts");
      Lumi->GetYaxis()->SetNdivisions(4);
      Lumi->GetXaxis()->SetTitle("bcid");
      Lumi->SetMarkerColor(4);
      Lumi->Draw("hist");
      //C->Print(outpath+"/compareRandomRawCorr_"+run.at(i)+"_"+l+"_"+ls1+"_"+ls2+".pdf");
    }                                      
    double *X = residual_type1->GetX();
    double *Y = residual_type1->GetY();
    
    for (int k = 0; k < residual_type1->GetN(); k++) {     
      //cout << run.at(i)<<" "<<"X: " << X[k] << " Y: " << Y[k] << endl;
      //cout<<Y[k]<<endl;
      //cout<<residual_type1->GetN()<<endl;
      //if(bincontent_avg_array[k] > 0.05){
	residualvsinstlumi[i]->SetPoint(residualvsinstlumi[i]->GetN(), bincontent_avg_array[k], Y[k]); 
	std::cout<<residualvsinstlumi[i]->GetN()<<" "<<run.at(i)<<" "<<bincontent_avg_array[k]<<" "<<Y[k]<<" "<<std::endl;
	//}
    }
  
    //residual_type1->SetTitle(TString("Run: ")+run.at(i));
    residual_type1->SetTitle(TString("Run: ") + TString::Itoa(run.at(i), 10));
    residual_type1->GetYaxis()->SetTitle("Type1 residual fraction");
    residual_type1->GetYaxis()->SetNdivisions(4);
    residual_type1->GetXaxis()->SetTitle("Unique LS ID");
    residual_type1->SetMarkerColor(2);
    residual_type1->Draw("AP");
    //C->Print(outpath+"/residualtype1_"+run.at(i)+".pdf");                                                          
    
    //residualvsinstlumi[i]->GetYaxis()->SetTitle("Type1 residual");
    residualvsinstlumi[i]->GetYaxis()->SetNdivisions(4);
    //residualvsinstlumi[i]->GetXaxis()->SetTitle("Inst. lumi");
    residualvsinstlumi[i]->SetMarkerColor(dcol[i]);
    //residualvsinstlumi->GetYaxis()->SetRangeUser(0.00002, 0.00005);
    //residualvsinstlumi[i]->Draw("AP same");
    //C->Print(outpath+"residualtype1vsinstlumi_"+run.at(i)+".pdf");
  }
  
  //TCanvas *cf = new TCanvas("cf"," ", 0,67,1264,678);
  TCanvas *cf = new TCanvas("cf"," ", 0,67,800,600);
  cf->GetFrame()->SetFillColor(21);
  cf->GetFrame()->SetBorderSize(12); 
  
  TH2F *fhist = new TH2F("fhist","",1000,0,10,100,0,0.04);
  //TH2F *fhist = new TH2F("fhist","",1000,0,10,100,0,0.006);
  fhist->GetXaxis()->SetTitle("Instantaneous luminosity (Hz/ub)");
  fhist->GetYaxis()->SetTitle("Type1 residua fractionl");
  //fhist->GetYaxis()->SetTitle("Type2 residual");
  fhist->SetMarkerColor(kBlue);
  linear->SetLineColor(kBlue);
  //fhist->SetMarkerSize(1.5);
  //fhist->SetMarkerStyle(21);
  //fhist->GetYaxis()->SetTitle("Type2 residual");
  //fhist->GetXaxis()->CenterTitle();
  //fhist->GetYaxis()->CenterTitle();
  //c1->SetLogy();
  //gStyle->SetOptStat(111111);

  fhist->Draw("");
  
  for(int k = 0; k < run.size(); k++){
   
    for (int l = 0; l < residualvsinstlumi[k]->GetN(); l++) {
      double x, y;
      residualvsinstlumi[k]->GetPoint(l, x, y);
      hist->Fill(x, y);
    }

    //residualvsinstlumi[k]->SetMarkerStyle(dmarker[k]);
    residualvsinstlumi[k]->SetMarkerStyle(20);
    residualvsinstlumi[k]->SetMarkerColor(4);
    residualvsinstlumi[k]->SetMarkerSize(0.5);
    residualvsinstlumi[k]->Draw("P same");

    //leg->AddEntry(residualvsinstlumi[k],legname[k],"p");
    
  }
  //cf->Print(outpath+"residualtype1fractionvsinstlumi_"+".png");
  //cf->Print(outpath+"residualtype1vsinstlumi_"+".png");

  gStyle->SetOptStat(0);

  addCMSLabels(cf);

  // Update the canvas                                                                                                                                                                                             
  cf->Modified();
  cf->Update();

  cf->Print(outpath+"residualtype1fracvsinstlumi_"+".png");

  hist->SetMarkerStyle(20);
  hist->SetMarkerSize(0.5);
  hist->SetMarkerColor(4);
  hist->GetYaxis()->SetRangeUser(-0.05, 0.05);
  gStyle->SetOptStat(0);
  //hist->Draw("COLZ");
  //cf->Print(outpath+"residualtype2vsinstlumi_histo_"+".png");
  //gStyle->SetOptStat(1);

  //hist->Fit(linear);

  
  double totalSum = 0.0;
  int totalPoints = 0;
  std::vector<double> all_y_values;
  double mean = 0.0;
  double variance = 0.0;
  double std_dev = 0.0;
  double rms = 0.0;

  
  double totalSumx = 0.0;
  int totalPointsx = 0;
  std::vector<double> all_x_values;
  double meanx = 0.0;



  for(int r = 0; r < run.size(); r++){
  for(int k = 0; k < residualvsinstlumi[k]->GetN(); k++) {
    TGraph* g = residualvsinstlumi[k];
    int nPoints = g->GetN();
    int nPointsx = g->GetN();

    totalPoints += nPoints;
    totalPointsx += nPointsx;

    double *y_values = g->GetY();
    double *x_values = g->GetX();
    for(int i = 0; i < nPoints; i++) {
      totalSum += y_values[i];
      all_y_values.push_back(y_values[i]);
    }

    for(int j = 0; j < nPoints; j++) {
      totalSumx += x_values[j];
      all_x_values.push_back(x_values[j]);
    }

  }

  mean = totalSum / totalPoints;
  meanx = totalSumx / totalPointsx;

  double squared_diff_sum = 0.0;
  for(int i = 0; i < totalPoints; i++) {
    squared_diff_sum += std::pow((all_y_values[i] - mean), 2);
  }

  variance = squared_diff_sum / totalPoints;
  std_dev = std::sqrt(variance);
  rms = std_dev; 

  }
  cout << " total number of points "<<totalPoints<<" total sum is  "<<totalSum<<"   mean x   "<<meanx<<"  mean y  "<<mean <<"  Standard Deviation: " << std_dev << ", RMS: " << rms << endl;


  /**

  double totalSumX = 0;
  double totalPointsX = 0;
  double meanX = 0;
  double varianceX = 0;
  double std_devX = 0;
  double rmsX = 0;
  std::vector<double> all_x_values;

  for(int r = 0; r < run.size(); r++){
    for(int k = 0; k < residualvsinstlumi[k]->GetN(); k++) {
      TGraph* g = residualvsinstlumi[k];
      int nPoints = g->GetN();

      double *y_values = g->GetY();
      double *x_values = g->GetX();
      for(int i = 0; i < nPoints; i++) {
	if(x_values[i] > 1) {  // only include points where x > 1
	  totalSum += y_values[i];
	  all_y_values.push_back(y_values[i]);

	  totalSumX += x_values[i];
	  all_x_values.push_back(x_values[i]);

	  totalPoints++;
	  totalPointsX++;
	}
      }
    }

    mean = totalSum / totalPoints;
    meanX = totalSumX / totalPointsX;

    double squared_diff_sum = 0.0;
    double squared_diff_sumX = 0.0;
    for(int i = 0; i < totalPoints; i++) {
      squared_diff_sum += std::pow((all_y_values[i] - mean), 2);
      squared_diff_sumX += std::pow((all_x_values[i] - meanX), 2);
    }

    variance = squared_diff_sum / totalPoints;
    std_dev = std::sqrt(variance);
    rms = std_dev;

    varianceX = squared_diff_sumX / totalPointsX;
    std_devX = std::sqrt(varianceX);
    rmsX = std_devX;
  }

  cout << "Total number of points: "<<totalPoints<<"\n";
  cout << "Total Y sum: "<<totalSum<<"\n";
  cout << "Mean Y value: "<<mean<<"\n";
  cout << "Y Standard Deviation: " << std_dev << ", RMS Y: " << rms << "\n";
  cout << "Total X sum: "<<totalSumX<<"\n";
  cout << "Mean X value: "<<meanX<<"\n";
  cout << "X Standard Deviation: " << std_devX << ", RMS X: " << rmsX << endl;













  // Assuming residualvsinstlumi is an array of TGraph pointers
  //double totalSum = 0.0;
  //int totalPoints = 0;

  //for(int k = 0; k < residualvsinstlumi[k]->GetN(); k++) { // Replace NUM_GRAPHS with the actual number of TGraphs
  //TGraph* g = residualvsinstlumi[k];
  //int nPoints = g->GetN();
  //totalPoints += nPoints;

  //double *y_values = g->GetY();
  //for(int i = 0; i < nPoints; i++) {
  //  totalSum += y_values[i];
  //}
  //}

  //double overallMean = totalSum / totalPoints;
  //cout << "total� points "<<totalPoints<<"Overall Mean: " << overallMean << endl;


  //double squared_diff_sum = 0.0;
  //for(int i = 0; i < totalPoints; i++) {
  //squared_diff_sum += std::pow((all_y_values[i] - overallMean), 2);
  //}

  //double variance = squared_diff_sum / totalPoints;
  //double std_dev = std::sqrt(variance);
  //double rms = std_dev; // In this case, RMS is equivalent to standard deviation

  //cout << "Standard Deviation: " << std_dev << ", RMS: " << rms << endl;

  //TPaveText *pt = new TPaveText(0.6, 0.7, 0.9, 0.9, "NDC");
  //pt->AddText(Form("Slope: %.2f +/- %.2f", linear->GetParameter(1), linear->GetParError(1)));
  //pt->AddText(Form("Intercept: %.2f +/- %.2f", linear->GetParameter(0), linear->GetParError(0)));
  //hist->Draw("");
  //linear->Draw("same");
  //pt->Draw("same");
  //cf->Print(outpath+"histo_residua2type1vsinstlumi_"+".png");
  **/
}

