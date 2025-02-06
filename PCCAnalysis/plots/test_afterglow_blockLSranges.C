#include <iostream>
#include <vector>
//#include <dirent.h>
#include "globals.h"

void test_afterglow_blockLSranges(TString inpath, TString RUNLIST="") {
  cout<<"input path: "<<inpath<<endl;

  std::string runlist;
  if(RUNLIST.CompareTo("")!=0) runlist=RUNLIST.Data();
  cout<<runlist<<endl;

  ////////////////////
  std::stringstream ss(runlist.c_str());
  int Run;
  while (ss >> Run) {
    cout<<Run<<endl;  
    ss.ignore(1);   

    TString filename=inpath+"/"+Run+".root";
    if(gSystem->AccessPathName(filename.Data())){ cout<<"bad file: "<<filename<<endl; continue;}

    TFile InputFile(filename.Data(),"read");
    if(InputFile.IsZombie()) continue;

    std::map<int,int> first;
    std::map<int,int> last;

    
    TIter next(InputFile.GetListOfKeys());
    TObject* key;
    int counter=0;
    while ((key = next())) {
      //cout<<Run<<" "<<key->GetName()<<endl;
      //continue;
      
      TString kname(key->GetName());    //TString("RawHISTAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
      if(!kname.Contains("RawLumiAvg_")) continue;

      counter++;
      
      TObjArray * a = kname.Tokenize("_");
      int r=atoi(((TObjString*)(*a)[1])->GetName());
      int l=atoi(((TObjString*)(*a)[2])->GetName());
      int ls1=atoi(((TObjString*)(*a)[3])->GetName());
      int ls2=atoi(((TObjString*)(*a)[4])->GetName());

      first[l]=ls1;
      last[l]=ls2;
      
      cout<<r<<" "<<l<<" "<<ls1<<" "<<ls2<<endl;
    }

    for(int c=0; c<counter; c++){
      std::cout<<c<<" "<<first[c]<<" "<<last[c]<<std::endl;
    }
    
  }
  
}
