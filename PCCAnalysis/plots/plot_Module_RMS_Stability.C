#include"globals.h" 
#include <iostream>
#include <vector>
#include <dirent.h>

bool makePerLayerPlots=1;

float RMSThr=100000;
float StabilityThr=0.03;
float SlopeThr=10000; //3v3 = 0.05, 3v3_4 = 0.03

float DeviationMax=0.2;

#define NTOTALLS 8000
#define NLSBLOCK 50
#define MAXTOTPCC 500E6 
#define MINTOTPCC 0
#define MINTOTPCCAVG 1E3
#define MAXTOTPCCAVG 300E3


//TString OutPath = "/afs/cern.ch/user/b/benitezj/www/BRIL/PCC_lumi/ModuleVeto2022";
TString OutPath = "./ModuleVeto2022";

TCanvas C("C");
  
//TF1 Pfit("Pfit","[0]+[1]*x",MINTOTPCC,MAXTOTPCC);


float fitModule(TProfile* P, float weight, int idx=0){
  
  for(int b=1;b<=P->GetNbinsX();b++)
    if(!(P->GetBinContent(b)>0. && P->GetBinError(b)/P->GetBinContent(b)<0.02) ){
      P->SetBinContent(b,0);
      P->SetBinError(b,0);
    }

  TF1 F("FitF","[0]",P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  F.SetLineColor(2);
  
  F.SetParameter(0,weight);
  int status=P->Fit(&F,"Q","",P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  
  float diff=0.;
  for(int b=1;b<=P->GetNbinsX();b++)
    if(P->GetBinContent(b)>0){
      diff = ((P->GetBinContent(b)-F.Eval(P->GetBinCenter(b)))/weight);
      diff *=diff;
    }
  diff = sqrt(diff);


  if(idx!=0){
    P->SetTitle(TString("Module ")+MODID[idx]+",    fit status="+status);
    P->GetYaxis()->SetTitle("Module Weight");
    //P->GetXaxis()->SetTitle("Lumi section");
    P->SetStats(0);
    P->GetYaxis()->SetRangeUser(0.8*weight,1.2*weight);
    C.Clear();
    P->Draw();
    C.Print(OutPath+"/Module_RMS_Stability_module_stabilityfit_"+idx+".png");
  }
  
  return diff;
}
      


void plot_Module_RMS_Stability() {


  //TString ModVeto = "BRILAnalysisCode/PCCAnalysis/test/veto_B0.txt";
  //TString ModVeto = "BRILAnalysisCode/PCCAnalysis/test/veto_lateRunD_lowcut_tight_B3.txt";
  //TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_CDEFG_3_2022.txt";
  //TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022E_1.txt";

  TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022E_Stability3p.txt";
  
  //TString Path = "./Run2022D";// "/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022D";
  //std::vector<int> run_number = {357538,357539,357540,357542,357550,357610,357611,357688,357697,357698,357700,357706};

  TString Path = "./Run2022E"; //"/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022E";  
  std::vector<int> run_number = {359602,359603,359604,359608,359609,359611,359612,359614,359663,359664,359684,359685,359686,359693,359699,359717,359718,359751,359764,359776,359806,359808,359812,359813,359814,359871,359998,360017,360018,360019,360075,360090,360116,360121,360122,360123,360124,360126,360128,360129,360131,360141,360225,360295,360327};

  //TString Path = "/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022F";  
  //std::vector<int> run_number = {360458,360459,360460};
  //std::vector<int> run_number = {360390,360391,360392,360393,360400,360413,360428,360430,360432,360433,360435,360437,360438,360439,360440,360441,360442,360458,360459,360460,360486,360490,360491,360737,360761,360794,360795,360796,360797,360820,360821,360825,360826,360856,360874,360875,360876,360887,360888,360889,360890,360892,360893,360894,360895,360919,360920,360921,360927,360941,360942,360945,360946,360948,360950,360951,360991,360992,361020,361044,361045,361050,361052,361053,361054,361059,361083,361091,361105,361106,361107,361108,361110,361188,361193,361195,361197,361198,361223,361239,361240,361272,361280,361283,361284,361297,361303,361318,361319,361320,361333,361361,361362,361363,361364,361365,361366,361400,361417,361443,361468,361475,361512,361569,361573,361579,361580,361881,361882,361883,361884,361885,361886,361887,361889,361890,361893,361906,361909,361910,361912,361913,361915,361916,361917,361919,361921,361922,361923,361925,361926,361927,361929,361932,361933,361948,361953,361954,361955,361956,361957,361971,361989,361990,361992,361993,361994,362009,362058,362059,362060,362061,362062,362063,362064,362085,362087,362091,362104,362105,362106,362107,362148,362152,362153,362154,362159,362160,362161,362163,362166,362167};



  /////////////////////////////////////////
  readModRPhiZCoordinates();
 
  if(ModVeto.CompareTo("")!=0)
    readModVeto(ModVeto);

  int run=0;
  int ls=0;
  int nls_tot=0;
  int nls_tot_run=0;
  int ls_idx=0;
  float m_count[NMOD];

  TH2F* TotalPCC=new TH2F("Histo_totpcc","", NTOTALLS/NLSBLOCK,0,NTOTALLS,300,0,MAXTOTPCC);
  TH2F* TotalPCCAvg=new TH2F("Histo_totpccavg","", NTOTALLS/NLSBLOCK,0,NTOTALLS,300,0,300e3);  
  
  TGraph *ModWeight=new TGraph();

  TGraph *ModWeightRMS=new TGraph();
  TH1D *hRMS=new TH1D("RMS/Mean", "",100,0,0.2);
  TH2F *histo_counts[NMOD];
  for (unsigned int i=0;i<NMOD;i++)
    if(MODVETO[MODID[i]]==0){
      histo_counts[i]=new TH2F(TString("Histo_counts")+i,"", NTOTALLS/NLSBLOCK,0,NTOTALLS,100,0,0.01);
      histo_counts[i]->GetXaxis()->SetTitle("Lumi section");
    }

  
  TGraph* gStabilityStatus=new TGraph();
  TGraph* gStabilityDeviation=new TGraph();
  TH1D* hStabilityDeviation=new TH1D("StabilityDeviation", "",100,0,DeviationMax); 

  
  TGraph *ModWeightSlope=new TGraph();
  TH1D *hSlope=new TH1D("LinearityDeviation", "",100,0,DeviationMax); 
  TH1D *hSlopeResidual=new TH1D("LinearityResidual", "",100,-10,10);
  TH2F *histo_slope[NMOD];
  for (unsigned int i=0;i<NMOD;i++)
    if(MODVETO[MODID[i]]==0)
      histo_slope[i]=new TH2F(TString("Histo_counts_linearity")+i,"",50,MINTOTPCC,MAXTOTPCC,100,0,0.01);
  

  TH2F* histo_L1=new TH2F("Histo_Layer1","",NTOTALLS/NLSBLOCK,0,NTOTALLS,200,0,1.1);
  TH2F* histo_L2=new TH2F("Histo_Layer2","",NTOTALLS/NLSBLOCK,0,NTOTALLS,200,0,1.1);
  TH2F* histo_L3=new TH2F("Histo_Layer3","",NTOTALLS/NLSBLOCK,0,NTOTALLS,200,0,1.1);
  TH2F* histo_L4=new TH2F("Histo_Layer4","",NTOTALLS/NLSBLOCK,0,NTOTALLS,200,0,1.1);
  TH2F* histo_D1S1=new TH2F("Histo_Disk1S1","",NTOTALLS/NLSBLOCK,0,NTOTALLS,200,0,1.1);
  TH2F* histo_D2S1=new TH2F("Histo_Disk2S1","",NTOTALLS/NLSBLOCK,0,NTOTALLS,200,0,1.1);
  TH2F* histo_D3S1=new TH2F("Histo_Disk3S1","",NTOTALLS/NLSBLOCK,0,NTOTALLS,200,0,1.1);
  TH2F* histo_D1S2=new TH2F("Histo_Disk1S2","",NTOTALLS/NLSBLOCK,0,NTOTALLS,200,0,1.1);
  TH2F* histo_D2S2=new TH2F("Histo_Disk2S2","",NTOTALLS/NLSBLOCK,0,NTOTALLS,200,0,1.1);
  TH2F* histo_D3S2=new TH2F("Histo_Disk3S2","",NTOTALLS/NLSBLOCK,0,NTOTALLS,200,0,1.1);

  TH2F* histoLinearity_L1=new TH2F("HistoLinearity_Layer1","",200,MINTOTPCCAVG,MAXTOTPCCAVG,200,0,1.1);
  TH2F* histoLinearity_L2=new TH2F("HistoLinearity_Layer2","",200,MINTOTPCCAVG,MAXTOTPCCAVG,200,0,1.1);
  TH2F* histoLinearity_L3=new TH2F("HistoLinearity_Layer3","",200,MINTOTPCCAVG,MAXTOTPCCAVG,200,0,1.1);
  TH2F* histoLinearity_L4=new TH2F("HistoLinearity_Layer4","",200,MINTOTPCCAVG,MAXTOTPCCAVG,200,0,1.1);
  TH2F* histoLinearity_D1S1=new TH2F("HistoLinearity_Disk1S1","",200,MINTOTPCCAVG,MAXTOTPCCAVG,200,0,1.1);
  TH2F* histoLinearity_D2S1=new TH2F("HistoLinearity_Disk2S1","",200,MINTOTPCCAVG,MAXTOTPCCAVG,200,0,1.1);
  TH2F* histoLinearity_D3S1=new TH2F("HistoLinearity_Disk3S1","",200,MINTOTPCCAVG,MAXTOTPCCAVG,200,0,1.1);
  TH2F* histoLinearity_D1S2=new TH2F("HistoLinearity_Disk1S2","",200,MINTOTPCCAVG,MAXTOTPCCAVG,200,0,1.1);
  TH2F* histoLinearity_D2S2=new TH2F("HistoLinearity_Disk2S2","",200,MINTOTPCCAVG,MAXTOTPCCAVG,200,0,1.1);
  TH2F* histoLinearity_D3S2=new TH2F("HistoLinearity_Disk3S2","",200,MINTOTPCCAVG,MAXTOTPCCAVG,200,0,1.1);
  
    

  for (unsigned int j=0;j<run_number.size();j++){
    TString infile=Path+"/"+run_number.at(j)+".csv"; 
    std::cout<< run_number.at(j)<<std::endl;  
    
    ifstream myfile (infile.Data());    
    if (!myfile.is_open()){
      cout << "Unable to open file: "<<infile.Data()<<endl; 
      continue;
    }
    
    std::string line;    
    std::getline(myfile, line);
    std::stringstream iss(line);
    std::string token;


    int nls_run=0;
    while (std::getline(myfile, line)){

      float totcount=0;      
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

      int ngood_mod=0;
 
      
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
      ls_idx=nls_tot_run+ls;
     
      float count=0;
      for (unsigned int i=0;i<NMOD;i++){
	std::getline(iss,token, ',');
	std::stringstream countiss(token);
        countiss>>count;

	m_count[i]=0;
	if(MODVETO[MODID[i]]==0){
	  m_count[i]=count;                                                                                       
	  totcount+=count;
	  ngood_mod++;

	  if(makePerLayerPlots){
	    if(BPIXorFPIX[MODID[i]]==1){
	      if(LY[MODID[i]]==1)	      count_L1+=count;
	      if(LY[MODID[i]]==2)	      count_L2+=count;
	      if(LY[MODID[i]]==3)	      count_L3+=count;
	      if(LY[MODID[i]]==4)	      count_L4+=count;
	    }
	    
	    if(BPIXorFPIX[MODID[i]]==2){
	      if(DISK[MODID[i]]==2)	      count_D1S1+=count;
	      if(DISK[MODID[i]]==1)	      count_D2S1+=count;
	      if(DISK[MODID[i]]==0)	      count_D3S1+=count;
	      if(DISK[MODID[i]]==3)	      count_D1S2+=count;
	      if(DISK[MODID[i]]==4)	      count_D2S2+=count;
	      if(DISK[MODID[i]]==5)	      count_D3S2+=count;	      
	    }  
	  }

	}
      }


      if(ngood_mod>0){
	 float totcount_avg=totcount/ngood_mod;
	 
	 if(totcount_avg > MINTOTPCCAVG){
	   
	   TotalPCC->Fill(ls_idx, totcount);
	   TotalPCCAvg->Fill(ls_idx, totcount_avg);
	   
	   for (unsigned int i=0;i<NMOD;i++)
	     if(MODVETO[MODID[i]]==0){
	       histo_counts[i]->Fill(ls_idx, m_count[i]/totcount);
	       histo_slope[i]->Fill(totcount, m_count[i]/totcount);
	     }
	   
	   if(makePerLayerPlots){
	     histo_L1->Fill(ls_idx, count_L1/totcount);
	     histo_L2->Fill(ls_idx, count_L2/totcount);
	     histo_L3->Fill(ls_idx, count_L3/totcount);
	     histo_L4->Fill(ls_idx, count_L4/totcount);
	     histo_D1S1->Fill(ls_idx, count_D1S1/totcount);
	     histo_D2S1->Fill(ls_idx, count_D2S1/totcount);
	     histo_D3S1->Fill(ls_idx, count_D3S1/totcount);
	     histo_D1S2->Fill(ls_idx, count_D1S2/totcount);
	     histo_D2S2->Fill(ls_idx, count_D2S2/totcount);
	     histo_D3S2->Fill(ls_idx, count_D3S2/totcount);
	     
	     histoLinearity_L1->Fill(totcount_avg, count_L1/totcount);
	     histoLinearity_L2->Fill(totcount_avg, count_L2/totcount);
	     histoLinearity_L3->Fill(totcount_avg, count_L3/totcount);
	     histoLinearity_L4->Fill(totcount_avg, count_L4/totcount);
	     histoLinearity_D1S1->Fill(totcount_avg, count_D1S1/totcount);
	     histoLinearity_D2S1->Fill(totcount_avg, count_D2S1/totcount);
	     histoLinearity_D3S1->Fill(totcount_avg, count_D3S1/totcount);
	     histoLinearity_D1S2->Fill(totcount_avg, count_D1S2/totcount);
	     histoLinearity_D2S2->Fill(totcount_avg, count_D2S2/totcount);
	     histoLinearity_D3S2->Fill(totcount_avg, count_D3S2/totcount);
	   }
	   
	 }
      
      }
    }
     
    myfile.close();    

    nls_tot_run+=nls_run;

    cout<<"# of LS "<<nls_run<<endl;
  }
  cout<<"Total # of LS "<<nls_tot<<endl;



  cout<<"bad modules: "<<endl;


  ///Make a Profile , fit , get chi2
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){      
      float mean=histo_counts[i]->GetMean(2);
      float rms=histo_counts[i]->GetRMS(2);
      if(mean>0){
	ModWeight->SetPoint(ModWeight->GetN(), i, mean);
	ModWeightRMS->SetPoint(ModWeightRMS->GetN(), i, rms/mean);
	hRMS->Fill(rms/mean);
      }
      if(rms/mean>RMSThr) cout<<MODID[i]<<endl;

      ////////////////////////
      ///Stability fit
      TProfile* P_stability=histo_counts[i]->ProfileX(TString("Histo_Profile_Stability")+i);      
      float diff_stability=fitModule(P_stability,mean);
      gStabilityDeviation->SetPoint(gStabilityDeviation->GetN(),i,diff_stability);
      hStabilityDeviation->Fill(diff_stability);
      if(diff_stability>StabilityThr) cout<<MODID[i]<<endl;
	

      ////////////////////////
      /// linearity fit
      
//      TProfile*P=histo_slope[i]->ProfileX(TString("Histo_Profile_Linearity")+i);
//      for(int b=1;b<=P->GetNbinsX();b++)
//      	if(!(P->GetBinContent(b)>0. && P->GetBinError(b)/P->GetBinContent(b)<0.03) ){
//      	  P->SetBinContent(b,0);
//      	  P->SetBinError(b,0);
//      	}
//      P->Fit(&Pfit,"Q","",MINTOTPCC,MAXTOTPCC);

      
//         P->SetTitle(TString("Module ")+MODID[i]);
//         P->GetYaxis()->SetTitle("Module weight");
//         P->GetXaxis()->SetTitle("Total PCC");
//         P->SetStats(0);
//         P->GetYaxis()->SetRangeUser(0.95*mean,1.05*mean);
//         C.Clear();
//         P->Draw();
//         C.Print(OutPath+"/Module_RMS_Stability_module_slopefit_"+i+".png");


      ///module selection based on the chi2 of the  linear fit
      // ModWeightSlope->SetPoint(ModWeightSlope->GetN(), i, Pfit.GetChisquare()/Pfit.GetNDF());
      // hSlope->Fill(Pfit.GetChisquare()/Pfit.GetNDF());
      // if(Pfit.GetChisquare()/Pfit.GetNDF()>SlopeThr) cout<<MODID[i]<<endl;


//      //calculate the avg diff in % of weight
//      float diff_avg=0.;
//      for(int b=1;b<=P->GetNbinsX();b++)
//        if(P->GetBinContent(b)>0){
//	  diff_avg += ((P->GetBinContent(b)-Pfit.Eval(P->GetBinCenter(b)))/mean) * ((P->GetBinContent(b)-Pfit.Eval(P->GetBinCenter(b)))/mean) ;
//	}
//      diff_avg = sqrt(diff_avg);
//      ModWeightSlope->SetPoint(ModWeightSlope->GetN(), i,diff_avg);
//      hSlope->Fill(diff_avg);
//      if(diff_avg>SlopeThr) cout<<MODID[i]<<endl;
//
//      for(int b=1;b<=P->GetNbinsX();b++)
//        if(P->GetBinError(b)>0)
//	  hSlopeResidual->Fill((P->GetBinContent(b)-Pfit.Eval(P->GetBinCenter(b)))/P->GetBinError(b));
//
      
    }
  }





  TotalPCC->SetStats(0);
  TotalPCC->SetMarkerStyle(8);
  TotalPCC->SetMarkerSize(0.4);
  TotalPCC->GetYaxis()->SetTitle("Total PCC per LS");
  TotalPCC->GetXaxis()->SetTitle("Lumi section");
  TotalPCC->GetYaxis()->SetRangeUser(0,MAXTOTPCC);
  TProfile* TotalPCC_P=TotalPCC->ProfileX();
  TotalPCC_P->SetStats(0);
  TotalPCC_P->GetYaxis()->SetTitle("Total PCC per LS");
  C.Clear();
  TotalPCC_P->Draw("histp");                                                                                                                   
  C.Print(OutPath+"/Module_RMS_Stability_totalcount.png");

  TotalPCCAvg->SetStats(0);
  TotalPCCAvg->SetMarkerStyle(8);
  TotalPCCAvg->SetMarkerSize(0.4);
  TotalPCCAvg->GetYaxis()->SetTitle("Avg. PCC per module per LS");
  TotalPCCAvg->GetXaxis()->SetTitle("Lumi section");
  TotalPCCAvg->GetYaxis()->SetRangeUser(0,1e6);
  TProfile* TotalPCCAvg_P=TotalPCCAvg->ProfileX();
  TotalPCCAvg_P->SetStats(0);
  TotalPCCAvg_P->GetYaxis()->SetTitle("Avg. PCC per module per LS");
  C.Clear();
  TotalPCCAvg_P->Draw("histp");                                                                                                                   
  C.Print(OutPath+"/Module_RMS_Stability_totalcountavg.png");

 
  ModWeight->SetMarkerStyle(8);
  ModWeight->SetMarkerSize(0.5);
  ModWeight->GetXaxis()->SetTitle("Module ID");                                                                                 
  ModWeight->GetYaxis()->SetTitle("Module Weight");
  ModWeight->GetYaxis()->SetRangeUser(0, 0.008);
  C.Clear();
  ModWeight->Draw("AP"); 
  C.Print(OutPath+"/Module_RMS_Stability_weights.png");


  ////////////////
  /// Stability RMS results
  ModWeightRMS->SetMarkerStyle(8);
  ModWeightRMS->SetMarkerSize(0.5);
  ModWeightRMS->GetYaxis()->SetTitle("RMS / Mean");
  ModWeightRMS->GetXaxis()->SetTitle("Module ID");
  ModWeightRMS->GetYaxis()->SetRangeUser(0,DeviationMax);    
  C.Clear();
  ModWeightRMS->Draw("AP");
  C.Print(OutPath+"/Module_RMS_Stability_RMS.png");

  hRMS->GetXaxis()->SetTitle("RMS / Mean");
  hRMS->GetYaxis()->SetTitle("# of modules");
  C.Clear();
  hRMS->Draw("hist");
  C.Print(OutPath+"/Module_RMS_Stability_RMS_hist.png");




  ///////////////
  ///Stability Fit results
  gStabilityDeviation->SetMarkerStyle(8);
  gStabilityDeviation->SetMarkerSize(0.5);
  gStabilityDeviation->GetYaxis()->SetTitle("Stability Deviation");
  gStabilityDeviation->GetXaxis()->SetTitle("Module ID");
  gStabilityDeviation->GetYaxis()->SetRangeUser(0,DeviationMax);    
  C.Clear();
  gStabilityDeviation->Draw("AP");
  C.Print(OutPath+"/Module_RMS_StabilityDeviation.png");

  hStabilityDeviation->GetXaxis()->SetTitle("Stability Deviation");
  hStabilityDeviation->GetYaxis()->SetTitle("# of modules");
  C.Clear();
  hStabilityDeviation->Draw("hist");
  C.Print(OutPath+"/Module_RMS_StabilityDeviation_hist.png");

  
  ////////////////
  ///Linearity fit results
  ModWeightSlope->SetMarkerStyle(8);
  ModWeightSlope->SetMarkerSize(0.5);
  ModWeightSlope->GetYaxis()->SetTitle("Avg. Linearity Deviation");
  ModWeightSlope->GetXaxis()->SetTitle("Module ID");
  ModWeightSlope->GetYaxis()->SetRangeUser(0,DeviationMax);    
  C.Clear();
  ModWeightSlope->Draw("AP");
  C.Print(OutPath+"/Module_RMS_Stability_Slope.png");

  hSlope->GetXaxis()->SetTitle("Avg. Linearity Deviation");
  hSlope->GetYaxis()->SetTitle("# of modules");
  C.Clear();
  hSlope->Draw("hist");
  C.Print(OutPath+"/Module_RMS_Stability_Slope_hist.png");

  /* hSlopeResidual->GetXaxis()->SetTitle("pull"); */
  /* hSlopeResidual->GetYaxis()->SetTitle("counts"); */
  /* C.Clear(); */
  /* hSlopeResidual->Draw("hist"); */
  /* C.Print(OutPath+"/Module_RMS_Stability_SlopePull_hist.png"); */

  

  if(makePerLayerPlots){

    //////////Layer / Disk graphs
    TProfile* P_L1 = histo_L1->ProfileX();
    TProfile* P_L2 = histo_L2->ProfileX();
    TProfile* P_L3 = histo_L3->ProfileX();
    TProfile* P_L4 = histo_L4->ProfileX();
    TProfile* P_D1S1 = histo_D1S1->ProfileX();
    TProfile* P_D2S1 = histo_D2S1->ProfileX();
    TProfile* P_D3S1 = histo_D3S1->ProfileX();
    TProfile* P_D1S2 = histo_D1S2->ProfileX();
    TProfile* P_D2S2 = histo_D2S2->ProfileX();
    TProfile* P_D3S2 = histo_D3S2->ProfileX();
    
    cout<<histo_L1->GetMean(2)<<endl;;
    cout<<histo_L2->GetMean(2)<<endl;;
    cout<<histo_L3->GetMean(2)<<endl;;
    cout<<histo_L4->GetMean(2)<<endl;;
    cout<<histo_D1S1->GetMean(2)<<endl;;
    cout<<histo_D2S1->GetMean(2)<<endl;;
    cout<<histo_D3S1->GetMean(2)<<endl;;
    cout<<histo_D1S2->GetMean(2)<<endl;;
    cout<<histo_D2S2->GetMean(2)<<endl;;
    cout<<histo_D3S2->GetMean(2)<<endl;;


     //P_L1->Scale(1./histo_L1->GetMean(2));;
     P_L2->Scale(1./histo_L2->GetMean(2));;
     P_L3->Scale(1./histo_L3->GetMean(2));;
     P_L4->Scale(1./histo_L4->GetMean(2));;
     P_D1S1->Scale(1./histo_D1S1->GetMean(2));;
     P_D2S1->Scale(1./histo_D2S1->GetMean(2));;
     P_D3S1->Scale(1./histo_D3S1->GetMean(2));;
     P_D1S2->Scale(1./histo_D1S2->GetMean(2));;
     P_D2S2->Scale(1./histo_D2S2->GetMean(2));;
     P_D3S2->Scale(1./histo_D3S2->GetMean(2));;
     
    //P_L1->SetMarkerColor(1);
    P_L2->SetMarkerColor(2);
    P_L3->SetMarkerColor(3);
    P_L4->SetMarkerColor(4);    
    P_D1S1->SetMarkerColor(1);
    P_D2S1->SetMarkerColor(6);
    P_D3S1->SetMarkerColor(7);
    P_D1S2->SetMarkerColor(8);
    P_D2S2->SetMarkerColor(9);
    P_D3S2->SetMarkerColor(11);
    
    P_L1->SetMarkerStyle(8);
    P_L2->SetMarkerStyle(8);
    P_L3->SetMarkerStyle(8);
    P_L4->SetMarkerStyle(8);    
    P_D1S1->SetMarkerStyle(8);
    P_D2S1->SetMarkerStyle(8);
    P_D3S1->SetMarkerStyle(8);
    P_D1S2->SetMarkerStyle(8);
    P_D2S2->SetMarkerStyle(8);
    P_D3S2->SetMarkerStyle(8);

    P_L1->SetMarkerSize(0.5);
    P_L2->SetMarkerSize(0.5);
    P_L3->SetMarkerSize(0.5);
    P_L4->SetMarkerSize(0.5);    
    P_D1S1->SetMarkerSize(0.5);
    P_D2S1->SetMarkerSize(0.5);
    P_D3S1->SetMarkerSize(0.5);
    P_D1S2->SetMarkerSize(0.5);
    P_D2S2->SetMarkerSize(0.5);
    P_D3S2->SetMarkerSize(0.5);
    
    auto legend = new TLegend(0.905,0.2,1,0.92);
    //legend->AddEntry(P_L1,"B1","p");
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
    
    
    P_L1->SetStats(0);
    //P_L1->GetYaxis()->SetRangeUser(0.05,0.20);
    P_L1->GetYaxis()->SetRangeUser(0.95,1.05);
    P_L1->GetYaxis()->SetTitle("Layer/Disk Normalized Weight");
    P_L1->GetXaxis()->SetTitle("Lumi section");
    C.Clear();
    P_L1->Draw("histp");
    P_L2->Draw("histpsame");
    P_L3->Draw("histpsame");
    P_L4->Draw("histpsame");
    P_D1S1->Draw("histpsame");
    P_D2S1->Draw("histpsame");
    P_D3S1->Draw("histpsame");
    P_D1S2->Draw("histpsame");
    P_D2S2->Draw("histpsame");
    P_D3S2->Draw("histpsame");
    legend->Draw();
    C.Print(OutPath+"/Module_RMS_Stability_internal_perlayerdisk.png");    
    



    //////////////////////////////////////////////////
    ///Linearity
    TProfile* PLinearityL1 = histoLinearity_L1->ProfileX();
    TProfile* PLinearityL2 = histoLinearity_L2->ProfileX();
    TProfile* PLinearityL3 = histoLinearity_L3->ProfileX();
    TProfile* PLinearityL4 = histoLinearity_L4->ProfileX();
    TProfile* PLinearityD1S1 = histoLinearity_D1S1->ProfileX();
    TProfile* PLinearityD2S1 = histoLinearity_D2S1->ProfileX();
    TProfile* PLinearityD3S1 = histoLinearity_D3S1->ProfileX();
    TProfile* PLinearityD1S2 = histoLinearity_D1S2->ProfileX();
    TProfile* PLinearityD2S2 = histoLinearity_D2S2->ProfileX();
    TProfile* PLinearityD3S2 = histoLinearity_D3S2->ProfileX();

    cout<<histoLinearity_L1->GetMean(2)<<endl;;
    cout<<histoLinearity_L2->GetMean(2)<<endl;;
    cout<<histoLinearity_L3->GetMean(2)<<endl;;
    cout<<histoLinearity_L4->GetMean(2)<<endl;;
    cout<<histoLinearity_D1S1->GetMean(2)<<endl;;
    cout<<histoLinearity_D2S1->GetMean(2)<<endl;;
    cout<<histoLinearity_D3S1->GetMean(2)<<endl;;
    cout<<histoLinearity_D1S2->GetMean(2)<<endl;;
    cout<<histoLinearity_D2S2->GetMean(2)<<endl;;
    cout<<histoLinearity_D3S2->GetMean(2)<<endl;;


     //PLinearityL1->Scale(1./histoLinearity_L1->GetMean(2));;
     PLinearityL2->Scale(1./histoLinearity_L2->GetMean(2));;
     PLinearityL3->Scale(1./histoLinearity_L3->GetMean(2));;
     PLinearityL4->Scale(1./histoLinearity_L4->GetMean(2));;
     PLinearityD1S1->Scale(1./histoLinearity_D1S1->GetMean(2));;
     PLinearityD2S1->Scale(1./histoLinearity_D2S1->GetMean(2));;
     PLinearityD3S1->Scale(1./histoLinearity_D3S1->GetMean(2));;
     PLinearityD1S2->Scale(1./histoLinearity_D1S2->GetMean(2));;
     PLinearityD2S2->Scale(1./histoLinearity_D2S2->GetMean(2));;
     PLinearityD3S2->Scale(1./histoLinearity_D3S2->GetMean(2));;
     
     
    //PLinearityL1->SetMarkerColor(1);
    PLinearityL2->SetMarkerColor(2);
    PLinearityL3->SetMarkerColor(3);
    PLinearityL4->SetMarkerColor(4);    
    PLinearityD1S1->SetMarkerColor(1);
    PLinearityD2S1->SetMarkerColor(6);
    PLinearityD3S1->SetMarkerColor(7);
    PLinearityD1S2->SetMarkerColor(8);
    PLinearityD2S2->SetMarkerColor(9);
    PLinearityD3S2->SetMarkerColor(11);
    
    //PLinearityL1->SetLineColor(1);
    PLinearityL2->SetLineColor(2);
    PLinearityL3->SetLineColor(3);
    PLinearityL4->SetLineColor(4);    
    PLinearityD1S1->SetLineColor(1);
    PLinearityD2S1->SetLineColor(6);
    PLinearityD3S1->SetLineColor(7);
    PLinearityD1S2->SetLineColor(8);
    PLinearityD2S2->SetLineColor(9);
    PLinearityD3S2->SetLineColor(11);
    
    PLinearityL1->SetMarkerStyle(8);
    PLinearityL2->SetMarkerStyle(8);
    PLinearityL3->SetMarkerStyle(8);
    PLinearityL4->SetMarkerStyle(8);    
    PLinearityD1S1->SetMarkerStyle(8);
    PLinearityD2S1->SetMarkerStyle(8);
    PLinearityD3S1->SetMarkerStyle(8);
    PLinearityD1S2->SetMarkerStyle(8);
    PLinearityD2S2->SetMarkerStyle(8);
    PLinearityD3S2->SetMarkerStyle(8);
    
    PLinearityL1->SetMarkerSize(0.5);
    PLinearityL2->SetMarkerSize(0.5);
    PLinearityL3->SetMarkerSize(0.5);
    PLinearityL4->SetMarkerSize(0.5);    
    PLinearityD1S1->SetMarkerSize(0.5);
    PLinearityD2S1->SetMarkerSize(0.5);
    PLinearityD3S1->SetMarkerSize(0.5);
    PLinearityD1S2->SetMarkerSize(0.5);
    PLinearityD2S2->SetMarkerSize(0.5);
    PLinearityD3S2->SetMarkerSize(0.5);
    
    auto legendLinearity = new TLegend(0.905,0.2,1,0.92);
    //legendLinearity->AddEntry(PLinearityL1,"B1","p");
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


 
     
     PLinearityL1->SetStats(0);
     //PLinearityL1->GetYaxis()->SetRangeUser(0.05,0.20);
     PLinearityL1->GetYaxis()->SetRangeUser(0.95,1.05);
     PLinearityL1->GetYaxis()->SetTitle("Layer/Disk Normalized Weight");
     PLinearityL1->GetXaxis()->SetTitle("Avg. PCC per module per LS");
     
     C.Clear();
     PLinearityL1->Draw("histp");
     PLinearityL2->Draw("histpsame");
     PLinearityL3->Draw("histpsame");
     PLinearityL4->Draw("histpsame");
     PLinearityD1S1->Draw("histpsame");
     PLinearityD2S1->Draw("histpsame");
     PLinearityD3S1->Draw("histpsame");
     PLinearityD1S2->Draw("histpsame");
     PLinearityD2S2->Draw("histpsame");
     PLinearityD3S2->Draw("histpsame");
     legendLinearity->Draw();
     C.Print(OutPath+"/Module_RMS_Stability_linearity_perlayerdisk.png");    
     
     /* C.Clear(); */
     /* PLinearityL1->Draw("histp"); */
     /* PLinearityL2->Draw("histpsame"); */
     /* PLinearityL3->Draw("histpsame"); */
     /* PLinearityL4->Draw("histpsame"); */
     /* legendLinearity->Draw(); */
     /* C.Print(OutPath+"/Module_RMS_Stability_linearity_perlayerdisk_BPIX.png");     */
     
     /* C.Clear(); */
     /* PLinearityL1->Draw("histp"); */
     /* PLinearityD1S1->Draw("histpsame"); */
     /* PLinearityD2S1->Draw("histpsame"); */
     /* PLinearityD3S1->Draw("histpsame"); */
     /* legendLinearity->Draw(); */
     /* C.Print(OutPath+"/Module_RMS_Stability_linearity_perlayerdisk_FPIXS1.png");     */
     
     /* C.Clear(); */
     /* PLinearityL1->Draw("histp"); */
     /* PLinearityD1S2->Draw("histpsame"); */
     /* PLinearityD2S2->Draw("histpsame"); */
     /* PLinearityD3S2->Draw("histpsame"); */
     /* legendLinearity->Draw(); */
     /* C.Print(OutPath+"/Module_RMS_Stability_linearity_perlayerdisk_FPIXS2.png");     */

  }



  
}

  

