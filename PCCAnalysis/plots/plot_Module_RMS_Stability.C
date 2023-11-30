#include"globals.h" 
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include "modules_361913.h"


TString OutPath = "./tmp";

float weightMin=0.;//1E-5;
float weightMax=0.;

//int SSBIN=575;//SS1 2022
//int SSBIN=2345;//SS2 2022
int SSBIN=160;//SS1 2023
float SSBkgMax=0;

float RMSThr=0.;

float StabilityThr=0.9;
float StabilityMax=0.5;//for plot range

float LinearityThr=0.0;
float LinearityMax=0.02;//for plot 

int selectLayer=0; //keep modules only in this layer (1,2,3,4), reject FPIX and other layers
int selectDisk=0;  //keep modules only in this disk (1,2,3,4,5,6) , reject Barrel and other disks


///////////////////////////////////////
/// Graph parameters (no module selections below)
TCanvas C("Canvas","",800,600);
TLine line;
#define MARKERSIZE 0.3
#define MAXMODWEIGHT 7E-2
#define MAXLAYERWEIGHT 0.7
#define NBINMODWEIGHT 3000
#define NLSBLOCK 100
#define NBINTOTPCCAVG 1000
bool makeModuleGraphs=0;//total counts per module , not the weights
float StabilityMaxPerLayer=0.05;
float LinearityMaxPerLayer=0.03;

TString ModVeto = "BRILAnalysisCode/PCCAnalysis/test/veto_B0.txt";//initial veto



///////////////////////////////////////////////////////
///// 2022 analysis
#define MINTOTPCC 250E6
#define MAXTOTPCC 700E6

#define MINTOTPCCAVG 100E3  //Layer0 veto
#define MAXTOTPCCAVG 300E3

//#define MINTOTPCCAVG 70E3 //Old veto
//#define MAXTOTPCCAVG 250E3  

//#define MINTOTPCCAVG 50E3  //2022F final veto 
//#define MAXTOTPCCAVG 230E3

//#define MINTOTPCCAVG 50E3  //final 2022FCDEG veto
//#define MAXTOTPCCAVG 155E3

//#define MINTOTPCCAVG 0E3    //vdM fill
//#define MAXTOTPCCAVG 130E3

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
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto2022_FStab2p08pLin04p025p_CStab2p1pLin04p03p_EStab06pLin03p_GStab06pLin03p_DStab06pLin03p_vdmStab1p04pNoise05p.txt";


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
#define LS_ID_MIN 0
#define LS_ID_MAX 60000
std::vector<int> run_number = {3360390,360391,360392,360393,360400,360411,360413,360414,360428,360429,360430,360431,360432,360433,360434,360435,360436,360437,360438,360439,360440,360441,360442,360444,360445,360458,360459,360460,360472,360486,360490,360491,360519,360520,360523,360525,360526,360528,360530,360533,360538,360542,360590,360591,360594,360659,360660,360666,360681,360683,360684,360685,360688,360696,360697,360698,360703,360736,360737,360761,360787,360788,360794,360795,360796,360797,360813,360815,360816,360817,360819,360820,360821,360822,360823,360825,360826,360850,360856,360874,360882,360886,360887,360888,360889,360890,360891,360892,360893,360894,360895,360912,360919,360920,360921,360927,360940,360941,360942,360943,360944,360945,360946,360948,360949,360950,360951,360968,360989,360991,360992,361020,361032,361038,361044,361045,361046,361047,361048,361050,361051,361052,361053,361054,361059,361074,361082,361083,361088,361089,361090,361091,361092,361101,361105,361106,361107,361108,361110,361177,361183,361188,361193,361195,361196,361197,361198,361208,361223,361239,361240,361267,361269,361270,361271,361272,361279,361280,361283,361284,361292,361294,361297,361303,361318,361319,361320,361333,361361,361362,361363,361364,361365,361366,361370,361398,361400,361417,361424,361442,361443,361464,361467,361468,361475,361503,361512,361562,361564,361569,361570,361573,361579,361580,361604,361606,361607,361609,361614,361618,361677,361685,361686,361722,361723,361724,361740,361790,361791,361807,361808,361810,361848,361849,361852,361859,361860,361872,361881,361885,361887,361889,361890,361893,361894,361895,361896,361898,361905,361906,361948,361954,361957,361968,361971,361974,361989,361990,361992,361993,361994,362009,362058,362059,362060,362061,362062,362063,362064,362085,362086,362087,362091,362095,362104,362105,362106,362107,362108,362148,362152,362153,362154,362159,362160,362161,362163,362166,362167}; ///collisions2022 from OMS

//std::vector<int> run_number = {360458,360459,360460}; //fill with linearity issue in Antonio's veto


//#define LS_ID_MIN 0
//#define LS_ID_MAX 3500
//std::vector<int> run_number = {361909,361910,361912,361913,361915,361916,361917,361919,361921,361922,361923,361925,361926,361927,361929,361932,361933}; //vdm
///SS1: 571 - 681
///SS2: 2342 - 2354

//#define LS_ID_MIN 140
//#define LS_ID_MAX 180
//std::vector<int> run_number = {361919}; //vdM noise fits

//#define LS_ID_MIN 250
//#define LS_ID_MAX 550
//std::vector<int> run_number = {361913}; //vdM noise fits


//#define LS_ID_MIN 0
//#define LS_ID_MAX 8000
//TString InputPath = "./ModuleVeto2022/data/Run2022G"; //"/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022F";   
//std::vector<int> run_number = {362433,362434,362435,362436,362437,362438,362439,362452,362457,362459,362469,362471,362482,362484,362485,362487,362497,362503,362524,362527,362528,362548,362550,362587,362596,362597,362613,362614,362615,362616,362617,362618,362622,362623,362624,362625,362628,362646,362653,362654,362655,362656,362657,362673,362695,362696,362697,362698,362720,362721,362722,362723,362724,362725,362726,362727,362728,362757,362758,362759,362760}; //oms collisions2022


