#include"globals.h" 
#include <iostream>
#include <vector>
#include <dirent.h>

//#define MAXTOTPCC 400E6 //Layer0 veto
//#define MINTOTPCC 100E6
#define MAXTOTPCC 120E6 //2022 CDEFG veto
#define MINTOTPCC 50E6

#define NTotalLS 12000
#define NLSBLOCK 20

void plot_Module_RMS_Stability() {
  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/rootlogon.C");

  TString OutPath = "/afs/cern.ch/user/b/benitezj/www/BRIL/PCC_lumi/ModuleVeto2022";
  //TString ModVeto = "BRILAnalysisCode/PCCAnalysis/test/veto_B0.txt";
  TString ModVeto = "BRILAnalysisCode/PCCAnalysis/veto_2022/veto_CDEFG_3_2022.txt";

  
  //TString Path = "/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022D";  
  //std::vector<int> run_number = {357538,357539,357540,357542,357550,357610,357611,357688,357697,357698,357700,357706};

  TString Path = "/eos/user/b/benitezj/BRIL/PCC_Run3/LinearityStudy_2022Veto/Run2022E";  
  std::vector<int> run_number = {359280,359281,359282,359283,359284,359285,359286,359287,359288,359289,359291,359293,359294,359296,359297,359342,359343,359571,359575,359576,359584,359585,359595,359597,359602,359603,359604,359608,359609,359611,359612,359614,359663,359664,359684,359685,359686,359693,359699,359717,359718,359751,359764,359776,359806,359808,359812,359813,359814,359871,359998,360017,360018,360019,360075,360090,360116,360121,360122,360123,360124,360126,360128,360129,360131,360141,360225,360295,360327};

 
  readModRPhiZCoordinates();
 
  if(ModVeto.CompareTo("")!=0)
    readModVeto(ModVeto);

  int LS=0;
  int run=0;
  int ls=0;


  TH1D *hRMS=new TH1D("hRMS", "",100,0,0.3);
  TGraph *ModWeight=new TGraph();
  TGraph *ModWeightRMS=new TGraph();
  TGraph *TotalPCC=new TGraph();
  TH2F *histo_counts[NMOD];
  for (unsigned int i=0;i<NMOD;i++)
    histo_counts[i]=new TH2F(TString("Histo_counts")+i,"", NTotalLS/NLSBLOCK,0,NTotalLS,300,0,0.015);
  
  
  TH2F* histo_L1=new TH2F("Histo_Layer1","",NTotalLS/NLSBLOCK,0,NTotalLS,200,0,0.50);
  TH2F* histo_L2=new TH2F("Histo_Layer2","",NTotalLS/NLSBLOCK,0,NTotalLS,200,0,0.50);
  TH2F* histo_L3=new TH2F("Histo_Layer3","",NTotalLS/NLSBLOCK,0,NTotalLS,200,0,0.50);
  TH2F* histo_L4=new TH2F("Histo_Layer4","",NTotalLS/NLSBLOCK,0,NTotalLS,200,0,0.50);
  TH2F* histo_D1S1=new TH2F("Histo_Disk1S1","",NTotalLS/NLSBLOCK,0,NTotalLS,200,0,0.50);
  TH2F* histo_D2S1=new TH2F("Histo_Disk2S1","",NTotalLS/NLSBLOCK,0,NTotalLS,200,0,0.50);
  TH2F* histo_D3S1=new TH2F("Histo_Disk3S1","",NTotalLS/NLSBLOCK,0,NTotalLS,200,0,0.50);
  TH2F* histo_D1S2=new TH2F("Histo_Disk1S2","",NTotalLS/NLSBLOCK,0,NTotalLS,200,0,0.50);
  TH2F* histo_D2S2=new TH2F("Histo_Disk2S2","",NTotalLS/NLSBLOCK,0,NTotalLS,200,0,0.50);
  TH2F* histo_D3S2=new TH2F("Histo_Disk3S2","",NTotalLS/NLSBLOCK,0,NTotalLS,200,0,0.50);


  TH2F* histoLinearity_L1=new TH2F("HistoLinearity_Layer1","",100,0,MAXTOTPCC,200,0,1.5);
  TH2F* histoLinearity_L2=new TH2F("HistoLinearity_Layer2","",100,0,MAXTOTPCC,200,0,1.5);
  TH2F* histoLinearity_L3=new TH2F("HistoLinearity_Layer3","",100,0,MAXTOTPCC,200,0,1.5);
  TH2F* histoLinearity_L4=new TH2F("HistoLinearity_Layer4","",100,0,MAXTOTPCC,200,0,1.5);
  TH2F* histoLinearity_D1S1=new TH2F("HistoLinearity_Disk1S1","",100,0,MAXTOTPCC,200,0,1.5);
  TH2F* histoLinearity_D2S1=new TH2F("HistoLinearity_Disk2S1","",100,0,MAXTOTPCC,200,0,1.5);
  TH2F* histoLinearity_D3S1=new TH2F("HistoLinearity_Disk3S1","",100,0,MAXTOTPCC,200,0,1.5);
  TH2F* histoLinearity_D1S2=new TH2F("HistoLinearity_Disk1S2","",100,0,MAXTOTPCC,200,0,1.5);
  TH2F* histoLinearity_D2S2=new TH2F("HistoLinearity_Disk2S2","",100,0,MAXTOTPCC,200,0,1.5);
  TH2F* histoLinearity_D3S2=new TH2F("HistoLinearity_Disk3S2","",100,0,MAXTOTPCC,200,0,1.5);

  
    

  for (unsigned int j=0;j<run_number.size();j++){
    TString infile=Path+"/"+run_number.at(j)+".csv"; 
    std::cout<< run_number.at(j)<<std::endl;  
    
    ifstream myfile (infile.Data());    
    if (!myfile.is_open()){
      cout << "Unable to open file: "<<infile.Data()<<endl; 
      //return;
      continue;
    }
    
    std::string line;    
    std::getline(myfile, line);
    std::stringstream iss(line);
    std::string token;
    
    int lumisec_count=0;
    while (std::getline(myfile, line)){

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
   
      lumisec_count++;      
      LS++;
      
      float m_count[NMOD];
      float count=0;
      float totcount=0;      
      for (unsigned int i=0;i<NMOD;i++){
	std::getline(iss,token, ',');
	std::stringstream countiss(token);
        countiss>>count;

	m_count[i]=0;

	if(MODVETO[MODID[i]]==0){

	  m_count[i]=count;                                                                                       
	  totcount+=count;
	  
	  if(BPIXorFPIX[MODID[i]]==1){
	    if(LY[MODID[i]]==1)	      count_L1+=count;
	    if(LY[MODID[i]]==2)	      count_L2+=count;
	    if(LY[MODID[i]]==3)	      count_L3+=count;
	    if(LY[MODID[i]]==4)	      count_L4+=count;
	    //cout<<"MOD "<<MODID[i]<<" "<<BPIXorFPIX[MODID[i]]<<" "<<LY[MODID[i]]<<" "<<DISK[MODID[i]]<<endl;
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
      //return;
  
      
      if(totcount > MINTOTPCC){
	TotalPCC->SetPoint(TotalPCC->GetN(), LS, totcount);
	
	for (unsigned int i=0;i<NMOD;i++)
	  if(MODVETO[MODID[i]]==0)
	    histo_counts[i]->Fill(LS, m_count[i]/totcount);

	histo_L1->Fill(LS, count_L1/totcount);
	histo_L2->Fill(LS, count_L2/totcount);
	histo_L3->Fill(LS, count_L3/totcount);
	histo_L4->Fill(LS, count_L4/totcount);
	histo_D1S1->Fill(LS, count_D1S1/totcount);
	histo_D2S1->Fill(LS, count_D2S1/totcount);
	histo_D3S1->Fill(LS, count_D3S1/totcount);
	histo_D1S2->Fill(LS, count_D1S2/totcount);
	histo_D2S2->Fill(LS, count_D2S2/totcount);
	histo_D3S2->Fill(LS, count_D3S2/totcount);

	histoLinearity_L1->Fill(totcount, count_L1/count_L4);
	histoLinearity_L2->Fill(totcount, count_L2/count_L4);
	histoLinearity_L3->Fill(totcount, count_L3/count_L4);
	histoLinearity_L4->Fill(totcount, count_L4/count_L4);
	histoLinearity_D1S1->Fill(totcount, count_D1S1/count_L4);
	histoLinearity_D2S1->Fill(totcount, count_D2S1/count_L4);
	histoLinearity_D3S1->Fill(totcount, count_D3S1/count_L4);
	histoLinearity_D1S2->Fill(totcount, count_D1S2/count_L4);
	histoLinearity_D2S2->Fill(totcount, count_D2S2/count_L4);
	histoLinearity_D3S2->Fill(totcount, count_D3S2/count_L4);
      }
      
       
    }
     
    myfile.close();    

    cout<<"# of LS "<<lumisec_count<<endl;
  }
  cout<<"Total # of LS "<<LS<<endl;
  
    
  for (unsigned int i=0;i<NMOD;i++){
    if(MODVETO[MODID[i]]==0){      
      TH1D*  RMSModule = histo_counts[i]->ProjectionY(TString("Projection_")+i);
      float mean=RMSModule->GetMean();
      float rms=RMSModule->GetRMS();    
      if(mean>0){
	ModWeight->SetPoint(ModWeight->GetN(), i, mean);
	ModWeightRMS->SetPoint(ModWeightRMS->GetN(), i, rms/mean);
	hRMS->Fill(rms/mean);
      }
      delete RMSModule;
    }
  }
  


  TCanvas C("C");
  C.cd();

  TotalPCC->SetMarkerStyle(8);
  TotalPCC->SetMarkerSize(0.5);
  TotalPCC->GetYaxis()->SetTitle("Total PCC");
  TotalPCC->GetXaxis()->SetTitle("Lumi section");
  TotalPCC->GetYaxis()->SetRangeUser(0,MAXTOTPCC);
  C.Clear();
  TotalPCC->Draw("ap");                                                                                                                   
  C.Print(OutPath+"/Module_RMS_Stability_totalcount.png");

 
  ModWeight->SetMarkerStyle(8);
  ModWeight->SetMarkerSize(0.5);
  ModWeight->GetXaxis()->SetTitle("Module ID");                                                                                 
  ModWeight->GetYaxis()->SetTitle("Module Weight");
  ModWeight->GetYaxis()->SetRangeUser(0, 0.008);
  C.Clear();
  ModWeight->Draw("AP"); 
  C.Print(OutPath+"/Module_RMS_Stability_weights.png");


  ModWeightRMS->SetMarkerStyle(8);
  ModWeightRMS->SetMarkerSize(0.5);
  ModWeightRMS->GetYaxis()->SetTitle("RMS / Mean");
  ModWeightRMS->GetXaxis()->SetTitle("Module ID");
  ModWeightRMS->GetYaxis()->SetRangeUser(0, 0.2);    
  C.Clear();
  ModWeightRMS->Draw("AP");
  C.Print(OutPath+"/Module_RMS_Stability_RMS.png");

  hRMS->SetStats(1);
  hRMS->GetXaxis()->SetTitle("RMS / Mean");
  hRMS->GetYaxis()->SetTitle("# of modules");
  C.Clear();
  hRMS->Draw("hist");
  C.Print(OutPath+"/Module_RMS_Stability_RMS_hist.png");



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

  P_L1->SetMarkerSize(0.7);
  P_L2->SetMarkerSize(0.7);
  P_L3->SetMarkerSize(0.7);
  P_L4->SetMarkerSize(0.7);    
  P_D1S1->SetMarkerSize(0.7);
  P_D2S1->SetMarkerSize(0.7);
  P_D3S1->SetMarkerSize(0.7);
  P_D1S2->SetMarkerSize(0.7);
  P_D2S2->SetMarkerSize(0.7);
  P_D3S2->SetMarkerSize(0.7);

  auto legend = new TLegend(0.905,0.14,1,0.9);
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
  P_L1->GetYaxis()->SetRangeUser(0.05,0.20);
  P_L1->GetYaxis()->SetTitle("Layer/Disk Weight");
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




  ///Linearity
  //////////Layer / Disk graphs
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

  PLinearityL1->SetMarkerSize(0.7);
  PLinearityL2->SetMarkerSize(0.7);
  PLinearityL3->SetMarkerSize(0.7);
  PLinearityL4->SetMarkerSize(0.7);    
  PLinearityD1S1->SetMarkerSize(0.7);
  PLinearityD2S1->SetMarkerSize(0.7);
  PLinearityD3S1->SetMarkerSize(0.7);
  PLinearityD1S2->SetMarkerSize(0.7);
  PLinearityD2S2->SetMarkerSize(0.7);
  PLinearityD3S2->SetMarkerSize(0.7);

  auto legendLinearity = new TLegend(0.905,0.14,1,0.9);
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


  PLinearityL1->SetStats(0);
  //PLinearityL1->GetYaxis()->SetRangeUser(0.05,0.20);
  PLinearityL1->GetYaxis()->SetRangeUser(0.94,1.06);
  PLinearityL1->GetYaxis()->SetTitle("Layer/Disk Normalized Weight");
  PLinearityL1->GetXaxis()->SetTitle("Total PCC");

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

  C.Clear();
  PLinearityL1->Draw("histp");
  PLinearityL2->Draw("histpsame");
  PLinearityL3->Draw("histpsame");
  PLinearityL4->Draw("histpsame");
  legendLinearity->Draw();
  C.Print(OutPath+"/Module_RMS_Stability_linearity_perlayerdisk_BPIX.png");    

  C.Clear();
  PLinearityL1->Draw("histp");
  PLinearityD1S1->Draw("histpsame");
  PLinearityD2S1->Draw("histpsame");
  PLinearityD3S1->Draw("histpsame");
  legendLinearity->Draw();
  C.Print(OutPath+"/Module_RMS_Stability_linearity_perlayerdisk_FPIXS1.png");    

  C.Clear();
  PLinearityL1->Draw("histp");
  PLinearityD1S2->Draw("histpsame");
  PLinearityD2S2->Draw("histpsame");
  PLinearityD3S2->Draw("histpsame");
  legendLinearity->Draw();
  C.Print(OutPath+"/Module_RMS_Stability_linearity_perlayerdisk_FPIXS2.png");    





  
}

  

