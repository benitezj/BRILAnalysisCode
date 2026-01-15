#define NYEARS 4
std::vector<TString> YearNames;
std::vector<float> lumi;
std::vector<TString> SystNames;
std::vector<float*> SystValues;
std::vector<float> totalsys;
std::vector<TMatrixF> SystMatrices;


float lumitotal(){
  float lumitotal_=0;
  for (int j=0; j<lumi.size() ; j++){
    lumitotal_+=lumi[j];
  }
  return lumitotal_;
}

void printLumis(){
  cout<<"Integrated lumi "<<endl;
  for (int j=0; j<lumi.size() ; j++){
    cout<<YearNames[j]<<" : "<<lumi[j]<<endl;
  }
  cout<<"Total : "<<lumitotal()<<endl;
}

void printSystValues(std::vector<float*> SystValues){
  cout<<endl<<endl;
  for (int i=0; i<lumi.size() ; i++)
    totalsys.push_back(0.);


  cout<<"---------------------------------------"<<endl;
  cout<<"Systematic values in % "<<endl;
  cout<<"---------------------------------------"<<endl;
  cout<<"             |";
  for (int i=0; i<lumi.size() ; i++){
   if(lumi[i]==0) continue;
   cout<< YearNames[i] <<" | ";
  }
  std::cout << endl;
  for (int s=0; s<SystValues.size() ; s++) {
    std::cout<< SystNames[s]<<" | ";
    for (int i=0; i<lumi.size() ; i++) {
      if(lumi[i]==0) continue;
      totalsys[i]+= SystValues[s][i] * SystValues[s][i] ;
      std::cout << SystValues[s][i] <<" | ";
    }
    std::cout << endl;

  }
 
  cout<<"---------------------------------------"<<endl;
  cout<<"TOTAL ";
  for (int i=0; i<lumi.size() ; i++){
    if(lumi[i]==0) continue;
    totalsys[i] = sqrt(totalsys[i]);
    cout<<" | "<< totalsys[i];
  }
  std::cout << " | "<<endl;
  cout<<"---------------------------------------"<<endl;
}


void completeSystMatrices(){
  cout<<endl<<endl;
  cout<<"---------------------------------------"<<endl;
  cout<<"Correlation matrices  "<<endl;
  cout<<"---------------------------------------"<<endl;
  for (int s=0; s < SystMatrices.size() ; s++){
    for (int i=0; i < lumi.size() ; i++){
      if(lumi[i]==0) continue;
      for (int j=0; j < lumi.size() ; j++){
	if(lumi[j]==0) continue;
	if(i==j) (SystMatrices[s])[i][j]=1;                       // diagonals always equal 1
	if(i>j) (SystMatrices[s])[i][j] = (SystMatrices[s])[j][i];  // copy the upper off-diagonals 
      }
    }
    
    //print the correlation matrices
    std::cout<< SystNames[s]<<": "<<endl;
    for (int i=0; i < lumi.size() ; i++){
      if(lumi[i]==0) continue;
      for (int j=0; j < lumi.size() ; j++){
	if(lumi[j]==0) continue;
	cout<< (SystMatrices[s])[i][j]<<" ";
      }
      cout<<endl;
    }
  }

}


