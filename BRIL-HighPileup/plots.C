#include "config.C"

void plot_lumi_vstime(std::vector<TString> detlist,std::vector<long> bxlist){

  TLegend bx_leg(0.72,0.9-detlist.size()*bxlist.size()*0.06,0.95,0.9);
  bx_leg.SetLineColor(0);
  bx_leg.SetLineStyle(0);
  bx_leg.SetShadowColor(0);
  bx_leg.SetFillColor(0);
  bx_leg.SetFillStyle(0);
  bx_leg.SetBorderSize(0);


  TH2F* Time2D_det = (TH2F*)Time2D->Clone(TString("Time2D_"));
  Time2D_det->Scale(0);
  Time2D_det->GetXaxis()->SetTitle("time  [s]");
  Time2D_det->GetYaxis()->SetRangeUser(0,maxsbil);
  Time2D_det->GetYaxis()->SetTitle(" sbil   ");
  Time2D_det->GetYaxis()->SetTitleOffset(0.7);
  Time2D_det->GetYaxis()->SetNdivisions(5);


  C->Clear();
  int counter=0;
  Time2D_det->Draw("hist");
  for(long i=0;i<detlist.size();i++){
    for(long b=0;b<bxlist.size();b++){
      int color = counter+1;

      TH2F* Time2D_bx = (TH2F*)Time2D->Clone(TString("Time2D_vstime_")+i+"_b"+b);
      Time2D_bx->SetMarkerColor(color);
      Time2D_bx->SetLineColor(color);

      //TString cut = TString("(bx==")+bxlist[b]+")";               //no time/ls selection
      //TString cut = CUTTime+TString("&&(bx==")+bxlist[b]+")";     //time selection
      TString cut = CUTLumis+TString("&&(bx==")+bxlist[b]+")";      //ls selection
      
      tree->Draw(detlist[i]+":"+timeref+">>"+Time2D_bx->GetName(),cut.Data(),"histpsame");
      bx_leg.AddEntry(Time2D_bx,TString("")+DETName[detlist[i]]+",bx="+bxlist[b],"lp");
      
      counter++;
    }

  }

  draw_time_boundaries(0,maxsbil);   ///Draw the vertical lines to show the boundaries
  bx_leg.Draw();
  C->Update();
  C->Print(OUTPATH+"/plot_lumi_vstime.png");      
    
}

void plot_lumi_vsls(std::vector<TString> detlist,std::vector<long> bxlist){

  TLegend bx_leg(0.72,0.9-detlist.size()*bxlist.size()*0.06,0.95,0.9);
  bx_leg.SetLineColor(0);
  bx_leg.SetLineStyle(0);
  bx_leg.SetShadowColor(0);
  bx_leg.SetFillColor(0);
  bx_leg.SetFillStyle(0);
  bx_leg.SetBorderSize(0);


  TH1F* Time2D[NDET][NBX];
  TGraphErrors* Graph[NDET][NBX];
  int counter=0;
  for(long i=0;i<detlist.size();i++){
    for(long b=0;b<bxlist.size();b++){
      int color = counter+1;

      TString name=TString("Time2D_vsls_")+i+"_b"+b;
      Time2D[i][b] = new TH1F(name,"",lsmax-lsmin+1,lsmin-0.5,lsmax+0.5);
      Time2D[i][b]->SetMarkerColor(color);
      Time2D[i][b]->SetLineColor(color);
      Time2D[i][b]->GetXaxis()->SetTitle("lumi section");
      Time2D[i][b]->GetYaxis()->SetTitle(" sbil   ");
      Time2D[i][b]->GetYaxis()->SetTitleOffset(0.7);
      Time2D[i][b]->GetYaxis()->SetNdivisions(5);

      
      //TString cut=TString("(bx==")+bxlist[b]+")";
      TString cut=CUTLumis+"&&(bx=="+bxlist[b]+")";
      tree->Draw(TString("ls>>")+Time2D[i][b]->GetName(),TString("(")+cut.Data()+")*"+detlist[i]);

      TH1F* Time2DN = new TH1F(TString(Time2D[i][b]->GetName())+"_count","",lsmax-lsmin+1,lsmin-0.5,lsmax+0.5);
      tree->Draw(TString("ls>>")+Time2DN->GetName(),cut);

      Time2D[i][b]->Divide(Time2DN);
      counter++;

      Graph[i][b]=new TGraphErrors();
      Graph[i][b]->SetMarkerColor(color);
      Graph[i][b]->SetLineColor(color);
      Graph[i][b]->GetXaxis()->SetTitle("lumi section");
      Graph[i][b]->GetYaxis()->SetTitle(" sbil   ");
      Graph[i][b]->GetYaxis()->SetTitleOffset(0.7);
      Graph[i][b]->GetYaxis()->SetNdivisions(5);
      int pcntr=0;
      for(int l=0;l<lsmax-lsmin+1;l++){
	if(Time2D[i][b]->GetBinContent(l+1)>0){
	  Graph[i][b]->SetPoint(pcntr,Time2D[i][b]->GetBinCenter(l+1),Time2D[i][b]->GetBinContent(l+1));
	  Graph[i][b]->SetPointError(pcntr,Time2D[i][b]->GetBinWidth(l+1)/2,0);
	  pcntr++;
	}
      }
    }
  }



  C->Clear();
  for(long i=0;i<detlist.size();i++){
    for(long b=0;b<bxlist.size();b++){
      Graph[i][b]->GetYaxis()->SetRangeUser(0,maxsbil);
      Graph[i][b]->Draw((i==0&&b==0)?"ap":"psame");
      bx_leg.AddEntry(Graph[i][b],TString("")+DETName[detlist[i]]+",bx="+bxlist[b],"lp");
    }
  }	
  bx_leg.Draw();
  C->Update();
  C->Print(OUTPATH+"/plot_lumi_vsls.png");      
    
}


