
void plotPCCWeights(){
  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  TFile F("BRILAnalysisCode/BRIL-HighPileup/bril_7358_RunDModVeto_PCCSplitNoWeight.root");
  TTree* t=(TTree*)F.Get("lumi");

  TH1F HW("HW","",9,0.5,9.5);

  TCanvas C;
  C.Divide(3,3);
  TH1F*hb[3];
  for(long b=1;b<=3;b++){
    C.cd(b);
    hb[b-1]=new TH1F(TString("h_b")+b,"",100,0,0.3);
    t->Draw(TString("pccb")+b+"/pcc>>"+hb[b-1]->GetName(),"ls>30&&ls<36");
    HW.SetBinContent(b,hb[b-1]->GetMean());
    cout<<"b"<<b<<" "<<hb[b-1]->GetMean()<<endl;
  }
  
  TH1F*hf1[3];
  for(long d=1;d<=3;d++){
    C.cd(3+d);
    hf1[d-1]=new TH1F(TString("h_f1d")+d,"",100,0,0.3);
    t->Draw(TString("pccf")+d+"p1/pcc>>"+hf1[d-1]->GetName(),"ls>30&&ls<36");
    HW.SetBinContent(3+d,hf1[d-1]->GetMean());
    cout<<"f1d"<<d<<" "<<hf1[d-1]->GetMean()<<endl;
  }

  TH1F*hf2[3];
  for(long d=1;d<=3;d++){
    C.cd(6+d);
    hf2[d-1]=new TH1F(TString("h_f2d")+d,"",100,0,0.3);
    t->Draw(TString("pccf")+d+"p2/pcc>>"+hf2[d-1]->GetName(),"ls>30&&ls<36");
    HW.SetBinContent(6+d,hf2[d-1]->GetMean());
    cout<<"f2d"<<d<<" "<<hf2[d-1]->GetMean()<<endl;
  }

  C.Print("PCCWeights_distributions.gif");

  C.Clear();
  HW.GetYaxis()->SetRangeUser(0,0.3);
  HW.GetYaxis()->SetTitle("weight");
  HW.GetXaxis()->SetTitle("PCC sub-detector");
  HW.Draw("hist");
  C.Print("PCCWeights.gif");

}
