#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "globals.h"
#include "rootlogon.C"

int runsel=370093;
int firstLS=50;//300;
int lastLS=150;//400;

//plot ranges
float ymax=520000;
float ratiomin=0.97;
float ratiomax=1.03;
int MAXNLS = 135000;

float ymaxbx=300;
float ratiominbx=0.97;
float ratiomaxbx=1.03;
int  bxplotmin=470;
int  bxplotmax=515;


void getLumi(TString inputfile, std::map<long,float> & lumi, std::map<long,float> & lumibx){

  ifstream myfile(inputfile.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<inputfile.Data()<<std::endl;
    return;
  }

  std::string line;
  long run=0;
  long ls=0;
  long bx=0;
  float lm=0.;
  string tmp;
  int nls=0;
  while (std::getline(myfile, line)){
    std::stringstream iss(line);
    nls++;
    
    //366724 8675 82 82 1682632854 STABLE BEAMS 6800 1730.787678365 1575.919689397 30.9 PLTZERO
    //iss>>run>>tmp>>ls>>tmp>>tmp>>tmp>>tmp>>tmp>>lumi[run*10000+ls];
        
    //370093 9029 1 1 07/03/23 23 52 21 STABLE BEAMS 6800 244202.141017648 199984.641274177 31.8 PXL
    iss>>run>>tmp>>ls>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>lm;
    if(run!=runsel || ls<firstLS || ls>lastLS) continue;  

    lumi[run*10000+ls]=lm;
    //cout<<run<<"="<<ls<<":"<<lumi[run*10000+ls]<<endl;

    //now read the per bx and integrate
    iss>>tmp>>tmp>>tmp; //Rec pu DET 
    for(int b=1;b<=NBX;b++){
      iss>>bx>>lm>>tmp;//bx del rec
      if(bx!=b) continue;
      lumibx[run*10000+bx]=lumibx[run*10000+bx] + lm;
      //cout<<bx<<" "<<lm<<endl;
    }
  }

  cout<<"Done reading: "<<inputfile<<", NLS="<<nls<<endl;
}


void getLumiPCC(TString inputfile, std::map<long,float> & lumi){

  ifstream myfile(inputfile.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open ref lumi file: "<<inputfile.Data()<<std::endl;
    return;
  }

  std::string line;
  long run=0;
  long ls=0;
  string tmp;
  int nls=0;
  while (std::getline(myfile, line)){
    std::istringstream iss(line);
    std::getline(iss, tmp, ','); run = atoi(tmp.c_str());
    std::getline(iss, tmp, ','); ls = atoi(tmp.c_str());
    long idx=run*10000+ls;
    std::getline(iss, tmp, ','); lumi[idx] = atof(tmp.c_str()) / getSigmaVis(run);
    //cout<<"="<<run<<"="<<ls<<"="<<idx<<":"<<lumi[idx]<<endl;
    if(run<=100000 || run>=1000000 || ls<=0) continue;
    nls++;
  }

  cout<<"Done reading: "<<inputfile<<", NLS="<<nls<<endl;
}



