#include "/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/globals.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>


TString OutPath = "/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp";

///cut on the weights
float weightMin=0.;//1E-5;
float weightMax=0.;

/////for vdM data cut on the noise
//int SSBIN=575;//SS1 2022
//int SSBIN=2345;//SS2 2022
int SSBIN=160;//SS1 2023
float SSBkgMax=0;

//cut on the RMS of per LS data
float RMSThr=0.;

//cut on stability profile per 100 LS
float StabilityThr=0.0;
float StabilityMax=0.03;//for plot range

//cut on linearity profile
float LinearityThr=0.0;
float LinearityMax=0.02;//for plot 

//select layers/disks
int selectLayer=0; //keep modules only in this layer (1,2,3,4), reject FPIX and other layers
int selectDisk=0;  //keep modules only in this disk (1,2,3,4,5,6) , reject Barrel and other disks


///////////////////////////////////////
/// Graph parameters (no module selections below)
TCanvas* C=NULL;
TLine line;
#define MARKERSIZE 0.3
#define MAXMODWEIGHT 2E-2
#define MAXLAYERWEIGHT 0.7
#define NBINMODWEIGHT 3000
#define NLSBLOCK 100
#define NBINTOTPCCAVG 200
bool makeModuleGraphs=0;//total counts per module , not the weights
float StabilityMaxPerMod=0.05;
float StabilityMaxPerLayer=0.05; //0.05
float LinearityMaxPerLayer=0.0;  //0.03

///Period C, D
#define MINTOTPCC 100000000
#define MAXTOTPCC 500000000
#define MINTOTPCCAVG 0
#define MAXTOTPCCAVG 150000

