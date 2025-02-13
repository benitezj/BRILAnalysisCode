

void fitAfterglowPlots(TString inputfile=""){
  if(inputfile.CompareTo("")==0) return;

  TFile InputFile(inputfile.Data(),"read");
  
  TTree* T= (TTree*)InputFile.Get("Tree");
  if(!T) return;

  int nbinsX=7; int xmin=0, xmax=52;
  int nbinsY=100;
  float yminA=5e-4, ymaxA=20e-4;
  float yminB=5e-3, ymaxB=15e-3;
  float yminF=0, ymaxF=0.06;
  
  TCanvas C;
  
  TH2F hT2a("hT2a","",nbinsX,xmin,xmax,nbinsY,yminA,ymaxA);
  hT2a.SetMarkerStyle(8);
  hT2a.SetMarkerSize(0.8);
  hT2a.GetYaxis()->SetTitle("Type 2 A parameter");
  hT2a.GetXaxis()->SetTitle("50 LS block id");
  T->Draw("Type2a:lsblock>>hT2a");
  C.Clear();
  hT2a.Draw("scat");
  C.Print("Type2a.png");


  TH2F hT2b("hT2b","",nbinsX,xmin,xmax,nbinsY,yminB,ymaxB);
  hT2b.SetMarkerStyle(8);
  hT2b.SetMarkerSize(0.8);
  hT2b.GetYaxis()->SetTitle("Type 2 B parameter");
  hT2b.GetXaxis()->SetTitle("50 LS block id");
  T->Draw("Type2b:lsblock>>hT2b");
  C.Clear();
  hT2b.Draw("scat");
  C.Print("Type2b.png");


  TH2F hT1f("hT1f","",nbinsX,xmin,xmax,nbinsY,yminF,ymaxF);
  hT1f.SetMarkerStyle(8);
  hT1f.SetMarkerSize(0.8);
  hT1f.GetYaxis()->SetTitle("Type 1 fraction");
  hT1f.GetXaxis()->SetTitle("50 LS block id");
  T->Draw("Type1f:lsblock>>hT1f");
  C.Clear();
  hT1f.Draw("scat");
  C.Print("Type1f.png");
  

}
