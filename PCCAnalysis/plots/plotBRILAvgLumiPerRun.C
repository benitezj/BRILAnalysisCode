
#include <iostream>
#include <fstream>

int firstrun=360390;
int lastrun=362167;

void plotBRILAvgLumiPerRun(){

  TH1F HR("HR","TotalLumi per Run",(lastrun-firstrun)+1,firstrun-0.5,lastrun+0.5);
  TH1F HRN("HRN","LS's per Run",(lastrun-firstrun)+1,firstrun-0.5,lastrun+0.5);
  
  ifstream data("Run2022F.dat");
  std::string line;
  int run=0;
  int ls=0;
  double totL=0;  
  while (std::getline(data, line)){
    
    std::stringstream iss(line);
    iss>>run>>ls>>totL;

    HR.Fill(run,totL);
    HRN.Fill(run);
  }

  
  TCanvas C;
  C.Clear();
  C.SetLogy(1);
  HR.SetMarkerStyle(8);
  HR.SetMarkerSize(1.);
  HR.SetStats(0);
  HR.Draw("histp");
  C.Print("plotBRILAvgLumiPerRun_totL.png");

  C.Clear();
  C.SetLogy(0);
  HRN.Draw("hist");
  C.Print("plotBRILAvgLumiPerRun_NLS.png");

  HR.Divide(&HRN);
  HR.SetTitle("Avg Lumi");
  C.Clear();
  C.SetLogy(1);
  HR.Draw("histp");
  C.Print("plotBRILAvgLumiPerRun_AvgL.png");

  for(int i=1;i<=HR.GetNbinsX();i++){
    if(HR.GetBinContent(i)>10000)
      //cout<<HR.GetBinCenter(i)<<" "<<HR.GetBinContent(i)<<endl;
      cout<<HR.GetBinCenter(i)<<",";
  }
  
  
}
