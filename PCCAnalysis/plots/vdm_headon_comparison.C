



void readcsv(std::map<string,float> * datamap, TString filename, unsigned index=1){

  ifstream file(filename);
  if (!file.is_open()){
    std::cout << "Unable to open "<<filename<<endl;
    return;
  }
  
  std::string line;
  int run=0;
  int ls=0;
  float x;
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

  std::map<string,float> pcctime;
  readcsv(&pcctime,"pccLumi_fill8381_pcc22v2_23v1.csv",1);
  
  std::map<string,float> pcclumi;
  readcsv(&pcclumi,"pccLumi_fill8381_pcc22v2_23v1.csv",2);

  std::map<string,float> hflumi;
  readcsv(&hflumi,"hfet-v2.csv",1);


  TGraph GLumi;
  TGraph GLumiHF;
  TGraph GLumiRatio;
  TH1F HPCC("HPCC","",100,0,25);
  TH1F HHF("HHF","",100,0,25);
  for (auto iter = pcctime.begin(); iter != pcctime.end(); ++iter) {
    if(iter->second<=0. || pcclumi[iter->first]<=0 || hflumi[iter->first]<=0) continue;

    float t=(int(iter->second)-1668109440)/3600.;
      
    //cout<<iter->first<<" "<<int(iter->second)<<endl;

    GLumi.SetPoint(GLumi.GetN(),t,pcclumi[iter->first]);
    GLumiHF.SetPoint(GLumiHF.GetN(),t,hflumi[iter->first]);
    GLumiRatio.SetPoint(GLumiRatio.GetN(),t,pcclumi[iter->first]/hflumi[iter->first]);
    HPCC.Fill(t,pcclumi[iter->first]);
    HHF.Fill(t,hflumi[iter->first]);
    
  }


  TCanvas C;
  C.Clear();
  GLumi.SetMarkerStyle(8);
  GLumi.SetMarkerSize(0.3);
  GLumi.GetYaxis()->SetTitle("Lumi [Hz/ub]");
  GLumi.GetXaxis()->SetTitle("Time [hr]");
  GLumi.GetYaxis()->SetRangeUser(0.6,0.7);
  GLumi.Draw("ap");
  GLumiHF.SetMarkerStyle(3);
  GLumiHF.SetMarkerSize(0.3);
  GLumiHF.SetMarkerColor(2);
  GLumiHF.Draw("psame");
  C.Print("vdm_headon_comparison.png");

  float ratiorange=0.015;
  C.Clear();
  GLumiRatio.SetMarkerStyle(8);
  GLumiRatio.SetMarkerSize(0.3);
  GLumiRatio.GetYaxis()->SetRangeUser(1-ratiorange,1+ratiorange);
  GLumiRatio.GetYaxis()->SetTitle("Lumi Ratio");
  GLumiRatio.GetXaxis()->SetTitle("Time [hr]");
  GLumiRatio.Draw("ap");
  C.Print("vdm_headon_comparison_ratio.png");

  HPCC.Divide(&HHF);
  HPCC.SetStats(0);
  HPCC.SetMarkerStyle(8);
  HPCC.SetMarkerSize(0.3);
  C.Clear();
  HPCC.GetYaxis()->SetTitle("Lumi Ratio");
  HPCC.GetXaxis()->SetTitle("Time [hr]");
  HPCC.GetYaxis()->SetRangeUser(1-ratiorange,1+ratiorange);
  HPCC.Draw("histp");
  C.Print("vdm_headon_comparison_ratioavg.png");


  cout<<pcclumi.size()<<endl;
  
}
