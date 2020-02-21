#include <iostream>
#include <fstream>
#include <string>

#include "BRILAnalysisCode/PCCAnalysis/plots/globals.h"

TString Path="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018B_dynamicVeto_modules";

TString OutPath="/afs/cern.ch/user/b/benitezj/www/BRIL/PCC_lumi/DynamicVeto";


void plotPCCModules(long Run){
 
  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  readModCoordinates();


  ///Open the lumi 
  TString infile=Path+"/"+Run+".mod";
  ifstream myfile (infile.Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<infile.Data()<<endl; 
    return;
  }


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

  TH1F * GFPIX[2];
  for(int p=0;p<2;p++)
    GFPIX[p] = new TH1F(TString("GFPIX_")+(long)p, "" , 6*FPIX_nBL, 0.5, 6*FPIX_nBL + 0.5);
  

  //read the file
  std::string line;
  unsigned int mod=0;
  unsigned short counter=0;
  unsigned lumi=0.;
  unsigned int modcount[NMOD];
  unsigned int modid[NMOD];
  for(int m=0;m<NMOD;m++){
    modcount[m]=0;
    modid[m]=0;
  }

  while (std::getline(myfile, line)){
    
    std::stringstream iss(line);
    std::string token;
    
    ///read the module id
    std::getline(iss,token,',');
    std::stringstream modentry(token);
    modentry>>mod;
    if(mod < 30000000) continue;//skip 
    
    modid[counter] = mod;
    
    ///sum the bcids
    for(int bx=0;bx<NBX;bx++){
      std::getline(iss,token,',');
      std::stringstream lumentry(token);
      lumentry>>lumi;
      modcount[counter] += lumi;
      
      HModVsBX.SetBinContent(bx+1,counter,lumi);
    }
    
    HMod.SetBinContent(counter+1,modcount[counter]);
    

    //BPIX modules
    if ( MD.find(mod) != MD.end() ){
      GBPIX[LY[mod]-1]->SetBinContent( BPIX_nLD[LY[mod]-1]*(MD[mod]-1) + LD[mod] , modcount[counter] );
      //cout<<mod<<":"<<LY[mod]<<","<<MD[mod]<<","<<LD[mod]<<endl;
    }


    // //FPIX modules
    // if( SD.find(mod) != SD.end() ){
    //   unsigned disc = (SD[mod]==1?-1:1)*DI[mod] + 3;
    //   if( disc > 3 ) disc--;
    //   GFPIX[PN[mod]-1]->SetBinContent( FPIX_nBL * disc + BL[mod] , modcount[counter] );
    // }
    
    //cout<<mod<<endl;//<<" , "<<HModVsBX.GetBinContent(1,1)<<endl;
    counter++;
  }
    
  ///close files
  myfile.close();
    
    
  /////////////////////////////////////////////////////
  ///   make the plots
  ///////////////////////////////////////////////////
  gStyle->SetOptStat(0);  
  TCanvas C;
  TLine Line;
  Line.SetLineStyle(2);
  TLatex text;
  TCanvas C2("C2","",800,500);


  ///////////////////////////////////////////////////////////
  ///// BPIX
  C.Clear();
  HModVsBX.Draw("colz");
  C.Print(OutPath+"/plotPCCModules_ModVsBX.png");
  
  C.Clear();
  HMod.Draw("histp");
  C.Print(OutPath+"/plotPCCModules_Mod.png");
  
  for(long l=0;l<4;l++){
    C2.Clear();
    GBPIX[l]->SetTitle(TString("                                       Barrel Layer ")+l);
    GBPIX[l]->GetYaxis()->SetTitle("counts per module");
    GBPIX[l]->GetXaxis()->SetTitle("ladder");
    GBPIX[l]->GetXaxis()->SetNdivisions(0);
    GBPIX[l]->GetYaxis()->SetRangeUser(0,GBPIX[l]->GetMaximum());//*1.5);
    GBPIX[l]->SetMarkerSize(1);
    GBPIX[l]->Draw("histp");

    for(int m=0;m<8;m++){
      Line.DrawLine(BPIX_nLD[l]*m, 0, BPIX_nLD[l]*m, GBPIX[l]->GetMaximum()*1.);
      text.SetTextSize(0.035);
      if(m==0) text.DrawLatex(BPIX_nLD[l]*(m+0.5), GBPIX[l]->GetMaximum()*0.15, TString("z Module"));
      text.DrawLatex(BPIX_nLD[l]*(m+0.5), GBPIX[l]->GetMaximum()*0.1, TString("")+(long)(m>=4?m-3:m-4));
    }
    
    text.SetTextSize(0.04);
    text.DrawLatex(0,-GBPIX[l]->GetMaximum()*0.1,"1");
    text.DrawLatex(BPIX_nLD[l]/2.,-GBPIX[l]->GetMaximum()*0.1,"--");
    text.DrawLatex(BPIX_nLD[l],-GBPIX[l]->GetMaximum()*0.1,TString("")+(long)BPIX_nLD[l]+" ladder (#phi)");
    
    C2.Print(OutPath+"/plotPCCModules_BPIXModVsLadder_"+l+".png");
  }

  

  /////////////////////////////////////////
  /// FPIX
  for(int p=0;p<2;p++){
    C2.Clear();
    GFPIX[p]->SetTitle(TString("                                       FPIX  Panel ")+(long)(p+1));
    GFPIX[p]->GetYaxis()->SetTitle("counts per module");
    //GFPIX[p]->GetXaxis()->SetTitle("blade");
    GFPIX[p]->GetXaxis()->SetNdivisions(0);
    GFPIX[p]->GetYaxis()->SetRangeUser(0,GFPIX[p]->GetMaximum());
    GFPIX[p]->SetMarkerSize(1);
    GFPIX[p]->Draw("histp");

    text.SetTextSize(0.035);
    for(int d=0;d<6;d++){
      Line.DrawLine(FPIX_nBL*d, 0, FPIX_nBL*d, GFPIX[p]->GetMaximum()*1.);
      text.DrawLatex(FPIX_nBL*(d+0.5), GFPIX[p]->GetMaximum()*0.1, TString("")+(long)(d>=3?d-2:d-3));
    }
    text.DrawLatex(FPIX_nBL/2., GFPIX[p]->GetMaximum()*0.15, TString("z Disk "));

    text.SetTextSize(0.04);
    text.DrawLatex(0,-GFPIX[p]->GetMaximum()*0.1,"1");
    text.DrawLatex(FPIX_nBL/2.,-GFPIX[p]->GetMaximum()*0.1,"--");
    text.DrawLatex(FPIX_nBL,-GFPIX[p]->GetMaximum()*0.1,TString("")+(long)FPIX_nBL+" blade (#phi)");

    C2.Print(OutPath+"/plotPCCModules_FPIXModVsBlade_"+(long)p+".png");
  }

  gROOT->ProcessLine(".q");
}

