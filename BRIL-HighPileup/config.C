
////////////
std::map<TString,TString> DETName = {{"hfoc","HFOC"},{"hfet","HFET"},{"plt","PLTZERO"},{"bcm","BCM1F"},{"pcc","PCC"},{"pccb","PCC_B"},{"pccf","PCC_F"}};
std::map<TString,int> DETColor = {{"hfoc",1},{"hfet",2},{"plt",3},{"bcm",4},{"pcc",6},{"pccb",6},{"pccf",6}};//note color should be set with +1
#define NDET 2
TString DETLIST[NDET] = {"hfoc","pcc"};//"hfet","plt","bcm",
TString detsel("hfoc");
#define NPLT 16
#define NBX 3564
#define NLS 500


long FILL;
std::vector<long> BXLIST;
std::vector<long> BXLeading;
std::vector<long> BXTrain;
std::vector<long> BXSpecial;
std::vector<long> BXSpecialTrain;
std::vector<long> BXSel;
long NBXTrain;
TString CUTBX;

std::vector<long> TimeStep;
long tmin;
long tmax;
TString timeref;
TString CUTTime;
TString CUTLumis;


TCanvas C;
TChain tree("lumi");
TH2F * Time2D;
TLine line;
TText labeltext;

float BXCorr[NBX];


