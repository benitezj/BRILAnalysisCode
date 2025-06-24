#include "globals.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

//This code creates a veto which selects good modules in a layer or disk based on a prior module veto 
//BPIXFPIX: 1: BPIX , 2: FPIX
//LAYERDISK BPIX (1,2,3,4) 
//LAYERDISK FPIX (2,1,0,3,4,5) order of z position (?)

// BPIXFPIX=3 //special selection for 2017 

void createVetoListPerLayer(string InputVeto="inputveto.txt", int BPIXFPIX=0, int LAYERDISK=0){

  if(!readModRPhiZCoordinates()) return;
  
  if(!readModVeto(InputVeto)) return;

  system("rm -f veto.txt");  
  //write the new veto list
  std::ofstream o("veto.txt");
  for (unsigned int i=0;i<NMOD;i++){

    if(BPIXFPIX==1 || BPIXFPIX==2){
      if(BPIXorFPIX[MODID[i]]!=BPIXFPIX)      
	o<<MODID[i]<<std::endl;
      else if(BPIXorFPIX[MODID[i]]==1 && LY[MODID[i]]!=LAYERDISK)
	o<<MODID[i]<<std::endl;
      else if(BPIXorFPIX[MODID[i]]==2 && DISK[MODID[i]]!=LAYERDISK)
	o<<MODID[i]<<std::endl;
      else if(MODVETO[MODID[i]]==1)
	o<<MODID[i]<<std::endl;
    }

    
    if( BPIXFPIX==3 ){
      if(MODVETO[MODID[i]]==1)         // modules in the veto file 
	o<<MODID[i]<<std::endl;
      else if(BPIXorFPIX[MODID[i]]==1) // modules in the all the BPIX layers 
	o<<MODID[i]<<std::endl;
    }
    
  }

  
  o.close();
  cout<<"veto.txt created"<<endl;
  system("wc veto.txt");
  gROOT->ProcessLine(".q");
}
