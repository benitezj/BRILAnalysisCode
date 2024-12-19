#include <iostream>
#include <vector>
#include <dirent.h>
#include "globals.h"
#include "rootlogon.C"

void plotlabels(){
  //drawCMSPrelim();
  drawFillYear(0,2017);
  //drawPCCLuminometer();
}

float MinPCCColliding=100; 

//std::string runlist="362064";

//std::string runlist="360390,360391,360392,360393,360400,360411,360413,360414,360428,360429,360430,360431,360432,360433,360434,360435,360436,360437,360438,360439,360440,360441,360442,360444,360445,360458,360459,360460,360472,360486,360490,360491,360519,360520,360523,360525,360526,360528,360530,360533,360538,360542,360590,360591,360594,360659,360660,360666,360681,360683,360684,360685,360688,360696,360697,360698,360703,360736,360737,360761,360787,360788,360794,360795,360796,360797,360813,360815,360816,360817,360819,360820,360821,360822,360823,360825,360826,360850,360856,360874,360882,360886,360887,360888,360889,360890,360891,360892,360893,360894,360895,360912,360919,360920,360921,360927,360940,360941,360942,360943,360944,360945,360946,360948,360949,360950,360951,360968,360989,360991,360992,361020,361032,361038,361044,361045,361046,361047,361048,361050,361051,361052,361053,361054,361059,361074,361082,361083,361088,361089,361090,361091,361092,361101,361105,361106,361107,361108,361110,361177,361183,361188,361193,361195,361196,361197,361198,361208,361223,361239,361240,361267,361269,361270,361271,361272,361279,361280,361283,361284,361292,361294,361297,361303,361318,361319,361320,361333,361361,361362,361363,361364,361365,361366,361370,361398,361400,361417,361424,361442,361443,361464,361467,361468,361475,361503,361512,361562,361564,361569,361570,361573,361579,361580,361604,361606,361607,361609,361614,361618,361677,361685,361686,361722,361723,361724,361740,361790,361791,361807,361808,361810,361848,361849,361852,361859,361860,361872,361881,361885,361887,361889,361890,361893,361894,361895,361896,361898,361905,361906,361948,361954,361957,361968,361971,361974,361989,361990,361992,361993,361994,362009,362058,362059,362060,362061,362062,362063,362064,362085,362086,362087,362091,362095,362104,362105,362106,362107,362108,362148,362152,362153,362154,362159,362160,362161,362163,362166,362167";

//std::string runlist="365753,365754,365755,365756,365757,365765,365775,365834,365835,365836,365837,365838,365839,365840,365841,365842,365843,365844,365876,365881,365882,365883,365884,365888,365889,366035";
//std::string runlist="366403,366406,366409,366410,366413,366419,366422,366424,366426,366427,366429,366432,366434,366436,366437,366438,366439,366440,366441,366442,366449,366450,366451,366469,366470,366471,366494,366495,366496,366497,366498,366499,366500,366504,366533,366724,366727,366728,366729,366793,366794,366795,366797,366798,366800,366801,366802,366804,366820,366821,366829,366832,366833,366838,366839,366840,366841,366850,366873,366874,366876,366891,366892,366893,366895,366933,367062,367063,367064,367065,367070,367071,367072,367073,367079";
//std::string runlist="367516,367552,367553,367554,367589,367591,367592,367593,367615,367616,367617,367618,367619,367622,367661,367662,367663,367664,367665,367666,367691,367692,367693,367694,367695,367696,367697,367699,367730,367742,367758,367770,367771,367772,367790,367823,367831,367836,367838,367840,367881,367882,367883,367884,367902,367904,367905,367906,367907,367908,367910,368151,368223,368224,368229,368247,368318,368320,368321,368326,368331,368332,368333,368334,368336,368337,368338,368339,368340,368341,368343,368382,368383,368384,368388,368389,368400,368406,368410,368411,368412,368423,368437,368440,368443,368451,368452,368453,368454,368489,368542,368546,368547,368548,368566,368567,368609,368611,368612,368613,368636,368669,368670,368671,368672,368673,368674,368675,368676,368678,368684,368685,368723,368724,368746,368748,368749,368752,368753,368762,368763,368764,368765,368774,368815,368816,368817,368818,368822,368823,369279,369436,369444,369447,369794,369795,369797,369802";
//std::string runlist="369867,369869,369870,369872,369873,369901,369927,369937,369938,369941,369942,369943,369956,369978,369994,369998,369999,370000,370005,370091,370092,370093,370095,370096,370097,370100,370101,370102,370129,370144,370169,370171,370172,370175,370192,370195,370196,370293,370294,370300,370302,370304,370307,370321,370332,370340,370354,370355,370357,370406,370436,370460,370472,370497,370522,370560,370579,370580,370664,370665,370666,370667,370717,370725,370749,370750,370751,370753,370772,370774,370775,370776,370790,371225";