void plot_det_ratio_vstime(std::vector<long> bxlist,float rmin=0.9, float rmax=1.1){
  
  /////Draw detector ratios vs time
  TH2F * Ratio[NDET][NBX];
  int counter=1;
  for(long i=0;i<DETLIST.size();i++){
    for(long b=0;b<bxlist.size();b++){
      TString name=TString("Ratio_vs_time_")+i+"_"+b;
      Ratio[i][b] = new TH2F(name,"",500,0,tmax-tmin,1500,rmin,rmax);
      Ratio[i][b]->GetYaxis()->SetTitle("sbil ratio");
      Ratio[i][b]->GetXaxis()->SetTitle("time [s]");
      Ratio[i][b]->SetMarkerColor(counter);
      Ratio[i][b]->SetLineColor(counter);      

      tree->Draw(DETLIST[i]+"/"+detsel+":"+timeref+">>"+name,CUTTime+"&&"+CUTDetSelMin+"&&(bx=="+bxlist[b]+")");
      counter++;
    } 
  }



  C->Clear();
  TLegend ratio_leg(0.2,0.75,0.9,0.91);
  ratio_leg.SetLineColor(0);
  ratio_leg.SetLineStyle(0);
  ratio_leg.SetLineWidth(0);
  ratio_leg.SetShadowColor(0);
  ratio_leg.SetFillColor(0);
  ratio_leg.SetFillStyle(0);
  ratio_leg.SetNColumns(2);
  ratio_leg.SetTextSize(0.035);
 
  for(long i=0;i<DETLIST.size();i++){
    for(long b=0;b<bxlist.size();b++){
      Ratio[i][b]->Draw((i==0&&b==0)?"histp":"histpsame");
      ratio_leg.AddEntry(Ratio[i][b],DETName[DETLIST[i]]+"/"+DETName[detsel]+",bx="+bxlist[b],"l");
    }
  }
  ratio_leg.Draw();
  draw_time_boundaries(rmin,rmax);
  line.SetLineColor(1);
  line.SetLineWidth(1);
  line.DrawLine(0,1,tmax-tmin,1);
  C->Print(OUTPATH+"/plot_linearity_det_ratio_vstime.png");

}


