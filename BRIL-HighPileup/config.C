
////////////
std::map<TString,TString> DETName = {{"hfoc","HFOC"},{"hfet","HFET"},{"plt","PLTZERO"},{"bcm","BCM1F"},{"pcc","PCC"}};
std::map<TString,int> DETColor = {{"hfoc",1},{"hfet",2},{"plt",3},{"bcm",4},{"pcc",6}};//note color should be set with +1
#define NDET 2
TString DETLIST[NDET] = {"hfoc","pcc"};//"hfet","plt","bcm",
int detsel=0;
#define NPLT 16
#define NBX 3564
#define NLS 500


long FILL;
std::vector<long> BXLIST;
std::vector<long> BXLeading;
std::vector<long> BXSpecial;
std::vector<long> TimeStep;
std::vector<long> BXSel;
long NBXTrain;
TString CUTBX;
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


/////////////////////////////////////////////////
void configFill(long fill=0){///set fill specific configurations
  FILL=fill;
 
  if(FILL==6847){
    tree.Add("bril_6847_RunDModVeto_NoCorr.root");
    tmin = 1530010610 ;
    //tmax = tmin + 400 ;
    //tmax = tmin + 790 ;
    tmax = tmin + 1770;
    BXSel = std::vector<long>{686};
    NBXTrain = 1;
    BXLeading = std::vector<long>{686,816,2591,2612,2633};
    //TimeStep = std::vector<long>{tmin+5,tmin+60,tmin+111,tmin+162,tmin+214,tmin+265,tmin+316,tmin+368,tmin+421,tmin+472,tmin+523,tmin+574,tmin+625,tmin+677,tmin+729,tmin+783};
    TimeStep = std::vector<long>{tmin+5,tmin+60,tmin+111,tmin+162,tmin+214,tmin+265,tmin+316,tmin+368,tmin+421,tmin+472,tmin+523,tmin+574,tmin+625,tmin+677,tmin+729,tmin+783,tmin+831,tmin+879,tmin+927,tmin+975,tmin+1023,tmin+1071,tmin+1119,tmin+1151,tmin+1201,tmin+1253,tmin+1304,tmin+1356,tmin+1409,tmin+1460,tmin+1512,tmin+1563,tmin+1615,tmin+1667,tmin+1718};
    BXSpecial = std::vector<long>{686,816,2591,2612,2633};//high stats bunches
  }
  if(FILL==6854){
    tree.Add("bril_6854_RunDModVeto_NoCorr.root");
    tmin = 1530139020 ;
    tmax = tmin + 770 ;
    BXSel = std::vector<long>{1631};
    NBXTrain = 10;
    //BXLeading = std::vector<long>{62,149,443,1043,1337,1631,1937,2231,2831,312};
    BXLeading = std::vector<long>{1631};
    TimeStep = std::vector<long>{tmin+41,tmin+102,tmin+153,tmin+205,tmin+255,tmin+307,tmin+358,tmin+410,tmin+461,tmin+514,tmin+565,tmin+617,tmin+668,tmin+720};
    //BXSpecial = std::vector<long>{1631,1651,1678,2321,2355};//high stats bunches
    BXSpecial = std::vector<long>{1631};//high stats leading bunches
  }
  if(FILL==7274){
    tree.Add("bril_7274.root");
    tmin = 1539215350;
    tmax = tmin + 2000;
    BXSel = std::vector<long>{62};
    NBXTrain = 10;
    BXLeading = std::vector<long>{62,196,385,574,767,901,1090,1279,1468,1661,1795,1984,2173,2362,2555,2689};
    TimeStep = std::vector<long>{tmin+195,tmin+271,tmin+345,tmin+421,tmin+518,tmin+614,tmin+760,tmin+999,tmin+1144,tmin+1241,tmin+1340,tmin+1415,tmin+1490,tmin+1564};
  }
  if(FILL==7358){
    tree.Add("bril_7358_RunDModVeto_NoCorr.root");
    tmin = 1540537829;
    tmax = tmin + 690;
    BXSel = std::vector<long>{750};
    NBXTrain = 10 ;
    BXLeading = std::vector<long>{750,1644};
    TimeStep = std::vector<long>{1540537800+64,1540537800+140,1540537800+237,1540537800+358,1540537800+477,1540537800+646};
    BXSpecial = std::vector<long>{11,536,750,1644};//leading/solo bunches
  }

  
  //// General
  timeref=TString("(time-")+tmin+")";
    
  Time2D=new TH2F(TString("Time2D")+FILL,"",600,0,tmax-tmin,800,0,20);
  
  for(int i=0;i<BXLeading.size();i++)
    for(int j=0;j<NBXTrain;j++)
      BXLIST.push_back(BXLeading[i] + j);

  
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


}


