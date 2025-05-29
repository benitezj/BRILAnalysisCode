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
class PCCEventToTuple : public edm::one::EDAnalyzer<edm::one::SharedResources>  {

public:
  explicit PCCEventToTuple(const edm::ParameterSet&);
  ~PCCEventToTuple();
  
private:
  
  virtual void beginJob() override {}; 
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

  const edm::EDGetTokenT<reco::PixelClusterCountsInEvent> pccToken_;

  TTree* tree;
  unsigned int run = 0;
  unsigned short LS = 0;
  unsigned short LN = 0;
  unsigned short bunchCrossing = 0;
  UInt_t timeStamp_begin;
  unsigned short pcc=0;

  const std::vector<int> modVeto_;
};


PCCEventToTuple::PCCEventToTuple(const edm::ParameterSet& iConfig):
  pccToken_(consumes<reco::PixelClusterCountsInEvent>(iConfig.getParameter<edm::InputTag>("inputTag"))),
  modVeto_(iConfig.getParameter<std::vector<int>>("modVeto"))
{
  
  
  edm::Service<TFileService> fs;
  tree = fs->make<TTree>("tree", "Pixel Cluster Counters");
  tree->Branch("run", &run, "run/I");
  tree->Branch("LS", &LS, "LS/s");
  tree->Branch("LN", &LN, "LN/s");
  tree->Branch("bunchCrossing", &bunchCrossing, "bunchCrossing/s");
  tree->Branch("timeStamp_begin", &timeStamp_begin, "timeStamp_begin/i");
  tree->Branch("pcc", &pcc, "pcc/s");
    
}


PCCEventToTuple::~PCCEventToTuple(){}


void PCCEventToTuple::analyze(const edm::Event& iEvent, const edm::EventSetup&)
{
  run = iEvent.id().run();
  LS = iEvent.getLuminosityBlock().luminosityBlock();
  LN = ((int)(iEvent.orbitNumber() >> 12) % 64);
  bunchCrossing = iEvent.bunchCrossing();
  timeStamp_begin = iEvent.time().unixTime();


  ///retrive the clusters from the event
  pcc=0;
  edm::Handle<reco::PixelClusterCountsInEvent> pccHandle;
  iEvent.getByToken(pccToken_, pccHandle);
  if (!pccHandle.isValid()) {
    std::cout<<"Invalid pccHandle"<<std::endl;
    return;
  }
  const reco::PixelClusterCountsInEvent inputPcc = *pccHandle;
  std::vector<int> inputpcc = inputPcc.counts();
  std::vector<int> inputmodid = inputPcc.modID();

  //apply the module veto and calculate total PCC
  for(long unsigned int i=0;i<inputmodid.size();i++){
    if (std::find(modVeto_.begin(), modVeto_.end(), inputmodid[i]) == modVeto_.end()) {
      pcc+=inputpcc[i];
    }
  }

  
  tree->Fill();
}


//define this as a plug-in
DEFINE_FWK_MODULE(PCCEventToTuple);
