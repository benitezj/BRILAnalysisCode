#include"globals.h" 
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include "modules_361913.h"

//TString OutPath = "./tmp";
//TString OutPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/Reprocess2024/PromptReco/vdM/stability";
//TString OutPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/Reprocess2024/PromptReco/All_Blocks/Layers";
TString OutPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/Reprocess2024/PromptReco/All_Blocks/test/Lumi_days";

////////////////////////////////////////
//Module selections
///////////////////////////////////////
// cut on the weights
float weightMin=0.0;//1E-5;
float weightMax=0.0;//1E-3;

/////for vdM data cut on the noise
//int SSBIN=575; //SS1 2022
//int SSBIN=2345;//SS2 2022
int SSBIN=3259;   //SS1 2023
float SSBkgMax=0;

//cut on the RMS of per LS data
float RMSThr=0;

//cut on stability profile per 100 LS
float StabilityThr=0.0; //.015
float StabilityMax=0.05; //for histogram distribution 0.03

//cut on linearity profile
float LinearityThr_Low=-0.003;
float LinearityThr=0.002; //.012
float LinearityMax=0.01; //for histogram distribution
float LinearityMin=-0.01;
float ForSlopePerModThr=0.02;

int PerModule=0; //<--graficas por modulo
int histomax=20;
int spare_cut=100; 

//select layers/disks
int selectLayer=0; //keep modules only in this layer (1,2,3,4), reject FPIX and other layers
int selectDisk=0;  //keep modules only in this disk (1,2,3,4,5,6) , reject Barrel and other disks

int pcc_total_bool=1;


///////////////////////////////////////
/// Graph parameters (no module selections below)
TCanvas* C=NULL;
TLine line;
#define MARKERSIZE 0.3
#define MAXMODWEIGHT 2E-2
#define MAXLAYERWEIGHT 0.7
#define NBINMODWEIGHT 3000
#define NLSBLOCK 100
#define NBINTOTPCCAVG 500
bool makeModuleGraphs=0;//total counts per module , not the weights
float StabilityMaxPerMod=.05;
float StabilityMaxPerLayer=0.05; //0.05
float LinearityMaxPerLayer=0.02; //.02 //0.03

//TString ModVeto = "../test/veto_B0.txt";//initial veto
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1.txt";
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2.txt";
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0.txt";
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty.txt";
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1.txt";
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2.txt";
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2+Block2_Sty1.txt";
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2+Block2_Sty1_Sty2.txt";
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2+Block2_Sty1_Sty2+Block3_Sty1.txt"; 
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2_+Block2_Sty1_Sty2+Block3_Sty1_Sty2.txt";
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2_+Block2_Sty1_Sty2+Block3_Sty1_Sty2+Block4_Sty1.txt";
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2_+Block2_Sty1_Sty2+Block3_Sty1_Sty2+Block4_Sty1_Sty2.txt";
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2_+Block2_Sty1_Sty2+Block3_Sty1_Sty2+Block4_Sty1_Sty2+Block5_Sty1.txt";
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2_+Block2_Sty1_Sty2+Block3_Sty1_Sty2+Block4_Sty1_Sty2+Block5_Sty1_Sty2.txt"; 
//TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2_+Block2_Sty1_Sty2+Block3_Sty1_Sty2+Block4_Sty1_Sty2+Block5_Sty1_Sty2+Block6_Sty1_Sty2+All_Blocks_Sty_Lty.txt";

TString ModVeto ="tmp/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2_+Block2_Sty1_Sty2+Block3_Sty1_Sty2+Block4_Sty1_Sty2+Block5_Sty1_Sty2+Block6_Sty1_Sty2+All_Blocks_Sty.txt"; 

//////////////////////////////////////////////////////
////// vdM Run 
//#define MINTOTPCC 0
//#define MAXTOTPCC 400
//#define MINTOTPCCAVG 0
//#define MAXTOTPCCAVG .2
//TString InputPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/tuples_to_csv/ZB0";
//#define LS_ID_MIN 140
//#define LS_ID_MAX 320
//std::vector<int> run_number = {380847}; 

//////////////////////////////////////////////////////                                                                       
////// vdM Fill                                                                                                               
//#define MINTOTPCC 230  //230                                                                                         
//#define MAXTOTPCC 400 //400                                                                                                
//#define MINTOTPCCAVG 0                                                                                                      
//#define MAXTOTPCCAVG .2                                                                                                     
//TString InputPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/tuples_to_csv/ZB0";                                                 
//#define LS_ID_MIN 0       //0     fill                                                                                  
//#define LS_ID_MAX 5300    // 5300         
//std::vector<int> run_number = {380844,380845,380846,380847,380848}; 

//Block 1
                                                        
//#define MINTOTPCC 6000 //6000
//#define MAXTOTPCC 20000 //20000                                                                 
//#define MINTOTPCCAVG 2  //2                                                                         
//#define MAXTOTPCCAVG 8  //8                                                                                              
//TString InputPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/AlCaPCCZeroBias-PromptReco/Blocks/Block1";

//#define LS_ID_MIN 0                                                                            
//#define LS_ID_MAX 21000
//std::vector<int> run_number = {378981, 378985, 378993, 378994, 378995, 378996, 378997, 378998, 378999, 379000, 379001, 379002, 379003, 379004, 379005, 379006, 379007, 379008, 379009, 379010, 379011, 379012, 379028, 379029, 379031, 379058, 379059, 379060, 379063, 379075, 379142, 379146, 379154, 379197, 379238, 379243, 379252, 379253, 379315, 379316, 379317, 379318, 379337, 379338, 379339, 379349, 379350, 379351, 379352, 379353, 379354, 379355, 379415, 379416, 379420, 379425, 379433, 379442, 379454, 379456, 379470, 379525, 379530, 379613, 379614, 379615, 379616, 379617, 379618, 379660, 379661, 379729, 379765, 379774};