//////////////////
/// FULL YEAR Plots
//#define LS_ID_MAX 120000
//TString InputPath = "./ModuleVeto2022/data/Run2022"; 
//std::vector<int> run_number = {355862,355863,355865,355869,355870,355871,355872,355890,355891,355892,355912,355913,355921,355922,355927,355933,355941,355942,355976,355988,355989,355990,355991,355992,355993,355994,355995,355996,355997,355998,355999,356001,356002,356003,356004,356005,356010,356011,356030,356031,356032,356033,356041,356042,356043,356044,356045,356046,356047,356048,356071,356073,356074,356075,356076,356077,356091,356101,356135,356152,356158,356161,356162,356163,356164,356165,356169,356170,356174,356175,356177,356223,356241,356248,356286,356290,356309,356316,356321,356322,356323,356363,356365,356371,356375,356376,356377,356378,356381,356382,356383,356384,356385,356386,356409,356413,356426,356427,356428,356431,356432,356433,356434,356435,356436,356445,356446,356465,356466,356467,356468,356469,356470,356471,356472,356473,356474,356475,356476,356478,356479,356480,356481,356488,356489,356516,356522,356523,356524,356525,356526,356527,356528,356529,356530,356531,356563,356568,356569,356570,356576,356577,356578,356579,356580,356581,356582,356613,356614,356615,356616,356617,356618,356619,356633,356653,356655,356671,356684,356685,356690,356692,356709,356719,356720,356721,356722,356727,356729,356780,356788,356789,356810,356811,356812,356813,356814,356815,356824,356825,356831,356849,356902,356906,356908,356918,356919,356936,356937,356943,356944,356945,356946,356947,356948,356949,356950,356951,356952,356953,356954,356955,356956,356968,356969,356970,356977,356980,356997,356998,356999,357000,357001,357005,357059,357070,357076,357077,357078,357079,357080,357081,357096,357098,357100,357101,357102,357103,357104,357105,357106,357110,357112,357150,357212,357268,357269,357270,357271,357301,357302,357318,357328,357329,357330,357331,357332,357333,357339,357360,357391,357401,357406,357438,357440,357441,357442,357447,357472,357478,357479,357482,357538,357539,357540,357542,357550,357610,357611,357612,357613,357688,357696,357697,357698,357699,357700,357701,357704,357705,357706,357720,357732,357733,357758,357759,357767,357776,357777,357778,357779,357780,357781,357802,357803,357804,357805,357806,357807,357808,357809,357812,357813,357814,357842,357898,357900,359685,359686,359691,359693,359694,359699,359717,359718,359746,359747,359750,359751,359762,359763,359764,359765,359766,359776,359779,359781,359806,359808,359809,359810,359812,359813,359814,359864,359865,359867,359868,359869,359870,359871,359884,359885,359887,359889,359899,359901,359903,359904,359905,359906,359908,359909,359915,359917,359920,359934,359936,359943,359976,359993,359998,360012,360017,360018,360019,360075,360088,360090,360116,360120,360121,360122,360123,360124,360125,360126,360127,360128,360129,360130,360131,360141,360152,360190,360208,360217,360224,360225,360274,360279,360285,360286,360295,360296,360309,360327,3360390,360391,360392,360393,360400,360411,360413,360414,360428,360429,360430,360431,360432,360433,360434,360435,360436,360437,360438,360439,360440,360441,360442,360444,360445,360458,360459,360460,360472,360486,360490,360491,360519,360520,360523,360525,360526,360528,360530,360533,360538,360542,360590,360591,360594,360659,360660,360666,360681,360683,360684,360685,360688,360696,360697,360698,360703,360736,360737,360761,360787,360788,360794,360795,360796,360797,360813,360815,360816,360817,360819,360820,360821,360822,360823,360825,360826,360850,360856,360874,360882,360886,360887,360888,360889,360890,360891,360892,360893,360894,360895,360912,360919,360920,360921,360927,360940,360941,360942,360943,360944,360945,360946,360948,360949,360950,360951,360968,360989,360991,360992,361020,361032,361038,361044,361045,361046,361047,361048,361050,361051,361052,361053,361054,361059,361074,361082,361083,361088,361089,361090,361091,361092,361101,361105,361106,361107,361108,361110,361177,361183,361188,361193,361195,361196,361197,361198,361208,361223,361239,361240,361267,361269,361270,361271,361272,361279,361280,361283,361284,361292,361294,361297,361303,361318,361319,361320,361333,361361,361362,361363,361364,361365,361366,361370,361398,361400,361417,361424,361442,361443,361464,361467,361468,361475,361503,361512,361562,361564,361569,361570,361573,361579,361580,361604,361606,361607,361609,361614,361618,361677,361685,361686,361722,361723,361724,361740,361790,361791,361807,361808,361810,361848,361849,361852,361859,361860,361872,361881,361885,361887,361889,361890,361893,361894,361895,361896,361898,361905,361906,361948,361954,361957,361968,361971,361974,361989,361990,361992,361993,361994,362009,362058,362059,362060,362061,362062,362063,362064,362085,362086,362087,362091,362095,362104,362105,362106,362107,362108,362148,362152,362153,362154,362159,362160,362161,362163,362166,362167,362433,362434,362435,362436,362437,362438,362439,362452,362457,362459,362469,362471,362482,362484,362485,362487,362497,362503,362524,362527,362528,362548,362550,362587,362596,362597,362613,362614,362615,362616,362617,362618,362622,362623,362624,362625,362628,362646,362653,362654,362655,362656,362657,362673,362695,362696,362697,362698,362720,362721,362722,362723,362724,362725,362726,362727,362728,362757,362758,362759,362760}; 




