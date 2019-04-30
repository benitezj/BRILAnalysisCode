#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"

std::vector<std::string> dets={"hfoc","ramses","pltzero","hfet","pcc"};
std::vector<std::string> detnames={"HFOC","RAMSES","PLT","HFET","PCC"};
std::vector<int> colors={0,4,3,6,2};


TH1F * readDetector(TString Path, long RUN, TString det){

  ///Open the lumi csv file
  ifstream myfile ((Path+"/"+RUN+"_"+det+".txt").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<endl; 
    return 0;
  }

  TH1F* H = new TH1F(TString("Lumi")+det,"",NLS,0.5,NLS+0.5);

  std::string line;
  int run=0;
  int ls=0;
  int fill=0;
  double L=0;//lumi for given LS
  int h,m,s;
  int hstart=-1;
  int dcounter=-1;
  int counter=0;
  char date[100];
  char tmp[100];
  while (std::getline(myfile, line)){
    std::stringstream iss(line);

    ///line format: 324293 7270 2347 2347 10/09/18 14 04 56 STABLE BEAMS 6500 89792.454689641 66744.437627449 10.8 HFOC

    iss>>run>>fill>>ls>>ls>>date>>h>>m>>s>>tmp>>tmp>>tmp>>L;
    if(hstart==-1||h<hstart){
      hstart=h;
      dcounter++;
    }
    H->SetBinContent(ls,L*InstLumiScale);

  }
    
  ///close files
  myfile.close();

  return H;
}


void plotLHCFill_Linearity(TString Path, long RUN, long FILL,long year){
  //provide: path, fill number, fill starting date
  
  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  std::vector<TH1F*> lumi;
  std::vector<TGraph*> ratio;
  for(int i=0;i<dets.size();i++){
    lumi.push_back(readDetector(Path,RUN,dets[i]));

    TGraph * g=new TGraph();
    int counter=0;
    float Avg=0.;
    int AvgCount=0;
    float AvgX=0.;
    for(int b=1;b<NLS;b++){
      if(lumi[0]->GetBinContent(b)<0.4)continue;
      if(fabs(lumi[i]->GetBinContent(b)/lumi[0]->GetBinContent(b) - 1)>0.05) continue;

      AvgX += lumi[0]->GetBinContent(b);
      Avg += lumi[i]->GetBinContent(b)/lumi[0]->GetBinContent(b);
      AvgCount++;
      if(AvgCount<30)continue;

      g->SetPoint(counter,AvgX/AvgCount,Avg/AvgCount); //lumi[i]->GetBinContent(b)/lumi[0]->GetBinContent(b));
      counter++;
      AvgX=0.;
      Avg=0.;
      AvgCount=0;
    }

    g->SetMarkerSize(1.2);
    g->SetMarkerColor(colors[i]);
    g->SetLineColor(colors[i]);
    ratio.push_back(g);
  }

  
  /////////////////////////////////////////////////////
  ///   make the plot
  ///////////////////////////////////////////////////
  TCanvas C("C","",800,600);

  ratio[0]->GetXaxis()->SetTitle(InstLumiAxisTitle.Data());
  ratio[0]->GetXaxis()->SetLabelSize(0.04);
  ratio[0]->GetXaxis()->SetTitleSize(0.05);
  ratio[0]->GetXaxis()->SetTitleOffset(0.9);
  ratio[0]->GetXaxis()->SetRangeUser(0,2);
  ratio[0]->GetYaxis()->SetTitle("Inst. Luminosity Ratio");
  ratio[0]->GetYaxis()->SetLabelSize(0.04);
  ratio[0]->GetYaxis()->SetTitleSize(0.05);
  ratio[0]->GetYaxis()->SetTitleOffset(1.0);
  ratio[0]->GetYaxis()->SetRangeUser(0.94,1.06);
  ratio[0]->SetMarkerStyle(0);
  ratio[0]->SetMarkerSize(0.0);
  ratio[0]->Draw("ap");


  TLegend leg(0.65,0.65,.92,0.88);
  leg.SetLineColor(0);
  leg.SetLineStyle(0);
  leg.SetShadowColor(0);
  leg.SetFillColor(0);
  leg.SetFillStyle(0);
  //leg.SetMarkerSize(2);

  for(int i=1;i<dets.size();i++){
    if(!lumi[i]){cout<<dets[i]<<" is null"<<endl; return;}
    ratio[i]->Draw("psame");
    lumi[i]->SetMarkerSize(1);//control marker shown in lengend
    lumi[i]->SetMarkerColor(colors[i]);
    leg.AddEntry(lumi[i],(detnames[i]+" / "+detnames[0]).c_str(),"p");
  }
  

  leg.Draw();
  drawLumiTitle(FILL);
  drawCMSPrelim(year);

  C.Print(Path+"/linearity_"+FILL+".png");
  C.Print(Path+"/linearity_"+FILL+".pdf");
  
  gROOT->ProcessLine(".q");
}

