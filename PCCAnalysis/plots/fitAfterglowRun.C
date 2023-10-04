
#include "fitAfterglowTrain.C"

TString outputpath="./tmp";

///////////////
//  2018 data
//TString inpath="./AfterglowModel2018"; //2018 data final veto
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={1}, int NCOLLIDINGBCIDS=11, int NTOTALBCIDS=100){//1-wagon
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={188,1070,1964,2858}, int NCOLLIDINGBCIDS=103, int NTOTALBCIDS=200){//2-wagons
//std::vector<int> RunList={320995}; std::vector<int> LeadBCIDList={188}; int NCOLLIDINGBCIDS=103; int NTOTALBCIDS=200;
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={454,1336,2230,3124}, int NCOLLIDINGBCIDS=158, int NTOTALBCIDS=240){//3-wagons
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995,320996}, std::vector<int> LeadBCIDList={1336}, int NCOLLIDINGBCIDS=158, int NTOTALBCIDS=240){//3-wagons
//void fitAfterglowRun(TString inpath=".", std::vector<int> RunList={320995}, std::vector<int> LeadBCIDList={3124}, int NCOLLIDINGBCIDS=158, int NTOTALBCIDS=240){

//std::vector<int> RunList={320995}; std::vector<int> LeadBCIDList={188+55}; int NCOLLIDINGBCIDS=48; int NTOTALBCIDS=200;//thesis plot


///////////////
///    2022 Data
TString inpath="./AfterglowModel_2022Data_SummerVeto/data_v2/Run2022F" ; //2022 final veto
//std::vector<int> RunList={361948}; std::vector<int> LeadBCIDList={1018,2806}; int NCOLLIDINGBCIDS=189; int NTOTALBCIDS=250; //600b fill 8383 -> few blocks
//std::vector<int> RunList={360991}; std::vector<int> LeadBCIDList={1018,2806}; int NCOLLIDINGBCIDS=189; int NTOTALBCIDS=250; //600b Fill 8307  
//std::vector<int> RunList={361957}; std::vector<int> LeadBCIDList={66,2748}; int NCOLLIDINGBCIDS=208; int NTOTALBCIDS=270;//1800b fill 8385 -> mu scan fill, fit smooth part
std::vector<int> RunList={360991}; std::vector<int> LeadBCIDList={1018+153}; int NCOLLIDINGBCIDS=36; int NTOTALBCIDS=100; //Thesis plot Fill 8307




//////////////////////////////////////////////
void fitAfterglowRun(){

  if(inpath.CompareTo("")==0) return;

  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");
  
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
	fitAfterglowTrain(H,key->GetName(), LeadBCIDList[i], NCOLLIDINGBCIDS, NTOTALBCIDS,outputpath);


    }

  }

  
  outputfile.cd();
  if(Tree) Tree->Write();
  outputfile.ls();
  outputfile.Close();
  
}
