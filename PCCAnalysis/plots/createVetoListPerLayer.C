#include <iostream>
#include <fstream>
#include <string>

std::vector<long> veto;

void createVetoListPerLayer(string VetoList="module_L1_rms_mean_gr_0.07_totcount10^8_third_iteration.txt", int BF=1, int LY=3, int PN=2){

  ///// Parameters:
  //BF: BPIX=0 ,  FPIX=1
  //LY: Layer for BPIX (0,1,2,3) or Disk for FPIX (1,2,3) 
  //PN: Panel # for FPIX only (1,2)

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
  ifstream cfile_bpix("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_BPix.txt");
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
    //cout<<module<<"  "<<ly<<"  "<<ld<<"  "<<"  "<<mod<<endl;
    //cout<<module<<"  "<<ly<<endl; 
 }
  
  
  //// FPIX
  ifstream cfile_fpix("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_FPix.txt");
  if (!cfile_fpix.is_open()){
    std::cout << "Unable to open coordinates"<<endl;
    return;
  }
  std::string cline_fpix;
  while (std::getline(cfile_fpix,cline_fpix)){
    std::stringstream iss(cline_fpix);
    long module,sd,di,bl,pn;
    iss>>module>>sd>>di>>bl>>pn;
    if(!(BF==1 && LY==di && (PN==pn||PN==-1))
       && std::find(veto.begin(),veto.end(),module) == veto.end()
       )
      o<<module<<std::endl;
    //cout<<module<<"  "<<sd<<"  "<<di<<"  "<<"  "<<bl<<"  "<<pn<<endl;
    //cout<<module<<"  "<<sd<<"  "<<di<<endl;
  }

  o.close();
  gROOT->ProcessLine(".q");
}
