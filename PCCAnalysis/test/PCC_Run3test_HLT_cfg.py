#########################
## standalone config to test PCC modules
## Jose Benitez, Attila Radl
#########################
import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run2_2018_cff import Run2_2018

#switch for ZeroBias vs Random streams
#stream="ZeroBias"
stream="Random"

process = cms.Process('hlt',Run2_2018)

process.source = cms.Source("PoolSource",
#fileNames = cms.untracked.vstring('file:/eos/cms/store/data/Run2016H/AlCaLumiPixels0/RAW/v1/000/281/601/00000/2C65B0FE-6D8C-E611-8CB6-FA163E00F43A.root')
fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/b/benitezj/public/BRIL/PCC/Run3Dev/Run2018D-AlCaLumiPixels-RAW-323702-D3FCD0FC-6328-B24E-AD3D-C22C55B968DD.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)



# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.AlCaRecoStreams_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')


from EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi import siPixelDigis
process.siPixelDigisForLumi = siPixelDigis.cpu.clone(
   InputLabel = "hltFEDSelectorLumiPixels"
)

from RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizerPreSplitting_cfi import siPixelClustersPreSplitting
process.siPixelClustersForLumi = siPixelClustersPreSplitting.cpu.clone(
    src = "siPixelDigisForLumi"
)

################################
#RECO->ALCAPCC 
process.hltAlcaPixelClusterCounts = cms.EDProducer("AlcaPCCEventProducer",
    pixelClusterLabel = cms.InputTag("siPixelClustersForLumi"),
    trigstring = cms.untracked.string(stream) 
)


#################################
# OutPath products
process.ALCARECOStreamPromptCalibProdPCC = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOPromptCalibProdPCC')
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('ALCAPROMPT'),
        filterName = cms.untracked.string('PromptCalibProdPCC')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('PCC.root'),
    outputCommands = cms.untracked.vstring('drop *', 
                                           #'keep *_hltFEDSelectorLumiPixels_*_*',
                                           #'keep *_siPixelDigisForLumi_*_*',
                                           #'keep *_siPixelClustersForLumi_*_*',
                                           'keep *_hltAlcaPixelClusterCounts_*_*'
                                       )
)




####################
### sequences/paths
process.seqALCARECOPromptCalibProdPCC = cms.Sequence(process.siPixelDigisForLumi+process.siPixelClustersForLumi+process.hltAlcaPixelClusterCounts)

process.pathALCARECOPromptCalibProdPCC = cms.Path(process.seqALCARECOPromptCalibProdPCC)
process.ALCARECOStreamPromptCalibProdOutPath = cms.EndPath(process.ALCARECOStreamPromptCalibProdPCC)
process.schedule = cms.Schedule(*[ process.pathALCARECOPromptCalibProdPCC, process.ALCARECOStreamPromptCalibProdOutPath ])


#################################################
process.MessageLogger = cms.Service("MessageLogger",
    cerr = cms.untracked.PSet(
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        FwkReport = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            reportEvery = cms.untracked.int32(100)
        ),
        FwkSummary = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            reportEvery = cms.untracked.int32(100)
        ),
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        Root_NoDictionary = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        noTimeStamps = cms.untracked.bool(False),
        threshold = cms.untracked.string('INFO'),
        enableStatistics = cms.untracked.bool(True),
        statisticsThreshold = cms.untracked.string('WARNING')
    ),
    debugModules = cms.untracked.vstring(),
    default = cms.untracked.PSet(),
    suppressDebug = cms.untracked.vstring(),
    suppressInfo = cms.untracked.vstring(),
    suppressWarning = cms.untracked.vstring()
)


process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