void plot_det_ratio_vsls(std::vector<long> bxlist,float rmin=0.90,float rmax=1.10){
  
  /////Draw detector ratios vs time
  TH2F * Ratio[NDET][NBX];
  for(long i=0;i<DETLIST.size();i++){
    for(long b=0;b<bxlist.size();b++){
      TString name=TString("Ratio_vs_ls_")+i+"_"+b;
      Ratio[i][b]=getLinearityHistoAvgLS(name,DETLIST[i],std::vector<long>{bxlist[b]},1);
    }    
  }


  C->Clear();
  TLegend ratio_leg(0.2,0.75,0.9,0.91);
  ratio_leg.SetLineColor(0);
  ratio_leg.SetLineStyle(0);
  ratio_leg.SetLineWidth(0);
  ratio_leg.SetShadowColor(0);
  ratio_leg.SetFillColor(0);
  ratio_leg.SetFillStyle(0);
  ratio_leg.SetNColumns(2);
  ratio_leg.SetTextSize(0.035);
  
  int counter=1;
  int minls=10000;
  int maxls=0;
  for(long b=0;b<bxlist.size();b++){
    for(long i=0;i<DETLIST.size();i++){
         
      TString ratio = DETName[DETLIST[i]]+"/"+DETName[detsel];
      TH1D* H = Ratio[i][b]->ProfileX()->ProjectionX(TString(Ratio[i][b]->GetName())+"proj","e");
      TGraphErrors* G = new TGraphErrors();
      int n=0;
      for(int k=1;k<=NLS;k++){
	//cout<<H->GetBinCenter(k)<<" "<<H->GetBinContent(k)<<endl;
	if(H->GetBinContent(k)>0){
	  G->SetPoint(n,H->GetBinCenter(k),H->GetBinContent(k));
	  G->SetPointError(n,H->GetBinWidth(k)/2,H->GetBinError(k));
	  n++;
	  if(minls>H->GetBinCenter(k)) minls=H->GetBinCenter(k);
	  if(maxls<H->GetBinCenter(k)) maxls=H->GetBinCenter(k);
	}
      }
	
      
      G->GetYaxis()->SetTitle("sbil ratio");
      G->GetXaxis()->SetTitle(Ratio[i][b]->GetXaxis()->GetTitle());
      G->GetYaxis()->SetRangeUser(rmin,rmax);
      G->SetMarkerColor(counter);
      G->SetLineColor(counter);
      G->SetMarkerSize(0.8);
      G->SetMarkerStyle(1);
      G->Draw(counter==1?"ap":"psame");
      
      ratio_leg.AddEntry(G,ratio+","+TString("bx=")+bxlist[b],"pl");
      counter++;
    }
  }
  line.SetLineColor(1);
  line.SetLineWidth(1);
  line.DrawLine(minls,1,maxls,1);
  ratio_leg.Draw();
  C->Print(OUTPATH+"/plot_linearity_det_ratio_vsls.png");
  
}

void plot_det_correlation(TString CUTBX){
  
  TH2F* Correlation[NDET];
  TF1* FCorrFit[NDET];
  for(int i=0;i<DETLIST.size();i++){
    TString name=TString("Correlation_")+DETLIST[i];
    Correlation[i] = new TH2F(name,"",100,0.1,20,100,0.1,20);
    Correlation[i]->SetMarkerColor(DETColor[DETLIST[i].Data()]);
    Correlation[i]->SetLineColor(DETColor[DETLIST[i].Data()]);
    tree->Draw(TString("")+DETLIST[i]+":"+detsel+">>"+Correlation[i]->GetName(),CUTTime+"&&"+CUTBX+"&&"+detsel+">1.0");
    FCorrFit[i] = fitLinearity(Correlation[i]);
  }
  
  TH1F HFrame("HFrame","",1,0,20);
  HFrame.GetYaxis()->SetRangeUser(0,20);
  HFrame.GetYaxis()->SetTitle("sbil");
  HFrame.GetXaxis()->SetTitle(TString("")+DETName[detsel.Data()]+"  sbil");
  
  TLegend correlation_leg(0.5,0.17,0.87,0.45);
  correlation_leg.SetLineColor(0);
  correlation_leg.SetLineWidth(0);
  correlation_leg.SetShadowColor(0);
  correlation_leg.SetFillColor(0);
  correlation_leg.SetFillStyle(0);
  

  labeltext.SetTextSize(0.045);
  int counter=0; 
  C->Clear();
  for(int i=0;i<DETLIST.size();i++){
    if(DETLIST[i].CompareTo(detsel)==0)continue;
    Correlation[i]->GetYaxis()->SetRangeUser(0,20);
    Correlation[i]->GetYaxis()->SetTitle(DETName[DETLIST[i].Data()]+" sbil");
    Correlation[i]->GetXaxis()->SetTitle(DETName[detsel.Data()]+"  sbil");
    Correlation[i]->Draw("psame");
    FCorrFit[i]->Draw("lsame");
    char text[100];
    sprintf(text," y = (%.2f+/-%.2f) + (%.3f+/-%.3f)*x ",
	    FCorrFit[i]->GetParameter(0),FCorrFit[i]->GetParError(0),
	    FCorrFit[i]->GetParameter(1),FCorrFit[i]->GetParError(1));
    labeltext.DrawTextNDC(0.3,0.2,text);
  }
  C->Print(OUTPATH+"/plot_linearity_det_correlation.png");
 	     
}



