#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "globals.h"
#include "rootlogon.C"

//plot ranges
float ymax=400000;
float ratiomin=0.96;
float ratiomax=1.04;
int MAXNLS = 230000;//213848
int firstrun=999999;
int lastrun=1;

///selections
int firstLS=0;
int lastLS=220000;
//int ncolliding=2345;


void getLumi(TString inputfile, std::map<long,float> & lumi){

  ifstream myfile(inputfile.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<inputfile.Data()<<std::endl;
    return;
  }

  std::string line;
  long run=0;
  long ls=0;
  string tmp;
  int nls=0;
  while (std::getline(myfile, line)){
    TString LINE(line.c_str());
    LINE.ReplaceAll(":"," ");
    LINE.ReplaceAll(","," ");

    std::stringstream iss(LINE.Data());
    iss>>run>>tmp>>ls;

    if(run<=100000 || run>=1000000 || ls<=0 || ls>10000) continue;

    //format: brilcalc lumi --begin 297046 --end 306462 --type=pxl --normtag pcc17v8 --byls -b "STABLE BEAMS" --output-style csv
    //297046:5839,34:34,06/16/17 19:30:22,STABLE BEAMS,6500,72415.375054791,66219.819217254,18.2,PXL
    iss>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>lumi[run*10000+ls];
    
    //cout<<run<<"="<<ls<<"="<<run*10000+ls<<":"<<lumi[run*10000+ls]<<endl;    
    if(run<firstrun)firstrun=run;
    if(run>lastrun)lastrun=run;
    nls++;
  }

  cout<<"Done reading: "<<inputfile<<", NLS="<<nls<<endl;
}


void getLumiPCC(TString inputfile, std::map<long,float> & lumi){

  ifstream myfile(inputfile.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<inputfile.Data()<<std::endl;
    return;
  }

  std::string line;
  long run=0;
  long ls=0;
  string tmp;
  int nls=0;
  while (std::getline(myfile, line)){
    std::istringstream iss(line);
    std::getline(iss, tmp, ','); run = atoi(tmp.c_str());
    std::getline(iss, tmp, ','); ls = atoi(tmp.c_str());
    long idx=run*10000+ls;
    std::getline(iss, tmp, ','); lumi[idx] = atof(tmp.c_str()) / getSigmaVis(run);
    //cout<<"="<<run<<"="<<ls<<"="<<idx<<":"<<lumi[idx]<<endl;
    if(run<=100000 || run>=1000000 || ls<=0) continue;
    if(run<firstrun)firstrun=run;
    if(run>lastrun)lastrun=run;
    nls++;
  }

  cout<<"Done reading: "<<inputfile<<", NLS="<<nls<<endl;
}



