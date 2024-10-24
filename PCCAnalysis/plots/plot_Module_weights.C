#include "globals.h" 
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>


TString OutPath = "./tmp";


#define MARKERSIZE 0.3
#define MAXMODWEIGHT 0.01
#define NBINMODWEIGHT 500 //for TH1 distribution

#define NLSBLOCK 1
#define LS_ID_MIN 0
#define LS_ID_MAX 2500

#define NBINTOTPCC 500
#define MINTOTPCC 0
#define MAXTOTPCC 20000
#define MINTOTPCCAVG 0
#define MAXTOTPCCAVG 10

bool makeModuleGraphs=0;//total counts per module , not the weights

//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/test/veto_B0.txt";//initial veto
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/test/vetoModules_2017.txt";//2017 veto from Sam
TString ModVeto = "BRILAnalysisCode/PCCAnalysis/test/vetoModules_2017_fixed.txt";


TString InputPath = "./AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/Run2017B/";
std::vector<std::string> run_number = {"181E6C2A-1E12-FF49-88E5-556411741312.csv","91A1032B-93B8-2946-9701-690A0CB1CDCA.csv","E57DFC3F-F7D0-8B40-8067-C6512B09FE63.csv", "670BF0D8-3ED8-224F-B750-E85BEBAE2EAE.csv", "D72ECBEC-6CD4-AC4D-A02A-EDAB07C08D50.csv", "FD3B4EC4-2B4D-C840-9223-7188F6DBCD01.csv"};




