void PCCperrun() {

  TString Path = "/eos/user/a/asehrawa/BRIL-new/";
  
  TCanvas C("C");
  C.cd();


  TGraph * g = new TGraph("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_newveto_2018A/Run2018A_ZB/\
runs.dat"); 
  
  //TGraph * g = new TGraph("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_newveto_2018A/Run2018A_ZB/runs.dat");
  //TGraph * g = new TGraph("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_newveto_2018B/Run2018B_ZB/runs.dat");
  TGraph * g = new TGraph("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_newveto_2018C/Run2018C_ZB/runs.dat");



  //TH1* h = new TH1D("h", "PCC per run ratio", 146, 315252, 316995); 
  //TH1* h = new TH1D("h", "PCC per run ratio", 132, 317080, 319311);
  TH1* h = new TH1D("h", "PCC per run ratio", 88, 319337, 320065);

  auto nPoints = g->GetN(); 
  for(int i=0; i < nPoints; ++i) {
    double x,y;
    g->GetPoint(i, x, y);
    h->Fill(x,y); 
  }

  h->SetMarkerColor(1);
  h->SetLineColor(1);
  //h->GetYaxis()->SetRangeUser(0.3, 0.8);
  //h->GetYaxis()->SetRangeUser(0.8, 1.8);
  h->GetYaxis()->SetRangeUser(1, 1.2);
  g->SetMarkerStyle(20);
  g->SetMarkerSize(0.5);
  g->SetMarkerColor(1);
  g->SetLineColor(1);
  g->SetTitle("PCC per run");
  g->GetXaxis()->SetTitle("run number");
  g->GetYaxis()->SetTitle("PCC");
  //g->Draw("ap");
  //C.Print(Path+"PCCperrun_newveto_RunA"+".png");
  
 
  //TGraph * g1 = new TGraph("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_oldveto_Run2018A/Run2018A/runs.dat");
  //TGraph * g1 = new TGraph("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_oldveto_Run2018B/Run2018B/runs.dat");
  TGraph * g1 = new TGraph("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_oldveto_Run2018C/Run2018C/runs.dat");


  //TH1* h1 = new TH1D("h1", "PCC per run (old veto)", 146, 315252, 316995);
  //TH1* h1 = new TH1D("h1", "PCC per run (old veto)", 132, 317080, 319311);
  TH1* h1 = new TH1D("h1", "PCC per run (old veto)", 88, 319337, 320065);
 
  auto nPoints1 = g1->GetN();                                                                        
  for(int j=0; j < nPoints1; ++j) {
    double x1,y1;
    g1->GetPoint(j, x1, y1);
    h1->Fill(x1,y1);                                                                                                               
  }
  
  h1->SetMarkerColor(2);
  h1->SetLineColor(2);


  g1->SetMarkerStyle(20);
  g1->SetMarkerSize(0.5);
  g1->SetMarkerColor(2);
  g1->SetLineColor(2);
  g1->SetTitle("PCC per run (old veto)");
  g1->GetXaxis()->SetTitle("run number");
  g1->GetYaxis()->SetTitle("PCC");
  //g1->Draw("psame");


  auto* legend = new TLegend(0.77,0.77,0.89,0.89);                                                                                           
  legend->AddEntry(g,"new veto","l");
  legend->AddEntry(g1,"old veto","l");  
  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  //legend->Draw("same");    

  C.Print(Path+"PCCperrun_old_new_veto_RunC"+".png");

  h->Divide(h1);
  h->Draw("histp");
  //h1->Draw("histsame");
  
  C.Print(Path+"PCCperrun_histo_old_new_veto_RunC"+".png");

}