#define LS_ID_MIN 0
#define LS_ID_MAX 11500

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TF1* fitvdM(TH1F* P){
  TF1 *F = new TF1(TString("FitVDM")+P->GetName(), "[0]+[1]*exp(-(x-[2])*(x-[2])/(0.5*[3]*[3]))", P->GetBinCenter(1), P->GetBinCenter(P->GetNbinsX()));
  F->SetLineColor(2);
  F->SetParameter(1,P->GetMaximum());
  F->SetParLimits(1,0.05*P->GetMaximum(),2*P->GetMaximum());
  F->SetParameter(2,( P->GetBinCenter(P->GetNbinsX()) - P->GetBinCenter(1) )/2);
  F->SetParLimits(2,P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  F->SetParameter(3,0.5*P->GetNbinsX());
  F->SetParLimits(3,1,P->GetNbinsX());
  P->Fit(F,"Q","Q",P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  P->SetTitle(TString("")+P->GetTitle()+" ,      B/S = "+((F->GetParameter(0)/F->GetParameter(1))*100)+" %");
  return F;
}


float evaluateModuleStability(TH1F* Num, TH1F* Den, float weight, int idx=0, TString xtitle=""){
  if(Num==NULL || Den==NULL){ cout<<"No histogram idx="<<idx<<endl; return 0.;}


  TH1F P(TString(Num->GetName())+"_p","",Num->GetNbinsX(),Num->GetXaxis()->GetXmin(),Num->GetXaxis()->GetXmax());
  for(int b=1;b<=P.GetNbinsX();b++)
    if(Den->GetBinContent(b)>0)
      P.SetBinContent(b,(Num->GetBinContent(b)/Den->GetBinContent(b))/weight);
  
  float diff=0.;
  int c=0;
  for(int b=1;b<=P.GetNbinsX();b++){
    if(P.GetBinContent(b)>0){ // && P.GetBinCenter(b)< 3000 ){
      if(P.GetBinContent(b)>=1)
	diff += (P.GetBinContent(b)-1);
      else
	diff += (1 - P.GetBinContent(b));
      c++;
    }
  }
  //cout<<"counter : "<<c<<endl;
  if(c>0) diff = diff/c;
  else diff=1000;


  if(idx!=0){
    //P.SetTitle(TString("Module ")+idx+" ID:"+MODID[idx]+",    weight="+int(weight*100000)+"E-5"+ ",     stability="+int(diff*10000)+"E-4");
    P.GetYaxis()->SetTitle("Normalized module weight");
    P.GetXaxis()->SetTitle(xtitle.Data());
    P.SetStats(0);
    P.SetMarkerStyle(8);
    P.SetMarkerSize(MARKERSIZE);
    P.GetYaxis()->SetRangeUser((1-StabilityMaxPerMod),(1+StabilityMaxPerMod));
    //P.GetYaxis()->SetRangeUser((1-0.5),(1+0.5));
    C->Clear();
    P.Draw("histp");
    line.SetLineColor(2);
    line.DrawLine(Num->GetXaxis()->GetXmin(),1,Num->GetXaxis()->GetXmax(),1);
    drawCMSPrelim(0.18,0.85,"#font[62]{CMS} #font[52]{Preliminary}");
    drawFillYear(0,2023);
    drawPCCLuminometer(0.18,0.80,TString("module ")+MODID[idx]);
    C->Print(OutPath+"/Module_RMS_Stability_fit_"+P.GetName()+".png");
  }
  
  return diff;
}

/////////////////////////////////////



////////////////////////////////////

void analysiscode() {

  gROOT->ProcessLine(".x /home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  C=new TCanvas("Canvas","",50,50,800,600);
  TString ModVeto = "/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/veto_2022/veto2022_FStab2p08pLin04p025p_CStab2p1pLin04p03p_EStab06pLin03p_GStab06pLin03p_DStab06pLin03p_vdmStab1p04pNoise05p.txt";
  TString InputPath = "/home/dguzmanl/thesisdav/datos/Run2022D";
  readModRPhiZCoordinates();
  std::vector<int> run_number = {357538, 357539, 357540, 357542, 357550, 357610, 357611, 357612, 357613, 357688, 357696, 357697, 357698, 357699, 357700, 357701, 357704, 357705, 357706, 357720, 357732, 357733, 357734, 357735, 357756, 357757, 357758, 357759, 357767, 357769, 357770, 357776, 357777, 357778, 357779, 357780, 357781, 357802, 357803, 357804, 357805, 357806, 357807, 357808, 357809, 357812, 357813, 357814, 357815, 357898, 357900};
 
  if(!readModVeto(ModVeto)) return;

  


      
 
  // TCanvas *C = new TCanvas("MODVETO_Plot", "MODVETO Plot", 1800, 600);

  //int n = MODID.size()-100;
  //double x[n], y[n];

  //for (int i = 0; i < n; ++i) {
  //    x[i] = i;             // Índice en el eje x
  //    y[i] = MODVETO[i];    // Valor de MODVETO en el eje y
  // }

  //TGraph *graph = new TGraph(n, x, y);
  //graph->SetTitle("MODVETO vs. Index;Index;MODVETO");
  //graph->SetMarkerStyle(20);
  //graph->Draw("AP");

  //C->Draw();


 

   // Imprime la cabecera de la tabla
   //  std::cout << "Module ID\tVeto Status\n";

    // Obtén la longitud de los vectores
  //  size_t length = std::max(MODID.size(), MODVETO.size());

    // Recorre los vectores y muestra el estado de veto para cada módulo
  //  for (size_t i = 0; i < length; ++i) {
        // Si MODID o MODVETO se agotan, usa un valor predeterminado
  //      int moduleID = (i < MODID.size()) ? MODID[i] : -1;
  //      int vetoStatus = (i < MODVETO.size()) ? MODVETO[i] : -1;

  //      std::cout << moduleID << "\t\t" << (vetoStatus == 0 ? "Encendido" : "Apagado") << "\n";
  //  }


  int run=0;
  int ls=0;
  int nls_tot=0.;
  int nls_tot_run=0.;
  int ls_idx=0.;
  float m_count[NMOD];

  TH2F PCC_vs_LS("Histo_totpcc","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,500,0,MAXTOTPCC);
  TH2F PCCPerMod_vs_LS("Histo_totpccavg","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
  
  TH1F * h_modcount[NMOD];
  TGraph ModWeight;
  TGraph ModWeightRMS;
  TH1D hRMS("RMS/Mean", "",100,0,0.2);

  TH1F h_totcount_vs_LS(TString("h_totcount_vs_LS"),"",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX);
  TH1F *h_modcount_vs_LS[NMOD];
  
  TH1F hStabilityDeviation("StabilityDeviation", "",200,0,StabilityMax); 
  TGraph gStabilityDeviation;

  TH1F h_totcount_vs_totpcc(TString("h_totcount_vs_totpcc"),"",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
  TH1F *h_modcount_vs_totpcc[NMOD];
  TGraph gLinearityDeviation;
  TH1F hLinearityDeviation("LinearityDeviation", "",200,0,LinearityMax); 

  TH1F ModVdmNoise("ModVdmNoise","Noise / Signal",100,0,0.05);

  TGraph gModuleNoiseSS;
  float m_modNoiseSS[NMOD];
 
  int ngood_mod=0;
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
      ngood_mod++;
      h_modcount[i]=new TH1F(TString("h_modcount")+i,"",NBINMODWEIGHT,0,MAXMODWEIGHT);
      h_modcount_vs_LS[i]=new TH1F(TString("h_modcount_vs_LS")+i,"",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX);
      h_modcount_vs_LS[i]->Sumw2();
      h_modcount_vs_totpcc[i]=new TH1F(TString("h_modcount_vs_totpcc")+i,"",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
      h_modcount_vs_totpcc[i]->Sumw2();
    }
  }
  cout<<"Number of good modules: "<<ngood_mod<<endl;

  TH2D* histo_L1=new TH2D("Histo_Layer1","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_L2=new TH2D("Histo_Layer2","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_L3=new TH2D("Histo_Layer3","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_L4=new TH2D("Histo_Layer4","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_D1S1=new TH2D("Histo_Disk1S1","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_D2S1=new TH2D("Histo_Disk2S1","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_D3S1=new TH2D("Histo_Disk3S1","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_D1S2=new TH2D("Histo_Disk1S2","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_D2S2=new TH2D("Histo_Disk2S2","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);
  TH2D* histo_D3S2=new TH2D("Histo_Disk3S2","",(LS_ID_MAX-LS_ID_MIN)/NLSBLOCK,LS_ID_MIN,LS_ID_MAX,4000,0,MAXLAYERWEIGHT);

  TH2D* histoLinearity_L1=new TH2D("HistoLinearity_Layer1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_L2=new TH2D("HistoLinearity_Layer2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_L3=new TH2D("HistoLinearity_Layer3","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_L4=new TH2D("HistoLinearity_Layer4","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_D1S1=new TH2D("HistoLinearity_Disk1S1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_D2S1=new TH2D("HistoLinearity_Disk2S1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_D3S1=new TH2D("HistoLinearity_Disk3S1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_D1S2=new TH2D("HistoLinearity_Disk1S2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_D2S2=new TH2D("HistoLinearity_Disk2S2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  TH2D* histoLinearity_D3S2=new TH2D("HistoLinearity_Disk3S2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,2000,0,MAXLAYERWEIGHT);
  

  std::list<int> badruns;
  
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
      float totcount=0;
      float totcountgood=0;
      float count_L1=0;
      float count_L2=0;
      float count_L3=0;
      float count_L4=0;
      float count_D1S1=0;
      float count_D2S1=0;
      float count_D3S1=0;
      float count_D1S2=0;
      float count_D2S2=0;
      float count_D3S2=0;      


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

      // cout << "run:" << run << endl;
      //cout << "ls:" << ls << endl;
      //cout << "ls_idx:" << ls_idx << endl;

      if(ls_idx<5000) badruns.push_back(run);

      
      for (unsigned int i=0;i<NMOD;i++)
	m_count[i]=0.;
      
      for (unsigned int i=0;i<NMOD;i++){
	std::getline(iss,token, ',');
	std::stringstream countiss(token);
	countiss>>m_count[i];
	
	totcount += m_count[i];
       

	if(ls_idx==SSBIN) 
	  m_modNoiseSS[i]=m_count[i];

	
	if(MODVETO[MODID[i]]==0){                                                                                        
	  totcountgood += m_count[i];
	  
	  if(BPIXorFPIX[MODID[i]]==1){
	    if(LY[MODID[i]]==1)	      count_L1+=m_count[i];
	    if(LY[MODID[i]]==2)	      count_L2+=m_count[i];
	    if(LY[MODID[i]]==3)	      count_L3+=m_count[i];
	    if(LY[MODID[i]]==4)	      count_L4+=m_count[i];

	    //if(LY[MODID[i]]==4) totcountgood += m_count[i];
	  }
 	    
	  if(BPIXorFPIX[MODID[i]]==2){
	    if(DISK[MODID[i]]==2)	      count_D1S1+=m_count[i];
	    if(DISK[MODID[i]]==1)	      count_D2S1+=m_count[i];
	    if(DISK[MODID[i]]==0)	      count_D3S1+=m_count[i];
	    if(DISK[MODID[i]]==3)	      count_D1S2+=m_count[i];
	    if(DISK[MODID[i]]==4)	      count_D2S2+=m_count[i];
	    if(DISK[MODID[i]]==5)	      count_D3S2+=m_count[i];	      
	  }  
 
	}
      }

      
 
  
      float totcountgood_avg=float(totcountgood)/ngood_mod;

      // cout << "ngood_mod:" << ngood_mod << endl;
      // cout << "totcount:" << totcount << endl;

      //need to cut ls_idx otherwise code crashes if way out of bounds
      if(LS_ID_MIN < ls_idx && ls_idx < LS_ID_MAX  && ngood_mod>0){
	    
	//	 if(MINTOTPCC < totcount  && totcount < MAXTOTPCC){  corte para baja luminosidad, habilitar segun el ruido
	//cout << "totcount:" << totcount << endl;
       	//cout << "totcountgood_avg:" << totcountgood_avg << endl;
	  PCC_vs_LS.Fill(ls_idx,totcount);
	  PCCPerMod_vs_LS.Fill(ls_idx,totcountgood_avg);
	

	  
	  h_totcount_vs_LS.Fill(ls_idx,totcountgood);//total PCC per LS Block interval
	  h_totcount_vs_totpcc.Fill(totcountgood_avg,totcountgood);///total PCC per inst. Lumi interval
	  for (unsigned int i=0;i<NMOD;i++){
	    if(MODVETO[MODID[i]]==0){
	      h_modcount[i]->Fill(m_count[i]/totcountgood);
	      h_modcount_vs_LS[i]->Fill(ls_idx,m_count[i]);
	      h_modcount_vs_totpcc[i]->Fill(totcountgood_avg,m_count[i]);
	    }
	  }

	     
	  if(StabilityMaxPerLayer>0.){
	    histo_L1->Fill(ls_idx, float(count_L1)/totcountgood);
	    histo_L2->Fill(ls_idx, float(count_L2)/totcountgood);
	    histo_L3->Fill(ls_idx, float(count_L3)/totcountgood);
	    histo_L4->Fill(ls_idx, float(count_L4)/totcountgood);
	    histo_D1S1->Fill(ls_idx, float(count_D1S1)/totcountgood);
	    histo_D2S1->Fill(ls_idx, float(count_D2S1)/totcountgood);
	    histo_D3S1->Fill(ls_idx, float(count_D3S1)/totcountgood);
	    histo_D1S2->Fill(ls_idx, float(count_D1S2)/totcountgood);
	    histo_D2S2->Fill(ls_idx, float(count_D2S2)/totcountgood);
	    histo_D3S2->Fill(ls_idx, float(count_D3S2)/totcountgood);
	  }
	    
	  if(LinearityMaxPerLayer>0.){
	    histoLinearity_L1->Fill(totcountgood_avg, float(count_L1)/totcountgood);
	    histoLinearity_L2->Fill(totcountgood_avg, float(count_L2)/totcountgood);
	    histoLinearity_L3->Fill(totcountgood_avg, float(count_L3)/totcountgood);
	    histoLinearity_L4->Fill(totcountgood_avg, float(count_L4)/totcountgood);
	    histoLinearity_D1S1->Fill(totcountgood_avg, float(count_D1S1)/totcountgood);
	    histoLinearity_D2S1->Fill(totcountgood_avg, float(count_D2S1)/totcountgood);
	    histoLinearity_D3S1->Fill(totcountgood_avg, float(count_D3S1)/totcountgood);
	    histoLinearity_D1S2->Fill(totcountgood_avg, float(count_D1S2)/totcountgood);
	    histoLinearity_D2S2->Fill(totcountgood_avg, float(count_D2S2)/totcountgood);
	    histoLinearity_D3S2->Fill(totcountgood_avg, float(count_D3S2)/totcountgood);
	  }

 
	 
      }
   
    }

    
    csvfile.close();    
    nls_tot_run+=nls_run;
   
    cout<<"# of LS "<<nls_run<<endl;
  }

  cout<<"Total # of LS "<<nls_tot<<endl;
  cout<<"Last LS idx "<<ls_idx<<endl;

  // Imprimir la lista MODID
  //cout << "MODID List:" << endl;
  //for (unsigned int i = 0; i < NMOD; i++) {
  //  cout << MODID[i] << " ";
  //}
  //cout << endl;

//  Imprimir la lista MODVETO
// cout << "MODVETO List:" << endl;
//for (unsigned int i = 0; i < NMOD; i++) {
//    cout << MODVETO[MODID[i]] << " ";
//}
//cout << endl;

  /* badruns.unique(); */
  /* for (std::list<int>::iterator it=badruns.begin(); it!=badruns.end(); ++it) */
  /*   std::cout<< *it << '\n'; */
  
  /////////////////////////////////////////////////////////////////////////////////////////
  
  cout<<"bad modules (veto.tmp) "<< endl;
  std::ofstream vetostream("veto.tmp",std::ofstream::out);
  float max_mod_weight=0;
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){
    
      float mean=h_modcount[i]->GetMean();
      float rms=h_modcount[i]->GetRMS();
 
      ModWeight.SetPoint(ModWeight.GetN(),i, mean);

      //find the largest weight 
      if(mean > max_mod_weight) max_mod_weight = mean;

      ///remove other layers/disks
      if(selectLayer>0 && (BPIXorFPIX[MODID[i]]==2 || LY[MODID[i]]!=selectLayer)) vetostream<<MODID[i]<<endl;

      
      //remove empty modules
      if(weightMin>0. && mean<weightMin) vetostream<<MODID[i]<<endl;
      //Veto modules with high weights
      if(weightMax>0 && mean>weightMax) vetostream<<MODID[i]<<endl;
     
      //Veto modules wight high SS background value in vdM
      gModuleNoiseSS.SetPoint(gModuleNoiseSS.GetN(),i,m_modNoiseSS[i]);
      if(SSBkgMax>0. && m_modNoiseSS[i]>SSBkgMax)  vetostream<<MODID[i]<<endl;
      
      
      ///Fill the weight and RMS graphs
      if(mean>0){
	ModWeightRMS.SetPoint(ModWeightRMS.GetN(), i, rms/mean);
	hRMS.Fill(rms/mean);
 
	///Veto using the RMS/Mean value
	if(RMSThr>0. && rms/mean>RMSThr) vetostream<<MODID[i]<<endl;
      }else {
	ModWeightRMS.SetPoint(ModWeightRMS.GetN(), i, 0);
	hRMS.Fill(0);
      }
      
     
      ///module count vs LS 
      if(makeModuleGraphs){
	C->Clear();
	//h_modcount_vs_LS[i]->SetStats(0);
	h_modcount_vs_LS[i]->GetYaxis()->SetRangeUser(0,1.2*MAXTOTPCCAVG);
	h_modcount_vs_LS[i]->SetTitle(TString("Module ")+MODID[i]);
	h_modcount_vs_LS[i]->Draw("histp");
	C->Print(OutPath+"/Module_RMS_Stability_modulecount_"+i+".png");
      }

    
      ////////////////////////
      /// stability evaluation
      if(StabilityThr>0){
 	float diff_stability = evaluateModuleStability(h_modcount_vs_LS[i],&h_totcount_vs_LS,mean,i,"Lumi Section");
 	gStabilityDeviation.SetPoint(gStabilityDeviation.GetN(),i,diff_stability);
 	hStabilityDeviation.Fill(diff_stability);
 	if(diff_stability>StabilityThr) vetostream<<MODID[i]<<endl;
      }


      ////////////////////////
      ///Linearity evaluation
      if(LinearityThr>0){
	float diff_linearity=evaluateModuleStability(h_modcount_vs_totpcc[i],&h_totcount_vs_totpcc,mean,i,"Avg. PCC per module");
	gLinearityDeviation.SetPoint(gLinearityDeviation.GetN(),i,diff_linearity);
	hLinearityDeviation.Fill(diff_linearity);
	if(diff_linearity>LinearityThr) vetostream<<MODID[i]<<endl;
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
  
  

  /////////////////////////////////////////
  
  C->Clear();
  PCC_vs_LS.GetYaxis()->SetTitle("Total PCC");
  PCC_vs_LS.GetXaxis()->SetTitle("Lumi section");
  PCC_vs_LS.SetStats(0);
  PCC_vs_LS.Draw("scat");
  PCC_vs_LS.Draw("histp");
  C->Print("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_Stability_totalcount.png");

  C->Clear();
  //C->SetLogy(1);
  PCCPerMod_vs_LS.GetYaxis()->SetTitle("Avg. PCC per module");
  PCCPerMod_vs_LS.GetXaxis()->SetTitle("Lumi section");
  PCCPerMod_vs_LS.SetStats(0);
  PCCPerMod_vs_LS.Draw("scat");
  PCCPerMod_vs_LS.Draw("histp");                                                                                                               
  C->Print(OutPath+"/Module_RMS_Stability_totalcountavg.png");
  C->SaveAs("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_Stability_totalcountavg.png");
  //C->SetLogy(0);

  
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
  //C->SetLogy(1);
  ModWeight.Draw("AP"); 
  C->Print("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_Stability_weights.png");
  //C->SetLogy(0);



  if(SSBkgMax>0.){  
    ////Module noise from SS data in vdM
    gModuleNoiseSS.SetMarkerStyle(8);
    gModuleNoiseSS.SetMarkerSize(MARKERSIZE);
    gModuleNoiseSS.GetYaxis()->SetTitle("SS background");
    gModuleNoiseSS.GetXaxis()->SetTitle("Module ID");
    gModuleNoiseSS.GetYaxis()->SetRangeUser(1E-5,10);     
    C->Clear();
    C->SetLogy(1);
    gModuleNoiseSS.Draw("ap");
    C->Print("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_ModuleNoiseSS.png");
    C->SetLogy(0);
  }


  if(RMSThr>0.){
    ////////////////////
    /// Module RMS 
    ModWeightRMS.SetMarkerStyle(8);
    ModWeightRMS.SetMarkerSize(MARKERSIZE);
    ModWeightRMS.GetYaxis()->SetTitle("RMS / Mean");
    ModWeightRMS.GetXaxis()->SetTitle("Module ID");
    ModWeightRMS.GetYaxis()->SetRangeUser(0,StabilityMax);    
    C->Clear();
    ModWeightRMS.Draw("AP");
    C->Print("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_Stability_RMS.png");
    
    hRMS.GetXaxis()->SetTitle("RMS / Mean");
    hRMS.GetYaxis()->SetTitle("# of modules");
    C->Clear();
    hRMS.Draw("hist");
    C->Print("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_Stability_RMS_hist.png");
  }



 
  ///////////////
  ///Stability Fit results
  if(StabilityThr>0){
    gStabilityDeviation.SetMarkerStyle(8);
    gStabilityDeviation.SetMarkerSize(MARKERSIZE);
    gStabilityDeviation.GetYaxis()->SetTitle("Stability Deviation");
    gStabilityDeviation.GetXaxis()->SetTitle("Module ID");
    gStabilityDeviation.GetYaxis()->SetRangeUser(0,StabilityMax);    
    C->Clear();
    gStabilityDeviation.Draw("AP");
    C->Print("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_StabilityDeviation.png");

    gStyle->SetOptStat(1111111);
    hStabilityDeviation.GetXaxis()->SetTitle("Stability Deviation");
    hStabilityDeviation.GetYaxis()->SetTitle("# of modules");
    C->Clear();
    hStabilityDeviation.Draw("hist");
    drawCMSPrelim(0.18,0.85,"#font[62]{CMS} #font[52]{Preliminary}");
    drawFillYear(0,2023);
    drawPCCLuminometer(0.18,0.80);
    C->SetLogy(1);
    C->Print("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_StabilityDeviation_hist.png");
    C->SetLogy(0);
    gStyle->SetOptStat(0);
  }


  
  ///////////////
  ///Linearity Fit results
  if(LinearityThr>0){
    gLinearityDeviation.SetMarkerStyle(8);
    gLinearityDeviation.SetMarkerSize(MARKERSIZE);
    gLinearityDeviation.GetYaxis()->SetTitle("Linearity Deviation");
    gLinearityDeviation.GetXaxis()->SetTitle("Module ID");
    gLinearityDeviation.GetYaxis()->SetRangeUser(0,LinearityMax);    
    C->Clear();
    gLinearityDeviation.Draw("AP");
    C->Print("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_LinearityDeviation.png");

    gStyle->SetOptStat(1111111);
    hLinearityDeviation.GetXaxis()->SetTitle("Linearity Deviation");
    hLinearityDeviation.GetYaxis()->SetTitle("# of modules");
    C->Clear();
    hLinearityDeviation.Draw("hist");
    drawCMSPrelim(0.18,0.85,"#font[62]{CMS} #font[52]{Preliminary}");
    drawFillYear(0,2023);
    drawPCCLuminometer(0.18,0.80);
    C->SetLogy(1);
    C->Print("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_LinearityDeviation_hist.png");
    C->SetLogy(0);
    gStyle->SetOptStat(0);
  }

 

  if(StabilityMaxPerLayer>0.){
    //////////Layer / Disk graphs
    TProfile* P_L2 = histo_L2->ProfileX();
    TProfile* P_L3 = histo_L3->ProfileX();
    TProfile* P_L4 = histo_L4->ProfileX();
    TProfile* P_D1S1 = histo_D1S1->ProfileX();
    TProfile* P_D2S1 = histo_D2S1->ProfileX();
    TProfile* P_D3S1 = histo_D3S1->ProfileX();
    TProfile* P_D1S2 = histo_D1S2->ProfileX();
    TProfile* P_D2S2 = histo_D2S2->ProfileX();
    TProfile* P_D3S2 = histo_D3S2->ProfileX();
   
    cout<<histo_L2->GetMean(2)<<endl;;
    cout<<histo_L3->GetMean(2)<<endl;;
    cout<<histo_L4->GetMean(2)<<endl;;
    cout<<histo_D1S1->GetMean(2)<<endl;;
    cout<<histo_D2S1->GetMean(2)<<endl;;
    cout<<histo_D3S1->GetMean(2)<<endl;;
    cout<<histo_D1S2->GetMean(2)<<endl;;
    cout<<histo_D2S2->GetMean(2)<<endl;;
    cout<<histo_D3S2->GetMean(2)<<endl;;

    P_L2->SetMarkerColor(2);
    P_L3->SetMarkerColor(3);
    P_L4->SetMarkerColor(4);    
    P_D1S1->SetMarkerColor(1);
    P_D2S1->SetMarkerColor(6);
    P_D3S1->SetMarkerColor(7);
    P_D1S2->SetMarkerColor(8);
    P_D2S2->SetMarkerColor(9);
    P_D3S2->SetMarkerColor(11);
   
    P_L2->SetMarkerStyle(8);
    P_L3->SetMarkerStyle(8);
    P_L4->SetMarkerStyle(8);    
    P_D1S1->SetMarkerStyle(8);
    P_D2S1->SetMarkerStyle(8);
    P_D3S1->SetMarkerStyle(8);
    P_D1S2->SetMarkerStyle(8);
    P_D2S2->SetMarkerStyle(8);
    P_D3S2->SetMarkerStyle(8);

    P_L2->SetMarkerSize(MARKERSIZE);
    P_L3->SetMarkerSize(MARKERSIZE);
    P_L4->SetMarkerSize(MARKERSIZE);    
    P_D1S1->SetMarkerSize(MARKERSIZE);
    P_D2S1->SetMarkerSize(MARKERSIZE);
    P_D3S1->SetMarkerSize(MARKERSIZE);
    P_D1S2->SetMarkerSize(MARKERSIZE);
    P_D2S2->SetMarkerSize(MARKERSIZE);
    P_D3S2->SetMarkerSize(MARKERSIZE);
  
    auto legend = new TLegend(0.905,0.2,1,0.92);
    legend->AddEntry(P_L2,"B2","p");
    legend->AddEntry(P_L3,"B3","p");
    legend->AddEntry(P_L4,"B4","p");
    legend->AddEntry(P_D1S1,"F1S1","p");
    legend->AddEntry(P_D2S1,"F2S1","p");
    legend->AddEntry(P_D3S1,"F3S1","p");
    legend->AddEntry(P_D1S2,"F1S2","p");
    legend->AddEntry(P_D2S2,"F2S2","p");
    legend->AddEntry(P_D3S2,"F3S2","p");
    legend->SetFillColor(0);
    legend->SetLineColor(1);
   
    P_L4->SetStats(0);
    P_L4->GetYaxis()->SetRangeUser(0,MAXLAYERWEIGHT);
    P_L4->GetYaxis()->SetTitle("Weight");
    P_L4->GetYaxis()->SetNdivisions(8);
    P_L4->GetXaxis()->SetRangeUser(0,12000);
    P_L4->GetXaxis()->SetTitle("Lumi section");
    C->Clear();
    P_L4->Draw("histp");
    P_L2->Draw("histpsame");
    P_L3->Draw("histpsame");
    P_D1S1->Draw("histpsame");
    P_D2S1->Draw("histpsame");
    P_D3S1->Draw("histpsame");
    P_D1S2->Draw("histpsame");
    P_D2S2->Draw("histpsame");
    P_D3S2->Draw("histpsame");
    legend->Draw();
    C->Print("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_Stability_internal_perlayerdisk_weight.png");  


    P_L2->Scale(1./histo_L2->GetMean(2));;
    P_L3->Scale(1./histo_L3->GetMean(2));;
    P_L4->Scale(1./histo_L4->GetMean(2));;
    P_D1S1->Scale(1./histo_D1S1->GetMean(2));;
    P_D2S1->Scale(1./histo_D2S1->GetMean(2));;
    P_D3S1->Scale(1./histo_D3S1->GetMean(2));;
    P_D1S2->Scale(1./histo_D1S2->GetMean(2));;
    P_D2S2->Scale(1./histo_D2S2->GetMean(2));;
    P_D3S2->Scale(1./histo_D3S2->GetMean(2));;
    P_L4->SetStats(0);
    P_L4->GetYaxis()->SetRangeUser(1-StabilityMaxPerLayer,1+StabilityMaxPerLayer);
    P_L4->GetYaxis()->SetTitle("Layer/Disk Normalized Weight");
    P_L4->GetXaxis()->SetTitle("Lumi section");
 
    C->Clear();
    P_L4->Draw("histp");
    P_L2->Draw("histpsame");
    P_L3->Draw("histpsame");
    P_D1S1->Draw("histpsame");
    P_D2S1->Draw("histpsame");
    P_D3S1->Draw("histpsame");
    P_D1S2->Draw("histpsame");
    P_D2S2->Draw("histpsame");
    P_D3S2->Draw("histpsame");
    legend->Draw();
    C->Print("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_Stability_internal_perlayerdisk.png");

  }


  
  if(LinearityMaxPerLayer>0.){
    //////////////////////////////////////////////////
    ///Linearity
    TProfile* PLinearityL2 = histoLinearity_L2->ProfileX();
    TProfile* PLinearityL3 = histoLinearity_L3->ProfileX();
    TProfile* PLinearityL4 = histoLinearity_L4->ProfileX();
    TProfile* PLinearityD1S1 = histoLinearity_D1S1->ProfileX();
    TProfile* PLinearityD2S1 = histoLinearity_D2S1->ProfileX();
    TProfile* PLinearityD3S1 = histoLinearity_D3S1->ProfileX();
    TProfile* PLinearityD1S2 = histoLinearity_D1S2->ProfileX();
    TProfile* PLinearityD2S2 = histoLinearity_D2S2->ProfileX();
    TProfile* PLinearityD3S2 = histoLinearity_D3S2->ProfileX();

    cout<<histoLinearity_L2->GetMean(2)<<endl;;
    cout<<histoLinearity_L3->GetMean(2)<<endl;;
    cout<<histoLinearity_L4->GetMean(2)<<endl;;
    cout<<histoLinearity_D1S1->GetMean(2)<<endl;;
    cout<<histoLinearity_D2S1->GetMean(2)<<endl;;
    cout<<histoLinearity_D3S1->GetMean(2)<<endl;;
    cout<<histoLinearity_D1S2->GetMean(2)<<endl;;
    cout<<histoLinearity_D2S2->GetMean(2)<<endl;;
    cout<<histoLinearity_D3S2->GetMean(2)<<endl;;

    PLinearityL2->SetMarkerColor(2);
    PLinearityL3->SetMarkerColor(3);
    PLinearityL4->SetMarkerColor(4);    
    PLinearityD1S1->SetMarkerColor(1);
    PLinearityD2S1->SetMarkerColor(6);
    PLinearityD3S1->SetMarkerColor(7);
    PLinearityD1S2->SetMarkerColor(8);
    PLinearityD2S2->SetMarkerColor(9);
    PLinearityD3S2->SetMarkerColor(11);
   
    PLinearityL2->SetLineColor(2);
    PLinearityL3->SetLineColor(3);
    PLinearityL4->SetLineColor(4);    
    PLinearityD1S1->SetLineColor(1);
    PLinearityD2S1->SetLineColor(6);
    PLinearityD3S1->SetLineColor(7);
    PLinearityD1S2->SetLineColor(8);
    PLinearityD2S2->SetLineColor(9);
    PLinearityD3S2->SetLineColor(11);
   
    PLinearityL2->SetMarkerStyle(8);
    PLinearityL3->SetMarkerStyle(8);
    PLinearityL4->SetMarkerStyle(8);    
    PLinearityD1S1->SetMarkerStyle(8);
    PLinearityD2S1->SetMarkerStyle(8);
    PLinearityD3S1->SetMarkerStyle(8);
    PLinearityD1S2->SetMarkerStyle(8);
    PLinearityD2S2->SetMarkerStyle(8);
    PLinearityD3S2->SetMarkerStyle(8);
   
    PLinearityL2->SetMarkerSize(MARKERSIZE);
    PLinearityL3->SetMarkerSize(MARKERSIZE);
    PLinearityL4->SetMarkerSize(MARKERSIZE);    
    PLinearityD1S1->SetMarkerSize(MARKERSIZE);
    PLinearityD2S1->SetMarkerSize(MARKERSIZE);
    PLinearityD3S1->SetMarkerSize(MARKERSIZE);
    PLinearityD1S2->SetMarkerSize(MARKERSIZE);
    PLinearityD2S2->SetMarkerSize(MARKERSIZE);
    PLinearityD3S2->SetMarkerSize(MARKERSIZE);
   
    auto legendLinearity = new TLegend(0.905,0.2,1,0.92);
    legendLinearity->AddEntry(PLinearityL2,"B2","p");
    legendLinearity->AddEntry(PLinearityL3,"B3","p");
    legendLinearity->AddEntry(PLinearityL4,"B4","p");
    legendLinearity->AddEntry(PLinearityD1S1,"F1S1","p");
    legendLinearity->AddEntry(PLinearityD2S1,"F2S1","p");
    legendLinearity->AddEntry(PLinearityD3S1,"F3S1","p");
    legendLinearity->AddEntry(PLinearityD1S2,"F1S2","p");
    legendLinearity->AddEntry(PLinearityD2S2,"F2S2","p");
    legendLinearity->AddEntry(PLinearityD3S2,"F3S2","p");
    legendLinearity->SetFillColor(0);
    legendLinearity->SetLineColor(1);
   

    C->Clear();
    PLinearityL3->SetStats(0);
    PLinearityL3->GetYaxis()->SetRangeUser(0,MAXLAYERWEIGHT);
    PLinearityL3->GetYaxis()->SetTitle("Layer/Disk  Weight");
    PLinearityL3->GetXaxis()->SetTitle("Average PCC per module");    
    PLinearityL3->Draw("histp");
    PLinearityL2->Draw("histpsame");
    PLinearityL4->Draw("histpsame");
    PLinearityD1S1->Draw("histpsame");
    PLinearityD2S1->Draw("histpsame");
    PLinearityD3S1->Draw("histpsame");
    PLinearityD1S2->Draw("histpsame");
    PLinearityD2S2->Draw("histpsame");
    PLinearityD3S2->Draw("histpsame");
    legendLinearity->Draw();
    C->Print(OutPath+"/Module_RMS_Stability_linearity_perlayerdisk_weight.png");    
 
   
    C->Clear();
    PLinearityL2->Scale(1./histoLinearity_L2->GetMean(2));;
    PLinearityL3->Scale(1./histoLinearity_L3->GetMean(2));;
    PLinearityL4->Scale(1./histoLinearity_L4->GetMean(2));;
    PLinearityD1S1->Scale(1./histoLinearity_D1S1->GetMean(2));;
    PLinearityD2S1->Scale(1./histoLinearity_D2S1->GetMean(2));;
    PLinearityD3S1->Scale(1./histoLinearity_D3S1->GetMean(2));;
    PLinearityD1S2->Scale(1./histoLinearity_D1S2->GetMean(2));;
    PLinearityD2S2->Scale(1./histoLinearity_D2S2->GetMean(2));;
    PLinearityD3S2->Scale(1./histoLinearity_D3S2->GetMean(2));;
    PLinearityL3->SetStats(0);
    PLinearityL3->GetYaxis()->SetRangeUser(1-LinearityMaxPerLayer,1+LinearityMaxPerLayer);
    PLinearityL3->GetYaxis()->SetTitle("Layer/Disk Normalized Weight");
    PLinearityL3->GetXaxis()->SetTitle("Average PCC per module");    
    PLinearityL3->Draw("histp");
    PLinearityL2->Draw("histpsame");
    PLinearityL4->Draw("histpsame");
    PLinearityD1S1->Draw("histpsame");
    PLinearityD2S1->Draw("histpsame");
    PLinearityD3S1->Draw("histpsame");
    PLinearityD1S2->Draw("histpsame");
    PLinearityD2S2->Draw("histpsame");
    PLinearityD3S2->Draw("histpsame");
    legendLinearity->Draw();
    C->Print("/home/dguzmanl/BRILAnalysisCode/PCCAnalysis/plots/tmp/Module_RMS_Stability_linearity_perlayerdisk.png");    
    

  }

}
