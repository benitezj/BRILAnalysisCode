#include"globals.h" 
#include <iostream>
#include <vector>
#include <dirent.h>
void modcount_2018C_copy_new() {
  
  TString Path1 = "/eos/user/a/asehrawa/modcountgraphs_goodmodules/C/";  
  TCanvas C("C");
  C.cd();
  //C.SetLogy();
  //C.SetCanvasSize(2000,600); 
  //gROOT->ProcessLine(".x /afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  //gStyle->SetOptStat(1111111);
    
  std::vector<int> run_number = {319337, 319347, 319348, 319349, 319449, 319450, 319452, 319456, 319459, 319460, 319462, 319463, 319464, 319466, 319467, 319468, 319469, 319470, 319471, 319472, 319486, 319488, 319503, 319506, 319524, 319525, 319526, 319528, 319579, 319606, 319625, 319639, 319656, 319657, 319658, 319659, 319678, 319687, 319697, 319698, 319756, 319840, 319841, 319847, 319848, 319849, 319851, 319852, 319853, 319854, 319908, 319909, 319910, 319912, 319913, 319914, 319915, 319928, 319941, 319942, 319950, 319991, 319992, 319993, 320002, 320003, 320006, 320007, 320008, 320009, 320010, 320011, 320012, 320023, 320024, 320025, 320026, 320038, 320039, 320040, 320058, 320059, 320060, 320061, 320062, 320063, 320064, 320065};

  //std::vector<int> run_number = {319337};
  
  std::cout<<run_number.size()<<std::endl;
  readModRPhiZCoordinates();
  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/module_L1_rms_mean_gr_0.07_totcount10^8_first_iteration.txt");
  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/module_L1_rms_mean_gr_0.07_totcount10^8_second_iteration.txt");
  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/module_L1_rms_mean_gr_0.07_totcount10^8_third_iteration.txt");

  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/test/veto_lateRunD_lowcut_tight_Run2018B.txt");
  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/test/veto_lateRunD_lowcut_tight1.txt");
  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/module_L1_rms_mean_gr_0.07_totcount10^8_third_iteration_vdM.txt");

  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/Run2018C_vetolist.txt");

  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/veto_Run2018C_0p04.txt");

  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/veto_Run2018C_0p02.txt");
  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/veto_Run2018C_0p02_3sigma_weights_cut.txt");

  
  int LS=0;
  int run=0;
  int ls=0;
  int previousrunlumisec_count=0;
  
  TGraph *gr[1856];
  
  TGraph *gr1;
  TGraph *gr2;
  TGraph *gr3;
  TGraph *gr4;
  TGraph *gr5;
  TGraph *gr6;
  TGraph *gr7;
  TGraph *gr8;
  TGraph *gr9;  
  TH1D *h;
  TGraph *gr_modweight;
  
  TH2F *histo_counts[1856];
  
  TH2F *histo_L2;
  TH2F *histo_L3;
  TH2F *histo_L4;
  TH2F *histo_D1S1;
  TH2F *histo_D2S1;
  TH2F *histo_D3S1;
  TH2F *histo_D1S2;
  TH2F *histo_D2S2;
  TH2F *histo_D3S2;
  
  TProfile *P_L2;
  TProfile *P_L3;
  TProfile *P_L4;
  
  TProfile *P_D1S1;
  TProfile *P_D2S1;
  TProfile *P_D3S1;
  TProfile *P_D1S2;
  TProfile *P_D2S2;
  TProfile *P_D3S2;
  
  TH1D *ProjY1[1856];
  TH1D *ProjY2[1856];
  
  TH1D *ProjY_L2;
  TH1D *ProjY_L3;
  TH1D *ProjY_L4;
  
  TH1D *ProjY_D1S1;
  TH1D *ProjY_D2S1;
  TH1D *ProjY_D3S1;
  TH1D *ProjY_D1S2;
  TH1D *ProjY_D2S2;
  TH1D *ProjY_D3S2;
  
  gr1=new TGraph();
  gr2=new TGraph();
  gr3=new TGraph();
  gr4=new TGraph();
  gr5=new TGraph();
  gr6=new TGraph();
  gr7=new TGraph();
  gr8=new TGraph();
  gr9=new TGraph();
  h=new TH1D("histo1D", "rms/mean",100,0,0.5);
  gr_modweight=new TGraph();
  
  histo_L2=new TH2F("Histo_Layer2","",200,0,35000,300,0,0.35);
  histo_L3=new TH2F("Histo_Layer3","",200,0,35000,300,0,0.35);
  histo_L4=new TH2F("Histo_Layer4","",200,0,35000,300,0,0.35);
  histo_D1S1=new TH2F("Histo_Disk1S1","",200,0,35000,300,0,0.35);
  histo_D2S1=new TH2F("Histo_Disk2S1","",200,0,35000,300,0,0.35);
  histo_D3S1=new TH2F("Histo_Disk3S1","",200,0,35000,300,0,0.35);
  histo_D1S2=new TH2F("Histo_Disk1S2","",200,0,35000,300,0,0.35);
  histo_D2S2=new TH2F("Histo_Disk2S2","",200,0,35000,300,0,0.35);
  histo_D3S2=new TH2F("Histo_Disk3S2","",200,0,35000,300,0,0.35);
  
  for (unsigned int i=0;i<1856;i++){
    gr[i]=new TGraph();
    histo_counts[i]=new TH2F(TString("Histo_counts")+i,"", 700,0,35000,300,0,0.016);
  }
  
  int modid[1856];
  for (unsigned int j=0;j<run_number.size();j++){
    TString Path = "/eos/user/a/asehrawa/BRIL-PCC_25Sep2021/ZeroBias/Run2018C"; 
    TString infile=Path+"/"+run_number.at(j)+".csv"; 
    std::cout<< run_number.at(j)<<std::endl;  
    
    ifstream myfile (infile.Data());    
    if (!myfile.is_open()){
      cout << "Unable to open file: "<<infile.Data()<<endl; 
      return;
    }
    
    std::string line;    
    std::getline(myfile, line);
    std::stringstream iss(line);
    std::string token;
    
    for (unsigned int i=0;i<1856;i++){
      std::getline(iss,token, ',');
      std::stringstream modidiss(token);
      modidiss>>modid[i];
      
    }
    
    int lumisec_count=0;
    while (std::getline(myfile, line)){
      int count_L2=0;
      int count_L3=0;
      int count_L4=0;
      int count_D1S1=0;
      int count_D2S1=0;
      int count_D3S1=0;
      int count_D1S2=0;
      int count_D2S2=0;
      int count_D3S2=0;      
      std::stringstream iss(line);
      std::string token;
      
      std::getline(iss,token, ',');
      std::stringstream runiss(token);
      runiss>>run;
      
      std::getline(iss,token, ',');
      std::stringstream lsiss(token);
      lsiss>>ls;
      //std::cout<<run <<" ";
      //std::cout<< ls;
      lumisec_count++;      
      LS = previousrunlumisec_count + ls; 
      
      int m_count[1856];
      int count=0;
      float totcount=0;      
      for (unsigned int i=0;i<1856;i++){
	std::getline(iss,token, ',');
	std::stringstream modidiss(token);
        modidiss>>count;
	m_count[i]=count;        
	if(MODVETO[modid[i]]==0){                                                                               
	totcount+=count;
	if(BPIXorFPIX[modid[i]]==1){
	  if(LY[modid[i]]==2){
	    count_L2+=count;
	  }    
	  if(LY[modid[i]]==3){
	    count_L3+=count;
	  }
	  if(LY[modid[i]]==4){
	    count_L4+=count;
	  }
	}

	if(BPIXorFPIX[modid[i]]==2){
	  if(DISK[modid[i]]==2){
	    count_D1S1+=count;
	  }    
	  if(DISK[modid[i]]==1){
	    count_D2S1+=count;
	  } 
	  if(DISK[modid[i]]==0){
	    count_D3S1+=count;
	  }     
	  if(DISK[modid[i]]==3){
	    count_D1S2+=count;
	  }    
	  if(DISK[modid[i]]==4){
	    count_D2S2+=count;
	  }    
	  if(DISK[modid[i]]==5){
	    count_D3S2+=count;
	          
	  }
	}  
      }
      }
      
      
      
      //loop over modules
      for (unsigned int i=0;i<1856;i++){
	if(totcount > 70000000){
	if(MODVETO[modid[i]]==0){
	histo_counts[i]->Fill(LS, m_count[i]/totcount);
	//std::cout<<i<<" "<<countsum[i]<<" "<<totcountsum<<" "<<countsum[i]/totcountsum<<std::endl; 
	//std::cout<<i<<" "<<m_count[i]/totcount<<std::endl;  
	}
	}
      }
      
      
      //if(totcount > 0){
      if(totcount > 70000000){
      histo_L2->Fill(LS, count_L2/totcount);
      histo_L3->Fill(LS, count_L3/totcount);
      histo_L4->Fill(LS, count_L4/totcount);
      histo_D1S1->Fill(LS, count_D1S1/totcount);
      histo_D2S1->Fill(LS, count_D2S1/totcount);
      histo_D3S1->Fill(LS, count_D3S1/totcount);
      histo_D1S2->Fill(LS, count_D1S2/totcount);
      histo_D2S2->Fill(LS, count_D2S2/totcount);
      histo_D3S2->Fill(LS, count_D3S2/totcount);
      gr9->SetPoint(gr9->GetN(), LS, totcount);
      //std::cout<<LS<<" "<< totcount<<std::endl;
      //std::cout<<run_number.at(j)<<std::endl;
      }
      
      //if(totcount > 10000000) {
      //std::cout<<run_number.at(j)<<std::endl;
      //}
      
    }
     
    previousrunlumisec_count+=lumisec_count;  
    myfile.close();    
    
  }
  
    
  TProfile* P[1856];
  TH1D* ProjY[1856];
  C.Clear();   
  bool var=0;
  int g_count=0;
  int g_plot=0;
  
  //auto legend = new TLegend(0.7,0.15,0.85,0.85);            
  auto legend = new TLegend(0.91,0.1,1,0.9); 
  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetFillColor(0);

  for (unsigned int i=0;i<1856;i++){
    if(MODVETO[modid[i]]==0){      
    //if(LY[modid[i]]==0){
    //histo_counts[i]->SetTitle(modid[i] + TString(" Side ") + SD[modid[i]] + " FPIX Disk " + DISK[modid[i]] + " Ring " + RING[modid[i]]);
    //} 
    //if(LY[modid[i]]!=0){
    //histo_counts[i]->SetTitle(modid[i]+ TString(" BPIX layer ") + LY[modid[i]] +" ladder " + LD[modid[i]] + " MD " + MD[modid[i]]);
    // }
    histo_counts[i]->SetTitle(TString(" TH2F Histogram of cluster counts per lumi section "));      
    //histo_counts[i]->GetXaxis()->SetRangeUser(0, 70000);  
    //histo_counts[i]->GetYaxis()->SetRangeUser(0, 0.016);      
    histo_counts[i]->GetXaxis()->SetTitle("Lumi section");
    histo_counts[i]->GetYaxis()->SetTitle("Normalized count");
    //histo_counts[i]->Draw("COLZ");                                                                                                   
    //char *hist_name1 = new char[20];                                                     
    //if(modid[i]==344253444){                                                
    //sprintf(hist_name1,"histo2D_%d.png",i);                                                                                              
    //C.Print(Path1 + "Graph1D_test1/"+hist_name1);    
    //C.Print(Path1+"Graphs_cut_totcount/"+hist_name1);      
    //}
      
    //if(MODVETO[modid[i]]==0){
    P[i] = histo_counts[i]->ProfileX();
    ProjY[i] = histo_counts[i]->ProjectionY(TString("Projection_")+i,0,700);
    gr_modweight->SetPoint(gr_modweight->GetN(), i, ProjY[i]->GetMean());
    //ProjY1[i] = histo_counts[i]->ProjectionY(TString("Projection1_")+i,0,250);
    //ProjY2[i] = histo_counts[i]->ProjectionY(TString("Projection2_")+i,330,430);
    //} 
      
    //P[i]->GetXaxis()->SetRangeUser(0, 70000);
    //copy content of TProfile into TGraph by looping over bin content of TProfile
    int x_value;
    float bincontent;
    float binerror;
    for(int j = 1; j <= P[i]->GetNbinsX(); j++) {
      x_value =  P[i]->GetBinCenter(j);
      bincontent = P[i]->GetBinContent(j);
      binerror = P[i]->GetBinError(j);
      if(bincontent > 0 && (binerror/bincontent) < 0.05) {
	gr[i] ->SetPoint(gr[i]->GetN(), x_value, bincontent/(ProjY[i]->GetMean()));    
	//std::cout<< P[i]->GetBinContent(j)<<" "<<std::endl;
      }
    }
      
    gr[i]->GetXaxis()->SetRangeUser(0, 35000);
    gr[i]->GetXaxis()->SetNdivisions(10);                                                                                                
    gr[i]->GetYaxis()->SetRangeUser(0.5, 1.5);
    gr[i]->SetMarkerStyle(8);  
    gr[i]->SetMarkerSize(0.5); 
    //gr[i]->SetLineStyle(1);
    gr[i]->GetXaxis()->SetTitle("Lumi section");
    gr[i]->GetYaxis()->SetTitle("Normalized count");
    //if(DISK[modid[i]]==2 || DISK[modid[i]]==3){      
    //if(g_count==0){
    //C.Clear();
    //legend->Clear();
    //gr[i]->SetLineColor(g_count+1);
    //gr[i]->Draw("AL");
    //legend->AddEntry(gr[i], TString("modid ")+modid[i], "l");
    //g_count++;
    //} else if(g_count>0&&g_count<19) {
    //if(g_count<9){
    //gr[i]->SetLineColor(g_count+1);
    //}
    //if(g_count>=9 && g_count<19){
    //gr[i]->SetLineColor(g_count+20);
    //}
    //gr[i]->Draw("LSAME");
    //legend->AddEntry(gr[i], TString("modid ")+modid[i], "l");
    //g_count++;
    //} else if(g_count==19) { 
    //gr[i]->SetLineColor(g_count+27);
    //gr[i]->Draw("LSAME"); 
    //legend->AddEntry(gr[i], TString("modid ")+modid[i], "l");
    //legend->Draw("SAME");
    //char* histname2 = new char[100];
    //sprintf(histname2, "graph_%d", g_plot++);
    //legend->Draw("SAME");
    //C.Print(Path1+"Graphs_cut_totcount/"+histname2+".png");
    //C.Print(Path1+"Graph1D_test1/"+histname2+".png");
    //g_count=0;      
    //}
    //}

    //if(LY[modid[i]]==0){
    //gr[i]->SetTitle(modid[i]+ TString(" Side ") + SD[modid[i]] +" FPIX Disk " + DISK[modid[i]] + " Ring " + RING[modid[i]]);
    // }
    //if(LY[modid[i]]!=0){
    //gr[i]->SetTitle(modid[i]+ TString(" BPIX layer ") + LY[modid[i]] +" ladder " + LD[modid[i]] + " MD " + MD[modid[i]]);
    //}
    //gr[i]->SetTitle(" Module stability profile ");
      
    //gr[i]->Draw("AP");
    //char *hist_name = new char[100];
    //if(LY[modid[i]]==0){                                                                                                                  
    //sprintf(hist_name, TString("Graph1D_") + modid[i]+ "_Side_" + SD[modid[i]] +"_Disk_" + DISK[modid[i]] + "_Ring_" + RING[modid[i]]+".png", i);                                                                                                               
    //}                                                                                                                                    
    //if(LY[modid[i]]!=0){                                                                                                                 
    //sprintf(hist_name, TString("Graph1D_") + modid[i]+ "_layer_" + LY[modid[i]] +"_ladder_" + LD[modid[i]] + "_MDnumber_" + MD[modid[i]]+".png", i);                                                                                                            
    //}   
    //C.Print(Path1+"Graphs_cut_totcount/"+hist_name+".png"); 


    //if(modid[i]==344253444){
    //sprintf(hist_name,"Graph1D_%d.png",i);
    //C.Print(Path1+"Graphs_cut_totcount/"+hist_name);
    //}
    //C.Print(Path1 + "Graph1D_test1/"+hist_name);
      
    //float p1=ProjY1[i]->GetMean();
    //float p2= ((ProjY2[i]->GetMean())-(ProjY1[i]->GetMean()))/p1;
    //gr1->SetPoint(gr1->GetN(), i, p1);
    //gr2->SetPoint(gr2->GetN(), i, p2);      
    //gr1->GetXaxis()->SetTitle("module number index");
    //gr1->GetYaxis()->SetTitle("Mean");
    //gr1->GetXaxis()->SetRangeUser(0, 1900);
    //gr1->GetYaxis()->SetRangeUser(0, 0.012);
    //gr1->SetMarkerStyle(8);
    //gr1->SetMarkerSize(0.5);
    //gr2->GetXaxis()->SetTitle("module number index");
    //gr2->GetYaxis()->SetTitle("Normalized mean difference");
    //gr2->SetMarkerStyle(8);
    //gr2->SetMarkerSize(0.5);
    //gr2->GetXaxis()->SetRangeUser(0, 1900);
    //gr2->GetYaxis()->SetRangeUser(-0.25, 0.25);
      
    //if(LY[modid[i]]==0){
    //P[i]->SetTitle(modid[i]+ TString(" Side ") + SD[modid[i]] +" FPIX Disk " + DISK[modid[i]] + " Ring " + RING[modid[i]]);
    //}
    //if(LY[modid[i]]!=0){
    //P[i]->SetTitle(modid[i]+ TString(" BPIX layer ") + LY[modid[i]] +" ladder " + LD[modid[i]] + " MD " + MD[modid[i]]);
    //}   
      
    //P[i]->Draw();
    //char* hist_name1 = new char[100];
    //sprintf(hist_name1,"ProfileX_%d.png",i);                                                                                            
    //C.Print(Path1 + "Graph1D_test1/"+hist_name1);
      
    //if(LY[modid[i]]==0){
    //sprintf(histname1, TString("ProfileX_Forward_Disk_Modid_") + modid[i]+ "_Side_" + SD[modid[i]] +"_Disk_" + DISK[modid[i]] + "_Ring_" + RING[modid[i]]+".png", i);
    // }
    //if(LY[modid[i]]!=0){
    //sprintf(histname1, TString("ProfileX_Barrel_layer_Modid_") + modid[i]+ "_layer_" + LY[modid[i]] +"_ladder_" + LD[modid[i]] + "_MDnumber_" + MD[modid[i]]+".png", i);
    //}
      
    //gr[i]->SetMarkerStyle(8);
    //gr[i]->SetMarkerSize(0.5);
    //gr[i]->SetLineStyle(1);
    //gr[i]->SetLineSize(0.5);
    //gr[i]->GetXaxis()->SetRangeUser(0, 80000);
    //gr[i]->GetXaxis()->SetNdivisions(10);
    //gr[i]->GetYaxis()->SetRangeUser(0.5, 1.5);
    //gr[i]->GetXaxis()->SetTitle("Lumi section");
    //gr[i]->GetYaxis()->SetTitle("Normalized count");  
    //if(LY[modid[i]]==0){
    //gr[i]->SetTitle(modid[i]+ TString(" Side ") + SD[modid[i]] +" FPIX Disk " + DISK[modid[i]] + " Ring " + RING[modid[i]]);
    //}
    //if(LY[modid[i]]!=0){
    //gr[i]->SetTitle(modid[i]+ TString(" BPIX layer ") + LY[modid[i]] +" ladder " + LD[modid[i]] + " MD " + MD[modid[i]]);
    //}
      
    //if(LY[modid[i]]==0){
    //gr3->SetPoint(gr3->GetN(), i, FPIX_Z[modid[i]]);
    //}
    //if(LY[modid[i]]!=0){
    //gr3->SetPoint(gr3->GetN(), i, BPIX_Z[modid[i]]);
    //}
    //gr4->SetPoint(gr4->GetN(), FPIX_Z[modid[i]],  DISK[modid[i]]);
      
    //gr3->SetMarkerStyle(8);
    //gr3->SetMarkerSize(0.5);
    //gr3->GetYaxis()->SetRangeUser(-100, 100);
    //gr3->GetXaxis()->SetRangeUser(0, 1900);
    //gr3->GetYaxis()->SetTitle("z coordinate");
    //gr3->GetXaxis()->SetTitle("module number index");
      
    //gr4->SetMarkerStyle(8);
    //gr4->SetMarkerSize(0.5);
    //gr4->GetXaxis()->SetRangeUser(-80, 80);
    //gr4->GetYaxis()->SetRangeUser(0, 5);
    //gr4->GetXaxis()->SetTitle("z coordinate");
    //gr4->GetYaxis()->SetTitle("Disk");
      
    //gr5->SetPoint(gr5->GetN(), i, LY[modid[i]]);   
    //if(SD[modid[i]]==1){
    //gr6->SetPoint(gr6->GetN(), i, abs(DISK[modid[i]]-3));
    //}
    //if(SD[modid[i]]==2){
    //gr6->SetPoint(gr6->GetN(), i, abs(DISK[modid[i]]-2));
    //}
    //if(SD[modid[i]]==1){
    //gr7->SetPoint(gr7->GetN(), SD[modid[i]], DISK[modid[i]]); 
    //}
    //if(SD[modid[i]]==2){
    //gr7->SetPoint(gr7->GetN(), FPIX_Z[modid[i]], abs(DISK[modid[i]]-2));
    //}
      
    gr8->SetMarkerStyle(8);
    gr8->SetMarkerSize(0.4);
    //gr8->GetYaxis()->SetNdivisions(80);
    gr8->GetYaxis()->SetLabelSize(0.02);
    gr8->GetXaxis()->SetRangeUser(0, 1900);
    //gr8->GetYaxis()->SetRangeUser(0, 0.1);
    //gr8->SetTitle(" Run2018B (317080-319311)");
    gr8->GetYaxis()->SetTitle("RMS/Mean");
    gr8->GetXaxis()->SetTitle("module number index");
  
    h->SetMarkerStyle(8);
    h->SetMarkerSize(0.5);
    //h->GetYaxis()->SetRangeUser(0, 200);
    //gr10->GetYaxis()->SetNdivisions(80);                                                                                                 
    //gr10->GetYaxis()->SetLabelSize(0.02);              
    //h->GetXaxis()->SetRangeUser(0, 1);
    h->GetYaxis()->SetTitle("Entries");                                                                                             
    h->GetXaxis()->SetTitle("rms/mean");
    //h->SetTitle("              Run2018B (317080-319311)");                                                                              
      
    float mean=ProjY[i]->GetMean();
    float rms=ProjY[i]->GetRMS();    
    if(mean!=0){
      gr8->SetPoint(gr8->GetN(), i, rms/mean);
      h->Fill(rms/mean);
      //std::cout<<i<<"  "<<rms<<"  "<<mean<<"  "<<rms/mean<<std::endl;
      //std::cout<<modid[i]<<" "<<rms/mean<<std::endl;
    }
    //}
      
    if(LY[modid[i]]==1 || rms/mean>=0.02){
    //std::cout<<modid[i]<<" "<<rms/mean<<std::endl;
    //std::cout<<modid[i]<<std::endl;    
    }
     
    //gr11->SetPoint(gr11->GetN(), i, summodcount/totmodcount); 
    //std::cout<<summodcount<<" "<<totmodcount<<"  "<< summodcount/totmodcount<<std::endl;    
    //std::cout<<(ProjY[i]->GetRMS())/(ProjY[i]->GetMean())<<std::endl;                                                                   
    //if((rms/mean)==0 || (rms/mean)>=0.015){
    //std::cout<<i<<" "<<(ProjY[i]->GetRMS())/(ProjY[i]->GetMean())<<std::endl;     
    //std::cout<<modid[i]<<std::endl; 
    //}
      
    float h_mean=h->GetMean();
    float h_rms=h->GetStdDev();
    float rms_mean=rms/mean;

    //if((rms_mean==0) || (0.4*(rms_mean-h_mean)>= h_rms)) {
    //std::cout<<modid[i]<<std::endl;
    //std::cout<<i<<" "<<modid[i]<<std::endl;
    //}

    //ProjY[i]->GetXaxis()->SetRangeUser(0.002, 0.004);
    //if(LY[modid[i]]==0){
    //ProjY[i]->SetTitle(modid[i]+ TString(" Side ") + SD[modid[i]] +" FPIX Disk " + DISK[modid[i]] + " Ring " + RING[modid[i]]);
    //}
    //if(LY[modid[i]]!=0){
    //ProjY[i]->SetTitle(modid[i]+ TString(" BPIX layer ") + LY[modid[i]] +" ladder " + LD[modid[i]] + " MD " + MD[modid[i]]);
    //}
    //ProjY[i]->SetTitle(TString(" ProjectionY of TH2F histogram "));
      
    //ProjY[i]->Draw();
    //char* hist_name3 = new char[100];
    //if(modid[i]==344253444){
      
    //sprintf(hist_name3,"ProjectionY_%d.png",i);
    //C.Print(Path1 + "Graphs_cut_totcount/"+hist_name3);
    //}      
    //if(LY[modid[i]]==0){
    //sprintf(histname3, TString("ProjectionY_Forward_Disk_Modid_") + modid[i]+ "_Side_" + SD[modid[i]] +"_Disk_" + DISK[modid[i]] + "_Ring_" + RING[modid[i]]+".png", i);
    //}
    //if(LY[modid[i]]!=0){
    //sprintf(histname3, TString("ProjectionY_Barrel_layer_Modid_") + modid[i]+ "_layer_" + LY[modid[i]] +"_ladder_" + LD[modid[i]] + "_MDnumber_" + MD[modid[i]]+".png", i);
      
    //} 
    //}
    }
  }
  
  //if(g_count!=0){ 
  //legend->Draw("SAME"); 
  //char* histname2 = new char[100];
  //sprintf(histname2, "graph_%d", g_plot++);
  //C.Print(Path1+"Graphs_cut_totcount/"+histname2+".png");
  //}
  
  //gr1->Draw("AP");  
  //C.Print(Path1+"Graphs_cut_totcount/"+"graphprojy1"+".png");
  //C.Print(Path1+"Graph1D_test1/"+"graphprojy1"+".png");
  
  //gr2->Draw("AP");
  //C.Print(Path1+"Graphs_cut_totcount/"+"graphprojy2"+".png");
  //C.Print(Path1+"Graph1D_test1/"+"graphprojy2"+".png");
  
  //TLine *l = new TLine(1180,-100,1180,100);
  //l->SetLineStyle(2);
  //gr3->Draw("AP");
  //l->Draw("SAME");  
  //C.Print(Path1+"Graph1D_test1/"+"graph_modindex_z"+".png");
  
  //gr4->Draw("AP");
  //C.Print(Path1+"Graph1D_test1/"+"graph_disk_z"+".png");
  
  //gr8->SetTitle("                Run2018A (315252-316995)");
  //gr8->SetTitle("              Run2018B (317080-319311)");                                                                            
  gr8->SetTitle("              Run2018C (319337-320065)");                                                                            
  //gr8->SetTitle("              Run2018D (320500-325175)");
  //gr8->GetYaxis()->SetRangeUser(0, 0.018);
  gr8->GetYaxis()->SetRangeUser(0, 0.2);    
  gr8->Draw("AP");
  TLine *l = new TLine(0,0.03,2050,0.03);                                                                                                
  l->SetLineStyle(2);                                                                                                                    
  //l->Draw("SAME"); 
  //C.Print(Path1+"Graphs_cut_totcount/"+"RMS_Mean_Run2018A_(B+A)veto"+".png");
  //C.Print(Path1+"Graphs_cut_totcount/"+"RMS_Mean_2p5sigma"+".png"); 
  C.Print(Path1+"Graphs_cut_totcount/"+"RMS_Mean_testing"+".png");
  
  //gr9->GetXaxis()->SetTitle("Lumi section");
  //gr9->GetYaxis()->SetTitle("totcount");
  //gr9->SetTitle("              Run2018A (315252-316995)");
  //gr9->SetTitle("            Run2018B (317080-319311)");                                                                            
  gr9->SetTitle("            Run2018C (319337-320065)");
  //gr9->SetTitle("            Run2018D (320500-325175)");
  
  //gr9->GetXaxis()->SetNdivisions(10);
  gr9->GetXaxis()->SetLabelSize(0.03);
  gr9->GetYaxis()->SetTitle("totcount");
  gr9->GetXaxis()->SetTitle("Lumi section");
  gr9->Draw("AP");                                                                                                                   
  //C.Print(Path1 + "Graph1D_test1/"+"LStot_RunA"+".root"); 
  //C.Print(Path1 + "Graph1D_test1/"+"LStot_RunB_lowlumi"+".root");
  //C.Print(Path1 + "Graph1D_test1/"+"LStot_RunC"+".root");
  C.Print(Path1 + "Graphs_cut_totcount/"+"LStot_RunB_testing"+".root");
 
  h->GetXaxis()->SetRangeUser(0, 0.2); 
  h->SetTitle("              Run2018C (319337-320065)");
  //h->GetXaxis()->SetRangeUser(0, 0.018);
  h->GetYaxis()->SetRangeUser(0, 810);
  h->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"RMS_Mean_histo_2p5sigma"+".png");
  C.Print(Path1+"Graphs_cut_totcount/"+"RMS_Mean_histo_testing"+".png");


  gr_modweight->SetTitle("              Run2018C (319337-320065)");
  //gr_modweight->SetTitle("              Run2018B (317080-319311)");
  gr_modweight->GetXaxis()->SetTitle("module number index");                                                                                 
  gr_modweight->GetYaxis()->SetTitle("module weight");
  gr_modweight->GetXaxis()->SetRangeUser(0,2000);
  gr_modweight->GetYaxis()->SetRangeUser(0, 0.0045);
  gr_modweight->Draw("AP"); 
  C.Print(Path1+"Graphs_cut_totcount/"+"mod_weight_testing"+".png");
  
  histo_L2->GetXaxis()->SetTitle("Lumi section");
  histo_L2->GetYaxis()->SetTitle("Lumi ratio");
  histo_L2->GetXaxis()->SetRangeUser(0,70000);
  //histo_L2->SetTitle("              Run2018A (315252-316995)");
  //histo_L2->SetTitle("              Run2018B (317080-319311)");                                                                          
  histo_L2->SetTitle("              Run2018C (319337-320065)"); 
  //histo_L2->SetTitle("            Run2018D (320500-325175)");
  histo_L3->GetXaxis()->SetTitle("Lumi section");
  histo_L3->GetYaxis()->SetTitle("Lumi ratio");
  histo_L4->GetXaxis()->SetTitle("Lumi section");
  histo_L4->GetYaxis()->SetTitle("Lumi ratio");
  histo_D1S1->GetXaxis()->SetTitle("Lumi section");
  histo_D1S1->GetYaxis()->SetTitle("Lumi ratio");
  histo_D2S1->GetXaxis()->SetTitle("Lumi section");
  histo_D2S1->GetYaxis()->SetTitle("Lumi ratio");
  histo_D3S1->GetXaxis()->SetTitle("Lumi section");
  histo_D3S1->GetYaxis()->SetTitle("Lumi ratio");
  histo_D1S2->GetXaxis()->SetTitle("Lumi section");
  histo_D1S2->GetYaxis()->SetTitle("Lumi ratio");
  histo_D2S2->GetXaxis()->SetTitle("Lumi section");
  histo_D2S2->GetYaxis()->SetTitle("Lumi ratio");
  histo_D3S2->GetXaxis()->SetTitle("Lumi section");
  histo_D3S2->GetYaxis()->SetTitle("Lumi ratio");
  
  histo_L2->Draw("COLZ"); 
  histo_L3->Draw("COLZSAME");
  histo_L4->Draw("COLZSAME");
  histo_D1S1->Draw("COLZSAME");
  histo_D2S1->Draw("COLZSAME");
  histo_D3S1->Draw("COLZSAME");
  histo_D1S2->Draw("COLZSAME");
  histo_D2S2->Draw("COLZSAME");
  histo_D3S2->Draw("COLZSAME");
  //C.Print(Path1+"Graphs_cut_totcount/"+"Histo_stability_combined_2p5sigma"+".png"); 
  //C.Print(Path1+"Graphs_cut_totcount/"+"Histo_stability_combined_newveto(B+A)_Run2018A_update"+".png");  
  C.Print(Path1+"Graphs_cut_totcount/"+"Histo_stability_combined_testing"+".png");
  
  //histo_L2->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityL2"+".png"); 
  //histo_L3->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityL3"+".png");
  //histo_L4->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityL4"+".png");
  //histo_D1S1->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityD1S1"+".png");
  //histo_D2S1->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityD2S1"+".png");
  //histo_D3S1->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityD3S1"+".png");
  //histo_D1S2->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityD1S2"+".png");
  //histo_D2S2->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityD2S2"+".png");
  //histo_D3S2->Draw("COLZ");
  //C.Print(Path1+"Graph1D_test1/"+"Histo_stabilityD3S2"+".png");
  
  P_L2 = histo_L2->ProfileX();
  //P_L2->SetTitle("BPIX Layer 2");
  ProjY_L2 = histo_L2->ProjectionY(TString("Projection_L2"),0,200);
  P_L2->GetXaxis()->SetRangeUser(0, 35000);
  P_L2->GetXaxis()->SetNdivisions(50);
  P_L2->GetYaxis()->SetRangeUser(0, 0.35);
  //P_L2->GetXaxis()->SetNdivisions(7);
  
  P_L2->GetXaxis()->SetTitle("Lumi section");
  P_L2->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  //P_L2->SetTitle("              Run2018A (315252-316995)");
  //P_L2->SetTitle("              Run2018B (317080-319311)");
  P_L2->SetTitle("              Run2018C (319337-320065)");
  //P_L2->SetTitle("            Run2018D (320500-325175)");
  P_L3 = histo_L3->ProfileX();
  P_L3->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_L3 = histo_L3->ProjectionY(TString("Projection_L3"),0,200);
  //P_L3->GetYaxis()->SetRangeUser(0, 0.3);
  P_L3->SetTitle("BPIX Layer 3");
  P_L3->GetXaxis()->SetTitle("Lumi section");
  P_L3->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  P_L4 = histo_L4->ProfileX();
  P_L4->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_L4 = histo_L4->ProjectionY(TString("Projection_L4"),0,200);
  //P_L4->GetYaxis()->SetRangeUser(0, 0.3);
  P_L4->SetTitle("BPIX Layer 4");
  P_L4->GetXaxis()->SetTitle("Lumi section");
  P_L4->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  P_D1S1 = histo_D1S1->ProfileX();
  P_D1S1->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_D1S1 = histo_D1S1->ProjectionY(TString("Projection_D1S1"),0,200);
  //P_D1S1->GetYaxis()->SetRangeUser(0, 0.3);
  P_D1S1->SetTitle("FPIX Disk 1 Side 1");
  P_D1S1->GetXaxis()->SetTitle("Lumi section");
  P_D1S1->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  P_D2S1 = histo_D2S1->ProfileX();
  P_D2S1->SetTitle("FPIX Disk 2 Side 1");
  P_D2S1->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_D2S1 = histo_D2S1->ProjectionY(TString("Projection_D2S1"),0,200);
  //P_D2S1->GetYaxis()->SetRangeUser(0, 0.3);
  P_D2S1->GetXaxis()->SetTitle("Lumi section");
  P_D2S1->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  P_D3S1 = histo_L2->ProfileX();
  // P_D3S1->SetTitle("FPIX Disk 3 Side 1");
  P_D3S1->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_D3S1 = histo_D3S1->ProjectionY(TString("Projection_D3S1"),0,200);
  //P_D3S1->GetYaxis()->SetRangeUser(0, 0.3);
  P_D3S1->GetXaxis()->SetTitle("Lumi section");
  P_D3S1->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  ProjY_D3S1->SetTitle(" ProjectionY_D3S1");
  ProjY_D3S1->GetXaxis()->SetRangeUser(0, 0.15);
  P_D1S2 = histo_D1S2->ProfileX();
  P_D1S2->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_D1S2 = histo_D1S2->ProjectionY(TString("Projection_D1S2"),0,200);
  //P_D1S2->GetYaxis()->SetRangeUser(0, 0.3);
  P_D1S2->SetTitle("FPIX Disk 1 Side 2");
  P_D1S2->GetXaxis()->SetTitle("Lumi section");
  P_D1S2->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  P_D2S2 = histo_D2S2->ProfileX();
  P_D2S2->SetTitle("FPIX Disk 2 Side 2");
  
  P_D2S2->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_D2S2 = histo_D2S2->ProjectionY(TString("Projection_D2S2"),0,200);
  ProjY_D2S2->SetTitle(" ProjectionY_D2S2");
  ProjY_D2S2->GetXaxis()->SetRangeUser(0, 0.15);
  //P_D2S2->GetYaxis()->SetRangeUser(0, 0.3);
  P_D2S2->GetXaxis()->SetTitle("Lumi section");
  P_D2S2->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
  P_D3S2 = histo_D3S2->ProfileX();
  P_D3S2->SetTitle("FPIX Disk 3 Side 2");
  P_D3S2->GetYaxis()->SetRangeUser(0, 0.35);
  ProjY_D3S2 = histo_D3S2->ProjectionY(TString("Projection_D3S2"),0,200);
  //P_D3S2->GetYaxis()->SetRangeUser(0, 0.3);
  P_D3S2->GetXaxis()->SetTitle("Lumi section");
  P_D3S2->GetYaxis()->SetTitle("Lumi fraction (per 350 lumi section)");
    
  std::cout<<ProjY_L2->GetMean()<<std::endl;
  std::cout<<ProjY_L3->GetMean()<<std::endl;
  std::cout<<ProjY_L4->GetMean()<<std::endl;
  std::cout<<ProjY_D1S1->GetMean()<<std::endl;
  std::cout<<ProjY_D2S1->GetMean()<<std::endl;
  std::cout<<ProjY_D3S1->GetMean()<<std::endl;
  std::cout<<ProjY_D1S2->GetMean()<<std::endl;
  std::cout<<ProjY_D2S2->GetMean()<<std::endl;
  std::cout<<ProjY_D3S2->GetMean()<<std::endl;
  
  P_L2->SetLineColor(1);
  P_L2->SetMarkerColor(1);
  P_L3->SetLineColor(2);
  P_L3->SetMarkerColor(2);
  P_L4->SetLineColor(3);
  P_L4->SetMarkerColor(3);    
  P_D1S1->SetLineColor(4);
  P_D1S1->SetMarkerColor(4);
  P_D2S1->SetLineColor(5);
  P_D2S1->SetMarkerColor(5);
  P_D3S1->SetLineColor(6);
  P_D3S1->SetMarkerColor(6);
  P_D1S2->SetLineColor(7);
  P_D1S2->SetMarkerColor(7);
  P_D2S2->SetLineColor(8);
  P_D2S2->SetMarkerColor(8);
  P_D3S2->SetLineColor(9);
  P_D3S2->SetMarkerColor(9);
  
  P_L2->Draw();
  P_L3->Draw("SAME");
  P_L4->Draw("SAME");
  P_D1S1->Draw("SAME");
  P_D2S1->Draw("SAME");
  P_D3S1->Draw("SAME");
  P_D1S2->Draw("SAME");
  P_D2S2->Draw("SAME");
  P_D3S2->Draw("SAME");
    
  auto legend1 = new TLegend(0.91,0.15,1,0.9);
  //auto legend1 = new TLegend(0.7,0.35,0.85,0.8);
  legend1->AddEntry(P_L2,"B1","l");
  legend1->AddEntry(P_L3,"B2","l");
  legend1->AddEntry(P_L4,"B3","l");
  legend1->AddEntry(P_D1S1,"F1S1","l");
  legend1->AddEntry(P_D2S1,"F2S1","l");
  legend1->AddEntry(P_D3S1,"F3S1","l");
  legend1->AddEntry(P_D1S2,"F1S2","l");
  legend1->AddEntry(P_D2S2,"F2S2","l");
  legend1->AddEntry(P_D3S2,"F3S2","l");
  legend1->SetFillColor(0);
  legend1->SetLineColor(0);
  legend1->SetFillColor(0);
  legend1->Draw("SAME");
  
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_combined_2p5sigma"+".png");
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_combined_newveto(B+A)_Run2018A_update"+".png");
  C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_combined_testing"+".png");
  
  //P_L2->Draw();  
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_L2"+".png"); 
  //P_L3->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_L3"+".png");
  //P_L4->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_L4"+".png");
  //P_D1S1->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_D1S1"+".png");
  //P_D2S1->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_D2S1"+".png");
  //P_D3S1->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_D3S1"+".png");
  //P_D1S2->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_D1S2"+".png");
  //P_D2S2->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_D2S2"+".png");
  //P_D3S2->Draw();
  //C.Print(Path1+"Graphs_cut_totcount/"+"ProfileX_D3S2"+".png");
  
  ProjY_L2->Draw();                                                                                                                        
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_L2"+".png");                                                                                     
  ProjY_L3->Draw();                                                                                                                        
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_L3"+".png");                                                                                     
  ProjY_L4->Draw();                                                                                                                        
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_L4"+".png");                                                                                     
  ProjY_D1S1->Draw();                                                                                                                      
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_D1S1"+".png");                                                                                   
  ProjY_D2S1->Draw();                                                                                                                      
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_D2S1"+".png");                                                                                   
  ProjY_D3S1->Draw();                                                                                                                      
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_D3S1"+".png");                                                                                   
  ProjY_D1S2->Draw();                                                                                                                      
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_D1S2"+".png");                                                                                   
  ProjY_D2S2->Draw();                                                                                                                      
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_D2S2"+".png");                                                                                   
  ProjY_D3S2->Draw();                                                                                                                      
  //C.Print(Path1+"Graph1D_test1/"+"ProjY_D3S2"+".png"); 
  //gROOT->ProcessLine(".q");




  TString Path11 = "/eos/user/a/asehrawa/modcountgraphs_goodmodules/C/"; 
  TCanvas C2("C2");
  C2.cd();
  gStyle->SetOptStat(1111111);
    
  std::vector<int> run_number_C = {319337, 319347, 319348, 319349, 319449, 319450, 319452, 319456, 319459, 319460, 319462, 319463, 319464, 319466, 319467, 319468, 319469, 319470, 319471, 319472, 319486, 319488, 319503, 319506, 319524, 319525, 319526, 319528, 319579, 319606, 319625, 319639, 319656, 319657, 319658, 319659, 319678, 319687, 319697, 319698, 319756, 319840, 319841, 319847, 319848, 319849, 319851, 319852, 319853, 319854, 319908, 319909, 319910, 319912, 319913, 319914, 319915, 319928, 319941, 319942, 319950, 319991, 319992, 319993, 320002, 320003, 320006, 320007, 320008, 320009, 320010, 320011, 320012, 320023, 320024, 320025, 320026, 320038, 320039, 320040, 320058, 320059, 320060, 320061, 320062, 320063, 320064, 320065};
  
  std::cout<<run_number_C.size()<<std::endl;
  
  //readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/Run2018A_vetolist_second_iteration.txt");
  readModVeto("/afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/veto_Run2018C_0p02_3sigma_weights_cut.txt");

  int LS2=0;
  int run2=0;
  int ls2=0;
  int previousrunlumisec_count2=0;
  
  TGraph *g11;
  g11=new TGraph();

  TH2F *histo_counts1[1856];
  for (unsigned int i=0;i<1856;i++){
    histo_counts1[i]=new TH2F(TString("Histo_counts1")+i,"", 700,0,35000,300,0,0.016);
  }
  
  int modid2[1856];
  for (unsigned int j=0;j<run_number_C.size();j++){
    TString Path = "/eos/user/a/asehrawa/BRIL-PCC_25Sep2021/ZeroBias/Run2018C"; 
    TString infile=Path+"/"+run_number_C.at(j)+".csv"; 
    std::cout<< run_number_C.at(j)<<std::endl;  
    
    ifstream myfile (infile.Data());    
    if (!myfile.is_open()){
      cout << "Unable to open file: "<<infile.Data()<<endl; 
      return;
    }
    
    std::string line;    
    std::getline(myfile, line);
    std::stringstream iss(line);
    std::string token;
    
    for (unsigned int i=0;i<1856;i++){
      std::getline(iss,token, ',');
      std::stringstream modidiss(token);
      modidiss>>modid2[i];
      
    }
    
    int lumisec_count2=0;
    while (std::getline(myfile, line)){
      
      std::stringstream iss(line);
      std::string token;
      
      std::getline(iss,token, ',');
      std::stringstream runiss(token);
      runiss>>run;
      
      std::getline(iss,token, ',');
      std::stringstream lsiss(token);
      lsiss>>ls;
      //std::cout<<run <<" ";
      //std::cout<< ls;
      lumisec_count2++;      
      LS2 = previousrunlumisec_count2 + ls2; 
      
      int m_count[1856];
      int count=0;
      float totcount=0;      
      for (unsigned int i=0;i<1856;i++){
	std::getline(iss,token, ',');
	std::stringstream modidiss(token);
	modidiss>>count;
	m_count[i]=count;        
	if(MODVETO[modid[i]]==0){                                                                               
	  totcount+=count;
	}
      }
      
      for (unsigned int i=0;i<1856;i++){
	if(totcount > 50000000){
	  if(MODVETO[modid[i]]==0){
	    histo_counts1[i]->Fill(LS, m_count[i]/totcount);
	  }
	}
      }
    }
    
    previousrunlumisec_count2+=lumisec_count2;  
    myfile.close();    
    
  }
  
  
  TH1D* ProjY3[1856];
  for (unsigned int i=0;i<1856;i++){
    if(MODVETO[modid[i]]==0){ 
      ProjY3[i] = histo_counts1[i]->ProjectionY(TString("Projection3_")+i,0,700);
      g11->SetPoint(g11->GetN(), i, ProjY3[i]->GetMean());
      //std::cout<<g1->GetN()<<" "<<i<<" "<<ProjY[i]->GetMean()<<std::endl;
    }
  }
  
  
  std::vector<int> run_number1 = {317080, 317087, 317088, 317089, 317170, 317182, 317212, 317213, 317239, 317279,317291, 317292, 317295, 317296, 317297, 317319, 317320,317338, 317339, 317340, 317382, 317383, 317391, 317392, 317434, 317435, 317438, 317475, 317478, 317479, 317480, 317481, 317482, 317484, 317488, 317509, 317510, 317511, 317512, 317527, 317580, 317591, 317626, 317640, 317641, 317648,317649, 317650, 317661, 317663, 317683, 317696, 318070, 318622, 318653, 318661, 318662, 318663, 318667, 318669, 318670, 318675, 318712,318714, 318733, 318734, 318785,318816, 318817, 318819, 318820, 318828, 318834, 318836, 318837, 318872, 318874, 318876, 318877, 318939, 318944, 318945, 318953, 318980, 318981, 318982, 318983, 318984, 318992, 319006, 319011, 319015, 319016, 319018, 319019, 319077, 319097, 319098, 319099, 319100, 319101, 319103, 319104, 319124, 319125, 319159, 319160, 319173, 319174, 319175, 319176, 319177, 319190, 319222, 319223, 319254, 319255, 319256, 319260, 319262, 319263, 319264, 319265, 319266, 319267, 319268, 319270, 319273, 319274, 319300, 319310, 319311};
  
  std::cout<<run_number1.size()<<std::endl;

  int LS1=0;
  int run1=0;
  int ls1=0;
  int previousrunlumisec_count1=0;
  
  TGraph *g12;
  g12=new TGraph();

  TH2F *RunB_histo[1856];
  for (unsigned int i=0;i<1856;i++){
    RunB_histo[i]=new TH2F(TString("RunB_histogram")+i,"", 700,0,35000,300,0,0.016);
  }
  
  int modid1[1856];
  for (unsigned int j=0;j<run_number1.size();j++){
    TString Path1 = "/eos/user/a/asehrawa/BRIL-PCC_25Sep2021/ZeroBias/Run2018B"; 
    TString infile1=Path1+"/"+run_number1.at(j)+".csv"; 
    std::cout<< run_number1.at(j)<<std::endl;  
    
    ifstream myfile1 (infile1.Data());    
    if (!myfile1.is_open()){
      cout << "Unable to open file: "<<infile1.Data()<<endl; 
      return;
    }
    
    std::string line1;    
    std::getline(myfile1, line1);
    std::stringstream iss1(line1);
    std::string token1;
    
    for (unsigned int i=0;i<1856;i++){
      std::getline(iss1,token1, ',');
      std::stringstream modidiss1(token1);
      modidiss1>>modid1[i];
      
    }
    
    int lumisec_count1=0;
    while (std::getline(myfile1, line1)){
      std::stringstream iss1(line1);
      std::string token1;
      
      std::getline(iss1,token1, ',');
      std::stringstream runiss1(token1);
      runiss1>>run1;
      
      std::getline(iss1,token1, ',');
      std::stringstream lsiss1(token1);
      lsiss1>>ls1;
      //std::cout<<run <<" ";
      //std::cout<< ls;
      lumisec_count1++;      
      LS1 = previousrunlumisec_count1 + ls1; 
      
      int m_count1[1856];
      int count1=0;
      float totcount1=0;      
      for (unsigned int i=0;i<1856;i++){
	std::getline(iss1,token1, ',');
	std::stringstream modidiss1(token1);
	modidiss1>>count1;
	m_count1[i]=count1;        
	if(MODVETO[modid1[i]]==0){                                                                               
	  totcount1+=count1;
	        
	}
      }
      
      
      for (unsigned int i=0;i<1856;i++){
	if(totcount1 > 100000000){
	  if(MODVETO[modid1[i]]==0){
	    RunB_histo[i]->Fill(LS1, m_count1[i]/totcount1);
	  }
	}
      }
    }
    
    previousrunlumisec_count1+=lumisec_count1;  
    myfile1.close();    
    
  }
  
  
  TH1D* ProjY4[1856];
  for (unsigned int i=0;i<1856;i++){
    if(MODVETO[modid1[i]]==0){ 
      ProjY4[i] = RunB_histo[i]->ProjectionY(TString("RunB_Projection_")+i,0,700);
      g12->SetPoint(g12->GetN(), i, ProjY4[i]->GetMean());
      //std::cout<<g2->GetN()<<" "<<i<<" "<<ProjY1[i]->GetMean()<<std::endl;
    }
  }
  
  TGraph *g13;
  g13=new TGraph();
  TGraph *g14;
  g14=new TGraph();
  TH1D *h1;
  h1=new TH1D("histo1D", "weights (C-B)/B",100,-0.05,0.05);
  for (unsigned int i=0;i<1856;i++){
    //h1=new TH1D("histo1D"+i, "weights (A-B)/B",100,-0.05,0.05);
    if(MODVETO[modid1[i]]==0){
      g13->SetPoint(g13->GetN(), i, ((ProjY3[i]->GetMean())-(ProjY4[i]->GetMean()))/(ProjY4[i]->GetMean()));
      //std::cout<<g3->GetN()<<" "<<i<<" "<<((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean())<<std::endl;
      h1->Fill(((ProjY3[i]->GetMean())-(ProjY4[i]->GetMean()))/(ProjY4[i]->GetMean()));
      if(LY[modid[i]]!=0) {
	g14->SetPoint(g14->GetN(), BPIX_Z[modid[i]], ((ProjY3[i]->GetMean())-(ProjY4[i]->GetMean()))/(ProjY4[i]->GetMean()));
	//g4->SetPoint(g4->GetN(), i, FPIX_Z[modid[i]]);
	//std::cout<<BPIX_Z[modid[i]]<<"  "<<((ProjY_D[i]->GetMean())-(ProjY_B[i]->GetMean()))/(ProjY_B[i]->GetMean())<<std::endl;
      }
      if(LY[modid[i]]==0) {
	g14->SetPoint(g14->GetN(), FPIX_Z[modid[i]], ((ProjY3[i]->GetMean())-(ProjY4[i]->GetMean()))/(ProjY4[i]->GetMean()));
	//std::cout<<BPIX_Z[modid[i]]<<"  "<<((ProjY_D[i]->GetMean())-(ProjY_B[i]->GetMean()))/(ProjY_B[i]->GetMean())<<std::endl;
      }
    }
  }
  
  for (unsigned int i=0;i<1856;i++){
    if(MODVETO[modid1[i]]==0){
      //  if(((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean())<-0.01 || ((ProjY[i]->GetMean())-(ProjY1[i]->GetMean()))/(ProjY1[i]->GetMean())>0.01){
      //std::cout<<modid1[i]<<std::endl;
      //std::cout<<modid1[i]<<"  "<< ((ProjY3[i]->GetMean())-(ProjY4[i]->GetMean()))/(ProjY4[i]->GetMean())<<std::endl;                       
      //  }
    }
  }

  for (unsigned int i=0;i<1856;i++){
    if(MODVETO[modid1[i]]==0){
      if(((ProjY3[i]->GetMean())-(ProjY4[i]->GetMean()))/(ProjY4[i]->GetMean())<4.97936e-04-3*2.30851e-03 || ((ProjY3[i]->GetMean())-(ProjY4[i]->GetMean()))/(ProjY4[i]->GetMean())>4.97936e-04+3*2.30851e-03){
	//std::cout<<modid1[i]<<std::endl;
      }
    }
  }

  g11->SetMarkerStyle(8);
  g11->SetMarkerSize(0.4);
  g11->SetMarkerColor(1);
  g11->GetXaxis()->SetTitle("module number index");
  g11->GetYaxis()->SetTitle("module weights");
  g11->SetTitle(TString("Run2018C (Black)") +" & "+ TString("Run2018B (Red)"));
  g12->SetMarkerStyle(8);
  g12->SetMarkerSize(0.4);
  g12->SetMarkerColor(2);
  g13->SetMarkerStyle(8);
  g13->SetMarkerSize(0.4);
  g13->SetMarkerColor(4);
  g13->GetYaxis()->SetRangeUser(-0.05, 0.05);
  g13->GetXaxis()->SetTitle("module number index");
  g13->GetYaxis()->SetTitle("module weights (C-B)/B");
  g14->SetMarkerStyle(8);
  g14->SetMarkerSize(0.4);
  g14->SetMarkerColor(2);
  g14->GetXaxis()->SetRangeUser(-60,60);
  g14->GetYaxis()->SetRangeUser(-0.05, 0.05);
  g14->GetXaxis()->SetTitle("z coordinate");
  g14->GetYaxis()->SetTitle("module weights (C-B)/B");
  h1->GetXaxis()->SetTitle("weights (C-B)/B");
  h1->GetYaxis()->SetTitle("entries");
  g11->Draw("AP");
  g12->Draw("PSAME");
  C2.Print(Path11+"Graphs_cut_totcount/"+"module_weight_C_B_testing"+".png");

  g13->Draw("AP");
  C2.Print(Path11+"Graphs_cut_totcount/"+"module_weight_(C-B)_B_testing"+".png");
  
  g14->Draw("AP");
  TLine *l10 = new TLine(-60,0,60,0);
  l10->SetLineStyle(7);
  l10->SetLineWidth(3);
  l10->Draw("SAME"); 
  C2.Print(Path11+"Graphs_cut_totcount/"+"module_weight_(C-B)_C_vs_zcoordinate_testing"+".png");

  h1->Draw();
  C2.Print(Path11+"Graphs_cut_totcount/"+"module_weight_(C-B)_B_histogram_testing"+".png");
  





  
}


//see how modcount looks for entire Run 2018A, B and C without normalization
//sum counts and normalize individual module count graphs with respect to total counts 
//when we plot many runs, we should be careful about order of lumi sections (time ordering of lumi section values)
//next steps:- make loop over runs , create a vector with run list and iterate over this. Need to add a counter for the total number of lumi sections, then use this counter as x-axis value in the Graph.- need to fix order of the lumi sections, they are not time ordered in the csv file. for now maybe can define a variable like LS = run_index * 2500 + ls , where ls is the lumi section read from the file. 3000 is like a maximum number of lumi sections per run. run_index is just a counter for the runs, the run vector should be ordered.
//make ProfileX and ProjectionY graphs.
//make two more projections. First from 0 to 25000 lumi sections and second from 33000 to 43000 lumi sections
//graph of weights/mean values with module number on x axis and weight/mean value from ProjectionY on y axis.
//using second projection, plot module number vs difference (difference between mean values from first projection from 0 to 25000 and second projection from 33000 to 43000) in mean values.
//draw projections in a separate module loop.