void plot_det_linearity_perbx(std::vector<long> bxlist){
  
  ///////////// Draw detector ratios vs sbil
  TH2F * Linearity[NDET][NBX];
  TF1 *  FLinearityFit[NDET][NBX];
  TGraphErrors * Gp1[NDET];
  TGraphErrors * Gp0[NDET];
  int bxcount[NDET];

  TGraphErrors * GLinearity[NDET][NBX];

  for(int i=0;i<DETLIST.size();i++){
    if(DETLIST[i].CompareTo(detsel)==0) continue;

    //get_bx_corrections(DETLIST[i]);

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
      
      //Linearity[i][j] = getLinearityHisto(name,DETLIST[i],std::vector<long>{bxlist[j]});
      Linearity[i][j] = getLinearityHistoAvgLS(name,DETLIST[i],std::vector<long>{bxlist[j]});      

      //FLinearityFit[i][j] = fitLinearity(Linearity[i][j]);

      GLinearity[i][j] = getLinearityGraph(Linearity[i][j]);
      FLinearityFit[i][j] = fitLinearity(GLinearity[i][j]);
           
      Gp1[i]->SetPoint(bxcount[i],bxcount[i]+1,FLinearityFit[i][j]->GetParameter(1));
      Gp1[i]->SetPointError(bxcount[i],0,FLinearityFit[i][j]->GetParError(1));

      Gp0[i]->SetPoint(bxcount[i],bxcount[i]+1,FLinearityFit[i][j]->GetParameter(0));
      Gp0[i]->SetPointError(bxcount[i],0,FLinearityFit[i][j]->GetParError(0));

      cout<<DETLIST[i]<<" "<<bxlist[j]<<" "
	  <<FLinearityFit[i][j]->GetParameter(0)<<" "<<FLinearityFit[i][j]->GetParError(0)<<" "
	  <<FLinearityFit[i][j]->GetParameter(1)<<" "<<FLinearityFit[i][j]->GetParError(1)<<endl;

      bxcount[i]++;      
    }

  }


  TH1F HFrame("HFrame","",1,0,20);
  HFrame.GetYaxis()->SetRangeUser(0.9,1.1);
  HFrame.GetXaxis()->SetTitle(DETName[detsel]+"  SBIL");
  
  labeltext.SetTextSize(0.04);
  labeltext.SetTextColor(2);

  ///show the individual bcids
  for(int i=0;i<DETLIST.size();i++){
    if(DETLIST[i].CompareTo(detsel)==0) continue;

    HFrame.GetYaxis()->SetTitle(DETName[DETLIST[i]]+" / "+DETName[detsel]);
      
    int bxcounter=0; 
    for(int j=0;j<bxlist.size();j++){     
      C->Clear();
      HFrame.Draw("hist");

      //Linearity[i][j]->Draw("histpsame");      
      GLinearity[i][j]->Draw("pesame");

      labeltext.DrawTextNDC(0.3,0.8,TString("bcid ")+bxlist[j]);

      FLinearityFit[i][j]->SetLineColor(2);
      FLinearityFit[i][j]->Draw("lsame");

      char text[100];
      sprintf(text,"y-intercept = %.2f+/-%.2f",FLinearityFit[i][j]->GetParameter(0),FLinearityFit[i][j]->GetParError(0));
      labeltext.DrawTextNDC(0.3,0.3,text);
      sprintf(text,"slope = %.2f+/-%.2f %%",100*FLinearityFit[i][j]->GetParameter(1),100*FLinearityFit[i][j]->GetParError(1));
      labeltext.DrawTextNDC(0.3,0.2,text);

      C->Print(OUTPATH+"/plot_det_linearity_perbx_"+DETLIST[i]+"_"+FILL+"_"+bxlist[j]+".png");
    }
  }
  
  

  
  ////plot the slope vs bcid
  TLegend linearity_leg(0.20,0.8,0.6,0.90);
  linearity_leg.SetLineColor(0);
  linearity_leg.SetLineWidth(0);
  linearity_leg.SetShadowColor(0);
  linearity_leg.SetFillColor(0);
  linearity_leg.SetFillStyle(0);
  linearity_leg.SetNColumns(2);

  C->Clear();
  TH1F HFrameLinearity("HFrameLinearity","",1,0,1+bxlist.size());
  HFrameLinearity.GetXaxis()->SetTitle("bcid");  
  HFrameLinearity.GetYaxis()->SetRangeUser(-0.020,0.020);
  HFrameLinearity.GetYaxis()->SetTitle(" slope    ");
  //HFrameLinearity.GetXaxis()->SetLabelSize(.08);
  //  for(int j=0;j<bxlist.size();j++)
  //HFrameLinearity.GetXaxis()->SetBinLabel(j+1,TString("")+(long)bxlist[j]);
   
  HFrameLinearity.Draw();
  for(int i=0;i<DETLIST.size();i++){
    Gp1[i]->Draw("pesame");
    linearity_leg.AddEntry(Gp1[i],DETName[DETLIST[i]]+" / "+DETName[detsel]+",  Fill "+FILL,"lp");
  }
  line.SetLineStyle(1); line.SetLineColor(1);
  line.DrawLine(HFrameLinearity.GetXaxis()->GetXmin(),0,HFrameLinearity.GetXaxis()->GetXmax(),0);
  linearity_leg.Draw();
  C->Print(OUTPATH+"/plot_det_linearity_perbx_slope"+"_"+FILL+".png");
  
  C->Clear();
  HFrameLinearity.GetYaxis()->SetRangeUser(0.95,1.05);
  HFrameLinearity.GetYaxis()->SetTitle("  y-intercept   ");
  HFrameLinearity.Draw();
  for(int i=0;i<DETLIST.size();i++){
    Gp0[i]->Draw("pesame");
  }
  line.SetLineStyle(1); line.SetLineColor(1);
  line.DrawLine(0,1,HFrameLinearity.GetXaxis()->GetXmax(),1);
  linearity_leg.Draw();
  C->Print(OUTPATH+"/plot_det_linearity_perbx_y0"+"_"+FILL+".png"); 

  
}




