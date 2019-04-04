#include "config.C"


void plot_lumi_vstime_perbx(std::vector<long> bxlist){

  TLegend bx_leg(0.87,0.2,1.0,0.9);
  bx_leg.SetLineColor(0);
  bx_leg.SetLineStyle(0);
  bx_leg.SetShadowColor(0);
  bx_leg.SetFillColor(0);
  bx_leg.SetFillStyle(0);
 

  Time2D->Scale(0);
  Time2D->GetYaxis()->SetRangeUser(0,20);
  Time2D->GetYaxis()->SetTitle("sbil");
  Time2D->GetXaxis()->SetRangeUser(0,(tmax-tmin));
  Time2D->GetXaxis()->SetTitle("time  [s]");

  C.Clear();
  C.Divide(1,2);
  for(long i=0;i<NDET;i++){

    TLegend * leg=(TLegend*) bx_leg.Clone();
    TVirtualPad*p=C.cd(i+1);
    p->SetLeftMargin(0.1);

    for(long b=0;b<bxlist.size();b++){
      int color = b+1;

      TH2F* Time2D_bx = (TH2F*)Time2D->Clone(TString("Time2D_")+i+"_b"+b);
      Time2D_bx->GetYaxis()->SetTitle(DETName[DETLIST[i]]+" sbil");
      Time2D_bx->GetYaxis()->SetTitleOffset(0.7);
      Time2D_bx->SetMarkerColor(color);
      Time2D_bx->SetLineColor(color);

      //TString cut = CUTTime+TString("&&(bx==")+bxlist[b]+")";
      TString cut = CUTLumis+TString("&&(bx==")+bxlist[b]+")";
      tree.Draw(DETLIST[i]+":"+timeref+">>"+Time2D_bx->GetName(),cut.Data(),"histpsame");
      leg->AddEntry(Time2D_bx,TString("")+bxlist[b],"lp");

    }

    line.SetLineStyle(2);
    line.SetLineColor(3);
    for(long i=0;i<TimeStep.size()-1;i++)
      if(tmin<TimeStep[i]&&TimeStep[i]<tmax)
    	line.DrawLine(TimeStep[i]-tmin,0,TimeStep[i]-tmin,10);
    
    leg->Draw();
  }
  C.Print("plot_linearity.pdf");
  C.Print("plot_lumi_vstime_perbx.gif"); 
  
}


void plot_det_vstime(){

  ///draw each detector separately including all bcid's
  TH2F * Det[NDET];
  C.Clear();
  C.Divide(1,2);
  for(long i=0;i<NDET;i++){
    C.cd(i+1);
    TString name=TString("Det_vs_time_")+i;
    Det[i] = (TH2F*)Time2D->Clone(name); 
    Det[i]->GetYaxis()->SetTitle(DETName[DETLIST[i].Data()] + "  sbil");
    Det[i]->GetXaxis()->SetTitle("time [s]");
    Det[i]->SetMarkerColor(DETColor[DETLIST[i].Data()]);
    Det[i]->SetLineColor(DETColor[DETLIST[i].Data()]);
    tree.Draw(DETLIST[i]+":"+timeref+">>"+Det[i]->GetName(),CUTTime+"&&"+CUTBX);//
  }
  C.Print("plot_linearity.pdf");


  
// ///compare detectors in same plot, using only one bcid
// TH2F * Det[NDET];
// TLegend det_leg(0.87,0.2,1.0,0.9);
// det_leg.SetLineColor(0);
// det_leg.SetLineColor(0);
// det_leg.SetShadowColor(0);
// det_leg.SetFillColor(0);
// det_leg.SetFillStyle(0);
// for(long i=0;i<NDET;i++){
//   TString name=TString("Det_vs_time_")+i;
//   Det[i] = (TH2F*)Time2D->Clone(name); 
//   Det[i]->GetYaxis()->SetTitle("sbil");
//   Det[i]->GetXaxis()->SetTitle("time [s]");
//   Det[i]->SetMarkerColor(i+1);
//   Det[i]->SetLineColor(i+1);
//   
//   det_leg.AddEntry(Det[i],DETName[DETLIST[i].Data()],"pl");
//   tree.Draw(DETLIST[i]+":"+timeref+">>"+Det[i]->GetName(),CUTTime+"&&"+"bx=="+BXSel);//
// }
//
// Time2D->Scale(0);
// Time2D->GetYaxis()->SetRangeUser(0,20);
// Time2D->GetXaxis()->SetRangeUser(0,(tmax-tmin));
// Time2D->GetYaxis()->SetTitle(TString("sbil ,  bcid=")+BXSel);
// Time2D->GetXaxis()->SetTitle("time  [s]");
// C.Clear();
// Time2D->Draw("hist");
// for(long i=0;i<NDET;i++)
//   Det[i]->Draw("histplsame");
// det_leg.Draw();
// C.Print("plot_linearity.pdf");



  
}


