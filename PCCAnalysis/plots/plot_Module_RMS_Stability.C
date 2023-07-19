#include"globals.h" 
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>

TCanvas C("C");

bool makePerLayerPlots=1;
bool makeModuleFitStability=0;
bool makeModuleFitLinearity=0;

float DeviationMax=0.05;
float DeviationMaxPerLayer=0.05;
float LinearityMax=0.03;
float LinearityMaxPerLayer=0.03;

float RMSThr=100000;
float StabilityThr=10000;
float LinearityThr=10000;


#define NLSBLOCK 10

#define MAXMODWEIGHT 0.005

#define MINTOTPCC 0
#define MAXTOTPCC 500E6

#define MINTOTPCCAVG 70E3 //these two parameters need to be adjusted for different vetoes to select same set of data
#define MAXTOTPCCAVG 190E3
#define NBINTOTPCCAVG 100


//TString OutPath = "/afs/cern.ch/user/b/benitezj/www/BRIL/PCC_lumi/ModuleVeto2022";
TString OutPath = "./ModuleVeto2022";


//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/test/veto_B0.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/test/veto_lateRunD_lowcut_tight_B3.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_CDEFG_3_2022.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022E_1.txt";
//TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022E_Stability2p1p05p.txt";
TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_2022F_Stability2p08p_Linearity04p025p.txt";

//#define NTOTALLS 2500 
//TString InputPath = "./Run2022D";   // "/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022D";
//std::vector<int> run_number = {357538,357539,357540,357542,357550,357610,357611,357688,357697,357698,357700,357706};

#define NTOTALLS 15000
TString InputPath = "./Run2022E"; //"/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022E";  
std::vector<int> run_number = {359602,359603,359604,359608,359609,359611,359612,359614,359663,359664,359684,359685,359686,359693,359699,359717,359718,359751,359764,359776,359806,359808,359812,359813,359814,359871,359998,360017,360018,360019,360075,360090,360116,360121,360122,360123,360124,360126,360128,360129,360131,360141,360225,360295,360327};

//#define NTOTALLS 60000
//TString InputPath = "./Run2022F";//"/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022F";   
//std::vector<int> run_number = {360458,360459,360460}; //fill with linearity issue
//std::vector<int> run_number = {360392,360393,360400,360413,360428,360430,360432,360433,360435,360437,360438,360439,360440,360441,360442,360458,360459,360460,360486,360490,360491,360737,360761,360794,360795,360796,360797,360820,360821,360825,360826,360856,360874,360875,360887,360888,360889,360890,360892,360893,360894,360895,360919,360920,360921,360927,360941,360942,360945,360946,360948,360950,360951,360991,360992,361020,361044,361045,361050,361052,361053,361054,361059,361083,361091,361105,361106,361107,361108,361110,361188,361193,361195,361197,361198,361223,361239,361240,361272,361280,361283,361284,361297,361303,361318,361319,361320,361333,361361,361362,361363,361364,361365,361366,361400,361417,361443,361468,361475,361512,361569,361573,361579,361580,361954,361955,361957,361971,361989,361990,361992,361993,361994,362009,362058,362059,362060,362061,362062,362063,362064,362085,362087,362091,362104,362105,362106,362107,362148,362152,362153,362154,362159,362160,362161,362163,362166,362167}; //all runs - vdm
//std::vector<int> run_number = {3360390,360391,360392,360393,360400,360411,360413,360414,360428,360429,360430,360431,360432,360433,360434,360435,360436,360437,360438,360439,360440,360441,360442,360444,360445,360458,360459,360460,360472,360486,360490,360491,360519,360520,360523,360525,360526,360528,360530,360533,360538,360542,360590,360591,360594,360659,360660,360666,360681,360683,360684,360685,360688,360696,360697,360698,360703,360736,360737,360761,360787,360788,360794,360795,360796,360797,360813,360815,360816,360817,360819,360820,360821,360822,360823,360825,360826,360850,360856,360874,360882,360886,360887,360888,360889,360890,360891,360892,360893,360894,360895,360912,360919,360920,360921,360927,360940,360941,360942,360943,360944,360945,360946,360948,360949,360950,360951,360968,360989,360991,360992,361020,361032,361038,361044,361045,361046,361047,361048,361050,361051,361052,361053,361054,361059,361074,361082,361083,361088,361089,361090,361091,361092,361101,361105,361106,361107,361108,361110,361177,361183,361188,361193,361195,361196,361197,361198,361208,361223,361239,361240,361267,361269,361270,361271,361272,361279,361280,361283,361284,361292,361294,361297,361303,361318,361319,361320,361333,361361,361362,361363,361364,361365,361366,361370,361398,361400,361417,361424,361442,361443,361464,361467,361468,361475,361503,361512,361562,361564,361569,361570,361573,361579,361580,361604,361606,361607,361609,361614,361618,361677,361685,361686,361722,361723,361724,361740,361790,361791,361807,361808,361810,361848,361849,361852,361859,361860,361872,361881,361885,361887,361889,361890,361893,361894,361895,361896,361898,361905,361906,361948,361954,361957,361968,361971,361974,361989,361990,361992,361993,361994,362009,362058,362059,362060,362061,362062,362063,362064,362085,362086,362087,362091,362095,362104,362105,362106,362107,362108,362148,362152,362153,362154,362159,362160,362161,362163,362166,362167}; ///collisions22 menu






