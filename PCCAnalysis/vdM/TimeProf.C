#include <iostream>
#include <fstream>
using namespace std;

//--------------------------------------------------------------------------//
void TimeProf(){
  TFile F ("/eos/user/j/jmorenoc/BRIL/PCC/VDM/vdMMiniTreesJobs/vdMMT_2018_NEW_Veto2_FS/ZeroBias_NV2_FS.root");

  gROOT->cd();
  
  F.ls();
  TTree  *t = (TTree*) F.Get("pccminitree");


  TCanvas C("C","",1000,600);

  //test

  TH2F H("H","",32, 1530411064, 1530412028 ,7000,0,7000);  // clusters vs time

  //68343
  //4556.2
  //680
  H.GetXaxis()->SetTitle("Time (s)");
  H.GetYaxis()->SetTitle("PCC per event");
 
  t->Draw("nCluster:timeStamp>>H"); // llenar el histo

  // C.Clear(); 
  //H.SetStats(0); //remove stat box
  //H.Draw("colz");

  //C.Print("plots/pcc-timestamp_vdM1_YScan.png"); //CHANGE NAME*************

  //Profile

  TProfile * P =(TProfile*) H.ProfileX();  // sacar el profile en el tiempo

  P->SetStats(0); //remove stat box

  C.Clear(); 
  P->GetYaxis()->SetTitle("Average PCC per Event");
  P->Draw();

  C.Print("~/public/PlotsMacrotest.png");  //CHANGE NAME******
 
}
