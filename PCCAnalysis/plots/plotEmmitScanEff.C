#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"
#include "rootlogon.C"

float xoffset=0;//41.536;

void plotEmmitScanEff(TString DetName="HFET", TString OutPath="/eos/user/b/benitezj/www/plots/BRIL/"){
  rootlogon();
  
  ifstream myfile ((DetName+".dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<endl; 
    return;
  }

  TGraphErrors G;

  std::string line;
  float Lint;
  char tmpstr[100];
  float tmp;
  float eff;
  float eff_err;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);
    //41.536 1.025042196     1.033048942
    iss>>Lint>>eff>>eff_err;

    if(Lint<=0.1)continue;
    //cout<<Lint<<" "<<eff<<endl;
    Lint-=xoffset;
    G.SetPoint(G.GetN(),Lint,eff);
    G.SetPointError(G.GetN()-1,0,eff_err);
  }
   

  ///close files
  myfile.close();
  
  /////////////////////////////////////////////////////
  ///   make the plot
  ///////////////////////////////////////////////////
  TCanvas C("C","",800,600);

  G.GetXaxis()->SetTitle("Run 3 Integrated Luminosity [1/fb]");
  G.GetXaxis()->SetLabelSize(0.04);
  G.GetXaxis()->SetTitleSize(0.05);
  G.GetXaxis()->SetTitleOffset(0.9);
  G.GetXaxis()->SetNdivisions(4);
  //G.GetYaxis()->SetTitle("Efficiency");
  G.GetYaxis()->SetTitle("#sigma_{vis}^{Emitt.} / #sigma_{vis}^{vdM}");
  G.GetYaxis()->SetLabelSize(0.04);
  G.GetYaxis()->SetTitleSize(0.05);
  G.GetYaxis()->SetTitleOffset(1.0);
  G.GetYaxis()->SetRangeUser(0.97,1.07);
  //G.GetXaxis()->SetRangeUser(-10,35);
  G.SetMarkerStyle(8);
  G.SetMarkerSize(0.7);
  G.SetMarkerColor(1);


  G.Draw("ape");

  TLine Line;
  Line.SetLineStyle(2);
  Line.DrawLine(40,1,75,1);
  
  drawCMSPrelim();
  TLatex text;
  text.SetTextColor(1);
  text.SetTextSize(0.030);
  text.DrawLatexNDC(0.75,0.94,"#font[62]{13.6 TeV  (2023)}");
  text.DrawLatexNDC(0.20,0.80,TString("")+"#font[62]{"+DetName+"}");


  TLegend leg(0.55,0.65,0.87,0.89);
  leg.SetFillColor(0);
  leg.SetFillStyle(0);
  leg.SetLineColor(0);
  leg.SetLineStyle(0);
  leg.AddEntry(&G,"Emmitt. Scan Data","pe");

  TF1 *F1=NULL;TF1* F2=NULL;TF1* F3=NULL;TF1* F4=NULL;
  
  if(DetName.CompareTo("HFET")==0){
    F1=new TF1("F1","(1/0.992249403)*(1.05-x*0.0007080)",40,50);
    F1->SetLineColor(4);
    F1->Draw("lsame");
    leg.AddEntry(F1,"y = 1.0582 - x*0.000714","l");
    
    F2=new TF1("F2","(1/0.992249403)*(1.037418039-x*0.0007080)",50,75);
    F2->SetLineColor(2);
    F2->Draw("lsame");
    leg.AddEntry(F2,"y = 1.0455 - x*0.000714","l");
  }

  if(DetName.CompareTo("BCM1FuTCA")==0){ 
    F1=new TF1("F1","(1.09094-x*0.00167)",40,50);
    F1->SetLineColor(4);
    F1->Draw("lsame");
    leg.AddEntry(F1,"y = 1.09094 - x*0.00167","l");
    
    F2=new TF1("F2","(1.04038-x*0.000633)",50,62);
    F2->SetLineColor(2);
    F2->Draw("lsame");
    leg.AddEntry(F2,"y = 1.04038 - x*0.000633","l");
    
    F3=new TF1("F3","(1.1750-x*0.00266)",62,73);
    F3->SetLineColor(3);
    F3->Draw("lsame");
    leg.AddEntry(F3,"y = 1.1750 - x*0.00266","l");
    
    F4=new TF1("F4","1.016015281",73,74);
    F4->SetLineColor(6);
    F4->Draw("lsame");
    leg.AddEntry(F4,"y = 1.016","l");
  }

  //
  //
  if(DetName.CompareTo("PLT")==0){
    F1=new TF1("F1","(-x*0.0012194+1.06254438)/1.011462654",45,63);
    F1->SetLineColor(4);
    F1->Draw("lsame");
    leg.AddEntry(F1,"y = 1.0505 - x*0.001205","l");

    F2=new TF1("F2","(-x*0.0010541+1.078039)/1.01146265",63,75);
    F2->SetLineColor(2);
    F2->Draw("lsame");
    leg.AddEntry(F2,"y = 1.0658 - x*0.001042","l");
  }
  
  leg.Draw();
  
  C.Print(OutPath+"/"+DetName+"_EmmittScanEff.png");

  gROOT->ProcessLine(".q");
}

