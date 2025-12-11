

void test_sbil_weight(){

  float nonlinearity=0.01;// /(hz/ub)
  cout<<"nonlinearity = "<<nonlinearity<<endl;
  
  //Dataset 1
  float sbil1 = 1;// hz/ub
  float lumi1 = 1;// e.g. 1/fb
  float lumi1er=lumi1 * (sbil1 * nonlinearity);
  cout<<"lumi 1: "<<lumi1<<" +/- "<<lumi1er<<" ( "<<100*lumi1er/lumi1<<" %) at sbil="<<sbil1<<endl;
  
  //Dataset 2
  float sbil2 = 10;// hz/ub
  float lumi2 = lumi1;// 
  float lumi2er=lumi2 * (sbil2 * nonlinearity);
  cout<<"lumi 2: "<<lumi2<<" +/- "<<lumi2er<<" ( "<<100*lumi2er/lumi2<<" %) at sbil="<<sbil2<<endl;
  
  //Combination with correct error estimation
  float lumi = lumi1 + lumi2;
  float lumier = sqrt(lumi1er*lumi1er + lumi2er*lumi2er + lumi1er*lumi2er); //note: need to acount the correlation 
  cout<<"using error propagation, lumi total: "<<lumi<<" +/- "<<lumier<<" ( "<<100*lumier/lumi<<" %)"<<endl;

  ///////////////////////////////////
  // Now lets try with sbil averaging

  //Method 1: weight by number of LS's 
  float sbil1w_method1=100;                             // lets say it took 100 LS to get lumi1 (actual scale drops out)
  float sbil2w_method1=sbil1w_method1 * (sbil1/sbil2) ; // then it took 10 LS to get lumi2 for the values of sbil above (sbil2 = 10 sbil1), assuming same number of bunches
  float sbilw_method1=(sbil1w_method1 * sbil1 + sbil2w_method1 * sbil2)/(sbil1w_method1 + sbil2w_method1);
  float lumier_method1 = lumi * sbilw_method1 * nonlinearity;
  cout<<"sbil weight by # LS, lumi total: "<<lumi<<" +/- "<<lumier_method1<<" ( "<<100*lumier_method1/lumi<<" %)"<<endl;

  //Method 2: weight by lumi 
  float sbilw_method2=(lumi1 * sbil1 + lumi2 * sbil2)/(lumi1 + lumi2);
  float lumier_method2 = lumi * sbilw_method2 * nonlinearity;
  cout<<"sbil weight by lumi, lumi total: "<<lumi<<" +/- "<<lumier_method2<<" ( "<<100*lumier_method2/lumi<<" %)"<<endl;
   
}

