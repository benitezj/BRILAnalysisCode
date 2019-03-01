
std::map<TString,TString> DETName = {{"pcc","PCC"},{"hfoc","HFOC"},{"hfet","HFET"},{"plt","PLTZERO"},{"bcm","BCM1F"}};

#define NDET 5
TString DETLIST[NDET] = {"hfoc","hfet","plt","bcm","pcc"};
int detsel=0;

#define NPLT 16

//////////////
/// Fill 7358
long tmin=1540537829;  long tmax=1540538520;   ////Mu scan
//long tmin=1540541430;  long tmax=1540541830; ////Vdm Scan 

#define NTimeStep 6
long TimeStep[NTimeStep] = {1540537800+64,1540537800+140,1540537800+237,1540537800+358,1540537800+477,1540537800+643};

#define NBX 22
long BXLIST[NBX] = {750,751,752,753,754,755,756,757,758,759,760,1644,1645,1646,1647,1648,1649,1650,1651,1652,1653,1654};
//long BXLIST[NBX] = {750,751,752,753,754,755,756,757,758,759,760};
//long BXLIST[NBX] = {1644,1645,1646,1647,1648,1649,1650,1651,1652,1653,1654};
//long BXLIST[NBXLead] = {750,1644};
//long BXTrain[NBXTrain] = {751,752,753,754,755,756,757,758,759,760,1645,1646,1647,1648,1649,1650,1651,1652,1653,1654};
//long BXLIST[NBXTrain] = {760,1654};

#define BXSel 750 //for detector comparisons vs time


/////////
TString CUTTime;
TString CUTBX;
TString timeref=TString("(time-")+tmin+")";
TCanvas C;
TChain tree("lumi");
TH2F Time2D("Time2D","",200,0,tmax-tmin,250,0,25);


void plot_lumi_vstime_perbx(){//for the reference detector

  
  TH2F* Time2D_bx[NBX];
  TLegend bx_leg(0.87,0.2,1.0,0.9);
  bx_leg.SetLineColor(0);
  bx_leg.SetLineStyle(0);
  bx_leg.SetShadowColor(0);
  bx_leg.SetFillColor(0);
  bx_leg.SetFillStyle(0);

  Time2D.Scale(0);
  Time2D.GetYaxis()->SetTitle(DETName[DETLIST[detsel]]);
  Time2D.GetXaxis()->SetTitle("time  [s]");


  C.Clear();
  Time2D.Draw("hist");
  for(long i=0;i<NBX;i++){
    int color=i+1;
    Time2D_bx[i] = (TH2F*)Time2D.Clone(TString("Time2D_")+i);
    Time2D_bx[i]->SetMarkerColor(color);
    Time2D_bx[i]->SetLineColor(color);
    TString cut  = CUTTime+TString("&&(bx==")+BXLIST[i]+")";
    tree.Draw(DETLIST[detsel]+":"+timeref+">>"+Time2D_bx[i]->GetName(),cut.Data(),"histpsame");
    bx_leg.AddEntry(Time2D_bx[i],TString("bx=")+BXLIST[i],"lp");
  }
  bx_leg.Draw();
  C.Print("plot_linearity.pdf");


  /////Below separate into 4 graphs
  bx_leg.Clear();
  C.Clear();
  Time2D.Draw("hist");
  for(long i=0;i<NBX/4;i++){
    Time2D_bx[i]->Draw("histpsame");
    bx_leg.AddEntry(Time2D_bx[i],TString("bx=")+BXLIST[i],"lp");
  }
  bx_leg.Draw();
  C.Print("plot_linearity.pdf");

  bx_leg.Clear();
  C.Clear();
  Time2D.Draw("hist");
  for(long i=NBX/4;i<NBX/2;i++){
    Time2D_bx[i]->Draw("histpsame");
    bx_leg.AddEntry(Time2D_bx[i],TString("bx=")+BXLIST[i],"lp");
  }
  bx_leg.Draw();
  C.Print("plot_linearity.pdf");

  bx_leg.Clear();
  C.Clear();
  Time2D.Draw("hist");
  for(long i=NBX/2;i<3*NBX/4;i++){
    Time2D_bx[i]->Draw("histpsame");
    bx_leg.AddEntry(Time2D_bx[i],TString("bx=")+BXLIST[i],"lp");
  }
  bx_leg.Draw();
  C.Print("plot_linearity.pdf");

  bx_leg.Clear();
  C.Clear();
  Time2D.Draw("hist");
  for(long i=3*NBX/4;i<NBX;i++){
    Time2D_bx[i]->Draw("histpsame");
    bx_leg.AddEntry(Time2D_bx[i],TString("bx=")+BXLIST[i],"lp");
  }
  bx_leg.Draw();
  C.Print("plot_linearity.pdf");

}