void plot_det_linearity(std::vector<long> bxlist){

  ///////////// Draw detector ratios vs sbil
  TH2F* Linearity[NDET];
  TF1* FLinearityFit[NDET];
  
  for(int i=0;i<DETLIST.size();i++){
    TString name=TString("Linearity_det_")+DETLIST[i];
    //Linearity[i] = getLinearityHisto(name,DETLIST[i],bxlist);
    Linearity[i] = getLinearityHistoAvgLS(name,DETLIST[i],bxlist);
    FLinearityFit[i] = fitLinearity(Linearity[i]);
  }
  

  for(int i=0;i<DETLIST.size();i++){
    if(DETLIST[i].CompareTo(detsel)==0)continue;
    C->Clear();    
    Linearity[i]->GetYaxis()->SetRangeUser(0.9,1.1);
    Linearity[i]->Draw("p");
    FLinearityFit[i]->Draw("lsame");

    char text[100];
    sprintf(text,"slope = %.2f +/- %.2f",100*FLinearityFit[i]->GetParameter(1),100*FLinearityFit[i]->GetParError(1));
    labeltext.DrawTextNDC(0.4,0.2, TString(text) + " %");
    //C->Print(OUTPATH+"/plot_linearity.pdf");
    C->Print(OUTPATH+"/plot_det_linearity_"+DETLIST[i]+".png"); 
  }


}







