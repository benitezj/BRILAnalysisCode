#include "/home/ashish/json/single_include/nlohmann/json.hpp"
#include <iostream>
#include <fstream>
using json = nlohmann::json;
using namespace std;
void readjson(){
  
  std::ifstream file("/home/ashish/vcpkg/normtag_hfoc.json");
  json j;
  file >> j;
  //std::cout<<j<<endl;
  
  for (json::iterator it = j.begin(); it != j.end(); ++it) {
    //std::cout << *it << '\n';
  }
  
  for (auto& elem : j) {
    //std::cout << "str = " << elem.at(0).get<std::string>() << std::endl;
    auto &subelem = elem.at(1);
    //std::cout<<elem.at(1)<<std::endl;
    for (auto &subsub : subelem) {
      //std::cout << subsub.key() << " : " << subsub.value() << std::endl;
    }
    for (json::iterator it1 = elem.at(1).begin(); it1 != elem.at(1).end(); ++it1) {
      std::cout << "run number: " << it1.key() << ", good ls range:" << it1.value() << '\n';
      //std::cout << *it1 << '\n';
    }
    
    //std::cout << element << '\n';
  }
  //std::cout<<j.size()<<std::endl;
}


/**......................
   auto arr = nlohmann::json::parse("[[\"hfoc15PaperV0\", {\"246958\": [[1, 71]]}],"
   "[\"hfoc16PaperV0\", {\"246959\": [[1, 98]]}]]");
   
   for (auto &elem : arr) {
   std::cout << "str = " << elem.at(0).get<std::string>() << std::endl;
   auto &subelem = elem.at(1);
   for (auto &subsub : subelem) {
   std::cout << subsub.key() << " : " << subsub.value() << std::endl;
   }
   }
   ....................**/
// path to hfoc json file /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json
