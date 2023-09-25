#include"globals.h" 
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include "modules_361913.h"


TCanvas C("C");
bool fitNoise=0;
char fitres[200];
unsigned selectModule=-1;//0:all modules, -1:No modules, MODID:single module

int run=0;
int ls=0;
int ls_idx=0;
int timeref=1668109440;
float timestamp;
std::map<int,int> timemap;

float pcc;
float m_count[NMOD];
TH1F *h_pcc_vs_LS;
TH1F *h_modcount_vs_LS[NMOD];


TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto2022_FStab2p08pLin04p025p_CStab2p1pLin04p03p_EStab06pLin03p_GStab06pLin03p_DStab06pLin03p_vdmStab1p04pNoise05p.txt";
TString OutPath = "./fitPCCvdM_vdMData";

//TString InputPath = "./ModuleVeto2022/data/Run2022F";  //ALCARECO data per module  per run
std::vector<int> run_number = {361906,361909,361910,361912,361913,361915,361916,361917,361919,361921,361922,361923,361925,361926,361927,361929,361932,361933}; //vdM fill 8381

TString InputPath = "./LS_rates_avg.csv";        //vdM Tuple data


TF1* fitvdM(TH1F* P,ofstream*o){

  ///single Guass
  //TF1 *F = new TF1(TString("FitVDM")+P->GetName(),"[3]+[4]*x+[0]*exp(-(x-[1])*(x-[1])/(0.5*[2]*[2]))",P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));

  //Double Guass with same mean
  TF1 *F = new TF1(TString("FitVDM")+P->GetName(),"[3]+[4]*x+[0]*((1./[2])*exp(-(x-[1])*(x-[1])/(0.5*[2]*[2]))+([5]/[6])*exp(-(x-[1])*(x-[1])/(0.5*[6]*[6])))",P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));


  F->SetParLimits(0,0.0,2*P->GetMaximum());
  //F->FixParameter(0,0);
  F->SetParLimits(1,P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  F->SetParLimits(2,1,P->GetNbinsX());
  
  F->SetParLimits(5,0,.5);
  F->SetParLimits(6,1,P->GetNbinsX());
 
  P->Fit(F,"Q","Q",P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  sprintf(fitres,"%s , B/S=%.4f",P->GetTitle(), (F->GetParameter(3) + F->GetParameter(4)*(P->GetBinCenter(1) + P->GetBinCenter(P->GetNbinsX())/2) )  /F->GetParameter(0));

  F->SetLineColor(2);
  
  float x=(P->GetBinCenter(1) + P->GetBinCenter(P->GetNbinsX()))/2;
  (*o)<<P->GetTitle()<<" "<<x<<" "<<F->GetParameter(3)+F->GetParameter(4)*x <<endl;
  return F;
}



void readInfo(std::stringstream &iss){
  std::string token;   
  std::getline(iss,token, ','); 
  std::stringstream runiss(token);
  runiss>>run;
  std::getline(iss,token, ',');
  std::stringstream lsiss(token);
  lsiss>>ls;
}
void fillPCCALCA(){
  h_pcc_vs_LS->Fill(ls_idx,pcc);
  for (unsigned int i=0;i<NMOD;i++)
    if(MODVETO[MODID[i]]==0)
      h_modcount_vs_LS[i]->Fill(ls_idx,m_count[i]);
}


void readData(int LS_ID_MIN, int LS_ID_MAX){
  
  int nls_tot=0.;
  int nls_tot_run=0.;
  for (unsigned int j=0;j<run_number.size();j++){
    TString infile=InputPath+"/"+run_number.at(j)+".csv"; 
    ifstream csvfile (infile.Data());    
    if (!csvfile.is_open()){
      cout << "Unable to open file: "<<infile.Data()<<endl; 
      continue;
    }
    //cout<<"Reading data from: "<<infile<<endl;

    int nls_run=0;
    std::string line;    
    while (std::getline(csvfile, line)){
      std::stringstream iss(line);
      readInfo(iss);
      
      nls_run++;      
      nls_tot++;
      ls_idx = nls_tot_run + ls;
      
      if(ls_idx < LS_ID_MIN || ls_idx > LS_ID_MAX) continue;
           
      pcc=0;
      std::string token;
      for (unsigned int i=0;i<NMOD;i++){
	std::getline(iss,token, ',');
	std::stringstream countiss(token);
	countiss>>m_count[i];
	if(MODVETO[MODID[i]]==0)
	  pcc+=m_count[i];	
      }
      
      fillPCCALCA();
    }
 
    csvfile.close();    
    nls_tot_run += nls_run;
  }
  

}



void fillPCCHd5(){
  h_pcc_vs_LS->Fill(ls_idx,pcc);
}


void readDataHd5(int LS_ID_MIN, int LS_ID_MAX){
  
  int nls_tot_run=0;
  int nls_run=0;
  int prevrun=0;
  TString infile=InputPath;
  ifstream csvfile (infile.Data());    
  if (!csvfile.is_open()){
    cout << "Unable to open file: "<<infile.Data()<<endl; 
    return;
  }
  cout<<"Reading data from: "<<infile<<endl;


  //need to write out the map of ls_idx and time
  ofstream outfiletime(OutPath+"/time_ls_map.txt");
  if (!outfiletime.is_open()){
    cout << "Unable to open output time file";
    return;
  }
 
  
  std::string line;    
  while (std::getline(csvfile, line)){
    std::stringstream iss(line);
    readInfo(iss);

    nls_run++; 
    if(run!=prevrun&&prevrun!=0){
      nls_tot_run+=nls_run;
      nls_run=0;
      prevrun=run;
    }else{
      prevrun=run;
    }
    ls_idx = nls_tot_run+ls;
  
    std::string token;
    std::getline(iss,token, ',');
    std::stringstream countiss(token);
    countiss>>timestamp;

    std::getline(iss,token, ',');
    std::stringstream pcciss(token);
    pcciss>>pcc;
    pcc/=5;
    

    //
    outfiletime<<ls_idx<<" "<<long(timestamp)<<endl;
    
    if(ls_idx < LS_ID_MIN || ls_idx > LS_ID_MAX) continue;

    fillPCCHd5();
  }
  
  csvfile.close();
  outfiletime.close();
}



void fitPCCvdM(int LS_ID_MIN=600, int LS_ID_MAX=642){  
  //cout<<LS_ID_MIN<<" "<<LS_ID_MAX;
  
  ofstream outfile(OutPath+"/bkg_"+LS_ID_MIN+"_"+LS_ID_MAX+".dat");
  if (!outfile.is_open()){
    cout << "Unable to open output run file";
    return;
  }
 
  
  readModRPhiZCoordinates();
 
  if(!readModVeto(ModVeto)) return;
    
  h_pcc_vs_LS=new TH1F(TString("h_pcc_vs_LS_")+LS_ID_MIN+"_"+LS_ID_MAX,"",(LS_ID_MAX-LS_ID_MIN),LS_ID_MIN,LS_ID_MAX);
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
      h_modcount_vs_LS[i]=new TH1F(TString("h_modcount_vs_LS")+i,"",(LS_ID_MAX-LS_ID_MIN),LS_ID_MIN,LS_ID_MAX);
      h_modcount_vs_LS[i]->Sumw2();
      h_modcount_vs_LS[i]->SetStats(0);
    }
  }

  
  //readData(LS_ID_MIN,LS_ID_MAX);
  readDataHd5(LS_ID_MIN,LS_ID_MAX);
    

  ///Make a Profile , fit , get chi2
  C.SetLogy(1);

  ///total 
  C.Clear();
  h_pcc_vs_LS->GetYaxis()->SetRangeUser(0.01,h_pcc_vs_LS->GetMaximum()*5);
  h_pcc_vs_LS->GetXaxis()->SetTitle("lumi section");
  h_pcc_vs_LS->SetTitle(TString("PCCTOTAL"));
  h_pcc_vs_LS->SetMarkerStyle(8);
  h_pcc_vs_LS->SetMarkerSize(0.5);
  h_pcc_vs_LS->Draw("histp");
  if(fitNoise){
    TF1*F = fitvdM(h_pcc_vs_LS,&outfile);
    F->Draw("lsame");
  }
  C.Print(OutPath+"/fitPCCvdM_"+LS_ID_MIN+"_"+LS_ID_MAX+".png");     

  
  //per module
  if(selectModule>0){
    for (unsigned int i=0;i<NMOD;i++){
      if((selectModule==0 && MODVETO[MODID[i]]==0) || MODID[i]==selectModule){
	h_modcount_vs_LS[i]->GetYaxis()->SetRangeUser(100,h_modcount_vs_LS[i]->GetMaximum()*5);
	h_modcount_vs_LS[i]->GetXaxis()->SetTitle("lumi section");
	h_modcount_vs_LS[i]->SetTitle(TString("")+MODID[i]);
	h_modcount_vs_LS[i]->SetMarkerStyle(8);
	h_modcount_vs_LS[i]->SetMarkerSize(0.5);
	h_modcount_vs_LS[i]->Draw("histp");
	
	if(fitNoise){
	  C.Clear();
	  TF1*F = fitvdM(h_modcount_vs_LS[i],&outfile);
	  F->Draw("lsame");
	  C.Print(OutPath+"/fitPCCvdM_PerModule_"+LS_ID_MIN+"_"+LS_ID_MAX+"_"+MODID[i]+".png");     
	}
      }
    }
  }
   
  //Clean up per module plots
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
      delete h_modcount_vs_LS[i];
    }
  }

  outfile.close(); 

}

  


