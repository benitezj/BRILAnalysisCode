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
//#include "DataFormats/Luminosity/interface/LumiInfo.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Luminosity/interface/PixelClusterCounts.h"
#include "DataFormats/Luminosity/interface/PixelClusterCountsInEvent.h"

#include <TTree.h>



//
// class declaration
//
class PCCEventToTuple : public edm::one::EDAnalyzer<edm::one::SharedResources>  {

public:
  explicit PCCEventToTuple(const edm::ParameterSet&);
  ~PCCEventToTuple();
  
private:
  
  virtual void beginJob() override {}; 
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  //  void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override {};
  //  void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override {};

  const edm::EDGetTokenT<reco::PixelClusterCountsInEvent> pccToken_;
  TTree* tree;
  int run=0;
  int LS = -99;
  int LN = -99;
  int orbit = -99;             
  int bunchCrossing = -99;
  std::map<std::pair<int, int>, int> nPixelClusters;
  float timeStamp_begin;  
};


PCCEventToTuple::PCCEventToTuple(const edm::ParameterSet& iConfig):
  pccToken_(consumes<reco::PixelClusterCountsInEvent>(iConfig.getParameter<edm::InputTag>("inputTag")))
{
  
  
  edm::Service<TFileService> fs;
  tree = fs->make<TTree>("tree", "Pixel Cluster Counters");

  tree->Branch("run", &run, "run/I");
  tree->Branch("LS", &LS, "LS/I");
  tree->Branch("LN", &LN, "LN/I");
  tree->Branch("timeStamp_begin", &timeStamp_begin, "timeStamp_begin/i");
  tree->Branch("bunchCrossing", &bunchCrossing, "bunchCrossing/I");
  tree->Branch("nPixelClusters", "map<std::pair<int,int>,int>", &nPixelClusters);
    
}


PCCEventToTuple::~PCCEventToTuple(){}


void PCCEventToTuple::analyze(const edm::Event& iEvent, const edm::EventSetup&)
{
  edm::Handle<reco::PixelClusterCountsInEvent> pccHandle;
  iEvent.getByToken(pccToken_, pccHandle);
    
  if (!pccHandle.isValid()) {
    return;
  }


  
  
  run = iEvent.id().run();
  LS = iEvent.getLuminosityBlock().luminosityBlock();
  orbit = iEvent.orbitNumber();
  LN = ((int)(orbit >> 12) % 64);
  bunchCrossing = iEvent.bunchCrossing();
  
  if (timeStamp_begin > iEvent.time().unixTime())
    timeStamp_begin = iEvent.time().unixTime();

  std::pair<int, int> bxModKey; 
  bxModKey.first = bunchCrossing;
  bxModKey.second = -1;
  //bxModKey.second = detId();

  const reco::PixelClusterCountsInEvent inputPcc = *pccHandle;
  //streamCache(iID)->add(inputPcc);
  
}


//define this as a plug-in
DEFINE_FWK_MODULE(PCCEventToTuple);
