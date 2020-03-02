#include <iostream>
#include <fstream>
#include <string>

#include "BRILAnalysisCode/PCCAnalysis/plots/globals.h"

TString OutPath="/afs/cern.ch/user/b/benitezj/www/BRIL/PCC_lumi/DynamicVeto";

TLine Line;
TLatex text;
TCanvas Canvas("Canvas","",800,500);


void drawBPIX(TH1F * G[4], TString cord, float min=0.0, float max=0.0){
  Canvas.SetLeftMargin(0.15);

  for(long l=0;l<4;l++){
    Canvas.Clear();
    G[l]->SetTitle(TString("                                       BPIX Layer ")+l);
    G[l]->GetYaxis()->SetTitle(cord.Data());
    G[l]->GetYaxis()->SetTitleSize(0.05);
    G[l]->GetYaxis()->SetTitleOffset(0);
    G[l]->GetXaxis()->SetNdivisions(0);
    float ymin=G[l]->GetMinimum();
    float ymax=G[l]->GetMaximum();
    if(min!=0.0) ymin=min;
    if(max!=0.0) ymax=max;
    G[l]->GetYaxis()->SetRangeUser(ymin,ymax);
    G[l]->SetMarkerSize(1);
    G[l]->Draw("histp");
    //cout<<G[l]->GetMinimum()<<" "<<G[l]->GetMaximum()<<endl;

    float range=ymax-ymin;
    Line.SetLineStyle(2);
    text.SetTextSize(0.035);
    for(int m=0;m<8;m++){
      Line.DrawLine(BPIX_nLD[l]*m, ymin, BPIX_nLD[l]*m, ymax);
      text.DrawLatex(BPIX_nLD[l]*(m+0.5), ymin - range*0.05, TString("")+(long)(m>=4?m-3:m-4));
    }    
    text.DrawLatex(BPIX_nLD[l]*7.5, ymin - range*0.05, TString("   Module"));

    text.SetTextSize(0.04);
    text.DrawLatex(0, ymin - range*0.1,"1");
    text.DrawLatex(BPIX_nLD[l]/2., ymin - range*0.1,"--");
    text.DrawLatex(BPIX_nLD[l], ymin - range*0.1,TString("")+(long)BPIX_nLD[l]+" Ladder");
    
    Canvas.Print(OutPath+"/plotModuleCoords_BPIX_"+cord+"_L"+l+".png");
  }

}


void drawFPIX_0(TH1F ** G, TString cord){
  Canvas.SetLeftMargin(0.15);

  for(long p=0;p<2;p++){
    Canvas.Clear();
    G[p]->SetTitle(TString("                                       FPIX  Panel ")+(long)(p+1));
    G[p]->GetYaxis()->SetTitle(cord.Data());
    G[p]->GetYaxis()->SetTitleSize(0.05);
    G[p]->GetYaxis()->SetTitleOffset(0);
    G[p]->GetXaxis()->SetNdivisions(0);
    G[p]->GetYaxis()->SetRangeUser(G[p]->GetMinimum(),G[p]->GetMaximum());
    G[p]->SetMarkerSize(1);
    G[p]->Draw("histp");

    float range=G[p]->GetMaximum() - G[p]->GetMinimum();
    Line.SetLineStyle(2);
    text.SetTextSize(0.04);
    for(int d=0;d<6;d++){
      Line.DrawLine(FPIX_nBL*d,G[p]->GetMinimum(), FPIX_nBL*d, G[p]->GetMaximum());
      text.DrawLatex(FPIX_nBL*(d+0.5), G[p]->GetMinimum() - range*0.05, TString("")+(long)(d>=3?d-2:d-3));
    }
    text.DrawLatex(FPIX_nBL*5.5, G[p]->GetMinimum() - range*0.05, TString("   Disk "));

    text.SetTextSize(0.04);
    text.DrawLatex(0,G[p]->GetMinimum()-range*0.1,"1");
    text.DrawLatex(FPIX_nBL/2.,G[p]->GetMinimum()-range*0.1,"--");
    text.DrawLatex(FPIX_nBL,G[p]->GetMinimum()-range*0.1,TString("")+(long)FPIX_nBL+" Blade ");

    Canvas.Print(OutPath+"/plotModuleCoords_FPIX_"+cord+"_P"+p+".png");    
  }


}


