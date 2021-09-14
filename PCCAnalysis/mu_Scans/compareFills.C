
#include "config.C"

void compareFills(){
  
  bool debug=0;
  
  gROOT->ProcessLine(".x ../rootlogon.C");
  C=new TCanvas();
  
  
  TString det="pcc";
  std::vector<long> fills{7358,6847};
  std::vector<long> colors{1,2};
  
  std::vector<TH2F*> histos;
  std::vector<TGraphErrors*> graphs;
  std::vector<TF1*> fits;
  
  int nbx[10];
  std::vector<TH2F*> histos_bx[10];
  std::vector<TF1*> fits_bx[10];
  
  float rmin=0.9;
  float rmax=1.1;
  int REBIN=20;

  /////////////////////////////
  ///fill the histograms and fit 
  for(long i=0;i<fills.size();i++){
    configFill(fills[i]);
    //get_bx_corrections(det); //this will correct each bx to have y-intercept = 1

    nbx[i]=BXSpecial.size();

    ///////2D histogram containing the ratios
    TH2F* H=getLinearityHistoAvgLS(TString("H_fill")+i,det,BXSpecial);
    H->GetYaxis()->SetTitle(DETName[det.Data()] + "/" + DETName[detsel.Data()]);
    H->SetMarkerColor(colors[i]);
    H->SetMarkerSize(0.6);
    H->SetLineColor(colors[i]);
    histos.push_back(H);
    //fits.push_back(fitLinearity(H));
    
    
    /// per bx
    for(long b=0;b<BXSpecial.size();b++){
      TH2F* H=getLinearityHistoAvgLS(TString("H_fill")+b,det,std::vector<long>{BXSpecial[b]});
      H->GetYaxis()->SetTitle(DETName[det.Data()] + "/" + DETName[detsel.Data()]);
      H->SetMarkerColor(colors[i]);
      H->SetMarkerSize(0.6);
      H->SetLineColor(colors[i]);
      histos_bx[i].push_back(H);
      fits_bx[i].push_back(fitLinearity(H));
    }
  
  }



  /////////////////////////////////
  ///// perform averaging of points
  for(long i=0;i<fills.size();i++){
    TGraphErrors* G=getLinearityGraph(histos[i]);
    G->SetMarkerColor(colors[i]);
    G->SetMarkerSize(0.6);
    G->SetLineColor(colors[i]);
    graphs.push_back(G);
    fits.push_back(fitLinearity(G));
  }
  


  HLinearityFrame.GetYaxis()->SetTitle(DETName[det]+" / "+DETName[detsel]);
  HLinearityFrame.GetYaxis()->SetRangeUser(rmin,rmax);
  HLinearityFrame.GetXaxis()->SetTitle(DETName[detsel] + " SBIL");


  //////////////////////////////
  ///show the scatter plots per fill
  for(int i=0;i<fills.size();i++){
    C->Clear();
    HLinearityFrame.Draw("hist");
    //histos[i]->GetYaxis()->SetRangeUser(rmin,rmax);
    histos[i]->Draw("histpsame");
    fits[i]->Draw("lsame");
    C->Print(OUTPATH+"/compareFills_scatter_fill"+fills[i]+".png");

    for(long b=0;b<nbx[i];b++){
      C->Clear();
      histos_bx[i][b]->GetYaxis()->SetRangeUser(rmin,rmax);
      histos_bx[i][b]->Draw("histp");
      fits_bx[i][b]->Draw("lsame");
      C->Print(OUTPATH+"/compareFills_scatter_fill"+fills[i]+"_bcid"+b+".png");
    }
  }


  //////////////////////////////
  ///show the graphs
  for(int i=0;i<fills.size();i++){
    C->Clear();
    HLinearityFrame.Draw("hist");
    graphs[i]->Draw("pesame");
    fits[i]->Draw("lsame");
    C->Print(OUTPATH+"/compareFills_graph_fill"+fills[i]+".png");
  }
  
  
  ////////////////////////////
  /// combine graphs into one
  TGraphErrors* GECombined = (TGraphErrors*) graphs[0]->Clone();
  GECombined->SetName("GECombined");
  double x=0;
  double y=0;
  for(long i=1;i<fills.size();i++){
    for(int n=0;n<graphs[i]->GetN();n++){
      graphs[i]->GetPoint(n,x,y); 
      GECombined->SetPoint(graphs[0]->GetN()+n,x,y);
      GECombined->SetPointError(graphs[0]->GetN()+n, graphs[i]->GetErrorX(n), graphs[i]->GetErrorY(n));
    }
  }
  TF1* FComb = fitLinearity(GECombined);
  
  
  ////////////////////////
  ///show the fill graphs in same plot
  TLegend legend(0.2,0.2,.7,0.4);
  legend.SetLineColor(0);
  legend.SetLineStyle(0);
  legend.SetShadowColor(0);
  legend.SetFillColor(0);
  legend.SetFillStyle(0);
  char txt[100];
  C->Clear();
  HLinearityFrame.Draw("hist");
  for(long i=0;i<fills.size();i++){
    graphs[i]->Draw("pesame");
    fits[i]->Draw("lsame");
    sprintf(txt,"slope  = %.2f +/- %.2f %%", 100*fits[i]->GetParameter(1), 100*fits[i]->GetParError(1));
    legend.AddEntry(graphs[i],TString("FILL ")+fills[i]+", "+txt,"flp");
  }
  legend.Draw();
  C->Print(OUTPATH+"/compareFills_graph_fillstogether.png");


  TLegend legend2(0.2,0.2,.4,0.4);
  legend2.SetLineColor(0);
  legend2.SetLineStyle(0);
  legend2.SetShadowColor(0);
  legend2.SetFillColor(0);
  legend2.SetFillStyle(0);
  C->Clear();
  HLinearityFrame.Draw("hist");
  for(long i=0;i<fills.size();i++){
    graphs[i]->Draw("pesame");
    legend2.AddEntry(graphs[i],TString("FILL ")+fills[i],"flp");
  }
  FComb->SetLineColor(4);
  FComb->Draw("lsame");
  sprintf(txt,"slope  = %.2f +/- %.2f %%", 100*FComb->GetParameter(1), 100*FComb->GetParError(1));
  labeltext.SetTextColor(4);
  labeltext.SetTextSize(0.035); 
  labeltext.DrawTextNDC(0.5,0.32,txt);
  legend2.Draw();
  C->Print(OUTPATH+"/compareFills_graph_fillstogether_onefit.png");


  
  gROOT->ProcessLine(".q");
}



 
//  ////////////////////////////////////////////
//  /// show the profiles of the scatter plots
//  TLegend bx_leg(0.2,0.2,.7,0.4);
//  bx_leg.SetLineColor(0);
//  bx_leg.SetLineStyle(0);
//  bx_leg.SetShadowColor(0);
//  bx_leg.SetFillColor(0);
//  bx_leg.SetFillStyle(0);
//  
//  C->Clear();
//  char txt[100]; 
//  labeltext.SetTextSize(0.035); 
//  labeltext.SetTextColor(1);
//  labeltext.DrawTextNDC(0.24,0.36,txt);
//  std::vector<TProfile*> profiles;
//  for(long i=0;i<fills.size();i++){
//    TProfile *P = histos[i]->ProfileX(TString(histos[i]->GetName())+"_pfx",1,-1,"");
//    profiles.push_back(P);
//    P->RebinX(REBIN);
//    P->GetYaxis()->SetRangeUser(rmin,rmax);
//    P->GetYaxis()->SetTitle(histos[i]->GetYaxis()->GetTitle());
//
//    if(histos[i]->Integral()==0) continue;
//    P->Draw(i==0?"histpe":"histpesame");
//    fits[i]->Draw("lsame");
//
//    sprintf(txt,"slope  = %.2f +/- %.2f %%",100*fits[i]->GetParameter(1),100*fits[i]->GetParError(1));
//    //labeltext.SetTextColor(fits[i]->GetLineColor());
//    //labeltext.DrawTextNDC(0.24,0.32-i*0.05,txt);
//
//    bx_leg.AddEntry(P,TString("FILL ")+fills[i]+": "+txt,"flp");
//  }
//  bx_leg.Draw(); 
//  C->Print(OUTPATH+"/compareFills_profile_perfill.gif");
//
//
//  ////////////////////////////////
//  // per fill norm correction
//  TF1 fcomp("fcomp","[0]",0,20);
//  for(long i=1;i<fills.size();i++){
//
//    TGraphErrors GCompatibility;
//    int points=0;
//    for(int b=1;b <= profiles[0]->GetNbinsX();b++){
//      float ratio=0;
//      if(fabs(profiles[0]->GetBinContent(b)-1)>0.5 ||fabs(profiles[i]->GetBinContent(b)-1)>0.5 ) continue;
//      GCompatibility.SetPoint(points, profiles[0]->GetBinCenter(b), profiles[i]->GetBinContent(b)/profiles[0]->GetBinContent(b));
//      GCompatibility.SetPointError(points, profiles[0]->GetBinWidth(b)/2, sqrt(pow(profiles[0]->GetBinError(b),2) + pow(profiles[i]->GetBinError(b),2)));
//      points++;
//    }
//
//    GCompatibility.Fit(&fcomp,"","",0,20);    
//    GCompatibility.GetYaxis()->SetRangeUser(0.95,1.05);
//    GCompatibility.GetXaxis()->SetTitle(histos[0]->GetXaxis()->GetTitle());
//    GCompatibility.GetYaxis()->SetTitle(TString("Fill ")+fills[i]+" / Fill "+fills[0]);
//    C->Clear();
//    GCompatibility.Draw("ape");
//    fcomp.Draw("lsame");
//    sprintf(txt,"y=%.3f +/- %.3f,    chi2 = %.1f,   p-value = %.4f ",fcomp.GetParameter(0),fcomp.GetParError(0),fcomp.GetChisquare(), TMath::Prob(fcomp.GetChisquare(),fcomp.GetNDF()));
//    labeltext.SetTextSize(0.035);
//    labeltext.DrawTextNDC(0.3,0.2,txt);
//    C->Print(OUTPATH+TString("/compareFills_fillratio_")+fills[i]+"over"+fills[0]+".gif");
//  
//    ///apply corrections
//    profiles[i]->Scale(1./fcomp.GetParameter(0)); 
//  }
//
//  
//  ///////////////////////////////////
//  /// combined fills 
//  TLegend legend(0.2,0.2,.45,0.4);
//  legend.SetLineColor(0);
//  legend.SetLineStyle(0);
//  legend.SetShadowColor(0);
//  legend.SetFillColor(0);
//  legend.SetFillStyle(0);
//    
//  C->Clear();
//  for(long i=0;i<fills.size();i++){
//    if(histos[i]->Integral()==0) continue;
//    profiles[i]->SetMarkerStyle(8);
//    profiles[i]->SetMarkerSize(0.7);
//    profiles[i]->Draw(i==0?"histpe":"histpesame");
//    legend.AddEntry(profiles[i],TString("FILL ")+fills[i],"flp");
//  }
//
//
////  TProfile * P = (TProfile*) profiles[0]->Clone("PComb");
////  for(int i=1;i<fills.size();i++){
////    if(histos[i]->Integral()==0) continue; 
////    P->Add(profiles[i]);
////  }
////  TF1*FComb=fitLinearity(P);
//  //P->SetMarkerColor(3);
//  //P->Draw("histpesame");
//  //FComb->SetLineColor(4);
//  //FComb->Draw("lsame");
//  //sprintf(txt,"slope  = %.2f +/- %.2f %%", 100*FComb->GetParameter(1), 100*FComb->GetParError(1));
//  //labeltext.SetTextColor(4);
//  //labeltext.DrawTextNDC(0.24,0.32,txt);
//
//  legend.Draw();
//  C->Print(OUTPATH+"/compareFills_profile_combinedfills.gif");


//    //////////////////////////////////////////
//    ///// show the slope per bcid
//    for(long i=0;i<fills.size();i++){
//      TGraphErrors GSlopeVsBCID;
//      for(long b=0;b<nbx[i];b++){
//    	GSlopeVsBCID.SetPoint(b,b+1,fits_bx[i][b]->GetParameter(1));
//    	GSlopeVsBCID.SetPointError(b,0,fits_bx[i][b]->GetParError(1));
//      }
//      C->Clear();
//      GSlopeVsBCID.GetXaxis()->SetTitle("bcid");
//      GSlopeVsBCID.GetYaxis()->SetTitle("slope");
//      GSlopeVsBCID.GetYaxis()->SetRangeUser(-0.01,0.01);
//      GSlopeVsBCID.Draw("ape");
//      C->Print(OUTPATH+"/compareFills_SlopeVsBcid_"+fills[i]+".gif");
//    }

