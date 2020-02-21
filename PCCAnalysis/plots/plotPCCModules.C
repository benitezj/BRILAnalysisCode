#include <iostream>
#include <fstream>
#include <string>

#include "globals.h"

TString Path="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_21Sep2018VdmVeto/Run2018B_dynamicVeto_modules";

TString OutPath="/afs/cern.ch/user/b/benitezj/www/BRIL/PCC_lumi/DynamicVeto";

unsigned int modcount[NMOD];
unsigned int modid[NMOD];

void plotPCCModules(long Run){
 
  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");

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

  // TH1F * GBPIX[4][8];///4 layers, and 8 z segments ("module")
  // for(int l=0;l<4;l++)
  //   for(int m=0;m<8;m++){
  //     GBPIX[l][m]=new TH1F(TString("GBPIX_")+(long)l+"_"+(long)m, "" , BPIX_nLD[l], 0, BPIX_nLD[l]);
  //     GBPIX[l][m]->SetLineColor(m);
  //     GBPIX[l][m]->SetMarkerColor(m);
  //   }


  TH1F * GBPIX[4];
  TH1F * GBPIXMod[4][8];
  for(int l=0;l<4;l++){
    GBPIX[l] = new TH1F(TString("GBPIX_")+(long)l, "" , BPIX_nLD[l]*8, 0.5, BPIX_nLD[l]*8 + 0.5);
    //GBPIX[l]->SetLineColor(m);
    //GBPIX[l]->SetMarkerColor(m);
    for(int m=0;m<8;m++)
      GBPIXMod[l][m] = new TH1F(TString("GBPIX_")+(long)l+"_"+(long)m, "" , BPIX_nLD[l], 0.5, BPIX_nLD[l] + 0.5);
  }
  
  //initialize
  for(int m=0;m<NMOD;m++){
    modcount[m]=0;
    modid[m]=0;
  }

  //read the file
  std::string line;
  unsigned int mod=0;
  unsigned short counter=0;
  unsigned lumi=0.;
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
    

    //determine if module is BPIX
    if ( MD.find(mod) != MD.end() ){
      GBPIXMod[LY[mod]-1][MD[mod]-1]->Fill( LD[mod] , modcount[counter] );
      GBPIX[LY[mod]-1]->SetBinContent( BPIX_nLD[LY[mod]-1]*(MD[mod]-1) + LD[mod] , modcount[counter] );

      //cout<<mod<<":"<<LY[mod]<<","<<MD[mod]<<","<<LD[mod]<<endl;
      //cout<<BPIX_nLD[LY[mod]-1]<<" "<<BPIX_nLD[LY[mod]-1]*(MD[mod]-1) + LD[mod]<<endl;

    }
    
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
  
  C.Clear();
  HModVsBX.Draw("colz");
  C.Print(OutPath+"/plotPCCModules_ModVsBX.png");
  
  C.Clear();
  HMod.Draw("histp");
  C.Print(OutPath+"/plotPCCModules_Mod.png");
  
  TLine Line;
  Line.SetLineStyle(2);
  TLatex text;
  text.SetTextSize(0.035);
  TCanvas C2("C2","",800,500);
  for(long l=0;l<4;l++){
    C2.Clear();
    GBPIX[l]->GetYaxis()->SetTitle("counts");
    GBPIX[l]->GetXaxis()->SetTitle("ladder");
    GBPIX[l]->GetYaxis()->SetRangeUser(0,GBPIX[l]->GetMaximum());//*1.5);
    GBPIX[l]->Draw("histp");
    for(int m=0;m<8;m++){
      Line.DrawLine(BPIX_nLD[l]*m, 0, BPIX_nLD[l]*m, GBPIX[l]->GetMaximum()*1.);
      text.DrawLatex(BPIX_nLD[l]*(m+0.5), GBPIX[l]->GetMaximum()*0.1, TString("")+(long)(m>=4?m-3:m-4));
    }
    C2.Print(OutPath+"/plotPCCModules_BPIXModVsLadder_"+l+".png");
  }

  
  for(long l=0;l<4;l++){
    C.Clear();
    C.Divide(2,4);
    for(int m=0;m<8;m++){
      C.cd(m+1);
      GBPIXMod[l][m]->GetYaxis()->SetRangeUser(0,GBPIXMod[l][m]->GetMaximum());
      GBPIXMod[l][m]->Draw("histp");
    }
    C.Print(OutPath+"/plotPCCModules_BPIXModVsLadder_single_"+l+".png");
  }

  

  gROOT->ProcessLine(".q");
}

