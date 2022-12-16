

void plotPCCMinitree(){

  TString outpath="/afs/cern.ch/user/b/benitezj/www/BRIL/";

  //TFile F("/eos/cms/store/group/comm_luminosity/PCC/ForLumiComputation/2017/VdMFills/6016/merged.root");
  TFile F("/afs/cern.ch/user/b/benitezj/output/BRIL/CMSSW_10_2_2/src/pcc_Data_PixVtx_Event_92X_11_.root");
  TTree*t=(TTree*)F.Get("pccminitree");
  if(!t) return;

  TCanvas C;

  long bcid=41;
  TString BXidSelection=TString("(BXid==")+bcid+")";
  long T1=1501229350;
  long T2=1501229550;
  TString TimeSelection=TString("(timeStamp>")+T1+"&&timeStamp<"+T2+")";
  

  C.Clear();
  t->Draw("timeStamp",BXidSelection+"&&"+TimeSelection);
  C.Print(outpath+"plotPCCMinitree_timeStamp.png");
  

  TH1F HnCluster("HnCluster","nCluster",100,0,500);
  HnCluster.GetXaxis()->SetTitle("PCC");
  C.Clear();
  C.SetLogy(1);
  t->Draw("nCluster>>HnCluster",BXidSelection+"&&"+TimeSelection);
  C.Print(outpath+"plotPCCMinitree_nCluster.png");
  
  TH2F HnClusterVsTime("HnClusterVsTime","",20,0,T2-T1,100,-0.5,499.5);
  HnClusterVsTime.GetXaxis()->SetTitle("time (s)");
  HnClusterVsTime.GetYaxis()->SetTitle("PCC");
  C.Clear();
  C.SetLogy(0);
  t->Draw(TString("nCluster:timeStamp-")+T1+">>HnClusterVsTime",BXidSelection+"&&"+TimeSelection);
  C.Print(outpath+"plotPCCMinitree_nClusterVsTime.png");

  TProfile* P=HnClusterVsTime.ProfileX();
  P->GetYaxis()->SetRangeUser(0,50);
  P->GetXaxis()->SetTitle("time (s)");
  P->GetYaxis()->SetTitle("Avg. PCC");
  C.Clear();
  C.SetLogy(0);
  P->Draw();
  C.Print(outpath+"plotPCCMinitree_nClusterVsTimeProf.png");

}
