
float MINLUMI=0;
float MAXLUMI=2;
float MINLUMICUT=1.6;
float MINTIME=0; float MAXTIME=25; int   NTIME=196;
//float MINTIME=15.55; float MAXTIME=15.7; int NTIME=100;

long  T0=1668111133;
float ratiorange=0.015;


void readcsv(std::map<string,double> * datamap, TString filename, unsigned index=1){

  ifstream file(filename);
  if (!file.is_open()){
    std::cout << "Unable to open "<<filename<<endl;
    return;
  }
  
  std::string line;
  int run=0;
  int ls=0;
  double x;
  while (std::getline(file,line)){
    std::stringstream iss(line);
    std::string token;

    std::getline(iss,token, ',');
    run=atoi(token.c_str());
    std::getline(iss,token, ',');
    ls=atoi(token.c_str());

    ///read values up to index value
    std::getline(iss,token, ',');
    x=atof(token.c_str());
    for(int i=1;i<index;i++){
      std::getline(iss,token, ',');
      x=atof(token.c_str());
    }
    TString id=TString("")+run+ls;
    (*datamap)[id.Data()]=x;
  }
}



////////////////////////////////////////////////////////
void vdm_headon_comparison(){

  std::map<string,double> pcctime;
  //readcsv(&pcctime,"pccLumi_fill8381_pcc22v2_23v1.csv",1);
  readcsv(&pcctime,"rates_8381_2022-nominal.csv",1);
  
  std::map<string,double> pcclumi;
  //readcsv(&pcclumi,"pccLumi_fill8381_pcc22v2_23v1.csv",2);
  //readcsv(&pcclumi,"rates_8381_2022-nominal.csv",2);
  readcsv(&pcclumi,"pcc_13bcid.csv",1);

  std::map<string,double> hflumi;
  //readcsv(&hflumi,"hfet-v2.csv",1);
  readcsv(&hflumi,"hfet_13bcid.csv",1);


  ///calcualte the background
  float bkgSS1=0.;
  int bkgNSS1=0;
  float bkgSS2=0.;
  int bkgNSS2=0;
  for (auto iter = pcctime.begin(); iter != pcctime.end(); ++iter) {
    float t=(iter->second-T0)/3600.;

    if(3.31<t && t<3.39){
      bkgSS1+=pcclumi[iter->first];
      bkgNSS1++;
    }

    if(15.58<t && t<15.66){
      bkgSS2+=pcclumi[iter->first];
      bkgNSS2++;
    }

  }
  if(bkgNSS1>0){
    bkgSS1/=bkgNSS1;
  }
  if(bkgNSS2>0){
    bkgSS2/=bkgNSS2;
  }
  float bkg=(bkgSS1+bkgSS2)/2;
  cout<<"PCC background value: ("<<bkgSS1<<" + "<<bkgSS2<<")/2 =  "<<bkg<<endl;


  ofstream file("vdm2022HeadOnLumi_pcc_hfet_13bcid.csv");
  if (!file.is_open()){
    std::cout << "Unable to open output file "<<endl;
    return;
  }
  
  
  TGraph GLumi;
  TGraph GLumiHF;
  TGraph GLumiRatio;
  TH1F HPCC("HPCC","",NTIME,MINTIME,MAXTIME);
  TH1F HHF("HHF","",NTIME,MINTIME,MAXTIME);
  for (auto iter = pcctime.begin(); iter != pcctime.end(); ++iter) {
    if(iter->second<=0.) continue;

    float t=(iter->second - T0)/3600.;
   
    //print bad LS's
    char* lsstr=(char*)((iter->first).c_str());
    char runstr[100];
    sprintf(runstr,"%c%c%c%c%c%c",lsstr[0],lsstr[1],lsstr[2],lsstr[3],lsstr[4],lsstr[5]);
    lsstr+=6;



    ///invalidate lumi sections where some modules went bad 
    if((19<t && t<21)
       && hflumi[iter->first] > 0 && pcclumi[iter->first] > 0 
       && fabs((pcclumi[iter->first]-bkg)/hflumi[iter->first] - 1)>0.003
       )
      {
	//cout<<iter->first<<" "<<runstr<<","<<lsstr<<","<<t<<","<<(pcclumi[iter->first]-bkg)<<","<<hflumi[iter->first]<<endl;
	continue;
      }

    ///Write the final data to a csv
    file<<runstr<<","<<lsstr<<","<<(long)(iter->second)<<","<<(pcclumi[iter->first]-bkg)<<","<<hflumi[iter->first]<<endl;

    ///remove lumisections below with lumi below the headon position
    if(hflumi[iter->first]>MINLUMICUT && MINTIME<t && t<MAXTIME){  
      GLumi.SetPoint(GLumi.GetN(),t,pcclumi[iter->first]-bkg);
      GLumiHF.SetPoint(GLumiHF.GetN(),t,hflumi[iter->first]);    
      GLumiRatio.SetPoint(GLumiRatio.GetN(),t,(pcclumi[iter->first]-bkg)/hflumi[iter->first]);
      HPCC.Fill(t,pcclumi[iter->first]-bkg);
      HHF.Fill(t,hflumi[iter->first]);
    }
    
  }

  file.close();

  TCanvas C;

  TLatex label;
  label.SetTextSize(0.04);
  char plotlabel[100]="Fill 8381, 2022";
  C.Clear();
  GLumi.SetMarkerStyle(8);
  GLumi.SetMarkerSize(0.3);
  GLumi.GetYaxis()->SetTitle("Lumi [Hz/ub]");
  GLumi.GetXaxis()->SetTitle("Time [hr]");
  GLumi.GetYaxis()->SetRangeUser(MINLUMI,MAXLUMI);
  GLumi.Draw("ap");
  GLumiHF.SetMarkerStyle(3);
  GLumiHF.SetMarkerSize(0.3);
  GLumiHF.SetMarkerColor(2);
  GLumiHF.Draw("psame");
  label.SetTextColor(1);
  label.DrawLatexNDC(0.5,0.85,"#bullet PCC");
  label.DrawLatexNDC(0.1,0.92,plotlabel);
  label.SetTextColor(2);
  label.DrawLatexNDC(0.7,0.85,"#bullet HFET");
  C.Print("vdm_headon_comparison.png");

  TLine line;
  C.Clear();
  GLumiRatio.SetMarkerStyle(8);
  GLumiRatio.SetMarkerSize(0.3);
  GLumiRatio.SetMarkerColor(2);
  GLumiRatio.GetYaxis()->SetRangeUser(1-ratiorange,1+ratiorange);
  GLumiRatio.GetYaxis()->SetTitle("PCC Lumi / HFET Lumi  per 1 LS");
  GLumiRatio.GetXaxis()->SetTitle("Time [hr]");
  GLumiRatio.Draw("ap");
  line.DrawLine(MINTIME,1,MAXTIME,1);
  label.SetTextColor(1);
  label.DrawLatexNDC(0.1,0.92,plotlabel);
  C.Print("vdm_headon_comparison_ratio.png");

  HPCC.Divide(&HHF);
  HPCC.SetStats(0);
  HPCC.SetMarkerStyle(8);
  HPCC.SetMarkerSize(0.4);
  HPCC.SetMarkerColor(2);
  C.Clear();
  HPCC.GetYaxis()->SetTitle("PCC Lumi / HFET Lumi  per 20 LS");
  HPCC.GetXaxis()->SetTitle("Time [hr]");
  HPCC.GetYaxis()->SetRangeUser(1-ratiorange,1+ratiorange);
  HPCC.Draw("histp");
  line.DrawLine(MINTIME,1,MAXTIME,1);
  label.SetTextColor(1);
  label.DrawLatexNDC(0.1,0.92,plotlabel);
  C.Print("vdm_headon_comparison_ratioavg.png");


  cout<<pcclumi.size()<<endl;
  
}
