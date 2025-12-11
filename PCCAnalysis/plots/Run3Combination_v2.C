
#define NYEARS 3
std::vector<TString> SystNames;
TString SystYears[NYEARS]={"2022","2023","2024"};

float lumi[NYEARS];
float lumitotal=0;

float totalsys[NYEARS];
float totalsysTot=0;


void printSystValues(std::vector<float*> SystValues){

  for (int i=0; i<NYEARS ; i++)
    totalsys[i]=0;


  cout<<"---------------------------------------"<<endl;
  cout<<"Systematic values in % "<<endl;
  cout<<"---------------------------------------"<<endl;
  cout<<"             ";
  for (int j=0; j<NYEARS ; j++)
    cout<<" | "<< SystYears[j];
  std::cout << endl;
  cout<<"---------------------------------------"<<endl;
  for (int i=0; i<SystValues.size() ; i++) {
    std::cout<< SystNames[i]<<" | ";
    for (int j=0; j<NYEARS ; j++) {
      totalsys[j]+= SystValues[i][j] * SystValues[i][j] ;
      std::cout << SystValues[i][j] <<" | ";
    }
    std::cout << endl;

  }
 
  cout<<"---------------------------------------"<<endl;
  cout<<"TOTAL ";
  for (int i=0; i<NYEARS ; i++){
    totalsys[i] = sqrt(totalsys[i]);
    cout<<" | "<< totalsys[i];
  }
  std::cout << " | "<<endl;
  cout<<"---------------------------------------"<<endl;
}



void printSystValuesTotal(std::vector<float*> SystValues, std::vector<float> SystTotal){

  cout<<"---------------------------------------"<<endl;
  cout<<"Systematic values in % "<<endl;
  cout<<"---------------------------------------"<<endl;
  cout<<"          ";
  for (int j=0; j<NYEARS ; j++)
    cout<<" | "<< SystYears[j];
  cout<<" |  Combined  | ";
  std::cout << endl;
  
  cout<<"---------------------------------------"<<endl;
  for (int i=0; i<SystValues.size() ; i++) {
    std::cout<< SystNames[i]<<" | ";
    for (int j=0; j<NYEARS ; j++) {
      std::cout << SystValues[i][j] <<" | ";
    }
    std::cout << SystTotal[i] <<" | ";
    std::cout << endl;
    totalsysTot += SystTotal[i]*SystTotal[i] ;
  }
  cout<<"---------------------------------------"<<endl;

  totalsysTot = sqrt(totalsysTot);
  
  cout<<"TOTAL ";
  for (int j=0; j<NYEARS ; j++)
    cout<<" | "<< totalsys[j];
  cout<<" | "<< totalsysTot;
  std::cout << " | "<<endl;
  cout<<"---------------------------------------"<<endl;
}



