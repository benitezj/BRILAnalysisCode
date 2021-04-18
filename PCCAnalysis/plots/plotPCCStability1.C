#include <iostream>
#include <fstream>
#include <string>

void plotPCCStability1(TString inpath="/eos/user/a/asehrawa/B1/Run2018A", TString inpath1="/eos/user/a/asehrawa/B1/Run2018B",TString inpath2="/eos/user/a/asehrawa/B1/Run2018C",TString inpath3="/eos/user/a/asehrawa/B2/Run2018A",TString inpath4="/eos/user/a/asehrawa/B2/Run2018B",TString inpath5="/eos/user/a/asehrawa/B2/Run2018C",TString inpath6="/eos/user/a/asehrawa/B3/Run2018A",TString inpath7="/eos/user/a/asehrawa/B3/Run2018B",TString inpath8="/eos/user/a/asehrawa/B3/Run2018C",TString inpath9="/eos/user/a/asehrawa/F1/Run2018A",TString inpath10="/eos/user/a/asehrawa/F1/Run2018B",TString inpath11="/eos/user/a/asehrawa/F1/Run2018C",TString inpath12="/eos/user/a/asehrawa/F2/Run2018A",TString inpath13="/eos/user/a/asehrawa/F2/Run2018B",TString inpath14="/eos/user/a/asehrawa/F2/Run2018C",TString inpath15="/eos/user/a/asehrawa/F3/Run2018A",TString inpath16="/eos/user/a/asehrawa/F3/Run2018B",TString inpath17="/eos/user/a/asehrawa/F3/Run2018C",  TString ref=""){

  gROOT->ProcessLine(".x /afs/cern.ch/user/a/asehrawa/CMSSW_10_2_2/src/BRILAnalysisCode/rootlogon.C");

  ifstream myfile((inpath+"/ls.dat").Data());
  if (!myfile.is_open()){
    cout << "Unable to open file"; 
    return;

  }
  cout<<"opened "<<(inpath+"/ls.dat")<<std::endl;


  ifstream myfile1((inpath1+"/ls.dat").Data());
  if (!myfile1.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath1+"/ls.dat")<<std::endl;


  ifstream myfile2((inpath2+"/ls.dat").Data());
  if (!myfile2.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath2+"/ls.dat")<<std::endl;


  ifstream myfile3((inpath3+"/ls.dat").Data());
  if (!myfile3.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath3+"/ls.dat")<<std::endl;



  ifstream myfile4((inpath4+"/ls.dat").Data());
  if (!myfile4.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath4+"/ls.dat")<<std::endl;



  ifstream myfile5((inpath5+"/ls.dat").Data());
  if (!myfile5.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath5+"/ls.dat")<<std::endl;



  ifstream myfile6((inpath6+"/ls.dat").Data());
  if (!myfile6.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath6+"/ls.dat")<<std::endl;



  ifstream myfile7((inpath7+"/ls.dat").Data());
  if (!myfile7.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath7+"/ls.dat")<<std::endl;



  ifstream myfile8((inpath8+"/ls.dat").Data());
  if (!myfile8.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath8+"/ls.dat")<<std::endl;



  ifstream myfile9((inpath9+"/ls.dat").Data());
  if (!myfile9.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath9+"/ls.dat")<<std::endl;



  ifstream myfile10((inpath10+"/ls.dat").Data());
  if (!myfile10.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath10+"/ls.dat")<<std::endl;


  ifstream myfile11((inpath11+"/ls.dat").Data());
  if (!myfile11.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath11+"/ls.dat")<<std::endl;



  ifstream myfile12((inpath12+"/ls.dat").Data());
  if (!myfile12.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath12+"/ls.dat")<<std::endl;



  ifstream myfile13((inpath13+"/ls.dat").Data());
  if (!myfile13.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath13+"/ls.dat")<<std::endl;



  ifstream myfile14((inpath14+"/ls.dat").Data());
  if (!myfile14.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath14+"/ls.dat")<<std::endl;


  ifstream myfile15((inpath15+"/ls.dat").Data());
  if (!myfile15.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath15+"/ls.dat")<<std::endl;


  ifstream myfile16((inpath16+"/ls.dat").Data());
  if (!myfile16.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath16+"/ls.dat")<<std::endl;


  ifstream myfile17((inpath17+"/ls.dat").Data());
  if (!myfile17.is_open()){
    cout << "Unable to open file";
    return;
  }
  cout<<"opened "<<(inpath17+"/ls.dat")<<std::endl;



  ///create histogram
  int Lumicounter=0;

  TH2F H("H","", 200,0,20000,300,0,0.5);
  TH2F H1("H1","", 200,0,20000,300,0,0.5);
  TH2F H2("H2","", 200,0,20000,300,0,0.5);
  TH2F H3("H3","", 200,0,20000,300,0,0.5);
  TH2F H4("H4","", 200,0,20000,300,0,0.5);
  TH2F H5("H5","", 200,0,20000,300,0,0.5);
  TH2F H6("H6","", 200,0,20000,300,0,0.5);
  TH2F H7("H7","", 200,0,20000,300,0,0.5);
  TH2F H8("H8","", 200,0,20000,300,0,0.5);
  TH2F H9("H9","", 200,0,20000,300,0,0.5);
  TH2F H10("H10","", 200,0,20000,300,0,0.5);
  TH2F H11("H11","", 200,0,20000,300,0,0.5);
  TH2F H12("H12","", 200,0,20000,300,0,0.5);
  TH2F H13("H13","", 200,0,20000,300,0,0.5);
  TH2F H14("H14","", 200,0,20000,300,0,0.5);
  TH2F H15("H15","", 200,0,20000,300,0,0.5);
  TH2F H16("H16","", 200,0,20000,300,0,0.5);
  TH2F H17("H17","", 200,0,20000,300,0,0.5);

  
  std::string line;
  int run=0;
  int ls=0;

  double totL=0;   //lumi for given LS
  double totL1=0;
  double totL2=0;
  double totL3=0;
  double totL4=0;
  double totL5=0;
  double totL6=0;
  double totL7=0;
  double totL8=0;
  double totL9=0;
  double totL10=0;
  double totL11=0;
  double totL12=0;
  double totL13=0;
  double totL14=0;
  double totL15=0;
  double totL16=0;
  double totL17=0;

  double totLRef=0;//lumi for given LS

  while (std::getline(myfile, line), std::getline(myfile1, line), std::getline(myfile2, line), std::getline(myfile3, line), std::getline(myfile4, line), std::getline(myfile5, line), std::getline(myfile6, line), std::getline(myfile7, line), std::getline(myfile8, line), std::getline(myfile9, line), std::getline(myfile10, line), std::getline(myfile11, line), std::getline(myfile12, line), std::getline(myfile13, line), std::getline(myfile14, line), std::getline(myfile15, line),  std::getline(myfile16, line), std::getline(myfile17, line)){


    //cout<<line;
    std::stringstream iss(line);
    iss>>run>>ls>>totL>>totLRef;


    std::stringstream iss1(line);
    iss1>>run>>ls>>totL1>>totLRef;

    std::stringstream iss2(line);
    iss2>>run>>ls>>totL2>>totLRef;

    std::stringstream iss3(line);
    iss3>>run>>ls>>totL3>>totLRef;

    std::stringstream iss4(line);
    iss4>>run>>ls>>totL4>>totLRef;

    std::stringstream iss5(line);
    iss5>>run>>ls>>totL5>>totLRef;

    std::stringstream iss6(line);
    iss6>>run>>ls>>totL6>>totLRef;

    std::stringstream iss7(line);
    iss7>>run>>ls>>totL7>>totLRef;

    std::stringstream iss8(line);
    iss8>>run>>ls>>totL8>>totLRef;

    std::stringstream iss9(line);
    iss9>>run>>ls>>totL9>>totLRef;

    std::stringstream iss10(line);
    iss10>>run>>ls>>totL10>>totLRef;

    std::stringstream iss11(line);
    iss11>>run>>ls>>totL11>>totLRef;

    std::stringstream iss12(line);
    iss12>>run>>ls>>totL12>>totLRef;

    std::stringstream iss13(line);
    iss13>>run>>ls>>totL13>>totLRef;

    std::stringstream iss14(line);
    iss14>>run>>ls>>totL14>>totLRef;

    std::stringstream iss15(line);
    iss15>>run>>ls>>totL15>>totLRef;

    std::stringstream iss16(line);
    iss16>>run>>ls>>totL16>>totLRef;

    std::stringstream iss17(line);
    iss17>>run>>ls>>totL17>>totLRef;


    float ratio=0;
    float ratio1=0;
    float ratio2=0;
    float ratio3=0;
    float ratio4=0;
    float ratio5=0;
    float ratio6=0;
    float ratio7=0;
    float ratio8=0;
    float ratio9=0;
    float ratio10=0;
    float ratio11=0;
    float ratio12=0;
    float ratio13=0;
    float ratio14=0;
    float ratio15=0;
    float ratio16=0;
    float ratio17=0;

    if(totLRef>0){

      ratio=totL/totLRef;
      ratio1=totL1/totLRef;
      ratio2=totL2/totLRef;
      ratio3=totL3/totLRef;
      ratio4=totL4/totLRef;
      ratio5=totL5/totLRef;
      ratio6=totL6/totLRef;
      ratio7=totL7/totLRef;
      ratio8=totL8/totLRef;
      ratio9=totL9/totLRef;
      ratio10=totL10/totLRef;
      ratio11=totL11/totLRef;
      ratio12=totL12/totLRef;
      ratio13=totL13/totLRef;
      ratio14=totL14/totLRef;
      ratio15=totL15/totLRef;
      ratio16=totL16/totLRef;
      ratio17=totL17/totLRef;


      H.Fill(Lumicounter,ratio);
      H1.Fill(Lumicounter,ratio1);
      H2.Fill(Lumicounter,ratio2);
      H3.Fill(Lumicounter,ratio3);
      H4.Fill(Lumicounter,ratio4);
      H5.Fill(Lumicounter,ratio5);
      H6.Fill(Lumicounter,ratio6);
      H7.Fill(Lumicounter,ratio7);
      H8.Fill(Lumicounter,ratio8);
      H9.Fill(Lumicounter,ratio9);
      H10.Fill(Lumicounter,ratio10);
      H11.Fill(Lumicounter,ratio11);
      H12.Fill(Lumicounter,ratio12);
      H13.Fill(Lumicounter,ratio13);
      H14.Fill(Lumicounter,ratio14);
      H15.Fill(Lumicounter,ratio15);
      H16.Fill(Lumicounter,ratio16);
      H17.Fill(Lumicounter,ratio17);

      Lumicounter++;

    }
  }
    
  myfile.close();
  myfile1.close();
  myfile2.close();
  myfile3.close();
  myfile4.close();
  myfile5.close();
  myfile6.close();
  myfile7.close();
  myfile8.close();
  myfile9.close();
  myfile10.close();
  myfile11.close();
  myfile12.close();
  myfile13.close();
  myfile14.close();
  myfile15.close();
  myfile16.close();
  myfile17.close();
  
  TCanvas C;
 
  H.GetXaxis()->SetTitle(" # of lumi sections ");
  H.GetYaxis()->SetTitle(" lumi ratio ");

  H.Draw("colz");
  H1.Draw("colzsame");
  H2.Draw("colzsame");
  H3.Draw("colzsame");
  H4.Draw("colzsame");
  H5.Draw("colzsame");
  H6.Draw("colzsame");
  H7.Draw("colzsame");
  H8.Draw("colzsame");
  H9.Draw("colzsame");
  H10.Draw("colzsame");
  H11.Draw("colzsame");
  H12.Draw("colzsame");
  H13.Draw("colzsame");
  H14.Draw("colzsame");
  H15.Draw("colzsame");
  H16.Draw("colzsame");
  H17.Draw("colzsame");


  C.Print(inpath+"/combined.png"); 

  TCanvas C1;

  TProfile * P = H.ProfileX();
  P->Draw(); 
  TProfile * P1 = H1.ProfileX();
  P1->Draw("same");
  TProfile * P2 = H2.ProfileX();
  P2->Draw("same");
  TProfile * P3 = H3.ProfileX();
  P3->Draw("same");
  TProfile * P4 = H4.ProfileX();
  P4->Draw("same");
  TProfile * P5 = H5.ProfileX();
  P5->Draw("same");
  TProfile * P6 = H6.ProfileX();
  P6->Draw("same");
  TProfile * P7 = H7.ProfileX();
  P7->Draw("same");
  TProfile * P8 = H8.ProfileX();
  P8->Draw("same");
  TProfile * P9 = H9.ProfileX();
  P9->Draw("same");
  TProfile * P10 = H10.ProfileX();
  P10->Draw("same");
  TProfile * P11 = H11.ProfileX();
  P11->Draw("same");
  TProfile * P12 = H12.ProfileX();
  P12->Draw("same");
  TProfile * P13 = H13.ProfileX();
  P13->Draw("same");
  TProfile * P14 = H14.ProfileX();
  P14->Draw("same");
  TProfile * P15 = H15.ProfileX();
  P15->Draw("same");
  TProfile * P16 = H16.ProfileX();
  P16->Draw("same");
  TProfile * P17 = H17.ProfileX();
  P17->Draw("same");

  C1.Print(inpath+"/ProfileXcombined.png");
 
  gROOT->ProcessLine(".q");
}
