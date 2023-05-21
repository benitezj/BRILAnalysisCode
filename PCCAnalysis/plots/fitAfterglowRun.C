
#include "fitAfterglowTrain.C"
//,1575,2007,2901
void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={366800}, std::vector<int> LeadBCIDList={1113}, int NTRAINBCIDS=36, int NBCIDS=100){


  //TString input="./366800.root";
  //std::vector<int> FBList={1113,1575,2007,2901};
  //NColliding=36;   // number of colliding bunches in the train (train must be contiguous) 
  //NBins=100;       // number of bcids in train plus tail (full fit range)
  
  
  TString OutfileName=inpath+"/fitAfterglow_output.root";
  TFile outputfile(OutfileName,"recreate");
  
  gROOT->cd();
  makeTree();
  

  TFile File((inpath+"/"+RunList[0]+".root").Data(),"read");
  if(File.IsZombie()){
    cout<<(inpath+"/"+RunList[0]).Data()<<" not found"<<endl;
    return;
  }
  
  TIter next(File.GetListOfKeys());
  while (TObject* key = next()) {
    if(!key) continue;

    TString kname(key->GetName());
    if(!kname.Contains("RawLumiAvg")) continue;
    cout<<key->GetName()<<endl;

    TH1F* H=(TH1F*)File.Get(key->GetName());
    
    for(int i=0;i<LeadBCIDList.size();i++)
      fitAfterglowTrain(H,key->GetName(), LeadBCIDList[i], NTRAINBCIDS, NBCIDS);
       
  }


  outputfile.cd();
  if(Tree) Tree->Write();
  outputfile.ls();
  outputfile.Close();
  
}
