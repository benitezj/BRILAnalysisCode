#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"

void verify_CSV_NBX(TString inpath, std::string runlist=""){

  std::stringstream ss(runlist.c_str());
  int Run;
  while(ss >> Run) {
    cout<<Run<<":"<<endl;
    
    ///Open the lumi csv file
    TString infile=inpath+"/"+Run+".csv";
    ifstream csvfile (infile.Data());
    if (!csvfile.is_open()){
      cout << "Unable to open file: "<<infile.Data()<<endl; 
      ss.ignore(1); 
      continue;
    }
    

    std::string line;
    int run=0;
    int ls=0;
    float rawL=0.;
    float bxL=0.;
    int maxLS=0;
    int Nls=0;
    while (std::getline(csvfile, line)){
      std::stringstream iss(line);
      std::string token;
      
      ///read the run number
      std::getline(iss,token, ',');
      std::stringstream runiss(token);
      runiss>>run;
      
      ///read the LS
      std::getline(iss,token, ',');
      std::stringstream lsiss(token);
      lsiss>>ls;
  
      ///read the  Lumi per ls
      std::getline(iss,token, ',');
      std::stringstream totLiss(token);
      totLiss>>rawL;


      
      stringstream ss(line);
      string substr;
      getline(ss, substr, ',');//run
      getline(ss, substr, ',');//ls
      getline(ss, substr, ',');//total

      unsigned ncoll=0;
      unsigned nbx=0;
      std::getline(iss,token, ',');
      std::stringstream bxLiss(token);

      while (ss.good()) {
	getline(ss, substr, ',');
	bxL=atof(substr.c_str());
	//cout<<bxL<<endl;
	nbx++;
	if(bxL>0.1) ncoll++;
      }

      cout<<run<<" "<<ls<<" "<<ncoll<<" "<<nbx<<endl;
      if(nbx!=3564)cout<<"ERROR with number total BX in LS"<<endl;
      
    }

    csvfile.close();

    ss.ignore(1);
  }
  
  gROOT->ProcessLine(".q");
}
