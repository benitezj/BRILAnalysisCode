#include"globals.h" 
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
//#include "modules_361913.h"

TString OutPath = "/eos/user/b/benitezj/www/plots/BRIL/PCC_lumi/tmp";
//TString OutPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/Reprocess2024/PromptReco/vdM/stability";
//TString OutPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/Reprocess2024/PromptReco/All_Blocks/Layers";
//TString OutPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/Reprocess2024/PromptReco/All_Blocks/test/Lumi_days";

////////////////////////////////////////
//Module selections
///////////////////////////////////////

//cut on linearity profile
float LinearityThr_Low=-0.003;
float LinearityThr=0.01; //0.002 (Luis)
float LinearityMax=0.01; //for histogram distribution
float LinearityMin=-0.01;

float ForSlopePerModThr=0.02;//clean ratios >XX% from 1
float ForSlopePerModThrErr=0.01;  //clean points with bad error bars

int histomax=20;
int spare_cut=100; 


///////////////////////////////////////
/// Graph parameters (no module selections below)
TCanvas* C=NULL;
TLine line;
#define MARKERSIZE 0.3
#define MAXMODWEIGHT 2E-2
#define NBINMODWEIGHT 3000
#define NLSBLOCK 10
#define NBINTOTPCCAVG 100
bool makeModuleGraphs=1;
int selectModuleForGraphs=0;//306196508;//Module id of interest

float StabilityMaxPerMod=.05;

//TString ModVeto = "../test/veto_B0.txt";//initial veto
TString ModVeto ="./BRILAnalysisCode/PCCAnalysis/veto_2024/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2_+Block2_Sty1_Sty2+Block3_Sty1_Sty2+Block4_Sty1_Sty2+Block5_Sty1_Sty2+Block6_Sty1_Sty2+All_Blocks_Sty.txt"; 



//all Blocks
#define MINTOTPCC 0 //7000 //6000                                                             
#define MAXTOTPCC 20000 //20000                                                           
#define MINTOTPCCAVG 0
#define MAXTOTPCCAVG 5
TString InputPath = "/eos/user/l/lcuevasp/BRIL/PCC_Run3/AlCaPCCZeroBias-PromptReco/Blocks/All_Blocks";                

#define LS_ID_MIN 1200       //1250        //
#define LS_ID_MAX 3400    //281000 for complete year

std::vector<int> run_number = {385281,385282,385284,385285,385286};  //fill for limi days



///////////////////////////////////
float evaluateModuleStability(TGraphErrors* P, float weight, int idx=0, TString xtitle=""){
  if(P==NULL){ cout<<"No histogram idx="<<idx<<endl; return 0.;}

  TGraphErrors Filter;
  int n=0;
  for (int g = 1; g <= P->GetN(); g++) {                                         
    float binContent = P->GetPointY(g)/weight;
    float binErr = P->GetErrorY(g)/weight;                                          
    if (binContent >= 1-ForSlopePerModThr
	&& binContent <= 1+ForSlopePerModThr
	&& binErr > 0.0005
	&& binErr < ForSlopePerModThrErr
	) {                          
      Filter.SetPoint(n, P->GetPointX(g), binContent);
      Filter.SetPointError(n, 0, binErr);
      n++;
    }                                                                                
  }                                                                                  

  
  TF1 *linearFit = new TF1("linearFit", "pol1",MINTOTPCCAVG,MAXTOTPCCAVG);// Filter.GetXaxis()->GetXmin(), Filter.GetXaxis()->GetXmax());        
  Filter.Fit(linearFit,"Q","");                                                        
  linearFit->SetLineColor(2);                                                        
  double slope = linearFit->GetParameter(1);                                         
  double intercept = linearFit->GetParameter(0);                                     

  if(idx!=0){

    TString title = TString("Module ") + MODID[idx] + TString(" Weight=") +int(weight * 100000) + TString("E-5 Linearity=") + int(slope * 100000) + TString("E-5");                                                                                                                                                                                              
    TString title_Lty = TString("Module ") + MODID[idx] + TString("slope=") + int(slope * 100000) + TString("E-5") + TString("Intercept=") + int(intercept);

    TString histoname = Form("Filter_%d", MODID[idx]);
    
    Filter.SetName(histoname);
    TFile* file = new TFile(OutPath+"/PerModuleStability_veto_all_Blocks_Lty.root", "UPDATE");
    Filter.Write();
    file->Close();  

    if(makeModuleGraphs){
      Filter.GetYaxis()->SetRangeUser(1-ForSlopePerModThr,1+ForSlopePerModThr);
      Filter.GetYaxis()->SetTitle("Normalized module weight");                       
      Filter.GetXaxis()->SetTitle(xtitle.Data());                                    
      Filter.SetMarkerStyle(8);                                                      
      Filter.SetMarkerSize(MARKERSIZE);                                              
      Filter.SetTitle(title);
      
      C->Clear();
      Filter.Draw("ap");
      linearFit->Draw("same");      
      line.SetLineColor(1);
      line.DrawLine(MINTOTPCCAVG,1,MAXTOTPCCAVG,1);
      C->Print(OutPath+"/Module_RMS_Stability_fit_"+idx+".png");
    }
    
  }

  return slope;
}
      




