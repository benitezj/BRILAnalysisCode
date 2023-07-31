#include"globals.h" 
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include "modules_361913.h"

TCanvas C("C");


#define MAXMODWEIGHT 0.01

bool makeModuleGraphs=1;
bool makePerLayerPlots=0;

float RMSThr=100000;

bool makeModuleFitStability=0;
float StabilityThr=10000;
float StabilityMax=0.99;
float StabilityMaxPerLayer=0.03;

bool makeModuleFitLinearity=0;
float LinearityThr=10000;
float LinearityMax=0.03;
float LinearityMaxPerLayer=0.03;

bool fitModuleNoise=0;
float VdMNoiseThr=10000;


#define MAXTOTPCC 500E6

//#define MINTOTPCCAVG 70E3 //Old veto
//#define MAXTOTPCCAVG 250E3  

//#define MINTOTPCCAVG 50E3  //2022F final veto 
//#define MAXTOTPCCAVG 230E3

//#define MINTOTPCCAVG 50E3  //final 2022FCDEG veto
//#define MAXTOTPCCAVG 155E3

#define MINTOTPCCAVG 0E3    //vdM fill
#define MAXTOTPCCAVG 140E3

#define NBINTOTPCCAVG 100

#define NLSBLOCK 1


//TString OutPath = "/afs/cern.ch/user/b/benitezj/www/BRIL/PCC_lumi/ModuleVeto2022";
TString OutPath = "./tmp";


//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/test/veto_B0.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_CDEFG_3_2022.txt";

//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022F_Stability2p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022F_Stability2p08p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022F_Stability2p08p_Linearity04p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022F_Stability2p08p_Linearity04p025p.txt";

//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022FStab2p08pLin04p025p_2022CStab2p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022FStab2p08pLin04p025p_2022CStab2p1p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022FStab2p08pLin04p025p_2022CStab2p1pLin04p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022FStab2p08pLin04p025p_2022CStab2p1pLin04p03p.txt";

//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022FStab2p08pLin04p025p_2022CStab2p1pLin04p03p_2022EStab06p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022FStab2p08pLin04p025p_2022CStab2p1pLin04p03p_2022EStab06pLin03p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022FStab2p08pLin04p025p_2022CStab2p1pLin04p03p_2022EStab06pLin03p_2022GStab06p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022FStab2p08pLin04p025p_2022CStab2p1pLin04p03p_2022EStab06pLin03p_2022GStab06pLin03p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022FStab2p08pLin04p025p_2022CStab2p1pLin04p03p_2022EStab06pLin03p_2022GStab06pLin03p_DStab06p.txt";

//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto2022_FStab2p08pLin04p025p_CStab2p1pLin04p03p_EStab06pLin03p_GStab06pLin03p_DStab06pLin03p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto2022_FStab2p08pLin04p025p_CStab2p1pLin04p03p_EStab06pLin03p_GStab06pLin03p_DStab06pLin03p_vdmStab1p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto2022_FStab2p08pLin04p025p_CStab2p1pLin04p03p_EStab06pLin03p_GStab06pLin03p_DStab06pLin03p_vdmStab1p04p.txt";
TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto2022_FStab2p08pLin04p025p_CStab2p1pLin04p03p_EStab06pLin03p_GStab06pLin03p_DStab06pLin03p_vdmStab1p04pNoise05p.txt";


//#define LS_ID_MAX 35000
//TString InputPath = "./ModuleVeto2022/data/Run2022C"; 
//std::vector<int> run_number = {355862,355863,355865,355869,355870,355871,355872,355890,355891,355892,355912,355913,355921,355922,355927,355933,355941,355942,355976,355988,355989,355990,355991,355992,355993,355994,355995,355996,355997,355998,355999,356001,356002,356003,356004,356005,356010,356011,356030,356031,356032,356033,356041,356042,356043,356044,356045,356046,356047,356048,356071,356073,356074,356075,356076,356077,356091,356101,356135,356152,356158,356161,356162,356163,356164,356165,356169,356170,356174,356175,356177,356223,356241,356248,356286,356290,356309,356316,356321,356322,356323,356363,356365,356371,356375,356376,356377,356378,356381,356382,356383,356384,356385,356386,356409,356413,356426,356427,356428,356431,356432,356433,356434,356435,356436,356445,356446,356465,356466,356467,356468,356469,356470,356471,356472,356473,356474,356475,356476,356478,356479,356480,356481,356488,356489,356516,356522,356523,356524,356525,356526,356527,356528,356529,356530,356531,356563,356568,356569,356570,356576,356577,356578,356579,356580,356581,356582,356613,356614,356615,356616,356617,356618,356619,356633,356653,356655,356671,356684,356685,356690,356692,356709,356719,356720,356721,356722,356727,356729,356780,356788,356789,356810,356811,356812,356813,356814,356815,356824,356825,356831,356849,356902,356906,356908,356918,356919,356936,356937,356943,356944,356945,356946,356947,356948,356949,356950,356951,356952,356953,356954,356955,356956,356968,356969,356970,356977,356980,356997,356998,356999,357000,357001,357005,357059,357070,357076,357077,357078,357079,357080,357081,357096,357098,357100,357101,357102,357103,357104,357105,357106,357110,357112,357150,357212,357268,357269,357270,357271,357301,357302,357318,357328,357329,357330,357331,357332,357333,357339,357360,357391,357401,357406,357438,357440,357441,357442,357447,357472,357478,357479,357482}; //oms collisions22


//#define LS_ID_MAX 10000
//TString InputPath = "./ModuleVeto2022/data/Run2022D";   
//std::vector<int> run_number = {357538,357539,357540,357542,357550,357610,357611,357612,357613,357688,357696,357697,357698,357699,357700,357701,357704,357705,357706,357720,357732,357733,357758,357759,357767,357776,357777,357778,357779,357780,357781,357802,357803,357804,357805,357806,357807,357808,357809,357812,357813,357814,357842,357898,357900};


//#define LS_ID_MAX 15000
//TString InputPath = "./ModuleVeto2022/data/Run2022E"; //"/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022E";  
//std::vector<int> run_number = {359685,359686,359691,359693,359694,359699,359717,359718,359746,359747,359750,359751,359762,359763,359764,359765,359766,359776,359779,359781,359806,359808,359809,359810,359812,359813,359814,359864,359865,359867,359868,359869,359870,359871,359884,359885,359887,359889,359899,359901,359903,359904,359905,359906,359908,359909,359915,359917,359920,359934,359936,359943,359976,359993,359998,360012,360017,360018,360019,360075,360088,360090,360116,360120,360121,360122,360123,360124,360125,360126,360127,360128,360129,360130,360131,360141,360152,360190,360208,360217,360224,360225,360274,360279,360285,360286,360295,360296,360309,360327};//oms collisions2022  - vdm



