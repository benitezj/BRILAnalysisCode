
#include "globals.h"

//Model code :
// https://github.com/cms-sw/cmssw/blob/master/Calibration/LumiAlCaRecoProducers/plugins/CorrPCCProducer.cc
// type2_a_ * exp(-(float(bx) - 1) * type2_b_)
#define formula "[0]*exp(-(x - 1) * [1])"

//params:
// https://github.com/cms-sw/cmssw/blob/master/Calibration/LumiAlCaRecoProducers/python/CorrPCCProducer_cfi.py
// https://github.com/cms-sw/cmssw/blob/master/Calibration/LumiAlCaRecoProducers/python/AlcaLumiPCCHarvester_cff.py
//2018 model params
#define type2_a 0.00072
#define type2_b 0.014

///Type 1 from 2018 root files
// /eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom/Run2018B_dynamicVeto/317626.root
#define Type1Frac 0.024 


void plotAfterglowModel(){

#define modelrange 3600
#define range 25

  TF1 Type2Model("Type2Model",formula,0.5,modelrange+0.5);
  Type2Model.SetParameter(0,type2_a);
  Type2Model.SetParameter(1,type2_b);
  

  TH1F HF("HF","",range,0.5,range+0.5);
  HF.SetTitle("Afterglow Model");
  HF.GetYaxis()->SetTitle("F");
  HF.GetXaxis()->SetTitle("bunch number");
  HF.SetStats(0);
  HF.GetYaxis()->SetRangeUser(0,type2_a);

  TCanvas C;
  C.SetLeftMargin(0.15);

  /////////////////////////////////////////////////
  ///////// Plot of the per bunch Afterglow model
  C.Clear();
  HF.Draw("hist");
  Type2Model.Draw("lsame");
  TLatex text;
  text.DrawLatexNDC(0.3,0.2,TString("F = ")+formula);
  C.Print("plotAfterglowModel.png");


  
  ////////////////////////////////////////////////
  /////// Plot of the sum of 150 equally spaced bunches
///for vdM fill
#define ncolliding 150
#define interval 22

  TH1F HModelSumOrbit("HModelSumOrbit","",range,0.5,range+0.5);
  HModelSumOrbit.SetTitle("Afterglow for vdM fill");
  HModelSumOrbit.GetYaxis()->SetTitle(TString("Model Sum (n=")+ncolliding+", interval="+interval+")");
  HModelSumOrbit.GetXaxis()->SetTitle("bunch number");
  HModelSumOrbit.SetStats(0);

  for(int i=0;i<ncolliding;i++){
    for(int b=1;b<=range;b++)
      HModelSumOrbit.AddBinContent(b,Type2Model.Eval(i*interval+b));
  }
  C.Clear();
  HModelSumOrbit.Draw("histl");
  C.Print("plotAfterglowModelSum.png");  



  
  /////////////////////////////////////////////////////////////
  /////// Analysis of Afterglow uncertainty for 36 bunch trains
  int NBPTrain=36;
  cout<<"Number of bunches per train "<<NBPTrain<<endl;
  int NBPGap=7;
  cout<<"Number of bunches in gap "<<NBPGap<<endl;
  int NBTotal=NBPTrain*2 + NBPGap*2;
  int NTrain=50;         // needed for Type1 uncertainty 
  cout<<"Number of trains/wagons "<<NTrain<<endl;
    
  //1. Afterglow is normalized on the colliding bunches.
  //2. Unc is driven by sqrt(N) on colliding bunches
  //3. nominal rate for Randoms=400hz, nominal PCC per pp=100, integration time, pileup 
  int RDTriggerRate=400;
  cout<<"Total trigger "<<RDTriggerRate<<endl;
  float RDTriggerRatePerB=(float)RDTriggerRate/NBX;//Trigger is divided by full orbit
  cout<<"Trigger per bunch "<<RDTriggerRatePerB<<endl;
  int PCCPerPP=100;
  cout<<"PCC per pp collision "<<PCCPerPP<<endl;
  int NLSIntegration=50;
  float LSTime=23.31*NLSIntegration;  // scale by number of lumisections
  cout<<"Integration time "<<NLSIntegration<<" LS"<<endl;
  int Pileup=40;
  cout<<"Pileup  "<<Pileup<<endl;
 
  
  float collisionCount=PCCPerPP*Pileup*RDTriggerRatePerB*LSTime;
  cout<<"collisionCount= "<<collisionCount<<endl;
  
  TH1F HTrainSignal("HTrainSignal","",NBTotal,0.5,NBTotal+0.5);
  TH1F HTrainBKGType1("HTrainBKGType1","",NBTotal,0.5,NBTotal+0.5);
  TH1F HTrainBKGType2("HTrainBKGType2","",NBTotal,0.5,NBTotal+0.5);
  for(int i=1;i<=NBTotal;i++)
    if(i<=NBPTrain || (i>NBPTrain+NBPGap && i<=(NBPTrain*2+NBPGap))){
      HTrainSignal.Fill(i,collisionCount);
      HTrainBKGType1.Fill(i+1,collisionCount*Type1Frac);
      for(int j=i+1;j<=NBTotal;j++)
	HTrainBKGType2.Fill(j,collisionCount*Type2Model.Eval(j-i));
     }
  
  TH1F HTrainBKG("HTrainBKG","",NBTotal,0.5,NBTotal+0.5);
  HTrainBKG.Add(&HTrainBKGType1);
  HTrainBKG.Add(&HTrainBKGType2);
  HTrainBKG.SetLineColor(4);


  //estimate type1 fraction unc., type1 frac is determied from one bin at end of train wagon. this fraction should be averaged over all wagons giving better precision
  float Type1FracUnc_frac =  pow(sqrt(NTrain*collisionCount)/(NTrain*collisionCount),2) ///unc in denominator of fraction
    + pow(sqrt(NTrain*collisionCount*Type1Frac)/(NTrain*collisionCount*Type1Frac),2) ;   // unc in type1 afterglow bin
 
  float Type1FracUnc = Type1Frac*sqrt(Type1FracUnc_frac);
  cout<<"Type1 frac: "<<Type1Frac<<" +/- "<<Type1FracUnc<<endl;

  
  /* TH1D * HTrainBKGUncBCID[NBTotal]; */
  /* for(int i=0;i<NBTotal;i++) */
  /*   HTrainBKGUncBCID[i] = new TH1D(TString("HTrainBKGUncBCID")+i,"",NBTotal,0.5,NBTotal+0.5); */
  /* for(int i=1;i<=NBTotal;i++) */
  /*   if(i<=NBPTrain || (i>(NBPTrain+NBPGap) && i<=(NBPTrain*2+NBPGap))){ */
  /*     for(int j=i+1;j<=(NBTotal);j++){ */
  /* 	double unc = 0; */
  /* 	if(j==i+1) unc = sqrt(collisionCount)*(Type1Frac + Type2Model.Eval(j-i)); */
  /* 	if(j>i+1) unc = sqrt(collisionCount)*Type2Model.Eval(j-i); */
  /* 	HTrainBKGUncBCID[i-1]->Fill(j,unc); */
  /*     } */
  /*   } */
  
  TH1D HTrainBKGUnc("HTrainBKGUnc","",NBTotal,0.5,NBTotal+0.5);
  HTrainBKGUnc.SetLineColor(3);
  for(int i=1;i<=NBTotal;i++)
    if(i<=NBPTrain || (i>(NBPTrain+NBPGap) && i<=(NBPTrain*2+NBPGap))){
      for(int j=i;j<=NBTotal;j++){

//   AF= N * (T1 + T2);
//   AF_err^2 =  pow(sqrt(N)*T1,2) + pow(N*T1Err,2) +  pow(sqrt(N)*T2,2) = N*T1*T1 + pow(N*T1Err,2) + N*T2*T2   
//   AF= N * T2; 
//   AF_err = sqrt(N)*T2; 

	double unc = 0.00000001;
	if(j==(i+1)) unc = collisionCount*Type1Frac*Type1Frac + pow(collisionCount*Type1FracUnc,2) + collisionCount*pow(Type2Model.Eval(j-i),2);
	if(j>(i+1))  unc = collisionCount*pow(Type2Model.Eval(j-i),2);     // here only the type2 unc 
	HTrainBKGUnc.Fill(j,unc); // different colliding bcids will be added in quadrature
      }
    }  
  for(int i=1;i<=NBTotal;i++)
    HTrainBKGUnc.SetBinContent(i,sqrt(HTrainBKGUnc.GetBinContent(i)));

    
  HTrainSignal.GetYaxis()->SetRangeUser(0.01,1.1*collisionCount);
  HTrainSignal.GetYaxis()->SetTitle("Cluster Count");
  HTrainSignal.GetXaxis()->SetTitle("bx");
  HTrainSignal.SetMarkerStyle(8);
  HTrainSignal.SetMarkerSize(0.5);
  HTrainSignal.SetStats(0);
  C.Clear();
  C.SetLogy(1);
  HTrainSignal.Draw("histp");
  //HTrainBKGType1.Draw("histsame");
  //HTrainBKGType2.Draw("histsame");
  HTrainBKG.Draw("histsame");
  HTrainBKGUnc.Draw("histsame");
  C.Print("plotAfterglowModel_TrainSignalBKGCount.png");  


  
  TH1F HTrain("HTrain","",NBTotal,0.5,NBTotal+0.5);
  HTrain.Add(&HTrainSignal);
  HTrain.Add(&HTrainBKG);
  HTrain.SetMarkerStyle(8);
  HTrain.SetMarkerSize(0.7);
  HTrain.SetStats(0);
  HTrain.SetLineColor(4);
  C.Clear();
  C.SetLogy(1);
  HTrain.GetYaxis()->SetRangeUser(0.01,1.1*collisionCount);
  HTrain.GetYaxis()->SetTitle("Cluster Count");
  HTrain.GetXaxis()->SetTitle("bx");
  HTrain.Draw("hist");
  HTrainSignal.Draw("histpsame");
  C.Print("plotAfterglowModel_TotalTrainSignal.png");



 
  C.Clear();
  C.SetLogy(1);
  HTrainBKG.GetYaxis()->SetRangeUser(0.01,2*HTrainBKG.GetMaximum());
  HTrainBKG.GetYaxis()->SetTitle("Afterglow Cluster Count");
  HTrainBKG.GetXaxis()->SetTitle("bx");
  HTrainBKG.SetStats(0);
  HTrainBKG.Draw("hist");
  HTrainBKGUnc.Draw("histsame");
  //for(int i=0;i<NBTotal;i++)
  //HTrainBKGUncBCID[i]->Draw("histsame");
  C.Print("plotAfterglowModel_TrainBKGUnc.png");


  
  
  TH1F HTrainCorr("HTrainCorr","",NBTotal,0.5,NBTotal+0.5);
  for(int i=1;i<=NBTotal;i++){
    float c=HTrainSignal.GetBinContent(i)/HTrain.GetBinContent(i);
    HTrainCorr.SetBinContent(i,c);
    HTrainCorr.SetBinError(i,c*HTrainBKGUnc.GetBinContent(i)/HTrain.GetBinContent(i));
  }
  
  TH1F HTrainCorrUnc("HTrainCorrUnc","",NBTotal,0.5,NBTotal+0.5);
  for(int i=1;i<=NBTotal;i++)
    HTrainCorrUnc.SetBinContent(i,HTrainCorr.GetBinError(i));
  

  HTrainCorr.SetStats(0);
  HTrainCorr.GetYaxis()->SetRangeUser(0.92,1.01);
  HTrainCorr.GetYaxis()->SetTitle("Afterglow Correction");
  HTrainCorr.GetXaxis()->SetTitle("bx");
  HTrainCorr.SetMarkerStyle(8);
  HTrainCorr.SetMarkerSize(0.5);
  C.Clear();
  C.SetLogy(0);
  HTrainCorr.Draw("histlpe");
  C.Print("plotAfterglowModel_TrainCorr.png");

  HTrainCorrUnc.SetStats(0);
  HTrainCorrUnc.GetYaxis()->SetRangeUser(0,0.002);
  HTrainCorrUnc.GetYaxis()->SetTitle("Afterglow Correction Uncertainty");
  HTrainCorrUnc.GetXaxis()->SetTitle("bx");
  HTrainCorrUnc.SetMarkerStyle(8);
  HTrainCorrUnc.SetMarkerSize(0.5);
  C.Clear();
  C.SetLogy(0);
  HTrainCorrUnc.Draw("histp");
  C.Print("plotAfterglowModel_TrainCorrUnc.png");

}
