#include <list>


std::list<int>  readFile(std::string file){

  std::list<int> newlist;

  ifstream cfile(file.c_str());
  if (!cfile.is_open()){
    std::cout << "Unable to open coordinates"<<endl;
    return newlist;
  }
  std::string cline;
  int mod;
  while (std::getline(cfile,cline)){
    std::stringstream iss(cline);
    iss>>mod;

    newlist.push_back(mod);
  }

  newlist.sort();
  return newlist;

}


void overlapModuleLists(){
  

  std::list<int> list1=readFile("BRILAnalysisCode/PCCAnalysis/test/mergedModuleList.txt");
  std::list<int> list2=readFile("BRILAnalysisCode/PCCAnalysis/test/veto_master_VdM_ABCD_2018_newcuts.txt");
  
  
  std::list<int> out;
  std::set_intersection(list1.begin(), list1.end(), 
			list2.begin(), list2.end(),
			std::back_inserter(out));

  cout<<"l1 ="<<list1.size()
      <<" , l2 ="<<list2.size()
      <<" , overlap ="<<out.size()<<endl;

}
