
#include "fitAfterglowTrain.C"

TString input="./366800.root";
std::vector<int> FBList={1113,1575,2007,2901};
int NColliding=36;   // number of colliding bunches in the train (train must be contiguous) 
int NBins=100;       // number of bcids in train plus tail (full fit range)


void fitAfterglowRun(TString inputfile=input){

  TFile File(inputfile.Data(),"read");
  if(File.IsZombie()) return;
 
  
  TString TName=inputfile;
  TName.ReplaceAll(".root","_output.root");
  TFile outputfile(TName,"recreate");
  gROOT->cd();
  makeTree(inputfile);
  
  
  TIter next(File.GetListOfKeys());
  while (TObject* key = next()) {
    if(!key) continue;

    TString kname(key->GetName());
    if(!kname.Contains("RawLumiAvg")) continue;
    
    TString LSBlockName=kname.Data(); //"RawLumiAvg_366800_2_3_3";
    cout<<LSBlockName<<endl;
    
    for(int i=0;i<FBList.size();i++){
      fitAfterglowTrain(inputfile, LSBlockName, FBList[i], NColliding, NBins);
    }

    
  }


  outputfile.cd();
  Tree->Write();
  outputfile.ls();
  outputfile.Close();
  
}