void plot_Module_weights() {  

  
  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  TCanvas* C=new TCanvas("Canvas","",50,50,800,600);
  TLine line;
  
  readModRPhiZCoordinates();

  //dump print the module list:
  for (unsigned int i=0;i<NMOD;i++)
    std::cout<<MODID[i]<<std::endl;
  return;


  
  if(!readModVeto(ModVeto)) return;




  
  TH2F PCC_vs_LS("Histo_totpcc","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,NBINTOTPCC,0,MAXTOTPCC);
  TH2F PCCPerMod_vs_LS("Histo_totpccavg","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,NBINTOTPCC,MINTOTPCCAVG,MAXTOTPCCAVG);

  TH1F * h_modcount[NMOD];
  TGraph ModWeight;
  TH1F HModWeight("HModWeight","",NBINMODWEIGHT,0,MAXMODWEIGHT);
  
  int ngood_mod=0;
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
      ngood_mod++;
      h_modcount[i]=new TH1F(TString("h_modcount")+i,"",NBINMODWEIGHT,0,MAXMODWEIGHT);
    }
  }
  cout<<"Number of good modules: "<<ngood_mod<<endl;


  int run=0;
  int ls=0;
  
  //fill LS identifiers for time ordering
  std::list<int> lslist;  
  for (unsigned int j=0;j<run_number.size();j++){
    TString infile=InputPath+"/"+run_number[j].c_str(); 
    ifstream csvfile (infile.Data());    
    if (!csvfile.is_open()){
      cout << "Unable to open file: "<<infile.Data()<<endl; 
      continue;
    }
    cout<<"Opened: "<<infile.Data()<<endl;
    
    std::string line;    
    while (std::getline(csvfile, line)){
      std::stringstream iss(line);

      std::string token;
      std::getline(iss,token, ','); 
      std::stringstream runiss(token);
      runiss>>run;
     
      std::getline(iss,token, ',');
      std::stringstream lsiss(token);
      lsiss>>ls;
      
      lslist.push_back(run*2000+ls);
    }
  }
  lslist.sort();//time order the LS's


  ////Read the data
  int ls_idx=0.;
  cout<<"Reading data from: "<<InputPath<<endl;
  for (unsigned int j=0;j<run_number.size();j++){
     
    TString infile=InputPath+"/"+run_number[j].c_str(); 
    ifstream csvfile (infile.Data());    
    if (!csvfile.is_open()){
      cout << "Unable to open file: "<<infile.Data()<<endl; 
      continue;
    }
    cout<<"Opened: "<<infile.Data()<<endl;
    
    int nls_run=0;    
    std::string line;    
    while (std::getline(csvfile, line)){
      std::stringstream iss(line);
      std::string token;
      
      std::getline(iss,token, ','); 
      std::stringstream runiss(token);
      runiss>>run;
     
      std::getline(iss,token, ',');
      std::stringstream lsiss(token);
      lsiss>>ls;
     
      ls_idx = 0;
      //get time ordered index
      int i=0;
      for(int idx : lslist){
	if(idx == (run*2000+ls) ) ls_idx=i;
	i++;
      }
      
      float totcount=0;      
      float m_count[NMOD];      
      for (unsigned int i=0;i<NMOD;i++){
	m_count[i]=0.;
	std::getline(iss,token, ',');
	std::stringstream countiss(token);
	countiss>>m_count[i];
	if(MODVETO[MODID[i]]==0) totcount += m_count[i];
      }

      
      if(LS_ID_MIN < ls_idx && ls_idx < LS_ID_MAX  && ngood_mod>0){	    
	float totcount_avg=float(totcount)/ngood_mod;
	if(MINTOTPCC < totcount  && totcount < MAXTOTPCC){	  
	  PCC_vs_LS.Fill(ls_idx,totcount);
	  PCCPerMod_vs_LS.Fill(ls_idx,totcount_avg);

	  for (unsigned int i=0;i<NMOD;i++){
	    if(MODVETO[MODID[i]]==0){
	      h_modcount[i]->Fill(m_count[i]/totcount);
	    }
	  }

	}
	 
      }
   
    }
    
    csvfile.close();    
  }
  cout<<"Last LS idx "<<ls_idx<<endl;


  
  /////////////////////////////////////////////////////////////////////////////////////////
  
  float max_mod_weight=0;
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
    
      float mean=h_modcount[i]->GetMean();
      float rms=h_modcount[i]->GetRMS();

      ModWeight.SetPoint(ModWeight.GetN(),i, mean);
      HModWeight.Fill(mean);
      
      //find the largest weight 
      if(mean > max_mod_weight) max_mod_weight = mean;

     
//      ///module count vs LS 
//      if(makeModuleGraphs){
//	C->Clear();
//	//h_modcount_vs_LS[i]->SetStats(0);
//	h_modcount_vs_LS[i]->GetYaxis()->SetRangeUser(0,1.2*MAXTOTPCCAVG);
//	h_modcount_vs_LS[i]->SetTitle(TString("Module ")+MODID[i]);
//	h_modcount_vs_LS[i]->Draw("histp");
//	C->Print(OutPath+"/Module_RMS_Stability_modulecount_"+i+".png");
//      }
      
    }
  }
  cout<<"Max module weight: "<<max_mod_weight<<endl;


  
  //Clean up per module plots
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
      delete h_modcount[i];
      //delete h_modcount_vs_LS[i];
    }
  }

 
  ////////////////////////////////////////////
  //Make the plots
  ////////////////////////////////////////////

  ///Total PCC (sum all modules)
  C->Clear();
  PCC_vs_LS.GetYaxis()->SetTitle("Total PCC");
  PCC_vs_LS.GetXaxis()->SetTitle("Lumi section");
  PCC_vs_LS.SetStats(0);
  PCC_vs_LS.Draw("scat");
  C->Print(OutPath+"/plot_Module_weights_totalcount.png");

  ///Avg PCC per module
  C->Clear();
  PCCPerMod_vs_LS.GetYaxis()->SetTitle("Avg. PCC per module");
  PCCPerMod_vs_LS.GetXaxis()->SetTitle("Lumi section");
  PCCPerMod_vs_LS.SetStats(0);
  PCCPerMod_vs_LS.Draw("scat");
  C->Print(OutPath+"/plot_Module_weights_totalcountavg.png");

  
  /////////////////////
  /// Module weights
  ////////////////////
  ModWeight.SetMarkerStyle(8);
  ModWeight.SetMarkerSize(MARKERSIZE);
  ModWeight.SetMarkerColor(4);
  ModWeight.GetXaxis()->SetTitle("Module ID");                                                                                 
  ModWeight.GetYaxis()->SetTitle("Module Weight");
  ModWeight.GetYaxis()->SetRangeUser(1E-5,MAXMODWEIGHT);
  C->Clear();
  ModWeight.Draw("AP"); 
  C->Print(OutPath+"/plot_Module_weights_vs_id.png");

  
  HModWeight.GetXaxis()->SetTitle("Module Weight");                                                                                 
  HModWeight.GetYaxis()->SetTitle("# of modules");
  C->Clear();
  C->SetLogy(1);
  HModWeight.Draw("hist");
  C->Print(OutPath+"/plot_Module_weights_histo.png");
  C->SetLogy(0);
}

  

