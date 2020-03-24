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
//#include "CondFormats/DataRecord/interface/SiPixelQualityFromDbRcd.h"
#include "DataFormats/DetId/interface/DetId.h"
#include <TH1F.h>
#include <TFile.h>

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


            std::string modCountOutLabel_;                //file to save the module counts for job
            std::ofstream modfile;
            std::map<unsigned,long> modcount_;

            bool applySiPixelQual_;                 // read the CONDDB Pixel quality flags and remove bad modules
            std::string moduleFractionInputLabel_;       // file containing the module fractions
            std::map<unsigned,float> modfraction_; 
            std::string moduleFractionOutputLabel_;       // output file containing the visible crossection fraction
            std::ofstream modFracOut;

            bool applyCorr_;
            std::vector<float> correctionScaleFactors_;

            std::string corrRootFile_;       // Afterglow corrections from ROOT file
            TFile * corrFile = NULL;

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

    modCountOutLabel_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getUntrackedParameter<std::string>("modCountOutLabel",std::string(""));

    applySiPixelQual_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getUntrackedParameter<bool>("applySiPixelQual",false);
    moduleFractionInputLabel_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getUntrackedParameter<std::string>("moduleFractionInputLabel","");
    moduleFractionOutputLabel_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getUntrackedParameter<std::string>("moduleFractionOutputLabel","moduleFraction.csv");
    corrRootFile_ = iConfig.getParameter<edm::ParameterSet>("PCCProducerParameters").getUntrackedParameter<std::string>("corrRootFile","");

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

  if(corrRootFile_.compare("")!=0){
    corrFile = new TFile(corrRootFile_.c_str(),"read");
    if(!corrFile->IsZombie()) std::cout<<  " opened corrRootFile "<<corrRootFile_.c_str()<<std::endl;
  }


  if(applySiPixelQual_){

    ///read the module fraction values
    std::ifstream modfracfile(moduleFractionInputLabel_.c_str());
    if (!modfracfile.is_open())
      throw cms::Exception("PCCProducer:: ") <<  " unable to open the module fractions file";
    std::string line;
    unsigned mod; float frac;
    while (std::getline(modfracfile,line)){
      std::stringstream iss(line);
      iss>>mod>>frac;
      modfraction_[mod]=frac; 
    }
    modfracfile.close();

    //open file for output of the visible crossection fraction
    system((std::string("rm -f ")+moduleFractionOutputLabel_).c_str());
    modFracOut.open(moduleFractionOutputLabel_, std::ios_base::app);
    if (!modFracOut.is_open())
      throw cms::Exception("PCCProducer:: ") <<  " unable to open the output module fraction file";
  }


  if(modCountOutLabel_.compare("")!=0){
    system((std::string("rm -f ")+modCountOutLabel_).c_str());
    modfile.open(modCountOutLabel_, std::ios_base::out);
    if (!modfile.is_open())
      throw cms::Exception("PCCProducer:: ") <<  " unable to create the modCountOutLabel file.";
    modfile.close();  
  }

}

