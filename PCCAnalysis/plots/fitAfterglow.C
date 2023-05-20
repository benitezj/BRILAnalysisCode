//*//
// This code fits a single bunch tail.
//*//

int run=366800;
TString LSBlockName="RawLumiAvg_366800_2_3_3";
int firstb=2936; // trains in this data: [1113,1148], [1575,1610], [2007,2042], [2901,2936]
int nbins=50; //number of bins to fit


TString formula="[0]*((x==0.0) + [1]*(x==1.0) + [2]*exp(-[3]*(x-1))*(x>0.5))";


void fitAfterglow(TString inpath="."){

  TFile File(inpath+"/"+run+".root","read");
  TH1F* H=(TH1F*)File.Get(LSBlockName);
  if(!H){
    cout<<"Histogram not found"<<endl;
    return;
  }

  ///create the histogram for the fit and copy the data 
  TH1F HSel("HSel","",nbins,-0.5,(nbins-1)+0.5);
  for(int b=0;b<nbins;b++){
    HSel.SetBinContent(b+1,H->GetBinContent(firstb+b));
    HSel.SetBinError(b+1,H->GetBinError(firstb+b));
  }

  /////fit function and parameter limits
  TF1 Fit("Fit",formula,-0.5,(nbins-1)+0.5);//domain is x:[0,nbins-1]
  Fit.FixParameter(0,HSel.GetBinContent(1));
  //Fit.FixParameter(1,0.025);
  Fit.SetParLimits(1,0,0.2);
  //Fit.FixParameter(2,0.01);
  Fit.SetParLimits(2,0,0.5);
  Fit.SetParLimits(3,0,1000);
  HSel.Fit(&Fit); //simple ROOT fit

  ///Histograms containing the fit result and residuals
  TH1F HFit("HFit","",nbins,-0.5,(nbins-1)+0.5);
  TH1F HFitRes("HFitRes","",nbins,-0.5,(nbins-1)+0.5);
  for(int b=0;b<nbins;b++){
    float x=HSel.GetBinCenter(b+1);
    float y=Fit.Eval(x);
    HFit.SetBinContent(b+1,y);
    HFitRes.SetBinContent(b+1,(HSel.GetBinContent(b+1)-y)/y);
    HFitRes.SetBinError(b+1,HSel.GetBinError(b+1)/y);
  }


  
  TCanvas C;
  C.SetLogy(1);

  /// plot with the input data full orbit
  C.Clear();
  H->SetStats(0);
  H->GetYaxis()->SetTitle("Raw PCC");
  H->GetXaxis()->SetTitle("bcid");
  H->Draw("hist");
  C.Print("fitAfterglow_inputdata.png");


  // plot with the fitted data and fit results
  C.Clear();
  HSel.SetStats(0);
  HSel.SetMarkerStyle(8);
  HSel.SetMarkerSize(1);
  HSel.GetYaxis()->SetTitle("Raw PCC");
  HSel.GetXaxis()->SetTitle("bcid");
  HSel.Draw("histpe");
  HFit.SetLineStyle(1);
  HFit.SetLineColor(2);
  HFit.Draw("histsame");

  TLatex text;
  text.SetTextColor(2);
  text.SetTextSize(0.04);

  text.DrawLatexNDC(0.1,0.93,TString("Data: ")+ LSBlockName);
  text.DrawLatexNDC(0.7,0.93,TString("First bin: ")+firstb);  

  ///Model parameters
  formula.ReplaceAll("[0]","N");
  formula.ReplaceAll("[1]","A");
  formula.ReplaceAll("[2]","B");
  formula.ReplaceAll("[3]","C");
  text.DrawLatexNDC(0.2,0.8,TString("F(x) = ")+formula);
  char s[100];
  snprintf(s,10,"N=%d",int(Fit.GetParameter(0)));
  text.DrawLatexNDC(0.22,0.75,s);
  snprintf(s,10,"A=%0.4f",float(Fit.GetParameter(1)));
  text.DrawLatexNDC(0.36,0.75,s);
  snprintf(s,10,"B=%0.4f",float(Fit.GetParameter(2)));
  text.DrawLatexNDC(0.5,0.75,s);
  snprintf(s,10,"C=%0.4f",float(Fit.GetParameter(3)));
  text.DrawLatexNDC(0.64,0.75,s);
  C.Print("fitAfterglow.png");

  //plot with the Fit Residuals
  C.Clear();
  C.SetLogy(0);
  HFitRes.SetStats(0);
  HFitRes.SetMarkerStyle(8);
  HFitRes.SetMarkerSize(1);
  HFitRes.GetXaxis()->SetTitle("bcid");
  HFitRes.GetYaxis()->SetTitle("(Data - Fit) / Fit");
  HFitRes.GetYaxis()->SetRangeUser(-0.4,0.4);
  HFitRes.Draw("histpe");
  TLine line;
  line.DrawLine(-0.5,0,(nbins-1)+0.5,0);
  C.Print("fitAfterglow_residuals.png");
  
  

}
