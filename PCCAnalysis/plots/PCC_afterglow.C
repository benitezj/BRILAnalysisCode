void PCC_afterglow(int option=1) {
  
  TFile*f = new TFile("/eos/user/a/asehrawa/PCC/EXPRESS_datasets/AlCaPCCRandom/Run2018_RD_test/Run2018A/315690.root");
  TFile*f1 = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/315690.root");

  //TFile*f = new TFile("/eos/user/a/asehrawa/PCC/EXPRESS_datasets/AlCaPCCRandom/Run2018_RD_test/Run2018B/317511.root");
  //TFile*f1 = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/317511.root");

  //TFile*f = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018A_newveto_RD/AlCaPCCRandom/Run2018A/316110.root");
  //TFile*f1 = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/316110.root");
  
  //TFile*f = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/317696.root");                                            
  //TFile*f = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018B_newveto/AlCaPCCRandom/Run2018B_RD/317696.root");
  
  //TFile*f = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/319639.root");
  //TFile*f = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018C_newveto/AlCaPCCRandom/Run2018C_RD/319639.root");  
  
  TCanvas*C = new TCanvas("PCC","");
  //C->SetLogy();
  
  TH1F * H1 = NULL;
  TH1F * H2 = NULL;

  TH1F *H_new_colliding_bx;
  H_new_colliding_bx=new TH1F("h_new_collding_bx", "new veto Raw PCC collding bunches", 3500, 0.0, 3500);

  TH1F *H_old_colliding_bx;
  H_old_colliding_bx=new TH1F("h_old_colliding_bx", "old veto Raw PCC collding bunches", 3500, 0.0, 3500);

  TGraph *H_old_new_ratio;
  H_old_new_ratio=new TGraph();

  std::map<int, float> count_bx;

  H1 = (TH1F*)f->Get("RawLumiAvg_315690_10_513_563;1");
  H1->SetLineColor(1);
  H1->SetMarkerColor(1);
   if (H1==NULL){
  return;
  }
  

  //H1 = (TH1F*)f->Get("RawLumiAvg_317511_0_1_51;1");
  //H1->SetLineColor(1);
  //H1->SetMarkerColor(1);
  //if (H1==NULL){
  //return;
  //}

  //H2 = (TH1F*)f1->Get("RawLumiAvg_315690_10_513_563;1");
  //H2->SetLineColor(2);
  //H2->SetMarkerColor(2);
  
  //if (H2==NULL){
  //return;
  //}


  //H1 = (TH1F*)f->Get("CorrectedLumiAvg_315690_10_513_563;1");
  //H1->SetLineColor(1);
  //H1->SetMarkerColor(1);
  //if (H1==NULL){
  //return;
  //}

  H2 = (TH1F*)f->Get("CorrectedLumiAvg_315690_10_513_563;1");
  H2->SetLineColor(2);
  H2->SetMarkerColor(2);

  if (H2==NULL){
  return;
  }

  //H2 = (TH1F*)f1->Get("RawLumiAvg_317511_0_1_51;1");
  //H2->SetLineColor(2);
  //H2->SetMarkerColor(2);

  //if (H2==NULL){
  //return;
  //}


  if(option==1){
    H_new_colliding_bx->SetTitle("run number 315690 lumi block 10 (513-563)");
    //H_new_colliding_bx->SetTitle("run number 317511 lumi block 0 (1-51)");
    H_new_colliding_bx->GetXaxis()->SetTitle("bx");
    H_new_colliding_bx->GetYaxis()->SetTitle("PCC");
    //H_new_colliding_bx->GetYaxis()->SetTitle("Corr PCC");
    //H_new_colliding_bx->GetYaxis()->SetRangeUser(500, 2500);
    //H_new_colliding_bx->GetXaxis()->SetRangeUser(3250, 3500);
    //H_new_colliding_bx->GetYaxis()->SetRangeUser(10, 200);    
    //H_new_colliding_bx->GetXaxis()->SetRangeUser(1480, 1585);
    //H_new_colliding_bx->GetYaxis()->SetRangeUser(0, 2500);   


  int x_value;
  float bincontent;
  for(int j = 1; j <= H1->GetNbinsX(); j++) {
    x_value =  H1->GetBinCenter(j);
    bincontent = H1->GetBinContent(j); 
    //if (bincontent>=500){
      H_new_colliding_bx->Fill(x_value, bincontent);
      std::cout<<"new "<<x_value<<" "<<bincontent<<std::endl;
      count_bx[x_value]=bincontent;
      //}
  }
  
  int x_value1;
  float bincontent1;
  for(int k = 1; k <= H2->GetNbinsX(); k++) {
    x_value1 =  H2->GetBinCenter(k);
    bincontent1 = H2->GetBinContent(k);
    if (//bincontent1>=500 && 
count_bx[x_value1] > 0){
      H_old_colliding_bx->Fill(x_value1, bincontent1);
      std::cout<<"old "<<x_value1<<" "<<bincontent1<<std::endl;
      //H_old_new_ratio->SetPoint(H_old_new_ratio->GetN(), x_value1, count_bx[x_value1]/bincontent1);
      H_old_new_ratio->SetPoint(H_old_new_ratio->GetN(), x_value1, bincontent1/count_bx[x_value1]);
      //std::cout<<x_value<<"  "<<bincontent<<"  "<<x_value1<<" "<<bincontent1<<std::endl;      
     }
  }

  
  H_new_colliding_bx->SetLineColor(1);
  H_new_colliding_bx->SetMarkerColor(1);

  H_old_colliding_bx->SetLineColor(2);
  H_old_colliding_bx->SetMarkerColor(2);

  H_new_colliding_bx->Draw("histp");
  H_old_colliding_bx->Draw("histpsame");

  auto legend = new TLegend(0.75, 0.75, 0.88, 0.88);
  //legend->AddEntry(H_new_colliding_bx, "New veto", "l");
  //legend->AddEntry(H_old_colliding_bx, "Old veto", "l");
  legend->AddEntry(H_new_colliding_bx, "Raw PCC", "l");
  legend->AddEntry(H_old_colliding_bx, "Corr PCC", "l");

  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  legend->Draw("same");
  C->Print("/eos/user/a/asehrawa/BRIL-new/raw_pcc_old_new.png");
  //C->Print("/eos/user/a/asehrawa/BRIL-new/corr_pcc_old_new.png");

  H_old_new_ratio->SetTitle("run number 315690 lumi block 10 (513-563)");
  //H_old_new_ratio->SetTitle("run number 317511 lumi block 0 (1-51)");
  H_old_new_ratio->GetXaxis()->SetTitle("bx");
  H_old_new_ratio->GetYaxis()->SetTitle("new/old");
  //H_old_new_ratio->GetYaxis()->SetTitle("corr/raw");
  //H_old_new_ratio->GetYaxis()->SetRangeUser(1, 1.2);
  //H_old_new_ratio->GetYaxis()->SetRangeUser(0.8, 1);

  H_old_new_ratio->Draw("AP");
  //C->Print("/eos/user/a/asehrawa/BRIL-new/raw_pcc_old_new_ratio.png");
  C->Print("/eos/user/a/asehrawa/BRIL-new/corr_pcc_old_new_ratio.png");


  }

}
