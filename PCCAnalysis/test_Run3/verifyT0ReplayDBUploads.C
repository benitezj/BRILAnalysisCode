#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

#define NBX 3564
#define rmin 0.999999
#define rmax 1.000001

TString inputPath="./verify-T0Replay-RD";

/////////////////////////////////////////////////////
void readPayload_csv(TString filePath, int LS, float * SF){

  for(int bx=1;bx<=NBX;bx++)
    SF[bx-1]=0.;

  ifstream csvfile((filePath+"/rawPCC.csv").Data());
  if (!csvfile.is_open()){
    cout << "Unable to open file: "<<filePath.Data()<<endl;
    return;
  }

  std::string line;
  int run=0;
  int ls=0;
  double lsL=0; //lumi for given LS
  double sf=0.;
  while (std::getline(csvfile, line)){
    //cout<<line;

    std::stringstream iss(line);
    std::string token;

    ///read the run number
    std::getline(iss,token, ',');
    std::stringstream runiss(token);
    runiss>>run;
    
    ///read the LS
    std::getline(iss,token, ',');
    std::stringstream lsiss(token);
    lsiss>>ls;

    if(ls!=LS) continue;

    cout<<"Found payload at ls="<<ls<<endl;
      
    ///read the  Lumi per ls (only due to file format)
    std::getline(iss,token, ',');
    std::stringstream totLiss(token);
    totLiss>>lsL;
    
    
    ///
    for(int bx=1;bx<=NBX;bx++){
      std::getline(iss,token, ',');
      std::stringstream bxLiss(token);
      bxLiss>>SF[bx-1];
    }

    break;
  }    
  csvfile.close();
    
}

/////////////////////////////////////////////////////
int getLS(std::string name){
  std::istringstream input(name.c_str());
  std::string s;
  int lsidc=0;
  while (std::getline(input, s, '_')){
    if(lsidc==3) return  atoi(s.c_str()) + 1 ;
    lsidc++;
  }
  return 0;
}