void plot_Module_RMS_Stability_Lty_Jose() {  
  
  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  C=new TCanvas("Canvas","",50,50,800,600);
  
  readModRPhiZCoordinates();
 
  if(!readModVeto(ModVeto)) return;

  int run=0;
  int ls=0;
  int nls_tot=0.;
  int nls_tot_run=0.;
  int ls_idx=0.;
  float m_count[NMOD];

  TH2F PCC_vs_LS("Histo_totpcc","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,500,0,MAXTOTPCC);
  TH2F PCCGood_vs_LS("Histo_totpccgood","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,500,0,MAXTOTPCC);
  TH2F PCCPerMod_vs_LS("Histo_totpccavg","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
  
  TH1F * h_modcount[NMOD];
  TGraph ModWeight;
  TH1F h_totcount_vs_LS(TString("h_totcount_vs_LS"),"",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX);
  TH2F *h_modcount_vs_LS[NMOD];
  TH2F *h_modweight_vs_LS[NMOD];
  
  TH1F h_totcount_vs_totpcc(TString("h_totcount_vs_totpcc"),"",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
  TH2F *h_modcount_vs_totpcc[NMOD];
  TH2F *h_modw_vs_totpcc[NMOD];
  TGraph * g_modw_vs_totpcc[NMOD];  
  TGraph gLinearityDeviation;
  TH1F hLinearityDeviation("LinearityDeviation", "",200,0,LinearityMax); 
  TGraph gLinearityDeviation_slope;                                                                                           
  TH1F hLinearityDeviation_slope("LinearityDeviation_slope", "",200,LinearityMin,LinearityMax);
  
  int ngood_mod=0;
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
      ngood_mod++;
      h_modcount[i]=new TH1F(TString("h_modcount")+i,"",NBINMODWEIGHT,0,MAXMODWEIGHT);

      h_modcount_vs_LS[i]=new TH2F(TString("h_modcount_vs_LS")+i,"",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,400,0,10);
      h_modcount_vs_LS[i]->Sumw2();
      
      h_modweight_vs_LS[i]=new TH2F(TString("h_modweight_vs_LS")+i,"",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,400,0,MAXMODWEIGHT);
      h_modweight_vs_LS[i]->Sumw2();

      h_modcount_vs_totpcc[i]=new TH2F(TString("h_modcount_vs_totpcc")+i,"",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,400,0,10);
      h_modcount_vs_totpcc[i]->Sumw2();

      h_modw_vs_totpcc[i]=new TH2F(TString("h_modw_vs_totpcc")+i,"",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,400,0,MAXMODWEIGHT);
      h_modw_vs_totpcc[i]->Sumw2();
      
      g_modw_vs_totpcc[i]=new TGraph();
      g_modw_vs_totpcc[i]->SetName(TString("g_modw_vs_totpcc_")+i);
	    
    }
  }
  cout<<"Number of good modules: "<<ngood_mod<<endl;

  
  unsigned int ls_count = 0;
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

      
      float totcount=0;
      float totcountgood=0;

      for (unsigned int i=0;i<NMOD;i++)
	m_count[i]=0.;
      
      for (unsigned int i=0;i<NMOD;i++){
	std::getline(iss,token, ',');
	std::stringstream countiss(token);
	countiss>>m_count[i];
	totcount += m_count[i];

	if(MODVETO[MODID[i]]==0){
	  totcountgood += m_count[i];
	}
      }
      
      
      //	 //&& totcount > MINTOTPCC  && totcount < MAXTOTPCC
      float totcountgood_avg =0;
      if(ngood_mod>0) totcountgood_avg = float(totcountgood)/ngood_mod;
	
      if(   ls_idx > LS_ID_MIN 
 	 && ls_idx < LS_ID_MAX
	 && totcountgood_avg > MINTOTPCCAVG
	 && totcountgood_avg < MAXTOTPCCAVG
	 ){
	ls_count++;
	
	PCC_vs_LS.Fill(ls_idx,totcount);
	PCCGood_vs_LS.Fill(ls_idx,totcountgood);
	PCCPerMod_vs_LS.Fill(ls_idx,totcountgood_avg);
	
	h_totcount_vs_LS.Fill(ls_idx,totcountgood);
	h_totcount_vs_totpcc.Fill(totcountgood_avg,totcountgood);
	
	for (unsigned int i=0;i<NMOD;i++){
	  if(MODVETO[MODID[i]]==0){
	    h_modcount[i]->Fill(m_count[i]/totcountgood);

	    h_modcount_vs_LS[i]->Fill(ls_idx,m_count[i]);
	    h_modweight_vs_LS[i]->Fill(ls_idx,m_count[i]/totcountgood);
	    
	    h_modcount_vs_totpcc[i]->Fill(totcountgood_avg,m_count[i]);
	    h_modw_vs_totpcc[i]->Fill(totcountgood_avg,m_count[i]/totcountgood);
	    
	    g_modw_vs_totpcc[i]->SetPoint(g_modw_vs_totpcc[i]->GetN(),totcountgood_avg,m_count[i]/totcountgood);
	  }
	}
      }
    }

    
    csvfile.close();    
    nls_tot_run+=nls_run;
   
    cout<<"# of LS "<<nls_run<<endl;
  }
  cout<<"Total # of LS "<<nls_tot<<endl;
  cout<<"Last LS idx "<<ls_idx<<endl;
  std::cout << "Total de veces que se ejecuto el bloque para ls_idx: " << ls_count << std::endl;

  
  cout<<"bad modules (veto.tmp) "<<endl;
  std::ofstream vetostream("veto.tmp",std::ofstream::out);
 
  float max_mod_weight=0;
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){

      if(! (selectModuleForGraphs==0 || MODID[i]==selectModuleForGraphs)) continue; 
      
      //float mean=h_modcount[i]->GetMean();
      float mean = h_modcount[i]->GetBinCenter(h_modcount[i]->GetMaximumBin());
      float rms=h_modcount[i]->GetRMS();

      ModWeight.SetPoint(ModWeight.GetN(),i, mean);

      //find the largest weight 
      if(mean > max_mod_weight) max_mod_weight = mean;
      
     
      if(makeModuleGraphs){
	C->Clear();
	h_modcount[i]->SetTitle(TString("Module ")+MODID[i]);
	h_modcount[i]->GetXaxis()->SetTitle("PCC fraction");
	h_modcount[i]->Draw("hist");
	C->Print(OutPath+"/Module_Stability_modulecount_"+i+".png");
	
	C->Clear();
	h_modcount_vs_LS[i]->GetXaxis()->SetTitle("lumi section");
	h_modcount_vs_LS[i]->GetYaxis()->SetTitle("PCC");
	h_modcount_vs_LS[i]->SetTitle(TString("Module ")+MODID[i]);
	h_modcount_vs_LS[i]->Draw("histp");
	C->Print(OutPath+"/Module_Stability_modulecount_vsLS_"+i+".png");

	C->Clear();
	h_modweight_vs_LS[i]->GetXaxis()->SetTitle("lumi section");
	h_modweight_vs_LS[i]->GetYaxis()->SetTitle("module weight");
	h_modweight_vs_LS[i]->GetYaxis()->SetRangeUser(0.5*mean,1.5*mean);
	h_modweight_vs_LS[i]->SetTitle(TString("Module ")+MODID[i]);
	h_modweight_vs_LS[i]->Draw("histp");
	line.DrawLine(LS_ID_MIN,mean,LS_ID_MAX,mean);              
	C->Print(OutPath+"/Module_Stability_moduleweight_vsLS_"+i+".png");
	
	C->Clear();
	h_modw_vs_totpcc[i]->Divide(&h_totcount_vs_totpcc);
	h_modw_vs_totpcc[i]->GetXaxis()->SetTitle("<PCC> per module");
	h_modw_vs_totpcc[i]->GetYaxis()->SetTitle("module weight");
	h_modw_vs_totpcc[i]->GetYaxis()->SetRangeUser(0.5*mean,1.5*mean);
	h_modw_vs_totpcc[i]->SetTitle(TString("Module ")+MODID[i]);
	h_modw_vs_totpcc[i]->Draw("histp");
	line.DrawLine(MINTOTPCCAVG,mean,MAXTOTPCCAVG,mean);              
	C->Print(OutPath+"/Module_Stability_moduleweight_vsPCC_"+i+".png");
      }

    
      if(LinearityThr>0){
	for(int k=1;k<=h_modw_vs_totpcc[i]->GetNbinsX();k++){
	  for(int j=1;j<=h_modw_vs_totpcc[i]->GetNbinsY();j++){
	    if(h_modw_vs_totpcc[i]->GetYaxis()->GetBinCenter(j)<mean*0.98
	       || h_modw_vs_totpcc[i]->GetYaxis()->GetBinCenter(j)>mean*1.02)
	      h_modw_vs_totpcc[i]->SetBinContent(k,j,0);
	  }
	}
	
	TProfile * P = h_modw_vs_totpcc[i]->ProfileX();
	TGraphErrors G;
	for(int b=1;b<=P->GetNbinsX();b++){
	  G.SetPoint(b,P->GetBinCenter(b),P->GetBinContent(b));
	  G.SetPointError(b,0,P->GetBinError(b));
	}
	float diff_linearity=evaluateModuleStability(&G,h_modw_vs_totpcc[i]->GetMean(2),i,"<PCC> per module");
	
	//float diff_linearity=evaluateModuleStability(g_modw_vs_totpcc[i],mean,i,"<PCC> per module");

	gLinearityDeviation_slope.SetPoint(gLinearityDeviation_slope.GetN(),i,diff_linearity);      
        hLinearityDeviation_slope.Fill(diff_linearity);  
	if(diff_linearity>LinearityThr || diff_linearity<LinearityThr_Low) vetostream<<MODID[i]<<endl; 
      }	
      
    }
  }
  cout<<"Max module weight: "<<max_mod_weight<<endl;

  vetostream.close();


  
  //Clean up per module plots
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
      delete h_modcount[i];
      delete h_modcount_vs_LS[i];
      delete h_modcount_vs_totpcc[i];
    }
  }
  

  TFile* file = new TFile(OutPath+"/PerModuleStability_veto_all_Blocks_Lty.root", "UPDATE");

  C->Clear();
  PCC_vs_LS.GetYaxis()->SetTitle("Total PCC");
  PCC_vs_LS.GetXaxis()->SetTitle("Lumi section");
  PCC_vs_LS.SetStats(0);
  PCC_vs_LS.Write();
  PCC_vs_LS.Draw("scat");
  C->Print(OutPath+"/Module_RMS_Stability_totalcount.png");

  C->Clear();
  PCCGood_vs_LS.GetYaxis()->SetTitle("Total PCC");
  PCCGood_vs_LS.GetXaxis()->SetTitle("Lumi section");
  PCCGood_vs_LS.SetStats(0);
  PCCGood_vs_LS.Write();
  PCCGood_vs_LS.Draw("scat");
  C->Print(OutPath+"/Module_RMS_Stability_totalcountgood.png");

  C->Clear();
  PCCPerMod_vs_LS.GetYaxis()->SetTitle("<PCC> per module");
  PCCPerMod_vs_LS.GetXaxis()->SetTitle("Lumi section");
  PCCPerMod_vs_LS.SetStats(0);
  PCCPerMod_vs_LS.Write();
  PCCPerMod_vs_LS.Draw("scat");
  C->Print(OutPath+"/Module_RMS_Stability_totalcountavg.png");

  
  ModWeight.SetMarkerStyle(8);
  ModWeight.SetMarkerSize(MARKERSIZE);
  ModWeight.SetMarkerColor(4);
  ModWeight.GetXaxis()->SetTitle("Module ID");                                                                                 
  ModWeight.GetYaxis()->SetTitle("Module Weight");
  ModWeight.GetYaxis()->SetRangeUser(-.0001,.01);
  C->Clear();
  ModWeight.Draw("AP"); 
  C->Print(OutPath+"/Module_RMS_Stability_weights.png");

  
  if(LinearityThr>0){
    
    gLinearityDeviation_slope.SetMarkerStyle(8);                         
    gLinearityDeviation_slope.SetMarkerSize(MARKERSIZE);                
    gLinearityDeviation_slope.GetYaxis()->SetTitle("Linearity Deviation");
    gLinearityDeviation_slope.GetXaxis()->SetTitle("Module ID");          
    gLinearityDeviation_slope.GetYaxis()->SetRangeUser(LinearityMin,LinearityMax);   
    C->Clear();                                                           
    line.SetLineColor(2);                                                 
    line.SetLineWidth(3);                                                 
    gLinearityDeviation_slope.Write();
    gLinearityDeviation_slope.Draw("AP");
    line.DrawLine(spare_cut,LinearityThr_Low,2000,LinearityThr_Low);
    line.DrawLine(spare_cut,LinearityThr,2000,LinearityThr);              
    C->Print(OutPath+"/Module_RMS_LinearityDeviation_slope.png");         

    
    gStyle->SetOptStat(1111111);                                          
    hLinearityDeviation_slope.GetXaxis()->SetTitle("Linearity Deviation");
    hLinearityDeviation_slope.GetYaxis()->SetTitle("# of modules");   
    C->Clear();                                                       
    line.SetLineColor(2);                                             
    line.SetLineWidth(3);                                             
    hLinearityDeviation_slope.Write();
    hLinearityDeviation_slope.Draw("hist");
    line.DrawLine(LinearityThr_Low,0,LinearityThr_Low,histomax);
    line.DrawLine(LinearityThr,0,LinearityThr,histomax);
    C->SetLogy(1);                                                   
    C->Print(OutPath+"/Module_RMS_LinearityDeviation_hist_slope.png");                                       
  }     
  

  file->Close();
}

  