/////////////////////////////////////////////////////////////////////
/// 2023 Analysis
//////////////////////////////////////////////////////////////////////
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2023/veto_vdMBkg.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2023/veto_vdMBkgStab04p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2023/veto_vdMBkgStab04p_CStab1p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2023/veto_vdMBkgStab04p_CStab1pLin05p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2023/veto_vdMBkgStab04p_CStab1pLin05pStab04p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2023/veto_vdMBkgStab04p_CStab1pLin05pStab04pLin025p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2023/veto_vdMBkgStab04p_CStab1pLin05pStab04pLin025p_DStab07p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2023/veto_vdMBkgStab04p_CStab1pLin05pStab04pLin025p_DStab07pLin03p.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2023/veto_vdMBkgStab04p_CStab1pLin05pStab04pLin025p_DStab07pLin03p_BL4.txt";///verify that BLayer 4 is ok at beggining of Period D
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2023/veto_vdMBkgStab04p_CStab1pLin05pStab04pLin025p_DStab07pLin03pFPIXveto.txt";

////// vdM  Fill
//#define MINTOTPCC 440
//#define MAXTOTPCC 600
//#define MINTOTPCCAVG 2E-2
//#define MAXTOTPCCAVG 8E-2
//TString InputPath = "./ModuleVeto2023/data/Run2023C_vdM";
//#define LS_ID_MIN 0
//#define LS_ID_MAX 4000
//std::vector<int> run_number = {369802}; 


///Period C, D
// #define MINTOTPCC 8000
// #define MAXTOTPCC 20000
// #define MINTOTPCCAVG 2.0
// #define MAXTOTPCCAVG 8.0
// TString InputPath = "./ModuleVeto2023/data";

//#define LS_ID_MIN 0
//#define LS_ID_MAX 60000
//std::vector<int> run_number = {367095,367100,367102,367103,367104,367111,367112,367127,367128,367129,367130,367131,367132,367133,367134,367228,367229,367230,367231,367232,367234,367260,367261,367262,367263,367264,367267,367268,367269,367270,367271,367312,367314,367315,367334,367336,367337,367355,367385,367406,367413,367415,367416,367475,367476,367477,367515,367516,367552,367553,367554,367589,367591,367592,367593,367615,367616,367617,367618,367619,367622,367661,367662,367663,367664,367665,367666,367691,367692,367693,367694,367695,367696,367697,367699,367730,367742,367758,367770,367771,367772,367790,367823,367831,367836,367838,367840,367881,367882,367883,367884,367902,367904,367905,367906,367907,367908,367910,368151,368223,368224,368229,368247,368318,368320,368321,368326,368331,368332,368333,368334,368336,368337,368338,368339,368340,368341,368343,368382,368383,368384,368388,368389,368400,368406,368410,368411,368412,368423,368437,368440,368443,368451,368452,368453,368454,368489,368542,368546,368547,368548,368566,368567,368609,368611,368612,368613,368636,368669,368670,368671,368672,368673,368674,368675,368676,368678,368684,368685,368723,368724,368746,368748,368749,368752,368753,368762,368763,368764,368765,368774,368815,368816,368817,368818,368822,368823,369279,369436,369444,369447,369794,369795,369797};

///Period D
//#define LS_ID_MIN 0
//#define LS_ID_MAX 35000
//std::vector<int> run_number = {369867,369869,369870,369872,369873,369901,369927,369937,369938,369941,369942,369943,369956,369978,369994,369998,369999,370000,370005,370091,370092,370093,370095,370096,370097,370100,370101,370102,370129,370144,370169,370171,370172,370175,370192,370195,370196,370293,370294,370300,370302,370304,370307,370321,370332,370340,370354,370355,370357,370406,370436,370460,370472,370497,370522,370560,370579,370580,370664,370665,370666,370667,370717,370725,370749,370750,370751,370753,370772,370774,370775,370776,370790,371225};

///FULL Year
 //#define LS_ID_MIN 0
//#define LS_ID_MAX 100000
//std::vector<int> run_number = {367095,367100,367102,367103,367104,367111,367112,367127,367128,367129,367130,367131,367132,367133,367134,367228,367229,367230,367231,367232,367234,367260,367261,367262,367263,367264,367267,367268,367269,367270,367271,367312,367314,367315,367334,367336,367337,367355,367385,367406,367413,367415,367416,367475,367476,367477,367515,367516,367552,367553,367554,367589,367591,367592,367593,367615,367616,367617,367618,367619,367622,367661,367662,367663,367664,367665,367666,367691,367692,367693,367694,367695,367696,367697,367699,367730,367742,367758,367770,367771,367772,367790,367823,367831,367836,367838,367840,367881,367882,367883,367884,367902,367904,367905,367906,367907,367908,367910,368151,368223,368224,368229,368247,368318,368320,368321,368326,368331,368332,368333,368334,368336,368337,368338,368339,368340,368341,368343,368382,368383,368384,368388,368389,368400,368406,368410,368411,368412,368423,368437,368440,368443,368451,368452,368453,368454,368489,368542,368546,368547,368548,368566,368567,368609,368611,368612,368613,368636,368669,368670,368671,368672,368673,368674,368675,368676,368678,368684,368685,368723,368724,368746,368748,368749,368752,368753,368762,368763,368764,368765,368774,368815,368816,368817,368818,368822,368823,369279,369436,369444,369447,369794,369795,369797,369867,369869,369870,369872,369873,369901,369927,369937,369938,369941,369942,369943,369956,369978,369994,369998,369999,370000,370005,370091,370092,370093,370095,370096,370097,370100,370101,370102,370129,370144,370169,370171,370172,370175,370192,370195,370196,370293,370294,370300,370302,370304,370307,370321,370332,370340,370354,370355,370357,370406,370436,370460,370472,370497,370522,370560,370579,370580,370664,370665,370666,370667,370717,370725,370749,370750,370751,370753,370772,370774,370775,370776,370790,371225};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TF1* fitvdM(TH1F* P){
  TF1 *F = new TF1(TString("FitVDM")+P->GetName(),
		   "[0]+[1]*exp(-(x-[2])*(x-[2])/(0.5*[3]*[3]))",
		   P->GetBinCenter(1),
		   P->GetBinCenter(P->GetNbinsX()));
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


