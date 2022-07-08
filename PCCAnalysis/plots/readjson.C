//#include </home/ashish/vcpkg/installed/x64-linux/include/json/json.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include<stdio.h>
void readjson(){
  
  //Json::Value val;
  //Json::Reader reader;
  std::string normtag;
  std::string run;
  std::string goodlsrangebegin;
  std::string goodlsrangeend;
  
  std::string line;
  int lineNum;
  std::ifstream file("/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json");
  if (file.is_open()){
    lineNum = 0;
    while(std::getline(file, line)) {
      lineNum++;
      
      //reader.parse(file, line)
      
      ///**........................
      std::stringstream iss(line);
      std::string token;
      
      std::getline(iss,token, ',');
      std::stringstream normtagiss(token);
      normtagiss>>normtag;
      
      std::getline(iss,token, ',');
      std::stringstream runiss(token);
      runiss>>run;
      
      std::getline(iss,token, ',');
      std::stringstream lsissbegin(token);
      lsissbegin>>goodlsrangebegin;
      
      std::getline(iss,token, ',');
      std::stringstream lsissend(token);
      lsissend>>goodlsrangeend;
      
      //std::cout<<normtag<<" "<<run<<"  "<<goodlsrangebegin<<" "<<goodlsrangeend<<endl;
      cout <<line<<endl;
      //.................**/
  
    }
  }
}

///cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json
