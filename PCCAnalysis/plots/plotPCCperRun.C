#include <iostream>
#include <fstream>
#include <string>

long RUNOFFSET=315000;



void plotPCCperRun(TString inpath){

  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");  

  std::vector<std::string> Paths;

  float ymin=-0.006;
  float ymax=0.006;
  TString title="slope";
  Paths.push_back("Run2018A/slope.dat");
  Paths.push_back("Run2018B/slope.dat");
  Paths.push_back("Run2018C/slope.dat");
  Paths.push_back("Run2018D/slope.dat");
  

  ///create histogram
  TGraphErrors LumiRun;
  std::string rline;
  int run=0;
  float totLRun=0.;
  float Err=0.;
  int counter=0;

  std::vector<int> periods;


  for(int f=0;f<Paths.size();f++){

    TString infile=inpath+"/"+Paths[f];
    ifstream myfile(infile.Data());
    if (!myfile.is_open()){
      cout << "Unable to open file"<<std::endl; 
      return;
    }
    cout<<"opened "<<infile<<std::endl;
    
    while (std::getline(myfile,rline)){
      std::stringstream iss(rline);
      iss>>run>>totLRun>>Err;
      if(Err<0.0001 || Err>0.003) continue;
      LumiRun.SetPoint(counter,counter+1,totLRun);
      LumiRun.SetPointError(counter,0,Err);
      counter++;
    }
    myfile.close();


    periods.push_back(counter);

  }



  TCanvas C;

  //C.SetLogy(1);

  LumiRun.Draw("ape");
  LumiRun.GetYaxis()->SetRangeUser(ymin,ymax);
  LumiRun.SetMarkerStyle(8);
  LumiRun.SetMarkerSize(0.5);
  LumiRun.GetXaxis()->SetTitle("run");
  LumiRun.GetYaxis()->SetTitle(title);
  TF1 f("fit","[0]",0,counter);
  f.SetLineColor(2);
  LumiRun.Fit(&f,"","",periods[0],periods[periods.size()-1]);

  C.Clear();
  LumiRun.Draw("ape");
  f.Draw("lsame");
  
  TLine line;
  line.SetLineColor(3); line.SetLineStyle(2);
  for(int p=0;p<periods.size();p++)
    line.DrawLine(periods[p],ymin,periods[p],ymax);

  C.Print(inpath+"/plotPCCperRun.png");

  gROOT->ProcessLine(".q");
}