void Run3Combination_v2(){

  ///Total Lumi (1/fb)
  lumi[0]=34.7486;//41.47;
  lumi[1]=28.40;// 32.74;
  lumi[2]=110.11;//124.04;

  cout<<"---------------------------------------"<<endl;
  cout<<"Lumi: "<<endl;
  for (int j=0; j<NYEARS ; j++){
    cout<<SystYears[j]<<" : "<< lumi[j]<<endl;
    lumitotal+=lumi[j];
  }
  std::cout << endl;
  cout<<"---------------------------------------"<<endl;
  cout<<"TOTAL = "<<lumitotal<<endl;
  cout<<"---------------------------------------"<<endl;


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
  float x3[NYEARS]={0.3,0.16,0.19};
  float x4[NYEARS]={0.4,0.34,0.35};
  float x5[NYEARS]={0.2,0.2,0.2};
  float x6[NYEARS]={0.2,0.1,0.28};
  float x7[NYEARS]={0.5,0.28,0.43};
  float x8[NYEARS]={0.1,0.06,0.08};
  float x9[NYEARS]={0.4,0.16,0.2};
  float x10[NYEARS]={0.2,0.,0.}; 
  float x11[NYEARS]={0.5,0.71,0.57};
  float x12[NYEARS]={0.5,0,0};
  float x13[NYEARS]={0,0.59,1.15};

  std::vector<float*> SystValues;
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
  printSystValues(SystValues);

  
  //correlation matrices
  TMatrixF M0(NYEARS,NYEARS); M0[0][1]=0; M0[0][2]=0; M0[1][2]=0;
  TMatrixF M1(NYEARS,NYEARS); M1[0][1]=0; M1[0][2]=0; M1[1][2]=0;
  TMatrixF M2(NYEARS,NYEARS); M2[0][1]=0; M2[0][2]=0; M2[1][2]=0;
  TMatrixF M3(NYEARS,NYEARS); M3[0][1]=1; M3[0][2]=1; M3[1][2]=1;
  TMatrixF M4(NYEARS,NYEARS); M4[0][1]=1; M4[0][2]=1; M4[1][2]=1;
  TMatrixF M5(NYEARS,NYEARS); M5[0][1]=1; M5[0][2]=1; M5[1][2]=1;
  TMatrixF M6(NYEARS,NYEARS); M6[0][1]=1; M6[0][2]=1; M6[1][2]=1;
  TMatrixF M7(NYEARS,NYEARS); M7[0][1]=0; M7[0][2]=0; M7[1][2]=0;
  TMatrixF M8(NYEARS,NYEARS); M8[0][1]=0; M8[0][2]=0; M8[1][2]=0;
  TMatrixF M9(NYEARS,NYEARS); M9[0][1]=0; M9[0][2]=0; M9[1][2]=0;
  TMatrixF M10(NYEARS,NYEARS); M10[0][1]=0; M10[0][2]=0; M10[1][2]=0;
  TMatrixF M11(NYEARS,NYEARS); M11[0][1]=0; M11[0][2]=0; M11[1][2]=0;
  TMatrixF M12(NYEARS,NYEARS); M12[0][1]=1; M12[0][2]=0; M12[1][2]=0;
  TMatrixF M13(NYEARS,NYEARS); M13[0][1]=0; M13[0][2]=1; M13[1][2]=1;
  std::vector<TMatrixF> SystMatrices;
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

  for (int s=0; s < SystMatrices.size() ; s++){
    (SystMatrices[s])[0][0]=(SystMatrices[s])[1][1]=(SystMatrices[s])[2][2]=1;//diagonals
    (SystMatrices[s])[1][0]=(SystMatrices[s])[0][1];
    (SystMatrices[s])[2][0]=(SystMatrices[s])[0][2];
    (SystMatrices[s])[2][1]=(SystMatrices[s])[1][2];
    
    std::cout<< SystNames[s]<<" : "<<endl;
    for (int i=0; i < NYEARS ; i++){
      for (int j=0; j < NYEARS ; j++){
	cout<< (SystMatrices[s])[i][j]<<" ";
      }
      cout<<endl;
    }
  }


  

  //////////////////////////////////////////////////////////
  float totalunc=0;
  cout<<"---------------------------------------"<<endl;
  cout<<"Total Systematic "<<endl;
  cout<<"---------------------------------------"<<endl;
  std::vector<float> SystTotal; // per systematic
  TMatrixF CovM(NYEARS,NYEARS); // cov matrix
  for (int i=0; i<NYEARS ; i++) {
    for (int j=0; j<NYEARS ; j++) {
      CovM[i][j]=0.;
    }
  }

  for (int s=0; s<SystValues.size() ; s++) {
    float totalunc_syst=0;
    for (int i=0; i<NYEARS ; i++) {
      for (int j=0; j<NYEARS ; j++) {
	float e =(SystValues[s][i]*lumi[i]/100.) * (SystValues[s][j]*lumi[j]/100.)  * ((SystMatrices[s])[i][j]);
	CovM[i][j] += e; //sum over systematics 
	totalunc_syst += e ; // sum over years 
      }
    }
    cout<<SystNames[s]<<":  "<<100*sqrt(totalunc_syst)/lumitotal<<endl;

    SystTotal.push_back(100*sqrt(totalunc_syst)/lumitotal);
    totalunc += totalunc_syst;
  }
  cout<<"---------------------------------------"<<endl;
  cout<<"Total Unc.  = " << 100*sqrt(totalunc)/lumitotal <<endl;
  cout<<"---------------------------------------"<<endl;

  
  ////Readable table
  printSystValuesTotal(SystValues,SystTotal);
  cout<<"---------------------------------------"<<endl;
  cout<<"Luminosity "<<endl;
  cout<<"---------------------------------------"<<endl;
  for (int j=0; j<NYEARS ; j++)
    cout<<SystYears[j]<<" : "<< lumi[j] <<" +/- "<< totalsys[j] * lumi[j] / 100. <<" ("<<totalsys[j]<<" %)"<<endl;
  cout<<"Combined : "<< lumitotal <<" +/- "<< totalsysTot * lumitotal / 100.  <<" ("<<totalsysTot<<" %)"<<endl;


  //// Cov Matrix
  cout<<"---------------------------------------"<<endl;
  cout<<"Covariance Matrix "<<endl;
  cout<<"---------------------------------------"<<endl;
  float totcheck=0.;
  for (int i=0; i<NYEARS ; i++) {
    for (int j=0; j<NYEARS ; j++) {
      cout<<CovM[i][j]<<"  ";
      totcheck+=CovM[i][j];
    }
    cout<<endl;
  }
  cout<<"Combined : "<< lumitotal <<" +/- "<< sqrt(totcheck)  <<" ("<<100*sqrt(totcheck)/lumitotal<<" %)"<<endl;


  ///Cov Matrix in %
  cout<<"---------------------------------------"<<endl;
  cout<<"Covariance Matrix in %^2 "<<endl;
  cout<<"---------------------------------------"<<endl;
  for (int i=0; i<NYEARS ; i++) {
    for (int j=0; j<NYEARS ; j++) {
      cout<< (100*100) * CovM[i][j] / (lumi[i] * lumi[j]) <<"  ";
    }
    cout<<endl;
  }




  //////////////////////////////////////////////
  /// 2022 + 2023 combination:
  cout<<"---------------------------------------"<<endl;
  cout<<"2022 + 2023 combination,  Covariance Matrix "<<endl;
  cout<<"---------------------------------------"<<endl;
  float totlumi2223=0.;
  float totsyst2223=0.;
  for (int i=0; i<NYEARS ; i++) {
    if( i != 2 ) totlumi2223 += lumi[i]; 
    for (int j=0; j<NYEARS ; j++) {
      if( i == 2 || j == 2 ) continue;
      totsyst2223 += CovM[i][j];
    }
  }
  cout<<"Combined 2022 + 2023 : "<< totlumi2223 <<" +/- "<< sqrt(totsyst2223)  <<" ("<<100*sqrt(totsyst2223)/totlumi2223<<" %)"<<endl;


  /// 2023 + 2024 combination:
  cout<<"---------------------------------------"<<endl;
  cout<<"2023 + 2024 combination,  Covariance Matrix "<<endl;
  cout<<"---------------------------------------"<<endl;
  float totlumi2324=0.;
  float totsyst2324=0.;
  for (int i=0; i<NYEARS ; i++) {
    if( i != 0 ) totlumi2324 += lumi[i]; 
    for (int j=0; j<NYEARS ; j++) {
      if( i == 0 || j == 0 ) continue;
      totsyst2324 += CovM[i][j];
    }
  }
  cout<<"Combined 2023 + 2024 : "<< totlumi2324 <<" +/- "<< sqrt(totsyst2324)  <<" ("<<100*sqrt(totsyst2324)/totlumi2324<<" %)"<<endl;


  

  
}


