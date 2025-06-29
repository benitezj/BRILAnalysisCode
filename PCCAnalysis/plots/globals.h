#include <iostream>
#include <fstream>
#include <string>
#include "modules.h" // this is matches to the table in the RawPCCProducer_modveto plugin
//#include "modules_361913.h" //list extracted from a vdm run


#define NBX 3564
#define NMOD 1856
std::map<long,bool> MODVETO;

#define ORBITF 11245.6

#define NLS 4000 //max number of lumi sections for histograms
#define InstLumiScale (1e30/23.31)/1e34 //brilcalc lumi/lumisection [1/microbarn], 1 barn = 10^-24 cm^2,  y value will be in units of 1e34


#define MAXPCCRUN 5e10
#define PCCZEROLUMI 1e3
#define PCCRUNLUMIRatioMin 18
#define PCCRUNLUMIRatioMax 22
long RUNOFFSET=315000;


///plot labels
TString RefLumi="HFOC";
TString InstLumiAxisTitle("HFOC Inst. Luminosity [ E34#scale[1.0]{cm}^{-2} s^{-1} ]");
void drawLumiTitle(long FILL,long year=2022){
  TLatex text;
  text.SetTextColor(1);
  text.SetTextSize(0.040);
  text.DrawLatexNDC(0.25,0.94,TString("CMS Offline Luminosity, LHC Fill ")+(long)FILL+",  "+year+",  (#sqrt{s} = 13 TeV)");
}
void drawFillYear(long FILL=0,long year=0){
  TLatex text;
  text.SetTextColor(1);
  text.SetTextSize(0.03);
  //text.DrawLatexNDC(0.6,0.94,TString("Fill ")+(long)FILL+",  "+year+",  (#sqrt{s} = 13 TeV)");
  TString FillYear="";
  float xpos=0.8;
  if(FILL!=0){FillYear+=TString(" Fill ")+(long)FILL+",";  xpos-=0.1;}
  if(year!=0){FillYear+=TString("  ")+year+",";  xpos-=0.1;}
  if(year>=2022) FillYear+=TString("  ")+" #sqrt{s} = 13.6 TeV ";
  else FillYear+=TString("  ")+" #sqrt{s} = 13 TeV ";
  text.DrawLatexNDC(xpos,0.94,FillYear.Data());
}
void drawCMSPrelim(float xpos=0.19,float ypos=0.85, TString label="#font[62]{CMS} #font[52]{Preliminary}"){
  TLatex text;
  text.SetTextColor(1);
  text.SetTextSize(0.040);
  text.DrawLatexNDC(xpos,ypos,label);
}
void drawPCCLuminometer(float xpos=0.19,float ypos=0.80, TString subdet=""){
  TLatex text;
  text.SetTextColor(1);
  text.SetTextSize(0.035);
  text.DrawLatexNDC(xpos,ypos,TString("#font[52]{PCC} "));
  if(subdet.CompareTo("")!=0) text.DrawLatexNDC(xpos+0.07,ypos,TString("#font[52]{")+subdet+"}");
}


///////////crossection
float getSigmaVis(int run){
  
  if(run>=366403){ //2023
    //return 0.9104*1.403e6/ORBITF;  // first veto (264 modules) by Luis, scaled by the 24 removed modules
    //return 1.29843e6/ORBITF;  // Luis analysis 12 December https://docs.google.com/presentation/d/1h-ID3W8VFmSQBEhrFkKsymI7uuIHoap9WyPMLWLX7AU/edit#slide=id.g263901dc7ed_0_52
    return 1.30773239e6/ORBITF; // Fabio pcc23VdMRescaledv0
  }else if(run>=359268){ //2022 EFG
    //return 6.037e6/ORBITF;  // 2018 ABC veto, Luis calibration using fill 8178 (Sept. 2022), Mattermost slides Dec 5 
    //return 4.1533e6/ORBITF; // veto_CDEFG_3_2022.txt, Luis thesis 6/28/2023 
    return 1.0760e6/ORBITF;  // final veto, vdM by Nimmitha+Joanna (AN 18Sept2023)
  }else if(run>=355862){ //2022 CD 
    return (1.008*1.0760e6)/ORBITF;  
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
  }else if(run>=306896){// Run2017H
    return 0.973*(117/158.)*1591.9e3/ORBITF; //calibration from Peter  (june 26) https://mattermost.web.cern.ch/dr-benitez/pl/yncai3zg6ir6zpprohmbid1dmh 
  }else if(run>=306473){// Run2017G
    return 0.9702*3.2074e6/ORBITF; 
  }else if(run>=297046){ // 2017B 
    return 4.6470e6/ORBITF; //result from Peter in Run 2 paper AN (14/Nov2024) https://www.overleaf.com/project/65af85cc5c3a839be2bffb71 , with bkg values from tail averates: 4.6693, with Const: 4.6745
  }

  cout<<"sigma vis not found for run : "<<run<<endl;
  return 1;
}


