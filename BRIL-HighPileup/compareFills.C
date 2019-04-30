
#include "config.C"


void compareFills(){
  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");
  C=new TCanvas();

  TLegend bx_leg(0.6,0.2,.82,0.4);
  bx_leg.SetLineColor(0);
  bx_leg.SetLineStyle(0);
  bx_leg.SetShadowColor(0);
  bx_leg.SetFillColor(0);
  bx_leg.SetFillStyle(0);
  
  TString det="pcc";
  
  std::vector<long> fills{7358,7274,6854,6847};
  std::vector<long> colors{1,2,4,6};
  

  float rmin=0.9;
  float rmax=1.1;

  int REBIN=40;
  
  ///fill the histograms and fit 
  std::vector<TH2F*> histos;
  std::vector<TF1*> fits;

  std::vector<TH2F*> histos_corr;
  std::vector<TF1*> fits_corr;
  
  for(long i=0;i<fills.size();i++){
    configFill(fills[i]);
    
    TH2F* H=getLinearityHistoAvgLS(TString("H_fill")+i,det,BXSpecial);
    H->GetYaxis()->SetTitle(DETName[det.Data()] + "/" + DETName[detsel.Data()]);
    H->SetMarkerColor(colors[i]);
    H->SetMarkerSize(0.6);
    H->SetLineColor(colors[i]);
    histos.push_back(H);
    fits.push_back(fitLinearity(H));

    get_bx_corrections(det);  
    H = getLinearityHistoAvgLS(TString("H_fill")+i,det,BXSpecial);
    H->GetYaxis()->SetTitle(DETName[det.Data()] + "/" + DETName[detsel.Data()]);
    H->SetMarkerColor(colors[i]);
    H->SetMarkerSize(0.6);
    H->SetLineColor(colors[i]);
    histos_corr.push_back(H);
    fits_corr.push_back(fitLinearity(H));
  }


  ///plot the scatter distributions
  C->Clear();
  for(int i=0;i<fills.size();i++){
    TH2F* H=histos[i];
    if(H->Integral()==0) continue;
    H->GetYaxis()->SetRangeUser(rmin,rmax);
    H->Draw(i==0?"histp":"histpsame");
    bx_leg.AddEntry(H,TString("FILL ")+fills[i],"flp");
  }
  bx_leg.Draw();
  C->Print(OUTPATH+"/compareFills.gif");

  C->Clear();
  char txt[100]; 
  labeltext.SetTextSize(0.035); 

  labeltext.SetTextColor(1);
  labeltext.DrawTextNDC(0.24,0.36,txt);

  std::vector<TProfile*> profiles;
  for(long i=0;i<fills.size();i++){
    TProfile *P = histos[i]->ProfileX(TString(histos[i]->GetName())+"_pfx",1,-1,"");
    profiles.push_back(P);
    P->RebinX(REBIN);
    P->GetYaxis()->SetRangeUser(rmin,rmax);
    P->GetYaxis()->SetTitle(histos[i]->GetYaxis()->GetTitle());

    if(histos[i]->Integral()==0) continue;
    P->Draw(i==0?"histpe":"histpesame");
    fits[i]->Draw("lsame");

    sprintf(txt,"slope  = %.2f +/- %.2f %%",100*fits[i]->GetParameter(1),100*fits[i]->GetParError(1));
    labeltext.SetTextColor(fits[i]->GetLineColor());
    labeltext.DrawTextNDC(0.24,0.32-i*0.05,txt);
  }
  bx_leg.Draw(); 
  C->Print(OUTPATH+"/compareFills_profile.gif");





  ////////////////
  /// with per bcid scale offset corrections
  C->Clear();
  for(long i=0;i<fills.size();i++){
    TH2F* H=histos_corr[i];
    if(H->Integral()==0) continue;
    H->GetYaxis()->SetRangeUser(rmin,rmax);
    H->Draw(i==0?"histp":"histpsame"); 
  }
  bx_leg.Draw();
  C->Print(OUTPATH+"/compareFills_corr.gif");

  
  C->Clear();
  labeltext.SetTextColor(1);
  labeltext.DrawTextNDC(0.24,0.36,txt);

  std::vector<TProfile*> profiles_corr;
  for(long i=0;i<fills.size();i++){
    TProfile *P = histos_corr[i]->ProfileX(TString(histos_corr[i]->GetName())+"_pfx",1,-1,"");
    profiles_corr.push_back(P);
    P->RebinX(REBIN);
    P->GetYaxis()->SetRangeUser(rmin,rmax);
    P->GetYaxis()->SetTitle(histos_corr[i]->GetYaxis()->GetTitle());

    if(histos_corr[i]->Integral()==0) continue;
    P->Draw(i==0?"histpe":"histpesame");
    fits_corr[i]->Draw("lsame");

    sprintf(txt,"slope  = %.2f +/- %.2f %%",100*fits_corr[i]->GetParameter(1),100*fits_corr[i]->GetParError(1));
    labeltext.SetTextColor(fits_corr[i]->GetLineColor());
    labeltext.DrawTextNDC(0.24,0.32-i*0.05,txt);
  }
  bx_leg.Draw(); 
  C->Print(OUTPATH+"/compareFills_profile_corr_perbx.gif");



  ////////////////////////////////
  // per fill correction
  for(long i=0;i<fills.size();i++){

    TGraphErrors GCompatibility;
    int points=0;
    for(int b=1;b <= profiles_corr[0]->GetNbinsX();b++){
      float ratio=0;
      
      if(fabs(profiles_corr[0]->GetBinContent(b)-1)>0.5
	 ||fabs(profiles_corr[i]->GetBinContent(b)-1)>0.5 ) continue;
      
      GCompatibility.SetPoint(points,profiles_corr[0]->GetBinCenter(b),
			      profiles_corr[i]->GetBinContent(b)/profiles_corr[0]->GetBinContent(b));

      GCompatibility.SetPointError(points,profiles_corr[0]->GetBinWidth(b)/2,
				   sqrt(pow(profiles_corr[0]->GetBinError(b),2) + pow(profiles_corr[i]->GetBinError(b),2)));
      points++;
      
    }



    TF1 fcomp("fcomp","[0]",0,20);
    GCompatibility.Fit(&fcomp,"","",0,20);
    
    GCompatibility.GetYaxis()->SetRangeUser(0.95,1.05);
    GCompatibility.GetXaxis()->SetTitle(histos[0]->GetXaxis()->GetTitle());
    GCompatibility.GetYaxis()->SetTitle(TString("Fill ")+fills[i]+" / Fill "+fills[0]);
  
    C->Clear();
    GCompatibility.Draw("ape");
    fcomp.Draw("lsame");

    sprintf(txt,"y=%.3f +/- %.3f,    chi2 = %.1f,   p-value = %.4f ",fcomp.GetParameter(0),fcomp.GetParError(0),fcomp.GetChisquare(), TMath::Prob(fcomp.GetChisquare(),fcomp.GetNDF()));
    labeltext.SetTextSize(0.035);
    labeltext.DrawTextNDC(0.3,0.2,txt);

    C->Print(OUTPATH+TString("compareFills_compatibility_fill")+fills[i]+".gif");
  
    ///apply corrections
    profiles_corr[i]->Scale(1./fcomp.GetParameter(0)); 

  }


  
  
  ///////////////////////////////////
  TProfile * P = (TProfile*) profiles_corr[0]->Clone("PComb");
  for(int i=1;i<fills.size();i++){
    if(histos_corr[i]->Integral()==0) continue; 
    P->Add(profiles_corr[i]);
  }
  TF1*FComb=fitLinearity(P);
  FComb->SetLineColor(1);

  labeltext.SetTextColor(1);
  
  // C->Clear();
  // P->GetYaxis()->SetRangeUser(rmin,rmax);
  // P->Draw("histpe");
  // FComb->Draw("lsame");
  // sprintf(txt,"slope  = %.2f +/- %.2f %%",100*FComb->GetParameter(1),100*FComb->GetParError(1));
  // labeltext.SetTextColor(FComb->GetLineColor());
  // labeltext.DrawTextNDC(0.24,0.32,txt);
  // //C->Print(OUTPATH+"/compareFills.pdf");
  // C->Print(OUTPATH+"/compareFills_combined_fills_corr.gif");


  C->Clear();
  for(long i=0;i<fills.size();i++){
    if(histos_corr[i]->Integral()==0) continue;
    profiles_corr[i]->Draw(i==0?"histpe":"histpesame");
  }
  
  bx_leg.Draw();
  
  FComb->Draw("lsame");
  sprintf(txt,"slope  = %.2f +/- %.2f %%",100*FComb->GetParameter(1),100*FComb->GetParError(1));
  labeltext.DrawTextNDC(0.24,0.32,txt);
  C->Print(OUTPATH+"/compareFills_profile_corr_perfill.gif");


  
  gROOT->ProcessLine(".q");
}
