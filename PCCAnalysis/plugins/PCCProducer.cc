/**_________________________________________________________________
class:   RawPCCProducer.cc

description: Creates a LumiInfo object that will contain the luminosity per bunch crossing,
along with the total luminosity and the statistical error.

authors:Sam Higginbotham (shigginb@cern.ch) and Chris Palmer (capalmer@cern.ch) 

________________________________________________________________**/
#include <string>
#include <iostream> 
#include <fstream> 
#include <vector>
#include "DataFormats/Luminosity/interface/PixelClusterCounts.h"
#include "DataFormats/Luminosity/interface/LumiInfo.h"
#include "DataFormats/Luminosity/interface/LumiConstants.h"
#include "CondFormats/Luminosity/interface/LumiCorrections.h"
#include "CondFormats/DataRecord/interface/LumiCorrectionsRcd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "TMath.h"

#include "CondFormats/SiPixelObjects/interface/SiPixelQuality.h"
#include "CondFormats/DataRecord/interface/SiPixelQualityRcd.h"
#include "DataFormats/DetId/interface/DetId.h"


class PCCProducer : public edm::one::EDProducer<edm::EndLuminosityBlockProducer,
    edm::one::WatchLuminosityBlocks> {
        public:
            explicit PCCProducer(const edm::ParameterSet&);
            ~PCCProducer() override;

        private:
            void beginLuminosityBlock     (edm::LuminosityBlock const& lumiSeg, const edm::EventSetup& iSetup) final;
            void endLuminosityBlock       (edm::LuminosityBlock const& lumiSeg, const edm::EventSetup& iSetup) final;
            void endLuminosityBlockProduce(edm::LuminosityBlock& lumiSeg, const edm::EventSetup& iSetup) final;
            void produce                  (edm::Event& iEvent, const edm::EventSetup& iSetup) final;

            virtual void beginJob() override;
            virtual void endJob() override;
  
            edm::EDGetTokenT<reco::PixelClusterCounts>  pccToken;
            std::string   pccSource_;                   //input file EDproducer module label 
            std::string   prodInst_;                    //input file product instance 
            std::string   takeAverageValue_;            //Output average values 

            std::vector<int>   modVeto_;                //The list of modules to skip in the lumi calc. 
            std::string outputProductName_;             //specifies the trigger Rand or ZeroBias 
            std::vector<int> clustersPerBXInput_;       //Will fill this with content from PCC
            std::vector<int> modID_;                    //vector with Module IDs 1-1 map to bunch x-ing in clusers
            std::vector<int> events_;                   //vector with total events at each bxid.
            std::vector<int> clustersPerBXOutput_;      //new vector containing clusters per bxid 
            std::vector<float> corrClustersPerBXOutput_;//new vector containing clusters per bxid with afterglow corrections 
            std::vector<float> errorPerBX_;             //Stat error (or number of events)
            std::vector<int> goodMods_;                 //The indicies of all the good modules - not vetoed
            float totalLumi_;                           //The total raw luminosity from the pixel clusters - not scaled
            float statErrOnLumi_;                       //the statistical error on the lumi - large num ie sqrt(N)

            std::string csvOutLabel_;
            bool saveCSVFile_;

            std::string modOutLabel_;
            std::ofstream modfile;
            unsigned int modid_[2000];
            unsigned int modcount_[2000][LumiConstants::numBX];//integrates counts per module and bx

            bool applyCorr_;
            std::vector<float> correctionScaleFactors_;

            std::unique_ptr<LumiInfo> outputLumiInfo;

            std::ofstream csvfile;
    };

//--------------------------------------------------------------------------------------------------
PCCProducer::PCCProducer(const edm::ParameterSet& iConfig)
{
    pccSource_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getParameter<std::string>("inputPccLabel");
    prodInst_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getParameter<std::string>("ProdInst");
    takeAverageValue_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getUntrackedParameter<std::string>("OutputValue",std::string("Totals")); 
    outputProductName_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getUntrackedParameter<std::string>("outputProductName","alcaLumi");
    modVeto_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getParameter<std::vector<int>>("modVeto");
    applyCorr_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getUntrackedParameter<bool>("ApplyCorrections",false);
    saveCSVFile_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getUntrackedParameter<bool>("saveCSVFile",false);
    csvOutLabel_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getUntrackedParameter<std::string>("label",std::string("rawPCC.csv"));
    modOutLabel_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getUntrackedParameter<std::string>("modOutLabel",std::string(""));

    edm::InputTag PCCInputTag_(pccSource_, prodInst_);
    clustersPerBXOutput_.resize(LumiConstants::numBX,0);//vector containing clusters per bxid 
    corrClustersPerBXOutput_.resize(LumiConstants::numBX,0);//vector containing clusters per bxid 
    pccToken=consumes<reco::PixelClusterCounts, edm::InLumi>(PCCInputTag_);
    produces<LumiInfo, edm::Transition::EndLuminosityBlock>(outputProductName_);

    if(!applyCorr_){
        correctionScaleFactors_.resize(LumiConstants::numBX,1.0);
    }
}