void plot_det_vstime(){//for a selected bunch id


  C.Clear();
  C.Divide(2,3);
  for(long i=0;i<NDET;i++){
    C.cd(i+1);
    tree.Draw(DETLIST[i]+":"+timeref,CUTTime+"&&"+CUTBX);//
  }
  C.Print("plot_linearity.pdf");

  
  TH2F * Det[NDET];
  TLegend det_leg(0.87,0.2,1.0,0.9);
  det_leg.SetLineColor(0);
  det_leg.SetLineColor(0);
  det_leg.SetShadowColor(0);
  det_leg.SetFillColor(0);
  det_leg.SetFillStyle(0);
  for(long i=0;i<NDET;i++){
    TString name=TString("Det_vs_time_")+i;
    Det[i] = (TH2F*)Time2D.Clone(name); 
    Det[i]->GetYaxis()->SetTitle("sbil");
    Det[i]->GetXaxis()->SetTitle("time [s]");
    Det[i]->SetMarkerColor(i+1);
    Det[i]->SetLineColor(i+1);
    det_leg.AddEntry(Det[i],DETName[DETLIST[i].Data()],"pl");
    tree.Draw(DETLIST[i]+":"+timeref+">>"+name,CUTTime+"&&"+"bx=="+BXSel,"plsame");//
  }

  
  C.Clear();
  Time2D.Scale(0);
  Time2D.GetYaxis()->SetTitle(TString("sbil ,  bcid=")+BXSel);
  Time2D.GetXaxis()->SetTitle("time  [s]");
  Time2D.Draw("hist");
  for(long i=0;i<NDET;i++)
    Det[i]->Draw("histplsame");
  det_leg.Draw();
  C.Print("plot_linearity.pdf");
  
}


void plot_det_ratio_vstime(TString CUTBX){

  /////Draw detector ratios vs time
  TH2F * Ratio[NDET];
  TLegend ratio_leg(0.6,0.2,0.8,0.45);
  ratio_leg.SetLineColor(0);
  ratio_leg.SetLineStyle(0);
  ratio_leg.SetLineWidth(0);
  ratio_leg.SetShadowColor(0);
  ratio_leg.SetFillColor(0);
  ratio_leg.SetFillStyle(0);
  for(long i=0;i<NDET;i++){

    TString ratio=DETLIST[i]+"/"+DETLIST[detsel];
    TString name=TString("Ratio_vs_time_")+i;
    TString title=DETName[DETLIST[i].Data()]+"/"+DETName[DETLIST[detsel].Data()];

    Ratio[i] = new TH2F(name,"",100,0,tmax-tmin,100,0.7,1.3);
    Ratio[i]->GetYaxis()->SetTitle("sbil ratio");
    Ratio[i]->GetXaxis()->SetTitle("time [s]");
    Ratio[i]->SetMarkerColor(i+1);
    Ratio[i]->SetLineColor(i+1);
    
    if(i==detsel) continue;
    ratio_leg.AddEntry(Ratio[i],title,"pl");
    tree.Draw(ratio+":"+timeref+">>"+name,CUTTime+"&&"+CUTBX);
  }

  
  C.Clear();
  Ratio[detsel]->Draw("histl");
  for(long i=0;i<NDET;i++){
    if(i==detsel) continue;
    Ratio[i]->Draw("histpsame");
  }
  ratio_leg.Draw();
  C.Print("plot_linearity.pdf");

}

