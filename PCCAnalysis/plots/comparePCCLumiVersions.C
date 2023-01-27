#include "globals.h"

float yrange=0.05;
std::map<int,float> lumimap;
float cutreflumi=300;
std::map<int,int> lastLSmap;//for each run stores the last LS number

bool fillLumimap(TString file){

  ifstream reffile(file.Data());
  if (!reffile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<file.Data()<<std::endl;
    return 0;
  }

  std::string line;
  int run=0;
  int ls=0;
  float L=0.;
  int counter=0;
  while (std::getline(reffile, line)){
    std::stringstream iss(line);
    iss>>run>>ls>>L;
    lumimap[run*10000+ls]=L;
    counter++;
  }
  cout<<"Reference LS counter:"<<counter<<endl;

  return 1;
}


bool fillLastLSmap(TString file){

  ifstream reffile(file.Data());
  if (!reffile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<file.Data()<<std::endl;
    return 0;
  }

  std::string line;
  int run=0;
  int ls=0;
  float L=0.;
  while (std::getline(reffile, line)){
    std::stringstream iss(line);
    iss>>run>>ls>>L;

    if(ls>lastLSmap[run]){ 
      lastLSmap[run]=ls;
    }
  }


  return 1;
}




void comparePCCLumiVersions(TString refDir, TString newDir){


  TGraph LumiRef;
  TGraph LumiNew;
  TGraph LumiRatio;
  //  TGraph LumiGainedRuns;
  //  TGraph LumiLostRuns;
  TH1F HRatio("HRatio","",100,1-yrange,1+yrange);
  
  if(!fillLumimap(newDir+"/ls.dat"))
    return;

  if(!fillLastLSmap(refDir+"/ls.dat"))
    return;

  ifstream reffile((refDir+"/ls.dat").Data());
  if (!reffile.is_open()){
    std::cout << "Unable to open ls.dat in "<<newDir.Data()<<std::endl;
    return;
  }

  std::string line;
  int run=0;
  int ls=0;
  float newLumi = 0.;
  float refLumi = 0.;
  int counter=0;
  int lastrun=0;
  int lastLS=0;
  int x=0;
  while (std::getline(reffile,line)){
    std::stringstream iss(line);
    iss>>run>>ls>>refLumi;
    newLumi = lumimap[run*10000+ls];

    if(run!=lastrun){
      lastrun=run;
      lastLS+=lastLSmap[run];
      //cout<<run<<" "<<lastLSmap[run]<<" "<<lastLS<<" "<<(lastLS-lastLSmap[run])+ls<<endl;
    }


    x=(lastLS-lastLSmap[run])+ls;
    LumiRef.SetPoint(counter,x,refLumi);
    LumiNew.SetPoint(counter,x,newLumi);
    
    if(refLumi>cutreflumi){
      LumiRatio.SetPoint(counter,x,newLumi/refLumi);
      HRatio.Fill(newLumi/refLumi);
    }else{
      LumiRatio.SetPoint(counter,x,0);
    }

    counter++;
  }
  reffile.close();
  cout<<"Total number of lumi sections: "<<counter<<endl;

  TCanvas C;

  TGraph LumiFrame;
  LumiFrame.SetPoint(0,0,1);
  LumiFrame.SetPoint(1,counter,1);
  LumiFrame.GetXaxis()->SetTitle("lumi section");//TString("run - ")+RUNOFFSET);
  LumiFrame.GetYaxis()->SetTitle("lumi ratio");
  LumiFrame.GetYaxis()->SetRangeUser(1-yrange,1+yrange);
  LumiFrame.Draw("ap");

  LumiRatio.SetMarkerStyle(8);
  LumiRatio.SetMarkerSize(0.5);
  LumiRatio.Draw("psame");

  C.Print(newDir+"/comparePCCLumiVersions_ratio.png");


  C.Clear();
  gStyle->SetOptStat(1111111);
  HRatio.GetYaxis()->SetTitle("# of LS");
  HRatio.GetXaxis()->SetTitle("lumi ratio");
  HRatio.Draw("hist");
  C.Print(newDir+"/comparePCCLumiVersions_histo.png");

  C.SetLogy(1);
  C.Clear();
  LumiRef.GetYaxis()->SetRangeUser(1,25000);
  LumiRef.GetYaxis()->SetTitle("pcc [hz/ub]");
  LumiRef.GetXaxis()->SetTitle("lumi section");
  LumiRef.Draw("ap");
  C.Print(newDir+"/comparePCCLumiVersions_lumiRef.png");

  C.Clear();
  LumiNew.GetYaxis()->SetRangeUser(1,25000);
  LumiNew.GetYaxis()->SetTitle("pcc [hz/ub]");
  LumiNew.GetXaxis()->SetTitle("lumi section");
  LumiNew.Draw("ap");
  C.Print(newDir+"/comparePCCLumiVersions_lumiNew.png");
  
  


  gROOT->ProcessLine(".q");
}