void compareBrilcalcLumis(TString path, TString file1, TString file2, TString file3=""){
  rootlogon();

  std::map<long,float> lumi1;
  std::map<long,float> lumi2;
  std::map<long,float> lumi3;

  
  getLumi(path+"/"+file1,lumi1); //  //getLumiPCC(path+"/"+file1,lumi1);
  getLumi(path+"/"+file2,lumi2);
  if(file3.CompareTo("")!=0) getLumi(path+"/"+file3,lumi3);
  

  ///create histograms
  TH1F HL1("HL1","",MAXNLS,0.5,MAXNLS+0.5);
  TH1F HL2("HL2","",MAXNLS,0.5,MAXNLS+0.5);
  TH1F HL3("HL3","",MAXNLS,0.5,MAXNLS+0.5);
  TH1F HL12R("HL12R","",MAXNLS,0.5,MAXNLS+0.5);
  TH1F HL13R("HL13R","",MAXNLS,0.5,MAXNLS+0.5);

  TH1F H12R("H12R","",100,ratiomin,ratiomax);
  TH1F H13R("H13R","",100,ratiomin,ratiomax);

  //TGraphErrors G12R_vsLumi;
  //TGraphErrors G13R_vsLumi;
  
  
  float maxlumi=0.;
  float totlumi=0.;
  int ls=1;
  float sbil=0;
  map<long,float>::iterator it;
  for (it = lumi1.begin(); it != lumi1.end(); it++){
    if(it->second<=0) continue;
    ls++;
    
    if(ls<firstLS || ls>lastLS) continue;


    //cout<<it->first<<" "<<lumi2[it->first]<<" "<<it->second<<endl;
    HL1.SetBinContent(ls,it->second);
    HL2.SetBinContent(ls,lumi2[it->first]);
    HL12R.SetBinContent(ls,lumi2[it->first]/it->second);
    H12R.Fill(lumi2[it->first]/it->second,it->second);
    //G12R_vsLumi.SetPoint(G12R_vsLumi.GetN(),sbil,lumi2[it->first]/it->second);
    
    if(file3.CompareTo("")!=0){
      HL3.SetBinContent(ls,lumi3[it->first]);
      HL13R.SetBinContent(ls,lumi3[it->first]/it->second);
      H13R.Fill(lumi3[it->first]/it->second,it->second);
      //G13R_vsLumi.SetPoint(G13R_vsLumi.GetN(),sbil,lumi3[it->first]/it->second);
    }
    
    if(it->second>maxlumi) maxlumi=it->second;
    totlumi+=it->second;
    
  }
  cout<<"Done processing input file"<<endl;

  cout<<"max lumi value: "<<maxlumi<<" ,  Average lumi value: "<<totlumi/ls<<endl;
  

  /////////////////////////////////////////////////////
  ///   make the plots
  ///////////////////////////////////////////////////
  gStyle->SetOptStat(0);
  TCanvas C;

  TPad can_1("can_1", "can_1", 0.0, 0.5, 1.0, 1.0);
  can_1.SetTopMargin(0.08);
  can_1.SetBottomMargin(0.15);
  can_1.SetLeftMargin(0.1);
  can_1.SetRightMargin(0.05);
  can_1.SetFrameBorderMode(0);

  can_1.cd();
  HL1.GetXaxis()->SetTitle("lumisection");
  HL1.GetYaxis()->SetTitle("lumi [1/ub]");
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
  HL1.SetMarkerColor(4);
  HL1.Draw("histp");

  HL2.SetMarkerStyle(8);
  HL2.SetMarkerSize(0.3);
  HL2.SetMarkerColor(2);
  HL2.Draw("histpsame");

  HL3.SetMarkerStyle(8);
  HL3.SetMarkerSize(0.3);
  HL3.SetMarkerColor(3);
  if(file3.CompareTo("")!=0) HL3.Draw("histpsame");

  
  TLatex text;
  text.SetTextSize(0.06);
  text.DrawLatexNDC(0.15,0.95,TString("Runs: ")+firstrun+" - "+lastrun);
  text.SetTextColor(4);
  text.DrawLatexNDC(0.4,0.95,TString(file1.Data()).ReplaceAll(".csv",""));
  text.SetTextColor(2);
  text.DrawLatexNDC(0.7,0.95,TString(file2.Data()).ReplaceAll(".csv",""));
  text.SetTextColor(3);
  if(file3.CompareTo("")!=0)text.DrawLatexNDC(0.4,0.88,TString(file3.Data()).ReplaceAll(".csv",""));


 
  ///////Ratio plot
  TPad can_2("can_2", "can_2",0.0, 0.0, 1.0, 0.5);
  can_2.SetTopMargin(0.06);
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
  HL12R.SetMarkerColor(1);
  HL12R.Draw("histp");

  HL13R.SetMarkerStyle(8);
  HL13R.SetMarkerSize(0.3);
  HL13R.SetMarkerColor(5);
  if(file3.CompareTo("")!=0)HL13R.Draw("histpsame");

  TLine tline;
  tline.SetLineColor(1);
  tline.DrawLine(0,1,ls,1);  

  char txt12[100];  
  sprintf(txt12,"%s ",(TString("")+file2.ReplaceAll(".csv","")+" / "+TString(file1.Data()).ReplaceAll(".csv","")).Data());
  text.SetTextColor(1);
  text.DrawLatexNDC(0.15,0.95,txt12);

  if(file3.CompareTo("")!=0){
    char txt13[100];  
    sprintf(txt13,"%s ",(TString("")+file3.ReplaceAll(".csv","")+" / "+TString(file1.Data()).ReplaceAll(".csv","")).Data());
    text.SetTextColor(5);
    text.DrawLatexNDC(0.5,0.95,txt13);
  }
  
  TString ofilename=TString("")+file1.ReplaceAll(".csv","")+"_"+TString(file2.Data()).ReplaceAll(".csv","");
  if(file3.CompareTo("")!=0) ofilename=ofilename+"_"+TString(file3.Data()).ReplaceAll(".csv","");
  C.Clear();
  can_1.Draw();
  can_2.Draw();
  C.Print(ofilename+".png");


  C.Clear();
  H12R.GetXaxis()->SetTitle("ratio");
  H12R.GetYaxis()->SetTitle("lumi [1/ub]");
  H12R.SetLineColor(1);
  H12R.Draw("hist");
  H13R.SetLineColor(5);
  if(file3.CompareTo("")!=0)H13R.Draw("histsame");

  text.SetTextSize(0.03);
  text.SetTextColor(1);
  text.DrawLatexNDC(0.2,0.85,TString("Runs: ")+firstrun+" - "+lastrun);

  text.SetTextColor(1);
  char txt12H[100];  
  sprintf(txt12H,"%s",(TString("")+file2.ReplaceAll(".csv","")+" / "+TString(file1.Data()).ReplaceAll(".csv","")).Data());
  text.DrawLatexNDC(0.6,0.85,txt12H);
  sprintf(txt12H,"#mu=%.3f, #sigma=%.4f",H12R.GetMean(),H12R.GetRMS());  
  text.DrawLatexNDC(0.6,0.80,txt12H);


  if(file3.CompareTo("")!=0){
    char txt13H[100];
    text.SetTextColor(5);
    sprintf(txt13H,"%s",(TString("")+file3.ReplaceAll(".csv","")+" / "+TString(file1.Data()).ReplaceAll(".csv","")).Data());
    text.DrawLatexNDC(0.6,0.75,txt13H);
    sprintf(txt13H,"#mu=%.3f, #sigma=%.4f",H13R.GetMean(),H13R.GetRMS());
    text.DrawLatexNDC(0.6,0.70,txt13H);
  }
  C.Print(ofilename+"_hist.png");



//  //Linearity: ratio vs lumi
//  C.Clear();
//  G12R_vsLumi.SetMarkerStyle(8);
//  G12R_vsLumi.SetMarkerSize(0.3);
//  G12R_vsLumi.SetMarkerColor(2);
//  G12R_vsLumi.GetYaxis()->SetTitle("Ratio");
//  G12R_vsLumi.GetXaxis()->SetTitle("SBIL");
//  G12R_vsLumi.GetYaxis()->SetRangeUser(ratiomin,ratiomax);
//  G12R_vsLumi.Draw("ap");
//
//  TF1 FLin12("FLin12","[0]+[1]*x",0,12);
//  FLin12.SetLineColor(2);
//  G12R_vsLumi.Fit(&FLin12);
//
//  char txt[100];
//  sprintf(txt,"-- %s: p0 = %.3f,   p1=%.5f",(TString("")+file2.ReplaceAll(".csv","")+"/"+TString(file1.Data()).ReplaceAll(".csv","")).Data(), FLin12.GetParameter(0),FLin12.GetParameter(1));
//  text.SetTextColor(2);
//  text.DrawLatexNDC(0.4,0.97,txt);
//
//  text.SetTextColor(1);
//  text.DrawLatexNDC(0.1,0.98,TString("Runs: ")+firstrun+" - "+lastrun);
//  
//  if(file3.CompareTo("")!=0){
//    G13R_vsLumi.SetMarkerStyle(8);
//    G13R_vsLumi.SetMarkerSize(0.3);
//    G13R_vsLumi.SetMarkerColor(4);
//    G13R_vsLumi.Draw("psame");
//
//    TF1 FLin13("FLin13","[0]+[1]*x",0,12);
//    FLin13.SetLineColor(4);
//    G13R_vsLumi.Fit(&FLin13);
//
//    sprintf(txt,"-- %s: p0 = %.3f,   p1 = %.5f",(TString("")+file3.ReplaceAll(".csv","")+"/"+TString(file1.Data()).ReplaceAll(".csv","")).Data(),FLin13.GetParameter(0),FLin13.GetParameter(1));
//    text.SetTextColor(4);
//    text.DrawLatexNDC(0.4,0.93,txt);
//  }
//  C.Print(ofilename+"_lin.png");


  gROOT->ProcessLine(".q");
}

