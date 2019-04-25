#include <iostream>
#include <fstream>
#include <string>

#define NLS 3000
float Lmin=0;
float Lmax=2;

float scale=(1e30/23.31)/1e34; //brilcalc lumi/lumisection [1/microbarn], 1 barn = 10^-24 cm^2,  y value will be in unites of 1e34

void plotLHCFill_LumiVsTime(TString Path,long FILL,long year=0, long month=0, long day=0){
  //provide: path, fill number, fill starting date
  

  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");

  ///Open the lumi csv file
  ifstream myfile (Path.Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<endl; 
    return;
  }

  TGraphErrors G;

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

    ///ling format: 324293 7270 2347 2347 10/09/18 14 04 56 STABLE BEAMS 6500 89792.454689641 66744.437627449 10.8 HFOC

    iss>>run>>fill>>ls>>ls>>date>>h>>m>>s>>tmp>>tmp>>tmp>>tmp>>L;
    if(hstart==-1||h<hstart){
      hstart=h;
      dcounter++;
    }

    G.SetPoint(counter,dcounter*24*3600+h*3600+m*60+s,L*scale);
    counter++;
  }
    
  ///close files
  myfile.close();
  
  /////////////////////////////////////////////////////
  ///   make the plot
  ///////////////////////////////////////////////////
  TCanvas C("C","",1000,600);

  G.GetXaxis()->SetTitle("Date Time                            ");
  G.GetXaxis()->SetLabelSize(0.04);
  G.GetXaxis()->SetTitleSize(0.05);
  G.GetXaxis()->SetTitleOffset(0.9);
  G.GetYaxis()->SetTitle("Inst. Luminosity [E34 cm^{-2}s^{-1}]");
  G.GetYaxis()->SetLabelSize(0.04);
  G.GetYaxis()->SetTitleSize(0.05);
  G.GetYaxis()->SetTitleOffset(1.0);
  G.GetYaxis()->SetRangeUser(Lmin,Lmax);
  G.SetMarkerStyle(8);
  G.SetMarkerSize(0.5);
  G.SetMarkerColor(4);


  TDatime dh(year,month,day,00,00,00);
  G.GetXaxis()->SetTimeDisplay(1);
  G.GetXaxis()->SetTimeFormat("%m/%d %H:%M");
  G.GetXaxis()->SetTimeOffset(dh.Convert());
 
  G.Draw("ap");

  
  TLatex text;
  text.SetTextColor(1);
  text.SetTextSize(0.05);
  text.DrawLatexNDC(0.5,0.85,TString("#font[62]{CMS} #font[52]{Preliminary ")+year+"}");
  text.SetTextColor(4);
  text.SetTextSize(0.045);
  text.DrawLatexNDC(0.25,0.94,TString("CMS Offline Luminosity, LHC Fill ")+(long)FILL+", #sqrt{s} = 13 TeV");
 
  C.Print(TString("fill")+FILL+".png");
  C.Print(TString("fill")+FILL+".pdf");

  gROOT->ProcessLine(".q");
}