//Block 2                                                                                                                                           
//#define MINTOTPCC 7000 //7000                                                          
//#define MAXTOTPCC 20000                                                          
//#define MINTOTPCCAVG 2.5                                
//#define MAXTOTPCCAVG 6.5               
//TString InputPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/AlCaPCCZeroBias-PromptReco/Blocks/Block2";                                     
//#define LS_ID_MIN 0                                                                       
//#define LS_ID_MAX 36000
//std::vector<int> run_number = {379831, 379866, 379984, 380001, 380005, 380006, 380007, 380029, 380030, 380031,380032, 380033, 380043, 380049, 380050, 380051, 380052, 380053, 380056, 380066,380074, 380115, 380126, 380127, 380128, 380195, 380196, 380197, 380235, 380236, 380237, 380238, 380306, 380307, 380308, 380309, 380310, 380346, 380348, 380349, 380360, 380377, 380384, 380385, 380399, 380400, 380401, 380403, 380428, 380444, 380446, 380447, 380466, 380470, 380481, 380513, 380516, 380517, 380531, 380532, 380533, 380534, 380535, 380537, 380538, 380564, 380565, 380567, 380623, 380624, 380625, 380626, 380627, 380647, 380648, 380649};  



//Block 3      
                       
//#define MINTOTPCC 7000 //7000                                                                                 
//#define MAXTOTPCC 20000                                                                        
//#define MINTOTPCCAVG 2                                                                 
//#define MAXTOTPCCAVG 7.5
//TString InputPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/AlCaPCCZeroBias-PromptReco/Blocks/Block3";

//#define LS_ID_MIN 0000       //0                                                                  
//#define LS_ID_MAX 35000   //35000                                                                              
//std::vector<int> run_number = {380883};//, 380933, 380945, 380947, 380963, 381022, 381053, 381065, 381067, 381068, 381069, 381070, 381075, 381078, 381079, 381080, 381113, 381114, 381115, 381147, 381148, 381149, 381150, 381151, 381152, 381164, 381166, 381190, 381191, 381199,381212, 381277, 381286, 381289, 381290, 381291, 381292, 381294, 381298, 381309,381358, 381364, 381365, 381371, 381379, 381380, 381384, 381398, 381417, 381443,381477, 381478, 381479, 381480, 381484, 381499, 381500, 381515, 381516, 381543,381544};


//381594, 381053, 381078


//Block 4                                                                                                                                                                                                             
//#define MINTOTPCC 7000 //7000
//#define MAXTOTPCC 20000                               
///#define MINTOTPCCAVG 2                                           
//#define MAXTOTPCCAVG 7.5
//TString InputPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/AlCaPCCZeroBias-PromptReco/Blocks/Block4";          
//#define LS_ID_MIN 0000       //0                       
//#define LS_ID_MAX 35000   //35000                                                                                                                                                                                                              
//std::vector<int> run_number = { 381946, 382114, 382134, 382209, 382213, 382216, 382224, 382225, 382226, 382227,382229, 382250, 382255, 382256, 382257, 382258, 382259, 382260, 382261, 382262,382298, 382299, 382300, 382313, 382314, 382328, 382329, 382343, 382344, 382504,382562, 382568, 382580, 382594, 382595, 382617, 382626, 382649, 382650, 382654,382655, 382656, 382679, 382684, 382685, 382686, 382725, 382729, 382730, 382749,382750, 382751, 382752, 382769, 382770, 382791, 382792, 382794, 382795, 382799,382810, 382811, 382830, 382834, 382856, 382878, 382913, 382921, 382922, 382923,382924, 382937, 382960, 383028, 383034, 383035, 383036};



//Block 5                                                                                                                                                                                             
//#define MINTOTPCC 7000 //7000                                                                                 
//#define MAXTOTPCC 20000                                                                                                        
//#define MINTOTPCCAVG 2                                                                                                           
//#define MAXTOTPCCAVG 7.5                                                                                                         
//TString InputPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/AlCaPCCZeroBias-PromptReco/Blocks/Block5"; 
//#define LS_ID_MIN 0000       //0                                                       
//#define LS_ID_MAX 85000   //35000                                                      
//std::vector<int> run_number = { 383134, 383148, 383153, 383154, 383155, 383162, 383163, 383173, 383174, 383175,383247, 383254, 383255, 383275, 383276, 383277, 383323, 383324, 383325, 383326,383327, 383331, 383332, 383333, 383334, 383363, 383365, 383366, 383367, 383368,383377, 383417, 383418, 383447, 383448, 383449, 383467, 383468, 383485, 383486,383487, 383496, 383512, 383514, 383536, 383537, 383538, 383539, 383540, 383541,383629, 383630, 383631, 383647, 383648, 383649, 383650, 383661, 383662, 383669,383687, 383692, 383693, 383694, 383695, 383712, 383723, 383724, 383740, 383741,383743, 383756, 383757, 383758, 383767, 383779, 383811, 383812, 383813, 383814,383830, 383832, 383833, 383834, 383854, 383855, 383900, 383907, 383908, 383948,383949, 383996, 384014, 384029, 384030, 384031, 384032, 384033, 384034, 384035,384036, 384052, 384069, 384070, 384071, 384113, 384124, 384126, 384127, 384128,384187, 384188, 384202, 384203, 384204, 384209, 384231, 384238, 384239, 384243,384244, 384264, 384265, 384266, 384290, 384291, 384318, 384322, 384323, 384331,384332, 384377, 384378, 384380, 384382, 384383, 384406, 384413, 384445, 384446,384464, 384468, 384485, 384486, 384487, 384488, 384489, 384490, 384491, 384492,384565, 384579, 384591, 384593, 384610, 384614, 384638, 384644};                                                


//Block 6 

//#define MINTOTPCC 7000 //6000
//#define MAXTOTPCC 20000 //20000
//#define MINTOTPCCAVG 2.5  //2   
//#define MAXTOTPCCAVG 4.7  //8                                                                                                  
//TString InputPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/AlCaPCCZeroBias-PromptReco/Blocks/Block6";                         
//#define LS_ID_MIN 0
//#define LS_ID_MAX 71000 

//std::vector<int> run_number = { 384880, 384930, 384933, 384934, 384935, 384950, 384951, 384963, 384981, 385012,385016, 385094, 385100, 385127, 385134, 385142, 385152, 385153, 385168, 385178,385194, 385260, 385281, 385282, 385284, 385285, 385286, 385311, 385312, 385324,385354, 385355, 385383, 385384, 385385, 385386, 385387, 385390, 385391, 385408,385415, 385422, 385423, 385424, 385437, 385443, 385444, 385447, 385474, 385478,385479, 385480, 385481, 385483, 385484, 385511, 385512, 385513, 385514, 385515,385532, 385567, 385568, 385589, 385591, 385592, 385598, 385600, 385602, 385604,385606, 385618, 385619, 385620, 385697, 385712, 385713, 385727, 385728, 385738,385739, 385754, 385764, 385799, 385801, 385836, 385837, 385838, 385839, 385840,385841, 385842, 385863, 385882, 385883, 385887, 385888, 385889, 385908, 385915,385933, 385934, 385986, 386006, 386008, 386010, 386025, 386047, 386048, 386049,386050, 386071, 386119, 386211, 386279, 386285, 386308, 386313, 386319, 386446,386478, 386505, 386508, 386509, 386553, 386554}; 