//std::string runlist="366403,366406,366409,366410,366413,366419,366422,366424,366426,366427,366429,366432,366434,366436,366437,366438,366439,366440,366441,366442,366449,366450,366451,366469,366470,366471,366494,366495,366496,366497,366498,366499,366500,366504,366533,366724,366727,366728,366729,366793,366794,366795,366797,366798,366800,366801,366802,366804,366820,366821,366829,366832,366833,366838,366839,366840,366841,366850,366873,366874,366876,366891,366892,366893,366895,366933,367062,367063,367064,367065,367070,367071,367072,367073,367079,367516,367552,367553,367554,367589,367591,367592,367593,367615,367616,367617,367618,367619,367622,367661,367662,367663,367664,367665,367666,367691,367692,367693,367694,367695,367696,367697,367699,367730,367742,367758,367770,367771,367772,367790,367823,367831,367836,367838,367840,367881,367882,367883,367884,367902,367904,367905,367906,367907,367908,367910,368151,368223,368224,368229,368247,368318,368320,368321,368326,368331,368332,368333,368334,368336,368337,368338,368339,368340,368341,368343,368382,368383,368384,368388,368389,368400,368406,368410,368411,368412,368423,368437,368440,368443,368451,368452,368453,368454,368489,368542,368546,368547,368548,368566,368567,368609,368611,368612,368613,368636,368669,368670,368671,368672,368673,368674,368675,368676,368678,368684,368685,368723,368724,368746,368748,368749,368752,368753,368762,368763,368764,368765,368774,368815,368816,368817,368818,368822,368823,369279,369436,369444,369447,369794,369795,369797,369802,369867,369869,369870,369872,369873,369901,369927,369937,369938,369941,369942,369943,369956,369978,369994,369998,369999,370000,370005,370091,370092,370093,370095,370096,370097,370100,370101,370102,370129,370144,370169,370171,370172,370175,370192,370195,370196,370293,370294,370300,370302,370304,370307,370321,370332,370340,370354,370355,370357,370406,370436,370460,370472,370497,370522,370560,370579,370580,370664,370665,370666,370667,370717,370725,370749,370750,370751,370753,370772,370774,370775,370776,370790,371225";

//2023 mu-scan fill
//std::string runlist="370091,370093,370095,370096,370097,370102";


//2017
std::string runlist="297046,297047,297048,297049,297050,297056,297057,297099,297100,297101,297113,297114,297168,297169,297170,297171,297175,297176,297177,297178,297179,297180,297181,297211,297215,297218,297219,297224,297225,297227,297281,297282,297283,297284,297285,297286,297287,297288,297289,297290,297291,297292,297293,297296,297308,297359,297411,297424,297425,297426,297429,297430,297431,297432,297433,297434,297435,297467,297468,297469,297474,297483,297484,297485,297486,297487,297488,297494,297495,297496,297497,297498,297499,297500,297501,297502,297503,297504,297505,297557,297558,297559,297560,297562,297563,297598,297599,297603,297604,297605,297606,297620,297656,297657,297658,297659,297660,297661,297662,297663,297664,297665,297666,297670,297671,297672,297673,297674,297675,297678,297722,297723,298809,298853,298855,298996,298997,298998,299000,299042,299061,299062,299064,299065,299067,299084,299096,299149,299178,299180,299183,299184,299185,299316,299317,299318,299324,299325,299326,299327,299329";


