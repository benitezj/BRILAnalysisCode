#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"


void plotBPixVetoModules(string VetoList="veto_master_VdM_ABCD_2018_newcuts_BPix.txt"){

  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");

  //readModCoordinates();
  readModRPhiZCoordinates();

  //////////////////////////
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

  int BPIXNBAD=0;
  int BCountN[4]={0,0,0,0};
  int BCountP[4]={0,0,0,0};
  ifstream myfile(VetoList.c_str());
  if (!myfile.is_open()){
    std::cout << "Unable to open "<<endl;
    return;
  }
  std::string line;
  while (std::getline(myfile,line)){
    int module=atoi(line.c_str());
    if ( MD.find(module) == MD.end() ) continue;
    BPIXNBAD++;
    int md=MD[module]>4?MD[module]-4:MD[module]-5;
    int ld=LD[module]>bins[LY[module]-1]?LD[module]-bins[LY[module]-1]:LD[module]-bins[LY[module]-1]-1;
    //cout<<module<<" "<<LY[module]<<" "<<md<<" "<<ld<<endl;

    B[LY[module]-1].Fill(md,ld);
    if(md<0)BCountN[LY[module]-1]++;
    if(md>0)BCountP[LY[module]-1]++;
  }

  ////////////////////////////////
  ////FPIX
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

  int FPIXNBAD=0;
  int FCountN[2]={0,0};
  int FCountP[2]={0,0};
  int FCountDiskN[3]={0,0,0};
  int FCountDiskP[3]={0,0,0};
  while (std::getline(myfile_fpix,line)){
    int module=atoi(line.c_str());
    if( SD.find(module) == SD.end() ) continue;
    FPIXNBAD++;
    
    int di=(SD[module]==2?1:-1)*DI[module];
    int bl=BL[module]>28?BL[module]-28:BL[module]-29;
    //cout<<module<<" "<<di<<" "<<bl<<" "<<PN[module]<<endl;

    F[PN[module]-1].Fill(di,bl);
    if(di<0){
      FCountN[PN[module]-1]++;
      FCountDiskN[DI[module]-1]++;
    }
    if(di>0){
      FCountP[PN[module]-1]++;
      FCountDiskP[DI[module]-1]++;
    }
  }
  

  Int_t colors[] = {2}; // #colors >= #levels - 1
  gStyle->SetPalette((sizeof(colors)/sizeof(Int_t)), colors);
  Double_t levels[] = {-1.79e308,1.00,};

 
  TCanvas C;
  for(long l=0;l<4;l++){
    C.Clear();
    B[l].SetTitle(TString("BPIX Layer ")+(l+1));
    //B[l].GetZaxis()->SetRangeUser(0,0.5);
    B[l].SetContour((sizeof(levels)/sizeof(Double_t)), levels);
    B[l].Draw("col");
    C.Print(TString("VetoMap_Layer")+(l+1)+".png");
  }


  for(long l=0;l<2;l++){
    C.Clear();
    F[l].SetTitle(TString("FPIX ")+l);
    F[l].Draw("colz");
    C.Print(TString("VetoMap_FPIX")+(l+1)+".png");
  }




  //distribution of negative and positive side
  cout<<"Distibution of bad modules:"<<endl;
  cout<<"BPIX 0 : "<<BCountN[0]<<"N + "<<BCountP[0]<<"P  / "<<NBPIX[0]<<endl;
  cout<<"BPIX 1 : "<<BCountN[1]<<"N + "<<BCountP[1]<<"P  / "<<NBPIX[1]<<endl;
  cout<<"BPIX 2 : "<<BCountN[2]<<"N + "<<BCountP[2]<<"P  / "<<NBPIX[2]<<endl;
  cout<<"BPIX 3 : "<<BCountN[3]<<"N + "<<BCountP[3]<<"P  / "<<NBPIX[3]<<endl;
  cout<<"# BPIX total= "<<BPIXNBAD<<" / "<<NBPIX[0]+NBPIX[1]+NBPIX[2]+NBPIX[3]<<endl;  
  
  //cout<<"FPIX 0 : "<<FCountN[0]<<"N + "<<FCountP[0]<<"P "<<endl;
  //cout<<"FPIX 1 : "<<FCountN[1]<<"N + "<<FCountP[1]<<"P "<<endl;

  cout<<"FPIX Disk 1 : "<<FCountDiskN[0]<<"N + "<<FCountDiskP[0]<<"P "<<endl;
  cout<<"FPIX Disk 2 : "<<FCountDiskN[1]<<"N + "<<FCountDiskP[1]<<"P "<<endl;
  cout<<"FPIX Disk 3 : "<<FCountDiskN[2]<<"N + "<<FCountDiskP[2]<<"P "<<endl;
  
  cout<<"# FPIX total= "<<FPIXNBAD<<" / "<<NFPIX[0]+NFPIX[1]+NFPIX[2]+NFPIX[3]+NFPIX[4]+NFPIX[5]<<endl;  
}
