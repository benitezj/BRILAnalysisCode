
/////input and output paths
//TString INPATH("/afs/cern.ch/user/b/benitezj/output/BRIL/CMSSW_10_2_2/src/BRILAnalysisCode/BRIL-HighPileup");
//TString OUTPATH("/afs/cern.ch/user/b/benitezj/www/BRIL/HighPULinearity");

TString INPATH(".");
TString OUTPATH("./plots");


////////////
std::map<TString,TString> DETName = {{"hfoc","HFOC"},{"hfet","HFET"},{"plt","PLTZERO"},{"bcm","BCM1F"}
				     ,{"pcc","PCC"},{"pccb","PCC_B"},{"pccf","PCC_F"}
				     ,{"pccB1","PCC_B1"},{"pccB2","PCC_B2"},{"pccB3","PCC_B3"}
				     ,{"pccF1p1","PCC_F1P1"},{"pccF2p1","PCC_F2P1"},{"pccF3p1","PCC_F3P1"}
				     ,{"pccF1p2","PCC_F1P2"},{"pccF2p2","PCC_F2P2"},{"pccF3p2","PCC_F3P2"}
};
std::map<TString,int> DETColor = {{"hfoc",1},{"hfet",2},{"plt",3},{"bcm",4}
				  ,{"pcc",6},{"pccB",6},{"pccF",6}
				  ,{"pccB1",6},{"pccB2",6},{"pccB3",6}
				  ,{"pccF1p1",6},{"pccF2p1",6},{"pccF3p1",6}
				  ,{"pccF1p2",6},{"pccF2p2",6},{"pccF3p2",6}
};//note color should be set with +1


#define NDET 20
#define NPLT 16
#define NBX 3564
#define NLS 500
long timeboundary(24);
long FILL;
float BXCorr[NBX];
long NBXTrain;
TString detsel("hfoc");
TString minsbil("1.5");
TString CUTDetSelMin=TString("(")+detsel+">"+minsbil+")";
long maxsbil=10;

std::vector<TString> DETLIST;
std::vector<long> BXLIST;
std::vector<long> BXLeading;
std::vector<long> BXSpecial;
std::vector<long> LSlist;
long BXSel;


std::vector<long> TimeStep;
long tmin;
long tmax;
TString timeref;
TString CUTTime;
TString CUTLumis;
long lsmin;
long lsmax;

TCanvas * C;
TChain * tree;
TH2F * Time2D;
TLine line;
TText labeltext;

TH2F HLinearityFrame("HLinearityFrame","",200,0,20,200,0.5,1.5);

