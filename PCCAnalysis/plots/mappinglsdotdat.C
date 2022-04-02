#include <string>
#include <map>
#include <iostream>

void mappinglsdotdat() {  
  
  std::ifstream myfile("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_oldveto_Run2018A/Run2018A/ls.dat"); 
  
  if (!myfile) { 
    std::cout << "Error, could not open old veto lsdodat file." << std::endl; 
    return;
  }
  
  std::string line;
  
  // Construct your map.
  std::map<int, std::map<int, float> > cluster_count;
  
  // Read the file and load the data:
  int run, ls;
  float count;
  
  while (std::getline(myfile, line)){
    
    std::getline(myfile, line);
    std::stringstream iss(line);
    iss>>run>>ls>>count;
    if(run==315257) {      
      cluster_count[run][ls]=count;
      //std::cout<<run<<" "<<ls<<"  "<<cluster_count[run][ls]<<std::endl;
    }
    
  }
  
  for(auto & outer_map_pair : cluster_count) {
    for(auto & inner_map_pair : outer_map_pair.second) {
      cout << "old "<<outer_map_pair.first << " "<<inner_map_pair.first << " " << inner_map_pair.second<<endl;
    }
  }
  
  
  myfile.close();  
  
  std::ifstream myfile1("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_newveto_2018A/Run2018A_ZB/ls.dat"); 
  
  if (!myfile1) { 
    std::cout << "Error, could not open new veto lsdotdat file." << std::endl; 
    return;
  }
  
  std::string line1;
  
  // Construct your map.
  std::map<int, std::map<int, float> > cluster_count1;
  
  // Read the file and load the data:
  int run1, ls1;
  float count1;
  
  while (std::getline(myfile1, line1)){
    
    std::getline(myfile1, line1);
    std::stringstream iss1(line1);
    iss1>>run1>>ls1>>count1;
    
    if(run1==315257) {   
      cluster_count1[run1][ls1]=count1;
    //std::cout<<run<<" "<<ls<<"  "<<cluster_count[run][ls]<<std::endl;
    }
    
  }
  
  for(auto & outer_map_pair1 : cluster_count1) {
    for(auto & inner_map_pair1 : outer_map_pair1.second) {
      cout << "new "<<outer_map_pair1.first << "  "<< inner_map_pair1.first << "  " << inner_map_pair1.second << endl;
    }
  }
  
  myfile1.close();  
  

}