float fitModule(TH1F* P, float weight, int idx=0){
  if(P==NULL) return 0.;
  
  for(int b=1;b<=P->GetNbinsX();b++){
    if(!(P->GetBinContent(b)>0. && P->GetBinError(b)/P->GetBinContent(b)<0.03)){
      P->SetBinContent(b,0);
      P->SetBinError(b,0);
    }
    
  }

  int status=-1; 
  
  TF1 F("FitF","pol0",P->GetBinCenter(1),P->GetBinCenter(P->GetNbinsX()));
  F.SetParameter(0,weight); 
  F.SetLineColor(2); 


  float diff=0.;
  int c=0;
  TH1F*hD=(TH1F*)P->Clone(TString(P->GetName())+"Diff");
  hD->SetLineColor(2);
  for(int b=1;b<=P->GetNbinsX();b++){
    float d=0.;
    if(P->GetBinContent(b)>0.){
      if(P->GetBinContent(b)>=weight)
	d = (P->GetBinContent(b)-weight);
      else d = (weight - P->GetBinContent(b));
      diff += d/weight;
      c++;
      
      hD->SetBinContent(b,d);
    }
  }  
  diff = diff/c;


  if(idx!=0){
    P->SetTitle(TString("Module ")+MODID[idx]+",     diff="+int(diff*1000)+" ppm");
    P->GetYaxis()->SetTitle("Module Weight");
    P->SetStats(0);
    P->GetYaxis()->SetRangeUser(0.9*weight,1.1*weight);
    C.Clear();
    P->Draw();
    F.Draw("lsame");
    hD->Draw("histsame");
    C.Print(OutPath+"/Module_RMS_Stability_"+P->GetName()+".png");
  }
  
  return diff;
}
      




