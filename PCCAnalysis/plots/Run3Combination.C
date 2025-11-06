
void printLumiUncs(TString title, float L, float Corr, float UnCorr){
  /// Input uncertainties (Corr,UnCorr) must be in %
  float Tot=sqrt(Corr*Corr + UnCorr*UnCorr);
  cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
  cout<<title<<":  "<<L<<" +/- "<<(L*Corr/100)<<" Corr ("<<Corr<<" %)  +/- "<<(L*UnCorr/100)<<" UnCorr ("<<UnCorr<<" %)"<<endl;
  cout<<title<<":  "<<L<<" +/- "<<(L*Tot/100)<<" ("<<Tot<<" %)"<<endl;
  cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
}


float calculateErr(float L1, float L1err, float L2, float L2err, bool corr=0){
  /// Input uncertainties (L1err,L2err) must be in %
  /// returns value in %
  
  float LT=L1+L2;

  //correlated
  if(corr)
    return sqrt(pow(L1*L1err,2) + pow(L2*L2err,2) + 2*(L1*L1err)*(L2*L2err)) / LT;

  //uncorrelated
  return sqrt(pow(L1*L1err,2) + pow(L2*L2err,2)) / LT;
}


void Run3Combination(){

  ///Total Lumi (1/fb)
  float Lumi2022=41.47;
  float Lumi2023=32.74;
  float Lumi2024=120;
  
  ///Correlated Unc. (in %)
  float Corr2022=sqrt(pow(0.2,2) + pow(0.2,2) + pow(0.1,2) + pow(0.3,2) + pow(0.4,2) + pow(0.1,2) + pow(0.8,2) + pow(0.5,2));
  float Corr2023=sqrt(pow(0.2,2) + pow(0.1,2) + pow(0.02,2) + pow(0.16,2) + pow(0.34,2) + pow(0.2,2) + pow(0.67,2) + pow(0.59,2));
  float Corr2024=sqrt(pow(0.2,2) + pow(0.12,2) + pow(0.03,2) + pow(0.19,2) + pow(0.35,2) + pow(0.21,2) + pow(0.67,2) + pow(0.50,2));

  ///Uncorrelated Unc.(in %)
  float UnCorr2022=sqrt(pow(0.5,2) + pow(0.1,2) + pow(0.4,2) + pow(0.2,2) + pow(0.5,2));//contains the HFET Afterglow systematic
  float UnCorr2023=sqrt(pow(0.28,2) + pow(0.06,2) + pow(0.16,2) + pow(0.71,2));
  float UnCorr2024=sqrt(pow(0.37,2) + pow(0.15,2) + pow(0.12,2) + pow(0.50,2));

  
  //print the values
  printLumiUncs("2022",Lumi2022,Corr2022,UnCorr2022);
  printLumiUncs("2023",Lumi2023,Corr2023,UnCorr2023);
  printLumiUncs("2024",Lumi2024,Corr2024,UnCorr2024);
  
  //////////////////////////////////
  //Combinations
  
  //2022+2023
  float Lumi_2022_2023 = Lumi2022 + Lumi2023;
  float Corr_2022_2023 = calculateErr(Lumi2022,Corr2022,Lumi2023,Corr2023,1);
  float UnCorr_2022_2023 = calculateErr(Lumi2022,UnCorr2022,Lumi2023,UnCorr2023,0);
  printLumiUncs("2022+2023",Lumi_2022_2023,Corr_2022_2023,UnCorr_2022_2023);
  
  //2022+2023+2024
  float Lumi_2022_2023_2024 = Lumi_2022_2023 + Lumi2024;
  float Corr_2022_2023_2024 = calculateErr(Lumi_2022_2023,Corr_2022_2023,Lumi2024,Corr2024,1);
  float UnCorr_2022_2023_2024 = calculateErr(Lumi_2022_2023,UnCorr_2022_2023,Lumi2024,UnCorr2024,0);
  printLumiUncs("2022+2023+2024",Lumi_2022_2023_2024,Corr_2022_2023_2024,UnCorr_2022_2023_2024);
   
}