void readTimeLSMap(){
  TString infile=OutPath+"/time_ls_map.txt";
  ifstream timefile (infile.Data());    
  if (!timefile.is_open()){
    cout << "Unable to open file: "<<infile.Data()<<endl; 
    return;
  }

  int ls,time;
  std::string line;    
  while (std::getline(timefile, line)){
    std::stringstream iss(line);
    iss>>ls>>time;
    timemap[ls]=time;
    //cout<<ls<<" "<<time<<endl;
  }
  
}


void fillPCCvdM(int LS_ID_MIN, int LS_ID_MAX, TGraph *G){
  
  ifstream infile(OutPath+"/bkg_"+LS_ID_MIN+"_"+LS_ID_MAX+".dat");    
  if (!infile.is_open()){
    cout << "Unable to open file: "<<LS_ID_MIN<<","<<LS_ID_MAX<<endl; 
    return;
  }

  
  std::string module;
  float noise=0.;
  float LS=0.;
  int modcounter=0;
  std::string line;    
  while (std::getline(infile, line)){
    std::stringstream iss(line);
    iss>>module>>LS>>noise;
    
    if(module.compare("PCCTOTAL")==0){
      //cout<<module<<" "<<LS<<" "<<noise<<endl;
      G[NMOD].SetPoint(G[NMOD].GetN(),(timemap[LS]-timeref)/3600,noise);
      //G[NMOD].SetPoint(G[NMOD].GetN(),timemap[LS],noise);
    }else{
      G[modcounter].SetPoint(G[modcounter].GetN(),(timemap[LS]-timeref)/3600,noise);
      modcounter++;
    }
  }
  
  infile.close();
  
}