//all Blocks

#define MINTOTPCC 7000 //6000                                                             
#define MAXTOTPCC 20000 //20000                                                           
#define MINTOTPCCAVG 2//2.5                                                                 
#define MAXTOTPCCAVG 4.8//4.7  
TString InputPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/AlCaPCCZeroBias-PromptReco/Blocks/All_Blocks";                

#define LS_ID_MIN 0        //
#define LS_ID_MAX 3400    //281000 for complete year

std::vector<int> run_number = {385281,385282,385284,385285,385286};  //fill for limi days


//all runs all year

///std::vector<int> run_number = {378981, 378985, 378993, 378994, 378995, 378996, 378997, 378998, 378999, 379000, 379001, 379002, 379003, 379004, 379005, 379006, 379007, 379008, 379009, 379010, 379011, 379012, 379028, 379029, 379031, 379058, 379059, 379060, 379063, 379075, 379142, 379146, 379154, 379197, 379238, 379243, 379252, 379253, 379315, 379316, 379317, 379318, 379337, 379338, 379339, 379349, 379350, 379351, 379352, 379353, 379354, 379355, 379415, 379416, 379420, 379425, 379433, 379442, 379454, 379456, 379470, 379525, 379530, 379613, 379614, 379615, 379616, 379617, 379618, 379660, 379661, 379729, 379765, 379774, 379831, 379866, 379984, 380001, 380005, 380006, 380007, 380029, 380030, 380031, 380032, 380033, 380043, 380049, 380050, 380051, 380052, 380053, 380056, 380066, 380074, 380115, 380126, 380127, 380128, 380195, 380196, 380197, 380235, 380236, 380237, 380238, 380306, 380307, 380308, 380309, 380310, 380346, 380348, 380349, 380360, 380377, 380384, 380385, 380399, 380400, 380401, 380403, 380428, 380444, 380446, 380447, 380466, 380470, 380481, 380513, 380516, 380517, 380531, 380532, 380533, 380534, 380535, 380537, 380538, 380564, 380565, 380567, 380623, 380624, 380625, 380626, 380627, 380647, 380648, 380649, 380883, 380933, 380945, 380947, 380963, 381022, 381053, 381065, 381067, 381068, 381069, 381070, 381075, 381078, 381079, 381080, 381113, 381114, 381115, 381147, 381148, 381149, 381150, 381151, 381152, 381164, 381166, 381190, 381191, 381199, 381212, 381277, 381286, 381289, 381290, 381291, 381292, 381294, 381298, 381309, 381358, 381364, 381365, 381371, 381379, 381380, 381384, 381398, 381417, 381443, 381477, 381478, 381479, 381480, 381484, 381499, 381500, 381515, 381516, 381543, 381544, 381946, 382114, 382134, 382209, 382213, 382216, 382224, 382225, 382226, 382227, 382229, 382250, 382255, 382256, 382257, 382258, 382259, 382260, 382261, 382262, 382298, 382299, 382300, 382313, 382314, 382328, 382329, 382343, 382344, 382504, 382562, 382568, 382580, 382594, 382595, 382617, 382626, 382649, 382650, 382654, 382655, 382656, 382679, 382684, 382685, 382686, 382725, 382729, 382730, 382749, 382750, 382751, 382752, 382769, 382770, 382791, 382792, 382794, 382795, 382799, 382810, 382811, 382830, 382834, 382856, 382878, 382913, 382921, 382922, 382923, 382924, 382937, 382960, 383028, 383034, 383035, 383036, 383134, 383148, 383153, 383154, 383155, 383162, 383163, 383173, 383174, 383175, 383247, 383254, 383255, 383275, 383276, 383277, 383323, 383324, 383325, 383326, 383327, 383331, 383332, 383333, 383334, 383363, 383365, 383366, 383367, 383368, 383377, 383417, 383418, 383447, 383448, 383449, 383467, 383468, 383485, 383486, 383487, 383496, 383512, 383514, 383536, 383537, 383538, 383539, 383540, 383541, 383629, 383630, 383631, 383647, 383648, 383649, 383650, 383661, 383662, 383669, 383687, 383692, 383693, 383694, 383695, 383712, 383723, 383724, 383740, 383741, 383743, 383756, 383757, 383758, 383767, 383779, 383811, 383812, 383813, 383814, 383830, 383832, 383833, 383834, 383854, 383855, 383900, 383907, 383908, 383948, 383949, 383996, 384014, 384029, 384030, 384031, 384032, 384033, 384034, 384035, 384036, 384052, 384069, 384070, 384071, 384113, 384124, 384126, 384127, 384128, 384187, 384188, 384202, 384203, 384204, 384209, 384231, 384238, 384239, 384243, 384244, 384264, 384265, 384266, 384290, 384291, 384318, 384322, 384323, 384331, 384332, 384377, 384378, 384380, 384382, 384383, 384406, 384413, 384445, 384446, 384464, 384468, 384485, 384486, 384487, 384488, 384489, 384490, 384491, 384492, 384565, 384579, 384591, 384593, 384610, 384614, 384638, 384644, 384880, 384930, 384933, 384934, 384935, 384950, 384951, 384963, 384981, 385012, 385016, 385094, 385100, 385127, 385134, 385142, 385152, 385153, 385168, 385178, 385194, 385260, 385281, 385282, 385284, 385285, 385286, 385311, 385312, 385324, 385354, 385355, 385383, 385384, 385385, 385386, 385387, 385390, 385391, 385408, 385415, 385422, 385423, 385424, 385437, 385443, 385444, 385447, 385474, 385478, 385479, 385480, 385481, 385483, 385484, 385511, 385512, 385513, 385514, 385515, 385532, 385567, 385568, 385589, 385591, 385592, 385598, 385600, 385602, 385604,385606, 385618, 385619, 385620, 385697, 385712, 385713, 385727, 385728, 385738,385739, 385754, 385764, 385799, 385801, 385836, 385837, 385838, 385839, 385840,385841, 385842, 385863, 385882, 385883, 385887, 385888, 385889, 385908, 385915,385933, 385934, 385986, 386006, 386008, 386010, 386025, 386047, 386048, 386049,386050, 386071, 386119, 386211, 386279, 386285, 386308, 386313, 386319, 386446,386478, 386505, 386508, 386509, 386553, 386554};  


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

  TFile* file = new TFile("/eos/user/l/lcuevasp/BRIL/PCC_Run3/Reprocess2024/PromptReco/All_Blocks/test/Lumi_days/PerModuleStability_veto_all_Blocks_Lty.root", "UPDATE");

  TH1F P(TString(Num->GetName())+"_p","",Num->GetNbinsX(),Num->GetXaxis()->GetXmin(),Num->GetXaxis()->GetXmax());
  //cout<<"P.GetNbinsX() : "<<P.GetNbinsX()<<endl;
  for(int b=1;b<=P.GetNbinsX();b++){
    if(Den->GetBinContent(b)>0)
      P.SetBinContent(b,(Num->GetBinContent(b)/Den->GetBinContent(b))/weight);}
  
  /////////////////////////////////////////////////////////////////////////////////

  TH1F Filter("Filter", "", P.GetNbinsX(), P.GetXaxis()->GetXmin(), P.GetXaxis()->GetXmax());     
  //cout << "max: " << P.GetXaxis()->GetXmax() << endl;                              
  for (int g = 1; g <= P.GetNbinsX(); g++) {                                         
    // cout <<g<< "  " << binContent << endl;                                        
    double binContent = P.GetBinContent(g);                                          
    if (binContent >= 1-ForSlopePerModThr && binContent <= 1+ForSlopePerModThr) {                          
        Filter.SetBinContent(g, binContent);                                         
        //      cout <<g<< "  " << binContent << endl;                               
        // Filter.SetBinError(b, P.GetBinError(b));                                  
    }                                                                                
  }                                                                                  
  TF1 *linearFit = new TF1("linearFit", "pol1", Filter.GetXaxis()->GetXmin(), Filter.GetXaxis()->GetXmax());        
  Filter.Fit(linearFit, "R");                                                        
  // P.Fit(linearFit, "R");                                                          
  linearFit->SetLineColor(2);                                                        
  double slope = linearFit->GetParameter(1);                                         
  double intercept = linearFit->GetParameter(0);                                     
  cout << "slope: " << slope << endl;                                                
  cout << "Intercept: " << intercept << endl;                                                                                                                                                                      
  ///////////////////////////////////////////////////     
  
  float diff=0.;
  int c=0;
  for(int b=1;b<=P.GetNbinsX();b++){
    if(P.GetBinContent(b)>0){ // && P.GetBinCenter(b)< 3000 ){
      if(P.GetBinContent(b)>=1)
	diff += (P.GetBinContent(b)-1);
      else
	diff += (1 - P.GetBinContent(b));
      c++;
    }
  }
  //cout<<"counter : "<<c<<endl;
  if(c>0) diff = diff/c;
  else diff=1000;

			   //if(slope>0){               
			   //slope = slope;     
			   //}else{            
			   //slope = slope*-1;       
			   //}   
			   

  if(idx!=0){
    // P.SetTitle(TString("Module ")+idx+" ID:"+MODID[idx]+",    weight="+int(weight*100000)+"E-5"+ ",     stability="+int(diff*10000)+"E-4");
    P.GetYaxis()->SetTitle("Normalized module weight");
    P.GetXaxis()->SetTitle(xtitle.Data());
    P.SetStats(0);
    P.SetMarkerStyle(8);
    P.SetMarkerSize(MARKERSIZE);
    P.GetYaxis()->SetRangeUser((1-StabilityMaxPerMod),(1+StabilityMaxPerMod));
    //P.GetYaxis()->SetRangeUser((1-0.5),(1+0.5));
    TString title = TString("Module ") + MODID[idx] + TString(" Weight=") +int(weight * 100000) + TString("E-5 Linearity=") + int(slope * 100000) + TString("E-5");                                                                                                                                                                                              
    TString title_Lty = TString("Module ") + MODID[idx] + TString("slope=") + int(slope * 100000) + TString("E-5") + TString("Intercept=") + int(intercept);
    TString histoname = Form("Filter_%d", MODID[idx]);
    
    Filter.GetYaxis()->SetTitle("Normalized module weight");                       
    Filter.GetXaxis()->SetTitle(xtitle.Data());                                    
    Filter.SetStats(0);                                                            
    Filter.SetMarkerStyle(8);                                                      
    Filter.SetMarkerSize(MARKERSIZE);                                              
    Filter.GetYaxis()->SetRangeUser((1-StabilityMaxPerMod),(1+StabilityMaxPerMod));

    Filter.SetTitle(title);
    Filter.SetName(histoname);
    Filter.Write();
    file->Close();  
    
    C->Clear();
    if(StabilityThr>0){ P.Draw("histp");}
    
    if(LinearityThr>0){
      Filter.Draw("histp");
      linearFit->Draw("same");
      
    }

    
    line.SetLineColor(2);
    line.DrawLine(Num->GetXaxis()->GetXmin(),1,Num->GetXaxis()->GetXmax(),1);
    drawFillYear(0,2024);
    drawPCCLuminometer(0.18,0.85,TString("module ")+MODID[idx]);
    drawPCCText(0.18,0.80,TString("weight=")+int(weight*100000)+TString("E-5"));
    if(StabilityThr>0) drawPCCText(0.18,0.75,TString("Stability=")+int(diff*10000)+TString("E-4"));
    if(LinearityThr>0)drawPCCText(0.18,0.75,TString("Linearity=")+int(slope*100000)+TString("E-5")); 
    // drawPCCText(0.18,0.75,TString("Linearity=")+int(diff*10000)+TString("E-4"));
    if(PerModule==true)

      // TString title = TString("Module ") + MODID[idx] + TString(" Weight=") + 
      //        int(weight * 100000) + TString("E-5 Stability=") + 
      //        int(diff * 10000) + TString("E-4");

      //    P.SetTitle(title);
      // P.Write();
      // filePerModule->Close();  
    C->Print(OutPath+"/PerModule/Module_RMS_Stability_fit_"+P.GetName()+".png");
  }

  return slope;
  // return diff;
}
      