//--------------------------------------------------------------------------------------------------
PCCProducer::~PCCProducer(){
}

//-------------------------------------------------------------------------------------------------
void PCCProducer::beginJob(){
  if(modOutLabel_.compare("")!=0){
    system((std::string("rm -f ")+modOutLabel_).c_str());
    modfile.open(modOutLabel_, std::ios_base::out);
    if (!modfile.is_open())
      throw cms::Exception("PCCProducer:: ") <<  " unable to create the modOutLabel file.";
    modfile.close();

    for (int i=0;i<2000;i++){
      modid_[i] = 0 ;
      for (int bx=0;bx<int(LumiConstants::numBX);bx++)
	modcount_[i][bx] = 0 ;
    }
  }
}

void PCCProducer::endJob(){
  if(modOutLabel_.compare("")!=0){
    modfile.open(modOutLabel_, std::ios_base::out);
    for (int i=0;i<2000;i++){
      modfile<<modid_[i]<<",";
      for (int bx=0;bx<int(LumiConstants::numBX);bx++)
	modfile<<modcount_[i][bx]<<",";
      modfile<<std::endl;
    }
    modfile.close();
  }
}

//--------------------------------------------------------------------------------------------------
void PCCProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){


}

//--------------------------------------------------------------------------------------------------
void PCCProducer::beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, const edm::EventSetup& iSetup){

    outputLumiInfo = std::make_unique<LumiInfo>(); 

    if(saveCSVFile_){
        csvfile.open(csvOutLabel_, std::ios_base::app);
        csvfile<<std::to_string(lumiSeg.run())<<",";
        csvfile<<std::to_string(lumiSeg.luminosityBlock())<<",";
    }
}

