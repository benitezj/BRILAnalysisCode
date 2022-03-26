void PCC_scalefactors() {
  
  TFile*f = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018A_newveto_RD/AlCaPCCRandom/Run2018A/316110.root");
  TFile*f1 = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/316110.root");
  
  //TFile*f = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/317696.root");                                            
  //TFile*f1 = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018B_newveto/AlCaPCCRandom/Run2018B_RD/317696.root");                   
  //TFile*f = new TFile("/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/319639.root");
  //TFile*f1 = new TFile("/eos/user/a/asehrawa/BRIL-new/PCC_reprocess_2018C_newveto/AlCaPCCRandom/Run2018C_RD/319639.root");  
  
  TCanvas*C = new TCanvas("PCC","");
  
  TH2F * H1 = NULL;
  TH2F * H2 = NULL;
  
  H1 = (TH2F*)f->Get("ScaleFactorsAvg_316110_0_1_52;1");
  H1->SetLineColor(1);
  H1->SetMarkerColor(1);
  if (H1==NULL){
    return;
  }
  
  H2 = (TH2F*)f1->Get("ScaleFactorsAvg_316110_0_1_52;1");
  H2->SetLineColor(2);
  H2->SetMarkerColor(2);
  if (H2==NULL){
    return;
  }
  
  //H1 = (TH2F*)f->Get("ScaleFactorsAvg_317696_0_1_85;1");
  //H1->SetLineColor(1);
  //if (H1==NULL){
  //return;
  //}

  //H2 = (TH2F*)f1->Get("ScaleFactorsAvg_317696_0_1_85;1");
  //H2->SetLineColor(2);
  //if (H2==NULL){
  //return;
  //}
  
  //H1 = (TH2F*)f->Get("ScaleFactorsAvg_319639_0_1_78;1");
  //H1->SetLineColor(1);
  //if (H1==NULL){
  //return;
  // }
  
  //H2 = (TH2F*)f1->Get("ScaleFactorsAvg_319639_0_1_78;1");
  //H2->SetLineColor(2);
  //if (H2==NULL){
  //return;
  // }
  
  
  gStyle->SetOptStat(0);
  H1->GetXaxis()->SetTitle("bx");
  H1->GetYaxis()->SetTitle("background correction");
  //H1->GetYaxis()->SetTitle("new/old");
  //H1->SetTitle("Scale factors ratio for colliding bunches (run 316110 LS block 52)");
  H1->SetTitle("Scale factors (run 316110 LS block 52)"); 

  //H1->SetTitle("Scale factors (run 317696 LS block 85)");
  //H1->SetTitle("Scale factors (run 319639 LS block 78)"); 
  
  
  int x_value;
  float bincontent;
  for(int j = 1; j <= H1->GetNbinsX(); j++) {
    x_value =  H1->GetBinCenter(j);
    bincontent = H1->GetBinContent(j);
    std::cout<<"H1 "<<x_value<<"  "<<bincontent<<std::endl;
    //if(bincontent > 0){
    //gr ->SetPoint(gr->GetN(), x_value, bincontent);
    //}
  }
  
  
  int x_value1;
  float bincontent1;
  for(int k = 1; k <= H2->GetNbinsX(); k++) {
    x_value1 =  H2->GetBinCenter(k);
    bincontent1 = H2->GetBinContent(k);
    std::cout<<"H2 "<<x_value1<<"  "<<bincontent1<<std::endl;
    //if(bincontent > 0){                                                                                                                    
    //gr ->SetPoint(gr->GetN(), x_value, bincontent);                                                                                        
    //}                                                                                                                                      
  }
  
  
  //H1->GetYaxis()->SetRangeUser(0.9, 1);
  
  //if((bincontent > 0.8) && (bincontent1 > 0.8)){
  //H1->Divide(H2);
  //}
  H1->Draw("p");
  H2->Draw("psame");
  //}
  
  auto legend = new TLegend(0.75, 0.75, 0.88, 0.88);
  //legend->AddEntry(H1, "New (AS)", "l");
  //legend->AddEntry(H2, "Old (JB)", "l");
  
  legend->AddEntry(H1, "New veto", "l");
  legend->AddEntry(H2, "Old veto", "l");
  
  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  legend->Draw("same");
  
  C->Print("/eos/user/a/asehrawa/BRIL-new/scalefactors_old_new_as_jb_RunA.png");
}
