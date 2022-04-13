#include <iostream>
#include <fstream>
#include <string>
#include <TLegend.h>
void comparelsdotdat_express() {
  
  TString Path="/eos/user/a/asehrawa/BRIL-new";
  
  TString inpath="/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC/EXPRESS_datasets/Run2018_ZB_test/Run2018A";
  
  gROOT->ProcessLine(".x /afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/rootlogon.C");
  
  ifstream myfile((inpath+"/ls.dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file";
    return;
    
  }

  cout<<"opened "<<(inpath+"/ls.dat")<<std::endl;
  
  TGraph* graph = new TGraph(59103);
   
  std::string line;

  int run=0;
  int ls=0;
  int LS=0;

  double count=0;
  int Lumicounter=0;
  
  while (std::getline(myfile, line)){
    
    std::getline(myfile, line);
    std::stringstream iss(line);
    iss>>run>>ls>>count;
  
    graph->SetPoint(Lumicounter, Lumicounter, count);
    //cout<<"new "<<run<<"  "<<ls<<"  "<<Lumicounter<<"  "<<count<<endl;
    
    Lumicounter++;
  
  }

  myfile.close(); 



  TH1* h = new TH1D("h", "new/old", 59708, 0.0, 30000); 
  auto nPoints = graph->GetN(); 
  for(int i=0; i < nPoints; ++i) {
    double x,y;
    graph->GetPoint(i, x, y);
    h->Fill(x,y); 
  }
  


  TString inpath1="/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_oldveto_Run2018A/Run2018A";
    
  ifstream myfile1((inpath1+"/ls.dat").Data());
  if (!myfile1.is_open()){
    cout << "Unable to open file";
    return;
    
  }

  cout<<"opened "<<(inpath1+"/ls.dat")<<std::endl;
  
  TGraph* graph1 = new TGraph(59708);
   
  std::string line1;

  int run1=0;
  int ls1=0;

  double count1=0;
  int Lumicounter1=0;
  
  while (std::getline(myfile1, line1)){
    
    std::getline(myfile1, line1);
    std::stringstream iss1(line1);
    iss1>>run1>>ls1>>count1;
  
    graph1->SetPoint(Lumicounter1, Lumicounter1, count1);
    //cout<<"old "<<run1<<"  "<<ls1<<"  "<<Lumicounter1<<"  "<<count1<<endl;
    
    Lumicounter1++;
  
  }

  myfile1.close(); 


  TH1* h1 = new TH1D("h1", "PCC count per ls (old)", 59708, 0.0, 30000); 
  auto nPoints1 = graph1->GetN();                                                                        
  for(int j=0; j < nPoints1; ++j) {
    double x1,y1;
    graph1->GetPoint(j, x1, y1);
    h1->Fill(x1,y1);                                                                                                               
  }

   
  TCanvas C;
  //C.SetLogy();
  
  graph->SetTitle("PCC per lumi section (new veto)");
  graph->GetXaxis()->SetTitle("lumi section");
  graph->GetYaxis()->SetTitle("PCC");
  graph->SetMarkerColor(1);
  graph->SetMarkerSize(0.5);
  graph->SetMarkerStyle(8); 
  graph->Draw("AP");  

  C.Print(Path+"/compare_lsdotdat_Run2018A_newveto.png");
  
  graph1->SetTitle("PCC per lumi section (old veto)");
  graph1->GetXaxis()->SetTitle("lumi section");
  graph1->GetYaxis()->SetTitle("PCC");
  graph1->SetMarkerColor(2);
  graph1->SetMarkerSize(0.5);
  graph1->SetMarkerStyle(8);
  graph1->Draw("AP");

  C.Print(Path+"/compare_lsdotdat_Run2018A_oldveto.png");

  h->GetYaxis()->SetRangeUser(0, 3);
  h->GetXaxis()->SetTitle("lumi section");
  h->GetYaxis()->SetTitle("new/old");
  h->SetMarkerColor(1);
  h->SetMarkerSize(0.5);
  h->SetMarkerStyle(8);

  if(&y1!=0){
  h->Divide(h1);
  }
  h->Draw("histp");
  
  C.Print(Path+"/compare_lsdotdat_hist_Run2018A_newveto_ratio.png");

  //h1->Draw("histp");
  //C.Print(Path+"/compare_lsdotdat_hist_Run2018A_oldveto.png");

}
