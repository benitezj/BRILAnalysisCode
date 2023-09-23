#include"globals.h" 
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include "modules_361913.h"

bool fitModuleNoise=0;
char fitres[200];

TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto2022_FStab2p08pLin04p025p_CStab2p1pLin04p03p_EStab06pLin03p_GStab06pLin03p_DStab06pLin03p_vdmStab1p04pNoise05p.txt";
TString OutPath = "./tmp"; //"/afs/cern.ch/user/b/benitezj/www/BRIL/PCC_lumi/ModuleVeto2022";

TString InputPath = "./ModuleVeto2022/data/Run2022F";  //"/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022F";

#define LS_ID_MIN 0
#define LS_ID_MAX 2900
std::vector<int> run_number = {361906,361909,361910,361912,361913,361915,361916,361917,361919,361921,361922,361923,361925,361926,361927,361929,361932,361933}; //vdM fill 8385

//#define LS_ID_MIN 140
//#define LS_ID_MAX 180
//std::vector<int> run_number = {361919};


TF1* fitvdM(TH1F* P){
  TF1 *F = new TF1(TString("FitVDM")+P->GetName(),"[0]+[1]*exp(-(x-[2])*(x-[2])/(0.5*[3]*[3]))",P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  F->SetLineColor(2);
  F->SetParameter(1,P->GetMaximum());
  F->SetParLimits(1,0.05*P->GetMaximum(),2*P->GetMaximum());
  F->SetParameter(2,( P->GetBinCenter(P->GetNbinsX()) - P->GetBinCenter(1) )/2);
  F->SetParLimits(2,P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  F->SetParameter(3,0.5*P->GetNbinsX());
  F->SetParLimits(3,1,P->GetNbinsX());
  P->Fit(F,"Q","Q",P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  sprintf(fitres,"%s , B/S=%.4f",P->GetTitle(),(F->GetParameter(0)/F->GetParameter(1)));
  P->SetTitle(fitres);
  return F;
}


void fitPCCvdM_PerModule(){  
  
  readModRPhiZCoordinates();
 
  if(!readModVeto(ModVeto)) return;
  
  int run=0;
  int ls=0;
  int nls_tot=0.;
  int nls_tot_run=0.;
  int ls_idx=0.;
  int m_count[NMOD];

  TH1F *h_modcount_vs_LS[NMOD];
  int ngood_mod=0;
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
      ngood_mod++;
      h_modcount_vs_LS[i]=new TH1F(TString("h_modcount_vs_LS")+i,"",(LS_ID_MAX-LS_ID_MIN),LS_ID_MIN,LS_ID_MAX);
      h_modcount_vs_LS[i]->Sumw2();
      h_modcount_vs_LS[i]->SetStats(0);
    }
  }
    
  cout<<"Number of good modules: "<<ngood_mod<<endl;
  


  cout<<"Reading data from: "<<InputPath<<endl;
  for (unsigned int j=0;j<run_number.size();j++){
     
    TString infile=InputPath+"/"+run_number.at(j)+".csv"; 
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
     
      nls_run++;      
      nls_tot++;
      ls_idx = nls_tot_run + ls;

      
      if( ls_idx < LS_ID_MIN || ls_idx > LS_ID_MAX) continue; //select fit range
      
      for (unsigned int i=0;i<NMOD;i++){
	std::getline(iss,token, ',');
	std::stringstream countiss(token);
	countiss>>m_count[i];
	if(MODVETO[MODID[i]]==0){
	  h_modcount_vs_LS[i]->Fill(ls_idx, m_count[i]);
	}
      }

    }
    
    csvfile.close();    
    nls_tot_run+=nls_run;
    cout<<"# of LS "<<nls_run<<endl;
  }
  cout<<"Total # of LS "<<nls_tot<<endl;
  cout<<"Last LS idx "<<ls_idx<<endl;
  

  TCanvas C("C");
  C.SetLogy(1);
  ///Make a Profile , fit , get chi2
  float max_mod_weight=0;
  for (unsigned int i=0;i<350;i++){
    if(MODVETO[MODID[i]]==0){
      C.Clear();
      h_modcount_vs_LS[i]->GetYaxis()->SetRangeUser(100,h_modcount_vs_LS[i]->GetMaximum()*5);
      h_modcount_vs_LS[i]->GetXaxis()->SetTitle("lumi section");
      h_modcount_vs_LS[i]->SetTitle(TString("Module ")+MODID[i]);
      h_modcount_vs_LS[i]->SetMarkerStyle(8);
      h_modcount_vs_LS[i]->SetMarkerSize(0.5);
      h_modcount_vs_LS[i]->Draw("histp");
      
      if(fitModuleNoise){
	TF1*F = fitvdM(h_modcount_vs_LS[i]);
	F->Draw("lsame");
	float vdmnoise = F->GetParameter(0)/F->GetParameter(1);
	cout<<i<<" "<<vdmnoise<<endl;
      }
      C.Print(OutPath+"/fitPCCvdM_PerModule_"+i+".png");     
    }
  }

  
  //Clean up per module plots
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
      delete h_modcount_vs_LS[i];
    }
  }
  
}

  

