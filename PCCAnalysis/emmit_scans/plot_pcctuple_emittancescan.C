#define NBCID 3564 

void plot_pcctuple_emittancescan(TString path){

  //TFile F("pcctuple_392538.root");
  TFile F(path.Data());

  TH1F HBCID("HBCID","",NBCID+1,-0.5,NBCID+0.5);
 
  
  TTree * T = (TTree*)F.Get("PCCEventToTuple/tree");
  if(!T){ cout<<"no Tree found"<<endl; return;}
  
  TCanvas C("C","",700,600);

  T->Draw("bunchCrossing>>HBCID");
  TString bcidlist="";
  for(long i=1;i<=NBCID;i++){
    if(HBCID.GetBinContent(i)>10000)
      bcidlist += TString(",")+(long)HBCID.GetBinCenter(i);
  }
  HBCID.SetTitle(bcidlist);
  C.Clear();
  HBCID.Draw("hist");
  HBCID.GetXaxis()->SetTitle("BCID");
  HBCID.GetYaxis()->SetTitle("Events");
  C.SetLogy(1);
  C.Print("bcid.png");
  C.SetLogy(0);
  
  C.Clear();
  T->Draw("pcc:LS");
  C.Print("pcc_vs_LS.png");

  C.Clear();
  T->Draw("pcc:timeStamp_begin");
  C.Print("pcc_vs_time.png");
    
    
}