void plot_det_correlation(TString CUTBX){

  TH2F* Correlation[NDET];
  TProfile* Correlation_Profile[NBX];
  TF1* FCorrFit[NDET];
  for(int i=0;i<NDET;i++){
    TString name=TString("Correlation_")+DETLIST[i];
    Correlation[i] = new TH2F(name,"",100,0,20,100,0,20);
    Correlation[i]->SetMarkerColor(i+1);
    Correlation[i]->SetLineColor(i+1);
    tree.Draw(TString("")+DETLIST[i]+":"+DETLIST[detsel]+">>"+Correlation[i]->GetName(),CUTTime+"&&"+CUTBX);
    
    FCorrFit[i]=new TF1(name+"_fit","[0]+[1]*x",0,20);
    FCorrFit[i]->SetLineColor(i+1);
    Correlation[i]->Fit(FCorrFit[i],"Q","N",0,20);

    /* Correlation_Profile[i] = Correlation[i]->ProfileX(); */
    /* Correlation_Profile[i]->SetLineColor(i+1); */
    /* Correlation_Profile[i]->SetMarkerColor(i+1); */
    /* Correlation_Profile[i]->SetMarkerStyle(8); */
    /* Correlation_Profile[i]->SetMarkerSize(1.2); */
  }

  TH1F HFrame("HFrame","",1,0,20);
  HFrame.GetYaxis()->SetRangeUser(0,20);
  HFrame.GetYaxis()->SetTitle("sbil");
  HFrame.GetXaxis()->SetTitle(TString("")+DETName[DETLIST[detsel].Data()]+"  sbil");

  TLegend correlation_leg(0.5,0.17,0.87,0.45);
  correlation_leg.SetLineColor(0);
  correlation_leg.SetLineWidth(0);
  correlation_leg.SetShadowColor(0);
  correlation_leg.SetFillColor(0);
  correlation_leg.SetFillStyle(0);
    
  C.Clear();
  HFrame.Draw("hist");
  for(int i=0;i<NDET;i++){
    if(i==detsel)continue;
    Correlation[i]->Draw("psame");
    FCorrFit[i]->Draw("lsame");
    char text[100];
    sprintf(text," %s: (%.2f+/-%.2f) + (%.3f+/-%.3f)*x ",
	    DETName[DETLIST[i].Data()].Data(),
	    FCorrFit[i]->GetParameter(0),FCorrFit[i]->GetParError(0),
	    FCorrFit[i]->GetParameter(1),FCorrFit[i]->GetParError(1));
    correlation_leg.AddEntry(Correlation[i],text,"lp");
  }
  correlation_leg.Draw();
  C.Print("plot_linearity.pdf");
  
  /* C.Clear(); */
  /* HFrame.Draw("hist"); */
  /* for(int i=0;i<NDET;i++){ */
  /*   if(i==detsel)continue; */
  /*   Correlation_Profile[i]->Draw("psame"); */
  /*   FCorrFit[i]->Draw("lsame"); */
  /* } */
  /* correlation_leg.Draw(); */
  /* C.Print("plot_linearity.pdf"); */

}


void plot_det_linearity(TString CUTBX){

  ///////////// Draw detector ratios vs sbil
  TH2F* Linearity[NDET];
  TProfile* Linearity_Profile[NBX];
  TF1* FLinearityFit[NDET];
  for(int i=0;i<NDET;i++){
    TString name=TString("Linearity_det_")+DETLIST[i];
    Linearity[i] = new TH2F(name,"",200,0,20,200,0,2);
    tree.Draw(TString("")+DETLIST[i]+"/"+DETLIST[detsel]+":"+DETLIST[detsel]+">>"+Linearity[i]->GetName(),CUTTime+"&&"+CUTBX);
    Linearity[i]->SetMarkerColor(i+1);
    Linearity[i]->SetLineColor(i+1);
    FLinearityFit[i]=new TF1(name+"_fit","[0]+[1]*x",0,20);
    FLinearityFit[i]->SetLineColor(i+1);
    Linearity[i]->Fit(FLinearityFit[i],"Q","N",0,20);

    /* Linearity_Profile[i] =Linearity[i]->ProfileX(); */
    /* Linearity_Profile[i]->SetLineColor(i+1); */
    /* Linearity_Profile[i]->SetMarkerColor(i+1); */
    /* Linearity_Profile[i]->SetMarkerStyle(8); */
    /* Linearity_Profile[i]->SetMarkerSize(1.2); */

  }

  TH1F HFrameLinearity("HFrameLinearity","",1,0,20);
  HFrameLinearity.GetYaxis()->SetRangeUser(0.7,1.3);
  HFrameLinearity.GetYaxis()->SetTitle("ratio");
  HFrameLinearity.GetXaxis()->SetTitle(TString("")+DETName[DETLIST[detsel].Data()]+"  sbil");

  TLegend linearity_leg(0.20,0.17,0.5,0.4);
  linearity_leg.SetLineColor(0);
  linearity_leg.SetLineWidth(0);
  linearity_leg.SetShadowColor(0);
  linearity_leg.SetFillColor(0);
  linearity_leg.SetFillStyle(0);
  
  C.Clear();
  HFrameLinearity.Draw("hist");
  for(int i=0;i<NDET;i++){
    if(i==detsel)continue;
    Linearity[i]->Draw("psame");
    FLinearityFit[i]->Draw("lsame");

    char text[100];
    sprintf(text,"%s: (%.2f+/-%.2f) + (%.5f+/-%.5f)*x",
	    DETName[DETLIST[i].Data()].Data(),
	    FLinearityFit[i]->GetParameter(0),FLinearityFit[i]->GetParError(0),
	    FLinearityFit[i]->GetParameter(1),FLinearityFit[i]->GetParError(1));
    linearity_leg.AddEntry(Linearity[i],text,"lp");

  }
  linearity_leg.Draw();
  C.Print("plot_linearity.pdf");


  /* C.Clear(); */
  /* HFrameLinearity.Draw("hist"); */
  /* for(int i=0;i<NDET;i++){ */
  /*   if(i==detsel)continue; */
  /*   Linearity_Profile[i]->Draw("psame"); */
  /*   FLinearityFit[i]->Draw("lsame"); */
  /* } */
  /* linearity_leg.Draw(); */
  /* C.Print("plot_linearity.pdf"); */

}