void fitPCCNoise_vdMFill(){

  std::vector<std::pair<int,int>> list;
  
  ///// scan ranges based on the ZB Physics datasets
  list.push_back(std::pair<int,int>(575,584));//SS1  //for SS noise need to fix peak param to 0
  list.push_back(std::pair<int,int>(2343,2357));//SS2
  
  list.push_back(std::pair<int,int>(600,642));//vdM1X
  list.push_back(std::pair<int,int>(659,700));//vdM1Y

  list.push_back(std::pair<int,int>(1720,1760)); //vdM2 ?

  list.push_back(std::pair<int,int>(1793,1832)); //vdM3Y
  list.push_back(std::pair<int,int>(1850,1890));//vdM3X

  list.push_back(std::pair<int,int>(1908,1948));//offset X
  list.push_back(std::pair<int,int>(1966,2008));//offset Y

  list.push_back(std::pair<int,int>(2192,2232));//vdM4X
  list.push_back(std::pair<int,int>(2259,2299));//vdM4Y
   

//  list.push_back(std::pair<int,int>(500,2500));
  
  ///////////////////////////////////////////
  ///run scan fits and save the noise values 
  //for(int i=0;i<list.size();i++)
  //  fitPCCvdM(list[i].first,list[i].second);
  //return;


  ///////////////////////////////////////
  //plot the noise vs time
  readTimeLSMap();
  TGraph G[NMOD+1];
  for(int i=0;i<list.size();i++)
    fillPCCvdM(list[i].first,list[i].second,G);
  
  C.Clear();
  G[NMOD].GetYaxis()->SetTitle("Avg. PCC Background");
  G[NMOD].GetYaxis()->SetRangeUser(0,0.1);
  G[NMOD].GetXaxis()->SetTitle(TString("time [hr]"));
  G[NMOD].SetMarkerStyle(8);
  G[NMOD].SetMarkerSize(0.5);
  G[NMOD].Fit("pol1");
  G[NMOD].Draw("ap");
  C.Print(OutPath+"/fitPCCvdM_NoiseVsLS_PCCTOTAL.png");

  ///bkg linear fit results (y=m*x+b) as function of timestamp [s] (x)
  //p0                        =     -1325.06   +/-   171.462
  //p1                        =  7.94367e-07   +/-   1.02786e-07
  
}
