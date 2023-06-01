//*//
// This code fits one bunch train and the tail
//*//


bool makePlots=1;


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

void makeTree(){
  Tree=new TTree("Tree","Afterglow Fit results");
  Tree->Branch("chi2",&fit_chi2);
  Tree->Branch("status",&fit_status);
  Tree->Branch("Type1f",&fit_Type1f);
  Tree->Branch("Type2a",&fit_Type2a);
  Tree->Branch("Type2b",&fit_Type2b);
  Tree->Branch("AvgN",&fit_AvgN);
  Tree->Branch("firstb",&fit_firstb);
  Tree->Branch("ncoll",&fit_ncoll);
  Tree->Branch("nbins",&fit_nbins);
  Tree->Branch("run",&fit_run);
  Tree->Branch("ls",&fit_ls);
  Tree->Branch("lsblock",&fit_lsblock);
}

//void fitAfterglowTrain(TString inputfile=InputFile, TString lsblockname=LSBlockName, int firstb=FirstBin, int ncolliding=NColliding, int nbins=NBins){

void fitAfterglowTrain(TH1F* H, TString lsblockname, int firstb, int ncolliding, int nbins){

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
  
  /////fit function and parameter limits
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

    //if(i==0) Fit.SetParLimits(0,1,1e6);
    //else Fit.SetParLimits(i+3,1,1e6);
    
    if(i==0) Fit.SetParameter(0,HSel.GetBinContent(i+1));
    else Fit.SetParameter(3+i,HSel.GetBinContent(i+1));

  }

  //Fit.FixParameter(1,0.025);
  Fit.SetParLimits(1,0.0001,0.3);  //type 1 frac
  Fit.SetParameter(1,0.025);
  
  Fit.SetParameter(2,0.001);
  //Fit.FixParameter(2,0.02);
  //Fit.SetParLimits(2,0.00001,0.2); // type 2 a

  Fit.SetParameter(3,0.015);
  //Fit.FixParameter(3,0.017);
  Fit.SetParLimits(3,0.0001,0.5); // type 2 b

  
  TVirtualFitter::SetMaxIterations(50000); 
  TFitResultPtr r=HSel.Fit(&Fit,"SQ"); //simple ROOT fit
  
  cout<<"Chi2/NDF: "<<Fit.GetChisquare()<<"/"<<Fit.GetNDF()<<endl;
  cout<<"CovStatus: "<<r->CovMatrixStatus()<<endl;

  ///compute average of the colliding bunches
  float AvgN=0.;
  for(int i=0;i<ncolliding;i++){
    if(i==0) AvgN+=Fit.GetParameter(0);
    else AvgN+=Fit.GetParameter(i+3);
  }
  AvgN/=ncolliding;

  
  //fill TTree
  fit_chi2=Fit.GetChisquare()/Fit.GetNDF();
  fit_status=r->CovMatrixStatus();
  fit_AvgN=AvgN;
  fit_ncoll=ncolliding;
  fit_firstb=firstb;
  fit_nbins=nbins;
  fit_Type1f=Fit.GetParameter(1);
  fit_Type2a=Fit.GetParameter(2);
  fit_Type2b=Fit.GetParameter(3);
  if(Tree)Tree->Fill();


 
  if(makePlots){

    ///Histograms containing the fit result and residuals
    TH1F HFit("HFit","",nbins,-0.5,(nbins-1)+0.5);
    TH1F HFitRes("HFitRes","",nbins,-0.5,(nbins-1)+0.5);
    for(int b=0;b<nbins;b++){
      float x=HSel.GetBinCenter(b+1);
      float y=Fit.Eval(x);
      HFit.SetBinContent(b+1,y);
      HFitRes.SetBinContent(b+1,100*(HSel.GetBinContent(b+1)-y)/AvgN);
      HFitRes.SetBinError(b+1,100*HSel.GetBinError(b+1)/AvgN);
    }


    TCanvas C;

    /// plot with the input data full orbit
    C.Clear();
    C.SetLogy(1);
    H->SetStats(0);
    H->GetYaxis()->SetTitle("Raw PCC");
    H->GetXaxis()->SetTitle("bcid");
    H->Draw("hist");
    C.Print(TString("fitAfterglowTrain_inputdata-")+lsblockname+".png");


    /////////////
    // plot with the fitted data and fit results
    C.Clear();
    C.SetLogy(1);
    HSel.SetStats(0);
    HSel.SetMarkerStyle(8);
    HSel.SetMarkerSize(0.5);
    HSel.GetYaxis()->SetRangeUser(1,9*AvgN);
    HSel.GetYaxis()->SetTitle("Raw PCC");
    HSel.GetXaxis()->SetTitle("bcid");
    HSel.Draw("histpe");
    HFit.SetLineStyle(1);
    HFit.SetLineColor(2);
    HFit.Draw("histsame");

    TLatex text;
    text.SetTextColor(2);
    text.SetTextSize(0.035);

    text.DrawLatexNDC(0.1,0.93,TString("Data: ")+ lsblockname);
    text.DrawLatexNDC(0.55,0.93,TString("First bcid: ")+firstb);
    text.DrawLatexNDC(0.75,0.93,TString("Ncolliding: ")+ncolliding);  

    ///print formula
    formula.ReplaceAll("[0]","N_{k}");
    formula.ReplaceAll("[1]","f");
    formula.ReplaceAll("[2]","A");
    formula.ReplaceAll("[3]","B");
    text.DrawLatexNDC(0.2,0.85,TString("F_{k}(x) = ")+formula);

    //print param values
    char s[100];
    snprintf(s,40,"f=%0.5f (Type1)",float(Fit.GetParameter(1)));
    text.DrawLatexNDC(0.2,0.80,s);
    snprintf(s,40,"A=%0.7f (Type2)",float(Fit.GetParameter(2)));
    text.DrawLatexNDC(0.45,0.80,s);
    snprintf(s,40,"B=%0.5f (Type2)",float(Fit.GetParameter(3)));
    text.DrawLatexNDC(0.45,0.75,s);

    //print fit status
    snprintf(s,40,"Chi2/NDF=%.1f/%d",Fit.GetChisquare(),Fit.GetNDF());
    text.DrawLatexNDC(0.7,0.80,s);
    snprintf(s,40,"CovStatus=%d",r->CovMatrixStatus());
    text.DrawLatexNDC(0.7,0.75,s);
  

    C.Print(TString("fitAfterglowTrain_fit-")+lsblockname+"-"+firstb+".png");

    ////////////
    //plot with the Fit Residuals
    C.Clear();
    C.SetLogy(0);
    HFitRes.SetStats(0);
    HFitRes.SetMarkerStyle(8);
    HFitRes.SetMarkerSize(1);
    HFitRes.GetXaxis()->SetTitle("bcid");
    HFitRes.GetYaxis()->SetTitle("100 * (Data - Fit) / <N>   [%]");
    HFitRes.GetYaxis()->SetTitleOffset(1.2);
    HFitRes.GetYaxis()->SetRangeUser(-1,1);
    HFitRes.Draw("histpe");
    TLine line;
    line.DrawLine(-0.5,0,(nbins-1)+0.5,0);
    C.Print(TString("fitAfterglowTrain_residuals-")+lsblockname+"-"+firstb+".png");
 
  }


  
}