void plot_det_linearity_perbx(){

  ///////////// Draw detector ratios vs sbil
  TH2F * Linearity[NDET][NBX];
  TF1 *  FLinearityFit[NDET][NBX];
  TGraphErrors * G[NDET];
  int bxcount[NDET];

  for(int i=0;i<NDET;i++){
    G[i] = new TGraphErrors();
    G[i]->SetMarkerColor(i+1);
    G[i]->SetMarkerStyle(i+1);
    G[i]->SetLineColor(i+1);
    bxcount[i]=0;
    for(int j=0;j<NBX;j++){

      TString name=TString("Linearity_bx_det")+DETLIST[i]+"_bx"+BXLIST[j];
      Linearity[i][j] = new TH2F(name,"",100,0,20,200,0,2);      
      tree.Draw(TString("")+DETLIST[i]+"/"+DETLIST[detsel]+":"+DETLIST[detsel]+">>"+Linearity[i][j]->GetName(),CUTTime+"&&bx=="+BXLIST[j]);
      FLinearityFit[i][j]=new TF1(name+"_fit","[0]+[1]*x",0,20);
      Linearity[i][j]->Fit(FLinearityFit[i][j],"Q","N",0,20);

      G[i]->SetPoint(bxcount[i],bxcount[i]+1,FLinearityFit[i][j]->GetParameter(1));
      G[i]->SetPointError(bxcount[i],0,FLinearityFit[i][j]->GetParError(1));
      bxcount[i]++;
    }
  }


  TLegend linearity_leg(0.60,0.17,0.82,0.35);
  linearity_leg.SetLineColor(0);
  linearity_leg.SetLineWidth(0);
  linearity_leg.SetShadowColor(0);
  linearity_leg.SetFillColor(0);
  linearity_leg.SetFillStyle(0);
  
  TH1F HFrameLinearity("HFrameLinearity","",1,0,NBX+1);
  HFrameLinearity.GetYaxis()->SetRangeUser(-0.01,0.01);
  HFrameLinearity.GetYaxis()->SetTitle("slope");
  HFrameLinearity.GetXaxis()->SetTitle("bcid");

  C.Clear();
  HFrameLinearity.Draw();
  for(int i=0;i<NDET;i++){
    if(i==detsel) continue;
    G[i]->Draw("pesame");
    linearity_leg.AddEntry(G[i],DETName[DETLIST[i]]+"/"+DETName[DETLIST[detsel]],"lp");
  }
  linearity_leg.Draw();
  C.Print("plot_linearity.pdf");

}


