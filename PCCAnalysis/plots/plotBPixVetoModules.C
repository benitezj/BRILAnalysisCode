#include <iostream>
#include <fstream>
#include <string>

//BPIX
std::map<long,int> LY;
std::map<long,int> LD;
std::map<long,int> MD;

//FPIX
std::map<long,int> SD;
std::map<long,int> DI;
std::map<long,int> BL;
std::map<long,int> PN;


void plotBPixVetoModules(string VetoList="veto_master_VdM_ABCD_2018_newcuts_BPix.txt"){

  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");

  //BPIX
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
  
  //BPIX
  TH2F B[4];
  int bins[4]={6,14,22,32};
  for(long l=0;l<4;l++){
    B[l]=TH2F(TString("L")+l,"",9,-4.5,4.5,2*bins[l]+1,-bins[l]-0.5,bins[l]+.5);
    B[l].SetStats(0);
    B[l].GetXaxis()->SetTitle("Signed Module");
    B[l].GetYaxis()->SetTitle("Signed Ladder");
    //B[l].SetMarkerStyle(8);
  }

  ifstream myfile(VetoList.c_str());
  if (!myfile.is_open()){
    std::cout << "Unable to open "<<endl;
    return;
  }

  
  int BCountN[4]={0,0,0,0};
  int BCountP[4]={0,0,0,0};
  std::string line;
  while (std::getline(myfile,line)){
    int module=atoi(line.c_str());
    
    if ( MD.find(module) == MD.end() ) continue;
    
    int md=MD[module]>4?MD[module]-4:MD[module]-5;
    int ld=LD[module]>bins[LY[module]-1]?LD[module]-bins[LY[module]-1]:LD[module]-bins[LY[module]-1]-1;
    cout<<module<<" "<<LY[module]<<" "<<md<<" "<<ld<<endl;

    B[LY[module]-1].Fill(md,ld);
    if(md<0)BCountN[LY[module]-1]++;
    if(md>0)BCountP[LY[module]-1]++;
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


  TH2F F[2];
  for(long l=0;l<2;l++){
    F[l]=TH2F(TString("D")+l,"",7,-3.5,3.5,57,-28.5,28.5);
    F[l].SetStats(0);
    F[l].GetXaxis()->SetTitle("Signed Disk");
    F[l].GetYaxis()->SetTitle("Signed Blade");
    //F[l].SetMarkerStyle(8);
  }

  ifstream myfile_fpix(VetoList.c_str());
  if (!myfile_fpix.is_open()){
    std::cout << "Unable to open "<<endl;
    return;
  }

  int FCountN[2]={0,0};
  int FCountP[2]={0,0};
  while (std::getline(myfile_fpix,line)){
    int module=atoi(line.c_str());
    
    if( SD.find(module) == SD.end() ) continue;

    int di=(SD[module]==2?1:-1)*DI[module];
    int bl=BL[module]>28?BL[module]-28:BL[module]-29;
    cout<<module<<" "<<di<<" "<<bl<<" "<<PN[module]<<endl;

    F[PN[module]-1].Fill(di,bl);
    if(di<0)FCountN[PN[module]-1]++;
    if(di>0)FCountP[PN[module]-1]++;
  }
  

  TCanvas C;
  for(long l=0;l<4;l++){
    C.Clear();
    B[l].SetTitle(TString("BPIX Layer ")+l);
    B[l].Draw("col");
    C.Print(TString("VetoMap_Layer")+(l+1)+".png");
  }


  for(long l=0;l<2;l++){
    C.Clear();
    F[l].SetTitle(TString("FPIX ")+l);
    F[l].Draw("col");
    C.Print(TString("VetoMap_FPIX")+(l+1)+".png");
  }



  ///summary of counts
  cout<<"BPIX 0 : "<<BCountN[0]<<" , "<<BCountP[0]<<endl;
  cout<<"BPIX 1 : "<<BCountN[1]<<" , "<<BCountP[1]<<endl;
  cout<<"BPIX 2 : "<<BCountN[2]<<" , "<<BCountP[2]<<endl;
  cout<<"BPIX 3 : "<<BCountN[3]<<" , "<<BCountP[3]<<endl;

  cout<<"FPIX 0 : "<<FCountN[0]<<" , "<<FCountP[0]<<endl;
  cout<<"FPIX 1 : "<<FCountN[1]<<" , "<<FCountP[1]<<endl;
  
}