void printSystValuesTotal(){
  cout<<endl<<endl;
  cout<<"---------------------------------------"<<endl;
  cout<<"Total Systematic "<<endl;
  cout<<"---------------------------------------"<<endl;
  float totalunc=0; 
  for (int s=0; s<SystValues.size() ; s++) {
    float totalunc_syst=0;
    for (int i=0; i<lumi.size() ; i++) {
      if(lumi[i]==0) continue;
      for (int j=0; j<lumi.size() ; j++) {
	if(lumi[j]==0) continue;
	totalunc_syst +=  (SystValues[s][i]*lumi[i]/100.) * (SystValues[s][j]*lumi[j]/100.)  * ((SystMatrices[s])[i][j]); // = sigma_i * sigma_j * rho_i_j
      }
    }
    cout<<SystNames[s]<<":  "<<100*sqrt(totalunc_syst)/lumitotal()<<" % "<<endl;
    totalunc += totalunc_syst;
  }
  totalunc=sqrt(totalunc);
  
  float totaluncpercent=100*totalunc/lumitotal();
  cout<<"---------------------------------------"<<endl;
  cout<<"Total Unc.  = " << totaluncpercent<<" % "<<endl;
  cout<<"---------------------------------------"<<endl;


  ////Readable table
  cout<<endl;
  cout<<"---------------------------------------"<<endl;
  cout<<"Total Luminosity "<<endl;
  cout<<"---------------------------------------"<<endl;
  for (int i=0; i<lumi.size() ; i++){
    if(lumi[i]==0) continue;
    cout<<YearNames[i]<<" : "<< lumi[i] <<" +/- "<< totalsys[i] * lumi[i] / 100. <<" ("<<totalsys[i]<<" %)"<<endl;
  }
  cout<<"Combined : "<< lumitotal() <<" +/- "<< totalunc  <<" ("<<totaluncpercent<<" %)"<<endl;


}

TString getMatrixType(TMatrixF m){
  TString ty="";
  
  //test uncorrelated 
  bool pass=1;
  for (int i=0; i<lumi.size() ; i++){
    if(lumi[i]==0) continue;
    for (int j=0; j<lumi.size() ; j++){
      if(lumi[j]==0) continue;
      if(j!=i && m[i][j]!=0) pass =0;
    }
  }

  
  if(pass){
    //"0" is the uncorrelated type 
    ty="0";
  }else { 
    //other correlation types for 3x3 : 12,13,23,123 
    for (int i=0; i<lumi.size() ; i++){
      if(lumi[i]==0) continue;
      for (int j=0; j<lumi.size() ; j++){
	if(lumi[j]==0) continue;
	if(i!=j && m[i][j]!=0){
	  if(!(ty.Contains(TString("")+(i+1)))) ty+=(i+1);
	  if(!(ty.Contains(TString("")+(j+1)))) ty+=(j+1);
	}
      }
    }
  }
  
  //cout<<"matrix type :"<<ty<<endl;  
  return ty;
}


TMatrixF calculateCovMatrix(std::vector<float*> SystValues, std::vector<TMatrixF> SystMatrices, TString type=""){
  TMatrixF cov(lumi.size(),lumi.size());
  for (int s=0; s<SystValues.size() ; s++) {
    //cout<<SystNames[s]<<endl;
    
    // 0:uncorrelated,  1:fully correlated,  2: correlated elements 0-1,  3: correlated elements 0-2 ,  4: correlated elements 1-2  
    if( type.CompareTo("")!=0  && type.CompareTo(getMatrixType(SystMatrices[s]))!=0 ) continue;
    
    for (int i=0; i<lumi.size() ; i++) {
      if(lumi[i]==0) continue;
      for (int j=0; j<lumi.size() ; j++) {
	if(lumi[j]==0) continue;
	cov[i][j] += (SystValues[s][i]*lumi[i]/100.) * (SystValues[s][j]*lumi[j]/100.)  * ((SystMatrices[s])[i][j]); // = sigma_i * sigma_j * rho_i_j
      }
    }
  }

  return cov;
}