/////////////////////////////////////////////////
void configFill(long fill=0){///set fill specific configurations
  FILL=fill;

  cout<<"Beging Fill "<<FILL<<" configuration"<<endl;

  DETLIST.clear();
  //  DETLIST.push_back("hfoc");
  DETLIST.push_back("pcc");
//  DETLIST.push_back("hfet");
//  DETLIST.push_back("plt");
//  DETLIST.push_back("bcm");

//  DETLIST.push_back("pccB");
//  DETLIST.push_back("pccF");
//  DETLIST.push_back("pccB1");
//  DETLIST.push_back("pccB2");
//  DETLIST.push_back("pccB3");
//  DETLIST.push_back("pccF1p1"); 
//  DETLIST.push_back("pccF2p1"); 
//   DETLIST.push_back("pccF3p1");
//   DETLIST.push_back("pccF1p2");
//   DETLIST.push_back("pccF2p2");
//   DETLIST.push_back("pccF3p2");

  BXLIST.clear();
  BXLeading.clear();
  BXSpecial.clear();
  TimeStep.clear();
  BXSel=0;
  tree = new TChain("lumi");

  C=new TCanvas();


  if(FILL==6847){
    tree->Add(INPATH+"/bril_6847_RunDModVeto.root");
    //tree->Add(INPATH+"/bril_6847_RunDModVeto_NoCorr.root");
    tmin = 1530010610 ;
    //tmax = tmin + 400 ;//half a scan
    //tmax = tmin + 790 ;//one scan
    tmax = tmin + 1770;//both scans
    TimeStep = std::vector<long>{tmin+5,tmin+60,tmin+111,tmin+162,tmin+214,tmin+265,tmin+316,tmin+368,tmin+421,tmin+472,tmin+523,tmin+574,tmin+625,tmin+677,tmin+729,tmin+783,tmin+831,tmin+879,tmin+927,tmin+975,tmin+1023,tmin+1071,tmin+1119,tmin+1151,tmin+1201,tmin+1253,tmin+1304,tmin+1356,tmin+1409,tmin+1460,tmin+1512,tmin+1563,tmin+1615,tmin+1667,tmin+1718};
    BXSel = 686;
    NBXTrain = 1;
    BXLeading = std::vector<long>{686,816,2591,2612,2633};
    BXSpecial = BXLeading;
  }
  if(FILL==6854){
    tree->Add(INPATH+"/bril_6854_RunDModVeto.root");
    //tree->Add(INPATH+"/bril_6854_RunDModVeto_NoCorr.root");
    tmin = 1530139061 ; 
    tmax = tmin + 1650 ;
    TimeStep = std::vector<long>{tmin+61,tmin+112,tmin+164,tmin+214,tmin+266,tmin+317,tmin+369,tmin+420,tmin+473,tmin+524,tmin+576,tmin+627,tmin+679,tmin+734,tmin+780,tmin+826,tmin+872,tmin+900,tmin+954,tmin+1005,tmin+1057,tmin+1108,tmin+1159,tmin+1210,tmin+1261,tmin+1315,tmin+1366,tmin+1417,tmin+1469,tmin+1521,tmin+1572,tmin+1624};
    BXSel = 1631;

    //NBXTrain = 10;
    // BXLeading = std::vector<long>{62,149,443,1043,1337,1631,1937,2231,2831,312};//not avaialable for PCC

    NBXTrain = 1;
    BXLeading = std::vector<long>{1631};

    //BXSpecial = std::vector<long>{1631};      // high stats leading bunches
    BXSpecial = std::vector<long>{1631,1651,1678,2321,3255}; //all high stats bunches
    //BXSpecial = std::vector<long>{1651,1678,2321,3255}; //high stats train bunches
  }
  if(FILL==7274){
    tree->Add(INPATH+"/bril_7274_RunDModVeto.root");
    tmin = 1539215450;
    tmax = tmin + 1600;
    TimeStep = std::vector<long>{tmin+95,tmin+171,tmin+245,tmin+321,tmin+418,tmin+514,tmin+660,tmin+899,tmin+1044,tmin+1141,tmin+1240,tmin+1315,tmin+1390,tmin+1464};
    BXSel = 62;
    NBXTrain = 10;
    BXLeading = std::vector<long>{62,196,385,574,767,901,1090,1279,1468,1661,1795,1984,2173,2362,2555,2689};
    for(int i=0;i<BXLeading.size();i++){
      //BXSpecial.push_back(BXLeading[i]);
      BXSpecial.push_back(BXLeading[i]+NBXTrain-1);//last bunch in train 
    }
  }
  if(FILL==7358){
    //tree->Add(INPATH+"/bril.root");
    tree->Add(INPATH+"/bril_7358_RunDModVeto.root");
    //tree->Add(INPATH+"/bril_7358_RunDModVeto_NoCorr.root");
    //tree->Add(INPATH+"/bril_7358_RunDModVeto_NoHFCorr.root");
    tmin = 1540537829-25;
    tmax = tmin + 600;
    TimeStep = std::vector<long>{1540537800+64,1540537800+140,1540537800+237,1540537800+358,1540537800+477,1540537800+646};
    BXSel = 750;
    NBXTrain = 12 ;
    BXLeading = std::vector<long>{750,1644};
    BXSpecial = std::vector<long>{11,536,750,1644}; //leading/solo bunches
    //BXSpecial = std::vector<long>{750,751,752,753,754,755,756,757,758,759,760,761,1644,1645,1646,1647,1648,1649,1650,1651,1652,1653,1654,1655}; //trains
    //BXSpecial = std::vector<long>{758,759,760,761,1652,1653,1654,1655}; //last 4 bx's
    //BXSpecial = std::vector<long>{761,1655}; //last bx
    //BXSpecial = std::vector<long>{750,761};  //first bx
    maxsbil=17;
  }
  
  cout<<"tree entries: "<<tree->GetEntries()<<endl;


  ////////////////////////////
  TH1F HBX("HBX","",3650,0.5,3650.5);
  HBX.GetYaxis()->SetTitle(TString("Total ")+detsel+" lumi");
  HBX.GetXaxis()->SetTitle("bunch id");  
  tree->Draw("bx>>HBX",TString("")+detsel+">0.1");
  BXLIST.clear(); 
  cout<<"active bcids: ";
  for(long i=1;i<=HBX.GetNbinsX();i++){
    if(HBX.GetBinContent(i)>0){
      cout<<","<<(TString("")+i).Data();
      BXLIST.push_back(i);
    }
  }
  cout<<endl;

  
  ////// General
  timeref = TString("(time-")+tmin+")";
  Time2D = new TH2F(TString("Time2D")+FILL,"",600,0,(tmax-tmin),800,0,20);

  
  //// define the time cuts to remove the step boundaries
  CUTTime = TString("(")+tmin+"<time&&time<"+tmax+")";
  TString CUTTimeStep("(");
  for(long i=0;i<TimeStep.size()-1;i++)
    CUTTimeStep = CUTTimeStep + "abs(time-"+(TimeStep[i])+")>"+timeboundary+"&&";
  CUTTimeStep = CUTTimeStep + "abs(time-"+(TimeStep[TimeStep.size()-1])+")>"+timeboundary+")";
  CUTTime = CUTTime + "&&" + CUTTimeStep;
  cout<<"Time selection: "<<CUTTime<<endl;  

  

  ////determine list lumi sections
  TH1F HNDet("HNDet","",NLS,0.5,NLS+0.5);
  tree->Draw("ls>>HNDet",CUTTime);

  lsmin=10000;
  lsmax=0;
  CUTLumis="(";
  int countls=0;
  for(long l=1;l<=NLS;l++)
    if(HNDet.GetBinContent(l)>0){
      if(countls==0) CUTLumis=CUTLumis+"ls=="+l;
      else CUTLumis=CUTLumis+"||ls=="+l;
      LSlist.push_back(l);
      countls++;
      if(lsmin>l)lsmin=l;
      if(lsmax<l)lsmax=l;
    }
  CUTLumis=CUTLumis+")";
  cout<<"cut lumis: "<<CUTLumis<<endl;

  
  ///per bcid norm factors
  for(int b=0;b<NBX;b++)
    BXCorr[b]=1;


  
}