/////////////////////////////////////////////////
void configFill(long fill=0){///set fill specific configurations
  FILL=fill;

  cout<<"Beging Fill "<<FILL<<" configuration"<<endl;


  BXLIST.clear();
  BXLeading.clear();
  BXTrain.clear();
  BXSpecial.clear();
  BXSpecialTrain.clear();
  TimeStep.clear();
  BXSel.clear();
  
  
  if(FILL==6847){
    tree.Add("bril_6847_RunDModVeto.root");
    //tree.Add("bril_6847_RunDModVeto_NoCorr.root");
    tmin = 1530010610 ;
    //tmax = tmin + 400 ;//half a scan
    //tmax = tmin + 790 ;//one scan
    tmax = tmin + 1770;//both scans
    TimeStep = std::vector<long>{tmin+5,tmin+60,tmin+111,tmin+162,tmin+214,tmin+265,tmin+316,tmin+368,tmin+421,tmin+472,tmin+523,tmin+574,tmin+625,tmin+677,tmin+729,tmin+783,tmin+831,tmin+879,tmin+927,tmin+975,tmin+1023,tmin+1071,tmin+1119,tmin+1151,tmin+1201,tmin+1253,tmin+1304,tmin+1356,tmin+1409,tmin+1460,tmin+1512,tmin+1563,tmin+1615,tmin+1667,tmin+1718};
    BXSel = std::vector<long>{686};
    NBXTrain = 1;
    BXLeading = std::vector<long>{686,816,2591,2612,2633};
    BXSpecial = BXLeading;
  }
  if(FILL==6854){
    tree.Add("bril_6854_RunDModVeto.root");
    //tree.Add("bril_6854_RunDModVeto_NoCorr.root");
    tmin = 1530139061 ; 
    tmax = tmin + 1650 ;
    TimeStep = std::vector<long>{tmin+61,tmin+112,tmin+164,tmin+214,tmin+266,tmin+317,tmin+369,tmin+420,tmin+473,tmin+524,tmin+576,tmin+627,tmin+679,tmin+734,tmin+780,tmin+826,tmin+872,tmin+900,tmin+954,tmin+1005,tmin+1057,tmin+1108,tmin+1159,tmin+1210,tmin+1261,tmin+1315,tmin+1366,tmin+1417,tmin+1469,tmin+1521,tmin+1572,tmin+1624};
    BXSel = std::vector<long>{1631};

    //NBXTrain = 10;
    // BXLeading = std::vector<long>{62,149,443,1043,1337,1631,1937,2231,2831,312};//not avaialable for PCC

    NBXTrain = 1;
    BXLeading = std::vector<long>{1631};//not avaialable for PCC

    BXSpecial = std::vector<long>{1631};      // high stats leading bunches
    BXSpecialTrain = std::vector<long>{1651,1678,2321,3255}; //high stats train bunches
  }
  if(FILL==7274){
    tree.Add("bril_7274_RunDModVeto.root");
    tmin = 1539215450;
    tmax = tmin + 1600;
    TimeStep = std::vector<long>{tmin+95,tmin+171,tmin+245,tmin+321,tmin+418,tmin+514,tmin+660,tmin+899,tmin+1044,tmin+1141,tmin+1240,tmin+1315,tmin+1390,tmin+1464};
    BXSel = std::vector<long>{62};
    NBXTrain = 10;
    BXLeading = std::vector<long>{62,196,385,574,767,901,1090,1279,1468,1661,1795,1984,2173,2362,2555,2689};
  }
  if(FILL==7358){
    tree.Add("bril_7358_RunDModVeto.root");
    //tree.Add("bril_7358_RunDModVeto_NoCorr.root");
    //tree.Add("bril_7358_RunDModVeto_NoHFCorr.root");
    tmin = 1540537829;
    tmax = tmin + 690;
    TimeStep = std::vector<long>{1540537800+64,1540537800+140,1540537800+237,1540537800+358,1540537800+477,1540537800+646};
    BXSel = std::vector<long>{750};
    NBXTrain = 10 ;
    BXLeading = std::vector<long>{750,1644};
    BXSpecial = std::vector<long>{11,536,750,1644}; //leading/solo bunches
    //BXSpecialTrain = std::vector<long>{751, 752, 753, 754, 755, 756, 757, 758, 759, 1645, 1646, 1647, 1648, 1649, 1650, 1651, 1652, 1653}; //high stats train bunches
    BXSpecialTrain = std::vector<long>{756,757, 758, 759,  1650, 1651, 1652, 1653}; //high stats train bunches
  }



  ////////////////////////////
  /////  printActiveBunches
  TH1F HBX("HBX","",3650,0.5,3650.5);
  HBX.GetYaxis()->SetTitle(TString("Total ")+detsel+" lumi");
  HBX.GetXaxis()->SetTitle("bunch id");  
  tree.Draw("bx>>HBX",TString("")+detsel+">0.1");

  BXLIST.clear(); 
  cout<<"active bcids: ";
  for(int i=1;i<=HBX.GetNbinsX();i++){
    if(HBX.GetBinContent(i)>0){
      cout<<","<<(TString("")+i).Data();
      BXLIST.push_back(i);
    }
  }
  cout<<endl;


  /* //fill the bcid list */
  /* BXLIST.clear(); */
  /* cout<<"BX list : "; */
  /* for(int i=0;i<BXLeading.size();i++) */
  /*   for(int j=0;j<NBXTrain;j++){ */
  /*     BXLIST.push_back(BXLeading[i] + j); */
  /*     cout<<", "<<BXLeading[i] + j; */
  /*     if(j>0) BXTrain.push_back(BXLeading[i] + j); */
  /*   } */
  /* cout<<endl; */


  
  //// General
  timeref=TString("(time-")+tmin+")";
    
  Time2D=new TH2F(TString("Time2D")+FILL,"",600,0,tmax-tmin,800,0,20);

  
  //// define the time cuts to remove the step boundaries
  CUTTime = TString("(")+tmin+"<time&&time<"+tmax+")";
  
  TString CUTTimeStep("(");
  for(long i=0;i<TimeStep.size()-1;i++)
    CUTTimeStep = CUTTimeStep + "abs(time-"+(TimeStep[i])+")>24&&";
  CUTTimeStep = CUTTimeStep + "abs(time-"+(TimeStep[TimeStep.size()-1])+")>24)";

  CUTTime = CUTTime + "&&" + CUTTimeStep;
  cout<<"Time selection: "<<CUTTime<<endl;  

  

  ////determine list of PCC lumi sections
  TH1F HNDet("HNDet","",NLS,0.5,NLS+0.5);
  tree.Draw("ls>>HNDet",CUTTime);

  CUTLumis="(";
  int countls=0;
  for(long l=1;l<=NLS;l++)
    if(HNDet.GetBinContent(l)>0){
      if(countls==0) CUTLumis=CUTLumis+"ls=="+l;
      else CUTLumis=CUTLumis+"||ls=="+l;
      countls++;
    }
  CUTLumis=CUTLumis+")";
  cout<<"cut lumis: "<<CUTLumis<<endl;


  
  //// Define the cut on the active bcids
  CUTBX="(";
  for(long i=0;i<BXLIST.size();i++){
    if(i!=0) CUTBX+="||";
    CUTBX=CUTBX+"bx=="+BXLIST[i];
  }
  CUTBX+=")";
  cout<<"BX selection: "<<CUTBX<<endl;


  
  ///per bcid correction factors
  for(int b=0;b<NBX;b++)
    BXCorr[b]=1;


  
}


