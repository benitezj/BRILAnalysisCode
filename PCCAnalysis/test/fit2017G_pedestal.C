
void fit2017G_pedestal(){

  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  //TFile CorrLumi("/Users/benitezj/analysis/PCC2017PbPb/Run2017G_v3/306773.root");
  //TFile CorrLumi("/Users/benitezj/analysis/PCC2017PbPb/Run2017G_v4/306773.root");
  //TFile CorrLumi("/Users/benitezj/analysis/PCC2017PbPb/Run2017G_v3/306793.root");
  TFile CorrLumi("/Users/benitezj/analysis/PCC2017PbPb/Run2017G_v4/306793.root");

  gROOT->cd();
  
  TH1F* HC=0;
  
  TF1 F("fit","[0]",0,4000);
  TF1 FL("fitL","[0]",0,4000);

  TGraph GPedestal;
  TGraph GLumi;
  TGraph GPedFrac;
  int npts=0;
  
  TList * Keys = (TList*)CorrLumi.GetListOfKeys();
  for(int i=0;i<Keys->GetSize();i++){
    TKey * k = (TKey*) Keys->At(i);
    if(TString(k->GetClassName()).CompareTo("TH1F")==0
       && TString(k->GetName()).Contains("CorrectedLumiAvg")){
      cout<<k->GetClassName()<<" "<<k->GetTitle()<<" "<<k->GetName()<<endl;


      
      TH1F * H=(TH1F*)(CorrLumi.Get(k->GetName())->Clone("HP"));
      TH1F * HL=(TH1F*)(CorrLumi.Get(k->GetName())->Clone("HL"));
      
      for(int b=1;b<=H->GetNbinsX();b++){
	if(H->GetBinContent(b)>20){ 
	  H->SetBinContent(b,0);
	  H->SetBinError(b,0);
	}
	else {
	  HL->SetBinContent(b,0);
	  HL->SetBinError(b,0);
	}
      }
      
      H->Fit(&F,"","Q",0,4000);
      GPedestal.SetPoint(npts,npts,F.GetParameter(0));

      HL->Fit(&FL,"","Q",0,4000);
      GLumi.SetPoint(npts,npts,FL.GetParameter(0));

      GPedFrac.SetPoint(npts,npts,F.GetParameter(0)/FL.GetParameter(0));
      npts++;

      //save the first histo for example fit
      if(HC==0) HC=(TH1F*)(CorrLumi.Get(k->GetName())->Clone());
    }
  }



  TCanvas C;

  TString RawName=HC->GetName();
  RawName.ReplaceAll("Corrected","Raw"); // "RawLumiAvg_306773_0_1_49");
  TH1F * HR=(TH1F*)CorrLumi.Get(RawName.Data());
  

  ///Fit the pedestal for the example 
  TH1F * H=(TH1F*)HC->Clone("H");
  for(int b=1;b<=H->GetNbinsX();b++)
    if(H->GetBinContent(b)>20){ 
      H->SetBinContent(b,0);
      H->SetBinError(b,0);
    }
  H->Fit(&F);
  F.SetLineColor(2);


  C.SetLogy(1);
  HR->SetMarkerColor(4);
  HR->SetMarkerStyle(8);
  HR->SetMarkerSize(0.5);
  HR->GetYaxis()->SetTitle("PCC");
  HR->GetXaxis()->SetTitle("bx id");
  HC->SetMarkerStyle(8);
  HC->SetMarkerSize(0.5);
  
  C.Clear();
  HR->GetYaxis()->SetRangeUser(0.1,250);
  HR->Draw("histp");
  HC->Draw("histpsame");
  F.Draw("lsame");	
  C.Print("PCC2017PbPb/fit2017G_pedestal_RawAndCorr.gif");


  /* C.Clear(); */
  /* HR->GetXaxis()->SetRangeUser(400,800); */
  /* HR->Draw("histp"); */
  /* HC->Draw("histpsame"); */
  /* F.Draw("lsame");	 */
  /* C.Print("PCC2017PbPb/fit2017G_pedestal_RawAndCorr_zoom.gif"); */


  

  C.SetLogy(0);
  C.Clear();
  GPedestal.GetYaxis()->SetRangeUser(0,10);
  GPedestal.GetYaxis()->SetTitle("pedestal");
  GPedestal.GetXaxis()->SetTitle("lumi section block");
  GPedestal.Draw("ap");
  C.Print("PCC2017PbPb/fit2017G_pedestal_vs_lsblock.gif");

  C.Clear();
  GLumi.GetYaxis()->SetRangeUser(0,180);
  GLumi.GetYaxis()->SetTitle("PCC");
  GLumi.GetXaxis()->SetTitle("lumi section block");
  GLumi.Draw("ap");
  C.Print("PCC2017PbPb/fit2017G_pedestal_lumi_vs_lsblock.gif");

  C.Clear();
  GPedFrac.GetYaxis()->SetRangeUser(-0.001,0.15);
  GPedFrac.GetYaxis()->SetTitle("Pedestal / PCC");
  GPedFrac.GetXaxis()->SetTitle("lumi section block");
  GPedFrac.Draw("ap");
  C.Print("PCC2017PbPb/fit2017G_pedestal_frac_vs_lsblock.gif");


  
}