TH2F * getLinearityHisto(TString name, TString det, TString cut="1"){
  TH2F * H = new TH2F(name,"",200,0,20,200,0.5,1.5);
  tree->Draw(det+"/"+detsel+":"+detsel+">>"+H->GetName(),CUTTime+"&&("+detsel+">1.5)"+"&&"+cut);
  H->SetLineColor(DETColor[det.Data()]);
  H->SetMarkerColor(DETColor[det.Data()]);  
  H->GetYaxis()->SetTitle(DETName[det.Data()]+" / " + DETName[detsel.Data()]);
  H->GetXaxis()->SetTitle(DETName[detsel.Data()]+"  sbil");
  return H;
}

TH2F * getLinearityHisto(TString name, TString det, std::vector<long> bxlist){
  TString cut="(";
  for(long i=0;i<bxlist.size();i++){
    if(i!=0) cut+="||";
    cut=cut+"bx=="+bxlist[i];
  }
  cut+=")"; 
  return getLinearityHisto(name,det,cut);
}


TH2F*  getLinearityHistoAvgLS(TString name, TString det, std::vector<long> bxlist, int type=0){

  TString CUT=CUTLumis+"&&("+detsel+">"+minsbil+")";
  
  TH2F HDet("HDet","",NBX,0.5,NBX+0.5,NLS,0.5,NLS+0.5);
  HDet.Sumw2();
  tree->Draw("ls:bx>>HDet",TString("(")+CUT+")*"+det);
  TH2F HNDet("HNDet","",NBX,0.5,NBX+0.5,NLS,0.5,NLS+0.5);
  HNDet.Sumw2();
  tree->Draw("ls:bx>>HNDet",CUT);
  HDet.Divide(&HNDet); //average the points
  
  TH2F HRef("HRef","",NBX,0.5,NBX+0.5,NLS,0.5,NLS+0.5);
  HRef.Sumw2();
  tree->Draw("ls:bx>>HRef",TString("(")+CUT+")*"+detsel);
  TH2F HNRef("HNRef","",NBX,0.5,NBX+0.5,NLS,0.5,NLS+0.5);
  HNRef.Sumw2();
  tree->Draw("ls:bx>>HNRef",CUT);
  HRef.Divide(&HNRef);


  ////apply per bcid corrections
  for(int l=1;l<=NLS;l++)
    for(int b=1;b<=NBX;b++)
      if(BXCorr[b-1]>0.)
	HDet.SetBinContent(b,l,HDet.GetBinContent(b,l)/BXCorr[b-1]);
  

  ///
  TH2F* HRatio = (TH2F*)HDet.Clone("HRatio");  //calculate detector ratio
  HRatio->Divide(&HRef);

  TH2F * HLinearity = 0;
  if(type==0){
    HLinearity = new TH2F(name,"",400,0,20,500,0.5,1.5);   //vs sbil
    HLinearity->GetXaxis()->SetTitle(DETName[detsel.Data()]+"  sbil");
    for(int l=1;l<=NLS;l++)
      for(int b=0;b<bxlist.size();b++)
	if(fabs(HRatio->GetBinContent(bxlist[b],l)-1)<0.3)
	  HLinearity->Fill(HRef.GetBinContent(bxlist[b],l), HRatio->GetBinContent(bxlist[b],l));
  }
  if(type==1){
    HLinearity = new TH2F(name,"",NLS,-0.5,NLS-0.5,500,0.5,1.5);//vs LS
    HLinearity->GetXaxis()->SetTitle("lumi section");
    HLinearity->GetXaxis()->SetRangeUser(LSlist[0],LSlist[LSlist.size()-1]); 
    for(int l=1;l<=NLS;l++)
      for(int b=0;b<bxlist.size();b++)
	if(fabs(HRatio->GetBinContent(bxlist[b],l)-1)<0.3)
	  HLinearity->Fill(l, HRatio->GetBinContent(bxlist[b],l));
  }


  
  HLinearity->GetYaxis()->SetTitle(DETName[det.Data()]+" / " + DETName[detsel.Data()]);
  delete HRatio;
  return HLinearity;
}