void plot_linearity_compare(std::vector<long> bxlist1, std::vector<long> bxlist2, TString title1="", TString title2=""){

  ///////////// Draw detector ratios vs sbil
  TH2F* Linearity1[NDET];
  TF1* FLinearityFit1[NDET];

  TH2F* Linearity2[NDET];
  TF1* FLinearityFit2[NDET];

  for(int i=0;i<DETLIST.size();i++){
    TString name=TString("Linearity_det_")+DETLIST[i];

    Linearity1[i] = getLinearityHistoAvgLS(name+"1",DETLIST[i],bxlist1);
    FLinearityFit1[i] = fitLinearity(Linearity1[i]);

    Linearity2[i] = getLinearityHistoAvgLS(name+"2",DETLIST[i],bxlist2);
    FLinearityFit2[i] = fitLinearity(Linearity2[i]);

  }
  
  char text[100];
  labeltext.SetTextSize(0.035);
  for(int i=0;i<DETLIST.size();i++){

    if(DETLIST[i].CompareTo(detsel)==0)continue;

    C->Clear();    
    Linearity1[i]->GetYaxis()->SetRangeUser(0.9,1.1);
    Linearity1[i]->Draw("p");
    FLinearityFit1[i]->Draw("lsame");
    sprintf(text," slope = %.2f +/- %.2f %%  (%s) ",100*FLinearityFit1[i]->GetParameter(1),100*FLinearityFit1[i]->GetParError(1),title1.Data());
    labeltext.SetTextColor(1);
    labeltext.DrawTextNDC(0.3,0.25, TString(text));


    //Linearity2[i]->GetYaxis()->SetRangeUser(0.8,1.2);
    Linearity2[i]->SetMarkerColor(2);
    Linearity2[i]->Draw("psame");
    FLinearityFit2[i]->SetLineColor(2);
    FLinearityFit2[i]->Draw("lsame");
    sprintf(text," slope = %.2f +/- %.2f %%  (%s) ",100*FLinearityFit2[i]->GetParameter(1),100*FLinearityFit2[i]->GetParError(1),title2.Data());
    labeltext.SetTextColor(2);
    labeltext.DrawTextNDC(0.3,0.20, TString(text));

    
    //C->Print(OUTPATH+"/plot_linearity.pdf");
    C->Print(OUTPATH+"/plot_det_linearity_compare.png"); 

  }


}




void plot_det_linearity_pertrain(){

  
  ///////////// Draw detector ratios vs sbil
  TH2F * Linearity[NDET][NBXTrain];
  TF1 *  FLinearityFit[NDET][NBXTrain];
  TGraphErrors * Gp1[NDET];
  TGraphErrors * Gp0[NDET];
  int bxcount[NDET];
  
  for(int i=0;i<DETLIST.size();i++){
    if(DETLIST[i].CompareTo(detsel)==0) continue;
    
    Gp1[i] = new TGraphErrors();
    Gp1[i]->SetMarkerColor(DETColor[DETLIST[i].Data()]);
    Gp1[i]->SetLineColor(DETColor[DETLIST[i].Data()]);
    Gp1[i]->SetMarkerStyle(1);

    Gp0[i] = new TGraphErrors();
    Gp0[i]->SetMarkerColor(DETColor[DETLIST[i].Data()]);
    Gp0[i]->SetLineColor(DETColor[DETLIST[i].Data()]);
    Gp0[i]->SetMarkerStyle(1);

    bxcount[i]=0;

    C->Clear();
    C->Divide(3,3);

    for(long j=0;j<NBXTrain;j++){
      C->cd(j+1);

      //make list of bcids to average
      std::vector<long> bxlist;
      for(int t=1;t<BXLeading.size();t++)
	bxlist.push_back(BXLeading[t]+j);
      
      TString name=TString("Linearity_bx_det")+DETLIST[i]+"_bx"+j;
      //Linearity[i][j] = getLinearityHisto(name,DETLIST[i],bxlist);
      Linearity[i][j] = getLinearityHistoAvgLS(name,DETLIST[i],bxlist);      
      FLinearityFit[i][j] = fitLinearity(Linearity[i][j]);

      Gp1[i]->SetPoint(bxcount[i],bxcount[i]+1,FLinearityFit[i][j]->GetParameter(1));
      Gp1[i]->SetPointError(bxcount[i],0,FLinearityFit[i][j]->GetParError(1));
      Gp0[i]->SetPoint(bxcount[i],bxcount[i]+1,FLinearityFit[i][j]->GetParameter(0));
      Gp0[i]->SetPointError(bxcount[i],0,FLinearityFit[i][j]->GetParError(0));
      bxcount[i]++;      


      //Linearity[i][j]->Draw("histp");
      TProfile*P=Linearity[i][j]->ProfileX(TString(Linearity[i][j]->GetName())+"_profX");
      P->GetYaxis()->SetRangeUser(0.9,1.1);
      P->RebinX(10);
      P->Draw("histpe");
      FLinearityFit[i][j]->Draw("lsame");

    }
    C->Print(OUTPATH+"/plot_linearity_pertrain_perbx"+DETLIST[i]+".png");
  }


  
  TH1F HFrameLinearity("HFrameLinearity","",1,0,NBXTrain+1);
  HFrameLinearity.GetXaxis()->SetTitle("bcid");
  for(int i=0;i<DETLIST.size();i++){
    if(DETLIST[i].CompareTo(detsel)==0) continue;
    C->Clear();
    HFrameLinearity.GetYaxis()->SetRangeUser(-0.02,0.02);
    HFrameLinearity.GetYaxis()->SetTitle(DETName[DETLIST[i]] + " / "+DETName[detsel] + " slope");
    HFrameLinearity.Draw();
    Gp1[i]->Draw("pesame");
    //C->Print(OUTPATH+"/plot_linearity.pdf");
    C->Print(OUTPATH+"/plot_linearity_pertrain_slope"+DETLIST[i]+".png");

    C->Clear();
    HFrameLinearity.GetYaxis()->SetRangeUser(0.9,1.1);
    HFrameLinearity.GetYaxis()->SetTitle(DETName[DETLIST[i]] + " / "+DETName[detsel] + "  y-intercept");
    HFrameLinearity.Draw();
    Gp0[i]->Draw("pesame");
    //C->Print(OUTPATH+"/plot_linearity.pdf");
    C->Print(OUTPATH+"/plot_linearity_pertrain_y0"+DETLIST[i]+".png");
  }


}


