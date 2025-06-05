// -*- C++ -*-
//
// Original Author:  Jose  Benitez (jose.benitez@cern.ch)

#include <memory>
#include <fstream>
#include <iostream>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Luminosity/interface/PixelClusterCounts.h"
#include "DataFormats/Luminosity/interface/PixelClusterCountsInEvent.h"

#include <TTree.h>



//
// class declaration
//
class PCCEventToTuple : public edm::one::EDAnalyzer<edm::one::SharedResources,edm::one::WatchLuminosityBlocks>  {

public:
  explicit PCCEventToTuple(const edm::ParameterSet&);
  ~PCCEventToTuple();
  
private:
  
  virtual void beginJob() override {}; 
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override {};
  void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  
  const edm::EDGetTokenT<reco::PixelClusterCountsInEvent> pccToken_;
  
  const std::vector<int> bcidSelection_;
  const std::vector<int> modVeto_;

  
  TTree* tree = 0;
  unsigned int run = 0;
  unsigned short LS = 0;
  unsigned short LN = 0;


  std::map<int,int> nev_bx;//events per bx
  std::map<int,int> nev_bx_[16];//events per bx per NB4
  
  UInt_t timeStamp_begin;
  unsigned long timeStamp_begin_[16];
  unsigned int nev_[16];//events per NB4
  
  std::map<std::pair<int,int>,int> pcc_bx_mod; // pcc per bx per module
  std::map<std::pair<int,int>,int> pcc_bx_mod_[16]; // pcc per bx per module per NB4

};


PCCEventToTuple::PCCEventToTuple(const edm::ParameterSet& iConfig):
  pccToken_(consumes<reco::PixelClusterCountsInEvent>(iConfig.getParameter<edm::InputTag>("inputTag"))),
  bcidSelection_(iConfig.getParameter<std::vector<int>>("bcidSelection")),
  modVeto_(iConfig.getParameter<std::vector<int>>("modVeto"))
{

  for (unsigned short i=0;i<16;i++){
    timeStamp_begin_[i]=0;
    nev_[i]=0;
    nev_bx_[i].clear();
    pcc_bx_mod_[i].clear();
  }
  
  edm::Service<TFileService> fs;
  tree = fs->make<TTree>("tree", "Pixel Cluster Counters");
  tree->Branch("run", &run, "run/i");
  tree->Branch("LS", &LS, "LS/s");
  tree->Branch("LN", &LN, "LN/s");
  tree->Branch("timeStamp_begin", &timeStamp_begin, "timeStamp_begin/i");
  tree->Branch("pcc_bx_mod", "std::map<std::pair<int,int>,int>", &pcc_bx_mod);
  tree->Branch("nev_bx", "std::map<int,int>", &nev_bx);
}


PCCEventToTuple::~PCCEventToTuple(){}


void PCCEventToTuple::analyze(const edm::Event& iEvent, const edm::EventSetup&)
{
  LN = ((int)(iEvent.orbitNumber() >> 12) % 64) / 4; // NB4
  int bunchCrossing = iEvent.bunchCrossing();
  
  //apply bcid selection
  if (std::find(bcidSelection_.begin(), bcidSelection_.end(), bunchCrossing+1) == bcidSelection_.end()) 
    return;

  
  ///retrive the clusters from the event
  edm::Handle<reco::PixelClusterCountsInEvent> pccHandle;
  iEvent.getByToken(pccToken_, pccHandle);
  if (!pccHandle.isValid()) {
    std::cout<<"Invalid pccHandle"<<std::endl;
    return;
  }


  //sum events per LN
  timeStamp_begin_[LN] += iEvent.time().unixTime();
  nev_[LN] += 1;
  (nev_bx_[LN])[bunchCrossing] += 1;

  //sum pcc per LN per bx per mod
  const reco::PixelClusterCountsInEvent inputPcc = *pccHandle;
  std::vector<int> inputpcc = inputPcc.counts();
  std::vector<int> inputmodid = inputPcc.modID();
  std::pair<int,int> bxModKey;
  bxModKey.first = bunchCrossing;
  for(long unsigned int i=0;i<inputmodid.size();i++){
    if (std::find(modVeto_.begin(), modVeto_.end(), inputmodid[i]) == modVeto_.end()) {
      bxModKey.second = inputmodid[i];
      (pcc_bx_mod_[LN])[bxModKey] += inputpcc[i];
    }
  }

}



void PCCEventToTuple::endLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& isetup) {
  run=lumi.run();
  LS=lumi.luminosityBlock();
  
  for (unsigned short i=0;i<16;i++){
    if(nev_[i]>0){
      LN=i;
      timeStamp_begin = timeStamp_begin_[i]/nev_[i];
      nev_bx = nev_bx_[i];
      pcc_bx_mod = pcc_bx_mod_[i];
      tree->Fill();
    }

    
    timeStamp_begin_[i]=0;
    nev_[i]=0;
    nev_bx_[i].clear();
    pcc_bx_mod_[i].clear();
  }
    
}


//define this as a plug-in
DEFINE_FWK_MODULE(PCCEventToTuple);