///////////////////////////////////////////////////////
void verifyT0ReplayDBUploads(){

  gStyle->SetPadLeftMargin(0.15);


  ///Input from private test: step2 + step3 + step4
  TFile RootFile(inputPath+"/CorrectionHisto.root","read"); 

  ///Histograms corresponding to payload:
  std::list<std::string> payload;
  payload.push_back("ScaleFactorsAvg_346512_0_1_51");
  payload.push_back("ScaleFactorsAvg_346512_1_52_103");
  payload.push_back("ScaleFactorsAvg_346512_2_104_155");
  payload.push_back("ScaleFactorsAvg_346512_3_156_207");
  payload.push_back("ScaleFactorsAvg_346512_4_208_259");
  payload.push_back("ScaleFactorsAvg_346512_5_260_311");
  payload.push_back("ScaleFactorsAvg_346512_6_312_362");
  payload.push_back("ScaleFactorsAvg_346512_7_363_414");
  payload.push_back("ScaleFactorsAvg_346512_8_415_466");
  payload.push_back("ScaleFactorsAvg_346512_9_467_518");
  payload.push_back("ScaleFactorsAvg_346512_10_519_570");
  payload.push_back("ScaleFactorsAvg_346512_11_571_622");
  cout<<"Number of payloads: "<<payload.size()<<endl;

  
  TGraph G;//flattened scale factors vs bcid
  TGraph GDB;//flattened scale factors vs bcid
  TGraph GRatio;//ratios  
  TH2F H2ScaleFactorsRatio("H2ScaleFactorsRatio","",NBX,0.5,NBX+0.5,100,rmin,rmax);  //ratio values vs bx  in zoom

  TString selectPayload="ScaleFactorsAvg_346512_11_571_622";
  TGraph GPL;//flattened scale factors vs bcid
  TGraph GPLDB;//flattened scale factors vs bcid
  TGraph GPLRatio;// ratios

  float SF[NBX];
  
  int plcounter=0;
  std::list<std::string>::iterator it;
  for (it = payload.begin(); it != payload.end(); ++it){
    plcounter++;
    
    std::cout << it->c_str()<<std::endl;
    long lsid = getLS(it->c_str());
    cout << lsid << endl;
    
    TH1F* HScaleFactors=(TH1F*)RootFile.Get(it->c_str());
    if(!HScaleFactors) return;


    readPayload_csv(inputPath,lsid,SF);

    for(int bx=1;bx<=NBX;bx++){
      double privateValue = HScaleFactors->GetBinContent(bx-1);
      double conddbValue  = SF[bx-1];
 
      ///Make ratio
      float r=0.;
      if(conddbValue!=0.)
	r=privateValue/conddbValue;
      else if(conddbValue==0. && privateValue==0.)
	r=1.;
      else
	r=0.;
 	
      if(r<rmin || r>rmax){
	cout<<"bad ratio: bx = "<<bx<<" , TEST = "<<privateValue<<" , CONDDB = "<<conddbValue<<endl;
      }


      G.SetPoint(G.GetN(),G.GetN(),privateValue);
      GDB.SetPoint(GDB.GetN(),GDB.GetN(),conddbValue);
      GRatio.SetPoint(GRatio.GetN(),GRatio.GetN(),r);
      H2ScaleFactorsRatio.Fill(bx,r);
      
      if(selectPayload.CompareTo(it->c_str()) ==0 ){
	GPL.SetPoint(GPL.GetN(),bx,privateValue);
	GPLDB.SetPoint(GPLDB.GetN(),bx,privateValue);
	GPLRatio.SetPoint(GPLRatio.GetN(),bx,r);
      }
      
    }
    
   
  }


  

  TCanvas C;

  ///this is for one payload
  GPL.SetTitle(selectPayload);
  GPL.GetXaxis()->SetTitle("bcid");
  GPL.GetYaxis()->SetTitle("Scale factor");
  GPL.GetYaxis()->SetRangeUser(-0.1,1.1);
  C.Clear();
  GPL.Draw("al");
  GPLDB.SetMarkerColor(2);
  GPLDB.Draw("lsame"); 
  C.Print(inputPath+"/"+"verifyT0ReplayDBUploads_valuesPL.png");

  GPLRatio.SetTitle(selectPayload);
  GPLRatio.GetYaxis()->SetTitle("Scale factor ratio");
  GPLRatio.GetXaxis()->SetTitle("LS * bcid");
  GPLRatio.GetYaxis()->SetRangeUser(-0.1,2);
  C.Clear();
  GPLRatio.Draw("al"); 
  C.Print(inputPath+"/"+"verifyT0ReplayDBUploads_valuesPL_Ratio.png");

  
  ////all payloads Flattenned
  G.SetTitle("all payloads");
  G.GetXaxis()->SetTitle("LS * bcid");
  G.GetYaxis()->SetTitle("Scale factor");
  G.GetYaxis()->SetRangeUser(-0.1,1.1);
  C.Clear();
  G.Draw("al");
  GDB.SetMarkerColor(2);
  GDB.Draw("lsame"); 
  C.Print(inputPath+"/"+"verifyT0ReplayDBUploads_values.png");

  GRatio.SetTitle("all payloads");
  GRatio.GetYaxis()->SetTitle("Scale factor ratio");
  GRatio.GetXaxis()->SetTitle("LS * bcid");
  GRatio.GetYaxis()->SetRangeUser(-0.1,2);
  C.Clear();
  GRatio.Draw("al"); 
  C.Print(inputPath+"/"+"verifyT0ReplayDBUploads_values_Ratio.png");

  gStyle->SetOptStat(111111);
  H2ScaleFactorsRatio.SetStats(1);
  H2ScaleFactorsRatio.SetTitle("all payloads");
  C.Clear();
  H2ScaleFactorsRatio.Draw("histcolz"); 
  C.Print(inputPath+"/"+"verifyT0ReplayDBUploads_values_RatioZoom.png");

  
}

