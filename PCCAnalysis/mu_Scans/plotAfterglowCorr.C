
void plotAfterglowCorr(){
  gROOT->ProcessLine(".x ../rootlogon.C");

  ////HF 
  TFile F("/eos/user/b/benitezj/BRIL/HighPU/HFCorrections_JingyuApril2019/Overall_Correction_HFOC_2018_bt_corr_fills_7358.root");
  TH1F* HFBefore=(TH1F*)F.Get("Before_Corr_325309_LS1_LS20_Fill7358");
  TH1F* HFAfter=(TH1F*)F.Get("After_Corr_325309_LS1_LS20_Fill7358");
  if(!HFBefore || !HFAfter) return;
  HFAfter->Divide(HFBefore);

  TH1F* HFBefore_2=(TH1F*)F.Get("Before_Corr_325309_LS21_LS40_Fill7358");
  TH1F* HFAfter_2=(TH1F*)F.Get("After_Corr_325309_LS21_LS40_Fill7358");
  if(!HFBefore_2 || !HFAfter_2) return;
  HFAfter_2->Divide(HFBefore_2);
  HFAfter_2->SetLineColor(2);

  TLegend leg(0.25,0.2,0.5,0.35);
  leg.AddEntry(HFAfter,"Fill 7358, LS 1-20","l");
  leg.AddEntry(HFAfter_2,"Fill 7358, LS 21-40","l");
  leg.SetFillStyle(0);
  leg.SetLineStyle(0);
  leg.SetLineWidth(0);
  leg.SetLineColor(0);

  TCanvas C;
  C.Clear();
  HFAfter->SetTitle("");
  HFAfter->GetYaxis()->SetRangeUser(0.95,1.01);
  HFAfter->GetXaxis()->SetRangeUser(749,763);
  HFAfter->GetYaxis()->SetTitle("afterglow correction");
  HFAfter->GetXaxis()->SetTitle("bcid");
  HFAfter->Draw("hist");
  HFAfter_2->Draw("histsame");
  leg.Draw();
  C.Print("HFAfterglowCorr.png");


  ////PCC 
  TFile FPCC("/eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-Express/Run2018E_10LS/325309.root");
  TH1F* PCCAfter=(TH1F*)FPCC.Get("ScaleFactorsAvg_325309_0_1_11");
  if(!PCCAfter) return;
  for(int b=PCCAfter->GetNbinsX();b>0;b--)
    PCCAfter->SetBinContent(b,PCCAfter->GetBinContent(b-1));

  TH1F* PCCAfter_2=(TH1F*)FPCC.Get("ScaleFactorsAvg_325309_2_23_33");
  if(!PCCAfter_2) return;
  PCCAfter_2->SetLineColor(2);
  for(int b=PCCAfter_2->GetNbinsX();b>0;b--)
    PCCAfter_2->SetBinContent(b,PCCAfter_2->GetBinContent(b-1));

  leg.Clear();
  leg.AddEntry(PCCAfter,"Fill 7358, LS 1-11","l");
  leg.AddEntry(PCCAfter_2,"Fill 7358, LS 23-33","l");
  C.Clear();
  PCCAfter->SetTitle("");
  PCCAfter->GetYaxis()->SetRangeUser(0.95,1.01);
  PCCAfter->GetXaxis()->SetRangeUser(749,763);
  PCCAfter->GetYaxis()->SetTitle("afterglow correction");
  PCCAfter->GetXaxis()->SetTitle("bcid");
  PCCAfter->Draw("hist");
  PCCAfter_2->Draw("histsame");
  leg.Draw();
  C.Print("PCCAfterglowCorr.png");



  

}
