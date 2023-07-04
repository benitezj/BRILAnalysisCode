
#include "fitAfterglowTrain.C"

//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={366800}, std::vector<int> LeadBCIDList={1113,1575,2007,2901}, int NTRAINBCIDS=36, int NBCIDS=100){


//Fill 7036
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={1}, int NTRAINBCIDS=11, int NBCIDS=100){//1-wagon

//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={188,1070,1964,2858}, int NTRAINBCIDS=103, int NBCIDS=200){//2-wagons
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995}, std::vector<int> LeadBCIDList={188}, int NTRAINBCIDS=103, int NBCIDS=200){

void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={454,1336,2230,3124}, int NTRAINBCIDS=158, int NBCIDS=240){//3-wagons
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995}, std::vector<int> LeadBCIDList={3124}, int NTRAINBCIDS=158, int NBCIDS=240){

  
  
  TString OutfileName=inpath+"/fitAfterglow_output.root";
  TFile outputfile(OutfileName,"recreate");
  
  gROOT->cd();
  makeTree();
  

  for(int r=0;r<RunList.size();r++){
  
    TFile File((inpath+"/"+RunList[r]+".root").Data(),"read");
    if(File.IsZombie()){
      cout<<(inpath+"/"+RunList[r]).Data()<<" not found"<<endl;
      return;
    }
  
    TIter next(File.GetListOfKeys());
    while (TObject* key = next()) {
      TString kname(key->GetName());
      if(!kname.Contains("RawLumiAvg")) continue;
      cout<<key->GetName()<<endl;

      TH1F* H=(TH1F*)File.Get(key->GetName());
    
      for(int i=0;i<LeadBCIDList.size();i++)
	fitAfterglowTrain(H,key->GetName(), LeadBCIDList[i], NTRAINBCIDS, NBCIDS);

      //break;
    }

  }

  
  outputfile.cd();
  if(Tree) Tree->Write();
  outputfile.ls();
  outputfile.Close();
  
}
