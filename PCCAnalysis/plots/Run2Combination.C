#include "Run3Combination.C"

void Run2Combination(){

  YearNames.push_back("2015");
  YearNames.push_back("2016");
  YearNames.push_back("2017");
  YearNames.push_back("2018");
  
  ///Total Lumi (1/fb) Legacy values
  lumi.push_back(2.27);//2015
  lumi.push_back(36.31);//2016
  lumi.push_back(42.07);//2017
  lumi.push_back(59.56);//2018
  printLumis();

  ///////////////////////////////
  //// Systematic names
  SystNames.push_back("Factorization1516");
  SystNames.push_back("Factorization1718");
  SystNames.push_back("LengthScale1516");
  SystNames.push_back("LengthScale1718");
  SystNames.push_back("OrbitDriftSyst");
  SystNames.push_back("OrbitDriftRand");
  SystNames.push_back("BeamBeam");
  SystNames.push_back("BeamCurr");
  SystNames.push_back("GhostSatellites");
  SystNames.push_back("Other");
  SystNames.push_back("ScanToScan18");
  SystNames.push_back("ScanToScan17");
  SystNames.push_back("BunchToBunch18");
  SystNames.push_back("BunchToBunch17");
  SystNames.push_back("vdMConsistency18");
  SystNames.push_back("vdMConsistency17");
  SystNames.push_back("PCCAfterglowType11516");
  SystNames.push_back("PCCAfterglowType21516");
  SystNames.push_back("Stability");
  SystNames.push_back("PCCLinearity1516");
  SystNames.push_back("Linearity1718");
  SystNames.push_back("CMSDeadTime");
  

  ////////////////////////////////////////
  //uncertainty values
   float x0[NYEARS]={0.5, 0.5, 0.0,  0.0}; 
   float x1[NYEARS]={0.0, 0.0, 0.33, 0.36};
   float x2[NYEARS]={0.2, 0.3, 0.0,  0.0};
   float x3[NYEARS]={0.0, 0.0, 0.15, 0.18};
   float x4[NYEARS]={0.8, 0.5, 0.09, 0.19};
   float x5[NYEARS]={0.2, 0.1, 0.0,  0.0}; 
   float x6[NYEARS]={0.5, 0.5, 0.29, 0.30};
   float x7[NYEARS]={0.2, 0.2, 0.2,  0.2};
   float x8[NYEARS]={0.1, 0.1, 0.06, 0.07};
   float x9[NYEARS]={0.6, 0.3, 0.0,  0.0};
   float x10[NYEARS]={0.0, 0.0, 0.0,  0.27};
   float x11[NYEARS]={0.0, 0.0, 0.26, 0.0};
   float x12[NYEARS]={0.0, 0.0, 0.0,  0.1};
   float x13[NYEARS]={0.0, 0.0, 0.1,  0.0};
   float x14[NYEARS]={0.0, 0.0, 0.0,  0.14};
   float x15[NYEARS]={0.0, 0.0, 0.17, 0.0};
   float x16[NYEARS]={0.3, 0.3, 0.0,  0.0};
   float x17[NYEARS]={0.1, 0.3, 0.0,  0.0};
   float x18[NYEARS]={0.6, 0.5, 0.19, 0.30};
   float x19[NYEARS]={0.5, 0.3, 0.0,  0.0};
   float x20[NYEARS]={0.0, 0.0, 0.51, 0.42};
   float x21[NYEARS]={0.5, 0.1, 0.0,  0.0};
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
   SystValues.push_back(x14);
   SystValues.push_back(x15);
   SystValues.push_back(x16);
   SystValues.push_back(x17);
   SystValues.push_back(x18);
   SystValues.push_back(x19);
   SystValues.push_back(x20);
   SystValues.push_back(x21);
  printSystValues(SystValues); //totsys array gets filled here

  
  //correlation matrices  M[][]=1;
  TMatrixF M0(NYEARS,NYEARS); M0[0][1]=1;
  TMatrixF M1(NYEARS,NYEARS); M1[2][3]=1;
  TMatrixF M2(NYEARS,NYEARS); M2[0][1]=1;
  TMatrixF M3(NYEARS,NYEARS); M3[2][3]=1;
  TMatrixF M4(NYEARS,NYEARS); M4[0][1]=1;M4[0][2]=1;M4[0][3]=1; M4[1][2]=1;M4[1][3]=1; M4[2][3]=1;
  TMatrixF M5(NYEARS,NYEARS); //
  TMatrixF M6(NYEARS,NYEARS); M6[0][1]=1;M6[0][2]=1;M6[0][3]=1; M6[1][2]=1;M6[1][3]=1; M6[2][3]=1;
  TMatrixF M7(NYEARS,NYEARS); M7[0][1]=1;M7[0][2]=1;M7[0][3]=1; M7[1][2]=1;M7[1][3]=1; M7[2][3]=1;
  TMatrixF M8(NYEARS,NYEARS); M8[0][1]=1;M8[0][2]=1;M8[0][3]=1; M8[1][2]=1;M8[1][3]=1; M8[2][3]=1;
  TMatrixF M9(NYEARS,NYEARS);//
  TMatrixF M10(NYEARS,NYEARS);//
  TMatrixF M11(NYEARS,NYEARS);// 
  TMatrixF M12(NYEARS,NYEARS);//
  TMatrixF M13(NYEARS,NYEARS);//
  TMatrixF M14(NYEARS,NYEARS);//
  TMatrixF M15(NYEARS,NYEARS);//
  TMatrixF M16(NYEARS,NYEARS); M16[0][1]=1;
  TMatrixF M17(NYEARS,NYEARS); M17[0][1]=1;
  TMatrixF M18(NYEARS,NYEARS);//
  TMatrixF M19(NYEARS,NYEARS); M19[0][1]=1;
  TMatrixF M20(NYEARS,NYEARS); M20[2][3]=1;
  TMatrixF M21(NYEARS,NYEARS);//
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
  SystMatrices.push_back(M14);
  SystMatrices.push_back(M15);
  SystMatrices.push_back(M16);
  SystMatrices.push_back(M17);
  SystMatrices.push_back(M18);
  SystMatrices.push_back(M19);
  SystMatrices.push_back(M20);
  SystMatrices.push_back(M21);
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
  TMatrixF CovM15161718 = calculateCovMatrix(SystValues,SystMatrices,"1234");
  printMatrix(CovM15161718,"2015-2016-2017-2018 correlated covariance");
  TMatrixF CovM151617 = calculateCovMatrix(SystValues,SystMatrices,"123");
  printMatrix(CovM151617,"2015-2016-2017 correlated covariance");
  TMatrixF CovM161718 = calculateCovMatrix(SystValues,SystMatrices,"234");
  printMatrix(CovM161718,"2016-2017-2018 correlated covariance");
  TMatrixF CovM151618 = calculateCovMatrix(SystValues,SystMatrices,"124");
  printMatrix(CovM151618,"2015-2016-2018 correlated covariance");
  TMatrixF CovM151718 = calculateCovMatrix(SystValues,SystMatrices,"134");
  printMatrix(CovM151718,"2015-2017-2018 correlated covariance");
  TMatrixF CovM1516 = calculateCovMatrix(SystValues,SystMatrices,"12");
  printMatrix(CovM1516,"2015-2016 correlated covariance");
  TMatrixF CovM1517 = calculateCovMatrix(SystValues,SystMatrices,"13");
  printMatrix(CovM1517,"2015-2017 correlated covariance");
  TMatrixF CovM1518 = calculateCovMatrix(SystValues,SystMatrices,"14");
  printMatrix(CovM1518,"2015-2018 correlated covariance");
  TMatrixF CovM1617 = calculateCovMatrix(SystValues,SystMatrices,"23");
  printMatrix(CovM1617,"2016-2017 correlated covariance");
  TMatrixF CovM1618 = calculateCovMatrix(SystValues,SystMatrices,"24");
  printMatrix(CovM1618,"2016-2018 correlated covariance");
  TMatrixF CovM1718 = calculateCovMatrix(SystValues,SystMatrices,"34");
  printMatrix(CovM1718,"2017-2018 correlated covariance");


    
//  ////////////////////////////////////////////////////
//  ////Check nuisance parameters  from Peter
//  //https://indico.cern.ch/event/1617597/contributions/6820152/attachments/3184909/5675974/Run%202%20and%20Run%203%20combination.pdf
  cout<<endl<<endl<<" CROSSCHECK OF 4-parameter nuissance list for 2015-2018: "<<endl;
  std::vector<float*> nu15161718;
  float nu15161718_4_1[4]={1.0122, 1.0000, 1.0000, 1.0000};  nu15161718.push_back(nu15161718_4_1);
  float nu15161718_4_2[4]={1.0096, 1.0118, 1.0000, 1.0000};  nu15161718.push_back(nu15161718_4_2);
  float nu15161718_4_3[4]={1.0002, 1.0004, 1.0055, 1.0000};  nu15161718.push_back(nu15161718_4_3);
  float nu15161718_4_4[4]={1.0042, 1.0035, 1.0061, 1.0084};  nu15161718.push_back(nu15161718_4_4);
  calculateCovFromNuisance(nu15161718,4);
  cout<<endl<<endl<<" CROSSCHECK OF 7-parameter nuissance list for 2015-2018: "<<endl;
  std::vector<float*> nu15161718_7;
  float nu15161718_7_1[4]={1.01005,1      , 1     ,1       };  nu15161718_7.push_back(nu15161718_7_1);
  float nu15161718_7_2[4]={1      ,1.006  , 1     ,1       };  nu15161718_7.push_back(nu15161718_7_2);
  float nu15161718_7_3[4]={1      ,1      ,1.00378,1       };  nu15161718_7.push_back(nu15161718_7_3);
  float nu15161718_7_4[4]={1      ,1      ,1      ,1.00439 };  nu15161718_7.push_back(nu15161718_7_4);
  float nu15161718_7_5[4]={1.008  ,1.00781,1      ,1       };  nu15161718_7.push_back(nu15161718_7_5);
  float nu15161718_7_6[4]={1      ,1      ,1.00626,1.00582 };  nu15161718_7.push_back(nu15161718_7_6);
  float nu15161718_7_7[4]={1.0097 ,1.00742,1.00369,1.00414 };  nu15161718_7.push_back(nu15161718_7_7);
  calculateCovFromNuisance(nu15161718_7,4);
  cout<<endl<<endl<<" CROSSCHECK OF 4-parameter nuissance totals for 2015-2018: "<<endl;
  std::vector<float*> nu15161718_total;
  cout<<endl<<" nu_1: "<<endl; nu15161718_total.clear(); nu15161718_total.push_back(nu15161718_4_1);  calculateCovFromNuisance(nu15161718_total,4);
  cout<<endl<<" nu_2: "<<endl; nu15161718_total.clear(); nu15161718_total.push_back(nu15161718_4_2);  calculateCovFromNuisance(nu15161718_total,4);
  cout<<endl<<" nu_3: "<<endl; nu15161718_total.clear(); nu15161718_total.push_back(nu15161718_4_3);  calculateCovFromNuisance(nu15161718_total,4);
  cout<<endl<<" nu_4: "<<endl; nu15161718_total.clear(); nu15161718_total.push_back(nu15161718_4_4);  calculateCovFromNuisance(nu15161718_total,4);
 
  
  
  ///for 2016-2018 check below make sure lumi 2015 is set to 0 above.
  lumi[0]=0;
  cout<<endl<<endl<<" CROSSCHECK OF 3-parameter nuissance list for 2016-2018: "<<endl;
  std::vector<float*> nu161718;
  float nu161718_3_1[4]={1,1.0118, 1.0000, 1.0000};  nu161718.push_back(nu161718_3_1);
  float nu161718_3_2[4]={1,1.0004, 1.0055, 1.0000};  nu161718.push_back(nu161718_3_2);
  float nu161718_3_3[4]={1,1.0035, 1.0061, 1.0084};  nu161718.push_back(nu161718_3_3);
  calculateCovFromNuisance(nu161718,4);
  cout<<endl<<endl<<" CROSSCHECK OF 5-parameter nuissance list for 2016-2018: "<<endl;
  std::vector<float*> nu161718_5;
  float nu161718_5_1[4]={1,1.00985 , 1 , 1};  nu161718_5.push_back(nu161718_5_1);
  float nu161718_5_2[4]={1,1 , 1.00378 , 1};  nu161718_5.push_back(nu161718_5_2);
  float nu161718_5_3[4]={1,1 , 1 , 1.00439};  nu161718_5.push_back(nu161718_5_3);
  float nu161718_5_4[4]={1,1 , 1.00626 , 1.00582 };        nu161718_5.push_back(nu161718_5_4);
  float nu161718_5_5[4]={1,1.00742 , 1.00369 , 1.00414 };  nu161718_5.push_back(nu161718_5_5);
  calculateCovFromNuisance(nu161718_5,4);
  // Check totals of nuissance parameters for 2015-2018
  cout<<endl<<endl<<" CROSSCHECK OF 3-parameter nuissance totals for 2016-2018: "<<endl;
  std::vector<float*> nu161718_total;
  cout<<endl<<" nu_1: "<<endl; nu161718_total.clear(); nu161718_total.push_back(nu161718_3_1);  calculateCovFromNuisance(nu161718_total,4);
  cout<<endl<<" nu_2: "<<endl; nu161718_total.clear(); nu161718_total.push_back(nu161718_3_2);  calculateCovFromNuisance(nu161718_total,4);
  cout<<endl<<" nu_3: "<<endl; nu161718_total.clear(); nu161718_total.push_back(nu161718_3_3);  calculateCovFromNuisance(nu161718_total,4);
  
  
  
}