void plot_Module_RMS_Stability_Lty() {  

   TFile* file = new TFile("/eos/user/l/lcuevasp/BRIL/PCC_Run3/Reprocess2024/PromptReco/All_Blocks/test/Lumi_days/PerModuleStability_veto_all_Blocks_Lty.root", "RECREATE");
   
    file->Close();
    //  delete file;  
  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  C=new TCanvas("Canvas","",50,50,800,600);
  
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

  TGraph gLinearityDeviation_slope;                                                                                           
  TH1F hLinearityDeviation_slope("LinearityDeviation_slope", "",200,LinearityMin,LinearityMax);
  
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
  unsigned int ls_count = 0;
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
	//	cout<<i<<"total "<<m_count[i]<<" "<<endl;
	totcount += m_count[i];

	if(ls_idx==SSBIN) 
	  m_modNoiseSS[i]=m_count[i];
	//if(ls_idx==SSBIN)
	// cout<<i<<" "<<m_modNoiseSS[i]<<endl;
	
	if(MODVETO[MODID[i]]==0){
	  // cout<<i<<" MODVETO[MODID[i]] "<<MODID[i]<<endl; 
	  totcountgood += m_count[i];
         
	  if(BPIXorFPIX[MODID[i]]==1){
	    // cout<<",,,"<<m_count[i]<<endl;
	    if(LY[MODID[i]]==1)	      count_L1+=m_count[i];
	    if(LY[MODID[i]]==2)	      count_L2+=m_count[i];
	    if(LY[MODID[i]]==3)	      count_L3+=m_count[i];
	    if(LY[MODID[i]]==4)	      count_L4+=m_count[i];

	    //if(LY[MODID[i]]==4) totcountgood += m_count[i];
	  }
	  // cout<<"LS "<<ls_idx<<endl;  
	  if(BPIXorFPIX[MODID[i]]==2){
	    if(DISK[MODID[i]]==2){	      count_D1S1+=m_count[i];
	      //  cout<<"D1S1 "<<MODID[i]<<endl;
	    }
	    if(DISK[MODID[i]]==1){	      count_D2S1+=m_count[i];
	      // cout<<"D2S1 "<<MODID[i]<<endl;
	    }
	    if(DISK[MODID[i]]==0){	      count_D3S1+=m_count[i];
	      // cout<<"D3S1 "<<MODID[i]<<endl;
	    }
	    if(DISK[MODID[i]]==3){	      count_D1S2+=m_count[i];
	      // cout<<"D1S2 "<<MODID[i]<<endl;
	    }
	    if(DISK[MODID[i]]==4){	      count_D2S2+=m_count[i];
	      //cout<<"D2S2 "<<MODID[i]<<endl;
	    }
	    if(DISK[MODID[i]]==5){	      count_D3S2+=m_count[i];
	      // cout<<"D3S2 "<<MODID[i]<<endl;
	    }	      
	  }  
 
	}
      }
      

      float totcountgood_avg=float(totcountgood)/ngood_mod;
     
      //need to cut ls_idx otherwise code crashes if way out of bounds
      if(LS_ID_MIN < ls_idx && ls_idx < LS_ID_MAX  && ngood_mod>0){
        unsigned int ls_count = 0;/// 	    
	if(MINTOTPCC < totcount  && totcount < MAXTOTPCC){
	  ls_count++; //
	  PCC_vs_LS.Fill(ls_idx,totcount);
	  PCCPerMod_vs_LS.Fill(ls_idx,totcountgood_avg);

	  
	  h_totcount_vs_LS.Fill(ls_idx,totcountgood);//total PCC per LS Block interval
	  //<<"#  LS,PCC "<<ls_idx<<" "<<totcountgood<<endl;
	  h_totcount_vs_totpcc.Fill(totcountgood_avg,totcountgood);///total PCC per inst. Lumi interval
	  for (unsigned int i=0;i<NMOD;i++){
	    if(MODVETO[MODID[i]]==0){
	      //m_count[i]/totcountgood
	      h_modcount[i]->Fill(m_count[i]/totcountgood);
	      h_modcount_vs_LS[i]->Fill(ls_idx,m_count[i]);
	      // cout<<i<<" "<<totcountgood_avg<<" "<<m_count[i]<<endl;
	      h_modcount_vs_totpcc[i]->Fill(totcountgood_avg,m_count[i]);
	    }
	  }

	     
	  if(StabilityMaxPerLayer>0.){
	    histo_L1->Fill(ls_idx, float(count_L1)/totcountgood);
	    histo_L2->Fill(ls_idx, float(count_L2)/totcountgood);
	    // cout<<ls_idx<<" "<<count_L1<<count_L3<<count_L4<<count_L1<<endl;
	    //cout<<ls_idx<<" "<<count_D1S1<<count_D2S1<<count_D3S1<<count_D1S2<<count_D2S2<<count_D3S2<<endl;
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
  std::cout << "Total de veces que se ejecuto el bloque para ls_idx: " << ls_count << std::endl;

  /* badruns.unique(); */
  /* for (std::list<int>::iterator it=badruns.begin(); it!=badruns.end(); ++it) */
  /*   std::cout<< *it << '\n'; */


  


  
  /////////////////////////////////////////////////////////////////////////////////////////
  
  cout<<"bad modules (veto.tmp) "<<endl;
  std::ofstream vetostream("veto.tmp",std::ofstream::out);
 
  float max_mod_weight=0;
  // for (unsigned int i=0;i<10;i++){ 
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
      if(weightMin>0. && mean<weightMin) cout<<"min"<<MODID[i]<<endl;
      //Veto modules with high weights
      if(weightMax>0 && mean>weightMax) vetostream<<MODID[i]<<endl;
      if(weightMax>0 && mean>weightMax) cout<<"max"<<MODID[i]<<endl;
      //if(weightMax>0 && mean==0) cout<<"min"<<MODID[i]<<endl;
      
      //Veto modules wight high SS background value in vdM
      gModuleNoiseSS.SetPoint(gModuleNoiseSS.GetN(),i,m_modNoiseSS[i]);
      if(SSBkgMax>0. && m_modNoiseSS[i]>SSBkgMax)  vetostream<<MODID[i]<<endl;
      // if(SSBkgMax>0. && m_modNoiseSS[i]==0) cout<<MODID[i]<<endl;
      
      
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
	C->Clear();
	//h_modcount_vs_LS[i]->SetStats(0);
	h_modcount_vs_LS[i]->GetYaxis()->SetRangeUser(0,1.2*MAXTOTPCCAVG);
	h_modcount_vs_LS[i]->SetTitle(TString("Module ")+MODID[i]);
	h_modcount_vs_LS[i]->Draw("histp");
	C->Print(OutPath+"/ModuleGraph/Module_RMS_Stability_modulecount_"+i+".png");
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
	float diff_linearity=evaluateModuleStability(h_modcount_vs_totpcc[i],&h_totcount_vs_totpcc,mean,i,"Avg. PCC per module mean");
	//	gLinearityDeviation.SetPoint(gLinearityDeviation.GetN(),i,diff_linearity);
	//	hLinearityDeviation.Fill(diff_linearity);

        gLinearityDeviation_slope.SetPoint(gLinearityDeviation_slope.GetN(),i,diff_linearity);      
        hLinearityDeviation_slope.Fill(diff_linearity);  
	
	//	if(diff_linearity>LinearityThr) vetostream<<MODID[i]<<endl;
	if(diff_linearity>LinearityThr || diff_linearity<LinearityThr_Low) vetostream<<MODID[i]<<endl; 
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
  if(pcc_total_bool>0){ 
  TFile* file = new TFile("/eos/user/l/lcuevasp/BRIL/PCC_Run3/Reprocess2024/PromptReco/All_Blocks/test/Lumi_days/PerModuleStability_veto_all_Blocks_Lty.root", "UPDATE");
 
  C->Clear();
  PCC_vs_LS.GetYaxis()->SetTitle("Total PCC");
  PCC_vs_LS.GetXaxis()->SetTitle("Lumi section");
  PCC_vs_LS.SetStats(0);
  PCC_vs_LS.Write();
  PCC_vs_LS.Draw("scat");
  //PCC_vs_LS_P->Draw("histp");                                                                                                                   
  C->Print(OutPath+"/Module_RMS_Stability_totalcount.png");

 
  /* TProfile* PCCPerMod_vs_LS_P=PCCPerMod_vs_LS.ProfileX(); */
  /* PCCPerMod_vs_LS_P->SetStats(0); */
  /* PCCPerMod_vs_LS_P->GetYaxis()->SetTitle("Total PCC / # of good modules"); */
  /* PCCPerMod_vs_LS_P->GetXaxis()->SetTitle("Lumi section"); */
  /* PCCPerMod_vs_LS_P->GetYaxis()->SetRangeUser(0,PCCPerMod_vs_LS_P->GetMaximum()*1.2); */
  C->Clear();
  //C->SetLogy(1);
  PCCPerMod_vs_LS.GetYaxis()->SetTitle("Avg. PCC per module");
  PCCPerMod_vs_LS.GetXaxis()->SetTitle("Lumi section");
  PCCPerMod_vs_LS.SetStats(0);
  PCCPerMod_vs_LS.Write();////////////////////////////////////////////////////////////////////////
  PCCPerMod_vs_LS.Draw("scat");
  //PCCPerMod_vs_LS_P->Draw("histp");                                                                                                                   
  C->Print(OutPath+"/Module_RMS_Stability_totalcountavg.png");
  //C->SetLogy(0);
  file->Close();
  }
  /////////////////////
  /// Module weights
  ////////////////////
  ModWeight.SetMarkerStyle(8);
  ModWeight.SetMarkerSize(MARKERSIZE);
  ModWeight.SetMarkerColor(4);
  ModWeight.GetXaxis()->SetTitle("Module ID");                                                                                 
  ModWeight.GetYaxis()->SetTitle("Module Weight");
  ModWeight.GetYaxis()->SetRangeUser(-.0001,.01);//MAXMODWEIGHT
  C->Clear();
  //C->SetLogy(1);
  ModWeight.Draw("AP"); 
  C->Print(OutPath+"/Module_RMS_Stability_weights.png");
  //C->SetLogy(0);



  if(SSBkgMax>0.){  
    ////Module noise from SS data in vdM
    gModuleNoiseSS.SetMarkerStyle(8);
    gModuleNoiseSS.SetMarkerSize(MARKERSIZE);
    gModuleNoiseSS.GetYaxis()->SetTitle("SS background");
    gModuleNoiseSS.GetXaxis()->SetTitle("Module ID");
    gModuleNoiseSS.GetYaxis()->SetRangeUser(1E-5,10);     
    C->Clear();
    C->SetLogy(1);
    gModuleNoiseSS.Draw("ap");
    C->Print(OutPath+"/Module_RMS_ModuleNoiseSS.png");
    C->SetLogy(0);
  }


  if(RMSThr>0.){
    ////////////////////
    /// Module RMS 
    ModWeightRMS.SetMarkerStyle(8);
    ModWeightRMS.SetMarkerSize(MARKERSIZE);
    ModWeightRMS.GetYaxis()->SetTitle("RMS / Mean");
    ModWeightRMS.GetXaxis()->SetTitle("Module ID");
    ModWeightRMS.GetYaxis()->SetRangeUser(0,StabilityMax);    
    C->Clear();
    ModWeightRMS.Draw("AP");
    C->Print(OutPath+"/Module_RMS_Stability_RMS.png");
    
    hRMS.GetXaxis()->SetTitle("RMS / Mean");
    hRMS.GetYaxis()->SetTitle("# of modules");
    C->Clear();
    hRMS.Draw("hist");
    C->Print(OutPath+"/Module_RMS_Stability_RMS_hist.png");
  }



 
  ///////////////
  ///Stability Fit results
  if(StabilityThr>0){
    gStabilityDeviation.SetMarkerStyle(8);
    gStabilityDeviation.SetMarkerSize(MARKERSIZE);
    gStabilityDeviation.GetYaxis()->SetTitle("Stability Deviation");
    gStabilityDeviation.GetXaxis()->SetTitle("Module ID");
    gStabilityDeviation.GetYaxis()->SetRangeUser(0,StabilityMax);    
    C->Clear();
    line.SetLineColor(2);
    line.SetLineWidth(3);
    //  gStabilityDeviation.Write(); //////////////////////////////////////////////to .root
    gStabilityDeviation.Draw("AP");
    line.DrawLine(spare_cut,StabilityThr,2000,StabilityThr);
    C->Print(OutPath+"/Module_RMS_StabilityDeviation.png");

    gStyle->SetOptStat(1111111);
    hStabilityDeviation.GetXaxis()->SetTitle("Stability Deviation");
    hStabilityDeviation.GetYaxis()->SetTitle("# of modules");
    C->Clear();
    line.SetLineColor(2);
    line.SetLineWidth(3);
    //  hStabilityDeviation.Write(); ////////////////////////////////////////////to .root
    hStabilityDeviation.Draw("hist");
    line.DrawLine(StabilityThr,0,StabilityThr,histomax);
    //drawCMSPrelim(0.18,0.85,"#font[62]{CMS} #font[52]{Preliminary}");
    drawFillYear(0,2024);
    drawPCCLuminometer(0.18,0.80);
    C->SetLogy(1);
    C->Print(OutPath+"/Module_RMS_StabilityDeviation_hist.png");
    C->SetLogy(0);
    gStyle->SetOptStat(0);
    //  file->Close();
  }
  //file->Close();
  ///////////////                                                                                                                                                                                                                                                                                                            
  ///Linearity Fit results -->Slope                                                                                                                                                                     
  if(LinearityThr>0){
    TFile* file = new TFile("/eos/user/l/lcuevasp/BRIL/PCC_Run3/Reprocess2024/PromptReco/All_Blocks/test/Lumi_days/PerModuleStability_veto_all_Blocks_Lty.root", "UPDATE");
    gLinearityDeviation_slope.SetMarkerStyle(8);                         
    gLinearityDeviation_slope.SetMarkerSize(MARKERSIZE);                
    gLinearityDeviation_slope.GetYaxis()->SetTitle("Linearity Deviation");
    gLinearityDeviation_slope.GetXaxis()->SetTitle("Module ID");          
    gLinearityDeviation_slope.GetYaxis()->SetRangeUser(LinearityMin,LinearityMax);   
    C->Clear();                                                           
    line.SetLineColor(2);                                                 
    //line.SetLineStyle(2);                                               
    line.SetLineWidth(3);                                                 
    // line.DrawLine(0,LinearityThr,1856,LinearityThr);
    gLinearityDeviation_slope.Write();   /////////////////////////////////////////////////////<<--
    gLinearityDeviation_slope.Draw("AP");
    
    line.DrawLine(spare_cut,LinearityThr_Low,2000,LinearityThr_Low);
    line.DrawLine(spare_cut,LinearityThr,2000,LinearityThr);              

    C->Print(OutPath+"/Module_RMS_LinearityDeviation_slope.png");         
    gStyle->SetOptStat(1111111);                                          
    hLinearityDeviation_slope.GetXaxis()->SetTitle("Linearity Deviation");
    hLinearityDeviation_slope.GetYaxis()->SetTitle("# of modules");   
    C->Clear();                                                       
    line.SetLineColor(2);                                             
    // line.SetLineStyle(2);                                          
    line.SetLineWidth(3);                                             
    // line.DrawLine(LinearityThr,0,LinearityThr,15);
    hLinearityDeviation_slope.Write();    ///////////////////////////////////////////////////<<--
    hLinearityDeviation_slope.Draw("hist");

    line.DrawLine(LinearityThr_Low,0,LinearityThr_Low,histomax);
    line.DrawLine(LinearityThr,0,LinearityThr,histomax);
    
    //drawCMSPrelim(0.18,0.85,"#font[62]{CMS} #font[52]{Preliminary}");
    drawFillYear(0,2024);                                            
    drawPCCLuminometer(0.18,0.80);                                   
    C->SetLogy(1);                                                   
    C->Print(OutPath+"/Module_RMS_LinearityDeviation_hist_slope.png");                                       
    C->SetLogy(0);                                               
    gStyle->SetOptStat(0);
    file->Close();
  }     
  
  
  
  ///////////////
  ///Linearity Fit results
  if(LinearityThr>0){
    gLinearityDeviation.SetMarkerStyle(8);
    gLinearityDeviation.SetMarkerSize(MARKERSIZE);
    gLinearityDeviation.GetYaxis()->SetTitle("Linearity Deviation");
    gLinearityDeviation.GetXaxis()->SetTitle("Module ID");
    gLinearityDeviation.GetYaxis()->SetRangeUser(0,LinearityMax);    
    C->Clear();
    gLinearityDeviation.Draw("AP");
    C->Print(OutPath+"/Module_RMS_LinearityDeviation.png");

    gStyle->SetOptStat(1111111);
    hLinearityDeviation.GetXaxis()->SetTitle("Linearity Deviation");
    hLinearityDeviation.GetYaxis()->SetTitle("# of modules");
    C->Clear();
    hLinearityDeviation.Draw("hist");
    line.DrawLine(StabilityThr,0,StabilityThr,60);
    //drawCMSPrelim(0.18,0.85,"#font[62]{CMS} #font[52]{Preliminary}");
    drawFillYear(0,2024);
    drawPCCLuminometer(0.18,0.80);
    C->SetLogy(1);
    C->Print(OutPath+"/Module_RMS_LinearityDeviation_hist.png");
    C->SetLogy(0);
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

    P_L2->SetMarkerColor(kRed);
    P_L3->SetMarkerColor(kGreen);
    P_L4->SetMarkerColor(kBlue);    
    P_D1S1->SetMarkerColor(kBlack);
    P_D2S1->SetMarkerColor(kMagenta);
    P_D3S1->SetMarkerColor(kCyan);
    P_D1S2->SetMarkerColor(kOrange);//5
    P_D2S2->SetMarkerColor(kAzure);//9
    P_D3S2->SetMarkerColor(kGray);
   
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
    // legend->AddEntry(P_L2,"B2","p");
    // legend->AddEntry(P_L3,"B3","p");
    //legend->AddEntry(P_L4,"B4","p");
    //legend->AddEntry(P_D1S1,"F1S1","p");
    //legend->AddEntry(P_D2S1,"F2S1","p");
    //legend->AddEntry(P_D3S1,"F3S1","p");
    //legend->AddEntry(P_D1S2,"F1S2","p");
    //legend->AddEntry(P_D2S2,"F2S2","p");
    //legend->AddEntry(P_D3S2,"F3S2","p");
    //legend->SetFillColor(0);
    //legend->SetLineColor(1);

    //DrawLatexNDC(0.65, 0.82, "#bullet")
    TLatex text;
    text.SetTextSize(0.040);
    text.SetTextFont(42);
    
    // text.SetTextColor(kRed);
    // text.DrawLatexNDC(0.92 , 0.87, "B2");
    
    
    P_L4->SetStats(0);
    P_L4->GetYaxis()->SetRangeUser(0,MAXLAYERWEIGHT);
    P_L4->GetYaxis()->SetTitle("Weight");
    P_L4->GetYaxis()->SetNdivisions(8);
    P_L4->GetXaxis()->SetTitle("Lumi section");
    C->Clear();
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
    text.SetTextColor(kRed);             
    text.DrawLatexNDC(0.93 , 0.85, "B2");
    text.SetTextColor(kGreen);      
    text.DrawLatexNDC(0.93 , 0.78, "B3");
    text.SetTextColor(kBlue); 
    text.DrawLatexNDC(0.93 , 0.71, "B4");
    text.SetTextColor(kBlack);
    text.DrawLatexNDC(0.92 , 0.64, "F1S1");
    text.SetTextColor(kMagenta);
    text.DrawLatexNDC(0.92 , 0.57, "F2S1");
    text.SetTextColor(kCyan);
    text.DrawLatexNDC(0.92 , 0.50, "F3S1");
    text.SetTextColor(kOrange);
    text.DrawLatexNDC(0.92 , 0.43, "F1S2");
    text.SetTextColor(kAzure);
    text.DrawLatexNDC(0.92 , 0.36, "F2S2");
    text.SetTextColor(kGray);
    text.DrawLatexNDC(0.92 , 0.29, "F3S2");

    
    C->Print(OutPath+"/Module_RMS_Stability_internal_perlayerdisk_weight.png");    


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
 
    C->Clear();
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
    
    text.SetTextColor(kRed);       
    text.DrawLatexNDC(0.93 , 0.85, "B2");  
    text.SetTextColor(kGreen);             
    text.DrawLatexNDC(0.93 , 0.78, "B3");  
    text.SetTextColor(kBlue);              
    text.DrawLatexNDC(0.93 , 0.71, "B4");  
    text.SetTextColor(kBlack);             
    text.DrawLatexNDC(0.92 , 0.64, "F1S1");
    text.SetTextColor(kMagenta);           
    text.DrawLatexNDC(0.92 , 0.57, "F2S1"); 
    text.SetTextColor(kCyan);               
    text.DrawLatexNDC(0.92 , 0.50, "F3S1"); 
    text.SetTextColor(kOrange);            
    text.DrawLatexNDC(0.92 , 0.43, "F1S2");
    text.SetTextColor(kAzure);          
    text.DrawLatexNDC(0.92 , 0.36, "F2S2");
    text.SetTextColor(kGray);        
    text.DrawLatexNDC(0.92 , 0.29, "F3S2");
    
    C->Print(OutPath+"/Module_RMS_Stability_internal_perlayerdisk.png");


    
   
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

    PLinearityL2->SetMarkerColor(kRed);
    PLinearityL3->SetMarkerColor(kGreen);
    PLinearityL4->SetMarkerColor(kBlue);    
    PLinearityD1S1->SetMarkerColor(kBlack);
    PLinearityD2S1->SetMarkerColor(kMagenta);
    PLinearityD3S1->SetMarkerColor(kCyan);
    PLinearityD1S2->SetMarkerColor(kOrange);
    PLinearityD2S2->SetMarkerColor(kAzure);
    PLinearityD3S2->SetMarkerColor(kGray);
   
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
    // legendLinearity->AddEntry(PLinearityL2,"B2","p");
    // legendLinearity->AddEntry(PLinearityL3,"B3","p");
    //legendLinearity->AddEntry(PLinearityL4,"B4","p");
    //legendLinearity->AddEntry(PLinearityD1S1,"F1S1","p");
    //legendLinearity->AddEntry(PLinearityD2S1,"F2S1","p");
    //legendLinearity->AddEntry(PLinearityD3S1,"F3S1","p");
    //legendLinearity->AddEntry(PLinearityD1S2,"F1S2","p");
    //legendLinearity->AddEntry(PLinearityD2S2,"F2S2","p");
    //legendLinearity->AddEntry(PLinearityD3S2,"F3S2","p");
    //legendLinearity->SetFillColor(0);
    //legendLinearity->SetLineColor(1);

    TLatex text;                      
    text.SetTextSize(0.040);                             
    text.SetTextFont(42);    

    C->Clear();
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
    
    text.SetTextColor(kRed);                
    text.DrawLatexNDC(0.93 , 0.85, "B2");   
    text.SetTextColor(kGreen);              
    text.DrawLatexNDC(0.93 , 0.78, "B3");   
    text.SetTextColor(kBlue);               
    text.DrawLatexNDC(0.93 , 0.71, "B4");   
    text.SetTextColor(kBlack);              
    text.DrawLatexNDC(0.92 , 0.64, "F1S1"); 
    text.SetTextColor(kMagenta);            
    text.DrawLatexNDC(0.92 , 0.57, "F2S1"); 
    text.SetTextColor(kCyan);               
    text.DrawLatexNDC(0.92 , 0.50, "F3S1"); 
    text.SetTextColor(kOrange);             
    text.DrawLatexNDC(0.92 , 0.43, "F1S2"); 
    text.SetTextColor(kAzure);              
    text.DrawLatexNDC(0.92 , 0.36, "F2S2"); 
    text.SetTextColor(kGray);               
    text.DrawLatexNDC(0.92 , 0.29, "F3S2"); 
    C->Print(OutPath+"/Module_RMS_Stability_linearity_perlayerdisk_weight.png");    
 
   
    C->Clear();
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

    text.SetTextColor(kRed);            
    text.DrawLatexNDC(0.93 , 0.85, "B2");   
    text.SetTextColor(kGreen);              
    text.DrawLatexNDC(0.93 , 0.78, "B3");   
    text.SetTextColor(kBlue);               
    text.DrawLatexNDC(0.93 , 0.71, "B4");   
    text.SetTextColor(kBlack);              
    text.DrawLatexNDC(0.92 , 0.64, "F1S1"); 
    text.SetTextColor(kMagenta);            
    text.DrawLatexNDC(0.92 , 0.57, "F2S1"); 
    text.SetTextColor(kCyan);               
    text.DrawLatexNDC(0.92 , 0.50, "F3S1"); 
    text.SetTextColor(kOrange);             
    text.DrawLatexNDC(0.92 , 0.43, "F1S2"); 
    text.SetTextColor(kAzure);              
    text.DrawLatexNDC(0.92 , 0.36, "F2S2"); 
    text.SetTextColor(kGray);               
    text.DrawLatexNDC(0.92 , 0.29, "F3S2"); 
    
    C->Print(OutPath+"/Module_RMS_Stability_linearity_perlayerdisk.png");    
    

  file->Close();
  }


  
}

  

