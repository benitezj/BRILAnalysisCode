

void compareAfterglowBlocks(){

  TFile F1("Reprocess2023_v2/Random/Run2023D/370093.root","read");
  TFile F2("Reprocess2023_v2/Random/Run2023D/370095.root","read");
  if(F1.IsZombie()||F2.IsZombie()) return;

  //TString HName1="RawLumiAvg_370093_7_351_400";
  TString HName1="RawLumiAvg_370093_8_401_450";
  TString HName2="RawLumiAvg_370095_0_1_49";
  //TString HName2="RawLumiAvg_370095_1_50_99";
  //TString HName2="RawLumiAvg_370095_2_100_148";
  
  TH1F *H1=(TH1F*)F1.Get(HName1);
  TH1F *H2=(TH1F*)F2.Get(HName2);

  if(!H1 || !H2) return;

  TCanvas C("C","",1200,800);

  C.Clear();
  H1->GetYaxis()->SetTitle("PCC");
  H1->GetXaxis()->SetTitle("BCID");
  H1->GetYaxis()->SetRangeUser(0,1600);
  H1->SetStats(0);
  H1->SetMarkerStyle(8);
  H1->SetMarkerColor(1);
  H1->Draw("histp");
  H2->SetMarkerStyle(8);
  H2->SetMarkerColor(4);
  H2->Draw("histpsame");
  TLegend leg(0.5,0.8,0.9,0.9);
  leg.AddEntry(H1,HName1,"pl");
  leg.AddEntry(H2,HName2,"pl");
  leg.Draw();
  C.Print("compareAfterglowBlocks.png");

  C.Clear();
  TH1F* HRatio=(TH1F*)H2->Clone();
  HRatio->Divide(H1);
  HRatio->SetStats(0);
  HRatio->GetYaxis()->SetRangeUser(0.7,1.3);
  HRatio->GetYaxis()->SetTitle((HName2.ReplaceAll("RawLumiAvg_",""))+" / "+(HName1.ReplaceAll("RawLumiAvg_","")));
  HRatio->GetXaxis()->SetTitle("BCID");
  HRatio->Draw("histpl");
  TLine line;
  line.DrawLine(0,1,3565,1);
  C.Print("compareAfterglowBlocks_ratio.png");
  
  
}
