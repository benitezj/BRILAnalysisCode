#include <string>
#include <map>
#include <iostream>

void mappinglsdotdat() {  

  std::ifstream myfile("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_oldveto_Run2018A/Run2018A/ls.dat"); 
  
  if (!myfile) { 
    std::cout << "Error, could not open old lsdodat file." << std::endl; 
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
      
    cluster_count[run][ls]=count;

    //std::cout<<run<<" "<<ls<<"  "<<cluster_count[run][ls]<<std::endl;
  }
  

  for(auto & outer_map_pair : cluster_count) {
    cout << outer_map_pair.first << " contains: " << endl;
    for(auto & inner_map_pair : outer_map_pair.second) {
      cout << "old "<<inner_map_pair.first << ": " << inner_map_pair.second << endl;
    }
  }


  myfile.close();  
  


  std::ifstream myfile1("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRIL/PCC_lumi_newveto_2018A/Run2018A_ZB/ls.dat"); 
  
  if (!myfile1) { 
    std::cout << "Error, could not open new lsdotdat file." << std::endl; 
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
   
    //cluster_count.insert(std::pair<int, int> (ls, run), count);
    //cout<<run<< " "<<ls << "  "<<count<<endl;      
    //cluster_count.find(ls)->second = count;
    
    cluster_count1[run1][ls1]=count1;

    //std::cout<<run<<" "<<ls<<"  "<<cluster_count[run][ls]<<std::endl;
  }
  

  for(auto & outer_map_pair1 : cluster_count1) {
    cout << outer_map_pair1.first << " contains: " << endl;
    for(auto & inner_map_pair1 : outer_map_pair1.second) {
      cout << "new "<<inner_map_pair1.first << ": " << inner_map_pair1.second << endl;
    }
  }



  myfile1.close();  
  

}
