#include "globals.h"

TString outputpath="./tmp";

///////////////
//  2018 data
//TString inpath="./AfterglowModel2018"; //2018 data final veto
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={1}, int NCOLLIDINGBCIDS=11, int NTOTALBCIDS=100){//1-wagon
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={188,1070,1964,2858}, int NCOLLIDINGBCIDS=103, int NTOTALBCIDS=200){//2-wagons
//std::vector<int> RunList={320995}; std::vector<int> LeadBCIDList={188}; int NCOLLIDINGBCIDS=103; int NTOTALBCIDS=200;
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={454,1336,2230,3124}, int NCOLLIDINGBCIDS=158, int NTOTALBCIDS=240){//3-wagons
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={1336}, int NCOLLIDINGBCIDS=158, int NTOTALBCIDS=240){//3-wagons
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995}, std::vector<int> LeadBCIDList={3124}, int NCOLLIDINGBCIDS=158, int NTOTALBCIDS=240){

//std::vector<int> RunList={320995}; std::vector<int> LeadBCIDList={188+55}; int NCOLLIDINGBCIDS=48; int NTOTALBCIDS=148;//shows fit to only one wagon
//std::vector<int> RunList={320995}; std::vector<int> LeadBCIDList={188}; int NCOLLIDINGBCIDS=103; int NTOTALBCIDS=250; //2-wagons -> thesis

///////////////
///    2022 Data
//TString inpath="./AfterglowModel_2022Data_SummerVeto/data_v2/Run2022F" ; //2022 final veto
//std::vector<int> RunList={361948}; std::vector<int> LeadBCIDList={1018,2806}; int NCOLLIDINGBCIDS=189; int NTOTALBCIDS=250; //600b fill 8383 -> few blocks
//std::vector<int> RunList={360991}; std::vector<int> LeadBCIDList={1018,2806}; int NCOLLIDINGBCIDS=189; int NTOTALBCIDS=250; //600b Fill 8307  
//std::vector<int> RunList={361957}; std::vector<int> LeadBCIDList={66,2748}; int NCOLLIDINGBCIDS=208; int NTOTALBCIDS=270;//1800b fill 8385 -> mu scan fill, fit smooth part
//std::vector<int> RunList={360991}; std::vector<int> LeadBCIDList={1018+153}; int NCOLLIDINGBCIDS=36; int NTOTALBCIDS=100; //Thesis plot Fill 8307
//std::vector<int> RunList={360991}; std::vector<int> LeadBCIDList={1018}; int NCOLLIDINGBCIDS=189; int NTOTALBCIDS=300; //Thesis plot Fill 8307
//std::vector<int> RunList={360991}; std::vector<int> LeadBCIDList={1018}; int NCOLLIDINGBCIDS=79; int NTOTALBCIDS=110; //Thesis plot Fill 8307


////////////
/// 2017 ReReco
//TString inpath="./2017ReReco/Afterglow/Random_v6/Run2017B" ;
//TString inpath="./2017Afterglow_28Aug24_UL2017_PCCZeroBias_vetoModules_2017_fixed_W0_FPIXOnly_SSBkg25/Run2017B"  ;//FPIX veto
//std::vector<int> RunList={297219}; std::vector<int> LeadBCIDList={566,1448,2342,3236}; int NCOLLIDINGBCIDS=48; int NTOTALBCIDS=150; //https://cmsoms.cern.ch/cms/fills/report?cms_fill=5849
//std::vector<int> RunList={297219}; std::vector<int> LeadBCIDList={72}; int NCOLLIDINGBCIDS=542; int NTOTALBCIDS=740; //https://cmsoms.cern.ch/cms/fills/report?cms_fill=5849
//std::vector<int>   RunList={297219}; std::vector<int> LeadBCIDList={72}; int NCOLLIDINGBCIDS=160; int NTOTALBCIDS=190; //https://cmsoms.cern.ch/cms/fills/report?cms_fill=5849

/// 2017 ReReco Low PU
TString inpath="/Users/josebenitez/Documents/SONORA/Talks/PCC-Studies/2017CapsigmaIssue/AfterglowModel_LowPu/vetoModules_2017_fixed_W0_FPIXD05_Bkg25_lowPU_W0_Stab0p02"  ;
std::vector<int>   RunList={307076}; std::vector<int> LeadBCIDList={2}; int NCOLLIDINGBCIDS=10; int NTOTALBCIDS=40; //https://cmsoms.cern.ch/cms/fills/report?cms_fill=5849





////////////////////////////////////////////////////////////////
//*//
// This code fits one bunch train and the tail
//*//
bool makePlots=1;
float yRangeMin=0.05;


TTree* Tree=NULL;//This tree will be created in the function that calls this fitAfterglowTrain (in a different file).
float fit_chi2;
float fit_status;
float fit_Type1f;
float fit_Type2a;
float fit_Type2b;
float fit_AvgN;

int   fit_firstb;
int   fit_ncoll;
int   fit_nbins;
int   fit_run=0;
int   fit_ls;
int   fit_lsblock=0; int lastrun=0; int lastlsblock=0; int lastlsblockmax=0;

float fit_residNp1;
float fit_residNp2;
float fit_residNp3;
float fit_residNp4;
float fit_Type1f2;


void makeTree(){
  Tree=new TTree("Tree","Afterglow Fit results");
  Tree->Branch("chi2",&fit_chi2);
  Tree->Branch("status",&fit_status);
  Tree->Branch("Type1f",&fit_Type1f);
  Tree->Branch("Type2a",&fit_Type2a);
  Tree->Branch("Type2b",&fit_Type2b);
  Tree->Branch("AvgN",&fit_AvgN);
  Tree->Branch("residNp1",&fit_residNp1);
  Tree->Branch("residNp2",&fit_residNp2);
  Tree->Branch("residNp3",&fit_residNp3);
  Tree->Branch("residNp4",&fit_residNp4);
  Tree->Branch("Type1f2",&fit_Type1f2);
  Tree->Branch("firstb",&fit_firstb);
  Tree->Branch("ncoll",&fit_ncoll);
  Tree->Branch("nbins",&fit_nbins);
  Tree->Branch("run",&fit_run);
  Tree->Branch("ls",&fit_ls);
  Tree->Branch("lsblock",&fit_lsblock);
}


void fitAfterglowTrain(TH1F* H, TString lsblockname, int firstb, int ncolliding, int nbins, TString outpath="."){

  if(!H){
    cout<<"Histogram not found"<<endl;
    return;
  }

  TObjArray * a = lsblockname.Tokenize("_");
  

  
  if(atoi(((TObjString*)(*a)[2])->GetName())!=lastlsblock){
    fit_lsblock++;
    lastlsblock=atoi(((TObjString*)(*a)[2])->GetName());
  } 
  
  
  fit_run=atoi(((TObjString*)(*a)[1])->GetName());
  //fit_lsblock=atoi(((TObjString*)(*a)[2])->GetName());
  fit_ls=atoi(((TObjString*)(*a)[3])->GetName());
  //long ls2=atoi(((TObjString*)(*a)[4])->GetName());


  
  ///create the histogram for the fit and copy the data 
  TH1F HSel("HSel","",nbins,-0.5,(nbins-1)+0.5);
  for(int b=0;b<nbins;b++){
    HSel.SetBinContent(b+1,H->GetBinContent(firstb+b));
    HSel.SetBinError(b+1,H->GetBinError(firstb+b));
  }


  TString formula="[0]*((x-k==0.0)+(x-k==1.0)*[1]+(x-k>0.5)*[2]*exp(-[3]*(x-k-1)))";//base term, will be duplicated below 
  //TString formula="[0]*((x-k==0.0)+(x-k==1.0)*[1]+(x-k==2.0)*[4]+(x-k>0.5)*[2]*exp(-[3]*(x-k-1)))";//added another type1 term, below need to fix the 3 -> 4 
  TString TotalFormula;
  for(int i=0;i<ncolliding;i++){
    TString iterm=formula;
    iterm.ReplaceAll("k",TString("")+i);
    if(i>0)iterm.ReplaceAll("[0]",TString("[")+(3+i)+"]");
    if(i>0)TotalFormula=TotalFormula+"+"+iterm;
    else TotalFormula=iterm;
  }
  //cout<<TotalFormula<<endl;

  
  TF1 Fit("Fit",TotalFormula,-0.5,(nbins-1)+0.5);//domain is x:[0,nbins-1]
  for(int i=0;i<ncolliding;i++){
    if(i==0) Fit.SetParameter(0,HSel.GetBinContent(i+1));
    else Fit.SetParameter(3+i,HSel.GetBinContent(i+1));

  }

  Fit.SetParLimits(1,0,0.2);  //type 1 frac
  Fit.SetParLimits(2,0.0,0.01); //type 2 b, range
  //Fit.SetParLimits(3,0.0001,0.5); //type 2 b, range
  //Fit.SetParameter(2,0.001); //type2 a init
  //Fit.SetParameter(2,0.001); //type2 a init
  //Fit.SetParameter(3,0.015); //type2 b init

  //Fit.FixParameter(2,0.00072); //2017 PAS
  //Fit.FixParameter(3,0.0140); //2017 PAS 

  
  
  TVirtualFitter::SetMaxIterations(50000); 
  TFitResultPtr r=HSel.Fit(&Fit,"SQ"); //simple ROOT fit
  
  cout<<"Chi2/NDF: "<<Fit.GetChisquare()<<"/"<<Fit.GetNDF()<<endl;
  cout<<"CovStatus: "<<r->CovMatrixStatus()<<endl;

  ///compute average of the colliding bunches
  float AvgN=0.;
  for(int i=0;i<ncolliding;i++){
    if(i==0) AvgN+=Fit.GetParameter(0);
    else     AvgN+=Fit.GetParameter(i+3);
  }
  AvgN/=ncolliding;

  
  TH1F HFit("HFit","",nbins,-0.5,(nbins-1)+0.5);
  TH1F HFitRes("HFitRes","",nbins,-0.5,(nbins-1)+0.5);
  ///Histograms containing the fit result and residuals
  for(int b=0;b<nbins;b++){
    float x=HSel.GetBinCenter(b+1);
    float y=Fit.Eval(x);
    HFit.SetBinContent(b+1,y);
    HFitRes.SetBinContent(b+1,100*(HSel.GetBinContent(b+1)-y)/AvgN);
    HFitRes.SetBinError(b+1,100*HSel.GetBinError(b+1)/AvgN);
  }

    
  if(makePlots){
    TCanvas C("Canvas","",800,600);

    /// plot with the input data full orbit
    C.Clear();
    C.SetLogy(1);
    H->GetYaxis()->SetRangeUser(yRangeMin,10*AvgN);
    H->SetStats(0);
    H->SetTitle(lsblockname);
    H->GetYaxis()->SetTitle("Raw PCC");
    H->GetXaxis()->SetTitle("BCID");
    H->Draw("hist");
    //drawCMSPrelim(0.19,0.85,"#font[62]{CMS} #font[52]{Preliminary}");
    //drawFillYear(8307,2022);
    //drawCMSPrelim(0.19,0.85,"#font[62]{CMS} #font[52]{Work in progress}");
    //drawFillYear(7036,2018);
    //drawPCCLuminometer();
    C.Print(outpath+TString("/fitAfterglowTrain_inputdata-")+lsblockname+".png");


    /////////////
    // plot with the fitted data and fit results
    C.Clear();
    C.SetLogy(1);
    HSel.SetStats(0);
    HSel.SetMarkerStyle(8);
    HSel.SetMarkerSize(0.5);
    HSel.GetYaxis()->SetRangeUser(yRangeMin,10*AvgN);
    HSel.SetTitle(lsblockname);
    HSel.GetYaxis()->SetTitle("Raw PCC");
    HSel.GetXaxis()->SetTitle("BCID");
    HSel.Draw("histp");
    HFit.SetLineStyle(1);
    HFit.SetLineColor(2);
    HFit.Draw("histsame");

    TLatex text;
    text.SetTextColor(2);
    text.SetTextSize(0.035);

//   text.DrawLatexNDC(0.1,0.93,TString("Data: ")+ lsblockname);
//   text.DrawLatexNDC(0.55,0.93,TString("First BCID: ")+firstb);
//   text.DrawLatexNDC(0.75,0.93,TString("Ncolliding: ")+ncolliding);  

    /* ///print formula */
    /* formula.ReplaceAll("[0]","N_{k}"); */
    /* formula.ReplaceAll("[1]","f"); */
    /* formula.ReplaceAll("[2]","A"); */
    /* formula.ReplaceAll("[3]","B"); */
    /* text.DrawLatexNDC(0.2,0.85,TString("F_{k}(x) = ")+formula); */

    //print param values
    text.SetTextSize(0.02);
    char s[100];
    snprintf(s,40,"Type 1 f = %0.5f",float(Fit.GetParameter(1)));
    text.DrawLatexNDC(0.5,0.85,s);
    snprintf(s,40,"Type 2 A = %0.7f",float(Fit.GetParameter(2)));
    text.DrawLatexNDC(0.5,0.80,s);
    snprintf(s,40,"Type 2 B = %0.5f",float(Fit.GetParameter(3)));
    text.DrawLatexNDC(0.5,0.75,s);

    //print fit status
    // snprintf(s,40,"Chi2/NDF=%.1f/%d",Fit.GetChisquare(),Fit.GetNDF());
    //text.DrawLatexNDC(0.7,0.85,s);
    //snprintf(s,40,"CovStatus=%d",r->CovMatrixStatus());
    //text.DrawLatexNDC(0.7,0.80,s);

    TLegend leg(0.75,0.75,0.88,0.88);
    leg.SetLineStyle(0);
    leg.SetLineColor(0);
    leg.AddEntry(&HSel,"Data","p");
    leg.AddEntry(&HFit,"Fit","l");
    leg.Draw();

    //drawCMSPrelim(0.19,0.85,"#font[62]{CMS} #font[52]{Preliminary}");
    //drawFillYear(8307,2022);
    //drawCMSPrelim(0.19,0.85,"#font[62]{CMS} #font[52]{Work in progress}");
    //drawFillYear(7036,2018);
    //drawPCCLuminometer();
    C.Print(outpath+TString("/fitAfterglowTrain_fit-")+lsblockname+"-"+firstb+".png");

    ////////////
    //plot with the Fit Residuals
    C.Clear();
    C.SetLogy(0);
    HFitRes.SetStats(0);
    HFitRes.SetMarkerStyle(8);
    HFitRes.SetMarkerSize(1);
    HFitRes.GetXaxis()->SetTitle("BCID");
    HFitRes.GetYaxis()->SetTitle("(Data - Fit) / Avg. Colliding Rate  [%]");
    HFitRes.GetYaxis()->SetTitleOffset(1.2);
    HFitRes.GetYaxis()->SetRangeUser(-1.,1.);
    HFitRes.SetTitle(lsblockname);
    HFitRes.Draw("histp");
    TLine line;
    line.DrawLine(-0.5,0,(nbins-1)+0.5,0);
    C.Print(outpath+TString("/fitAfterglowTrain_residuals-")+lsblockname+"-"+firstb+".png");

 
  }



  //fill TTree
  fit_chi2=Fit.GetChisquare()/Fit.GetNDF();
  fit_status=r->CovMatrixStatus();
  fit_AvgN=AvgN;

  fit_residNp1=HFitRes.GetBinContent(ncolliding+1);
  fit_residNp2=HFitRes.GetBinContent(ncolliding+2);
  fit_residNp3=HFitRes.GetBinContent(ncolliding+3);
  fit_residNp4=HFitRes.GetBinContent(ncolliding+4);
  //fit_Type1f2=Fit.GetParameter(4);
  
  fit_ncoll=ncolliding;
  fit_firstb=firstb;
  fit_nbins=nbins;
  fit_Type1f=Fit.GetParameter(1);
  fit_Type2a=Fit.GetParameter(2);
  fit_Type2b=Fit.GetParameter(3);

  
  if(Tree)Tree->Fill();

  
  
}






//////////////////////////////////////////////
void fitAfterglowRun(){

  if(inpath.CompareTo("")==0) return;

  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  
  TString OutfileName=outputpath+"/fitAfterglow_output.root";
  TFile outputfile(OutfileName,"recreate");
  
  gROOT->cd();
  makeTree();
  

  for(int r=0;r<RunList.size();r++){
  
    TFile File((inpath+"/"+RunList[r]+".root").Data(),"read");
    if(File.IsZombie()){
      cout<<(inpath+"/"+RunList[r]).Data()<<" not found"<<endl;
      return;
    }
  
    TIter next(File.GetListOfKeys());
    while (TObject* key = next()) {
      TString kname(key->GetName());
      if(!kname.Contains("RawLumiAvg")) continue;

      cout<<key->GetName()<<endl;

      TH1F* H=(TH1F*)File.Get(key->GetName());
    
      for(int i=0;i<LeadBCIDList.size();i++)
	fitAfterglowTrain(H,key->GetName(), LeadBCIDList[i], NCOLLIDINGBCIDS, NTOTALBCIDS,outputpath);

      //break;
    }

  }

  
  outputfile.cd();
  if(Tree) Tree->Write();
  outputfile.ls();
  outputfile.Close();
  
}