float getNColl(TFile*F=NULL, int Run=-1, int I=-1,int type=0){
  if(F==NULL || Run<0 || I<0) return 0;
  float ncoll=0.;
  float sf=0.;
  TIter next(F->GetListOfKeys());
  TObject* key;
  while ((key = next())) {
    TString kname(key->GetName());    
    if(!kname.Contains("ScaleFactorsAvg_")) continue; //ScaleFactorsAvg_370093_7_351_400-
    TObjArray * a = kname.Tokenize("_");
    long r=atoi(((TObjString*)(*a)[1])->GetName());
    long l=atoi(((TObjString*)(*a)[2])->GetName());
    if(r!=Run || l!=I) continue; //select the desired block
    TH1F* HIST = (TH1F*) F->Get(kname.Data());
    if(!HIST) continue;
    for(int j = 1; j <= HIST->GetNbinsX(); j++){ 
      if(HIST->GetBinContent(j)>0.7){
	ncoll++;
	if(type==1)
	  sf+=HIST->GetBinContent(j);
      }
    }
  }
  
  if(type==1) return sf/ncoll; 
  return ncoll;
}



std::pair<float,float> getPCCAvg(TFile*F=NULL, int Run=-1, int I=-1, int type=1){
  if(F==NULL || Run<0 || I<0) return std::pair<float,float>();
  
  std::pair<float,float> bincontent_avg;
  bincontent_avg.first=0.;
  bincontent_avg.second=0.;
 

  TIter next(F->GetListOfKeys());
  TObject* key;
  while ((key = next())) {
    TString kname(key->GetName());    //TString("RawHISTAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
    if(!kname.Contains("RawLumiAvg_")) continue;
    
    TObjArray * a = kname.Tokenize("_");
    long r=atoi(((TObjString*)(*a)[1])->GetName());
    long l=atoi(((TObjString*)(*a)[2])->GetName());
    long ls1=atoi(((TObjString*)(*a)[3])->GetName());
    long ls2=atoi(((TObjString*)(*a)[4])->GetName());
  
    if(r!=Run || l!=I) continue; //select the desired block
    
    TH1F* Lumi = (TH1F*) F->Get(kname.Data());
    if(!Lumi)      continue;

    ///Average PCC from colliding bx
    if(type==1){
      int nbincounter=0;
      float bincontent_total=0.;
      for(int j = 1; j <= Lumi->GetNbinsX(); j++) {
	float bincontent = Lumi->GetBinContent(j);
	if (bincontent > MinPCCColliding){
	  bincontent_total += bincontent;
	  nbincounter++;
	}
      }
      if(nbincounter>0)
	bincontent_avg.first =bincontent_total/nbincounter;
    }


    ///background from non-colliding bx after train
    if(type==2){  
      int afterglowcounter=0;
      int nbincounter=0;
      float bincontent_total=0;
      int NAfterGlowBX=15;  
      
      for(int j = 1; j <= Lumi->GetNbinsX(); j++) {
	float bincontent = Lumi->GetBinContent(j);
	afterglowcounter++;
	if(bincontent > MinPCCColliding)
	  afterglowcounter=0;
	if(afterglowcounter>NAfterGlowBX && bincontent>0){
	  bincontent_total += bincontent;
	  nbincounter++;
	}
      }
      
      if(nbincounter>0){//calcuate average bkg and unc.
	bincontent_avg.first = bincontent_total / nbincounter;

	afterglowcounter=0;
	bincontent_total=0.;
	for(int j = 1; j <= Lumi->GetNbinsX(); j++) {
	  float bincontent = Lumi->GetBinContent(j);
	  afterglowcounter++;
	  if(bincontent > MinPCCColliding)
	    afterglowcounter=0;
	  if(afterglowcounter>NAfterGlowBX && bincontent>0)
	    bincontent_total += (bincontent-bincontent_avg.first)*(bincontent-bincontent_avg.first);
	}
	//bincontent_avg.second = sqrt(bincontent_total / nbincounter); // RMS
	bincontent_avg.second = sqrt(bincontent_total) / nbincounter; //SEM
      }      
    }
    
    

  }    

  return bincontent_avg;
}

