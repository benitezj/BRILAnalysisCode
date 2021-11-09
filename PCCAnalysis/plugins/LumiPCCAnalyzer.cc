// -*- C++ -*-
//
// Original Author:  Jose  Benitez (jose.benitez@cern.ch)
//         Created:  18 October 2021 
// Purpose: produces TH2 histogram with PCC lum per LS and per BX

// system include files
#include <memory>
#include <fstream>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Luminosity/interface/LumiInfo.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"


//#include "FWCore/ServiceRegistry/interface/Service.h"
//#include "CommonTools/UtilAlgos/interface/TFileService.h"

//
// class declaration
//
class LumiPCCAnalyzer : public edm::one::EDAnalyzer<edm::one::WatchLuminosityBlocks>  {

public:
  explicit LumiPCCAnalyzer(const edm::ParameterSet&);
  ~LumiPCCAnalyzer();
  
private:
  
  virtual void beginJob() override; 
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override {};
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override {};//this module does not use event-by-event info 

  edm::EDGetTokenT<LumiInfo>  lumiInfoToken;
  std::string   pccSrc_;//input file EDproducer module label
  std::string   prodInst_;//input file product instance


  std::string   csvfilename_;//
  std::ofstream csvfile;

  //TH1F * h_pt;
};


LumiPCCAnalyzer::LumiPCCAnalyzer(const edm::ParameterSet& iConfig)
  :
  pccSrc_(iConfig.getParameter<std::string>("LumiInfoName")),
  prodInst_(iConfig.getParameter<std::string>("LumiInfoInst")),
  csvfilename_("rawPCC.csv")
{

  edm::InputTag inputPCCTag_(pccSrc_, prodInst_);
  lumiInfoToken=consumes<LumiInfo, edm::InLumi>(inputPCCTag_);

  //edm::Service<TFileService> fs;
  //h_pt    = fs->make<TH1F>( "pt"  , "p_{t}", 100,  0., 1 );
}


LumiPCCAnalyzer::~LumiPCCAnalyzer(){}


void  LumiPCCAnalyzer::beginJob()
{

  ///check if its possible to create the output file
  system((std::string("rm -f ")+csvfilename_).c_str());
  csvfile.open(csvfilename_, std::ios_base::out);
  if (!csvfile.is_open())
    throw cms::Exception("LumiPCCAnalyzer:: ") <<  " unable to create the csv file.";
  csvfile.close();
  

}

//--------------------------------------------------------------------------------------------------
void  LumiPCCAnalyzer::endLuminosityBlock(const edm::LuminosityBlock& lumiSeg, const edm::EventSetup& iSetup)
{

  //get the lumi info input 
  edm::Handle<LumiInfo> PCCHandle;
  lumiSeg.getByToken(lumiInfoToken,PCCHandle);
  const LumiInfo& inLumiOb = *(PCCHandle.product());
  const std::vector<float> lumiByBX = inLumiOb.getInstLumiAllBX();
  //const std::vector<float> errByBX = inLumiOb.getErrorLumiAllBX();


  //fill the output histogram
  csvfile.open(csvfilename_, std::ios_base::app);
  csvfile<<std::to_string(lumiSeg.run())<<",";
  csvfile<<std::to_string(lumiSeg.luminosityBlock())<<",";
  csvfile<<std::to_string(inLumiOb.getTotalInstLumi());
  for(unsigned int i=0;i<lumiByBX.size();i++)
    csvfile<<","<<std::to_string(lumiByBX[i]);
  csvfile<<std::endl;
  csvfile.close();
  

  //std::cout<<std::to_string(lumiSeg.run())<<",";
  //std::cout<<std::to_string(lumiSeg.luminosityBlock())<<",";
  //std::cout<<std::to_string(inLumiOb.getTotalInstLumi())<<std::endl;

}

//define this as a plug-in
DEFINE_FWK_MODULE(LumiPCCAnalyzer);