TString InputPath = "./ModuleVeto2022/data/Run2022F";//"/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022F";   
//#define LS_ID_MIN 0
//#define LS_ID_MAX 60000
//std::vector<int> run_number = {3360390,360391,360392,360393,360400,360411,360413,360414,360428,360429,360430,360431,360432,360433,360434,360435,360436,360437,360438,360439,360440,360441,360442,360444,360445,360458,360459,360460,360472,360486,360490,360491,360519,360520,360523,360525,360526,360528,360530,360533,360538,360542,360590,360591,360594,360659,360660,360666,360681,360683,360684,360685,360688,360696,360697,360698,360703,360736,360737,360761,360787,360788,360794,360795,360796,360797,360813,360815,360816,360817,360819,360820,360821,360822,360823,360825,360826,360850,360856,360874,360882,360886,360887,360888,360889,360890,360891,360892,360893,360894,360895,360912,360919,360920,360921,360927,360940,360941,360942,360943,360944,360945,360946,360948,360949,360950,360951,360968,360989,360991,360992,361020,361032,361038,361044,361045,361046,361047,361048,361050,361051,361052,361053,361054,361059,361074,361082,361083,361088,361089,361090,361091,361092,361101,361105,361106,361107,361108,361110,361177,361183,361188,361193,361195,361196,361197,361198,361208,361223,361239,361240,361267,361269,361270,361271,361272,361279,361280,361283,361284,361292,361294,361297,361303,361318,361319,361320,361333,361361,361362,361363,361364,361365,361366,361370,361398,361400,361417,361424,361442,361443,361464,361467,361468,361475,361503,361512,361562,361564,361569,361570,361573,361579,361580,361604,361606,361607,361609,361614,361618,361677,361685,361686,361722,361723,361724,361740,361790,361791,361807,361808,361810,361848,361849,361852,361859,361860,361872,361881,361885,361887,361889,361890,361893,361894,361895,361896,361898,361905,361906,361948,361954,361957,361968,361971,361974,361989,361990,361992,361993,361994,362009,362058,362059,362060,362061,362062,362063,362064,362085,362086,362087,362091,362095,362104,362105,362106,362107,362108,362148,362152,362153,362154,362159,362160,362161,362163,362166,362167}; ///collisions2022 from OMS

//std::vector<int> run_number = {360458,360459,360460}; //fill with linearity issue in Antonio's veto

//#define LS_ID_MIN 0
//#define LS_ID_MAX 2900
//std::vector<int> run_number = {361906,361909,361910,361912,361913,361915,361916,361917,361919,361921,361922,361923,361925,361926,361927,361929,361932,361933}; //vdM fill 8385

//#define LS_ID_MIN 140
//#define LS_ID_MAX 180
//std::vector<int> run_number = {361919}; //vdM noise fits


#define LS_ID_MIN 250
#define LS_ID_MAX 550
std::vector<int> run_number = {361913}; //vdM noise fits




//#define LS_ID_MAX 8000
//TString InputPath = "./ModuleVeto2022/data/Run2022G"; //"/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022F";   
//std::vector<int> run_number = {362433,362434,362435,362436,362437,362438,362439,362452,362457,362459,362469,362471,362482,362484,362485,362487,362497,362503,362524,362527,362528,362548,362550,362587,362596,362597,362613,362614,362615,362616,362617,362618,362622,362623,362624,362625,362628,362646,362653,362654,362655,362656,362657,362673,362695,362696,362697,362698,362720,362721,362722,362723,362724,362725,362726,362727,362728,362757,362758,362759,362760}; //oms collisions2022