void plot_plt_linearity(TString CUTBX){

  ///////////// Draw detector ratios vs sbil
  TH2F* Linearity[NPLT];
  TF1* FLinearityFit[NPLT];
  for(int i=0;i<NPLT;i++){
    TString name=TString("Linearity_plt_")+i;
    Linearity[i] = new TH2F(name,"",100,0,20,200,0,2);
    //Linearity[i]->SetMarkerColor(i+1);
    //Linearity[i]->SetLineColor(i+1);
    tree.Draw(TString("plt_")+i+"/"+DETLIST[detsel]+":"+DETLIST[detsel]+">>"+Linearity[i]->GetName(),CUTTime+"&&"+CUTBX+"&&"+TString("abs(plt_")+i+"/"+DETLIST[detsel]+"-1)<0.7");
    FLinearityFit[i]=new TF1(name+"_fit","[0]+[1]*x",0,20);
    FLinearityFit[i]->SetLineColor(2);
    Linearity[i]->Fit(FLinearityFit[i],"Q","N",0,20);
  }

  TH1F HFrameLinearity("HFrameLinearity","",1,0,20);
  HFrameLinearity.GetYaxis()->SetRangeUser(0.4,1.6);
  HFrameLinearity.GetYaxis()->SetTitle("ratio");
  HFrameLinearity.GetXaxis()->SetTitle(TString("")+DETName[DETLIST[detsel].Data()]+"  sbil");

  TLegend linearity_leg(0.40,0.17,0.82,0.45);
  linearity_leg.SetLineColor(0);
  linearity_leg.SetLineWidth(0);
  linearity_leg.SetShadowColor(0);
  linearity_leg.SetFillColor(0);
  linearity_leg.SetFillStyle(0);
  

  TH1F HPLTSlope("HPLTChannelSlope","",NPLT,-0.5,NPLT-0.5);
  
  TText label;
  C.Clear();
  C.Divide(4,4);
  for(long i=0;i<NPLT;i++){
    C.cd(i+1);
    HFrameLinearity.Draw("hist");   
    Linearity[i]->Draw("histcolsame");
    FLinearityFit[i]->Draw("lsame");
    
    char text[100];
    sprintf(text,"%s",(TString("PLTZERO_")+i).Data());
    label.DrawTextNDC(0.25,0.95,text);
    
    sprintf(text,"p0 = %.2f+/-%.2f",FLinearityFit[i]->GetParameter(0),FLinearityFit[i]->GetParError(0));
    label.DrawTextNDC(0.4,0.3,text);
    sprintf(text,"p1 = %.5f+/-%.5f",FLinearityFit[i]->GetParameter(1),FLinearityFit[i]->GetParError(1));
    label.DrawTextNDC(0.4,0.2,text);

    if(Linearity[i]->Integral()>0){
      HPLTSlope.SetBinContent(i+1,FLinearityFit[i]->GetParameter(1));
      HPLTSlope.SetBinError(i+1,FLinearityFit[i]->GetParError(1));
    }
    
  }
  C.Print("plot_linearity.pdf");


  C.Clear();
  HPLTSlope.SetMarkerStyle(8);
  HPLTSlope.SetMarkerSize(1.3);
  HPLTSlope.GetYaxis()->SetTitle("slope");
  HPLTSlope.GetXaxis()->SetTitle("PLT channel");
  HPLTSlope.GetYaxis()->SetRangeUser(-0.02,0.02);
  HPLTSlope.Draw("histpe");
  C.Print("plot_linearity.pdf");

}
  
