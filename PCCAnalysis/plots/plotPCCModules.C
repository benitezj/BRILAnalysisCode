#include <iostream>
#include <fstream>
#include <string>

//#include "BRILAnalysisCode/PCCAnalysis/plots/globals.h"
#include "BRILAnalysisCode/PCCAnalysis/plots/plotModuleCoords.C"

void plotPCCModules(long Run){
 
  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");
  gStyle->SetOptStat(0);  

  TString Path="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018B_dynamicVeto_modules";
  TString OutPath="/afs/cern.ch/user/b/benitezj/www/BRIL/PCC_lumi/DynamicVeto";  

  readModRPhiZCoordinates();

  TH2F HModVsBX("HModVsBX","",NBX,0.5,NBX+0.5,NMOD,0,NMOD);
  HModVsBX.GetXaxis()->SetTitle("bcid");
  HModVsBX.GetYaxis()->SetTitle("Module");
  HModVsBX.GetZaxis()->SetTitle("counts");
  
  TH1F HMod("HMod","",NMOD,0,NMOD);
  HMod.GetXaxis()->SetTitle("Module");
  HMod.GetYaxis()->SetTitle("counts");

  TH1F * GBPIX[4];
  for(int l=0;l<4;l++)
    GBPIX[l] = new TH1F(TString("GBPIX_")+(long)l, "" , 8*BPIX_nLD[l], 0.5, 8*BPIX_nLD[l] + 0.5);

  TH1F * GFPIX[2][2];
  for(int p=0;p<2;p++)
    for(int r=0;r<2;r++){
      GFPIX[p][r] = new TH1F(TString("GFPIX_P")+(long)p+"_R"+(long)r, "" , 6*FPIX_nBLR[r], 0.5, 6*FPIX_nBLR[r] + 0.5);
    }


  int NBPIX[4]={0,0,0,0};//modules per layer
  int NFPIX[6]={0,0,0,0,0,0};//modules per disk
  

  //read the file
  ///Open the lumi 
  TString infile=Path+"/"+Run+".mod";
  cout<<"Reading :"<<infile<<endl;
  ifstream myfile (infile.Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<infile.Data()<<endl; 
    return;
  }
  std::string line;
  unsigned int mod=0;
  unsigned short counter=0;
  unsigned lumi=0.;
  //std::map<unsigned,unsigned> modcount;
  long modcount[NMOD];
  for(int m=0;m<NMOD;m++) modcount[m]=0;


  while (std::getline(myfile, line)){
    std::stringstream iss(line);
    std::string token;
    
    ///read the module id
    std::getline(iss,token,',');
    std::stringstream modentry(token);
    modentry>>mod;
    if(mod < 30000000) continue;//skip some lines
    
    ///sum the bcids
    for(int bx=0;bx<NBX;bx++){
      std::getline(iss,token,',');
      std::stringstream lumentry(token);
      lumentry>>lumi;
      modcount[counter] += lumi;      
      HModVsBX.SetBinContent(bx+1,counter+1,lumi);
    }    

    HMod.SetBinContent(counter+1,modcount[counter]);
    

    //BPIX modules
    if ( MD.find(mod) != MD.end() ){
      GBPIX[LY[mod]-1]->SetBinContent( BPIX_nLD[LY[mod]-1]*(MD[mod]-1) + LD[mod] , modcount[counter] );
      NBPIX[LY[mod]-1]++;
    }

   
    //FPIX modules
    if( SD.find(mod) != SD.end() ){
      int bin=FPIX_nBLR[RING[mod]] * DISK[mod] + (RING[mod]==0 ? BL[mod] : (BL[mod]-FPIX_nBLR[0]));
      GFPIX[PN[mod]-1][RING[mod]]->SetBinContent( bin , modcount[counter] );
      NFPIX[DISK[mod]]++;
    }
    
    counter++;
  }    
  ///close files
  myfile.close();
  
  cout<<"Total number of modules: "<<counter<<endl;
  cout<<"BPIX modules per layer:  "<<NBPIX[0]<<" , "<<NBPIX[1]<<" , "<<NBPIX[2]<<" , "<<NBPIX[3]<<endl;
  cout<<"FPIX modules per disk:  "<<NFPIX[0]<<" , "<<NFPIX[1]<<" , "<<NFPIX[2]<<" , "<<NFPIX[3]<<" , "<<NFPIX[4]<<" , "<<NFPIX[5]<<endl;

 
    
  /////////////////////////////////////////////////////
  ///   make the plots
  ///////////////////////////////////////////////////
  drawBPIX(GBPIX,"counts");
  drawFPIX(GFPIX,"counts");


  ///////////////////////////////////////////////////////////
//  C.Clear();
//  HModVsBX.Draw("colz");
//  C.Print(OutPath+"/plotPCCModules_ModVsBX.png");
//  C.Clear();
//  HMod.Draw("histp");
//  C.Print(OutPath+"/plotPCCModules_Mod.png");



  gROOT->ProcessLine(".q");
}

