#include <iostream>
#include <fstream>
#include <string>

#define NBX 3564
#define NMOD 2000


#define MAXPCCRUN 5e10
#define PCCZEROLUMI 1e3
#define PCCRUNLUMIRatioMin 18
#define PCCRUNLUMIRatioMax 22
long RUNOFFSET=315000;


#define NLS 3000 //max number of lumi sections for histograms
float InstLumiScale=(1e30/23.31)/1e34; //brilcalc lumi/lumisection [1/microbarn], 1 barn = 10^-24 cm^2,  y value will be in unites of 1e34

////CMS plot labels
TString InstLumiAxisTitle("HFOC Inst. Luminosity [ E34#scale[1.0]{cm}^{-2} s^{-1} ]");

void drawLumiTitle(long FILL){
  TLatex text;
  text.SetTextColor(4);
  text.SetTextSize(0.040);
  text.DrawLatexNDC(0.25,0.94,TString("CMS Offline Luminosity, LHC Fill ")+(long)FILL+", #sqrt{s} = 13 TeV");
}

void drawCMSPrelim(long year,float xpos=0.2){
  TLatex text;
  text.SetTextColor(1);
  text.SetTextSize(0.050);
  text.DrawLatexNDC(xpos,0.85,TString("#font[62]{CMS} #font[52]{Preliminary ")+year+"}");
}



/////////////////////////
// Module coordinates
////////////////////////
//BPIX
std::map<long,int> LY;
std::map<long,int> LD;
std::map<long,int> MD;
unsigned BPIX_nLD[4]={12,28,44,64};

//FPIX
std::map<long,int> SD;
std::map<long,int> DI;
std::map<long,int> BL;
std::map<long,int> PN;
unsigned FPIX_nBL = 56;

void readModCoordinates(){

  //BPix
  ifstream cfile_bpix("BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_BPix.txt");
  if (!cfile_bpix.is_open()){
    std::cout << "Unable to open coordinates"<<endl;
    return;
  }
  std::string cline_bpix;
  while (std::getline(cfile_bpix,cline_bpix)){
    std::stringstream iss(cline_bpix);
    long module,ly,ld,mod;
    iss>>module>>ly>>ld>>mod;
    LY[module]=ly;
    LD[module]=ld;
    MD[module]=mod;
  }



  ////FPIX
  ifstream cfile_fpix("BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_FPix.txt");
  if (!cfile_fpix.is_open()){
    std::cout << "Unable to open coordinates"<<endl;
    return;
  }
  std::string cline_fpix;
  while (std::getline(cfile_fpix,cline_fpix)){
    std::stringstream iss(cline_fpix);
    long module,sd,di,bl,pn;
    iss>>module>>sd>>di>>bl>>pn;
    SD[module]=sd;
    DI[module]=di;
    BL[module]=bl;
    PN[module]=pn;
  }


  cout<<"successfully read the module coordinates"<<endl;
}
