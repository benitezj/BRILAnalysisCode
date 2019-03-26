
#include "plot_linearity.C"


void compareFills(){


  C.Print("compareFills.pdf[");
    
  TLegend bx_leg(0.6,0.2,.82,0.4);
  bx_leg.SetLineColor(0);
  bx_leg.SetLineStyle(0);
  bx_leg.SetShadowColor(0);
  bx_leg.SetFillColor(0);
  bx_leg.SetFillStyle(0);
  
  TString det="pcc";
  
  std::vector<long> fills{6847,6854,7358};
  std::vector<TH2F*> histos;
  std::vector<TF1*> fits;
  for(int i=0;i<fills.size();i++){
    configFill(fills[i]);
    TH2F * H = getLinearityHisto(TString("H_fill")+i,det,BXLeading);
    H->SetMarkerColor(i+1);
    H->SetLineColor(i+1);
    histos.push_back(H);
    fits.push_back(fitLinearity(H));
  }


  C.Clear();
  for(int i=0;i<fills.size();i++){
    TH2F* H=histos[i];
    H->GetYaxis()->SetRangeUser(0.9,1.1);
    H->Draw(i==0?"histp":"histpsame");
    bx_leg.AddEntry(H,TString("FILL ")+fills[i],"flp");
  }
  bx_leg.Draw();
  C.Print("compareFills.pdf");
  
  
  C.Clear();
  for(int i=0;i<fills.size();i++){
    TH2F* H=histos[i];
    H->RebinX(20);
    TProfile * P=H->ProfileX(TString(H->GetName())+"_pfx",1,-1,"s");
    P->GetYaxis()->SetRangeUser(0.9,1.1);
    P->GetYaxis()->SetTitle(H->GetYaxis()->GetTitle());
    P->Draw(i==0?"histpe":"histpesame");
    fits[i]->Draw("lsame");
    //bx_leg.AddEntry(H,TString("FILL ")+fills[i],"flp");
  }
  bx_leg.Draw();
  C.Print("compareFills.pdf");
  
  C.Print("compareFills.pdf]");
  gROOT->ProcessLine(".q");

}