void plot_det_ratio_vstime(TString CUTBX){

  /////Draw detector ratios vs time
  TH2F * Ratio[NDET];
  TLegend ratio_leg(0.6,0.6,0.8,0.85);
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

    Ratio[i] = new TH2F(name,"",100,0,tmax-tmin,100,0.8,1.2);
    Ratio[i]->GetYaxis()->SetTitle(DETName[DETLIST[i].Data()] + " / " + DETName[DETLIST[detsel].Data()] );
    Ratio[i]->GetXaxis()->SetTitle("time [s]");
    Ratio[i]->SetMarkerColor(DETColor[DETLIST[i].Data()]);
    Ratio[i]->SetLineColor(DETColor[DETLIST[i].Data()]);
    
    if(i==detsel) continue;
    ratio_leg.AddEntry(Ratio[i],title,"pl");
    tree.Draw(ratio+":"+timeref+">>"+name,CUTTime+"&&"+CUTBX);
  }


  int counter=0;
  C.Clear();
  //C.Divide(2,2);
  for(long i=0;i<NDET;i++){
    if(i==detsel) continue;
    //C.cd(++counter);
    Ratio[i]->Draw("histp");
  }
  //ratio_leg.Draw();
  C.Print("plot_linearity.pdf");

}

void plot_det_correlation(TString CUTBX){
  
  TH2F* Correlation[NDET];
  TF1* FCorrFit[NDET];
  for(int i=0;i<NDET;i++){
    TString name=TString("Correlation_")+DETLIST[i];
    Correlation[i] = new TH2F(name,"",100,0.1,20,100,0.1,20);
    Correlation[i]->SetMarkerColor(DETColor[DETLIST[i].Data()]);
    Correlation[i]->SetLineColor(DETColor[DETLIST[i].Data()]);
    tree.Draw(TString("")+DETLIST[i]+":"+DETLIST[detsel]+">>"+Correlation[i]->GetName(),CUTTime+"&&"+CUTBX+"&&"+DETLIST[detsel]+">1.0");
    FCorrFit[i] = fitLinearity(Correlation[i]);
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
  

  labeltext.SetTextSize(0.045);
  int counter=0; 
  C.Clear();
  //C.Divide(2,2);
  for(int i=0;i<NDET;i++){
    if(i==detsel)continue;
    //C.cd(++counter);
    //HFrame.Draw("hist");
    Correlation[i]->GetYaxis()->SetRangeUser(0,20);
    Correlation[i]->GetYaxis()->SetTitle(DETName[DETLIST[i].Data()]+" sbil");
    Correlation[i]->GetXaxis()->SetTitle(DETName[DETLIST[detsel].Data()]+"  sbil");
    Correlation[i]->Draw("psame");
    FCorrFit[i]->Draw("lsame");
    char text[100];
    sprintf(text," y = (%.2f+/-%.2f) + (%.3f+/-%.3f)*x ",
	    FCorrFit[i]->GetParameter(0),FCorrFit[i]->GetParError(0),
	    FCorrFit[i]->GetParameter(1),FCorrFit[i]->GetParError(1));
    //correlation_leg.AddEntry(Correlation[i],text,"lp");
    labeltext.DrawTextNDC(0.3,0.2,text);
  }
  //correlation_leg.Draw();
  C.Print("plot_linearity.pdf");
 	     
}


void plot_det_linearity(std::vector<long> bxlist){

  ///////////// Draw detector ratios vs sbil
  TH2F* Linearity[NDET];
  TF1* FLinearityFit[NDET];
  
  for(int i=0;i<NDET;i++){
    TString name=TString("Linearity_det_")+DETLIST[i];
    //Linearity[i] = getLinearityHisto(name,DETLIST[i],bxlist);
    Linearity[i] = getLinearityHistoAvgLS(name,DETLIST[i],BXSpecial); 
    FLinearityFit[i] = fitLinearity(Linearity[i]);
  }
  
  int counter=0; 
  C.Clear();
  //C.Divide(2,2);
  for(int i=0;i<NDET;i++){
    if(i==detsel)continue;
    //C.cd(++counter);
    Linearity[i]->GetYaxis()->SetRangeUser(0.8,1.2);
    Linearity[i]->Draw("p");
    FLinearityFit[i]->Draw("lsame");

    char text[100];
    sprintf(text,"slope = %.2f +/- %.2f",
	    100*FLinearityFit[i]->GetParameter(1),100*FLinearityFit[i]->GetParError(1));
    //linearity_leg.AddEntry(Linearity[i],TString(text)+" %","lp");
    labeltext.DrawTextNDC(0.4,0.2, TString(text) + " %");
  }
  //linearity_leg.Draw();
  C.Print("plot_linearity.pdf");
  C.Print("plot_det_linearity.gif"); 


}




void plot_det_linearity_perbx(std::vector<long> bxlist){

  
  ///////////// Draw detector ratios vs sbil
  TH2F * Linearity[NDET][NBX];
  TF1 *  FLinearityFit[NDET][NBX];
  TGraphErrors * Gp1[NDET];
  TGraphErrors * Gp0[NDET];
  int bxcount[NDET];
  
  for(int i=0;i<NDET;i++){
    
    Gp1[i] = new TGraphErrors();
    Gp1[i]->SetMarkerColor(DETColor[DETLIST[i].Data()]);
    Gp1[i]->SetLineColor(DETColor[DETLIST[i].Data()]);
    Gp1[i]->SetMarkerStyle(1);

    
    Gp0[i] = new TGraphErrors();
    Gp0[i]->SetMarkerColor(DETColor[DETLIST[i].Data()]);
    Gp0[i]->SetLineColor(DETColor[DETLIST[i].Data()]);
    Gp0[i]->SetMarkerStyle(1);

    bxcount[i]=0;

    for(int j=0;j<bxlist.size();j++){     
      TString name=TString("Linearity_bx_det")+DETLIST[i]+"_bx"+bxlist[j];

      //Linearity[i] = getLinearityHisto(name,DETLIST[i],TString("bx==")+bxlist[j]);
      Linearity[i][j] = getLinearityHistoAvgLS(name,DETLIST[i],std::vector<long>{bxlist[j]});
      
      FLinearityFit[i][j] = fitLinearity(Linearity[i][j]);
      Gp1[i]->SetPoint(bxcount[i],bxcount[i]+1,FLinearityFit[i][j]->GetParameter(1));
      Gp1[i]->SetPointError(bxcount[i],0,FLinearityFit[i][j]->GetParError(1));

      Gp0[i]->SetPoint(bxcount[i],bxcount[i]+1,FLinearityFit[i][j]->GetParameter(0));
      Gp0[i]->SetPointError(bxcount[i],0,FLinearityFit[i][j]->GetParError(0));


      bxcount[i]++;      
    }

  }


  ///show the individual bcids
  for(int i=0;i<NDET;i++){
    if(i==detsel) continue;
    C.Clear();
    C.Divide(2,3);
    int bxcounter=0; 
    for(int j=0;j<bxlist.size();j++){     
      C.cd(++bxcounter);
      Linearity[i][j]->GetYaxis()->SetRangeUser(0.8,1.2);
      Linearity[i][j]->Draw("histp");
      FLinearityFit[i][j]->Draw("lsame");
    }
    C.Print("plot_linearity.pdf");
  }
  
  

  
  ////plot the slope vs bcid
  TLegend linearity_leg(0.60,0.17,0.85,0.35);
  //TLegend linearity_leg(0.87,0.6,1.0,0.9);
  linearity_leg.SetLineColor(0);
  linearity_leg.SetLineWidth(0);
  linearity_leg.SetShadowColor(0);
  linearity_leg.SetFillColor(0);
  linearity_leg.SetFillStyle(0);
  
  TH1F HFrameLinearity("HFrameLinearity","",1,0,bxlist.size()+1);

  
  HFrameLinearity.GetYaxis()->SetRangeUser(-0.02,0.02);
  HFrameLinearity.GetYaxis()->SetTitle(TString(" slope relative to ")+DETName[DETLIST[detsel]]);
  HFrameLinearity.GetXaxis()->SetTitle("bcid");
  C.Clear();
  for(int i=0;i<NDET;i++){
    if(i==detsel) continue;
    HFrameLinearity.SetTitle(DETName[DETLIST[i]]);
    HFrameLinearity.Draw();
    Gp1[i]->Draw("pesame");
    //line.DrawLine(0,1,HFrameLinearity.GetXaxis()->GetXmax(),1);
  }
  C.Print("plot_linearity.pdf");
  C.Print("plot_det_linearity_perbx.gif"); 

  
  HFrameLinearity.GetYaxis()->SetRangeUser(0.95,1.05);
  HFrameLinearity.GetYaxis()->SetTitle(TString(" y-intercept relative to ")+DETName[DETLIST[detsel]]);
  HFrameLinearity.GetXaxis()->SetTitle("bcid");
  C.Clear();
  for(int i=0;i<NDET;i++){
    if(i==detsel) continue;
    HFrameLinearity.SetTitle(DETName[DETLIST[i]]);
    HFrameLinearity.Draw();
    Gp0[i]->Draw("pesame");
    line.DrawLine(0,1,HFrameLinearity.GetXaxis()->GetXmax(),1);
  }
  C.Print("plot_linearity.pdf");

  
}

void plot_det_linearity_pertrain(){

  
  ///////////// Draw detector ratios vs sbil
  TH2F * Linearity[NDET];//[NBXTrain];
  TF1 *  FLinearityFit[NDET];//[NBXTrain];
  TGraphErrors * Gp1[NDET];
  int bxcount[NDET];
  
  for(int i=0;i<NDET;i++){
    Gp1[i] = new TGraphErrors();
    Gp1[i]->SetMarkerColor(DETColor[DETLIST[i].Data()]);
    Gp1[i]->SetLineColor(DETColor[DETLIST[i].Data()]);
    Gp1[i]->SetMarkerStyle(1);//i+1);
    bxcount[i]=0;

    for(int j=0;j<NBXTrain;j++){
      TString cutbx=TString("(bx==")+(BXLeading[0]+j);
      for(int t=1;t<BXLeading.size();t++){
	cutbx=cutbx+"||bx=="+(BXLeading[t]+j);	
      }
      cutbx=cutbx+")";

      TString name=TString("Linearity_bx_det")+DETLIST[i]+"_bx"+j;
      Linearity[i] = getLinearityHisto(name,DETLIST[i],cutbx);
      FLinearityFit[i] = fitLinearity(Linearity[i]);
      Gp1[i]->SetPoint(bxcount[i],bxcount[i]+1,FLinearityFit[i]->GetParameter(1));
      Gp1[i]->SetPointError(bxcount[i],0,FLinearityFit[i]->GetParError(1));
      bxcount[i]++;      

    }
  }


  TLegend linearity_leg(0.60,0.17,0.85,0.35);
  linearity_leg.SetLineColor(0);
  linearity_leg.SetLineWidth(0);
  linearity_leg.SetShadowColor(0);
  linearity_leg.SetFillColor(0);
  linearity_leg.SetFillStyle(0);
  
  TH1F HFrameLinearity("HFrameLinearity","",1,0,NBXTrain+1);
  HFrameLinearity.GetYaxis()->SetRangeUser(-0.02,0.02);
  HFrameLinearity.GetYaxis()->SetTitle(TString(" slope relative to ")+DETName[DETLIST[detsel]]);
  HFrameLinearity.GetXaxis()->SetTitle("bcid");

  C.Clear();
  HFrameLinearity.Draw();
  for(int i=0;i<NDET;i++){
    if(i==detsel) continue;
    Gp1[i]->Draw("pesame");
    linearity_leg.AddEntry(Gp1[i],DETName[DETLIST[i]],"lp");
  }
  linearity_leg.Draw();
  labeltext.DrawTextNDC(0.7,0.85,TString("Fill ")+FILL);
  C.Print("plot_linearity.pdf");

}


void plot_plt_linearity(TString CUTBX){

  ///////////// Draw detector ratios vs sbil
  TH2F* Linearity[NPLT];
  TF1* FLinearityFit[NPLT];
  for(int i=0;i<NPLT;i++){
    TString name=TString("Linearity_plt_")+i;
    //Linearity[i] = new TH2F(name,"",100,0,20,200,0,2);
    //Linearity[i]->SetLineColor(2);
    //tree.Draw(TString("plt_")+i+"/"+DETLIST[detsel]+":"+DETLIST[detsel]+">>"+Linearity[i]->GetName(),CUTTime+"&&"+CUTBX+"&&"+TString("abs(plt_")+i+"/"+DETLIST[detsel]+"-1)<0.7"+"&&"+DETLIST[detsel]+">1.0");

    Linearity[i] = getLinearityHisto(name,TString("plt_")+i,CUTBX);
    if(Linearity[i]->Integral()>10)
      FLinearityFit[i] = fitLinearity(Linearity[i]);
  }

  TH1F HFrameLinearity("HFrameLinearity","",1,0,20);
  HFrameLinearity.GetYaxis()->SetRangeUser(0.4,1.6);
  HFrameLinearity.GetYaxis()->SetTitle("ratio");
  HFrameLinearity.GetXaxis()->SetTitle(TString("")+DETName[DETLIST[detsel].Data()]+"  sbil");
  HFrameLinearity.GetYaxis()->SetTitleSize(0.07);
  HFrameLinearity.GetXaxis()->SetTitleSize(0.07);
  HFrameLinearity.GetYaxis()->SetTitleOffset(0.8);
  HFrameLinearity.GetXaxis()->SetTitleOffset(0.7);
  
  TLegend linearity_leg(0.40,0.17,0.82,0.45);
  linearity_leg.SetLineColor(0);
  linearity_leg.SetLineWidth(0);
  linearity_leg.SetShadowColor(0);
  linearity_leg.SetFillColor(0);
  linearity_leg.SetFillStyle(0);
  

  TGraphErrors HPLTSlope;
  int npts=0;
  
  TText label;
  C.Clear();
  C.Divide(4,4);
  for(long i=0;i<NPLT;i++){
    C.cd(i+1);
    if(Linearity[i]->Integral()<10) continue;

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

    HPLTSlope.SetPoint(npts,i+1,FLinearityFit[i]->GetParameter(1));
    HPLTSlope.SetPointError(npts,0,FLinearityFit[i]->GetParError(1));
    npts++;
    
  }
  C.Print("plot_linearity.pdf");


  C.Clear();
  HPLTSlope.SetMarkerStyle(8);
  HPLTSlope.SetMarkerSize(1.3);
  HPLTSlope.GetYaxis()->SetTitle("slope");
  HPLTSlope.GetXaxis()->SetTitle("PLT channel");
  HPLTSlope.GetYaxis()->SetRangeUser(-0.02,0.02);
  HPLTSlope.Draw("ape");
  C.Print("plot_linearity.pdf");

}
  
void plot_plt_chan_perbx(){
    
  TH2F* LinearityBX[NPLT];   //[NBX];
  TF1*  FLinearityFit[NPLT]; //[NBX];
  TGraphErrors * G0[NPLT];
  TGraphErrors * G1[NPLT];
  int bxcount[NPLT];

  for(long i=0;i<NPLT;i++){
    G0[i] = new TGraphErrors();
    G1[i] = new TGraphErrors();
    bxcount[i]=0;

    TString result_slope;
    char result_txt[100];
    
    ///this algorithm averages the trains
    for(int j=0;j<NBXTrain;j++){

      TString cutbx=TString("(bx==")+BXLeading[0]+j;
      for(int t=1;t<BXLeading.size();t++){
	cutbx=cutbx+"||bx=="+(BXLeading[t]+j);
      }
      cutbx+=")";

      TString name=TString("Linearity_plt")+i+"_bx"+j;

      //LinearityBX[i] = new TH2F(name,"",100,0,20,200,0.5,1.5);
      //LinearityBX[i]->SetMarkerColor(j+1);
      //LinearityBX[i]->SetLineColor(2);
      //tree.Draw(TString("plt_")+i+"/"+DETLIST[detsel]+":"+DETLIST[detsel]+">>"+LinearityBX[i]->GetName(),CUTTime+"&&"+cutbx+"&&"+TString("abs(plt_")+i+"/"+DETLIST[detsel]+"-1)<0.7"+"&&"+DETLIST[detsel]+">1.0");
      
      LinearityBX[i] = getLinearityHisto(name,TString("plt_")+i,cutbx);
      
      if(LinearityBX[i]->Integral()>10){
	
	FLinearityFit[i] = fitLinearity(LinearityBX[i]);
	
	G0[i]->SetPoint(bxcount[i],bxcount[i]+1,FLinearityFit[i]->GetParameter(0));
	G0[i]->SetPointError(bxcount[i],0,FLinearityFit[i]->GetParError(0));

	G1[i]->SetPoint(bxcount[i],bxcount[i]+1,FLinearityFit[i]->GetParameter(1));
	G1[i]->SetPointError(bxcount[i],0,FLinearityFit[i]->GetParError(1));

	sprintf(result_txt,"%1.2f",100*FLinearityFit[i]->GetParameter(1));
	if(j==0||j==4) result_slope += TString(result_txt)+"   ";
	
	bxcount[i]++;
      }
      
    }
    cout<<i<<" : "<<result_slope<<endl;   
  }


  TH1F HFrameLinearityGraph("HFrameLinearityGraph","",1,0,NBXTrain+1);
  HFrameLinearityGraph.GetYaxis()->SetRangeUser(-0.03,0.03);
  HFrameLinearityGraph.GetYaxis()->SetTitle("slope");
  HFrameLinearityGraph.GetXaxis()->SetTitle("bcid");
  HFrameLinearityGraph.GetYaxis()->SetTitleSize(0.07);
  HFrameLinearityGraph.GetXaxis()->SetTitleSize(0.07);
  HFrameLinearityGraph.GetYaxis()->SetTitleOffset(0.8);
  HFrameLinearityGraph.GetXaxis()->SetTitleOffset(0.7);
 
  C.Clear();
  C.Divide(4,4);
  for(long i=0;i<NPLT;i++){
    C.cd(i+1);
    HFrameLinearityGraph.Draw("hist");
    G1[i]->SetMarkerColor(2);
    G1[i]->SetLineColor(2);
    G1[i]->SetMarkerStyle(20);
    G1[i]->SetMarkerSize(0.5);
    G1[i]->Draw("psame");
  }
  C.Print("plot_linearity.pdf");


  
}



void plot_linearity(long fill=7358){
 
  configFill(fill);
   
  
  C.Print("plot_linearity.pdf[");

  ////////   check the active bunches ////////////
  TH1F HBX("HBX","",3650,0.5,3650.5);
  HBX.GetYaxis()->SetTitle(TString("Total ")+DETLIST[detsel]+" lumi");
  HBX.GetXaxis()->SetTitle("bunch id");  
  C.Clear();
  tree.Draw("bx>>HBX",TString("")+DETLIST[detsel]+">0.1","histpe");
  TText label;
  label.SetTextSize(0.01);
  int nactivebx=0;
  cout<<"active bcids: ";
  for(int i=1;i<=HBX.GetNbinsX();i++){
    if(HBX.GetBinContent(i)>0){
      label.DrawTextNDC(0.87,1.0-nactivebx*0.01,(TString("")+i).Data());
      nactivebx++;
      cout<<","<<(TString("")+i).Data();
    }
  }
  //C.Print("plot_linearity.pdf");
  cout<<endl;
  
  ////// show each bcid vs time
  plot_lumi_vstime_perbx(BXSpecial);
  //plot_lumi_vstime_perbx(BXSel);

  ////// Detectors vs time
  //plot_det_vstime(); //selected bcid's
  //plot_det_ratio_vstime(CUTBX);//all bcid
  
  ////// Detector correlations
  //plot_det_correlation(CUTBX);
  //plot_det_linearity(BXLIST);
  //plot_det_linearity(BXLeading);
  plot_det_linearity(BXSpecial);
  //plot_det_linearity_perbx(BXLIST);
  //plot_det_linearity_perbx(BXLeading);
  plot_det_linearity_perbx(BXSpecial);
  //plot_det_linearity_pertrain();

 
  /////  PLT channels
  //plot_plt_linearity(CUTBX);
  //plot_plt_chan_perbx();

  
  C.Print("plot_linearity.pdf]");
  gROOT->ProcessLine(".q");
  
}