//#define LS_ID_MAX 120000
//TString InputPath = "./ModuleVeto2022/data/Run2022"; 
//std::vector<int> run_number = {355862,355863,355865,355869,355870,355871,355872,355890,355891,355892,355912,355913,355921,355922,355927,355933,355941,355942,355976,355988,355989,355990,355991,355992,355993,355994,355995,355996,355997,355998,355999,356001,356002,356003,356004,356005,356010,356011,356030,356031,356032,356033,356041,356042,356043,356044,356045,356046,356047,356048,356071,356073,356074,356075,356076,356077,356091,356101,356135,356152,356158,356161,356162,356163,356164,356165,356169,356170,356174,356175,356177,356223,356241,356248,356286,356290,356309,356316,356321,356322,356323,356363,356365,356371,356375,356376,356377,356378,356381,356382,356383,356384,356385,356386,356409,356413,356426,356427,356428,356431,356432,356433,356434,356435,356436,356445,356446,356465,356466,356467,356468,356469,356470,356471,356472,356473,356474,356475,356476,356478,356479,356480,356481,356488,356489,356516,356522,356523,356524,356525,356526,356527,356528,356529,356530,356531,356563,356568,356569,356570,356576,356577,356578,356579,356580,356581,356582,356613,356614,356615,356616,356617,356618,356619,356633,356653,356655,356671,356684,356685,356690,356692,356709,356719,356720,356721,356722,356727,356729,356780,356788,356789,356810,356811,356812,356813,356814,356815,356824,356825,356831,356849,356902,356906,356908,356918,356919,356936,356937,356943,356944,356945,356946,356947,356948,356949,356950,356951,356952,356953,356954,356955,356956,356968,356969,356970,356977,356980,356997,356998,356999,357000,357001,357005,357059,357070,357076,357077,357078,357079,357080,357081,357096,357098,357100,357101,357102,357103,357104,357105,357106,357110,357112,357150,357212,357268,357269,357270,357271,357301,357302,357318,357328,357329,357330,357331,357332,357333,357339,357360,357391,357401,357406,357438,357440,357441,357442,357447,357472,357478,357479,357482,357538,357539,357540,357542,357550,357610,357611,357612,357613,357688,357696,357697,357698,357699,357700,357701,357704,357705,357706,357720,357732,357733,357758,357759,357767,357776,357777,357778,357779,357780,357781,357802,357803,357804,357805,357806,357807,357808,357809,357812,357813,357814,357842,357898,357900,359685,359686,359691,359693,359694,359699,359717,359718,359746,359747,359750,359751,359762,359763,359764,359765,359766,359776,359779,359781,359806,359808,359809,359810,359812,359813,359814,359864,359865,359867,359868,359869,359870,359871,359884,359885,359887,359889,359899,359901,359903,359904,359905,359906,359908,359909,359915,359917,359920,359934,359936,359943,359976,359993,359998,360012,360017,360018,360019,360075,360088,360090,360116,360120,360121,360122,360123,360124,360125,360126,360127,360128,360129,360130,360131,360141,360152,360190,360208,360217,360224,360225,360274,360279,360285,360286,360295,360296,360309,360327,3360390,360391,360392,360393,360400,360411,360413,360414,360428,360429,360430,360431,360432,360433,360434,360435,360436,360437,360438,360439,360440,360441,360442,360444,360445,360458,360459,360460,360472,360486,360490,360491,360519,360520,360523,360525,360526,360528,360530,360533,360538,360542,360590,360591,360594,360659,360660,360666,360681,360683,360684,360685,360688,360696,360697,360698,360703,360736,360737,360761,360787,360788,360794,360795,360796,360797,360813,360815,360816,360817,360819,360820,360821,360822,360823,360825,360826,360850,360856,360874,360882,360886,360887,360888,360889,360890,360891,360892,360893,360894,360895,360912,360919,360920,360921,360927,360940,360941,360942,360943,360944,360945,360946,360948,360949,360950,360951,360968,360989,360991,360992,361020,361032,361038,361044,361045,361046,361047,361048,361050,361051,361052,361053,361054,361059,361074,361082,361083,361088,361089,361090,361091,361092,361101,361105,361106,361107,361108,361110,361177,361183,361188,361193,361195,361196,361197,361198,361208,361223,361239,361240,361267,361269,361270,361271,361272,361279,361280,361283,361284,361292,361294,361297,361303,361318,361319,361320,361333,361361,361362,361363,361364,361365,361366,361370,361398,361400,361417,361424,361442,361443,361464,361467,361468,361475,361503,361512,361562,361564,361569,361570,361573,361579,361580,361604,361606,361607,361609,361614,361618,361677,361685,361686,361722,361723,361724,361740,361790,361791,361807,361808,361810,361848,361849,361852,361859,361860,361872,361881,361885,361887,361889,361890,361893,361894,361895,361896,361898,361905,361906,361948,361954,361957,361968,361971,361974,361989,361990,361992,361993,361994,362009,362058,362059,362060,362061,362062,362063,362064,362085,362086,362087,362091,362095,362104,362105,362106,362107,362108,362148,362152,362153,362154,362159,362160,362161,362163,362166,362167,362433,362434,362435,362436,362437,362438,362439,362452,362457,362459,362469,362471,362482,362484,362485,362487,362497,362503,362524,362527,362528,362548,362550,362587,362596,362597,362613,362614,362615,362616,362617,362618,362622,362623,362624,362625,362628,362646,362653,362654,362655,362656,362657,362673,362695,362696,362697,362698,362720,362721,362722,362723,362724,362725,362726,362727,362728,362757,362758,362759,362760}; 




