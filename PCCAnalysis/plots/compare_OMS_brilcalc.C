#include <iostream>
#include <fstream>
#include <string>
#include <TString.h>
#include "rootlogon.C"

bool recorded = 1;

void getlumi(TString path, std::map<int,float> &lummap, int position=6, float scale=1., TString filter=""){
  cout<<"Reading:"<<path<<endl;
  
  //OMS format
  //run_number,fill_number,duration,start_time,end_time,components_out,delivered_lumi,recorded_lumi,l1_triggers_counter,l1_hlt_mode_stripped,hlt_key,era,initial_prescale_index
  //390950,10489,345,2025-04-22 06:58:49,2025-04-22 07:04:34,"[]",1.4655901E-4,8.6430933E-5,22882060,collisions2025/v6,/cdaq/special/2025/FirstCollisions/v0.3.0/HLT/V1,Run2025A,0

  //brilcalc format
  //run:fill,time,nls,ncms,delivered(/ub),recorded(/ub)
  //390950:10489,04/22/25 07:04:39,11,0,57.624802023,0
  //390950,10489,04/22/25 07,04,39,11,0,57.624802023,0
    
  ifstream myfile(path.Data());
  if (!myfile.is_open()){
    std::cout << "Unable to open lumi file: "<<path<<std::endl;
    return;
  }

  TString Filter;
  int run;
  float lumi=0.;
  float totallum=0.;
  int counter=0;
  std::string line;
  while (std::getline(myfile, line)){
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> parsed_elements;
    while (std::getline(ss, token, ',')) 
      parsed_elements.push_back(token);

    if(parsed_elements.size()==0){
      cout<<"Bad line: "<<line<<endl;
      continue;
    }
    
    run=atoi(parsed_elements[0].c_str());
    if(run<100000 || run>1000000 ){
      cout<<"Bad run: "<<line<<endl;
      continue; //run number sanity
    }

    if(filter.CompareTo("")!=0){
      Filter=parsed_elements[9].c_str();//this only applies to OMS data
      if(! Filter.Contains(filter))
	continue;
    }
    
    lumi = atof(parsed_elements[position+recorded].c_str()) * scale;
    lummap[run] = lumi;
    totallum += lumi;
    counter++;
  }

  cout<<"Done reading: "<<path<<endl;
  cout<<"N runs: "<<counter<<endl;
  cout<<"Total lumi: "<<totallum<<endl;
}


void compare_OMS_brilcalc(){

  rootlogon();
  
  std::map<int,float> omslumi;  
  getlumi("./2025_lumi_publicplot/2025_oms.csv", omslumi, 6, 1,"collisions2025"); // 1/pb

  std::map<int,float> onlinelumi;
  getlumi("./2025_lumi_publicplot/2025_online.txt", onlinelumi, 7, 1E-6); //convert 1/ub -> 1/pb

  std::map<int,float> normtaglumi;
  getlumi("./2025_lumi_publicplot/2025_normtag.txt", normtaglumi, 7, 1E-6); //convert 1/ub -> 1/pb

  //return;
  
  //for loop to match the lumi per run
  for (const auto& pair : omslumi) {
    std::cout << pair.first << "," << pair.second <<","<< onlinelumi[pair.first]  <<","<< normtaglumi[pair.first] << std::endl;
  }  

  

  
  /////////////////////////////////////////////////////
  ///   make the plots
  ///////////////////////////////////////////////////
  //gStyle->SetOptStat(0);

  //TCanvas C;
  

  //C.Print(outpath+"/"+(long)Run+"_linearity_tgraph.png");

  
  
  gROOT->ProcessLine(".q");
}

