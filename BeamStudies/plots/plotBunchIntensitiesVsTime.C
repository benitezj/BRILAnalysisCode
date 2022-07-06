#include <iostream>
#include <fstream>
#include <string>
#define NBX 3564

TString TimeWindow("2022-07-04 ,  22:15 - 23:06 UTC");

void plotBunchIntensitiesVsTime(TString infile){

  //gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  ///define visible color for each bcid
  std::map<int,int> colors;
  colors[4]=1;
  colors[851]=2;
  colors[892]=3;
  colors[895]=4;
  colors[1201]=6;
  colors[1501]=7;
  colors[1786]=8;
  colors[1963]=9;
  colors[2101]=1;
  colors[2224]=2;
  colors[2451]=3;
  colors[2701]=4;
  colors[2801]=6;
  colors[2995]=7;
  colors[3118]=8;

  
  std::map<int,int> styles;
  styles[4]=1;
  styles[851]=1;
  styles[892]=1;
  styles[895]=1;
  styles[1201]=1;
  styles[1501]=1;
  styles[1786]=1;
  styles[1963]=1;
  styles[2101]=2;
  styles[2224]=2;
  styles[2451]=2;
  styles[2701]=2;
  styles[2801]=2;
  styles[2995]=2;
  styles[3118]=2;
  

 
  ///Open the lumi csv file
  ifstream myfile (infile.Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<infile.Data()<<endl; 
    return;
  }


  TGraph GBunchI0;
  TGraph GBunchI[NBX];
  float I0[NBX];

  std::string line;
  std::string token;
  long T0=0;
  long T=0;
  float Q=0;
  int firstBX=-1;
  std::string variablename;

  //remove first 3 header lines;
  std::getline(myfile, variablename);
  std::getline(myfile, line);
  std::getline(myfile, line);
  while (std::getline(myfile, line)){
    //cout<<line;
    std::stringstream iss(line);
 

    ///read the time
    std::getline(iss,token, ',');
    std::stringstream Tiss(token);
    Tiss>>T;
    if(T0==0) T0=T;


    //std::getline(iss,token, ',');
    //std::stringstream Qiss(token);
    //Qiss>>Q;


    for(int bx=0;bx<NBX;bx++){
      std::getline(iss,token, ',');
      std::stringstream Qiss(token);
      Qiss>>Q;

      ///fill the initial charges
      if(T==T0){
	I0[bx]=Q;
	
	if(Q>1){
	  GBunchI0.SetPoint(GBunchI0.GetN(),bx+1,Q/1e10);
	  cout<<bx<<endl;
	}
      }

      //Fill the time profiles
      if(Q>1){
	if(firstBX==-1)
	  firstBX=bx;//find which is the first non-zero bcid

	if(I0[bx]>1)
	  GBunchI[bx].SetPoint(GBunchI[bx].GetN(),(T-T0)/1000000.,Q/I0[bx]);
      }
    }
    
    //read the bx intensities
    //cout<<T-T0<<" : "<<Q<<endl;
    
  }

  ///close files
  myfile.close();


  //
  if(firstBX==-1){
    cout<<"There were no filled bunches"<<endl;
    return;
  }

  /////////////////////////////////////////////////////
  ///   make the plots
  ///////////////////////////////////////////////////
  gStyle->SetOptStat(0);
  TCanvas C("C","",1000,600);

  TLegend leg(0.88,0.5,0.98,0.95);


  //////////////////////////////////////////////
  ///Initial bunch intensities for all bx
  C.Clear();
  GBunchI0.SetTitle(variablename.c_str());
  GBunchI0.SetMarkerStyle(8);
  GBunchI0.SetMarkerSize(1.1);
  GBunchI0.GetYaxis()->SetTitle("Q_{0} ( x 10^{10} )");
  GBunchI0.GetYaxis()->SetRangeUser(0,15);
  GBunchI0.GetXaxis()->SetTitle("bcid");
  GBunchI0.Draw("ap");
  TString plotfilename0=infile;
  plotfilename0.ReplaceAll(".csv","_QvsBX.pdf");
  C.Print(plotfilename0.Data());


  ////////////////////////////////////////////////////
  ///bunch intensities vs time for all bx
  int gcounter=1;
  GBunchI[firstBX].SetTitle(TString("")+variablename.c_str()+"  ,  "+TimeWindow);
  GBunchI[firstBX].GetYaxis()->SetTitle(" Q / Q_{0}");
  GBunchI[firstBX].GetYaxis()->SetRangeUser(0.85,1.01);
  GBunchI[firstBX].GetXaxis()->SetTitle("time (s)");
  C.Clear();
  GBunchI[firstBX].SetLineColor(colors[firstBX+1]);
  GBunchI[firstBX].SetLineStyle(styles[firstBX+1]);
  GBunchI[firstBX].Draw("al");
  leg.AddEntry(&GBunchI[firstBX],TString("bx=")+(firstBX+1),"l");

  
  for(int bx=0;bx<NBX;bx++){
    if(GBunchI[bx].GetN()>0 && bx!=firstBX){
      gcounter++;
      GBunchI[bx].SetLineColor(colors[bx+1]);
      GBunchI[bx].SetLineStyle(styles[bx+1]);
      GBunchI[bx].Draw("lsame");
      leg.AddEntry(&GBunchI[bx],TString("bx=")+(bx+1),"l");
    }
  }
  leg.Draw();
  TString plotfilename=infile;
  plotfilename.ReplaceAll(".csv","_QvsTime.pdf");
  C.Print(plotfilename.Data());


  
  gROOT->ProcessLine(".q");
}

