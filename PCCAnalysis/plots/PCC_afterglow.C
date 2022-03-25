void PCC_afterglow() {

  TFile*f = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018A_newveto_RD/AlCaPCCRandom/Run2018A/316110.root");
  TFile*f1 = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/316110.root");
  
  //TFile*f = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/317696.root");                                            
  //TFile*f = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018B_newveto/AlCaPCCRandom/Run2018B_RD/317696.root");

  //TFile*f = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/319639.root");
  //TFile*f = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018C_newveto/AlCaPCCRandom/Run2018C_RD/319639.root");  

  TCanvas*C = new TCanvas("PCC","");

  TH2F * H1 = NULL;
  TH2F * H2 = NULL;
  
  H1 = (TH2F*)f->Get("RawLumiAvg_316110_0_1_52;1"); 
  H1->SetLineColor(1);
  H1->SetMarkerColor(1);
  if (H1==NULL){
  return;
  }

  H2 = (TH2F*)f1->Get("RawLumiAvg_316110_0_1_52;1");  
  H2->SetLineColor(2);
  H2->SetMarkerColor(2);

  if (H2==NULL){
  return;
  }

  //H1 = (TH2F*)f->Get("RawLumiAvg_317696_0_1_85;1");
  //H1->SetLineColor(1);
  //if (H1==NULL){
  //return;
  //}

  //H2 = (TH2F*)f->Get("CorrectedLumiAvg_317696_0_1_85;1");
  //H2->SetLineColor(2);
  //if (H2==NULL){
  //return;
  //}


  //H1 = (TH2F*)f->Get("RawLumiAvg_319639_0_1_78;1");
  //H1->SetLineColor(1);
  //if (H1==NULL){
  //return;
  //}

  //H2 = (TH2F*)f->Get("CorrectedLumiAvg_319639_0_1_78;1");
  //H2->SetLineColor(2);
  //if (H2==NULL){
  //return;
  //}


  gStyle->SetOptStat(0);
  H1->SetTitle("Raw histograms ratio run number 316110 lumi block 52");
  //H1->SetTitle("New (AS) run number 316110 lumi block 52");

  //H1->SetTitle("Old (JB) run number 317696 lumi block 85");                                                                                
  //H1->SetTitle("New (AS) run number 317696 lumi block 85");  

  //H1->SetTitle("Old (JB) run number 319639 lumi block 78");
  //H1->SetTitle("New (AS) run number 319639 lumi block 78");     

  H1->GetXaxis()->SetTitle("bx");
  H1->GetYaxis()->SetTitle("PCC");
  //H1->GetYaxis()->SetTitle("New/Old");
  H1->GetYaxis()->SetRangeUser(0, 6000);
  //H1->GetYaxis()->SetRangeUser(0.8, 1); 
  H1->Divide(H2);
  H1->Draw("p");
  //H2->Draw("psame");
  
  auto legend = new TLegend(0.75, 0.75, 0.88, 0.88);
  //legend->AddEntry(H1, "Raw Histo (JB)", "l");
  //legend->AddEntry(H2, "Raw Histo (AS)", "l");

  legend->AddEntry(H1, "New veto", "l");
  legend->AddEntry(H2, "Old veto", "l");


  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  legend->Draw("same");
  
 C->Print("/eos/user/a/asehrawa/BRIL-new/Raw_histogram_old_jb_RunA_ratio.png");
  //C->Print("/eos/user/a/asehrawa/BRIL-new/Raw_corr_histogram_new_as_RunA.png");

}