float evaluateModuleStability(TH1F* Num, TH1F* Den, float weight, int idx=0, TString xtitle=""){
  if(Num==NULL || Den==NULL){ cout<<"No histogram idx="<<idx<<endl; return 0.;}


  TH1F P(TString(Num->GetName())+"_p","",Num->GetNbinsX(),Num->GetXaxis()->GetXmin(),Num->GetXaxis()->GetXmax());
  for(int b=1;b<=P.GetNbinsX();b++)
    if(Den->GetBinContent(b)>0)
      P.SetBinContent(b,(Num->GetBinContent(b)/Den->GetBinContent(b))/weight);
  
  float diff=0.;
  int c=0;
  for(int b=1;b<=P.GetNbinsX();b++){
    if(P.GetBinContent(b)>0 && P.GetBinCenter(b)< 3000 ){
      if(P.GetBinContent(b)>=1)
	diff += (P.GetBinContent(b)-1);
      else
	diff += (1 - P.GetBinContent(b));
      c++;
    }
  }  
  if(c>0) diff = diff/c;
  else diff=1000;


  if(idx!=0){
    //P.SetTitle(TString("Module ")+idx+" ID:"+MODID[idx]+",    weight="+int(weight*100000)+"E-5"+ ",     stability="+int(diff*10000)+"E-4");
    P.GetYaxis()->SetTitle("Normalized module weight");
    P.GetXaxis()->SetTitle(xtitle.Data());
    P.SetStats(0);
    P.SetMarkerStyle(8);
    P.SetMarkerSize(MARKERSIZE);
    P.GetYaxis()->SetRangeUser((1-StabilityMax),(1+StabilityMax));
    C.Clear();
    P.Draw("histp");
    line.SetLineColor(2);
    line.DrawLine(Num->GetXaxis()->GetXmin(),1,Num->GetXaxis()->GetXmax(),1);
    drawCMSPrelim(0.16,0.85,"#font[62]{CMS} #font[52]{Preliminary}");
    drawFillYear(0,2022);
    drawPCCLuminometer(0.16,0.80,TString("module ")+MODID[idx]);
    C.Print(OutPath+"/Module_RMS_Stability_fit_"+P.GetName()+".png");
  }
  
  return diff;
}
      




