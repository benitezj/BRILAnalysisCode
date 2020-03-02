#include <iostream>
#include <fstream>
#include <string>

//#include "BRILAnalysisCode/PCCAnalysis/plots/globals.h"
#include "BRILAnalysisCode/PCCAnalysis/plots/plotModuleCoords.C"

void plotPCCModules(long Run){
 
  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");
  gStyle->SetOptStat(0);  

  TString Path="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018B_dynamicVeto_modules_v2";
  cout<<"Reading from: "<< Path<<endl;

  OutPath+=TString("/")+Run;
  system(TString("mkdir ")+OutPath);

  readModRPhiZCoordinates();
  readModVeto("BRILAnalysisCode/PCCAnalysis/test/veto_master_VdM_ABCD_2018_newcuts.txt");

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
  

  ///////////////////////////////////////////////
  TString infile=Path+"/"+Run+".mod";
  cout<<"Reading :"<<infile<<endl;
  ifstream myfile (infile.Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<infile.Data()<<endl; 
    return;
  }

  std::string line;
  unsigned mod=0;
  unsigned counter=0;
  long clusters=0;
  long totclusters=0;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);

    iss>>mod>>clusters;
    //cout<<mod<<" "<<clusters<<endl;

    if(mod < 30000000) continue;//skip wrong lines


    //here apply the veto list
    if(MODVETO[mod]) continue;


    //BPIX modules
    if ( MD.find(mod) != MD.end() ){
      GBPIX[LY[mod]-1]->SetBinContent( BPIX_nLD[LY[mod]-1]*(MD[mod]-1) + LD[mod] , clusters );
      NBPIX[LY[mod]-1]++;
    }
   
    //FPIX modules
    if( SD.find(mod) != SD.end() ){
      int bin=FPIX_nBLR[RING[mod]] * DISK[mod] + (RING[mod]==0 ? BL[mod] : (BL[mod]-FPIX_nBLR[0]));
      GFPIX[PN[mod]-1][RING[mod]]->SetBinContent( bin , clusters );
      NFPIX[DISK[mod]]++;
    }
    
    totclusters+=clusters;
    counter++;
  }    
  
  cout<<"Total number of modules: "<<counter<<endl;
  cout<<"BPIX modules per layer:  "<<NBPIX[0]<<" , "<<NBPIX[1]<<" , "<<NBPIX[2]<<" , "<<NBPIX[3]<<endl;
  cout<<"FPIX modules per disk:  "<<NFPIX[0]<<" , "<<NFPIX[1]<<" , "<<NFPIX[2]<<" , "<<NFPIX[3]<<" , "<<NFPIX[4]<<" , "<<NFPIX[5]<<endl;



  //////////////////////////////////////////////
  //// compute the module fractions
  ////////////////////////////////////////////
  std::ofstream ofile;
  ofile.open("./Module_fraction.txt",std::ios_base::out);
  if (!ofile.is_open())
    std::cout<<  " unable to create output file."<<endl;

  TH1F * GBPIXf[4];
  for(int l=0;l<4;l++){
    GBPIXf[l] = new TH1F(TString("GBPIXf_")+(long)l, "" , 8*BPIX_nLD[l], 0.5, 8*BPIX_nLD[l] + 0.5);
  }

  TH1F * GFPIXf[2][2];
  for(int p=0;p<2;p++)
    for(int r=0;r<2;r++){
      GFPIXf[p][r] = new TH1F(TString("GFPIXf_P")+(long)p+"_R"+(long)r, "" , 6*FPIX_nBLR[r], 0.5, 6*FPIX_nBLR[r] + 0.5);
    }

  myfile.clear();
  myfile.seekg (0, ios::beg);
  float fraction=0.;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);
    iss>>mod>>clusters;
    //cout<<mod<<", "<<clusters<<endl;
    if(mod < 30000000) continue;//skip wrong lines
    if(MODVETO[mod]) continue;

    fraction = clusters/(double)totclusters;

    if( MD.find(mod) != MD.end() )
      GBPIXf[LY[mod]-1]->SetBinContent( BPIX_nLD[LY[mod]-1]*(MD[mod]-1) + LD[mod] , 100*fraction );
   
    if( SD.find(mod) != SD.end() ){
      int bin=FPIX_nBLR[RING[mod]] * DISK[mod] + (RING[mod]==0 ? BL[mod] : (BL[mod]-FPIX_nBLR[0]));
      GFPIXf[PN[mod]-1][RING[mod]]->SetBinContent( bin , 100*fraction );
    }
   
    ofile<<mod<<" "<<fraction<<std::endl;
  }
  ofile.close();

    
  /////////////////////////////////////////////////////
  ///   make the plots
  ///////////////////////////////////////////////////
  drawBPIX(GBPIX,"counts");
  drawFPIX(GFPIX,"counts");
  drawBPIX(GBPIXf,"percent");
  drawFPIX(GFPIXf,"percent");


  ///close files
  myfile.close();
  gROOT->ProcessLine(".q");
}