void compareBrilcalcLumisPerBX(TString path, TString file1, TString file2, TString file3=""){
  rootlogon();

  std::map<long,float> lumi1;//index will be run*10000+ls
  std::map<long,float> lumi2;
  std::map<long,float> lumi3;

  std::map<long,float> bx1;//index will be run*10000+bx
  std::map<long,float> bx2;//index will be run*10000+bx
  std::map<long,float> bx3;//index will be run*10000+bx


  getLumi(path+"/"+file1,lumi1,bx1);
  getLumi(path+"/"+file2,lumi2,bx2);
  if(file3.CompareTo("")!=0)
    getLumi(path+"/"+file3,lumi3,bx3);
  

  ///create histograms
  TH1F HL1("HL1","",MAXNLS,0.5,MAXNLS+0.5);
  TH1F HL2("HL2","",MAXNLS,0.5,MAXNLS+0.5);
  TH1F HL3("HL3","",MAXNLS,0.5,MAXNLS+0.5);
  TH1F HL12R("HL12R","",MAXNLS,0.5,MAXNLS+0.5);
  TH1F HL13R("HL13R","",MAXNLS,0.5,MAXNLS+0.5);


  float maxlumi=0.;
  float totlumi=0.;
  int ls=1;
  float sbil=0;
  map<long,float>::iterator it;
  for (it = lumi1.begin(); it != lumi1.end(); it++){
    if(it->second<=0) continue;
    ls++;
    HL1.SetBinContent(ls,it->second);
    HL2.SetBinContent(ls,lumi2[it->first]);
    HL12R.SetBinContent(ls,lumi2[it->first]/it->second);
    if(file3.CompareTo("")!=0){
      HL3.SetBinContent(ls,lumi3[it->first]);
      HL13R.SetBinContent(ls,lumi3[it->first]/it->second);
    }
    if(it->second>maxlumi) maxlumi=it->second;
    totlumi+=it->second;
  }
  cout<<"Done processing ls lumi"<<endl;
  

  /////////////////////////////////////////////////////
  ///   make the lumi and ratio plots
  ///////////////////////////////////////////////////
  gStyle->SetOptStat(0);
  TCanvas C;

  TPad can_1("can_1", "can_1", 0.0, 0.5, 1.0, 0.99);
  can_1.SetTopMargin(0.08);
  can_1.SetBottomMargin(0.15);
  can_1.SetLeftMargin(0.1);
  can_1.SetRightMargin(0.05);
  can_1.SetFrameBorderMode(0);

  can_1.cd();
  HL1.GetXaxis()->SetTitle("Lumisection");
  HL1.GetYaxis()->SetTitle("Lumi");
  HL1.GetXaxis()->SetLabelSize(0.05);
  HL1.GetXaxis()->SetTitleSize(0.07);
  HL1.GetXaxis()->SetTitleOffset(0.9);
  HL1.GetYaxis()->SetLabelSize(0.04);
  HL1.GetYaxis()->SetTitleSize(0.06);
  HL1.GetYaxis()->SetTitleOffset(0.6);
  HL1.GetXaxis()->SetRangeUser(0,ls);
  HL1.GetYaxis()->SetRangeUser(0,ymax);
  HL1.GetYaxis()->SetNdivisions(5);
  HL1.SetMarkerStyle(8);
  HL1.SetMarkerSize(0.3);
  HL1.Draw("histp");

  HL2.SetMarkerStyle(8);
  HL2.SetMarkerSize(0.3);
  HL2.SetMarkerColor(2);
  HL2.Draw("histpsame");

  HL3.SetMarkerStyle(8);
  HL3.SetMarkerSize(0.3);
  HL3.SetMarkerColor(4);
  if(file3.CompareTo("")!=0)
    HL3.Draw("histpsame");

  
  TLatex text;
  text.SetTextSize(0.06);
  text.DrawLatexNDC(0.15,0.95,TString("Runs: ")+runsel);
  text.SetTextColor(1);
  text.DrawLatexNDC(0.3,0.95,TString(file1.Data()).ReplaceAll(".dat",""));
  text.SetTextColor(2);
  text.DrawLatexNDC(0.5,0.95,TString(file2.Data()).ReplaceAll(".dat",""));
  text.SetTextColor(4);
  if(file3.CompareTo("")!=0)
    text.DrawLatexNDC(0.7,0.95,TString(file3.Data()).ReplaceAll(".dat",""));


 
  ///////Ratio plot
  TPad can_2("can_2", "can_2",0.0, 0.0, 1.0, 0.5);
  can_2.SetTopMargin(0.02);
  can_2.SetBottomMargin(0.15);
  can_2.SetLeftMargin(0.1);
  can_2.SetRightMargin(0.05);
  can_2.SetFrameBorderMode(0);
  can_2.cd();

  HL12R.GetXaxis()->SetTitle(HL1.GetXaxis()->GetTitle());
  HL12R.GetYaxis()->SetTitle(TString("ratio"));
  HL12R.GetYaxis()->SetRangeUser(ratiomin,ratiomax);
  HL12R.GetXaxis()->SetRangeUser(0,ls);
  
  HL12R.GetXaxis()->SetLabelSize(0.05);
  HL12R.GetXaxis()->SetTitleSize(0.07);
  HL12R.GetXaxis()->SetTitleOffset(0.9);
  HL12R.GetYaxis()->SetLabelSize(0.04);
  HL12R.GetYaxis()->SetTitleSize(0.06);
  HL12R.GetYaxis()->SetTitleOffset(0.6);
  HL12R.GetYaxis()->SetNdivisions(10);
  HL12R.SetMarkerStyle(8);
  HL12R.SetMarkerSize(0.3);
  HL12R.SetMarkerColor(2);
  HL12R.Draw("histp");

  HL13R.SetMarkerStyle(8);
  HL13R.SetMarkerSize(0.3);
  HL13R.SetMarkerColor(4);
  if(file3.CompareTo("")!=0)
    HL13R.Draw("histpsame");

  TLine tline;
  tline.SetLineColor(1);
  tline.DrawLine(0,1,ls,1);  

  C.Clear();
  can_1.Draw();
  can_2.Draw();
  
  TString ofilename=TString("")+file1.ReplaceAll(".dat","")+"_"+TString(file2.Data()).ReplaceAll(".dat","")+"_"+TString(file3.Data()).ReplaceAll(".dat","");
  C.Print(ofilename+".png");



  //////////////////////////////////////////////////////////////////////////////////
  ////Lumi per bx
  ///////////////////////////////////////////////////////////////////////////////
  TH1F HBXL1("HBXL1","",NBX,0.5,NBX+0.5);
  TH1F HBXL2("HBXL2","",NBX,0.5,NBX+0.5);
  TH1F HBXL3("HBXL3","",NBX,0.5,NBX+0.5);
  TH1F HBXL12R("HBXL12R","",NBX,0.5,NBX+0.5);
  TH1F HBXL13R("HBXL13R","",NBX,0.5,NBX+0.5);

  //histograms of ratios for leading and next-to-leading bx
  TH1F HBXL12RDL("HBXL12RDL","",40,ratiominbx,ratiomaxbx);
  TH1F HBXL13RDL("HBXL13RDL","",40,ratiominbx,ratiomaxbx);
  TH1F HBXL12RDNL("HBXL12RDNL","",40,ratiominbx,ratiomaxbx);
  TH1F HBXL13RDNL("HBXL13RDNL","",40,ratiominbx,ratiomaxbx);

  //first 30 bcids of train, averaged over all trains
  TH1F HBXL12RT("HBXL12RT","",30,0.5,30+0.5);
  TH1F HBXL12RTN("HBXL12RTN","",30,0.5,30+0.5);
  TH1F HBXL13RT("HBXL13RT","",30,0.5,30+0.5);
  TH1F HBXL13RTN("HBXL13RTN","",30,0.5,30+0.5);
  
  
  int bcid=0;
  int lastleadbcid=1;
  float lumiprevbcid=0;
  bool leadbcidprev=0;
  map<long,float>::iterator itbx;
  for (itbx = bx1.begin(); itbx != bx1.end(); itbx++){
    bcid=(itbx->first)%10000;
    if(bcid<1 || bcid>NBX) continue;
        
    HBXL1.SetBinContent(bcid,itbx->second);
    HBXL2.SetBinContent(bcid,bx2[itbx->first]);
    if(itbx->second>0) HBXL12R.SetBinContent(bcid,bx2[itbx->first]/itbx->second);
     
    if(file3.CompareTo("")!=0){
      HBXL3.SetBinContent(bcid,bx3[itbx->first]);
      if(itbx->second>0) HBXL13R.SetBinContent(bcid,bx3[itbx->first]/itbx->second);
    }

    //find the leading bx
    bool leadbcid = 0;
    if(itbx->second>ymaxbx*0.3){
      leadbcid=(lumiprevbcid < 0.1*(itbx->second));
      if(leadbcid){
	HBXL12RDL.Fill(bx2[itbx->first]/itbx->second);
	if(file3.CompareTo("")!=0)
	  HBXL13RDL.Fill(bx3[itbx->first]/itbx->second);
      }

      if(leadbcidprev){
	HBXL12RDNL.Fill(bx2[itbx->first]/itbx->second);
	if(file3.CompareTo("")!=0)
	  HBXL13RDNL.Fill(bx3[itbx->first]/itbx->second);
      }

      HBXL12RT.AddBinContent(leadbcid?1:1+bcid-lastleadbcid,bx2[itbx->first]/itbx->second);
      HBXL12RTN.AddBinContent(leadbcid?1:1+bcid-lastleadbcid,1);
      if(file3.CompareTo("")!=0){
	HBXL13RT.AddBinContent(leadbcid?1:1+bcid-lastleadbcid,bx3[itbx->first]/itbx->second);
	HBXL13RTN.AddBinContent(leadbcid?1:1+bcid-lastleadbcid,1);
      }
    }

    if(leadbcid) lastleadbcid = bcid;
    lumiprevbcid = itbx->second;
    leadbcidprev = leadbcid;
  }
  
  cout<<"Done processing bx lumi"<<endl;


  can_1.cd();
  HBXL1.GetXaxis()->SetTitle("BCID");
  HBXL1.GetYaxis()->SetTitle("Lumi");
  HBXL1.GetXaxis()->SetLabelSize(0.05);
  HBXL1.GetXaxis()->SetTitleSize(0.07);
  HBXL1.GetXaxis()->SetTitleOffset(0.9);
  HBXL1.GetYaxis()->SetLabelSize(0.04);
  HBXL1.GetYaxis()->SetTitleSize(0.06);
  HBXL1.GetYaxis()->SetTitleOffset(0.6);
  HBXL1.GetXaxis()->SetRangeUser(bxplotmin,bxplotmax);
  HBXL1.GetYaxis()->SetRangeUser(0,ymaxbx*(lastLS-firstLS+1));
  HBXL1.GetYaxis()->SetNdivisions(5);
  HBXL1.SetMarkerStyle(8);
  HBXL1.SetMarkerSize(0.3);
  HBXL1.Draw("histp");

  HBXL2.SetMarkerStyle(8);
  HBXL2.SetMarkerSize(0.3);
  HBXL2.SetMarkerColor(2);
  HBXL2.Draw("histpsame");

  HBXL3.SetMarkerStyle(8);
  HBXL3.SetMarkerSize(0.3);
  HBXL3.SetMarkerColor(4);
  if(file3.CompareTo("")!=0)
    HBXL3.Draw("histpsame");

  
  text.SetTextSize(0.06);
  text.DrawLatexNDC(0.15,0.95,TString("Runs: ")+runsel);
  text.SetTextColor(1);
  text.DrawLatexNDC(0.3,0.95,TString(file1.Data()).ReplaceAll(".dat",""));
  text.SetTextColor(2);
  text.DrawLatexNDC(0.5,0.95,TString(file2.Data()).ReplaceAll(".dat",""));
  text.SetTextColor(4);
  if(file3.CompareTo("")!=0)
    text.DrawLatexNDC(0.7,0.95,TString(file3.Data()).ReplaceAll(".dat",""));

 
  ///////Ratio plot
  can_2.cd();
  HBXL12R.GetXaxis()->SetTitle(HBXL1.GetXaxis()->GetTitle());
  HBXL12R.GetYaxis()->SetTitle(TString("ratio"));
  HBXL12R.GetYaxis()->SetRangeUser(ratiominbx,ratiomaxbx);
  HBXL12R.GetXaxis()->SetRangeUser(bxplotmin,bxplotmax);
  
  HBXL12R.GetXaxis()->SetLabelSize(0.05);
  HBXL12R.GetXaxis()->SetTitleSize(0.07);
  HBXL12R.GetXaxis()->SetTitleOffset(0.9);
  HBXL12R.GetYaxis()->SetLabelSize(0.04);
  HBXL12R.GetYaxis()->SetTitleSize(0.06);
  HBXL12R.GetYaxis()->SetTitleOffset(0.6);
  HBXL12R.GetYaxis()->SetNdivisions(10);
  HBXL12R.SetMarkerStyle(8);
  HBXL12R.SetMarkerSize(0.3);
  HBXL12R.SetMarkerColor(2);
  HBXL12R.SetLineColor(2);
  HBXL12R.Draw("hist");

  HBXL13R.SetMarkerStyle(8);
  HBXL13R.SetMarkerSize(0.3);
  HBXL13R.SetMarkerColor(4);
  HBXL13R.SetLineColor(4);
  if(file3.CompareTo("")!=0)
    HBXL13R.Draw("histsame");
  
  tline.DrawLine(bxplotmin,1,bxplotmax,1);  

  C.Clear();
  can_1.Draw();
  can_2.Draw();
  C.Print(ofilename+"_bx.png");

  
  C.Clear();
  HBXL12RT.GetXaxis()->SetTitle("Train BCID");
  HBXL12RT.GetYaxis()->SetTitle("Ratio");
  HBXL12RT.GetYaxis()->SetRangeUser(ratiominbx,ratiomaxbx);
  HBXL12RT.SetLineColor(2);
  HBXL12RT.Divide(&HBXL12RTN);
  HBXL12RT.Draw("hist");
  HBXL13RT.SetLineColor(4);
  HBXL13RT.Divide(&HBXL13RTN);
  if(file3.CompareTo("")!=0)  HBXL13RT.Draw("histsame");
  tline.DrawLine(1,1,30,1);  
  C.Print(ofilename+"_bx_train.png");
  

  /* C.Clear(); */
  /* //leading */
  /* HBXL12RDL.SetLineColor(2); */
  /* HBXL12RDL.GetXaxis()->SetTitle("ratio"); */
  /* HBXL12RDL.GetYaxis()->SetRangeUser(0, HBXL12RDL.GetMaximum()*1.5); */
  /* HBXL12RDL.Draw("hist"); */
  /* HBXL13RDL.SetLineColor(4); */
  /* if(file3.CompareTo("")!=0) HBXL13RDL.Draw("histsame"); */
  /* //next to leading */
  /* HBXL12RDNL.SetLineStyle(2); */
  /* HBXL12RDNL.SetLineColor(2); */
  /* HBXL12RDNL.Draw("histsame"); */
  /* HBXL13RDNL.SetLineStyle(2); */
  /* HBXL13RDNL.SetLineColor(4); */
  /* if(file3.CompareTo("")!=0)HBXL13RDNL.Draw("histsame"); */
  /* C.Print(ofilename+"_bx_hist.png"); */

  
  gROOT->ProcessLine(".q");
}

