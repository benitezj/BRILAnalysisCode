#include "globals.h"

TString inpath="./May2022Checks/May2022Data/Random/ALCAPROMPT/Run2022B/";
TString PayloadDataPath("./May2022Checks/May2022Payloads/");
TString outpath("./May2022Checks/");


void comparePCLPayload(long run){

  system(TString("mkdir -p ")+outpath);
  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  gStyle->SetOptStat(111111);

    
  TString OutFileName("comparePCLPayload");

  TString Inputfile=inpath+"/"+run+".root";
  cout<<"Processing :"<<Inputfile<<endl;  

  TCanvas C("C","",1200,900);

  C.Print(outpath+"/"+OutFileName+"_"+run+".pdf[");

  
  //iterate over the TFile keys
  TFile FCORR(Inputfile.Data(),"read");
  gROOT->cd();
  TIter next(FCORR.GetListOfKeys());
  TObject* key;
  while ((key = next())) {
    if(!key) continue;

    TString kname(key->GetName());
    if(!kname.Contains("RawLumiAvg")) continue;

    cout<<"RawLumi: "<<kname<<endl;
    
    TObjArray * a = kname.Tokenize("_");
    long r=atoi(((TObjString*)(*a)[1])->GetName());
    long l=atoi(((TObjString*)(*a)[2])->GetName());
    long ls1=atoi(((TObjString*)(*a)[3])->GetName());
    long ls2=atoi(((TObjString*)(*a)[4])->GetName());
    
    if(r!=run) continue;
 

    
    TH1F* HSCALE = (TH1F*) FCORR.Get(TString("ScaleFactorsAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    if(!HSCALE){
      cout<<"Histo not found: "<<TString("ScaleFactorsAvg_")+r+"_"+l+"_"+ls1+"_"+ls2<<endl;
      continue;
    }
    cout<<"ScaleFactors: "<<kname<<endl;
    
  
    //for(int j=0;j<HSCALE->GetNbinsX();j++){
    //   cout<<j<<" - "<<HSCALE->GetBinContent(j+1)<<endl;
    //}
    

    
    //read the payload data
    TH1F HPAYLOAD(TString("HPAYLOAD")+r+"_"+ls1,"",HSCALE->GetNbinsX(),HSCALE->GetXaxis()->GetXmin(),HSCALE->GetXaxis()->GetXmax());
    TH1F HRatio(TString("HRatio")+r+"_"+ls1,"",HSCALE->GetNbinsX(),HSCALE->GetXaxis()->GetXmin(),HSCALE->GetXaxis()->GetXmax());
    TH1F HRatioProj(TString("HRatioProj")+r+"_"+ls1,"",1001,0.99,1.01);

    
    ifstream plfile;
    TString plfilename=PayloadDataPath+"/LumiCorrections_"+r+"_"+ls1+".payload";
    plfile.open(plfilename.Data());
    if(!plfile.is_open()){
      cout<<"Payload data not open: r="<<r<<" ls="<<ls1<<endl;
      return;
    }
    cout<<"Reading payload: "<<plfilename<<endl;
    
    std::string line;
    int PLrun=0;
    int PLls=0;
    float factor;
    while (std::getline(plfile, line)){
      std::stringstream iss(line);
      std::string token;
      std::getline(iss,token, ',');
      std::stringstream rvalue(token);
      rvalue>>PLrun;
      std::getline(iss,token, ',');
      std::stringstream lvalue(token);
      lvalue>>PLls;
      
      if(run!=PLrun || ls1!=PLls){
	cout<<" run or ls does not match: "<<run<<"!="<<PLrun<<" "<<ls1<<"!="<<PLls<<endl;
      }
      
      for(int j=0;j<NBX;j++){
	std::getline(iss,token, ',');
	std::stringstream fvalue(token);
	fvalue>>factor;
	HPAYLOAD.SetBinContent(j+1,factor);

	float scalefactor=HSCALE->GetBinContent(j);
	if(scalefactor>0.8){//select colliding bunches
	  float ratio=0.;
	  if(scalefactor!=0.)                    ratio=factor/scalefactor;
	  else if(factor==0. && scalefactor==0.) ratio=1.;
	  else                                   ratio=0.;

	  HRatio.SetBinContent(j+1,ratio);
	  HRatioProj.Fill(ratio);

	  if(fabs(ratio-1)>0.01) cout<<"Factors don't match, run="<<run<<" ls="<<ls1<<" bx="<<j+1<<" :  ratio ="<<ratio<<"  payload="<<factor<<"  private="<<scalefactor<<endl;
	}

      }
      
    }
    plfile.close();

    
    C.Clear();
    C.Divide(1,3);
    
    C.cd(1);
    HPAYLOAD.SetTitle(TString("Run: ")+run+" , LS: "+ls1+" - "+ls2);
    HPAYLOAD.GetYaxis()->SetTitle("PCL Payload");
    HPAYLOAD.GetYaxis()->SetNdivisions(4);
    HPAYLOAD.GetXaxis()->SetTitle("bcid");
    HPAYLOAD.SetMarkerColor(4);
    HPAYLOAD.GetYaxis()->SetRangeUser(-0.05,1.1);
    HPAYLOAD.Draw("histp");

    C.cd(2);
    HSCALE->GetYaxis()->SetRangeUser(-0.05,1.1);
    HSCALE->SetTitle(TString("Run: ")+run+" , LS: "+ls1+" - "+ls2);
    HSCALE->GetYaxis()->SetTitle("Private Payload");
    HSCALE->GetYaxis()->SetNdivisions(4);
    HSCALE->GetXaxis()->SetTitle("bcid");
    HSCALE->SetMarkerColor(4);
    HSCALE->Draw("histpsame");

//    HRatio.SetMarkerColor(2);
//    HRatio.GetYaxis()->SetNdivisions(4);
//    HRatio.GetYaxis()->SetTitle("ratio");
//    HRatio.GetYaxis()->SetRangeUser(-0.05,1.1);    
//    HRatio.Draw("histp");

    C.cd(3);
    HRatioProj.SetTitle(" colliding bunches");
    HRatioProj.GetXaxis()->SetTitle(" PCL / private ");
    HRatioProj.GetYaxis()->SetTitle(" # of bcids ");
    HRatioProj.GetYaxis()->SetNdivisions(4);
    HRatioProj.Draw("hist");
    gPad->Update();
    TPaveStats *st = (TPaveStats*)HRatioProj.FindObject("stats");
    st->SetY1NDC(0.4);
    C.Print(outpath+"/"+OutFileName+"_"+run+".pdf");
  }

  C.Print(outpath+"/"+OutFileName+"_"+run+".pdf]");

  
  gROOT->ProcessLine(".q");
}
