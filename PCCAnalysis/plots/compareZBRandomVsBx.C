#include "globals.h"

TH1F * readCSV(TString CSV,int ls1, int ls2){

  TH1F*HCSV = new TH1F(TString("HCSV_")+(long)ls1,"",NBX,0.5,NBX+0.5);

  ifstream csvfile(CSV.Data());
  if (!csvfile.is_open()){
    std::cout << "Unable to open csvfile "<<CSV.Data()<<std::endl;
    return 0;
  }

  std::string line;
  int run=0;
  int ls=0;
  float Lumi = 0.;
  while (std::getline(csvfile,line)){
    std::stringstream iss(line);
    std::string token;
    std::getline(iss,token, ',');
    std::stringstream runiss(token);
    runiss>>run;
    std::getline(iss,token, ',');
    std::stringstream lsiss(token);
    lsiss>>ls;
    std::getline(iss,token, ',');
    std::stringstream lumiiss(token);
    lumiiss>>Lumi;

    if(ls<ls1 || ls>ls2) continue;

    //cout<<run<<" "<<ls<<" "<<Lumi<<": "<<endl;

    for(int b=1;b<=NBX;b++){
      std::getline(iss,token, ',');
      std::stringstream biss(token);
      biss>>Lumi;
      HCSV->AddBinContent(b,Lumi);
    }

  }
  csvfile.close();



  return HCSV;
}

void compareZBRandomVsBx(){
  TString CSV="/eos/user/b/benitezj/BRIL/PCC/ZeroBias/AlCaLumiPixels_ZeroBias-PromptReco/Run2017G_v4_test/306550.csv";
  TString CORR="/eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-PromptReco/Run2017G_v4/306550.root";

  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");
  TCanvas C("C","",1200,500);

  //iterate over the TFile keys
  TFile FCORR(CORR.Data(),"read");
  TIter next(FCORR.GetListOfKeys());
  TObject* key;
  while ((key = next())) {
    TString kname(key->GetName());
    if(!kname.Contains("RawLumiAvg")) continue;
    TObjArray * a = kname.Tokenize("_");
    long r=atoi(((TObjString*)(*a)[1])->GetName());
    long l=atoi(((TObjString*)(*a)[2])->GetName());
    long ls1=atoi(((TObjString*)(*a)[3])->GetName());
    long ls2=atoi(((TObjString*)(*a)[4])->GetName());
    TString name=TString("RawLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2;

    TH1F* HCORR = (TH1F*) FCORR.Get(name);
    if(!HCORR){
      cout<<" not found "<<name<<endl;
      continue;
    }
    HCORR->Scale(1./HCORR->GetMaximum());
    
    TH1F* HCSV=readCSV(CSV,ls1,ls2);
    HCSV->Scale(1./HCSV->GetMaximum());
  
    C.Clear();
    HCORR->SetTitle(TString("LS: ")+ls1+" - "+ls2);
    HCORR->GetYaxis()->SetTitle("counts");
    HCORR->GetXaxis()->SetTitle("bcid");
    HCORR->SetLineWidth(2);
    HCORR->GetYaxis()->SetRangeUser(0,1.1*HCORR->GetMaximum());
    HCORR->Draw("hist");
    HCSV->SetMarkerStyle(8);
    HCSV->SetMarkerSize(1);
    HCSV->Draw("histpsame");
    C.Print(TString("compareZBRandomVsBx_")+l+".png");
    
    l++;
  }


  gROOT->ProcessLine(".q");
}