TGraphErrors * getLinearityGraph(TH2* H){
  TGraphErrors * G = new TGraphErrors();
  G->SetName(TString(H->GetName())+"_linearity_graph");

  const float interval=0.5;
  
  float xmin=0.;
  Double_t xavg=0.;
  Double_t yavg=0.;
  int navg=0;
  int npt=0;
  for(int x=1;x<=H->GetNbinsX();x++)
    for(int y=1;y<=H->GetNbinsY();y++){
      if(H->GetBinContent(x,y)>0){//use only non-empty bins
	//cout<<npt<<" "<<xmin<<" "<<navg<<" ("<< H->GetXaxis()->GetBinCenter(x)<<" , "<< H->GetYaxis()->GetBinCenter(y)<<")"<<endl;
	if(navg==0){
	  xmin = H->GetXaxis()->GetBinCenter(x);
	  xavg = H->GetXaxis()->GetBinCenter(x);
	  yavg = H->GetYaxis()->GetBinCenter(y);
	  navg = 1;

	}else if(H->GetXaxis()->GetBinCenter(x)<xmin+interval){
	  xavg += H->GetXaxis()->GetBinCenter(x);
	  yavg += H->GetYaxis()->GetBinCenter(y);
	  navg++;

	}else {//compute avg and reset
	  if(navg>1){
	    xavg /= navg;
	    yavg /= navg;
	    //cout<<npt<<" "<<xavg<<" "<<yavg<<" "<<xmin<<endl;
	    G->SetPoint(npt,xavg,yavg);
	    npt++;
	  }
	  
	  xmin = H->GetXaxis()->GetBinCenter(x);
	  xavg = H->GetXaxis()->GetBinCenter(x);
	  yavg = H->GetYaxis()->GetBinCenter(y);
	  navg = 1;
	}
      }
    }

  //catch last point
  if(navg>1){
    xavg /= navg;
    yavg /= navg;
    //cout<<npt<<" "<<xavg<<" "<<yavg<<" "<<xmin<<endl;
    G->SetPoint(npt,xavg,yavg);
  }
  
  
  //calculate standard deviations
  float minunc=0.001;
  xmin=0.;
  float dxavg=0.;
  float dyavg=0.;
  navg=0;
  npt=0;
  for(int x=1;x<=H->GetNbinsX();x++)
    for(int y=1;y<=H->GetNbinsY();y++){
      if(H->GetBinContent(x,y)>0){//use only non-empty bins

	if(navg==0){
	  G->GetPoint(npt,xavg,yavg);
	  xmin = H->GetXaxis()->GetBinCenter(x);
	  dxavg = pow(xavg - H->GetXaxis()->GetBinCenter(x),2);
	  dyavg = pow(yavg - H->GetYaxis()->GetBinCenter(y),2);
	  navg++;

	}else if(H->GetXaxis()->GetBinCenter(x)<xmin+interval){
	  G->GetPoint(npt,xavg,yavg);
	  dxavg += pow(xavg - H->GetXaxis()->GetBinCenter(x),2);
	  dyavg += pow(yavg - H->GetYaxis()->GetBinCenter(y),2);
	  navg++;
	}else {//compute cluster and reset
	  if(navg>1) {
	    dxavg /= navg;
	    dyavg /= navg;
	    G->SetPointError(npt,sqrt(dxavg),sqrt(dyavg)+minunc);//0.001 minimum uncertainty to avoid points with ~0 uncertainty
	    npt++;
	  }
	  
	  G->GetPoint(npt,xavg,yavg);
	  xmin = H->GetXaxis()->GetBinCenter(x);
	  dxavg = pow(xavg - H->GetXaxis()->GetBinCenter(x),2);
	  dyavg = pow(yavg - H->GetYaxis()->GetBinCenter(y),2);
	  navg = 1;
	}
      }
    }

  //catch last point
  if(navg>1){
    dxavg /= navg;
    dyavg /= navg;
    G->SetPointError(npt,sqrt(dxavg),sqrt(dyavg)+minunc);
  }
  

  
  return G;
}


