#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"

///**WARNING*** for 2017 This code cannot be used. The module list (MODID) is not same as for 2018. In the 2017 original veto there are id's not found in the 2018 module coordinate database. 

//This code creates a veto which selects good modules in a layer or disk based on a prior module veto 
//BPIXFPIX: 1: BPIX , 2: FPIX
//LAYERDISK BPIX (1,2,3,4) 
//LAYERDISK FPIX (2,1,0,3,4,5) order of z position (?)

void createVetoListPerLayer(string InputVeto="inputveto.txt", int BPIXFPIX=0, int LAYERDISK=0){

  ///read the full module list (MODID)
  readModRPhiZCoordinates();
  
  if(!readModVeto(InputVeto)) return;
  
  //write the new veto list
  std::ofstream o("veto.txt");
  for (unsigned int i=0;i<NMOD;i++){
    if(BPIXorFPIX[MODID[i]]!=BPIXFPIX)      
      o<<MODID[i]<<std::endl;
    else if(BPIXorFPIX[MODID[i]]==1 && LY[MODID[i]]!=LAYERDISK)
      o<<MODID[i]<<std::endl;
    else if(BPIXorFPIX[MODID[i]]==2 && DISK[MODID[i]]!=LAYERDISK)
      o<<MODID[i]<<std::endl;
    else if(MODVETO[MODID[i]]==1)
      o<<MODID[i]<<std::endl;
  }

  
  o.close();
  cout<<"veto.txt created"<<endl;
  gROOT->ProcessLine(".q");
}
