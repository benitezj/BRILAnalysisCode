#include <list>

std::list<int> mergedlist;


void mergeFile(std::string file){

  std::list<int> newlist;

  ifstream cfile(file.c_str());
  if (!cfile.is_open()){
    std::cout << "Unable to open coordinates"<<endl;
    return;
  }
  std::string cline;
  int mod;
  while (std::getline(cfile,cline)){
    std::stringstream iss(cline);
    iss>>mod;

    newlist.push_back(mod);
  }

  newlist.sort();
  mergedlist.merge(newlist);

}


void mergeModuleLists(){

  mergeFile("/afs/cern.ch/user/b/benitezj/www/BRIL/BadModuleStudy/SiPixelQuality_byPCL_prompt_v1/ModuleVetoList.txt");
  mergeFile("/afs/cern.ch/user/b/benitezj/www/BRIL/BadModuleStudy/SiPixelQuality_byPCL_prompt_v2/ModuleVetoList.txt");
  mergeFile("/afs/cern.ch/user/b/benitezj/www/BRIL/BadModuleStudy/SiPixelQuality_byPCL_stuckTBM_v1/ModuleVetoList.txt");
  
  mergedlist.unique();
  
  cout<<mergedlist.size()<<endl;

  for (std::list<int>::iterator it=mergedlist.begin(); it!=mergedlist.end(); ++it)
    std::cout <<*it<<endl;

}
