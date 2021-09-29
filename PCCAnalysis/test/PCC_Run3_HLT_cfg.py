#########################
## standalone config to test PCC modules
#########################
import FWCore.ParameterSet.Config as cms

process = cms.Process("PCC")

process.source = cms.Source("PoolSource",
#fileNames = cms.untracked.vstring('file:/eos/cms/store/data/Run2016H/AlCaLumiPixels0/RAW/v1/000/281/601/00000/2C65B0FE-6D8C-E611-8CB6-FA163E00F43A.root')
fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/b/benitezj/public/BRIL/PCC/Run3Dev/Run2018D-AlCaLumiPixels-RAW-323702-D3FCD0FC-6328-B24E-AD3D-C22C55B968DD.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)



############################
### DB inputs needed: Pix cabling map (RAW->Digi) and Geometry (Digi->Reco)
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = '101X_dataRun2_Prompt_v9'
#process.GlobalTag.globaltag = '106X_dataRun2_v10'
#process.GlobalTag.DumpStat = cms.untracked.bool( False )
#process.load("Configuration.StandardSequences.GeometryRecoDB_cff")


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

############################
#HLT filter for PCC  -> Not needed I think
#process.ALCARECOHltFilterForPCC = cms.EDFilter("HLTHighLevel",
#    HLTPaths = cms.vstring("*Random*"),
#    eventSetupPathsKey = cms.string(""),
#    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
#    andOr = cms.bool(True),
#    throw = cms.bool(False)
#)



##############################
#RAW->DIGI->RECO
process.siPixelDigisForLumi = cms.EDProducer("SiPixelRawToDigi",
    CablingMapLabel = cms.string(''),
    ErrorList = cms.vint32(29),
    IncludeErrors = cms.bool(True),
    InputLabel = cms.InputTag("hltFEDSelectorLumiPixels"),
    Regions = cms.PSet(),
    UsePhase1 = cms.bool(False),
    UsePilotBlade = cms.bool(False),
    UseQualityInfo = cms.bool(False),
    UserErrorList = cms.vint32(40)
)
process.siPixelClustersForLumi = cms.EDProducer("SiPixelClusterProducer",
    ChannelThreshold = cms.int32(1000),
    ClusterThreshold = cms.int32(4000),
    MissCalibrate = cms.bool(True),
    SeedThreshold = cms.int32(1000),
    SplitClusters = cms.bool(False),
    VCaltoElectronGain = cms.int32(65),
    VCaltoElectronOffset = cms.int32(-414),
    maxNumberOfClusters = cms.int32(-1),
    payloadType = cms.string('Offline'),
    src = cms.InputTag("siPixelDigisForLumi")
)

################################
#RECO->ALCAPCC 
process.alcaPCCEventProducer = cms.EDProducer("AlcaPCCEventProducer",
    pixelClusterLabel = cms.InputTag("siPixelClustersForLumi"),
    trigstring = cms.untracked.string("") 
)

################################
##ALCAPCC->ALCARECO
process.alcaPCCIntegrator = cms.EDProducer("AlcaPCCIntegrator",
    AlcaPCCIntegratorParameters = cms.PSet(
        inputPccLabel = cms.string("alcaPCCEventProducer"),
        trigstring = cms.untracked.string(""),
        ProdInst = cms.string("")
    ),
)

################################
##ALCARECO->csv
process.rawPCCProd = cms.EDProducer("RawPCCProducer",
    RawPCCProducerParameters = cms.PSet(
        inputPccLabel = cms.string("alcaPCCIntegrator"),
        ProdInst = cms.string(""),
        outputProductName = cms.untracked.string("lumiInfo"),
        ApplyCorrections=cms.untracked.bool(False),
        saveCSVFile=cms.untracked.bool(True),
        modVeto=cms.vint32(),
        OutputValue = cms.untracked.string("Average"),
    )
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
                                           'keep *_siPixelClustersForLumi_*_*',
                                           'keep *_alcaPCCEventProducer_*_*',
                                           'keep *_alcaPCCIntegrator_*_*',
                                           'keep *_rawPCCProd_*_*'
                                       )
)




####################
### cms sequences/paths
process.seqALCARECOPromptCalibProdPCC = cms.Sequence(process.siPixelDigisForLumi+process.siPixelClustersForLumi+process.alcaPCCEventProducer+process.alcaPCCIntegrator+process.rawPCCProd)
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
            reportEvery = cms.untracked.int32(1000)
        ),
        FwkSummary = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            reportEvery = cms.untracked.int32(1000)
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
