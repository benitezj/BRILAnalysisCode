#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"
#include "rootlogon.C"

float ymax=30000;
float ratiomin=0.95;
float ratiomax=1.05;
int MAXNLS = 20000;
int firstrun=999999;
int lastrun=1;

void getLumi(TString inputfile, std::map<long,float> & lumi){

  ifstream myfile(inputfile.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<inputfile.Data()<<std::endl;
    return;
  }

  std::string line;
  int run=0;
  int ls=0;
  string tmp;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);
    //cout<<line<<endl;
    iss>>run>>tmp>>ls;
    if(run<=100000 || run>=1000000 || ls<=0) continue;
    
    iss>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>lumi[run*10000+ls];
    
    //cout<<run<<" "<<ls<<" "<<lumi[run*10000+ls]<<endl;

    if(run<firstrun)firstrun=run;
    if(run>lastrun)lastrun=run;
  }

  cout<<"Done reading: "<<inputfile<<endl;
}



void compareBrilcalcLumis(TString path, TString file1, TString file2, TString file3){
  rootlogon();

  std::map<long,float> lumi1;
  std::map<long,float> lumi2;
  std::map<long,float> lumi3;
  
  getLumi(path+"/"+file1,lumi1);
  getLumi(path+"/"+file2,lumi2);
  getLumi(path+"/"+file3,lumi3);
  

  ///create histograms
  TH1F HL1("HL1","",MAXNLS,0.5,MAXNLS+0.5);
  TH1F HL2("HL2","",MAXNLS,0.5,MAXNLS+0.5);
  TH1F HL3("HL3","",MAXNLS,0.5,MAXNLS+0.5);
  TH1F HL12R("HL12R","",MAXNLS,0.5,MAXNLS+0.5);
  TH1F HL13R("HL13R","",MAXNLS,0.5,MAXNLS+0.5);

  TH1F H12R("H12R","",100,ratiomin,ratiomax);
  TH1F H13R("H13R","",100,ratiomin,ratiomax);

  
  float maxlumi=0.;
  float totlumi=0.;
  int ls=1;
  map<long,float>::iterator it;
  for (it = lumi1.begin(); it != lumi1.end(); it++){
    if(it->second==0) continue;
    
    HL1.SetBinContent(ls,it->second);

    HL2.SetBinContent(ls,lumi2[it->first]);
    HL12R.SetBinContent(ls,lumi2[it->first]/it->second);
    H12R.Fill(lumi2[it->first]/it->second,it->second);
    
    HL3.SetBinContent(ls,lumi3[it->first]);
    HL13R.SetBinContent(ls,lumi3[it->first]/it->second);
    H13R.Fill(lumi3[it->first]/it->second,it->second);

    
    if(it->second>maxlumi) maxlumi=it->second;
    totlumi+=it->second;
    ls++;
  }
  cout<<"Done processing input file"<<endl;

  cout<<"max lumi value: "<<maxlumi<<" ,  Average lumi value: "<<totlumi/ls<<endl;
  

  /////////////////////////////////////////////////////
  ///   make the plots
  ///////////////////////////////////////////////////
  gStyle->SetOptStat(0);
  TCanvas C;

  TPad can_1("can_1", "can_1", 0.0, 0.5, 1.0, 1.0);
  can_1.SetTopMargin(0.02);
  can_1.SetBottomMargin(0.15);
  can_1.SetLeftMargin(0.1);
  can_1.SetRightMargin(0.05);
  can_1.SetFrameBorderMode(0);

  can_1.cd();
  HL1.GetXaxis()->SetTitle("Lumisection");
  HL1.GetYaxis()->SetTitle("Inst. Lumi");
  HL1.GetXaxis()->SetLabelSize(0.05);
  HL1.GetXaxis()->SetTitleSize(0.07);
  HL1.GetXaxis()->SetTitleOffset(0.9);
  HL1.GetYaxis()->SetLabelSize(0.04);
  HL1.GetYaxis()->SetTitleSize(0.06);
  HL1.GetYaxis()->SetTitleOffset(0.6);
  HL1.GetXaxis()->SetRangeUser(0,ls);
  HL1.GetYaxis()->SetRangeUser(0,ymax);
  HL1.GetYaxis()->SetNdivisions(5);
  HL1.SetMarkerStyle(8);
  HL1.SetMarkerSize(0.3);
  HL1.Draw("histp");

  HL2.SetMarkerStyle(8);
  HL2.SetMarkerSize(0.3);
  HL2.SetMarkerColor(2);
  HL2.Draw("histpsame");

  HL3.SetMarkerStyle(8);
  HL3.SetMarkerSize(0.3);
  HL3.SetMarkerColor(4);
  HL3.Draw("histpsame");

  
  TLatex text;
  text.SetTextSize(0.06);
  text.DrawLatexNDC(0.25,0.30,TString("Runs: ")+firstrun+" - "+lastrun);
  text.SetTextColor(1);
  text.DrawLatexNDC(0.6,0.35,TString(file1.Data()).ReplaceAll(".dat",""));
  text.SetTextColor(2);
  text.DrawLatexNDC(0.6,0.30,TString(file2.Data()).ReplaceAll(".dat",""));
  text.SetTextColor(4);
  text.DrawLatexNDC(0.6,0.25,TString(file3.Data()).ReplaceAll(".dat",""));


 
  ///////Ratio plot
  TPad can_2("can_2", "can_2",0.0, 0.0, 1.0, 0.5);
  can_2.SetTopMargin(0.02);
  can_2.SetBottomMargin(0.15);
  can_2.SetLeftMargin(0.1);
  can_2.SetRightMargin(0.05);
  can_2.SetFrameBorderMode(0);
  can_2.cd();

  HL12R.GetXaxis()->SetTitle(HL1.GetXaxis()->GetTitle());
  HL12R.GetYaxis()->SetTitle(TString("ratio"));
  HL12R.GetYaxis()->SetRangeUser(ratiomin,ratiomax);
  HL12R.GetXaxis()->SetRangeUser(0,ls);
  
  HL12R.GetXaxis()->SetLabelSize(0.05);
  HL12R.GetXaxis()->SetTitleSize(0.07);
  HL12R.GetXaxis()->SetTitleOffset(0.9);
  HL12R.GetYaxis()->SetLabelSize(0.04);
  HL12R.GetYaxis()->SetTitleSize(0.06);
  HL12R.GetYaxis()->SetTitleOffset(0.6);
  HL12R.GetYaxis()->SetNdivisions(10);
  HL12R.SetMarkerStyle(8);
  HL12R.SetMarkerSize(0.3);
  HL12R.SetMarkerColor(2);
  HL12R.Draw("histp");

  HL13R.SetMarkerStyle(8);
  HL13R.SetMarkerSize(0.3);
  HL13R.SetMarkerColor(4);
  HL13R.Draw("histpsame");

  TLine tline;
  tline.SetLineColor(1);
  tline.DrawLine(0,1,ls,1);  

  
  C.Clear();
  can_1.Draw();
  can_2.Draw();
  C.Print("./compareBrilcalcLumis.png");


  C.Clear();
  H12R.GetXaxis()->SetTitle("ratio");
  H12R.GetYaxis()->SetTitle("lumi weighted entries");
  H12R.SetLineColor(2);
  H12R.Draw("hist");
  H13R.SetLineColor(4);
  H13R.Draw("histsame");
  text.SetTextSize(0.03);
  text.SetTextColor(2);
  text.DrawLatexNDC(0.6,0.85,TString(file2.Data()).ReplaceAll(".dat","")+"/"+TString(file1.Data()).ReplaceAll(".dat",""));
  text.DrawLatexNDC(0.6,0.82,TString("mean = 0.")+floor(10000*H12R.GetMean()));
  text.SetTextColor(4);
  text.DrawLatexNDC(0.6,0.78,TString(file3.Data()).ReplaceAll(".dat","")+"/"+TString(file1.Data()).ReplaceAll(".dat",""));
  text.DrawLatexNDC(0.6,0.75,TString("mean = 0.")+floor(10000*H13R.GetMean()));

  C.Print("./compareBrilcalcLumis_hist.png");
  


  gROOT->ProcessLine(".q");
}