float getsigmavis_as(int run1){
  //New veto list, Ashish Sehrawat
  if(run1>=324564){
    return 6.72261000001e6/ORBITF;   // for Period D4 
  }else if(run1>=323363){
    return 6.16056000002e6/ORBITF;   // for Period D3
  }else if(run1>=321710){
    return 5.23820999995e6/ORBITF;  // for Period D2
  }else if(run1>=320500){
    return 5.15907e6/ORBITF;        // for Period D1
  }else if(run1>=319337){
    return 6.41462000002e6/ORBITF;   // for Period C
  }else if(run1>=317080){
    return 9.23084000002e6/ORBITF;   // for Period B
  }else if(run1>=315252){
    return 4.30860999995e6/ORBITF;   // for Period A 
  }

  cout<<"sigma vis not found for run : "<<run1<<endl;
  return 1;
}


float getsigmavis_as_veto1701(int run2){
  //2% rms common veto list, Ashish Sehrawat
  if(run2>=315252){
    return 0.959939e6/ORBITF;   // for entire 2018 period
  }
  cout<<"sigma vis not found for run : "<<run2<<endl;
  return 1;
}



float getsigmavis_as_veto1615(int run3){
  if(run3>=315252){ //4% rms common veto list, Ashish Sehrawat
    return 1.673490e6/ORBITF;   // for entire 2018 period
  }
  cout<<"sigma vis not found for run : "<<run3<<endl;
  return 1;
}


/////////////////////////
// Module coordinates
////////////////////////
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

int NBPIX[4]={0,0,0,0};//modules per layer
int NFPIX[6]={0,0,0,0,0,0};//modules per disk


bool readModRPhiZCoordinates(){
  std::cout<<"Starting to read the Module coordinates"<<endl;
  std::cout<<" MODID list size: "<<MODID.size()<<endl;
  
  int counter=0;

  //BPix
  ifstream cfile_bpix("./BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_BPix_raw.txt");
  if (!cfile_bpix.is_open()){
    std::cout << "Unable to open BPIXcoordinates"<<endl;
    return 0;
  }
  
  std::cout<<"opened: BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_BPix_raw.txt"<<endl;
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
    
    //MODVETO[module]=0;
    BPIXorFPIX[module]=1;
    
    if (std::count(MODID.begin(), MODID.end(), module)!=1) {
      std::cout << "Module id bad: "<<module<<endl;
    }

    counter++;
  }


  ////FPIX
  ifstream cfile_fpix("./BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_FPix_raw.txt");
  if (!cfile_fpix.is_open()){
    std::cout << "Unable to open FPIX coordinates"<<endl;
    return 0;
  }
  std::cout<<"opened: BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_FPix_raw.txt"<<endl;
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

    BPIXorFPIX[module]=2;

    
    if (std::count(MODID.begin(), MODID.end(), module)!=1) {
      std::cout << "Module id bad: "<<module<<endl;
    }

    
    counter++;
  }

  cout<<"BPIX modules per layer:  "<<NBPIX[0]<<" , "<<NBPIX[1]<<" , "<<NBPIX[2]<<" , "<<NBPIX[3]<<endl;
  cout<<"FPIX modules per disk:  "<<NFPIX[0]<<" , "<<NFPIX[1]<<" , "<<NFPIX[2]<<" , "<<NFPIX[3]<<" , "<<NFPIX[4]<<" , "<<NFPIX[5]<<endl;
  cout<<"Total number of modules in coordinate files: "<<counter<<endl;

  cout<<"Done reading the  module coordinates."<<endl;
  return 1;
}


bool readModVeto(TString file=""){

  for (unsigned int i=0;i<NMOD;i++)
    MODVETO[MODID[i]]=0;
  
  std::cout<<"Starting to read the Module veto"<<endl;

  if(file.CompareTo("")==0){
    cout<<"no veto"<<endl;
    return 1;
  }
  
  std::cout<<" check MODID list size: "<<MODID.size()<<endl;
  
  ifstream inputfile(file.Data());
  if (!inputfile.is_open()){
    std::cout << "Unable to open module veto file"<<endl;
    return 0;
  }
  cout<<"opened file: "<<file<<endl;
  int count=0;
  std::string line;
  while (std::getline(inputfile,line)){
    long module=atoi(line.c_str());

    if (std::count(MODID.begin(), MODID.end(), module)!=1) {
      std::cout << "Module id is duplicated or not in the MODID List: "<<module<<endl;
    }

    if (MODVETO[module]==1) {
      std::cout << "Module is duplicate: "<<module<<endl;
    }

    
    MODVETO[module]=1;
    count++;
  }
  inputfile.close();
  cout<<"Done reading the module veto "<<endl;
  cout<<"Number of bad modules: "<<count<<endl;

  return 1;
}