TH2F * getLinearityHisto(TString name, TString det, TString cut="1"){
  TH2F * H = new TH2F(name,"",200,0,20,200,0.5,1.5);
  tree.Draw(det+"/"+DETLIST[detsel]+":"+DETLIST[detsel]+">>"+H->GetName(),CUTTime+"&&("+DETLIST[detsel]+">1.5)"+"&&"+cut);
  H->SetLineColor(DETColor[det.Data()]);
  H->SetMarkerColor(DETColor[det.Data()]);  
  H->GetYaxis()->SetTitle(DETName[det.Data()]+" / " + DETName[DETLIST[detsel].Data()]);
  H->GetXaxis()->SetTitle(DETName[DETLIST[detsel].Data()]+"  sbil");
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
  tree.Draw("ls:bx>>HDet",TString("(")+CUTLumis+"&&("+DETLIST[detsel]+">1.5)"+")*"+det);
  TH2F HNDet("HNDet","",NBX,0.5,NBX+0.5,NLS,0.5,NLS+0.5);
  HNDet.Sumw2();
  tree.Draw("ls:bx>>HNDet",CUTLumis+"&&("+DETLIST[detsel]+">1.5)");

  
  TH2F HRef("HRef","",NBX,0.5,NBX+0.5,NLS,0.5,NLS+0.5);
  HRef.Sumw2();
  tree.Draw("ls:bx>>HRef",TString("(")+CUTLumis+"&&("+DETLIST[detsel]+">1.5)"+")*"+DETLIST[detsel]);
  TH2F HNRef("HNRef","",NBX,0.5,NBX+0.5,NLS,0.5,NLS+0.5);
  HNRef.Sumw2();
  tree.Draw("ls:bx>>HNRef",CUTLumis+"&&("+DETLIST[detsel]+">1.5)");

  
  HDet.Divide(&HNDet);//average the points
  HRef.Divide(&HNRef);
  TH2F* HRatio = (TH2F*)HDet.Clone("HRatio");//calculate detector ratio
  HRatio->Divide(&HRef);

  //TGraphErrors HLinearity; //("HLinearity","",200,0,20,200,0.5,1.5);
  TH2F * HLinearity = new TH2F(name,"",400,0,20,200,0.5,1.5);

  //int counter=0;
  for(int l=1;l<=NLS;l++)
    for(int b=0;b<bxlist.size();b++){
      if(HRatio->GetBinContent(bxlist[b],l)>0.1){
	//HLinearity.SetPoint(counter++,HRef.GetBinContent(bxlist[b],l),HRatio->GetBinContent(bxlist[b],l));
	HLinearity->Fill(HRef.GetBinContent(bxlist[b],l),HRatio->GetBinContent(bxlist[b],l));
      }
    }
  delete HRatio;

  HLinearity->GetYaxis()->SetTitle(DETName[det.Data()]+" / " + DETName[DETLIST[detsel].Data()]);
  HLinearity->GetXaxis()->SetTitle(DETName[DETLIST[detsel].Data()]+"  sbil");
  return HLinearity;
}
  

TF1 * fitLinearity(TH2F* H){
  if(H==NULL) return 0;  
  TF1 * F = new TF1(TString(H->GetName())+"_Fit","[0]+[1]*x",0,20);
  F->SetLineColor(H->GetLineColor());
  F->SetMarkerColor(H->GetMarkerColor());
  H->Fit(F,"Q","N",0,20);
  return F;
}

