#include <iostream>
#include <fstream>
#include <string>

#define NLS 3000
float Lmin=0;
float Lmax=2;
float scale=(1e30/23.31)/1e34; //brilcalc lumi/lumisection [1/microbarn], 1 barn = 10^-24 cm^2,  y value will be in unites of 1e34

std::vector<std::string> dets={"hfoc","pcc","bcm1f","ramses","pltzero","hfet"};
std::vector<std::string> detnames={"HFOC","PCC","BCM","RAMSES","PLT","HFET"};
std::vector<int> colors={0,9,46,4,3,6};


TH1F * readDetector(TString Path, long RUN, TString det){

  ///Open the lumi csv file
  ifstream myfile ((Path+"/"+RUN+"_"+det+".txt").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<endl; 
    return 0;
  }

  TH1F* H = new TH1F(TString("Lumi")+det,"",NLS,0.5,NLS+0.5);

  std::string line;
  int run=0;
  int ls=0;
  int fill=0;
  double L=0;//lumi for given LS
  int h,m,s;
  int hstart=-1;
  int dcounter=-1;
  int counter=0;
  char date[100];
  char tmp[100];
  while (std::getline(myfile, line)){
    std::stringstream iss(line);

    ///line format: 324293 7270 2347 2347 10/09/18 14 04 56 STABLE BEAMS 6500 89792.454689641 66744.437627449 10.8 HFOC

    iss>>run>>fill>>ls>>ls>>date>>h>>m>>s>>tmp>>tmp>>tmp>>tmp>>L;
    if(hstart==-1||h<hstart){
      hstart=h;
      dcounter++;
    }

    // G.SetPoint(counter,dcounter*24*3600+h*3600+m*60+s,L*scale);
    // counter++;

    H->SetBinContent(ls,L*scale);

  }
    
  ///close files
  myfile.close();

  return H;
}


void plotLinearityPerInstLuminosity(TString Path, long RUN, long FILL,long year){
  //provide: path, fill number, fill starting date
  
  gROOT->ProcessLine(".x BRILAnalysisCode/rootlogon.C");

  std::vector<TH1F*> lumi;
  std::vector<TGraph*> ratio;
  for(int i=0;i<dets.size();i++){
    lumi.push_back(readDetector(Path,RUN,dets[i]));


    TGraph * g=new TGraph();
    int counter=0;
    for(int b=1;b<NLS;b++){
      g->SetPoint(counter,lumi[0]->GetBinContent(b),lumi[i]->GetBinContent(b)/lumi[0]->GetBinContent(b));
      counter++;
    }
    g->SetMarkerColor(colors[i]);
    g->SetLineColor(colors[i]);
    ratio.push_back(g);
  }
  
  /////////////////////////////////////////////////////
  ///   make the plot
  ///////////////////////////////////////////////////
  TCanvas C("C","",800,600);

  ratio[0]->GetXaxis()->SetTitle("Inst. Luminosity [E34 cm^{-2}s^{-1}]");
  ratio[0]->GetXaxis()->SetLabelSize(0.04);
  ratio[0]->GetXaxis()->SetTitleSize(0.05);
  ratio[0]->GetXaxis()->SetTitleOffset(0.9);
  ratio[0]->GetXaxis()->SetRangeUser(Lmin,Lmax);
  ratio[0]->GetYaxis()->SetTitle("Inst. Luminosity Ratio");
  ratio[0]->GetYaxis()->SetLabelSize(0.04);
  ratio[0]->GetYaxis()->SetTitleSize(0.05);
  ratio[0]->GetYaxis()->SetTitleOffset(1.0);
  ratio[0]->GetYaxis()->SetRangeUser(0.94,1.06);
  ratio[0]->SetMarkerStyle(0);
  ratio[0]->SetMarkerSize(0.0);
  ratio[0]->Draw("ap");


  TLegend leg(0.65,0.65,.92,0.88);
  leg.SetLineColor(0);
  leg.SetLineStyle(0);
  leg.SetShadowColor(0);
  leg.SetFillColor(0);
  leg.SetFillStyle(0);
  //leg.SetMarkerSize(2);

  for(int i=1;i<dets.size();i++){
    if(!lumi[i]){cout<<dets[i]<<" is null"<<endl; return;}
    ratio[i]->Draw("psame");
    lumi[i]->SetMarkerSize(1);//control marker shown in lengend
    lumi[i]->SetMarkerColor(colors[i]);
    leg.AddEntry(lumi[i],(detnames[i]+" / "+detnames[0]).c_str(),"p");
  }
  

  leg.Draw();

  TLatex text;
  text.SetTextColor(1);
  text.SetTextSize(0.05);
  text.DrawLatexNDC(0.25,0.85,TString("#font[62]{CMS} #font[52]{Preliminary ")+year+"}");
  text.SetTextColor(4);
  text.SetTextSize(0.045);
  text.DrawLatexNDC(0.25,0.94,TString("CMS Offline Luminosity, LHC Fill ")+(long)FILL+", #sqrt{s} = 13 TeV");
 
  C.Print(TString("linearity_")+FILL+".png");
  C.Print(TString("linearity_")+FILL+".pdf");

  gROOT->ProcessLine(".q");
}

