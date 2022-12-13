
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
  HR.Draw("hist");
  C.Print("plotBRILAvgLumiPerRun_totL.png");

  C.Clear();
  HRN.Draw("hist");
  C.Print("plotBRILAvgLumiPerRun_NLS.png");

  HR.Divide(&HRN);
  C.Clear();
  HR.Draw("hist");
  C.Print("plotBRILAvgLumiPerRun_AvgL.png");

  for(int i=1;i<=HR.GetNbinsX();i++){
    if(HR.GetBinContent(i)>1000)
      //cout<<HR.GetBinCenter(i)<<" "<<HR.GetBinContent(i)<<endl;
      cout<<HR.GetBinCenter(i)<<",";
  }
  
  
}
