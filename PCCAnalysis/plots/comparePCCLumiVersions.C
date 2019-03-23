#include "globals.h"

int refidx=0;

float getRefLumi(TString file, unsigned int RUN, unsigned int LS){
  
  ifstream reffile(file.Data());
  if (!reffile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<file.Data()<<std::endl;
    return 0.;
  }
  
  std::string line;
  int run=0;
  int ls=0;
  float L=0.;

  for(int i=0;i<refidx;i++)
    reffile.ignore(100,'\n');

  while (std::getline(reffile, line)){
    refidx++;
    std::stringstream iss(line);
    iss>>run>>ls>>L;
    if(run==RUN&&ls==LS) return L;
  }
  
  return 0.;
}


void comparePCCLumiVersions(TString refDir, TString newDir){

  TGraph LumiFrame;
  TGraph LumiRatio;
  TGraph LumiGainedRuns;
  TGraph LumiLostRuns;


  ifstream myfile((newDir+"/ls.dat").Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open ls.dat in "<<newDir.Data()<<std::endl;
    return;
  }

  std::string line;
  int run=0;
  int ls=0;
  int firstrun=9999999;
  int lastrun=0;
  float newLumi = 0.;
  float refLumi = 0.;
  int counter=0;
  int counter2=0;
  while (std::getline(myfile,line)){
    std::stringstream iss(line);
    iss>>run>>ls>>newLumi;

    // if(run<firstrun) firstrun=ls;
    // if(run>lastrun) lastrun=ls;


    refLumi = getRefLumi(refDir+"/ls.dat",run,ls);

    //cout<<run<<" "<<ls<<" "<<newLumi<<" "<<refLumi<<endl;
    

    if(refLumi>0.){
      if(newLumi/refLumi>2)
	LumiRatio.SetPoint(counter,counter,1.1);
      else LumiRatio.SetPoint(counter,counter,newLumi/refLumi);
      counter++;
    }else if(newLumi>0.) {
      LumiGainedRuns.SetPoint(counter2++,ls,1);
    }

  }
  myfile.close();

  //fix boundaries
  LumiFrame.SetPoint(0,0,0);
  LumiFrame.SetPoint(1,counter,0);
  LumiFrame.GetXaxis()->SetTitle("lumi section");//TString("run - ")+RUNOFFSET);
  LumiFrame.GetYaxis()->SetTitle("new lumi / old lumi");

  // //now loop over old run list and check if any where lost:
  // ifstream reffile((refDir+"/runs.dat").Data());
  // if (!reffile.is_open()){
  //   std::cout << "Unable to open runs in "<<newDir<<std::endl;
  //   return;
  // }
  // counter=0;
  // while (std::getline(reffile,line)){
  //   std::stringstream iss(line);
  //   iss>>run>>refLumi;
  //   newLumi = getRefLumi(newDir+"/runs.dat",run);

  //   if(refLumi>0.&&newLumi<=0.){ 
  //     cout<<"Lost: "<<run<<endl;
  //     LumiLostRuns.SetPoint(counter++,run,0.9);
  //   }
  // }
  // reffile.close();


  //gStyle->SetOptStat(0);

  TCanvas C;

  //LumiFrame.GetYaxis()->SetRangeUser(0,1.2);
  LumiFrame.GetXaxis()->SetRangeUser(0,counter);
  LumiFrame.Draw("ap");


  LumiRatio.SetMarkerStyle(8);
  LumiRatio.SetMarkerSize(0.5);
  LumiRatio.Draw("psame");

  TF1 Fit("pol1","[0]+[1]*x",0,counter);
  LumiRatio.Fit(&Fit);
  TText pars;
  pars.SetTextSize(0.04);
  char text[100];
  sprintf(text,"y = p0 + p1*x");
  pars.DrawTextNDC(0.6,0.85,text);
  sprintf(text,"p0 = %.3f +/- %.3f",Fit.GetParameter(0),Fit.GetParError(0));
  pars.DrawTextNDC(0.6,0.8,text);
  sprintf(text,"p1 = %.3f +/- %.3f",Fit.GetParameter(1),Fit.GetParError(1));
  pars.DrawTextNDC(0.6,0.75,text);



  LumiGainedRuns.SetMarkerStyle(8);
  LumiGainedRuns.SetMarkerSize(0.5);
  LumiGainedRuns.SetMarkerColor(3);
  LumiGainedRuns.Draw("psame");

  // LumiLostRuns.SetMarkerStyle(8);
  // LumiLostRuns.SetMarkerSize(0.5);
  // LumiLostRuns.SetMarkerColor(2);
  // LumiLostRuns.Draw("psame");
  
  C.Print(newDir+"/comparePCCLumiVersions.png");

  gROOT->ProcessLine(".q");
}
