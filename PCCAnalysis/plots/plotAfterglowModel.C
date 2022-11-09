
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


#define modelrange 3600
#define range 25

#define ncolliding 150
#define interval 22

void plotAfterglowModel(){
  
  TF1 F("F",formula,0.5,modelrange+0.5);
  F.SetParameter(0,type2_a);
  F.SetParameter(1,type2_b);
  

  TH1F HF("HF","",range,0.5,range+0.5);
  HF.SetTitle("Afterglow Model");
  HF.GetYaxis()->SetTitle("F");
  HF.GetXaxis()->SetTitle("bunch number");
  HF.SetStats(0);
  HF.GetYaxis()->SetRangeUser(0,type2_a);

  TCanvas C;
  C.SetLeftMargin(0.15);


  /////////Plot of the per bunch Afterglow model
  C.Clear();
  HF.Draw("hist");
  F.Draw("lsame");
  TLatex text;
  text.DrawLatexNDC(0.3,0.2,TString("F = ")+formula);
  C.Print("plotAfterglowModel.png");


  //Plot of the sum of 150 equally spaced bunches
  TH1F HModelSumOrbit("HModelSumOrbit","",range,0.5,range+0.5);
  HModelSumOrbit.SetTitle("Afterglow for vdM fill");
  HModelSumOrbit.GetYaxis()->SetTitle(TString("Model Sum (n=")+ncolliding+", interval="+interval+")");
  HModelSumOrbit.GetXaxis()->SetTitle("bunch number");
  HModelSumOrbit.SetStats(0);

  for(int i=0;i<150;i++){
    for(int b=1;b<=range;b++)
      HModelSumOrbit.AddBinContent(b,F.Eval(i*interval+b));
  }
  C.Clear();
  HModelSumOrbit.Draw("histl");
  C.Print("plotAfterglowModelSum.png");  


  /////////////////////////////////////////////////////////////
  ///Analysis of a train with 1800 collding bunches
  //https://lpc.web.cern.ch/fillingSchemes/2022/candidates/25ns_1815b_1802_1443_1542_180bpi_14inj_3INDIVs.csv
  //looks like 18 colliding bunches per train
  int NBPTrain=36;
  int NBPGap=7;
  
  //1. Afterglow is normalized on the colliding bunches.
  //2. Unc is driven by sqrt(N) on colliding bunches
  //3. nominal rate for Randoms=400hz, nominal PCC per pp=100, integration time, pileup 
  int RDTriggerRate=400;
  float RDTriggerRatePerB=(float)RDTriggerRate/NBX;
  int PCCPerPP=100;
  float LSTime=23.31*50;
  int Pileup=40;
  float Type1Frac=0.024;///eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom/Run2018B_dynamicVeto/317626.root
  
  float collisionCount=PCCPerPP*Pileup*RDTriggerRatePerB*LSTime;
  cout<<"collisionCount= "<<collisionCount<<endl;
  
  TH1F HTrainSignal("HTrainSignal","",NBPTrain+NBPGap,0.5,NBPTrain+NBPGap+0.5);
  for(int i=1;i<=NBPTrain;i++)
    HTrainSignal.Fill(i,collisionCount);

  TH1F HTrainBKGType1("HTrainBKGType1","",NBPTrain+NBPGap,0.5,NBPTrain+NBPGap+0.5);
  for(int i=1;i<=NBPTrain;i++)
    HTrainBKGType1.Fill(i+1,collisionCount*Type1Frac);
  
  TH1F HTrainBKGType2("HTrainBKGType2","",NBPTrain+NBPGap,0.5,NBPTrain+NBPGap+0.5);
  for(int i=1;i<=NBPTrain;i++)
    for(int j=i+1;j<=(NBPTrain+NBPGap);j++)
      HTrainBKGType2.Fill(j,collisionCount*F.Eval(j-i));

  TH1F HTrainBKG("HTrainBKG","",NBPTrain+NBPGap,0.5,NBPTrain+NBPGap+0.5);
  HTrainBKG.Add(&HTrainBKGType1);
  HTrainBKG.Add(&HTrainBKGType2);
  HTrainBKG.SetLineColor(4);
  
  TH1F HTrainBKGUnc("HTrainBKGUnc","",NBPTrain+NBPGap,0.5,NBPTrain+NBPGap+0.5);
  HTrainBKGUnc.SetLineColor(3);
  for(int i=1;i<=NBPTrain;i++)
    HTrainBKGUnc.Fill(i+1,sqrt(collisionCount)*Type1Frac);
  for(int i=1;i<=NBPTrain;i++)
    for(int j=i+1;j<=(NBPTrain+NBPGap);j++)
      HTrainBKGUnc.Fill(j,sqrt(collisionCount)*F.Eval(j-i));

  
  HTrainSignal.GetYaxis()->SetRangeUser(0.1,1.1*collisionCount);
  HTrainSignal.GetYaxis()->SetTitle("Cluster Count");
  HTrainSignal.GetXaxis()->SetTitle("bx");
  HTrainSignal.SetMarkerStyle(8);
  HTrainSignal.SetMarkerSize(0.5);
  HTrainSignal.SetStats(0);
  C.Clear();
  C.SetLogy(1);
  HTrainSignal.Draw("histp");
  HTrainBKG.Draw("histsame");
  //HTrainBKGUnc.Draw("histsame");
  C.Print("plotAfterglowModel_TrainSignalBKGCount.png");  
  
  TH1F HTrain("HTrain","",NBPTrain+NBPGap,0.5,NBPTrain+NBPGap+0.5);
  HTrain.Add(&HTrainSignal);
  HTrain.Add(&HTrainBKG);
  HTrain.SetMarkerStyle(8);
  HTrain.SetMarkerSize(0.7);
  HTrain.SetStats(0);
  for(int i=1;i<=(NBPTrain+NBPGap);i++)
    HTrainBKGUnc.Fill(i+1,sqrt(collisionCount)*Type1Frac);
  C.Clear();
  C.SetLogy(1);
  HTrain.GetYaxis()->SetRangeUser(0.1,1.1*collisionCount);
  HTrain.GetYaxis()->SetTitle("Cluster Count");
  HTrain.GetXaxis()->SetTitle("bx");
  HTrain.Draw("hist");
  HTrainSignal.Draw("histpsame");
  C.Print("plotAfterglowModel_Train.png");

  TH1F HTrainCorr("HTrainCorr","",NBPTrain+NBPGap,0.5,NBPTrain+NBPGap+0.5);
  TH1F HTrainCorrUnc("HTrainCorrUnc","",NBPTrain+NBPGap,0.5,NBPTrain+NBPGap+0.5);
  for(int i=1;i<=(NBPTrain+NBPGap);i++){
    HTrainCorr.SetBinContent(i,HTrainSignal.GetBinContent(i)/HTrain.GetBinContent(i));
    HTrainCorr.SetBinError(i,HTrainBKGUnc.GetBinContent(i)/HTrain.GetBinContent(i));

    HTrainCorrUnc.SetBinContent(i,HTrainBKGUnc.GetBinContent(i)/collisionCount);
  }

  HTrainCorr.SetStats(0);
  HTrainCorr.GetYaxis()->SetRangeUser(0.95,1.01);
  HTrainCorr.GetYaxis()->SetTitle("Afterglow Correction");
  HTrainCorr.GetXaxis()->SetTitle("bx");
  HTrainCorr.SetMarkerStyle(8);
  HTrainCorr.SetMarkerSize(0.4);
  C.Clear();
  C.SetLogy(0);
  HTrainCorr.Draw("histlpe");
  C.Print("plotAfterglowModel_TrainCorr.png");

  HTrainCorrUnc.SetStats(0);
  HTrainCorrUnc.GetYaxis()->SetRangeUser(0,0.005);
  HTrainCorrUnc.GetYaxis()->SetTitle("Afterglow Correction Uncertainty");
  HTrainCorrUnc.GetXaxis()->SetTitle("bx");
  HTrainCorrUnc.SetMarkerStyle(8);
  HTrainCorrUnc.SetMarkerSize(0.4);
  C.Clear();
  C.SetLogy(0);
  HTrainCorrUnc.Draw("histp");
  C.Print("plotAfterglowModel_TrainCorrUnc.png");

}
