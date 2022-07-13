#include <iostream>
#include <fstream>
#include <string>

#define NBX 3564
#define NMOD 2000
#define ORBITF 11245.6
#define NLS 4000 //max number of lumi sections for histograms
#define InstLumiScale (1e30/23.31)/1e34 //brilcalc lumi/lumisection [1/microbarn], 1 barn = 10^-24 cm^2,  y value will be in unites of 1e34


#define MAXPCCRUN 5e10
#define PCCZEROLUMI 1e3
#define PCCRUNLUMIRatioMin 18
#define PCCRUNLUMIRatioMax 22
long RUNOFFSET=315000;

TString RefLumi="hfoc";

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


///////////crossection
float getSigmaVis(int run){

  if(run>=352416){//Run 3 Data with 2018ABC veto
    return 1.00895*5.91e6/ORBITF; // new veto list, Georgios fixed pixel double counting
  }else if(run>=323700){
    return 3.14e6/ORBITF; // for second part of RunD, tighter stability cut ABCD

    //#define SigmaPCC 0.0117935*3.14e6/(23.31*11245.6) // for second part of RunD , BPIX B1
    //#define SigmaPCC 0.20715*3.14e6/(23.31*11245.6) // for second part of RunD , BPIX B2
    //#define SigmaPCC 0.280174*3.14e6/(23.31*11245.6) // for second part of RunD , BPIX B3
    //#define SigmaPCC 0.093988*3.14e6/(23.31*11245.6) // for second part of RunD , FPIX disk1 Panel1
    //#define SigmaPCC 0.118907*3.14e6/(23.31*11245.6) // for second part of RunD , FPIX disk2 Panel1
    //#define SigmaPCC 0.119082*3.14e6/(23.31*11245.6) // for second part of RunD , FPIX disk3 Panel1

  }else if(run>=315252){
    //#define SigmaPCC 5.8e6/ORBITF // old veto list, before stability analysis
    return 1.00895*5.91e6/ORBITF; // new veto list, Georgios fixed pixel double counting    
  }else if(run>=306473){
    return 0.9702*3.2074e6/ORBITF; // Run2017G
  }

  cout<<"sigma vis not found for run : "<<run<<endl;
  return 1;
}



/////////////////////////
// Module coordinates
////////////////////////
std::map<int,bool> MODVETO;
std::map<int,int> BPIXorFPIX; //1 or 2

//BPIX
std::map<int,int> LY;
std::map<int,int> LD;
std::map<int,int> MD;
unsigned BPIX_nLD[4]={12,28,44,64};
std::map<int,float> BPIX_R;
std::map<int,float> BPIX_PHI;
std::map<int,float> BPIX_Z;

//FPIX
std::map<int,int> SD;
std::map<int,int> DI;
std::map<int,int> BL;
std::map<int,int> PN;
unsigned FPIX_nBL = 56;
unsigned FPIX_nBLR[2] = {22,34};
std::map<int,int> DISK;//derived coordinates
std::map<int,int> RING;
std::map<int,float> FPIX_R;
std::map<int,float> FPIX_PHI;
std::map<int,float> FPIX_Z;

void readModRPhiZCoordinates(){

  int counter=0;
  int NBPIX[4]={0,0,0,0};//modules per layer
  int NFPIX[6]={0,0,0,0,0,0};//modules per disk

  //BPix
  ifstream cfile_bpix("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_BPix_raw.txt");
  if (!cfile_bpix.is_open()){
    std::cout << "Unable to open coordinates"<<endl;
    return;
  }

  std::string cline_bpix;
  while (std::getline(cfile_bpix,cline_bpix)){
    std::stringstream iss(cline_bpix);
    long module,ly,ld,mod;
    float r,phi,z;
    
    iss>>module>>ly>>ld>>mod>>r>>phi>>z;
    //cout<<module<<" "<<ly<<" "<<ld<<" "<<mod<<" "<<r<<" "<<phi<<" "<<z<<endl;

    LY[module]=ly;//1,2,3,4
    LD[module]=ld;//
    MD[module]=mod;//
    
    BPIX_R[module]=r;
    BPIX_PHI[module]=phi;
    BPIX_Z[module]=z;

    NBPIX[ly-1]++;
    
    MODVETO[module]=0;
    BPIXorFPIX[module]=1;
    
    counter++;
  }


  ////FPIX
  ifstream cfile_fpix("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_FPix_raw.txt");
  if (!cfile_fpix.is_open()){
    std::cout << "Unable to open coordinates"<<endl;
    return;
  }
  std::string cline_fpix;
  while (std::getline(cfile_fpix,cline_fpix)){
    //352589828    side/disk/blade/pannel/plaq=2/1/1/2/1    r/phi/z=7.74703/0.0443517/31.6927
    std::stringstream iss(cline_fpix);
    long module,sd,di,bl,pn,pl;
    float r,phi,z;
    iss>>module>>sd>>di>>bl>>pn>>pl>>r>>phi>>z;
    //cout<<module<<" "<<sd<<" "<<di<<" "<<bl<<" "<<pn<<" "<<r<<" "<<phi<<" "<<z<<endl;

    SD[module]=sd;//1,2
    DI[module]=di;//1,2,3
    BL[module]=bl;
    PN[module]=pn;//1,2
    if(sd==1) DISK[module]=-di+3;//0,1,2
    if(sd==2) DISK[module]=di+2; //3,4,5
    if(bl<=22) RING[module]=0;
    if(bl>22)  RING[module]=1;

    FPIX_R[module]  =r;
    FPIX_PHI[module]=phi;
    FPIX_Z[module]  =z;

    NFPIX[DISK[module]]++;

    MODVETO[module]=0;
    BPIXorFPIX[module]=2;

    counter++;
  }

  cout<<"Total number of modules: "<<counter<<endl;
  cout<<"BPIX modules per layer:  "<<NBPIX[0]<<" , "<<NBPIX[1]<<" , "<<NBPIX[2]<<" , "<<NBPIX[3]<<endl;
  cout<<"FPIX modules per disk:  "<<NFPIX[0]<<" , "<<NFPIX[1]<<" , "<<NFPIX[2]<<" , "<<NFPIX[3]<<" , "<<NFPIX[4]<<" , "<<NFPIX[5]<<endl;

  cout<<"Done reading the  module coordinates."<<endl;
}


void readModVeto(TString file){
  ifstream myfile(file.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open "<<endl;
    return;
  }
  std::string line;
  while (std::getline(myfile,line)){
    int module=atoi(line.c_str());
    MODVETO[module]=1;
  }
}