void printMatrix(TMatrixF CovM,TString name){

  cout<<endl<<endl;

  float sumdiag=0.;
  float sumoffdiag=0.;
  for (int i=0; i<lumi.size() ; i++) {
    if(lumi[i]==0) continue;
    for (int j=0; j<lumi.size() ; j++) {
      if(lumi[j]==0) continue;
      if(i==j) sumdiag += sqrt(CovM[i][j]/(lumi[i] * lumi[j]));
      if(i!=j) sumoffdiag += sqrt(CovM[i][j]/(lumi[i] * lumi[j]));
    }
  }

  if(sumoffdiag==0 && sumdiag==0){
    cout<<"---------------------------------------"<<endl;
    cout<<name<<" is empty. "<<endl;
    cout<<"---------------------------------------"<<endl;
    return;
  }
  
  cout<<"---------------------------------------"<<endl;
  cout<<name<<" [1/fb^2] : "<<endl;
  cout<<"---------------------------------------"<<endl;
  float totcheck=0.;
  for (int i=0; i<lumi.size() ; i++) {
    if(lumi[i]==0) continue;
    cout<<" | ";
    for (int j=0; j<lumi.size() ; j++) {
      if(lumi[j]==0) continue;
      cout<<CovM[i][j]<<" | ";
      totcheck+=CovM[i][j];
    }
    cout<<endl;
  }
  cout<<"Combined : "<< lumitotal() <<" +/- "<< sqrt(totcheck)  <<" ("<<100*sqrt(totcheck)/lumitotal()<<" %)"<<endl;
  cout<<"sqrt(diag): ";
  for (int i=0; i<lumi.size() ; i++) {
    if(lumi[i]==0) continue;
    cout<<sqrt(CovM[i][i])<<" | ";
  }
  cout<<endl; 
 
  
   ///Cov Matrix in %
   cout<<"---------------------------------------"<<endl;
   cout<<name<<" [%^2] :"<<endl;
   cout<<"---------------------------------------"<<endl;
   for (int i=0; i<lumi.size() ; i++) {
     if(lumi[i]==0) continue;
     cout<<" | ";
     for (int j=0; j<lumi.size() ; j++) {
       if(lumi[j]==0) continue;
       cout<< (100*100) * CovM[i][j] / (lumi[i] * lumi[j]) <<" | ";
     }
     cout<<endl;
   }
  cout<<"sqrt(diag)[%]: ";
  for (int i=0; i<lumi.size() ; i++) {
    if(lumi[i]==0) continue;
    cout<<100*sqrt(CovM[i][i]/(lumi[i] * lumi[i]))<<" | ";
  }
  cout<<endl; 
  

  ///Print the corresponding nuissance parameters
  if(sumoffdiag==0 && sumdiag!=0){
    cout<<"nuisances uncorrelated: "<<endl;
    for (int i=0; i<lumi.size() ; i++) {
      if(lumi[i]==0) continue;
      cout<<"lumi_ | ";
      for (int j=0; j<lumi.size() ; j++) {
	if(lumi[j]==0) continue;
	cout<< 1 + sqrt(CovM[i][j]/(lumi[i] * lumi[j]))<<" | ";
      }
      cout<<endl;
    }
  }
  if(sumoffdiag!=0 && sumdiag!=0){
    cout<<"nuisances correlated: "<<endl;
    cout<<"lumi_ | ";
    for (int i=0; i<lumi.size() ; i++) {
      if(lumi[i]==0) continue;
      cout<< 1 + sqrt(CovM[i][i]/(lumi[i] * lumi[i]))<<" | ";
    }
    cout<<endl; 
  }

}


void calculateCovFromNuisance(std::vector<float*> nu, int nyears){
  //works for any number of years independent of the  code above

  //Print the nuisance list
  cout<<"Nuisances: "<<endl;
  for (int n=0; n<nu.size() ; n++) {
    cout<<"lumi_"<<n<<" | ";
    for (int j=0; j<nyears ; j++) {
      if(lumi[j]==0) continue;
      cout<<nu[n][j]<<" | ";
    }
    cout<<endl; 
  }

  /////calculate the cov matrix
  //each nuisance corresponds to a matrix
  TMatrixF cov(nyears,nyears);
  for (int n=0; n<nu.size() ; n++) {
    for (int i=0; i<nyears ; i++) {
      if(lumi[i]==0) continue;
      for (int j=0; j<nyears ; j++) {
	if(lumi[j]==0) continue;
	cov[i][j] +=  100*100* (nu[n][i] -1)  * (nu[n][j] - 1);  
      }
    }
  }

  //Print the total matrix
  cout<<"cov matrix [1/fb^2 (%^2)] :"<<endl;
  float totcheck=0.;
  for (int i=0; i<nyears ; i++) {
    if(lumi[i]==0) continue;
    cout<<" | ";
    for (int j=0; j<nyears ; j++) {
      if(lumi[j]==0) continue;
      float c=cov[i][j]*(lumi[i]*lumi[j]/(100*100));
      cout<<c<<" ( "<<cov[i][j]<<" ) | ";
      totcheck+=c;
    }
    cout<<endl;
  }
  cout<<"Combined : "<< lumitotal() <<" +/- "<< sqrt(totcheck)  <<" ("<<100*sqrt(totcheck)/lumitotal()<<" %)"<<endl;

//  cout<<"cov matrix [%^2] :"<<endl;
//  for (int i=0; i<nyears ; i++) {
//    if(lumi[i]==0) continue;
//    cout<<" | ";
//    for (int j=0; j<nyears ; j++) {
//      if(lumi[j]==0) continue;
//      cout<<cov[i][j]<<" | ";
//    }
//    cout<<endl;
//  }
   
  
}