void PCCProducer::endJob(){

  if(corrFile) delete corrFile;

  if(applySiPixelQual_)
    modFracOut.close();

  if(modCountOutLabel_.compare("")!=0){
    modfile.open(modCountOutLabel_, std::ios_base::out);
    for( std::map<unsigned, long>::iterator iter = modcount_.begin();
	 iter != modcount_.end();
	 ++iter )
      modfile<<iter->first<<" "<<iter->second<<std::endl;
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


    double totfrac=0.;
    edm::ESHandle< SiPixelQuality > siPixelQualityHandle;
    const SiPixelQuality * siPixelQuality = NULL;
    if(applySiPixelQual_){
      iSetup.get< SiPixelQualityRcd >().get(siPixelQualityHandle);
      //iSetup.get< SiPixelQualityFromDbRcd >().get(siPixelQualityHandle);
      siPixelQuality = siPixelQualityHandle.product();
    }

    //making list of modules to sum over
    for (unsigned int i=0;i<modID_.size();i++){

      ///check if this is module is in the veto list
      if (std::find(modVeto_.begin(),modVeto_.end(), modID_.at(i)) != modVeto_.end())
	continue;
   
      ///check if it is in the PixelQuality bad list
      if(siPixelQuality ){
	bool badmod=0;
	auto theDisabledModules = siPixelQuality->getBadComponentList();
	for (const auto &mod : theDisabledModules)
	  if(mod.DetID == (unsigned int) modID_.at(i)) {
	    badmod =1;
	    //std::cout<<"Disabled module :"<<modID_.at(i)<<"  frac = "<<modfraction_[modID_.at(i)]<<std::endl;
	  }
	if(badmod) continue;
      }

      goodMods_.push_back(i);
      totfrac += modfraction_[modID_.at(i)];
    }
    //std::cout<<totfrac<<std::endl;

    //summing over good modules only 
    for (int bx=0;bx<int(LumiConstants::numBX);bx++){
        for (unsigned int i=0;i<goodMods_.size();i++){
            clustersPerBXOutput_.at(bx)+=clustersPerBXInput_.at(goodMods_.at(i)*int(LumiConstants::numBX)+bx);

        }
    }

    if(applyCorr_){
      if(corrRootFile_.compare("")!=0){

	TIter next(corrFile->GetListOfKeys());
	TObject* key;
	while ((key = next())) {
	  TString kname(key->GetName());//ScaleFactorsAvg_306546_0_1_66
	  if( ! kname.Contains("ScaleFactorsAvg") ) continue;
	  TH1F* HCORR = (TH1F*) corrFile->Get(kname);
	  //std::cout<<kname<<std::endl;

	  TObjArray * a = kname.Tokenize("_");
	  long r=atoi(((TObjString*)(*a)[1])->GetName());
	  //long l=atoi(((TObjString*)(*a)[2])->GetName());
	  long ls1=atoi(((TObjString*)(*a)[3])->GetName());
	  long ls2=atoi(((TObjString*)(*a)[4])->GetName());

	  if(r!=lumiSeg.run() || lumiSeg.luminosityBlock()<ls1 || lumiSeg.luminosityBlock()>ls2) continue;
	  
	  
	  correctionScaleFactors_.clear();
	  for(int b=0;b<HCORR->GetNbinsX();b++)
	    correctionScaleFactors_.push_back(HCORR->GetBinContent(b));
	  if(correctionScaleFactors_.size() != LumiConstants::numBX)
	    throw cms::Exception("PCCProducer:: ") <<" correctionScaleFactors: "<<correctionScaleFactors_.size()
						   <<" should be "<<LumiConstants::numBX<<std::endl;
	}

      }else {
	edm::ESHandle< LumiCorrections > corrHandle;
	iSetup.get<LumiCorrectionsRcd>().get(corrHandle);
	const LumiCorrections *pccCorrections = corrHandle.product();
	correctionScaleFactors_ = pccCorrections->getCorrectionsBX();
      }

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


    if(saveCSVFile_){
        csvfile<<std::to_string(totalLumi_);
	//std::cout<<lumiSeg.run()<<","<<lumiSeg.luminosityBlock()<<","<<std::to_string(totalLumi_)<<std::endl;
        
        if(totalLumi_>0){
            for(unsigned int bx=0;bx<LumiConstants::numBX;bx++){
                csvfile<<","<<std::to_string(corrClustersPerBXOutput_[bx]);
            }
            csvfile<<std::endl;   
        } else if (totalLumi_<0) {
            edm::LogInfo("WARNING")<<"WHY IS LUMI NEGATIVE?!?!?!? "<<totalLumi_;
        }

        csvfile.close();


	if(applySiPixelQual_){
	  modFracOut<<std::to_string(lumiSeg.run())<<",";
	  modFracOut<<std::to_string(lumiSeg.luminosityBlock())<<",";
	  modFracOut<<totfrac<<",";
	  modFracOut<<goodMods_.size()<<std::endl;
	}
    }


    if(modCountOutLabel_.compare("")!=0){
      for (unsigned int i=0;i<modID_.size();i++){      
	unsigned totclusters=0;
	for (int bx=0;bx<int(LumiConstants::numBX);bx++)
	  totclusters += clustersPerBXInput_.at(i*int(LumiConstants::numBX)+bx);
	modcount_[modID_.at(i)] += totclusters;
      }
    }

}

//--------------------------------------------------------------------------------------------------
void PCCProducer::endLuminosityBlockProduce(edm::LuminosityBlock& lumiSeg, const edm::EventSetup& iSetup){
    lumiSeg.put(std::move(outputLumiInfo), std::string(outputProductName_)); 

}

DEFINE_FWK_MODULE(PCCProducer);