void plot_afterglow_residual(TString inpath, TString outpath, TString RUNLIST="") {
  cout<<"input path: "<<inpath<<endl;
  cout<<"output path: "<<outpath<<endl;

  if(RUNLIST.CompareTo("")!=0) runlist=RUNLIST.Data();
  cout<<runlist<<endl;

  
  rootlogon();
  
  TGraph* gAvgVsIOV=new TGraph();
  TGraph* gT1fracVsPCC=new TGraph();
  TGraph* gT1fracVsIOV=new TGraph();
  TGraph* gT1ResidVsPCC=new TGraph();
  TGraph* gT1ResidVsIOV=new TGraph();
  TGraph* gT2ResidVsPCC=new TGraph();
  TGraph* gT2ResidVsIOV=new TGraph();
  TGraph* gPedVsIOV=new TGraph();
  TGraphErrors* gBkgVsIOV=new TGraphErrors();
  TGraph* gNCollVsIOV=new TGraph();
  TGraph* gAvgSFVsIOV=new TGraph();

  ////////////////////
  std::stringstream ss(runlist.c_str());
  int Run;
  while (ss >> Run) {
    cout<<Run<<endl;  
    ss.ignore(1);   

    TString filename=inpath+"/"+Run+".root";
    if(gSystem->AccessPathName(filename.Data())){ cout<<"bad file: "<<filename<<endl; continue;}

    TFile InputFile(filename.Data(),"read");
    if(InputFile.IsZombie()) continue;
    //InputFile.ls();

    TGraphErrors* gT1frac = (TGraphErrors*)InputFile.Get("Type1Fraction");
    TGraphErrors* gT1resid = (TGraphErrors*)InputFile.Get("Type1Res");
    TGraphErrors* gT2resid = (TGraphErrors*)InputFile.Get("Type2Res"); 
    TGraphErrors* gPed = (TGraphErrors*)InputFile.Get("Pedestal");
    if(!gT1frac || !gT2resid || !gT2resid){cout<<" objects  not found. run="<<Run<<endl; continue;}

    double *YT1frac = gT1frac->GetY();
    double *YT1resid = gT1resid->GetY();
    double *YT2resid = gT2resid->GetY();
    double *YPed = gPed->GetY();
    for (int l = 0; l < gT1frac->GetN(); l++) {
      cout<<gAvgVsIOV->GetN()<<",";  
      std::pair<float,float> pccavg=getPCCAvg(&InputFile,Run,l);
      if(pccavg.first>MinPCCColliding){
	gAvgVsIOV->SetPoint(gAvgVsIOV->GetN(),gAvgVsIOV->GetN(),pccavg.first);
	gT1fracVsPCC->SetPoint(gT1fracVsPCC->GetN(),pccavg.first, YT1frac[l]*100); 
	gT1fracVsIOV->SetPoint(gT1fracVsIOV->GetN(),gT1fracVsIOV->GetN(), YT1frac[l]*100); 
	gT1ResidVsPCC->SetPoint(gT1ResidVsPCC->GetN(),pccavg.first, YT1resid[l]*100); 
	gT1ResidVsIOV->SetPoint(gT1ResidVsIOV->GetN(),gT1ResidVsIOV->GetN(), YT1resid[l]*100);  
	gT2ResidVsPCC->SetPoint(gT2ResidVsPCC->GetN(),pccavg.first, YT2resid[l]*100);  
	gT2ResidVsIOV->SetPoint(gT2ResidVsIOV->GetN(),gT2ResidVsIOV->GetN(), YT2resid[l]*100); 
	gPedVsIOV->SetPoint(gPedVsIOV->GetN(),gPedVsIOV->GetN(), (YPed[l]/pccavg.first)*100);  
	
      }
      
      gNCollVsIOV->SetPoint(gNCollVsIOV->GetN(),gNCollVsIOV->GetN(),getNColl(&InputFile,Run,l));
      gAvgSFVsIOV->SetPoint(gAvgSFVsIOV->GetN(),gAvgSFVsIOV->GetN(),getNColl(&InputFile,Run,l,1));
    }

  }


  ////////////////////
  TCanvas Canvas;//("Canvas"," ", 0,67,1264,678);
  TLine line;

  ///////////////////////////////////
  ///type 1 fraction
  Canvas.Clear();
  gT1fracVsPCC->GetYaxis()->SetTitle("Type 1 Afterglow Fraction [%]");
  gT1fracVsPCC->GetXaxis()->SetTitle("Average PCC");
  gT1fracVsPCC->GetYaxis()->SetRangeUser(0.,6);
  gT1fracVsPCC->SetMarkerStyle(8);
  gT1fracVsPCC->SetMarkerSize(0.6);
  gT1fracVsPCC->Draw("ap");
  plotlabels();
  line.DrawLine(gT1fracVsPCC->GetXaxis()->GetXmin(),0,gT1fracVsPCC->GetXaxis()->GetXmax(),0);
  Canvas.Print(outpath+"/afterglow_t1f_vsinstlumi.png");

  Canvas.Clear();
  gT1fracVsIOV->GetYaxis()->SetTitle("Type 1 Afterglow Fraction [%]");
  gT1fracVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gT1fracVsIOV->GetYaxis()->SetRangeUser(0,6);
  gT1fracVsIOV->SetMarkerStyle(8);
  gT1fracVsIOV->SetMarkerSize(0.6);
  gT1fracVsIOV->Draw("ap");
  plotlabels();
  line.DrawLine(gT1fracVsIOV->GetXaxis()->GetXmin(),0,gT1fracVsIOV->GetXaxis()->GetXmax(),0);
  Canvas.Print(outpath+"/afterglow_t1f_vsLSBlock.png");

  ///////////////////////////////////////
  ///Type 1 residual
  Canvas.Clear();
  gT1ResidVsPCC->GetYaxis()->SetTitle("Type 1 Afterglow Residual [%]");
  gT1ResidVsPCC->GetXaxis()->SetTitle("Average PCC");
  gT1ResidVsPCC->GetYaxis()->SetRangeUser(-0.01,0.01);
  gT1ResidVsPCC->SetMarkerStyle(8);
  gT1ResidVsPCC->SetMarkerSize(0.6);
  gT1ResidVsPCC->Draw("ap");
  plotlabels();
  line.DrawLine(gT1ResidVsPCC->GetXaxis()->GetXmin(),0,gT1ResidVsPCC->GetXaxis()->GetXmax(),0);
  Canvas.Print(outpath+"/afterglow_t1res_vsinstlumi.png");

  Canvas.Clear();
  gT1ResidVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gT1ResidVsIOV->GetYaxis()->SetTitle("Type 1 Afterglow Residual [%]");
  gT1ResidVsIOV->GetYaxis()->SetRangeUser(-0.01,0.01);
  gT1ResidVsIOV->SetMarkerStyle(8);
  gT1ResidVsIOV->SetMarkerSize(0.6);
  gT1ResidVsIOV->Draw("ap");
  plotlabels();
  line.DrawLine(gT1ResidVsIOV->GetXaxis()->GetXmin(),0,gT1ResidVsIOV->GetXaxis()->GetXmax(),0);
  Canvas.Print(outpath+"/afterglow_t1res_vsLSBlock.png");


  ////////////////////////
  ///Type 2 residual
  Canvas.Clear();
  gT2ResidVsPCC->GetYaxis()->SetTitle("Type 2 Afterglow Residual [%]");
  gT2ResidVsPCC->GetXaxis()->SetTitle("Average PCC ");
  gT2ResidVsPCC->GetYaxis()->SetRangeUser(-2,2);
  gT2ResidVsPCC->SetMarkerStyle(8);
  gT2ResidVsPCC->SetMarkerSize(0.6);
  gT2ResidVsPCC->Draw("ap");
  plotlabels();
  line.DrawLine(gT2ResidVsPCC->GetXaxis()->GetXmin(),0,gT2ResidVsPCC->GetXaxis()->GetXmax(),0);
  Canvas.Print(outpath+"/afterglow_t2res_vsinstlumi.png");

  Canvas.Clear();
  gT2ResidVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gT2ResidVsIOV->GetYaxis()->SetTitle("Type 2 Afterglow Residual [%]");
  gT2ResidVsIOV->GetYaxis()->SetRangeUser(-2,2);
  gT2ResidVsIOV->SetMarkerStyle(8);
  gT2ResidVsIOV->SetMarkerSize(0.6);
  gT2ResidVsIOV->Draw("ap");
  plotlabels();
  line.DrawLine(gT2ResidVsIOV->GetXaxis()->GetXmin(),0,gT2ResidVsIOV->GetXaxis()->GetXmax(),0);
  Canvas.Print(outpath+"/afterglow_t2res_vsLSBlock.png");


  /////////Pedestal
  Canvas.Clear();
  gPedVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gPedVsIOV->GetYaxis()->SetTitle("Pedestal Fraction [%]");
  gPedVsIOV->GetYaxis()->SetRangeUser(-2,2);
  gPedVsIOV->SetMarkerStyle(8);
  gPedVsIOV->SetMarkerSize(0.6);
  gPedVsIOV->Draw("ap");
  plotlabels();
  line.DrawLine(gPedVsIOV->GetXaxis()->GetXmin(),0,gPedVsIOV->GetXaxis()->GetXmax(),0);
  Canvas.Print(outpath+"/afterglow_pedestal_vsLSBlock.png");


  /////////Avg Colliding 
  Canvas.Clear();
  gAvgVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gAvgVsIOV->GetYaxis()->SetTitle("Avg. PCC colliding");
  gAvgVsIOV->SetMarkerStyle(8);
  gAvgVsIOV->SetMarkerSize(0.6);
  gAvgVsIOV->Draw("ap");
  plotlabels();
  Canvas.Print(outpath+"/afterglow_avgpcc_vsLSBlock.png");


  Canvas.Clear();
  gNCollVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gNCollVsIOV->GetYaxis()->SetTitle("# of colliding bunches");
  gNCollVsIOV->SetMarkerStyle(8);
  gNCollVsIOV->SetMarkerSize(0.6);
  gNCollVsIOV->Draw("ap");
  plotlabels();
  Canvas.Print(outpath+"/afterglow_ncoll_vsLSBlock.png");




  Canvas.Clear();
  gAvgSFVsIOV->GetXaxis()->SetTitle("50 LS Block");
  gAvgSFVsIOV->GetYaxis()->SetTitle("Avg. Scale Factor");
  gAvgSFVsIOV->SetMarkerStyle(8);
  gAvgSFVsIOV->SetMarkerSize(0.6);
  gAvgSFVsIOV->Draw("ap");
  plotlabels();
  Canvas.Print(outpath+"/afterglow_avgsf_vsLSBlock.png");

  
}





 