void drawFPIX(TH1F * G[2][2], TString cord, float min=0.0, float max=0.0){
  Canvas.SetLeftMargin(0.15);

  for(long p=0;p<2;p++){
    for(long r=0;r<2;r++){
      Canvas.Clear();
      G[p][r]->SetTitle(TString("                                       FPIX  Panel ")+(p+1)+" Ring "+(r+1));
      G[p][r]->GetYaxis()->SetTitle(cord.Data());
      G[p][r]->GetYaxis()->SetTitleSize(0.05);
      G[p][r]->GetYaxis()->SetTitleOffset(0);
      G[p][r]->GetXaxis()->SetNdivisions(0);
      float ymin=G[p][r]->GetMinimum();
      float ymax=G[p][r]->GetMaximum();
      if(min!=0.0) ymin=min;
      if(max!=0.0) ymax=max;
      G[p][r]->GetYaxis()->SetRangeUser(ymin,ymax);
      G[p][r]->SetMarkerSize(1);
      G[p][r]->Draw("histp");
      
      float range=ymax-ymin;
      Line.SetLineStyle(2);
      text.SetTextSize(0.04);
      for(int d=0;d<6;d++){
	Line.DrawLine(FPIX_nBLR[r]*d, ymin , FPIX_nBLR[r]*d, ymax);
	text.DrawLatex(FPIX_nBLR[r]*(d+0.5), ymin - range*0.05, TString("")+(long)(d>=3?d-2:d-3));
      }
      text.DrawLatex(FPIX_nBLR[r]*5, ymin - range*0.1, TString("Disk"));
      
      text.SetTextSize(0.04);
      text.DrawLatex(0, ymin - range*0.1,TString("")+(long)(r==0 ? 1 : FPIX_nBLR[0]+1));
      text.DrawLatex(FPIX_nBLR[r]/2., ymin - range*0.1,"--");
      text.DrawLatex(FPIX_nBLR[r], ymin - range*0.1,TString("")+(long)(r==0 ? FPIX_nBLR[0] : FPIX_nBL)+" Blade");
      
      Canvas.Print(OutPath+"/plotModuleCoords_FPIX_"+cord+"_P"+p+"_R"+r+".png");    
    }
  }


}

void plotModuleCoords(){
 
  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  //readModCoordinates();
  readModRPhiZCoordinates();


  TH1F * GBPIX[4];
  TH1F * GBPIX_PHI[4];
  TH1F * GBPIX_Z[4];
  for(int l=0;l<4;l++){
    GBPIX[l] = new TH1F(TString("GBPIX_")+(long)l, "" , 8*BPIX_nLD[l], 0.5, 8*BPIX_nLD[l] + 0.5);
    GBPIX_PHI[l] = new TH1F(TString("GBPIX_PHI_")+(long)l, "" , 8*BPIX_nLD[l], 0.5, 8*BPIX_nLD[l] + 0.5);
    GBPIX_Z[l] = new TH1F(TString("GBPIX_Z_")+(long)l, "" , 8*BPIX_nLD[l], 0.5, 8*BPIX_nLD[l] + 0.5);
  }

  TH1F * GFPIX[2][2];//Panel 1,2; Ring 1,2
  TH1F * GFPIX_PHI[2][2];
  TH1F * GFPIX_Z[2][2];
  for(int p=0;p<2;p++)
    for(int r=0;r<2;r++){
      GFPIX[p][r] = new TH1F(TString("GFPIX_P")+(long)p+"_R"+(long)r, "" , 6*FPIX_nBLR[r], 0.5, 6*FPIX_nBLR[r] + 0.5);
      GFPIX_PHI[p][r] = new TH1F(TString("GFPIX_PHI_")+(long)p+"_R"+(long)r, "" , 6*FPIX_nBLR[r], 0.5, 6*FPIX_nBLR[r] + 0.5);
      GFPIX_Z[p][r] = new TH1F(TString("GFPIX_Z_")+(long)p+"_R"+(long)r, "" , 6*FPIX_nBLR[r], 0.5, 6*FPIX_nBLR[r] + 0.5);
    }

  /////////////
  ifstream cfile_bpix("BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_BPix_raw.txt");
  if (!cfile_bpix.is_open()){
    std::cout << "Unable to open coordinates"<<endl;
    return;
  }
  std::string cline_bpix;
  while (std::getline(cfile_bpix,cline_bpix)){
    std::stringstream iss(cline_bpix);
    long mod;
    iss>>mod;
    GBPIX[LY[mod]-1]->SetBinContent( BPIX_nLD[LY[mod]-1]*(MD[mod]-1) + LD[mod] , BPIX_R[mod]);
    GBPIX_PHI[LY[mod]-1]->SetBinContent( BPIX_nLD[LY[mod]-1]*(MD[mod]-1) + LD[mod] , BPIX_PHI[mod]);
    GBPIX_Z[LY[mod]-1]->SetBinContent( BPIX_nLD[LY[mod]-1]*(MD[mod]-1) + LD[mod] , BPIX_Z[mod]);
  }


  ifstream cfile_fpix("BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_FPix_raw.txt");
  if (!cfile_fpix.is_open()){
    std::cout << "Unable to open coordinates"<<endl;
    return;
  }
  std::string cline_fpix;
  while (std::getline(cfile_fpix,cline_fpix)){
    std::stringstream iss(cline_fpix);
    long mod; iss>>mod;
    int bin=FPIX_nBLR[RING[mod]] * DISK[mod] + (RING[mod]==0 ? BL[mod] : (BL[mod]-FPIX_nBLR[0]));
    GFPIX[PN[mod]-1][RING[mod]]->SetBinContent( bin , FPIX_R[mod] );
    GFPIX_PHI[PN[mod]-1][RING[mod]]->SetBinContent( bin, FPIX_PHI[mod] );
    GFPIX_Z[PN[mod]-1][RING[mod]]->SetBinContent( bin , FPIX_Z[mod] );
  }


  
  /////////////////////////////////////////////////////
  ///   make the plots
  ///////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////
  ///// BPIX
  drawBPIX(GBPIX,"radius");
  drawBPIX(GBPIX_PHI,"phi");
  drawBPIX(GBPIX_Z,"z");
  
  
  /////////////////////////////////////////
  /// FPIX
  drawFPIX(GFPIX,"radius");
  drawFPIX(GFPIX_PHI,"phi");
  drawFPIX(GFPIX_Z,"z");
  

  gROOT->ProcessLine(".q");
}

