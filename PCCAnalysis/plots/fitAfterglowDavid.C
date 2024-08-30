//#include "globals.h"

TString outputpath="./tmp";
TString inpath="./AfterglowModel_2022Data_SummerVeto/data_v2/Run2022F/362154.root" ;
TString histoname="RawLumiAvg_362154_15_738_787";

int FIRSTBCID=45;
int LASTBCID=75;
std::vector<int> CollidingBCIDList={55};

bool makePlots=1;
float yRangeMin=5;

float FIXPARAMT1F=0.0279; //value from individual in run 362154
float FIXPARAMT2A=0;  //value from last train in run 362154; 0.00199 when T1F fixed to above and fitting 3400 - 3500 
float FIXPARAMT2B=0;  //value from last train in run 362154; 0.0062 when T1F fixed to above and fitting 3400 - 3500 


///Parameters used for 2022 reprocessing
//output/BRIL/MoriondReprocessing_2022Data/CMSSW_12_4_12/src/SummerLUMPAS22/Random_v4/Run2022F/cfg.py
//        type2_a= cms.double(0.0008406),
//        type2_b= cms.double(0.01047),



void fitAfterglowTrain(TH1F* H, int firstbcid, int lastbcid){

  if(!H){
    cout<<"Histogram not found"<<endl;
    return;
  }

  int nbins=lastbcid-firstbcid+1;
  
  ///create the histogram for the fit and copy the data 
  TH1F HFitHisto("HFitHisto","",nbins,-0.5,nbins-0.5);
  for(int b=0;b<nbins;b++){
    HFitHisto.SetBinContent(b+1,H->GetBinContent(firstbcid+b));
    HFitHisto.SetBinError(b+1,H->GetBinError(firstbcid+b));
  }

  //TH1F HCollidingHisto("HCollidingHisto","",nbins,-0.5,nbins-0.5);

  TString AFModel("((x==K0)+[0]*(x==K1)+[1]*exp(-[2]*(x-K1))*(x>=K1))");

  TString formulaTotal;
  for(int k=0;k<CollidingBCIDList.size();k++){
    TString fk=AFModel;
    int K=CollidingBCIDList[k]-firstbcid;
    fk.ReplaceAll("K0",TString("")+K);
    fk.ReplaceAll("K1",TString("")+(K+1));
    formulaTotal+=TString("[")+(k+3)+"]*"+fk;
    if(k<CollidingBCIDList.size()-1) formulaTotal+="+";

    //HCollidingHisto.SetBinContent(K+1,H->GetBinContent(CollidingBCIDList[k]));
  }

  formulaTotal+=TString("+[")+(CollidingBCIDList.size()+3)+"]*exp(-["+(CollidingBCIDList.size()+4)+"]*x)";//background contribution from previous orbit
  //formulaTotal+=TString("+[")+(CollidingBCIDList.size()+3)+"]*exp(-[2]*x)";//background contribution from previous orbit
  cout<<formulaTotal<<endl;
  //return;
  
  
  ///Initialize the fit parameters
  TF1 Fit("Fit",formulaTotal,-0.5,nbins-0.5);
  Fit.SetParLimits(0,0.0001,0.3); //type 1 frac
  if(FIXPARAMT1F!=0)Fit.FixParameter(0,FIXPARAMT1F); 
  Fit.SetParameter(1,0.001); //type2 a
  if(FIXPARAMT2A!=0)Fit.FixParameter(1,FIXPARAMT2A); 
  Fit.SetParameter(2,0.015); //type2 b
  Fit.SetParLimits(2,0.0001,0.5); //type 2 b, restrict range to be positive
  if(FIXPARAMT2B!=0)Fit.FixParameter(2,FIXPARAMT2B);
 
  for(int k=0;k<CollidingBCIDList.size();k++)
    Fit.SetParameter(k+3,HFitHisto.GetBinContent(CollidingBCIDList[k]-firstbcid));
  
  Fit.SetParameter(CollidingBCIDList.size()+3,HFitHisto.GetBinContent(1)); //background contribution from previous orbit
  Fit.SetParLimits(CollidingBCIDList.size()+4,0.00001,0.9); 
 

  
  //Run the fit 
  TVirtualFitter::SetMaxIterations(50000); 
  TFitResultPtr r=HFitHisto.Fit(&Fit,"SQ"); //simple ROOT fit
  cout<<"Chi2/NDF: "<<Fit.GetChisquare()<<"/"<<Fit.GetNDF()<<endl;
  cout<<"CovStatus: "<<r->CovMatrixStatus()<<endl;

  
  ///compute colliding bunch rate average
  float AvgN=0;
  //float AvgN=HFitHisto.GetMaximum();
  //float AvgN=Fit.GetParameter(0);
  for(int k=0;k<CollidingBCIDList.size();k++)
    AvgN+=Fit.GetParameter(k+3);
  AvgN/=CollidingBCIDList.size();
  
  
  //create graphs of the fit and fit residuals
  TH1F HFit("HFit","",nbins,-0.5,nbins-0.5);
  TH1F HFitRes("HFitRes","",nbins,-0.5,nbins-0.5);
  for(int b=1;b<=nbins;b++){
    float x=HFitHisto.GetBinCenter(b);
    float y=Fit.Eval(x);
    HFit.SetBinContent(b,y);
    HFitRes.SetBinContent(b,100*(HFitHisto.GetBinContent(b)-y)/AvgN);
    HFitRes.SetBinError(b,100*HFitHisto.GetBinError(b)/AvgN);
  }


  ///make plots
  if(makePlots){
    TCanvas C("Canvas","",800,600);

    /// plot with the input data full orbit
    C.Clear();
    C.SetLogy(1);
    H->GetYaxis()->SetRangeUser(yRangeMin,1.5*AvgN);
    H->SetStats(0);
    H->GetYaxis()->SetTitle("PCC");
    H->GetXaxis()->SetTitle("BCID");
    H->SetTitle(H->GetName());
    H->Draw("hist");
    C.Print(outputpath+"/fitAfterglowTrain_inputdata.png");


    /////////////
    // plot with the fitted data and fit results
    C.Clear();
    C.SetLogy(1);
    HFitHisto.SetStats(0);
    HFitHisto.SetMarkerStyle(20);
    HFitHisto.SetMarkerSize(0.5);
    HFitHisto.GetYaxis()->SetRangeUser(yRangeMin,1.5*AvgN);
    HFitHisto.GetYaxis()->SetTitle("PCC");
    HFitHisto.GetXaxis()->SetTitle("BCID");
    HFitHisto.Draw("histpe");
//    HCollidingHisto.SetMarkerColor(4);
//    HCollidingHisto.SetMarkerStyle(8);
//    HCollidingHisto.SetMarkerSize(0.5);
//    HCollidingHisto.Draw("histpsame");
    HFit.SetLineStyle(1);
    HFit.SetLineColor(2);
    HFit.Draw("histsame");


    TLegend leg(0.73,0.8,0.88,0.88);
    leg.SetLineStyle(0);
    leg.SetLineColor(0);
    leg.AddEntry(&HFitHisto,"Data","p");
    leg.AddEntry(&HFit,"Fit","l");
    leg.Draw();

    char txt[100];
    TLatex text;
    text.SetTextColor(2);
    text.SetTextSize(0.025);
    snprintf(txt,100,"T1 frac = %.4f +/- %0.4f",Fit.GetParameter(0),Fit.GetParError(0));
    text.DrawLatexNDC(0.68,0.75,txt);
    snprintf(txt,100,"T2 A = %.5f +/- %0.5f",Fit.GetParameter(1),Fit.GetParError(1));
    text.DrawLatexNDC(0.68,0.70,txt);    
    snprintf(txt,100,"T2 B = %.4f +/- %0.4f",Fit.GetParameter(2),Fit.GetParError(2));
    text.DrawLatexNDC(0.68,0.65,txt);

    C.Print(outputpath+"/fitAfterglowTrain_fit.png");

    
    ////////////
    //plot with the Fit Residuals
    C.Clear();
    C.SetLogy(0);
    HFitRes.SetStats(0);
    HFitRes.SetMarkerStyle(8);
    HFitRes.SetMarkerSize(1);
    HFitRes.GetXaxis()->SetTitle("BCID");
    HFitRes.GetYaxis()->SetTitle(" (Data - Fit) / Avg. Colliding Rate   [%]");
    HFitRes.GetYaxis()->SetTitleOffset(1.2);
    HFitRes.GetYaxis()->SetRangeUser(-2,2);
    HFitRes.Draw("histp");
    TLine line;
    line.DrawLine(-0.5,0,(nbins-1)+0.5,0);
    C.Print(outputpath+"/fitAfterglowTrain_residuals.png");

 
  }

  
  
}






//////////////////////////////////////////////
void fitAfterglowDavid(){

  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  gROOT->cd();

  cout<<"Reading: "<<inpath<<endl;
  TFile File((inpath).Data(),"read");
  if(File.IsZombie()){
    cout<<"input file not found"<<endl;
    return;
  }
  
  
  TH1F* H=(TH1F*)File.Get(histoname);
  if(!H){
    cout<<"No input histogram "<<histoname<<endl;
  }


  //Fit the last train
  int FIRSTBCID=3340; int LASTBCID=3500;
  CollidingBCIDList=std::vector<int>{};
  for(int k=3347;k<=3382;k++) CollidingBCIDList.push_back(k);

  
  fitAfterglowTrain(H, FIRSTBCID, LASTBCID);

}