void plot_Module_RMS_Stability() {  

  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
    
  readModRPhiZCoordinates();
 
  if(!readModVeto(ModVeto)) return;


  int run=0;
  int ls=0;
  int nls_tot=0.;
  int nls_tot_run=0.;
  int ls_idx=0.;
  float m_count[NMOD];

  TH2F PCC_vs_LS("Histo_totpcc","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,500,0,MAXTOTPCC);
  TH2F PCCPerMod_vs_LS("Histo_totpccavg","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
  
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

  TGraph gModuleNoiseSS;
  float m_modNoiseSS[NMOD];
  
  int ngood_mod=0;
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
      ngood_mod++;
      h_modcount[i]=new TH1F(TString("h_modcount")+i,"",NBINMODWEIGHT,0,MAXMODWEIGHT);
      h_modcount_vs_LS[i]=new TH1F(TString("h_modcount_vs_LS")+i,"",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX);
      h_modcount_vs_LS[i]->Sumw2();
      h_modcount_vs_totpcc[i]=new TH1F(TString("h_modcount_vs_totpcc")+i,"",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
      h_modcount_vs_totpcc[i]->Sumw2();
    }
  }
  cout<<"Number of good modules: "<<ngood_mod<<endl;


  

  TH2D* histo_L1=new TH2D("Histo_Layer1","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_L2=new TH2D("Histo_Layer2","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_L3=new TH2D("Histo_Layer3","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_L4=new TH2D("Histo_Layer4","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_D1S1=new TH2D("Histo_Disk1S1","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_D2S1=new TH2D("Histo_Disk2S1","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_D3S1=new TH2D("Histo_Disk3S1","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_D1S2=new TH2D("Histo_Disk1S2","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_D2S2=new TH2D("Histo_Disk2S2","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_D3S2=new TH2D("Histo_Disk3S2","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);

  TH2D* histoLinearity_L1=new TH2D("HistoLinearity_Layer1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_L2=new TH2D("HistoLinearity_Layer2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_L3=new TH2D("HistoLinearity_Layer3","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_L4=new TH2D("HistoLinearity_Layer4","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_D1S1=new TH2D("HistoLinearity_Disk1S1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_D2S1=new TH2D("HistoLinearity_Disk2S1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_D3S1=new TH2D("HistoLinearity_Disk3S1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_D1S2=new TH2D("HistoLinearity_Disk1S2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_D2S2=new TH2D("HistoLinearity_Disk2S2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_D3S2=new TH2D("HistoLinearity_Disk3S2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  

  //std::list<int> badruns;
  
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
      float totcount=0;
      float totcountgood=0;
      float count_L1=0;
      float count_L2=0;
      float count_L3=0;
      float count_L4=0;
      float count_D1S1=0;
      float count_D2S1=0;
      float count_D3S1=0;
      float count_D1S2=0;
      float count_D2S2=0;
      float count_D3S2=0;      


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

      //if(ls_idx<5000) badruns.push_back(run);

      
      for (unsigned int i=0;i<NMOD;i++)
	m_count[i]=0.;
      
      for (unsigned int i=0;i<NMOD;i++){
	std::getline(iss,token, ',');
	std::stringstream countiss(token);
	countiss>>m_count[i];
	
	totcount += m_count[i];

	if(ls_idx==SSBIN) 
	  m_modNoiseSS[i]=m_count[i];

	
	if(MODVETO[MODID[i]]==0){                                                                                        
	  totcountgood += m_count[i];
	  
	  if(BPIXorFPIX[MODID[i]]==1){
	    if(LY[MODID[i]]==1)	      count_L1+=m_count[i];
	    if(LY[MODID[i]]==2)	      count_L2+=m_count[i];
	    if(LY[MODID[i]]==3)	      count_L3+=m_count[i];
	    if(LY[MODID[i]]==4)	      count_L4+=m_count[i];

	    //if(LY[MODID[i]]==4) totcountgood += m_count[i];
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
      

      float totcountgood_avg=float(totcountgood)/ngood_mod;

      //need to cut ls_idx otherwise code crashes if way out of bounds
      if(LS_ID_MIN < ls_idx && ls_idx < LS_ID_MAX  && ngood_mod>0){
	    
	if(MINTOTPCC < totcount  && totcount < MAXTOTPCC){	  
	  PCC_vs_LS.Fill(ls_idx,totcount);
	  PCCPerMod_vs_LS.Fill(ls_idx,totcountgood_avg);

	  
	  h_totcount_vs_LS.Fill(ls_idx,totcountgood);//total PCC per LS Block interval
	  h_totcount_vs_totpcc.Fill(totcountgood_avg,totcountgood);///total PCC per inst. Lumi interval
	  for (unsigned int i=0;i<NMOD;i++){
	    if(MODVETO[MODID[i]]==0){
	      h_modcount[i]->Fill(m_count[i]/totcountgood);
	      h_modcount_vs_LS[i]->Fill(ls_idx,m_count[i]);
	      h_modcount_vs_totpcc[i]->Fill(totcountgood_avg,m_count[i]);
	    }
	  }

	     
	  if(StabilityMaxPerLayer>0.){
	    histo_L1->Fill(ls_idx, float(count_L1)/totcountgood);
	    histo_L2->Fill(ls_idx, float(count_L2)/totcountgood);
	    histo_L3->Fill(ls_idx, float(count_L3)/totcountgood);
	    histo_L4->Fill(ls_idx, float(count_L4)/totcountgood);
	    histo_D1S1->Fill(ls_idx, float(count_D1S1)/totcountgood);
	    histo_D2S1->Fill(ls_idx, float(count_D2S1)/totcountgood);
	    histo_D3S1->Fill(ls_idx, float(count_D3S1)/totcountgood);
	    histo_D1S2->Fill(ls_idx, float(count_D1S2)/totcountgood);
	    histo_D2S2->Fill(ls_idx, float(count_D2S2)/totcountgood);
	    histo_D3S2->Fill(ls_idx, float(count_D3S2)/totcountgood);
	  }
	    
	  if(LinearityMaxPerLayer>0.){
	    histoLinearity_L1->Fill(totcountgood_avg, float(count_L1)/totcountgood);
	    histoLinearity_L2->Fill(totcountgood_avg, float(count_L2)/totcountgood);
	    histoLinearity_L3->Fill(totcountgood_avg, float(count_L3)/totcountgood);
	    histoLinearity_L4->Fill(totcountgood_avg, float(count_L4)/totcountgood);
	    histoLinearity_D1S1->Fill(totcountgood_avg, float(count_D1S1)/totcountgood);
	    histoLinearity_D2S1->Fill(totcountgood_avg, float(count_D2S1)/totcountgood);
	    histoLinearity_D3S1->Fill(totcountgood_avg, float(count_D3S1)/totcountgood);
	    histoLinearity_D1S2->Fill(totcountgood_avg, float(count_D1S2)/totcountgood);
	    histoLinearity_D2S2->Fill(totcountgood_avg, float(count_D2S2)/totcountgood);
	    histoLinearity_D3S2->Fill(totcountgood_avg, float(count_D3S2)/totcountgood);
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


  /* badruns.unique(); */
  /* for (std::list<int>::iterator it=badruns.begin(); it!=badruns.end(); ++it) */
  /*   std::cout<< *it << '\n'; */


  


  
  /////////////////////////////////////////////////////////////////////////////////////////
  
  cout<<"bad modules (veto.tmp) "<<endl;
  std::ofstream vetostream("veto.tmp",std::ofstream::out);
 

  float max_mod_weight=0;
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
    
      float mean=h_modcount[i]->GetMean();
      float rms=h_modcount[i]->GetRMS();

      ModWeight.SetPoint(ModWeight.GetN(),i, mean);

      //find the largest weight 
      if(mean > max_mod_weight) max_mod_weight = mean;

      ///remove other layers/disks
      if(selectLayer>0 && (BPIXorFPIX[MODID[i]]==2 || LY[MODID[i]]!=selectLayer)) vetostream<<MODID[i]<<endl;

      
      //remove empty modules
      if(weightMin>0. && mean<weightMin) vetostream<<MODID[i]<<endl;
      //Veto modules with high weights
      if(weightMax>0 && mean>weightMax) vetostream<<MODID[i]<<endl;
     
      //Veto modules wight high SS background value in vdM
      gModuleNoiseSS.SetPoint(gModuleNoiseSS.GetN(),i,m_modNoiseSS[i]);
      if(SSBkgMax>0. && m_modNoiseSS[i]>SSBkgMax)  vetostream<<MODID[i]<<endl;
      
      
      ///Fill the weight and RMS graphs
      if(mean>0){
	ModWeightRMS.SetPoint(ModWeightRMS.GetN(), i, rms/mean);
	hRMS.Fill(rms/mean);
 
	///Veto using the RMS/Mean value
	if(RMSThr>0. && rms/mean>RMSThr) vetostream<<MODID[i]<<endl;
      }else {
	ModWeightRMS.SetPoint(ModWeightRMS.GetN(), i, 0);
	hRMS.Fill(0);
      }
      
     
      ///module count vs LS 
      if(makeModuleGraphs){
	C.Clear();
	//h_modcount_vs_LS[i]->SetStats(0);
	h_modcount_vs_LS[i]->GetYaxis()->SetRangeUser(0,1.2*MAXTOTPCCAVG);
	h_modcount_vs_LS[i]->SetTitle(TString("Module ")+MODID[i]);
	h_modcount_vs_LS[i]->Draw("histp");
	C.Print(OutPath+"/Module_RMS_Stability_modulecount_"+i+".png");
      }

    
      ////////////////////////
      /// stability evaluation
      if(StabilityThr>0){
 	float diff_stability = evaluateModuleStability(h_modcount_vs_LS[i],&h_totcount_vs_LS,mean,i,"Lumi Section");
 	gStabilityDeviation.SetPoint(gStabilityDeviation.GetN(),i,diff_stability);
 	hStabilityDeviation.Fill(diff_stability);
 	if(diff_stability>StabilityThr) vetostream<<MODID[i]<<endl;
      }


      ////////////////////////
      ///Linearity evaluation
      if(LinearityThr>0){
	float diff_linearity=evaluateModuleStability(h_modcount_vs_totpcc[i],&h_totcount_vs_totpcc,mean,i,"Avg. PCC per module");
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
//  TProfile* PCC_vs_LS_P=PCC_vs_LS.ProfileX();
//  PCC_vs_LS_P->SetStats(0);
//  PCC_vs_LS_P->GetYaxis()->SetTitle("Total PCC");
//  PCC_vs_LS_P->GetXaxis()->SetTitle("Lumi section");
//  PCC_vs_LS_P->GetYaxis()->SetRangeUser(0,PCC_vs_LS_P->GetMaximum()*1.2);
  C.Clear();
  PCC_vs_LS.GetYaxis()->SetTitle("Total PCC");
  PCC_vs_LS.GetXaxis()->SetTitle("Lumi section");
  PCC_vs_LS.SetStats(0);
  PCC_vs_LS.Draw("scat");
  //PCC_vs_LS_P->Draw("histp");                                                                                                                   
  C.Print(OutPath+"/Module_RMS_Stability_totalcount.png");

 
  /* TProfile* PCCPerMod_vs_LS_P=PCCPerMod_vs_LS.ProfileX(); */
  /* PCCPerMod_vs_LS_P->SetStats(0); */
  /* PCCPerMod_vs_LS_P->GetYaxis()->SetTitle("Total PCC / # of good modules"); */
  /* PCCPerMod_vs_LS_P->GetXaxis()->SetTitle("Lumi section"); */
  /* PCCPerMod_vs_LS_P->GetYaxis()->SetRangeUser(0,PCCPerMod_vs_LS_P->GetMaximum()*1.2); */
  C.Clear();
  //C.SetLogy(1);
  PCCPerMod_vs_LS.GetYaxis()->SetTitle("Avg. PCC per module");
  PCCPerMod_vs_LS.GetXaxis()->SetTitle("Lumi section");
  PCCPerMod_vs_LS.SetStats(0);
  PCCPerMod_vs_LS.Draw("scat");
  //PCCPerMod_vs_LS_P->Draw("histp");                                                                                                                   
  C.Print(OutPath+"/Module_RMS_Stability_totalcountavg.png");
  //C.SetLogy(0);

  
  /////////////////////
  /// Module weights
  ////////////////////
  ModWeight.SetMarkerStyle(8);
  ModWeight.SetMarkerSize(MARKERSIZE);
  ModWeight.SetMarkerColor(4);
  ModWeight.GetXaxis()->SetTitle("Module ID");                                                                                 
  ModWeight.GetYaxis()->SetTitle("Module Weight");
  ModWeight.GetYaxis()->SetRangeUser(1E-5,MAXMODWEIGHT);
  C.Clear();
  //C.SetLogy(1);
  ModWeight.Draw("AP"); 
  C.Print(OutPath+"/Module_RMS_Stability_weights.png");
  //C.SetLogy(0);



  if(SSBkgMax>0.){  
    ////Module noise from SS data in vdM
    gModuleNoiseSS.SetMarkerStyle(8);
    gModuleNoiseSS.SetMarkerSize(MARKERSIZE);
    gModuleNoiseSS.GetYaxis()->SetTitle("SS background");
    gModuleNoiseSS.GetXaxis()->SetTitle("Module ID");
    gModuleNoiseSS.GetYaxis()->SetRangeUser(1E-5,10);     
    C.Clear();
    C.SetLogy(1);
    gModuleNoiseSS.Draw("ap");
    C.Print(OutPath+"/Module_RMS_ModuleNoiseSS.png");
    C.SetLogy(0);
  }


  if(RMSThr>0.){
    ////////////////////
    /// Module RMS 
    ModWeightRMS.SetMarkerStyle(8);
    ModWeightRMS.SetMarkerSize(MARKERSIZE);
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
  }



 
  ///////////////
  ///Stability Fit results
  if(StabilityThr>0){
    gStabilityDeviation.SetMarkerStyle(8);
    gStabilityDeviation.SetMarkerSize(MARKERSIZE);
    gStabilityDeviation.GetYaxis()->SetTitle("Stability Deviation");
    gStabilityDeviation.GetXaxis()->SetTitle("Module ID");
    gStabilityDeviation.GetYaxis()->SetRangeUser(0,StabilityMax);    
    C.Clear();
    gStabilityDeviation.Draw("AP");
    C.Print(OutPath+"/Module_RMS_StabilityDeviation.png");

    gStyle->SetOptStat(1111111);
    hStabilityDeviation.GetXaxis()->SetTitle("Stability Deviation");
    hStabilityDeviation.GetYaxis()->SetTitle("# of modules");
    C.Clear();
    hStabilityDeviation.Draw("hist");
    C.SetLogy(1);
    C.Print(OutPath+"/Module_RMS_StabilityDeviation_hist.png");
    C.SetLogy(0);
    gStyle->SetOptStat(0);
  }


  
  ///////////////
  ///Linearity Fit results
  if(LinearityThr>0){
    gLinearityDeviation.SetMarkerStyle(8);
    gLinearityDeviation.SetMarkerSize(MARKERSIZE);
    gLinearityDeviation.GetYaxis()->SetTitle("Linearity Deviation");
    gLinearityDeviation.GetXaxis()->SetTitle("Module ID");
    gLinearityDeviation.GetYaxis()->SetRangeUser(0,LinearityMax);    
    C.Clear();
    gLinearityDeviation.Draw("AP");
    C.Print(OutPath+"/Module_RMS_LinearityDeviation.png");

    gStyle->SetOptStat(1111111);
    hLinearityDeviation.GetXaxis()->SetTitle("Linearity Deviation");
    hLinearityDeviation.GetYaxis()->SetTitle("# of modules");
    C.Clear();
    hLinearityDeviation.Draw("hist");
    C.SetLogy(1);
    C.Print(OutPath+"/Module_RMS_LinearityDeviation_hist.png");
    C.SetLogy(0);
    gStyle->SetOptStat(0);
  }

 

  if(StabilityMaxPerLayer>0.){
    //////////Layer / Disk graphs
    TProfile* P_L2 = histo_L2->ProfileX();
    TProfile* P_L3 = histo_L3->ProfileX();
    TProfile* P_L4 = histo_L4->ProfileX();
    TProfile* P_D1S1 = histo_D1S1->ProfileX();
    TProfile* P_D2S1 = histo_D2S1->ProfileX();
    TProfile* P_D3S1 = histo_D3S1->ProfileX();
    TProfile* P_D1S2 = histo_D1S2->ProfileX();
    TProfile* P_D2S2 = histo_D2S2->ProfileX();
    TProfile* P_D3S2 = histo_D3S2->ProfileX();
   
    cout<<histo_L2->GetMean(2)<<endl;;
    cout<<histo_L3->GetMean(2)<<endl;;
    cout<<histo_L4->GetMean(2)<<endl;;
    cout<<histo_D1S1->GetMean(2)<<endl;;
    cout<<histo_D2S1->GetMean(2)<<endl;;
    cout<<histo_D3S1->GetMean(2)<<endl;;
    cout<<histo_D1S2->GetMean(2)<<endl;;
    cout<<histo_D2S2->GetMean(2)<<endl;;
    cout<<histo_D3S2->GetMean(2)<<endl;;

    P_L2->SetMarkerColor(2);
    P_L3->SetMarkerColor(3);
    P_L4->SetMarkerColor(4);    
    P_D1S1->SetMarkerColor(1);
    P_D2S1->SetMarkerColor(6);
    P_D3S1->SetMarkerColor(7);
    P_D1S2->SetMarkerColor(8);
    P_D2S2->SetMarkerColor(9);
    P_D3S2->SetMarkerColor(11);
   
    P_L2->SetMarkerStyle(8);
    P_L3->SetMarkerStyle(8);
    P_L4->SetMarkerStyle(8);    
    P_D1S1->SetMarkerStyle(8);
    P_D2S1->SetMarkerStyle(8);
    P_D3S1->SetMarkerStyle(8);
    P_D1S2->SetMarkerStyle(8);
    P_D2S2->SetMarkerStyle(8);
    P_D3S2->SetMarkerStyle(8);

    P_L2->SetMarkerSize(MARKERSIZE);
    P_L3->SetMarkerSize(MARKERSIZE);
    P_L4->SetMarkerSize(MARKERSIZE);    
    P_D1S1->SetMarkerSize(MARKERSIZE);
    P_D2S1->SetMarkerSize(MARKERSIZE);
    P_D3S1->SetMarkerSize(MARKERSIZE);
    P_D1S2->SetMarkerSize(MARKERSIZE);
    P_D2S2->SetMarkerSize(MARKERSIZE);
    P_D3S2->SetMarkerSize(MARKERSIZE);
  
    auto legend = new TLegend(0.905,0.2,1,0.92);
    legend->AddEntry(P_L2,"B2","p");
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
   
    P_L4->SetStats(0);
    P_L4->GetYaxis()->SetRangeUser(0,MAXLAYERWEIGHT);
    P_L4->GetYaxis()->SetTitle("Weight");
    P_L4->GetYaxis()->SetNdivisions(8);
    P_L4->GetXaxis()->SetTitle("Lumi section");
    C.Clear();
    P_L4->Draw("histp");
    P_L2->Draw("histpsame");
    P_L3->Draw("histpsame");
    P_D1S1->Draw("histpsame");
    P_D2S1->Draw("histpsame");
    P_D3S1->Draw("histpsame");
    P_D1S2->Draw("histpsame");
    P_D2S2->Draw("histpsame");
    P_D3S2->Draw("histpsame");
    legend->Draw();
    C.Print(OutPath+"/Module_RMS_Stability_internal_perlayerdisk_weight.png");    


    P_L2->Scale(1./histo_L2->GetMean(2));;
    P_L3->Scale(1./histo_L3->GetMean(2));;
    P_L4->Scale(1./histo_L4->GetMean(2));;
    P_D1S1->Scale(1./histo_D1S1->GetMean(2));;
    P_D2S1->Scale(1./histo_D2S1->GetMean(2));;
    P_D3S1->Scale(1./histo_D3S1->GetMean(2));;
    P_D1S2->Scale(1./histo_D1S2->GetMean(2));;
    P_D2S2->Scale(1./histo_D2S2->GetMean(2));;
    P_D3S2->Scale(1./histo_D3S2->GetMean(2));;
    P_L4->SetStats(0);
    P_L4->GetYaxis()->SetRangeUser(1-StabilityMaxPerLayer,1+StabilityMaxPerLayer);
    P_L4->GetYaxis()->SetTitle("Layer/Disk Normalized Weight");
    P_L4->GetXaxis()->SetTitle("Lumi section");
 
    C.Clear();
    P_L4->Draw("histp");
    P_L2->Draw("histpsame");
    P_L3->Draw("histpsame");
    P_D1S1->Draw("histpsame");
    P_D2S1->Draw("histpsame");
    P_D3S1->Draw("histpsame");
    P_D1S2->Draw("histpsame");
    P_D2S2->Draw("histpsame");
    P_D3S2->Draw("histpsame");
    legend->Draw();
    C.Print(OutPath+"/Module_RMS_Stability_internal_perlayerdisk.png");


    
   
  }


  
  if(LinearityMaxPerLayer>0.){
    //////////////////////////////////////////////////
    ///Linearity
    TProfile* PLinearityL2 = histoLinearity_L2->ProfileX();
    TProfile* PLinearityL3 = histoLinearity_L3->ProfileX();
    TProfile* PLinearityL4 = histoLinearity_L4->ProfileX();
    TProfile* PLinearityD1S1 = histoLinearity_D1S1->ProfileX();
    TProfile* PLinearityD2S1 = histoLinearity_D2S1->ProfileX();
    TProfile* PLinearityD3S1 = histoLinearity_D3S1->ProfileX();
    TProfile* PLinearityD1S2 = histoLinearity_D1S2->ProfileX();
    TProfile* PLinearityD2S2 = histoLinearity_D2S2->ProfileX();
    TProfile* PLinearityD3S2 = histoLinearity_D3S2->ProfileX();

    cout<<histoLinearity_L2->GetMean(2)<<endl;;
    cout<<histoLinearity_L3->GetMean(2)<<endl;;
    cout<<histoLinearity_L4->GetMean(2)<<endl;;
    cout<<histoLinearity_D1S1->GetMean(2)<<endl;;
    cout<<histoLinearity_D2S1->GetMean(2)<<endl;;
    cout<<histoLinearity_D3S1->GetMean(2)<<endl;;
    cout<<histoLinearity_D1S2->GetMean(2)<<endl;;
    cout<<histoLinearity_D2S2->GetMean(2)<<endl;;
    cout<<histoLinearity_D3S2->GetMean(2)<<endl;;

    PLinearityL2->SetMarkerColor(2);
    PLinearityL3->SetMarkerColor(3);
    PLinearityL4->SetMarkerColor(4);    
    PLinearityD1S1->SetMarkerColor(1);
    PLinearityD2S1->SetMarkerColor(6);
    PLinearityD3S1->SetMarkerColor(7);
    PLinearityD1S2->SetMarkerColor(8);
    PLinearityD2S2->SetMarkerColor(9);
    PLinearityD3S2->SetMarkerColor(11);
   
    PLinearityL2->SetLineColor(2);
    PLinearityL3->SetLineColor(3);
    PLinearityL4->SetLineColor(4);    
    PLinearityD1S1->SetLineColor(1);
    PLinearityD2S1->SetLineColor(6);
    PLinearityD3S1->SetLineColor(7);
    PLinearityD1S2->SetLineColor(8);
    PLinearityD2S2->SetLineColor(9);
    PLinearityD3S2->SetLineColor(11);
   
    PLinearityL2->SetMarkerStyle(8);
    PLinearityL3->SetMarkerStyle(8);
    PLinearityL4->SetMarkerStyle(8);    
    PLinearityD1S1->SetMarkerStyle(8);
    PLinearityD2S1->SetMarkerStyle(8);
    PLinearityD3S1->SetMarkerStyle(8);
    PLinearityD1S2->SetMarkerStyle(8);
    PLinearityD2S2->SetMarkerStyle(8);
    PLinearityD3S2->SetMarkerStyle(8);
   
    PLinearityL2->SetMarkerSize(MARKERSIZE);
    PLinearityL3->SetMarkerSize(MARKERSIZE);
    PLinearityL4->SetMarkerSize(MARKERSIZE);    
    PLinearityD1S1->SetMarkerSize(MARKERSIZE);
    PLinearityD2S1->SetMarkerSize(MARKERSIZE);
    PLinearityD3S1->SetMarkerSize(MARKERSIZE);
    PLinearityD1S2->SetMarkerSize(MARKERSIZE);
    PLinearityD2S2->SetMarkerSize(MARKERSIZE);
    PLinearityD3S2->SetMarkerSize(MARKERSIZE);
   
    auto legendLinearity = new TLegend(0.905,0.2,1,0.92);
    legendLinearity->AddEntry(PLinearityL2,"B2","p");
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
    PLinearityL3->GetYaxis()->SetRangeUser(0,MAXLAYERWEIGHT);
    PLinearityL3->GetYaxis()->SetTitle("Layer/Disk  Weight");
    PLinearityL3->GetXaxis()->SetTitle("Average PCC per module");    
    PLinearityL3->Draw("histp");
    PLinearityL2->Draw("histpsame");
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
    PLinearityL3->GetXaxis()->SetTitle("Average PCC per module");    
    PLinearityL3->Draw("histp");
    PLinearityL2->Draw("histpsame");
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

  

