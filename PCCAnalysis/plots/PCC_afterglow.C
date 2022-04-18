void PCC_afterglow() {

  TFile*f = new TFile("/eos/user/a/asehrawa/PCC/EXPRESS_datasets/AlCaPCCRandom/Run2018_RD_test/Run2018A/315690.root");
  //TFile*f = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018A_newveto_RD/AlCaPCCRandom/Run2018A/315690.root");
  TFile*f1 = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/315690.root");

  //TFile*f = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018A_newveto_RD/AlCaPCCRandom/Run2018A/316110.root");
  //TFile*f1 = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/316110.root");
  
  //TFile*f = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/317696.root");                                            
  //TFile*f = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018B_newveto/AlCaPCCRandom/Run2018B_RD/317696.root");
  
  //TFile*f = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/319639.root");
  //TFile*f = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018C_newveto/AlCaPCCRandom/Run2018C_RD/319639.root");  
  
  TCanvas*C = new TCanvas("PCC","");
  
  TH1F * H1 = NULL;
  TH1F * H2 = NULL;
  
  H1 = (TH1F*)f->Get("RawLumiAvg_315690_10_513_563;1");
  H1->SetLineColor(1);
  H1->SetMarkerColor(1);
  if (H1==NULL){
    return;
  }

  H2 = (TH1F*)f1->Get("RawLumiAvg_315690_10_513_563;1");
  H2->SetLineColor(2);
  H2->SetMarkerColor(2);

  if (H2==NULL){
    return;
  }

  int x_value[4000];
  float bincontent[4000];
  for(int j = 1; j <= H1->GetNbinsX(); j++) {
    x_value[j] =  H1->GetBinCenter(j);
    bincontent[j] = H1->GetBinContent(j); 
    if (bincontent[j] >= 500){
      std::cout<<"new "<<x_value[j]<<" "<<bincontent[j]<<std::endl;
      H1->Draw("p");
    }
  }

  int x_value1[4000];
  float bincontent1[4000];
  for(int k = 1; k <= H2->GetNbinsX(); k++) {
    x_value1[k] =  H2->GetBinCenter(k);
    bincontent1[k] = H2->GetBinContent(k);
    if (bincontent1[k] >=500){
      std::cout<<"old "<<x_value1[k]<<" "<<bincontent1[k]<<std::endl;
      H2->Draw("psame");
    }
  }

  gStyle->SetOptStat(0);
  H1->SetTitle("Raw histograms run number 315690 lumi block 10 (513-563)");  
  H1->GetXaxis()->SetTitle("bx");
  H1->GetYaxis()->SetTitle("PCC");
  //H1->GetYaxis()->SetTitle("New/Old");
  H1->GetYaxis()->SetRangeUser(0, 2500);
  //H1->GetYaxis()->SetRangeUser(0.8, 1); 
  //H1->Divide(H2);

  auto legend = new TLegend(0.75, 0.75, 0.88, 0.88);
  //legend->AddEntry(H1, "Raw Histo ", "l");
  //legend->AddEntry(H2, "Corr Histo ", "l");
  legend->AddEntry(H1, "New veto", "l");
  legend->AddEntry(H2, "Old veto", "l");
  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  legend->Draw("same");
  
  C->Print("/eos/user/a/asehrawa/BRIL-new/raw_old_new.png");
  //C->Print("/eos/user/a/asehrawa/BRIL-new/Raw_histogram_old_jb_RunA_ratio.png");

}

