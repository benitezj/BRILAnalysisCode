
#include "config.C"


void compareFills(){


  C.Print("compareFills.pdf[");
    
  TLegend bx_leg(0.6,0.2,.82,0.4);
  bx_leg.SetLineColor(0);
  bx_leg.SetLineStyle(0);
  bx_leg.SetShadowColor(0);
  bx_leg.SetFillColor(0);
  bx_leg.SetFillStyle(0);
  
  TString det="pcc";
  
  std::vector<long> fills{6847,7358}; //6854
  float rmin=0.9;
  float rmax=1.1;
  
   
  std::vector<TH2F*> histos;
  std::vector<TF1*> fits;
  std::vector<TF1*> fitsl;
  std::vector<TF1*> fitsh;
  for(int i=0;i<fills.size();i++){
    configFill(fills[i]);
    //TH2F * H = getLinearityHisto(TString("H_fill")+i,det,BXSpecial);
    TH2F * H = getLinearityHistoAvgLS(TString("H_fill")+i,det,BXSpecial);
    H->SetMarkerColor(i+1);
    H->SetMarkerSize(0.6);
    H->SetLineColor(i+1);
    histos.push_back(H);
    fits.push_back(fitLinearity(H));
    fitsl.push_back(fitLinearity(H,2,8));
    fitsh.push_back(fitLinearity(H,8,16));
  }


  C.Clear();
  for(int i=0;i<fills.size();i++){
    TH2F* H=histos[i];
    H->GetYaxis()->SetRangeUser(rmin,rmax);
    H->Draw(i==0?"histp":"histpsame");
    bx_leg.AddEntry(H,TString("FILL ")+fills[i],"flp");
  }
  bx_leg.Draw();
  C.Print("compareFills.pdf");
  C.Print("compareFills.gif");
  

  std::vector<TProfile*> profiles;
  C.Clear();
  for(int i=0;i<fills.size();i++){
    TH2F* H=histos[i];
    H->RebinX(40);
    TProfile *P = H->ProfileX(TString(H->GetName())+"_pfx",1,-1,"");
    P->GetYaxis()->SetRangeUser(rmin,rmax);
    P->GetYaxis()->SetTitle(H->GetYaxis()->GetTitle());
    P->Draw(i==0?"histpe":"histpesame");
    fits[i]->Draw("lsame");
    //fitsl[i]->Draw("lsame");
    //fitsh[i]->Draw("lsame");

    //bx_leg.AddEntry(H,TString("FILL ")+fills[i],"flp");
    profiles.push_back(P);    
  }

  char txt[100];
  labeltext.SetTextSize(0.035);
  
  sprintf(txt,"fit results: ");
  labeltext.SetTextColor(1);
  labeltext.DrawTextNDC(0.24,0.36,txt);
  
  sprintf(txt,"slope  = %.2f +/- %.2f %%",100*fits[0]->GetParameter(1),100*fits[0]->GetParError(1));
  labeltext.SetTextColor(fitsl[0]->GetLineColor());
  labeltext.DrawTextNDC(0.24,0.32,txt);

  sprintf(txt,"slope  = %.2f +/- %.2f %%",100*fits[1]->GetParameter(1),100*fits[1]->GetParError(1));
  labeltext.SetTextColor(fits[1]->GetLineColor());
  labeltext.DrawTextNDC(0.24,0.26,txt);
  
  /* sprintf(txt,"slope 1 = %.2f +/- %.2f %%",100*fitsl[1]->GetParameter(1),100*fitsl[1]->GetParError(1)); */
  /* labeltext.SetTextColor(fitsl[1]->GetLineColor()); */
  /* labeltext.DrawTextNDC(0.24,0.26,txt); */

  /* sprintf(txt,"slope 2 = %.2f +/- %.2f %%",100*fitsh[1]->GetParameter(1),100*fitsh[1]->GetParError(1)); */
  /* labeltext.SetTextColor(fitsl[1]->GetLineColor()); */
  /* labeltext.DrawTextNDC(0.24,0.20,txt); */

  bx_leg.Draw();
  C.Print("compareFills.pdf");
  C.Print("compareFills_profile.gif");



  //////////Compatability test
  TGraphErrors GCompatibility;
  int points=0;
  for(int i=0;i<profiles[0]->GetNbinsX();i++){
    float ratio=0;
    if(profiles[0]->GetBinContent(i)>0) ratio= profiles[1]->GetBinContent(i)/profiles[0]->GetBinContent(i);
    float err=sqrt(pow(profiles[0]->GetBinError(i),2) + pow(profiles[1]->GetBinError(i),2));
    cout<<i<<" "<<profiles[0]->GetBinCenter(i)<<" "<<ratio<<" "<<err<<endl;
    if(ratio>0){
      GCompatibility.SetPoint(points,profiles[0]->GetBinCenter(i),ratio);
      GCompatibility.SetPointError(points,profiles[0]->GetBinWidth(i)/2,err);
      points++;
    }
  }

  C.Clear();
  GCompatibility.GetYaxis()->SetRangeUser(0.95,1.05);
  GCompatibility.GetXaxis()->SetTitle(histos[0]->GetXaxis()->GetTitle());
  GCompatibility.GetYaxis()->SetTitle(TString("Fill ")+fills[1]+" / Fill "+fills[0]);
  TF1 fcomp("fcomp","[0]",0,20);
  fcomp.SetLineColor(3);
  GCompatibility.Fit(&fcomp,"","",0,20);

  GCompatibility.Draw("ape");

  line.DrawLine(GCompatibility.GetXaxis()->GetXmin(),1,GCompatibility.GetXaxis()->GetXmax(),1);
  
  //char txt[100];
  sprintf(txt,"y=%.3f +/- %.3f,    chi2 = %.1f,   p-value = %.4f ",fcomp.GetParameter(0),fcomp.GetParError(0),fcomp.GetChisquare(), TMath::Prob(fcomp.GetChisquare(),fcomp.GetNDF()));
  labeltext.SetTextSize(0.035);
  labeltext.DrawTextNDC(0.3,0.2,txt);

  C.Print("compareFills.pdf");
  C.Print("compareFills_compatibility.gif");
  
  C.Print("compareFills.pdf]");
  gROOT->ProcessLine(".q");

}