void Run3Combination(){

  YearNames.push_back("2022");
  YearNames.push_back("2023");
  YearNames.push_back("2024");
  YearNames.push_back("2025");
  
  ///Total Lumi (1/fb)
  lumi.push_back(34.75);
  lumi.push_back(28.40);
  lumi.push_back(110.11);
  lumi.push_back(0);
  printLumis();


  ///////////////////////////////
  //// Systematic names
  SystNames.push_back("Factorization");
  SystNames.push_back("LengthScale");
  SystNames.push_back("OrbitDrift");
  SystNames.push_back("ResidualOD");
  SystNames.push_back("BeamBeam");
  SystNames.push_back("BeamCurr");
  SystNames.push_back("GhostSatellites");
  SystNames.push_back("ScanToScan");
  SystNames.push_back("BunchToBunch");
  SystNames.push_back("vdMConsistency");
  SystNames.push_back("Bakground");
  SystNames.push_back("Stability");
  SystNames.push_back("Linearity22");
  SystNames.push_back("Linearity2324");

  ////////////////////////////////////////
  //uncertainty values
  float x0[NYEARS]={0.8,0.67,0.52}; 
  float x1[NYEARS]={0.1,0.2,0.39};
  float x2[NYEARS]={0.1,0.02,0.03};
  float x3[NYEARS]={0.33,0.16,0.19};//-->check 0.33
  float x4[NYEARS]={0.37,0.34,0.35};//-->check 0.37
  float x5[NYEARS]={0.2,0.2,0.2}; 
  float x6[NYEARS]={0.15,0.1,0.28};//-->check 0.15
  float x7[NYEARS]={0.48,0.28,0.43};//-->check 0.48
  float x8[NYEARS]={0.06,0.06,0.08};//--> check 0.06
  float x9[NYEARS]={0.4,0.16,0.2};
  float x10[NYEARS]={0.2,0.,0.}; 
  float x11[NYEARS]={0.5,0.71,0.57};
  float x12[NYEARS]={0.5,0,0};
  float x13[NYEARS]={0,0.59,1.15};
  SystValues.push_back(x0);
  SystValues.push_back(x1);
  SystValues.push_back(x2);
  SystValues.push_back(x3);
  SystValues.push_back(x4);
  SystValues.push_back(x5);
  SystValues.push_back(x6);
  SystValues.push_back(x7);
  SystValues.push_back(x8);
  SystValues.push_back(x9);
  SystValues.push_back(x10);
  SystValues.push_back(x11);
  SystValues.push_back(x12);
  SystValues.push_back(x13);
  printSystValues(SystValues); //totsys array gets filled here

  
  //correlation matrices
  TMatrixF M0(NYEARS,NYEARS);
  TMatrixF M1(NYEARS,NYEARS);
  TMatrixF M2(NYEARS,NYEARS);
  TMatrixF M3(NYEARS,NYEARS); M3[0][1]=1; M3[0][2]=1; M3[1][2]=1;
  TMatrixF M4(NYEARS,NYEARS); M4[0][1]=1; M4[0][2]=1; M4[1][2]=1;
  TMatrixF M5(NYEARS,NYEARS); M5[0][1]=1; M5[0][2]=1; M5[1][2]=1;
  TMatrixF M6(NYEARS,NYEARS); M6[0][1]=1; M6[0][2]=1; M6[1][2]=1;
  TMatrixF M7(NYEARS,NYEARS);
  TMatrixF M8(NYEARS,NYEARS);
  TMatrixF M9(NYEARS,NYEARS);
  TMatrixF M10(NYEARS,NYEARS);
  TMatrixF M11(NYEARS,NYEARS);
  TMatrixF M12(NYEARS,NYEARS);
  TMatrixF M13(NYEARS,NYEARS); M13[1][2]=1;
  SystMatrices.push_back(M0);
  SystMatrices.push_back(M1);
  SystMatrices.push_back(M2);
  SystMatrices.push_back(M3);
  SystMatrices.push_back(M4);
  SystMatrices.push_back(M5);
  SystMatrices.push_back(M6);
  SystMatrices.push_back(M7);
  SystMatrices.push_back(M8);
  SystMatrices.push_back(M9);
  SystMatrices.push_back(M10);
  SystMatrices.push_back(M11);
  SystMatrices.push_back(M12);
  SystMatrices.push_back(M13);
  completeSystMatrices();

  /////////////////////////////////////////////
  // sum the cov matrices and calculate total unc.
  printSystValuesTotal();

  

  /////////////////////////////////////////////
  ////Total  Cov Matrix
  TMatrixF CovM = calculateCovMatrix(SystValues,SystMatrices);
  printMatrix(CovM,"Full covariance");

  
  /////////////////////////////////////////////////
  /// Decomposition for correlated and uncorrelated parts
  TMatrixF CovMUncorr = calculateCovMatrix(SystValues,SystMatrices,"0");
  printMatrix(CovMUncorr,"Uncorrelated covariance");

  TMatrixF CovM2223 = calculateCovMatrix(SystValues,SystMatrices,"12");
  printMatrix(CovM2223,"2022-2023 correlated covariance");
  TMatrixF CovM2224 = calculateCovMatrix(SystValues,SystMatrices,"13");
  printMatrix(CovM2224,"2022-2024 correlated covariance");
  TMatrixF CovM2324 = calculateCovMatrix(SystValues,SystMatrices,"23");
  printMatrix(CovM2324,"2023-2024 correlated covariance");

  TMatrixF CovM222324 = calculateCovMatrix(SystValues,SystMatrices,"123");
  printMatrix(CovM222324,"2022-2023-2024 correlated covariance");

  ////////////////////////////////////////////////////
  ////Check nuisance parameters  from Peter
  //https://indico.cern.ch/event/1617597/contributions/6820152/attachments/3184909/5675974/Run%202%20and%20Run%203%20combination.pdf
  cout<<endl<<endl<<" CROSSCHECK OF 3-parameter nuissance list for 2022-2023-2024: "<<endl;
  std::vector<float*> nu;
  float nu_0[3]={1.0138,1.0017,1.0020};  nu.push_back(nu_0);
  float nu_1[3]={1.0000,1.0127,1.0068};  nu.push_back(nu_1);
  float nu_2[3]={1.0000,1.0000,1.0144};  nu.push_back(nu_2);
  calculateCovFromNuisance(nu,3);

  cout<<endl<<endl<<" CROSSCHECK OF 5-parameter nuissance list for 2022-2023-2024: "<<endl;
  std::vector<float*> nu5;
  float nu5_0[3]={1.01263, 1       , 1      };  nu5.push_back(nu5_0);
  float nu5_1[3]={1      , 1.01049 , 1      };  nu5.push_back(nu5_1);
  float nu5_2[3]={1      , 1       , 1.0099 };  nu5.push_back(nu5_2);
  float nu5_3[3]={1      , 1.0059  , 1.0115 };  nu5.push_back(nu5_3);
  float nu5_4[3]={1.00555, 1.00437 , 1.00526};  nu5.push_back(nu5_4);
  calculateCovFromNuisance(nu5,3);

  /////////////////////////////////////
  /// Check totals of each nuissance for analyzes using total datacards integrated over years and combined with analyzes that use datacards per year.
  


  
  
}


