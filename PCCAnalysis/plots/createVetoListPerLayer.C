#include <iostream>
#include <fstream>
#include <string>

//
std::vector<long> veto;


void createVetoListPerLayer(string VetoList="veto_master_VdM_ABCD_2018_newcuts_BPix.txt", int BF=0, int LY=0){

  ///// Parameters:
  //BF: BPIX=0 ,  FPIX=1
  //LY: Layer for BPIX (0,1,2,3) or Disk for FPIX (1,2,3) 

  //open the new veto list:
  std::ofstream o("veto.txt");


  //open the initial veto list
  ifstream myfile(VetoList.c_str());
  if (!myfile.is_open()){
    std::cout << "Unable to open the initial veto list. "<<endl;
    return;
  }

  //write the initial veto list into the new list
  std::string line;
  while (std::getline(myfile,line)){
    int module=atoi(line.c_str());
    veto.push_back(module);
    o<<module<<std::endl;
  }
  //cout<<"initial veto list size: "<<veto.size()<<endl;

  
  //// BPIX
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
    if(!(BF==0 && LY==ly-1)
       && std::find(veto.begin(),veto.end(),module) == veto.end()
       )
      o<<module<<std::endl;
  }
  
  
  //// FPIX
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
    if(!(BF==1 && LY==di)
       && std::find(veto.begin(),veto.end(),module) == veto.end()
       )
      o<<module<<std::endl;
  }

  o.close();
  gROOT->ProcessLine(".q");
}