void plot_Module_RMS_Stability() {  


  
 readModRPhiZCoordinates();
 
 if(ModVeto.CompareTo("")!=0)
   readModVeto(ModVeto);

 int run=0;
 int ls=0;
 float nls_tot=0.;
 float nls_tot_run=0.;
 float ls_idx=0.;
 float m_count[NMOD];

 TH2F TotalPCC("Histo_totpcc","", NTOTALLS/NLSBLOCK,0,NTOTALLS,1000,0,MAXTOTPCC);
 TH2F TotalPCCAvg("Histo_totpccavg","", NTOTALLS/NLSBLOCK,0,NTOTALLS,1000,0,300e3);  
  

 TH1F *histo_counts[NMOD];
 TGraph ModWeight;
 TGraph ModWeightRMS;
 TH1D hRMS("RMS/Mean", "",100,0,0.2);

 TH1F *h_weightprofile[NMOD];
 TH1F *h_weightprofile_count[NMOD];
 TGraph gStabilityDeviation;
 TH1F hStabilityDeviation("StabilityDeviation", "",200,0,DeviationMax); 

 TH1F *h_weightlinearity[NMOD];
 TH1F *h_weightlinearity_count[NMOD];
 TGraph gLinearityDeviation;
 TH1F hLinearityDeviation("LinearityDeviation", "",200,0,LinearityMax); 

  
 for (unsigned int i=0;i<NMOD;i++)
   if(MODVETO[MODID[i]]==0){
     histo_counts[i]=new TH1F(TString("Histo_counts")+i,"",3000,0,MAXMODWEIGHT);

     h_weightprofile[i]=new TH1F(TString("h_weightprofile")+i,"",NTOTALLS/NLSBLOCK,0,NTOTALLS);
     h_weightprofile_count[i]=new TH1F(TString("h_weightprofile_count")+i,"", NTOTALLS/NLSBLOCK,0,NTOTALLS);
     
     h_weightlinearity[i]=new TH1F(TString("h_weightlinearity")+i,"",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
     h_weightlinearity_count[i]=new TH1F(TString("h_weightlinearity_count")+i,"",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
   }


 TH2F* histo_L1=new TH2F("Histo_Layer1","",NTOTALLS/NLSBLOCK,0,NTOTALLS,1000,0,0.3);
 TH2F* histo_L2=new TH2F("Histo_Layer2","",NTOTALLS/NLSBLOCK,0,NTOTALLS,1000,0,0.3);
 TH2F* histo_L3=new TH2F("Histo_Layer3","",NTOTALLS/NLSBLOCK,0,NTOTALLS,1000,0,0.3);
 TH2F* histo_L4=new TH2F("Histo_Layer4","",NTOTALLS/NLSBLOCK,0,NTOTALLS,1000,0,0.3);
 TH2F* histo_D1S1=new TH2F("Histo_Disk1S1","",NTOTALLS/NLSBLOCK,0,NTOTALLS,1000,0,0.3);
 TH2F* histo_D2S1=new TH2F("Histo_Disk2S1","",NTOTALLS/NLSBLOCK,0,NTOTALLS,1000,0,0.3);
 TH2F* histo_D3S1=new TH2F("Histo_Disk3S1","",NTOTALLS/NLSBLOCK,0,NTOTALLS,1000,0,0.3);
 TH2F* histo_D1S2=new TH2F("Histo_Disk1S2","",NTOTALLS/NLSBLOCK,0,NTOTALLS,1000,0,0.3);
 TH2F* histo_D2S2=new TH2F("Histo_Disk2S2","",NTOTALLS/NLSBLOCK,0,NTOTALLS,1000,0,0.3);
 TH2F* histo_D3S2=new TH2F("Histo_Disk3S2","",NTOTALLS/NLSBLOCK,0,NTOTALLS,1000,0,0.3);

 TH2F* histoLinearity_L1=new TH2F("HistoLinearity_Layer1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,1000,0,0.3);
 TH2F* histoLinearity_L2=new TH2F("HistoLinearity_Layer2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,1000,0,0.3);
 TH2F* histoLinearity_L3=new TH2F("HistoLinearity_Layer3","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,1000,0,0.3);
 TH2F* histoLinearity_L4=new TH2F("HistoLinearity_Layer4","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,1000,0,0.3);
 TH2F* histoLinearity_D1S1=new TH2F("HistoLinearity_Disk1S1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,1000,0,0.3);
 TH2F* histoLinearity_D2S1=new TH2F("HistoLinearity_Disk2S1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,1000,0,0.3);
 TH2F* histoLinearity_D3S1=new TH2F("HistoLinearity_Disk3S1","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,1000,0,0.3);
 TH2F* histoLinearity_D1S2=new TH2F("HistoLinearity_Disk1S2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,1000,0,0.3);
 TH2F* histoLinearity_D2S2=new TH2F("HistoLinearity_Disk2S2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,1000,0,0.3);
 TH2F* histoLinearity_D3S2=new TH2F("HistoLinearity_Disk3S2","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG,1000,0,0.3);
  
    

 cout<<"Reading data from: "<<InputPath<<endl;
 for (unsigned int j=0;j<run_number.size();j++){
   int nls_run=0;
   float maxtotpccav_run =0.;
   
   TString infile=InputPath+"/"+run_number.at(j)+".csv"; 
   std::cout<< run_number.at(j)<<std::endl;  
   ifstream myfile (infile.Data());    
   if (!myfile.is_open()){
     cout << "Unable to open file: "<<infile.Data()<<endl; 
     continue;
   }

    
   std::string line;    
   std::getline(myfile, line);
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
     //cout<<run<<" "<<ls<<endl;
     
     nls_run++;      
     nls_tot++;
     ls_idx = nls_tot_run + ls;
    
     
     for (unsigned int i=0;i<NMOD;i++){
       float count=0;
       std::getline(iss,token, ',');
       std::stringstream countiss(token);
       countiss>>count;
       //cout<<count;

       m_count[i]=0.;
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
     
 
     if(ls_idx<NTOTALLS && ngood_mod>0){
       
       float totcount_avg=float(totcount)/ngood_mod;

       if(totcount_avg > maxtotpccav_run)  maxtotpccav_run = totcount_avg;
       
       if(totcount_avg > MINTOTPCCAVG && totcount_avg < MAXTOTPCCAVG){
 	   
	 TotalPCC.Fill(ls_idx, totcount);
	 TotalPCCAvg.Fill(ls_idx, totcount_avg);
 	   
	 for (unsigned int i=0;i<NMOD;i++){
	   if(MODVETO[MODID[i]]==0){
	     histo_counts[i]->Fill(m_count[i]/totcount);
  
	     h_weightprofile[i]->Fill(ls_idx, m_count[i]);
	     h_weightprofile_count[i]->Fill(ls_idx, totcount);
  
	     //h_weightlinearity[i]->AddBinContent(int(NBINTOTPCCAVG*(totcount_avg - MINTOTPCCAVG)/(MAXTOTPCCAVG - MINTOTPCCAVG)), m_count[i]);
	     //h_weightlinearity_count[i]->AddBinContent(int(NBINTOTPCCAVG*(totcount_avg - MINTOTPCCAVG)/(MAXTOTPCCAVG - MINTOTPCCAVG)), totcount);

	     h_weightlinearity[i]->Fill(totcount_avg, m_count[i]);
	     h_weightlinearity_count[i]->Fill(totcount_avg, totcount);
	   }
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
   cout<<" max pcc Avg "<<maxtotpccav_run<<endl;
 }
 cout<<"Total # of LS "<<nls_tot<<endl;
  

 cout<<"bad modules: "<<endl;
 std::ofstream vetostream("veto.tmp",std::ofstream::out);
  

 ///Make a Profile , fit , get chi2
 for (unsigned int i=0;i<NMOD;i++){
   if(MODVETO[MODID[i]]==0){
     //if(i!=96) continue;
    
     float mean=histo_counts[i]->GetMean();
     float rms=histo_counts[i]->GetRMS();
     if(mean>0){
       ModWeight.SetPoint(ModWeight.GetN(), i, mean);
       ModWeightRMS.SetPoint(ModWeightRMS.GetN(), i, rms/mean);
       hRMS.Fill(rms/mean);
     }
     if(mean ==0. || rms/mean>RMSThr) vetostream<<MODID[i]<<endl;

      
     ////////////////////////
     ///Stability fit
     if(makeModuleFitStability){
       TH1F hwp(TString(h_weightprofile[i]->GetName())+"_p","",NTOTALLS/NLSBLOCK,0,NTOTALLS);
       hwp.GetXaxis()->SetTitle("Lumi section");
       for(int b=1;b<=hwp.GetNbinsX();b++){
	 if(h_weightprofile_count[i]->GetBinContent(b)>0){
	   hwp.SetBinContent(b,h_weightprofile[i]->GetBinContent(b)/h_weightprofile_count[i]->GetBinContent(b));
	   hwp.SetBinError(b,sqrt(h_weightprofile[i]->GetBinContent(b))/h_weightprofile_count[i]->GetBinContent(b));
	 }
       }
       float diff_stability=fitModule(&hwp,mean,i);
       gStabilityDeviation.SetPoint(gStabilityDeviation.GetN(),i,diff_stability);
       hStabilityDeviation.Fill(diff_stability);
       if(diff_stability>StabilityThr) vetostream<<MODID[i]<<endl;
     }


     ////////////////////////
     ///Linearity fit
     if(makeModuleFitLinearity){
       TH1F hlp(TString(h_weightlinearity[i]->GetName())+"_p","",NBINTOTPCCAVG,MINTOTPCCAVG,MAXTOTPCCAVG);
       hlp.GetXaxis()->SetTitle("Avg. PCC per module per LS");
       for(int b=1;b<=hlp.GetNbinsX();b++){
	 if(h_weightlinearity_count[i]->GetBinContent(b)>0){
	   hlp.SetBinContent(b,h_weightlinearity[i]->GetBinContent(b)/h_weightlinearity_count[i]->GetBinContent(b));
	   hlp.SetBinError(b,sqrt(h_weightlinearity[i]->GetBinContent(b))/h_weightlinearity_count[i]->GetBinContent(b));
	 }
       }
       float diff_linearity=fitModule(&hlp,mean,i);
       gLinearityDeviation.SetPoint(gLinearityDeviation.GetN(),i,diff_linearity);
       hLinearityDeviation.Fill(diff_linearity);
       if(diff_linearity>LinearityThr) vetostream<<MODID[i]<<endl;
     }	

      
   }
 }


 vetostream.close();


 //Clean up per module plots
 for (unsigned int i=0;i<NMOD;i++){
   if(MODVETO[MODID[i]]==0){
     delete histo_counts[i];
     delete h_weightprofile[i];
     delete h_weightprofile_count[i];
     delete h_weightlinearity[i];
     delete h_weightlinearity_count[i];
   }
 }
  

  
 /////////////////////////////////////////
 //TotalPCC.SetStats(0);
 //TotalPCC.SetMarkerStyle(8);
 //TotalPCC.SetMarkerSize(0.4);
 //TotalPCC.GetYaxis()->SetTitle("Total PCC per LS");
 //TotalPCC.GetXaxis()->SetTitle("Lumi section");
 //TotalPCC.GetYaxis()->SetRangeUser(0,MAXTOTPCC);
 TProfile* TotalPCC_P=TotalPCC.ProfileX();
 TotalPCC_P->SetStats(0);
 TotalPCC_P->GetYaxis()->SetTitle("Total PCC per LS");
 TotalPCC_P->GetXaxis()->SetTitle("Lumi section");
 C.Clear();
 TotalPCC_P->Draw("histp");                                                                                                                   
 C.Print(OutPath+"/Module_RMS_Stability_totalcount.png");

 //TotalPCCAvg.SetStats(0);
 //TotalPCCAvg.SetMarkerStyle(8);
 //TotalPCCAvg.SetMarkerSize(0.4);
 //TotalPCCAvg.GetYaxis()->SetTitle("Avg. PCC per module per LS");
 //TotalPCCAvg.GetXaxis()->SetTitle("Lumi section");
 //TotalPCCAvg.GetYaxis()->SetRangeUser(0,1e6);
 TProfile* TotalPCCAvg_P=TotalPCCAvg.ProfileX();
 TotalPCCAvg_P->SetStats(0);
 TotalPCCAvg_P->GetYaxis()->SetTitle("Avg. PCC per module per LS");
 TotalPCCAvg_P->GetXaxis()->SetTitle("Lumi section");
 C.Clear();
 TotalPCCAvg_P->Draw("histp");                                                                                                                   
 C.Print(OutPath+"/Module_RMS_Stability_totalcountavg.png");

 
 ModWeight.SetMarkerStyle(8);
 ModWeight.SetMarkerSize(0.5);
 ModWeight.GetXaxis()->SetTitle("Module ID");                                                                                 
 ModWeight.GetYaxis()->SetTitle("Module Weight");
 ModWeight.GetYaxis()->SetRangeUser(0, MAXMODWEIGHT);
 C.Clear();
 ModWeight.Draw("AP"); 
 C.Print(OutPath+"/Module_RMS_Stability_weights.png");


 ////////////////
 /// Stability RMS results
 ModWeightRMS.SetMarkerStyle(8);
 ModWeightRMS.SetMarkerSize(0.5);
 ModWeightRMS.GetYaxis()->SetTitle("RMS / Mean");
 ModWeightRMS.GetXaxis()->SetTitle("Module ID");
 ModWeightRMS.GetYaxis()->SetRangeUser(0,DeviationMax);    
 C.Clear();
 ModWeightRMS.Draw("AP");
 C.Print(OutPath+"/Module_RMS_Stability_RMS.png");

 hRMS.GetXaxis()->SetTitle("RMS / Mean");
 hRMS.GetYaxis()->SetTitle("# of modules");
 C.Clear();
 hRMS.Draw("hist");
 C.Print(OutPath+"/Module_RMS_Stability_RMS_hist.png");




 ///////////////
 ///Stability Fit results
 gStabilityDeviation.SetMarkerStyle(8);
 gStabilityDeviation.SetMarkerSize(0.5);
 gStabilityDeviation.GetYaxis()->SetTitle("Stability Deviation");
 gStabilityDeviation.GetXaxis()->SetTitle("Module ID");
 gStabilityDeviation.GetYaxis()->SetRangeUser(0,DeviationMax);    
 C.Clear();
 gStabilityDeviation.Draw("AP");
 C.Print(OutPath+"/Module_RMS_StabilityDeviation.png");

 hStabilityDeviation.GetXaxis()->SetTitle("Stability Deviation");
 hStabilityDeviation.GetYaxis()->SetTitle("# of modules");
 C.Clear();
 hStabilityDeviation.Draw("hist");
 C.SetLogy(1);
 C.Print(OutPath+"/Module_RMS_StabilityDeviation_hist.png");
 C.SetLogy(0);


  
 ///////////////
 ///Linearity Fit results
 gLinearityDeviation.SetMarkerStyle(8);
 gLinearityDeviation.SetMarkerSize(0.5);
 gLinearityDeviation.GetYaxis()->SetTitle("Linearity Deviation");
 gLinearityDeviation.GetXaxis()->SetTitle("Module ID");
 gLinearityDeviation.GetYaxis()->SetRangeUser(0,LinearityMax);    
 C.Clear();
 gLinearityDeviation.Draw("AP");
 C.Print(OutPath+"/Module_RMS_LinearityDeviation.png");

 hLinearityDeviation.GetXaxis()->SetTitle("Linearity Deviation");
 hLinearityDeviation.GetYaxis()->SetTitle("# of modules");
 C.Clear();
 hLinearityDeviation.Draw("hist");
 C.SetLogy(1);
 C.Print(OutPath+"/Module_RMS_LinearityDeviation_hist.png");
 C.SetLogy(0);


 
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
   P_L1->GetYaxis()->SetRangeUser(1-DeviationMaxPerLayer,1+DeviationMaxPerLayer);
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
   PLinearityL1->GetYaxis()->SetRangeUser(1-LinearityMaxPerLayer,1+LinearityMaxPerLayer);
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
    


 }



  
}

  