TF1 * fitLinearity(TH1* H, float low=0, float high=20){
  if(H==NULL) return 0;  
  TF1 * F = new TF1(TString(H->GetName())+"_Fit","[0]+[1]*x",low,high);
  F->SetLineColor(H->GetLineColor());
  F->SetMarkerColor(H->GetMarkerColor());
  F->SetParameter(0,1);
  H->Fit(F,"NQ","N",low,high);
  return F;
}



TF1 * fitLinearity(TGraphErrors * H, float low=0, float high=20){
  if(H==NULL) return 0;  
  TF1 * F = new TF1(TString(H->GetName())+"_Fit","[0]+[1]*x",low,high);
  F->SetLineColor(H->GetLineColor());
  F->SetMarkerColor(H->GetMarkerColor());
  F->SetParameter(0,1);
  H->Fit(F,"NQ","N",low,high);
  return F;
}



void get_bx_corrections(TString det){

  for(int b=0;b<NBX;b++)
    BXCorr[b]=1;
     
  char txt[100];
  cout<<"bcid corrections: ";
  for(int j=0;j<BXLIST.size();j++){
    
      TString name=TString("Linearity_bx_det")+det+"_bx"+BXLIST[j];
      TH2F * Linearity = getLinearityHistoAvgLS(name,det,std::vector<long>{BXLIST[j]});

      if(Linearity->Integral()>0){
	TF1 * f = fitLinearity(Linearity);
	if( f->GetParError(0)>0.015 ){
	  //in case error is too large on y-intercept parameter then fit with a constant
	  delete f;
	  f =new TF1("f","[0]",0,20);
	  Linearity->Fit(f,"Q","N",0,20);
	}
	BXCorr[BXLIST[j]-1] = f->GetParameter(0);
	
	sprintf(txt,"(bx==%ld)*%.3f",BXLIST[j], f->GetParameter(0));
	cout<<txt<<",";
	delete f;
      }
      
      delete Linearity;

  }
  cout<<endl;

}


void draw_time_boundaries(float ymin, float ymax){

  line.SetLineStyle(2);
  line.SetLineColor(3);
  for(long i=0;i<TimeStep.size();i++){
    if(tmin<TimeStep[i]&&TimeStep[i]<tmax)
      line.DrawLine(TimeStep[i]-tmin,ymin,TimeStep[i]-tmin,ymax);
  }
    
}
