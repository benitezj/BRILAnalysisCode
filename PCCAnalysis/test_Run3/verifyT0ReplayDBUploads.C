#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

#define NBX 3564
#define rmin 0.999999
#define rmax 1.000001

void verifyT0ReplayDBUploads(){

  gStyle->SetPadLeftMargin(0.15);
  TCanvas C;
    
  ///Input from private test: step2 + step3 + step4
  TFile RootFile("CorrectionHisto.root","read");

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
  
  TH1F HScaleFactorsRatio("HScaleFactorsRatio","",NBX,0.5,NBX+0.5);  
  TH1F HScaleFactorsRatioProj("HScaleFactorsRatioProj","",200,rmin,rmax);
  TH2F H2ScaleFactorsRatio("H2ScaleFactorsRatio","",NBX,0.5,NBX+0.5,200,rmin,rmax);  

  std::list<std::string>::iterator it;
  for (it = payload.begin(); it != payload.end(); ++it){
    TString PayloadName=it->c_str();
    
    //if(PayloadName.CompareTo("ScaleFactorsAvg_346512_9_467_518") !=0 )   continue;
    //if(PayloadName.CompareTo("ScaleFactorsAvg_346512_11_571_622") !=0 )   continue;
        
    std::cout << it->c_str()<<std::endl;
    
    std::istringstream input(it->c_str());
    std::string s;
    long lsid = 0;
    int lsidc=0;
    while (std::getline(input, s, '_')){
      if(lsidc==3) lsid = atoi(s.c_str()) + 1 ;
      lsidc++;
    }
    cout << lsid << endl;
    
    
    TH1F* HScaleFactors=(TH1F*)RootFile.Get(it->c_str());
    if(!HScaleFactors) return;
    //cout<<"Nbins : "<<HScaleFactors->GetNbinsX() <<" / "<<NBX<<endl;
    TH1F HScaleFactorsDB("HScaleFactorsDB","",NBX,0.5,NBX+0.5);

    
    /////////////////////////////////////////////////////
    ///Scale factors produced by reading the CONDDB 
    TString csvfilename="./rawPCC.csv";
    ifstream csvfile(csvfilename.Data());
    if (!csvfile.is_open()){
      cout << "Unable to open file: "<<csvfilename.Data()<<endl;
      return;
    }

    std::string line;
    int run=0;
    int ls=0;
    double lsL=0; //lumi for given LS
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

      //cout<<run<<"  "<<ls<<endl;
      if(ls!=lsid) continue;
      cout<<"Found payload at ls="<<ls<<endl;
      
      ///read the  Lumi per ls (only due to file format)
      std::getline(iss,token, ',');
      std::stringstream totLiss(token);
      totLiss>>lsL;
    
    
      ///fill lumi per BX plots
      for(int bx=1;bx<=NBX;bx++){
	double privateValue=HScaleFactors->GetBinContent(bx-1);
  
	double conddbValue=0.;
	
	std::getline(iss,token, ',');
	std::stringstream bxLiss(token);
	bxLiss>>conddbValue;
	HScaleFactorsDB.SetBinContent(bx,conddbValue);

	if(conddbValue!=0.){
	  float r=privateValue/conddbValue;
	  
	  if(r<rmin || r>rmax){
	    cout<<"bad ratio: bx = "<<bx<<" , TEST = "<<privateValue<<" , CONDDB = "<<conddbValue<<endl;
	  }
	  
	  H2ScaleFactorsRatio.Fill(bx,privateValue/conddbValue);
	  if(PayloadName.CompareTo("ScaleFactorsAvg_346512_9_467_518") ==0 ){	    
	    HScaleFactorsRatio.Fill(bx,privateValue/conddbValue);
	    HScaleFactorsRatioProj.Fill(privateValue/conddbValue);
	  }
	}
	else if(conddbValue==0. && privateValue==0.){
	  H2ScaleFactorsRatio.Fill(bx,1);
	  if(PayloadName.CompareTo("ScaleFactorsAvg_346512_9_467_518") ==0 ){	    
	    HScaleFactorsRatio.SetBinContent(bx,1);
	    HScaleFactorsRatioProj.Fill(1);
	  }
	}
	else{
	  cout<<"bad ratio: bx = "<<bx<<" , TEST = "<<privateValue<<" , CONDDB = "<<conddbValue<<endl;
	  H2ScaleFactorsRatio.Fill(bx,0);
	  if(PayloadName.CompareTo("ScaleFactorsAvg_346512_9_467_518") ==0 ){	    
	    HScaleFactorsRatio.SetBinContent(bx,0);
	    HScaleFactorsRatioProj.Fill(0);
	  }
	}
      }

      break;
    }    
    csvfile.close();

    if(PayloadName.CompareTo("ScaleFactorsAvg_346512_9_467_518") ==0 ){

      HScaleFactors->SetTitle(PayloadName+" private test");
      HScaleFactors->SetMarkerColor(1);
      HScaleFactors->GetYaxis()->SetRangeUser(0.98,1.02);
      HScaleFactors->SetStats(0);
      HScaleFactors->GetXaxis()->SetTitle("bcid");
      C.Clear();
      HScaleFactors->Draw("histp");
      C.Print("verifyT0ReplayDBUploads.png");

      HScaleFactorsDB.SetTitle(PayloadName+" CONDDB");
      HScaleFactorsDB.SetMarkerColor(1);
      HScaleFactorsDB.GetYaxis()->SetRangeUser(0.98,1.02);
      HScaleFactorsDB.SetStats(0);
      HScaleFactorsDB.GetXaxis()->SetTitle("bcid");
      C.Clear();
      HScaleFactorsDB.Draw("histp");
      C.Print("verifyT0ReplayDBUploads_DB.png");

    }
    
  }


  
  HScaleFactorsRatio.SetStats(0);
  HScaleFactorsRatio.GetYaxis()->SetTitle("ratio");
  HScaleFactorsRatio.GetYaxis()->SetRangeUser(rmin,rmax);
  HScaleFactorsRatio.GetXaxis()->SetTitle("bcid");
  C.Clear();
  HScaleFactorsRatio.Draw("histp");
  C.Print("verifyT0ReplayDBUploads_ratio.png");

  gStyle->SetOptStat(111111);
  HScaleFactorsRatioProj.GetXaxis()->SetTitle(" ratio ");
  C.Clear();
  HScaleFactorsRatioProj.Draw("hist");
  C.Print("verifyT0ReplayDBUploads_proj.png");


  ///all payloads
  H2ScaleFactorsRatio.SetStats(1);
  H2ScaleFactorsRatio.GetYaxis()->SetTitle("ratio");
  H2ScaleFactorsRatio.GetXaxis()->SetTitle("bcid");
  C.Clear();
  H2ScaleFactorsRatio.Draw("colz");
  C.Print("verifyT0ReplayDBUploads_ratio2D.png");

  TH1F*Proj2D=(TH1F*)H2ScaleFactorsRatio.ProjectionY("Proj2D");
  C.Clear();
  Proj2D->Draw("hist");
  C.Print("verifyT0ReplayDBUploads_proj2D.png");

  
  
}