//--------------------------------------------------------------------------------------------------
void PCCProducer::endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, const edm::EventSetup& iSetup){
    totalLumi_=0.0;
    statErrOnLumi_=0.0;

    std::fill(clustersPerBXOutput_.begin(),  clustersPerBXOutput_.end(),0);
    std::fill(corrClustersPerBXOutput_.begin(),     corrClustersPerBXOutput_.end(),   0);

    goodMods_.clear();

    edm::Handle<reco::PixelClusterCounts> pccHandle; 
    lumiSeg.getByToken(pccToken,pccHandle);

    const reco::PixelClusterCounts& inputPcc = *(pccHandle.product()); 

    modID_ = inputPcc.readModID();
    events_= inputPcc.readEvents();
    clustersPerBXInput_ = inputPcc.readCounts();


    std::vector<unsigned int> goodmodID;
    edm::ESHandle< SiPixelQuality > siPixelQualityHandle;
    iSetup.get< SiPixelQualityRcd >().get(siPixelQualityHandle);
    const SiPixelQuality * siPixelQuality = siPixelQualityHandle.product();
    // if( siPixelQuality ){
    //   //std::cout<<"siPixelQuality is available"<<std::endl;
   //   auto theDisabledModules = siPixelQuality->getBadComponentList();
   //   for (const auto &mod : theDisabledModules){
   //     int BadRocCount(0);
   //     for (unsigned short n = 0; n < 16; n++){
   // 	unsigned short mask = 1 << n;  // 1 << n = 2^{n} using bitwise shift
   // 	if (mod.BadRocs & mask) BadRocCount++;
   //     }
   //     //std::cout<<"detId:" <<  mod.DetID << " error type:" << mod.errorType << " BadRocs:"  << BadRocCount <<  std::endl;
   //   }
   // }
  
  
 

    //making list of modules to sum over
    for (unsigned int i=0;i<modID_.size();i++){
      bool badmod=0;

      ///check if this is module is in the veto list
      if (std::find(modVeto_.begin(),modVeto_.end(), modID_.at(i)) != modVeto_.end())
	badmod=1;
   
      ///check if it is in the PixelQuality bad list
      if(!badmod && siPixelQuality ){
	auto theDisabledModules = siPixelQuality->getBadComponentList();
	for (const auto &mod : theDisabledModules)
	  if(mod.DetID == (unsigned int) modID_.at(i)) 
	    badmod =1;
      }
      
      if(!badmod){
	goodMods_.push_back(i);
	goodmodID.push_back(modID_.at(i));
      }
    }
    //for (const auto &modid : goodmodID)
    //std::cout<<", "<<modid;

    //summing over good modules only 
    for (int bx=0;bx<int(LumiConstants::numBX);bx++){
        for (unsigned int i=0;i<goodMods_.size();i++){
            clustersPerBXOutput_.at(bx)+=clustersPerBXInput_.at(goodMods_.at(i)*int(LumiConstants::numBX)+bx);

        }
    }

    if(applyCorr_){
        edm::ESHandle< LumiCorrections > corrHandle;
        iSetup.get<LumiCorrectionsRcd>().get(corrHandle);
        const LumiCorrections *pccCorrections = corrHandle.product();
        correctionScaleFactors_ = pccCorrections->getCorrectionsBX();
    }

    for (unsigned int i=0;i<clustersPerBXOutput_.size();i++){
        if (events_.at(i)!=0){
            corrClustersPerBXOutput_[i]=clustersPerBXOutput_[i]*correctionScaleFactors_[i];
        }else{
            corrClustersPerBXOutput_[i]=0.0;
        }
        totalLumi_+=corrClustersPerBXOutput_[i];
        statErrOnLumi_+=float(events_[i]);
    }

    errorPerBX_.clear();
    errorPerBX_.assign(events_.begin(),events_.end()); 

    if(takeAverageValue_=="Average"){
        unsigned int NActiveBX=0;
        for (int bx=0;bx<int(LumiConstants::numBX);bx++){
            if(events_[bx]>0){
                NActiveBX++;  
                // Counting where events are will only work 
                // for ZeroBias or AlwaysTrue triggers.
                // Random triggers will get all BXs.
                corrClustersPerBXOutput_[bx]/=float(events_[bx]);
                errorPerBX_[bx]=1/TMath::Sqrt(float(events_[bx]));
            }
        }
        if (statErrOnLumi_!=0) {
            totalLumi_=totalLumi_/statErrOnLumi_*float(NActiveBX);
            statErrOnLumi_=1/TMath::Sqrt(statErrOnLumi_)*totalLumi_;
        }
    }


    outputLumiInfo->setTotalInstLumi(totalLumi_);
    outputLumiInfo->setTotalInstStatError(statErrOnLumi_);

    outputLumiInfo->setErrorLumiAllBX(errorPerBX_);
    outputLumiInfo->setInstLumiAllBX(corrClustersPerBXOutput_);

    outputLumiInfo->setChannelIDs(goodmodID);

    if(saveCSVFile_){
        csvfile<<std::to_string(totalLumi_);
        
        if(totalLumi_>0){
            for(unsigned int bx=0;bx<LumiConstants::numBX;bx++){
                csvfile<<","<<std::to_string(corrClustersPerBXOutput_[bx]);
            }
            csvfile<<std::endl;   
        } else if (totalLumi_<0) {
            edm::LogInfo("WARNING")<<"WHY IS LUMI NEGATIVE?!?!?!? "<<totalLumi_;
        }

        csvfile.close();
    }



    if(modOutLabel_.compare("")!=0){
      for (unsigned int i=0;i<modID_.size();i++){      
	// modfile<<std::to_string(lumiSeg.run())<<",";
	// modfile<<std::to_string(lumiSeg.luminosityBlock())<<",";	
	// modfile<<std::to_string(modID_.at(i))<<",";	
	// for (int bx=0;bx<int(LumiConstants::numBX);bx++)
        //     modfile<<clustersPerBXInput_.at(i*int(LumiConstants::numBX)+bx)<<",";
	// modfile<<std::endl;   
	
	modid_[i] = modID_.at(i);
	for (int bx=0;bx<int(LumiConstants::numBX);bx++)
	  modcount_[i][bx] += clustersPerBXInput_.at(i*int(LumiConstants::numBX)+bx);
      }
    }


}

//--------------------------------------------------------------------------------------------------
void PCCProducer::endLuminosityBlockProduce(edm::LuminosityBlock& lumiSeg, const edm::EventSetup& iSetup){
    lumiSeg.put(std::move(outputLumiInfo), std::string(outputProductName_)); 

}

DEFINE_FWK_MODULE(PCCProducer);