TH2F * getLinearityHisto(TString name, TString det, TString cut="1"){
  TH2F * H = new TH2F(name,"",200,0,20,200,0.5,1.5);
  tree.Draw(det+"/"+detsel+":"+detsel+">>"+H->GetName(),CUTTime+"&&("+detsel+">1.5)"+"&&"+cut);
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


TH2F*  getLinearityHistoAvgLS(TString name, TString det, std::vector<long> bxlist){

  TH2F HDet("HDet","",NBX,0.5,NBX+0.5,NLS,0.5,NLS+0.5);
  HDet.Sumw2();
  tree.Draw("ls:bx>>HDet",TString("(")+CUTLumis+"&&("+detsel+">1.5)"+")*"+det);
  TH2F HNDet("HNDet","",NBX,0.5,NBX+0.5,NLS,0.5,NLS+0.5);
  HNDet.Sumw2();
  tree.Draw("ls:bx>>HNDet",CUTLumis+"&&("+detsel+">1.5)");
  HDet.Divide(&HNDet);//average the points
  
  TH2F HRef("HRef","",NBX,0.5,NBX+0.5,NLS,0.5,NLS+0.5);
  HRef.Sumw2();
  tree.Draw("ls:bx>>HRef",TString("(")+CUTLumis+"&&("+detsel+">1.5)"+")*"+detsel);
  TH2F HNRef("HNRef","",NBX,0.5,NBX+0.5,NLS,0.5,NLS+0.5);
  HNRef.Sumw2();
  tree.Draw("ls:bx>>HNRef",CUTLumis+"&&("+detsel+">1.5)");
  HRef.Divide(&HNRef);


  ////apply per bcid corrections
  for(int l=1;l<=NLS;l++)
    for(int b=1;b<=NBX;b++)
      if(BXCorr[b-1]>0.) HDet.SetBinContent(b,l,HDet.GetBinContent(b,l)/BXCorr[b-1]);
  

  ///
  TH2F* HRatio = (TH2F*)HDet.Clone("HRatio");//calculate detector ratio
  HRatio->Divide(&HRef);

  TH2F * HLinearity = new TH2F(name,"",400,0,20,200,0.5,1.5);
  for(int l=1;l<=NLS;l++)
    for(int b=0;b<bxlist.size();b++){
      if(fabs(HRatio->GetBinContent(bxlist[b],l)-1)<0.05){
	HLinearity->Fill(HRef.GetBinContent(bxlist[b],l),HRatio->GetBinContent(bxlist[b],l));
      }
    }
  delete HRatio;

  
  HLinearity->GetYaxis()->SetTitle(DETName[det.Data()]+" / " + DETName[detsel.Data()]);
  HLinearity->GetXaxis()->SetTitle(DETName[detsel.Data()]+"  sbil");
  return HLinearity;
}
  

TF1 * fitLinearity(TH1* H, float low=0, float high=20){
  if(H==NULL) return 0;  
  TF1 * F = new TF1(TString(H->GetName())+"_Fit","[0]+[1]*x",low,high);
  F->SetLineColor(H->GetLineColor());
  F->SetMarkerColor(H->GetMarkerColor());
  H->Fit(F,"Q","N",low,high);
  return F;
}






void get_bx_corrections(TString det){

  
  char txt[100];
  cout<<"bcid corrections: ";
  for(int j=0;j<BXLIST.size();j++){
    
      TString name=TString("Linearity_bx_det")+det+"_bx"+BXLIST[j];
      TH2F * Linearity = getLinearityHistoAvgLS(name,det,std::vector<long>{BXLIST[j]});

      if(Linearity->Integral()>0){
	TF1 * f = fitLinearity(Linearity);
	if( f->GetParError(0)>0.015 ){
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