TF1* fitvdM(TH1F* P){
  TF1 *F = new TF1(TString("FitVDM")+P->GetName(),"[0]+[1]*exp(-(x-[2])*(x-[2])/(0.5*[3]*[3]))",P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  F->SetLineColor(2);
  F->SetParameter(1,P->GetMaximum());
  F->SetParLimits(1,0.05*P->GetMaximum(),2*P->GetMaximum());
  F->SetParameter(2,( P->GetBinCenter(P->GetNbinsX()) - P->GetBinCenter(1) )/2);
  F->SetParLimits(2,P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  F->SetParameter(3,0.5*P->GetNbinsX());
  F->SetParLimits(3,1,P->GetNbinsX());
  P->Fit(F,"Q","Q",P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  P->SetTitle(TString("")+P->GetTitle()+" ,      B/S = "+((F->GetParameter(0)/F->GetParameter(1))*100)+" %");
  return F;
}


float fitModule(TH1F* P, float weight, int idx=0){
  if(P==NULL) return 0.;
  
  for(int b=1;b<=P->GetNbinsX();b++){
    if(!(P->GetBinContent(b)>0. && P->GetBinError(b)/P->GetBinContent(b)<0.03)){
      P->SetBinContent(b,0);
      P->SetBinError(b,0);
    }
    
  }

  int status=-1; 
  
  TF1 F("FitF","pol0",P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  F.SetParameter(0,weight); 
  F.SetLineColor(2); 


  float diff=0.;
  int c=0;
  TH1F*hD=(TH1F*)P->Clone(TString(P->GetName())+"Diff");
  hD->SetLineColor(2);
  for(int b=1;b<=P->GetNbinsX();b++){
    float d=0.;
    if(P->GetBinContent(b)>0.){
      if(P->GetBinContent(b)>=weight)
	d = (P->GetBinContent(b)-weight);
      else d = (weight - P->GetBinContent(b));
      diff += d/weight;
      c++;
      
      hD->SetBinContent(b,d);
    }
  }  
  diff = diff/c;


  if(idx!=0){
    P->SetTitle(TString("Module ")+MODID[idx]+",     diff="+int(diff*1000)+" ppm");
    P->GetYaxis()->SetTitle("Module Weight");
    P->SetStats(0);
    P->GetYaxis()->SetRangeUser((1-StabilityMax)*weight,(1+StabilityMax)*weight);
    C.Clear();
    P->Draw();
    F.Draw("lsame");
    hD->Draw("histsame");
    C.Print(OutPath+"/Module_RMS_Stability_"+P->GetName()+".png");
  }
  
  return diff;
}
      




void plot_Module_RMS_Stability() {  
  
  readModRPhiZCoordinates();
 
  if(!readModVeto(ModVeto)) return;

//  //check the module list from the 2022 vdm run:
//  cout<<"modules not in run 361913 :"<<endl;
//  for (unsigned int i=0;i<NMOD;i++){
//    if (std::count(MODID_vdm.begin(), MODID_vdm.end(), MODID.at(i))==0)
//      cout<<MODID.at(i)<<endl;
//  }


//  std::vector<int> modlist_missing = {304091152,304099336,304099340,304107552,304123920,304136200,304189444,305147928,305172484,305176592,305283084,305291292,306212876,306245644,306311188,306343948,306360324,352598020,352650244,352674820,352785412,352887812,352961540,353171460,353309700,353313796,353317892,353321988,344200196,344593412};
//  for (unsigned int i=0;i<modlist_missing.size();i++){
//    if(MODVETO[modlist_missing.at(i)]==0)
//      cout<<modlist_missing.at(i)<<endl;
//  }
//  cout<<endl;  
//  return;


  
  int run=0;
  int ls=0;
  int nls_tot=0.;
  int nls_tot_run=0.;
  int ls_idx=0.;
  int m_count[NMOD];

  TH2F TotalPCC("Histo_totpcc","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,5000,0,MAXTOTPCC);
  TH2F TotalPCCPerMod("Histo_totpccavg","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,1000,0,MAXTOTPCCAVG);
  
  TH1F * h_modcount[NMOD];
  TGraph ModWeight;
  TGraph ModWeightRMS;
  TH1D hRMS("RMS/Mean", "",100,0,0.2);

  TH1F h_totcount_vs_LS(TString("h_totcount_vs_LS"),"",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX);
  TH1F *h_modcount_vs_LS[NMOD];
  TH1F hStabilityDeviation("StabilityDeviation", "",200,0,StabilityMax); 
  TGraph gStabilityDeviation;

  TH1F h_totcount_vs_totpcc(TString("h_totcount_vs_totpcc"),"",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
  TH1F *h_modcount_vs_totpcc[NMOD];
  TGraph gLinearityDeviation;
  TH1F hLinearityDeviation("LinearityDeviation", "",200,0,LinearityMax); 

  TH1F ModVdmNoise("ModVdmNoise","Noise / Signal",100,0,0.05);
  
  int ngood_mod=0;
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
      ngood_mod++;
      h_modcount[i]=new TH1F(TString("h_modcount")+i,"",3000,0,MAXMODWEIGHT);
 
      h_modcount_vs_LS[i]=new TH1F(TString("h_modcount_vs_LS")+i,"",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX);
      h_modcount_vs_LS[i]->Sumw2();
      
      h_modcount_vs_totpcc[i]=new TH1F(TString("h_modcount_vs_totpcc")+i,"",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
      h_modcount_vs_totpcc[i]->Sumw2();
    }
  }
    
  cout<<"Number of good modules: "<<ngood_mod<<endl;
  

  TH2D* histo_L1=new TH2D("Histo_Layer1","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,2000,0,0.5);
  TH2D* histo_L2=new TH2D("Histo_Layer2","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,2000,0,0.5);
  TH2D* histo_L3=new TH2D("Histo_Layer3","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,2000,0,0.5);
  TH2D* histo_L4=new TH2D("Histo_Layer4","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,2000,0,0.5);
  TH2D* histo_D1S1=new TH2D("Histo_Disk1S1","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,2000,0,0.5);
  TH2D* histo_D2S1=new TH2D("Histo_Disk2S1","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,2000,0,0.5);
  TH2D* histo_D3S1=new TH2D("Histo_Disk3S1","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,2000,0,0.5);
  TH2D* histo_D1S2=new TH2D("Histo_Disk1S2","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,2000,0,0.5);
  TH2D* histo_D2S2=new TH2D("Histo_Disk2S2","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,2000,0,0.5);
  TH2D* histo_D3S2=new TH2D("Histo_Disk3S2","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,2000,0,0.5);

  TH2D* histoLinearity_L1=new TH2D("HistoLinearity_Layer1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,0.5);
  TH2D* histoLinearity_L2=new TH2D("HistoLinearity_Layer2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,0.5);
  TH2D* histoLinearity_L3=new TH2D("HistoLinearity_Layer3","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,0.5);
  TH2D* histoLinearity_L4=new TH2D("HistoLinearity_Layer4","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,0.5);
  TH2D* histoLinearity_D1S1=new TH2D("HistoLinearity_Disk1S1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,0.5);
  TH2D* histoLinearity_D2S1=new TH2D("HistoLinearity_Disk2S1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,0.5);
  TH2D* histoLinearity_D3S1=new TH2D("HistoLinearity_Disk3S1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,0.5);
  TH2D* histoLinearity_D1S2=new TH2D("HistoLinearity_Disk1S2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,0.5);
  TH2D* histoLinearity_D2S2=new TH2D("HistoLinearity_Disk2S2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,0.5);
  TH2D* histoLinearity_D3S2=new TH2D("HistoLinearity_Disk3S2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,0.5);
  


  cout<<"Reading data from: "<<InputPath<<endl;
  for (unsigned int j=0;j<run_number.size();j++){
     
    TString infile=InputPath+"/"+run_number.at(j)+".csv"; 
    ifstream csvfile (infile.Data());    
    if (!csvfile.is_open()){
      cout << "Unable to open file: "<<infile.Data()<<endl; 
      continue;
    }
    cout<<"Opened: "<<infile.Data()<<endl;
    
    int nls_run=0;    
    std::string line;    
    while (std::getline(csvfile, line)){
      int totcount=0;      
      int count_L1=0;
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
     
      nls_run++;      
      nls_tot++;
      ls_idx = nls_tot_run + ls;

      for (unsigned int i=0;i<NMOD;i++)
	m_count[i]=0;
      
      for (unsigned int i=0;i<NMOD;i++){
	std::getline(iss,token, ',');
	std::stringstream countiss(token);
	countiss>>m_count[i];
	
	if(MODVETO[MODID[i]]==0){                                                                                       
	  totcount += m_count[i];
 
	  if(makePerLayerPlots){
	    if(BPIXorFPIX[MODID[i]]==1){
	      if(LY[MODID[i]]==1)	      count_L1+=m_count[i];
	      if(LY[MODID[i]]==2)	      count_L2+=m_count[i];
	      if(LY[MODID[i]]==3)	      count_L3+=m_count[i];
	      if(LY[MODID[i]]==4)	      count_L4+=m_count[i];
	    }
 	    
	    if(BPIXorFPIX[MODID[i]]==2){
	      if(DISK[MODID[i]]==2)	      count_D1S1+=m_count[i];
	      if(DISK[MODID[i]]==1)	      count_D2S1+=m_count[i];
	      if(DISK[MODID[i]]==0)	      count_D3S1+=m_count[i];
	      if(DISK[MODID[i]]==3)	      count_D1S2+=m_count[i];
	      if(DISK[MODID[i]]==4)	      count_D2S2+=m_count[i];
	      if(DISK[MODID[i]]==5)	      count_D3S2+=m_count[i];	      
	    }  
	  }
 
	}
      }
      
 
      if(LS_ID_MIN < ls_idx && ls_idx < LS_ID_MAX && ngood_mod>0){ //need to cut ls_idx otherwise code crashes if wayt out of bounds
	float totcount_avg=float(totcount)/ngood_mod;

	TotalPCC.Fill(ls_idx,totcount);
	TotalPCCPerMod.Fill(ls_idx,totcount_avg);
	
	if(MINTOTPCCAVG < totcount_avg  && totcount_avg < MAXTOTPCCAVG){

	  h_totcount_vs_LS.Fill(ls_idx, totcount);
	  h_totcount_vs_totpcc.Fill(totcount_avg, totcount);
	  
	  for (unsigned int i=0;i<NMOD;i++){
	    if(MODVETO[MODID[i]]==0){
	      h_modcount[i]->Fill(float(m_count[i])/totcount);
	      h_modcount_vs_LS[i]->Fill(ls_idx, m_count[i]);
	      h_modcount_vs_totpcc[i]->Fill(totcount_avg, m_count[i]);
	    }
	  }

	     
	  if(makePerLayerPlots){
	    histo_L1->Fill(ls_idx, float(count_L1)/totcount);
	    histo_L2->Fill(ls_idx, float(count_L2)/totcount);
	    histo_L3->Fill(ls_idx, float(count_L3)/totcount);
	    histo_L4->Fill(ls_idx, float(count_L4)/totcount);
	    histo_D1S1->Fill(ls_idx, float(count_D1S1)/totcount);
	    histo_D2S1->Fill(ls_idx, float(count_D2S1)/totcount);
	    histo_D3S1->Fill(ls_idx, float(count_D3S1)/totcount);
	    histo_D1S2->Fill(ls_idx, float(count_D1S2)/totcount);
	    histo_D2S2->Fill(ls_idx, float(count_D2S2)/totcount);
	    histo_D3S2->Fill(ls_idx, float(count_D3S2)/totcount);
	     
	    histoLinearity_L1->Fill(totcount_avg, float(count_L1)/totcount);
	    histoLinearity_L2->Fill(totcount_avg, float(count_L2)/totcount);
	    histoLinearity_L3->Fill(totcount_avg, float(count_L3)/totcount);
	    histoLinearity_L4->Fill(totcount_avg, float(count_L4)/totcount);
	    histoLinearity_D1S1->Fill(totcount_avg, float(count_D1S1)/totcount);
	    histoLinearity_D2S1->Fill(totcount_avg, float(count_D2S1)/totcount);
	    histoLinearity_D3S1->Fill(totcount_avg, float(count_D3S1)/totcount);
	    histoLinearity_D1S2->Fill(totcount_avg, float(count_D1S2)/totcount);
	    histoLinearity_D2S2->Fill(totcount_avg, float(count_D2S2)/totcount);
	    histoLinearity_D3S2->Fill(totcount_avg, float(count_D3S2)/totcount);
	  }
  	   
	}
	 
      }
   
    }

    
    csvfile.close();    
    nls_tot_run+=nls_run;
   
    cout<<"# of LS "<<nls_run<<endl;
  }
  cout<<"Total # of LS "<<nls_tot<<endl;
  cout<<"Last LS idx "<<ls_idx<<endl;
  

  cout<<"bad modules (veto.tmp) "<<endl;
  std::ofstream vetostream("veto.tmp",std::ofstream::out);
  

  ///Make a Profile , fit , get chi2
  float max_mod_weight=0;
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
    
      float mean=h_modcount[i]->GetMean();
      float rms=h_modcount[i]->GetRMS();
      
      if(mean > max_mod_weight) max_mod_weight = mean;
      
      if(mean>0){
	ModWeight.SetPoint(ModWeight.GetN(), i, mean);
	ModWeightRMS.SetPoint(ModWeightRMS.GetN(), i, rms/mean);
	hRMS.Fill(rms/mean);
      }      
      if(mean<=0. || rms/mean>RMSThr) vetostream<<MODID[i]<<endl;
      
      
      ///module count graphs
      if(makeModuleGraphs){
	C.Clear();
	h_modcount_vs_LS[i]->GetYaxis()->SetRangeUser(0,h_modcount_vs_LS[i]->GetMaximum()*1.1);
	h_modcount_vs_LS[i]->SetTitle(TString("Module ")+MODID[i]);
	h_modcount_vs_LS[i]->Draw("histp");
	
	if(fitModuleNoise){
	  TF1*F = fitvdM(h_modcount_vs_LS[i]);
	  F->Draw("lsame");
	  float vdmnoise = F->GetParameter(0)/F->GetParameter(1);
	  ModVdmNoise.Fill(vdmnoise);
	if(vdmnoise > VdMNoiseThr) vetostream<<MODID[i]<<endl;
	}
	C.Print(OutPath+"/Module_RMS_Stability_modulecount_"+i+".png");
      }
      
      ////////////////////////
      ///Stability fit
      if(makeModuleFitStability){
	TH1F hwp(TString(h_modcount_vs_LS[i]->GetName())+"_p","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX);
	hwp.GetXaxis()->SetTitle("Lumi section");
	for(int b=1;b<=hwp.GetNbinsX();b++){
	  if(h_totcount_vs_LS.GetBinContent(b)>0){
	    hwp.SetBinContent(b,h_modcount_vs_LS[i]->GetBinContent(b)/h_totcount_vs_LS.GetBinContent(b));
	    hwp.SetBinError(b,sqrt(h_modcount_vs_LS[i]->GetBinContent(b))/h_totcount_vs_LS.GetBinContent(b));
	  }
	}
	float diff_stability=fitModule(&hwp,mean,i);
	gStabilityDeviation.SetPoint(gStabilityDeviation.GetN(),i,diff_stability);
	hStabilityDeviation.Fill(diff_stability);
	if(diff_stability>StabilityThr) vetostream<<MODID[i]<<endl;
      }


      ////////////////////////
      ///Linearity fit
      if(makeModuleFitLinearity){
	TH1F hlp(TString(h_modcount_vs_totpcc[i]->GetName())+"_p","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
	hlp.GetXaxis()->SetTitle("Total PCC / # of good modules");
	for(int b=1;b<=hlp.GetNbinsX();b++){
	  if(h_totcount_vs_totpcc.GetBinContent(b)>0){
	    hlp.SetBinContent(b,h_modcount_vs_totpcc[i]->GetBinContent(b)/h_totcount_vs_totpcc.GetBinContent(b));
	    hlp.SetBinError(b,sqrt(h_modcount_vs_totpcc[i]->GetBinContent(b))/h_totcount_vs_totpcc.GetBinContent(b));
	  }
	}
	float diff_linearity=fitModule(&hlp,mean,i);
	gLinearityDeviation.SetPoint(gLinearityDeviation.GetN(),i,diff_linearity);
	hLinearityDeviation.Fill(diff_linearity);
	if(diff_linearity>LinearityThr) vetostream<<MODID[i]<<endl;
      }	

      
    }
  }
  cout<<"Max module weight: "<<max_mod_weight<<endl;

  vetostream.close();


  
  //Clean up per module plots
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
      delete h_modcount[i];
      delete h_modcount_vs_LS[i];
      delete h_modcount_vs_totpcc[i];
     }
  }
  

  
 /////////////////////////////////////////
 TProfile* TotalPCC_P=TotalPCC.ProfileX();
 TotalPCC_P->SetStats(0);
 TotalPCC_P->GetYaxis()->SetTitle("Total PCC");
 TotalPCC_P->GetXaxis()->SetTitle("Lumi section");
 TotalPCC_P->GetYaxis()->SetRangeUser(0,TotalPCC_P->GetMaximum()*1.2);
 C.Clear();
 TotalPCC_P->Draw("histp");                                                                                                                   
 C.Print(OutPath+"/Module_RMS_Stability_totalcount.png");

 TProfile* TotalPCCPerMod_P=TotalPCCPerMod.ProfileX();
 TotalPCCPerMod_P->SetStats(0);
 TotalPCCPerMod_P->GetYaxis()->SetTitle("Total PCC / # of good modules");
 TotalPCCPerMod_P->GetXaxis()->SetTitle("Lumi section");
 TotalPCCPerMod_P->GetYaxis()->SetRangeUser(0,TotalPCCPerMod_P->GetMaximum()*1.2);
 C.Clear();
 TotalPCCPerMod_P->Draw("histp");                                                                                                                   
 C.Print(OutPath+"/Module_RMS_Stability_totalcountavg.png");

 
 ModWeight.SetMarkerStyle(8);
 ModWeight.SetMarkerSize(0.5);
 ModWeight.SetMarkerColor(4);
 ModWeight.GetXaxis()->SetTitle("Module ID");                                                                                 
 ModWeight.GetYaxis()->SetTitle("Module Weight");
 ModWeight.GetYaxis()->SetRangeUser(0,max_mod_weight);
 C.Clear();
 ModWeight.Draw("AP"); 
 C.Print(OutPath+"/Module_RMS_Stability_weights.png");

 
 ModWeightRMS.SetMarkerStyle(8);
 ModWeightRMS.SetMarkerSize(0.5);
 ModWeightRMS.GetYaxis()->SetTitle("RMS / Mean");
 ModWeightRMS.GetXaxis()->SetTitle("Module ID");
 ModWeightRMS.GetYaxis()->SetRangeUser(0,StabilityMax);    
 C.Clear();
 ModWeightRMS.Draw("AP");
 C.Print(OutPath+"/Module_RMS_Stability_RMS.png");

 
 hRMS.GetXaxis()->SetTitle("RMS / Mean");
 hRMS.GetYaxis()->SetTitle("# of modules");
 C.Clear();
 hRMS.Draw("hist");
 C.Print(OutPath+"/Module_RMS_Stability_RMS_hist.png");

 C.Clear();
 ModVdmNoise.SetMarkerStyle(8);
 ModVdmNoise.SetMarkerSize(0.6);
 ModVdmNoise.Draw("hist");
 C.Print(OutPath+"/Module_RMS_Stability_VdMNoise.png");
 

 ///////////////
 ///Stability Fit results
 if(makeModuleFitStability){
   gStabilityDeviation.SetMarkerStyle(8);
   gStabilityDeviation.SetMarkerSize(0.5);
   gStabilityDeviation.GetYaxis()->SetTitle("Stability Deviation");
   gStabilityDeviation.GetXaxis()->SetTitle("Module ID");
   gStabilityDeviation.GetYaxis()->SetRangeUser(0,StabilityMax);    
   C.Clear();
   gStabilityDeviation.Draw("AP");
   C.Print(OutPath+"/Module_RMS_StabilityDeviation.png");

   hStabilityDeviation.GetXaxis()->SetTitle("Stability Deviation");
   hStabilityDeviation.GetYaxis()->SetTitle("# of modules");
   C.Clear();
   hStabilityDeviation.Draw("hist");
   C.SetLogy(1);
   C.Print(OutPath+"/Module_RMS_StabilityDeviation_hist.png");
   C.SetLogy(0);
 }


  
 ///////////////
 ///Linearity Fit results
 if(makeModuleFitLinearity){
   gLinearityDeviation.SetMarkerStyle(8);
   gLinearityDeviation.SetMarkerSize(0.5);
   gLinearityDeviation.GetYaxis()->SetTitle("Linearity Deviation");
   gLinearityDeviation.GetXaxis()->SetTitle("Module ID");
   gLinearityDeviation.GetYaxis()->SetRangeUser(0,LinearityMax);    
   C.Clear();
   gLinearityDeviation.Draw("AP");
   C.Print(OutPath+"/Module_RMS_LinearityDeviation.png");

   hLinearityDeviation.GetXaxis()->SetTitle("Linearity Deviation");
   hLinearityDeviation.GetYaxis()->SetTitle("# of modules");
   C.Clear();
   hLinearityDeviation.Draw("hist");
   C.SetLogy(1);
   C.Print(OutPath+"/Module_RMS_LinearityDeviation_hist.png");
   C.SetLogy(0);
 }

 
 if(makePerLayerPlots){

   //////////Layer / Disk graphs
   TProfile* P_L1 = histo_L1->ProfileX();
   TProfile* P_L2 = histo_L2->ProfileX();
   TProfile* P_L3 = histo_L3->ProfileX();
   TProfile* P_L4 = histo_L4->ProfileX();
   TProfile* P_D1S1 = histo_D1S1->ProfileX();
   TProfile* P_D2S1 = histo_D2S1->ProfileX();
   TProfile* P_D3S1 = histo_D3S1->ProfileX();
   TProfile* P_D1S2 = histo_D1S2->ProfileX();
   TProfile* P_D2S2 = histo_D2S2->ProfileX();
   TProfile* P_D3S2 = histo_D3S2->ProfileX();
   
   cout<<histo_L1->GetMean(2)<<endl;;
   cout<<histo_L2->GetMean(2)<<endl;;
   cout<<histo_L3->GetMean(2)<<endl;;
   cout<<histo_L4->GetMean(2)<<endl;;
   cout<<histo_D1S1->GetMean(2)<<endl;;
   cout<<histo_D2S1->GetMean(2)<<endl;;
   cout<<histo_D3S1->GetMean(2)<<endl;;
   cout<<histo_D1S2->GetMean(2)<<endl;;
   cout<<histo_D2S2->GetMean(2)<<endl;;
   cout<<histo_D3S2->GetMean(2)<<endl;;

   
   //P_L1->SetMarkerColor(1);
   P_L2->SetMarkerColor(2);
   P_L3->SetMarkerColor(3);
   P_L4->SetMarkerColor(4);    
   P_D1S1->SetMarkerColor(1);
   P_D2S1->SetMarkerColor(6);
   P_D3S1->SetMarkerColor(7);
   P_D1S2->SetMarkerColor(8);
   P_D2S2->SetMarkerColor(9);
   P_D3S2->SetMarkerColor(11);
   
   //P_L1->SetMarkerStyle(8);
   P_L2->SetMarkerStyle(8);
   P_L3->SetMarkerStyle(8);
   P_L4->SetMarkerStyle(8);    
   P_D1S1->SetMarkerStyle(8);
   P_D2S1->SetMarkerStyle(8);
   P_D3S1->SetMarkerStyle(8);
   P_D1S2->SetMarkerStyle(8);
   P_D2S2->SetMarkerStyle(8);
   P_D3S2->SetMarkerStyle(8);

   //P_L1->SetMarkerSize(0.5);
   P_L2->SetMarkerSize(0.5);
   P_L3->SetMarkerSize(0.5);
   P_L4->SetMarkerSize(0.5);    
   P_D1S1->SetMarkerSize(0.5);
   P_D2S1->SetMarkerSize(0.5);
   P_D3S1->SetMarkerSize(0.5);
   P_D1S2->SetMarkerSize(0.5);
   P_D2S2->SetMarkerSize(0.5);
   P_D3S2->SetMarkerSize(0.5);
  
   auto legend = new TLegend(0.905,0.2,1,0.92);
   //legend->AddEntry(P_L1,"B1","p");
   //legend->AddEntry(P_L2,"B2","p");
   legend->AddEntry(P_L3,"B3","p");
   legend->AddEntry(P_L4,"B4","p");
   legend->AddEntry(P_D1S1,"F1S1","p");
   legend->AddEntry(P_D2S1,"F2S1","p");
   legend->AddEntry(P_D3S1,"F3S1","p");
   legend->AddEntry(P_D1S2,"F1S2","p");
   legend->AddEntry(P_D2S2,"F2S2","p");
   legend->AddEntry(P_D3S2,"F3S2","p");
   legend->SetFillColor(0);
   legend->SetLineColor(1);
   

   C.Clear();
   P_L3->SetStats(0);
   P_L3->GetYaxis()->SetRangeUser(0,0.4);
   P_L3->GetYaxis()->SetTitle("Weight");
   P_L3->GetYaxis()->SetNdivisions(8);
   P_L3->GetXaxis()->SetTitle("Lumi section");
   //P_L1->Draw("histp");
   P_L3->Draw("histp");
   //P_L2->Draw("histpsame");
   P_L4->Draw("histpsame");
   P_D1S1->Draw("histpsame");
   P_D2S1->Draw("histpsame");
   P_D3S1->Draw("histpsame");
   P_D1S2->Draw("histpsame");
   P_D2S2->Draw("histpsame");
   P_D3S2->Draw("histpsame");
   legend->Draw();
   C.Print(OutPath+"/Module_RMS_Stability_internal_perlayerdisk_weight.png");    

   
   C.Clear();
   //P_L1->Scale(1./histo_L1->GetMean(2));;
   P_L2->Scale(1./histo_L2->GetMean(2));;
   P_L3->Scale(1./histo_L3->GetMean(2));;
   P_L4->Scale(1./histo_L4->GetMean(2));;
   P_D1S1->Scale(1./histo_D1S1->GetMean(2));;
   P_D2S1->Scale(1./histo_D2S1->GetMean(2));;
   P_D3S1->Scale(1./histo_D3S1->GetMean(2));;
   P_D1S2->Scale(1./histo_D1S2->GetMean(2));;
   P_D2S2->Scale(1./histo_D2S2->GetMean(2));;
   P_D3S2->Scale(1./histo_D3S2->GetMean(2));;

   P_L3->SetStats(0);
   P_L3->GetYaxis()->SetRangeUser(1-StabilityMaxPerLayer,1+StabilityMaxPerLayer);
   P_L3->GetYaxis()->SetTitle("Layer/Disk Normalized Weight");
   P_L3->GetXaxis()->SetTitle("Lumi section");
   //P_L1->Draw("histp");
   P_L3->Draw("histp");
   //P_L2->Draw("histpsame");
   P_L4->Draw("histpsame");
   P_D1S1->Draw("histpsame");
   P_D2S1->Draw("histpsame");
   P_D3S1->Draw("histpsame");
   P_D1S2->Draw("histpsame");
   P_D2S2->Draw("histpsame");
   P_D3S2->Draw("histpsame");
   legend->Draw();
   C.Print(OutPath+"/Module_RMS_Stability_internal_perlayerdisk.png");    
   



   //////////////////////////////////////////////////
   ///Linearity
   TProfile* PLinearityL1 = histoLinearity_L1->ProfileX();
   TProfile* PLinearityL2 = histoLinearity_L2->ProfileX();
   TProfile* PLinearityL3 = histoLinearity_L3->ProfileX();
   TProfile* PLinearityL4 = histoLinearity_L4->ProfileX();
   TProfile* PLinearityD1S1 = histoLinearity_D1S1->ProfileX();
   TProfile* PLinearityD2S1 = histoLinearity_D2S1->ProfileX();
   TProfile* PLinearityD3S1 = histoLinearity_D3S1->ProfileX();
   TProfile* PLinearityD1S2 = histoLinearity_D1S2->ProfileX();
   TProfile* PLinearityD2S2 = histoLinearity_D2S2->ProfileX();
   TProfile* PLinearityD3S2 = histoLinearity_D3S2->ProfileX();

   cout<<histoLinearity_L1->GetMean(2)<<endl;;
   cout<<histoLinearity_L2->GetMean(2)<<endl;;
   cout<<histoLinearity_L3->GetMean(2)<<endl;;
   cout<<histoLinearity_L4->GetMean(2)<<endl;;
   cout<<histoLinearity_D1S1->GetMean(2)<<endl;;
   cout<<histoLinearity_D2S1->GetMean(2)<<endl;;
   cout<<histoLinearity_D3S1->GetMean(2)<<endl;;
   cout<<histoLinearity_D1S2->GetMean(2)<<endl;;
   cout<<histoLinearity_D2S2->GetMean(2)<<endl;;
   cout<<histoLinearity_D3S2->GetMean(2)<<endl;;

   //PLinearityL1->SetMarkerColor(1);
   PLinearityL2->SetMarkerColor(2);
   PLinearityL3->SetMarkerColor(3);
   PLinearityL4->SetMarkerColor(4);    
   PLinearityD1S1->SetMarkerColor(1);
   PLinearityD2S1->SetMarkerColor(6);
   PLinearityD3S1->SetMarkerColor(7);
   PLinearityD1S2->SetMarkerColor(8);
   PLinearityD2S2->SetMarkerColor(9);
   PLinearityD3S2->SetMarkerColor(11);
   
   //PLinearityL1->SetLineColor(1);
   PLinearityL2->SetLineColor(2);
   PLinearityL3->SetLineColor(3);
   PLinearityL4->SetLineColor(4);    
   PLinearityD1S1->SetLineColor(1);
   PLinearityD2S1->SetLineColor(6);
   PLinearityD3S1->SetLineColor(7);
   PLinearityD1S2->SetLineColor(8);
   PLinearityD2S2->SetLineColor(9);
   PLinearityD3S2->SetLineColor(11);
   
   //PLinearityL1->SetMarkerStyle(8);
   PLinearityL2->SetMarkerStyle(8);
   PLinearityL3->SetMarkerStyle(8);
   PLinearityL4->SetMarkerStyle(8);    
   PLinearityD1S1->SetMarkerStyle(8);
   PLinearityD2S1->SetMarkerStyle(8);
   PLinearityD3S1->SetMarkerStyle(8);
   PLinearityD1S2->SetMarkerStyle(8);
   PLinearityD2S2->SetMarkerStyle(8);
   PLinearityD3S2->SetMarkerStyle(8);
   
   //PLinearityL1->SetMarkerSize(0.5);
   PLinearityL2->SetMarkerSize(0.5);
   PLinearityL3->SetMarkerSize(0.5);
   PLinearityL4->SetMarkerSize(0.5);    
   PLinearityD1S1->SetMarkerSize(0.5);
   PLinearityD2S1->SetMarkerSize(0.5);
   PLinearityD3S1->SetMarkerSize(0.5);
   PLinearityD1S2->SetMarkerSize(0.5);
   PLinearityD2S2->SetMarkerSize(0.5);
   PLinearityD3S2->SetMarkerSize(0.5);
   
   auto legendLinearity = new TLegend(0.905,0.2,1,0.92);
   //legendLinearity->AddEntry(PLinearityL1,"B1","p");
   //legendLinearity->AddEntry(PLinearityL2,"B2","p");
   legendLinearity->AddEntry(PLinearityL3,"B3","p");
   legendLinearity->AddEntry(PLinearityL4,"B4","p");
   legendLinearity->AddEntry(PLinearityD1S1,"F1S1","p");
   legendLinearity->AddEntry(PLinearityD2S1,"F2S1","p");
   legendLinearity->AddEntry(PLinearityD3S1,"F3S1","p");
   legendLinearity->AddEntry(PLinearityD1S2,"F1S2","p");
   legendLinearity->AddEntry(PLinearityD2S2,"F2S2","p");
   legendLinearity->AddEntry(PLinearityD3S2,"F3S2","p");
   legendLinearity->SetFillColor(0);
   legendLinearity->SetLineColor(1);
   

   C.Clear();
   PLinearityL3->SetStats(0);
   PLinearityL3->GetYaxis()->SetRangeUser(0,0.4);
   PLinearityL3->GetYaxis()->SetTitle("Layer/Disk  Weight");
   PLinearityL3->GetXaxis()->SetTitle("Total PCC / # of good modules");    
   //PLinearityL1->Draw("histp");
   PLinearityL3->Draw("histp");
   //PLinearityL2->Draw("histpsame");
   PLinearityL4->Draw("histpsame");
   PLinearityD1S1->Draw("histpsame");
   PLinearityD2S1->Draw("histpsame");
   PLinearityD3S1->Draw("histpsame");
   PLinearityD1S2->Draw("histpsame");
   PLinearityD2S2->Draw("histpsame");
   PLinearityD3S2->Draw("histpsame");
   legendLinearity->Draw();
   C.Print(OutPath+"/Module_RMS_Stability_linearity_perlayerdisk_weight.png");    
 
   
   C.Clear();
   //PLinearityL1->Scale(1./histoLinearity_L1->GetMean(2));;
   PLinearityL2->Scale(1./histoLinearity_L2->GetMean(2));;
   PLinearityL3->Scale(1./histoLinearity_L3->GetMean(2));;
   PLinearityL4->Scale(1./histoLinearity_L4->GetMean(2));;
   PLinearityD1S1->Scale(1./histoLinearity_D1S1->GetMean(2));;
   PLinearityD2S1->Scale(1./histoLinearity_D2S1->GetMean(2));;
   PLinearityD3S1->Scale(1./histoLinearity_D3S1->GetMean(2));;
   PLinearityD1S2->Scale(1./histoLinearity_D1S2->GetMean(2));;
   PLinearityD2S2->Scale(1./histoLinearity_D2S2->GetMean(2));;
   PLinearityD3S2->Scale(1./histoLinearity_D3S2->GetMean(2));;
   PLinearityL3->SetStats(0);
   PLinearityL3->GetYaxis()->SetRangeUser(1-LinearityMaxPerLayer,1+LinearityMaxPerLayer);
   PLinearityL3->GetYaxis()->SetTitle("Layer/Disk Normalized Weight");
   PLinearityL3->GetXaxis()->SetTitle("Total PCC / # of good modules");    
   //PLinearityL1->Draw("histp");
   PLinearityL3->Draw("histp");
   //PLinearityL2->Draw("histpsame");
   PLinearityL4->Draw("histpsame");
   PLinearityD1S1->Draw("histpsame");
   PLinearityD2S1->Draw("histpsame");
   PLinearityD3S1->Draw("histpsame");
   PLinearityD1S2->Draw("histpsame");
   PLinearityD2S2->Draw("histpsame");
   PLinearityD3S2->Draw("histpsame");
   legendLinearity->Draw();
   C.Print(OutPath+"/Module_RMS_Stability_linearity_perlayerdisk.png");    
    


 }



  
}

  

