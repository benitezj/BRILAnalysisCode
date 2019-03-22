
#include "plot_linearity.C"


void compareFills(){


  TString det="pcc";
  long fill_low=6847;
  long fill_high=7358;
  
  configFill(fill_low);
  TH2F * H_low = getLinearityHisto("H_low",det,BXLeading); 

  configFill(fill_high);
  TH2F * H_high = getLinearityHisto("H_high",det,BXLeading); 

  C.Print("compareFills.pdf[");


  TLegend bx_leg(0.6,0.2,.82,0.4);
  bx_leg.SetLineColor(0);
  bx_leg.SetLineStyle(0);
  bx_leg.SetShadowColor(0);
  bx_leg.SetFillColor(0);
  bx_leg.SetFillStyle(0);
  
  C.Clear();
  H_low->SetMarkerColor(2);
  H_low->SetLineColor(2);
  H_low->GetYaxis()->SetRangeUser(0.85,1.15);
  H_low->Draw("histp");
  H_high->SetMarkerColor(1);
  H_high->SetLineColor(1);
  H_high->Draw("histpsame");
  bx_leg.AddEntry(H_low,TString("FILL ")+fill_low,"flp");
  bx_leg.AddEntry(H_high,TString("FILL ")+fill_high,"flp");
  bx_leg.Draw();
  C.Print("compareFills.pdf");
    
  C.Print("compareFills.pdf]");
  gROOT->ProcessLine(".q");

}