void plot_plt_chan_perbx(){
    
  ///per bx studies
  TH2F* LinearityBX[NPLT][NBX];
  TF1*  FLinearityFit[NPLT][NBX];
  TGraphErrors * G[NPLT];
  int bxcount[NPLT];

  for(long i=0;i<NPLT;i++){
    G[i] = new TGraphErrors();
    //G[i]->SetMarkerColor(i+1);
    //G[i]->SetMarkerStyle(i+1);
    //G[i]->SetLineColor(i+1);
    bxcount[i]=0;

    for(int j=0;j<NBX;j++){
      TString name=TString("Linearity_plt")+i+"_bx"+BXLIST[j];
      LinearityBX[i][j] = new TH2F(name,"",100,0,20,200,0,2);
      LinearityBX[i][j]->SetMarkerColor(j+1);
      tree.Draw(TString("plt_")+i+"/"+DETLIST[detsel]+":"+DETLIST[detsel]+">>"+LinearityBX[i][j]->GetName(),CUTTime+"&&bx=="+BXLIST[j]+"&&"+TString("abs(plt_")+i+"/"+DETLIST[detsel]+"-1)<0.7");

      if(LinearityBX[i][j]->Integral()>100){
	FLinearityFit[i][j]=new TF1(name+"_fit","[0]+[1]*x",0,20);
	FLinearityFit[i][j]->SetLineColor(2);
	LinearityBX[i][j]->Fit(FLinearityFit[i][j],"Q","N",0,20);
      
	G[i]->SetPoint(bxcount[i],bxcount[i]+1,FLinearityFit[i][j]->GetParameter(1));
	G[i]->SetPointError(bxcount[i],0,FLinearityFit[i][j]->GetParError(1));
	bxcount[i]++;
      }
      
    }
  }

  TH1F HFrameLinearity("HFrameLinearity","",1,0,20);
  HFrameLinearity.GetYaxis()->SetRangeUser(0.,2.);
  HFrameLinearity.GetYaxis()->SetTitle("ratio");
  HFrameLinearity.GetXaxis()->SetTitle(TString("")+DETName[DETLIST[detsel].Data()]+"  sbil");
  C.Clear();
  C.Divide(4,4);
  for(long i=0;i<NPLT;i++){
    C.cd(i+1);
    HFrameLinearity.Draw("hist");
    for(int j=0;j<NBX;j++){     
      LinearityBX[i][j]->Draw("histpsame");
    }
  }
  C.Print("plot_linearity.pdf");


  TH1F HFrameLinearityGraph("HFrameLinearityGraph","",1,0,NBX+1);
  HFrameLinearityGraph.GetYaxis()->SetRangeUser(-0.03,0.03);
  HFrameLinearityGraph.GetYaxis()->SetTitle("slope");
  HFrameLinearityGraph.GetXaxis()->SetTitle("bcid");
  C.Clear();
  C.Divide(4,4);
  for(long i=0;i<NPLT;i++){
    C.cd(i+1);
    HFrameLinearityGraph.Draw("hist");
    G[i]->Draw("pesame");
  }
  C.Print("plot_linearity.pdf");


  
  
}



void plot_linearity(){
  
  tree.Add("bril.root");  


  CUTTime = TString("(")+tmin+"<time&&time<"+tmax+")";
  TString CUTTimeStep("(");
  for(long i=0;i<NTimeStep-1;i++)
    CUTTimeStep = CUTTimeStep + "abs(time-"+(TimeStep[i])+")>23&&";
  CUTTimeStep = CUTTimeStep + "abs(time-"+(TimeStep[NTimeStep-1])+")>23)";
  CUTTime = CUTTime + "&&" + CUTTimeStep;
  cout<<"Time selection: "<<CUTTime<<endl;  

  CUTBX+="(";
  for(long i=0;i<NBX;i++){
    if(i!=0) CUTBX+="||";
    CUTBX=CUTBX+"bx=="+BXLIST[i];
  }
  CUTBX+=")";
  cout<<"BX selection: "<<CUTBX<<endl;

  
  C.Print("plot_linearity.pdf[");

  ////////   check the active bunches ////////////
  TH1F HBX("HBX","",3650,0.5,3650.5);
  HBX.GetYaxis()->SetTitle(TString("Total ")+DETLIST[detsel]+" lumi");
  HBX.GetXaxis()->SetTitle("bunch id");

  
  C.Clear();
  tree.Draw("bx>>HBX","","histpe");
  TText label;
  label.SetTextSize(0.025);
  int nactivebx=0;
  for(int i=1;i<=HBX.GetNbinsX();i++){
    if(HBX.GetBinContent(i)>0){
      label.DrawTextNDC(0.87,0.9-nactivebx*0.03,(TString("")+i).Data());
      nactivebx++;
    }
  }
  C.Print("plot_linearity.pdf");

  //////  lumi vs time per bx
  plot_lumi_vstime_perbx();

  ////// Detectors vs time, one bx
  plot_det_vstime();
  plot_det_ratio_vstime(CUTBX);
  
  ////// detector correlations
  plot_det_correlation(CUTBX);
  plot_det_linearity(CUTBX);
  plot_det_linearity_perbx();

  /////PLT channels
  plot_plt_linearity(CUTBX);
  plot_plt_chan_perbx();
  
  C.Print("plot_linearity.pdf]");
  gROOT->ProcessLine(".q");
  
}