void plot_plt_linearity(TString CUTBX){

  ///////////// Draw detector ratios vs sbil
  TH2F* Linearity[NPLT];
  TF1* FLinearityFit[NPLT];
  for(long i=0;i<NPLT;i++){
    TString name=TString("Linearity_plt_")+i;
    Linearity[i] = getLinearityHisto(name,TString("plt_")+i,CUTBX);
    if(Linearity[i]->Integral()>10)
      FLinearityFit[i] = fitLinearity(Linearity[i]);
  }

  TH1F HFrameLinearity("HFrameLinearity","",1,0,20);
  HFrameLinearity.GetYaxis()->SetRangeUser(0.4,1.6);
  HFrameLinearity.GetYaxis()->SetTitle("ratio");
  HFrameLinearity.GetXaxis()->SetTitle(TString("")+DETName[detsel.Data()]+"  sbil");
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
  C->Clear();
  C->Divide(4,4);
  for(long i=0;i<NPLT;i++){
    C->cd(i+1);
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
  //C->Print(OUTPATH+"/plot_linearity.pdf")
  C->Print(OUTPATH+"/plot_linearity_plt.png");


  C->Clear();
  HPLTSlope.SetMarkerStyle(8);
  HPLTSlope.SetMarkerSize(1.3);
  HPLTSlope.GetYaxis()->SetTitle("slope");
  HPLTSlope.GetXaxis()->SetTitle("PLT channel");
  HPLTSlope.GetYaxis()->SetRangeUser(-0.02,0.02);
  HPLTSlope.Draw("ape");
  //C->Print(OUTPATH+"/plot_linearity.pdf");
  C->Print(OUTPATH+"/plot_linearity_plt_slope");

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
    for(long j=0;j<NBXTrain;j++){

      TString cutbx=TString("(bx==")+BXLeading[0]+j;
      for(long t=1;t<BXLeading.size();t++){
	cutbx=cutbx+"||bx=="+(BXLeading[t]+j);
      }
      cutbx+=")";

      TString name=TString("Linearity_plt")+i+"_bx"+j;
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
 
  C->Clear();
  C->Divide(4,4);
  for(long i=0;i<NPLT;i++){
    C->cd(i+1);
    HFrameLinearityGraph.Draw("hist");
    G1[i]->SetMarkerColor(2);
    G1[i]->SetLineColor(2);
    G1[i]->SetMarkerStyle(20);
    G1[i]->SetMarkerSize(0.5);
    G1[i]->Draw("psame");
  }
  //C->Print(OUTPATH+"/plot_linearity.pdf");
  C->Print(OUTPATH+"/plot_linearity_plt_perchan.png");


  
}

