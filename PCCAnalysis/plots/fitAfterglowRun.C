
#include "fitAfterglowTrain.C"

//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={366800}, std::vector<int> LeadBCIDList={1113,1575,2007,2901}, int NCOLLIDINGBCIDS=36, int NTOTALBCIDS=100){


///////////////
//  2018 data
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={1}, int NCOLLIDINGBCIDS=11, int NTOTALBCIDS=100){//1-wagon
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={188,1070,1964,2858}, int NCOLLIDINGBCIDS=103, int NTOTALBCIDS=200){//2-wagons
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995}, std::vector<int> LeadBCIDList={188}, int NCOLLIDINGBCIDS=103, int NTOTALBCIDS=200){
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={454,1336,2230,3124}, int NCOLLIDINGBCIDS=158, int NTOTALBCIDS=240){//3-wagons
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={1336}, int NCOLLIDINGBCIDS=158, int NTOTALBCIDS=240){//3-wagons
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995}, std::vector<int> LeadBCIDList={3124}, int NCOLLIDINGBCIDS=158, int NTOTALBCIDS=240){


///////////////
///    2022 Data
//600b Nov 2022 (fill 8383)  --> Results from tuple:  <T1f> = 0.03134 , <T2a> = 8.178e-4, <T2b> = 0.01109
//std::vector<int> RunList={361948}; std::vector<int> LeadBCIDList={1018,2806}; int NCOLLIDINGBCIDS=189; int NTOTALBCIDS=250;

//1800b Nov 2022 (fill 8385) --> Results from tuple: <T1f> = 0.03284 , <T2a> = 8.207e-4, <T2b> = 0.01107
std::vector<int> RunList={361957}; std::vector<int> LeadBCIDList={66,2748}; int NCOLLIDINGBCIDS=208; int NTOTALBCIDS=270;


//////////////////////////////////////////////
void fitAfterglowRun(TString inpath){

  if(inpath.CompareTo("")==0) return;

  
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
	fitAfterglowTrain(H,key->GetName(), LeadBCIDList[i], NCOLLIDINGBCIDS, NTOTALBCIDS,inpath);

    }

  }

  
  outputfile.cd();
  if(Tree) Tree->Write();
  outputfile.ls();
  outputfile.Close();
  
}
