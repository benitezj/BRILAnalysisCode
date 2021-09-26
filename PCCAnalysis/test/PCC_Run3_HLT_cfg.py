#########################
#########################
import FWCore.ParameterSet.Config as cms

process = cms.Process("ALCAPCC")

process.source = cms.Source("PoolSource",
#fileNames = cms.untracked.vstring('file:/eos/cms/store/data/Run2015D/AlCaLumiPixels/ALCARECO/LumiPixels-PromptReco-v4/000/260/039/00000/1CF2A210-5B7E-E511-8F4F-02163E014145.root')
#fileNames = cms.untracked.vstring('file:/eos/cms/store/data/Run2018D/ZeroBias/RAW/v1/000/323/841/00000/FDCDC8F6-35E7-FE49-9D99-4D7CA208D807.root')
fileNames = cms.untracked.vstring('file:/eos/cms/store/data/Run2016H/AlCaLumiPixels0/RAW/v1/000/281/572/00000/BAE55739-5384-E611-8E2D-02163E011FA9.root')
)

# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)



############################
### DB
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = '101X_dataRun2_Prompt_v9'
process.GlobalTag.DumpStat = cms.untracked.bool( False )
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")

############################
#HLT filter for PCC
process.ALCARECOHltFilterForPCC = cms.EDFilter("HLTHighLevel",
    HLTPaths = cms.vstring("*Random*"),
    eventSetupPathsKey = cms.string(""),
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    andOr = cms.bool(True),
    throw = cms.bool(False)
)



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
#RECO->PCC 
process.alcaPCCEventProducer = cms.EDProducer("AlcaPCCEventProducer",
    pixelClusterLabel = cms.InputTag("siPixelClustersForLumi"),
    #Mod factor to count lumi and the string to specify output 
    trigstring = cms.untracked.string("") 
)

#process.seqALCARECOPromptCalibProdPCC = cms.Sequence(process.alcaPCCEventProducer)
#process.seqALCARECOPromptCalibProdPCC = cms.Sequence(process.ALCARECOHltFilterForPCC+process.alcaPCCEventProducer)
#process.seqALCARECOPromptCalibProdPCC = cms.Sequence(process.ALCARECOHltFilterForPCC+process.siPixelDigisForLumi+process.siPixelClustersForLumi+process.alcaPCCEventProducer)
process.seqALCARECOPromptCalibProdPCC = cms.Sequence(process.siPixelDigisForLumi+process.siPixelClustersForLumi+process.alcaPCCEventProducer)
process.pathALCARECOPromptCalibProdPCC = cms.Path(process.seqALCARECOPromptCalibProdPCC)


#################################
#OutPath products
process.ALCARECOStreamPromptCalibProdPCC = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOPromptCalibProdPCC')
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('ALCAPROMPT'),
        filterName = cms.untracked.string('PromptCalibProdPCC')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('ALCAPCC.root'),
    outputCommands = cms.untracked.vstring('drop *', 
                                           'keep *_alcaPCCEventProducer_*_*',
                                           'keep *_MEtoEDMConvertSiStrip_*_*')
)


process.ALCARECOStreamPromptCalibProdOutPath = cms.EndPath(process.ALCARECOStreamPromptCalibProdPCC)


#################################################
process.MessageLogger = cms.Service("MessageLogger",
    cerr = cms.untracked.PSet(
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        FwkReport = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            reportEvery = cms.untracked.int32(100000)
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
    default = cms.untracked.PSet(

    ),
    suppressDebug = cms.untracked.vstring(),
    suppressInfo = cms.untracked.vstring(),
    suppressWarning = cms.untracked.vstring()
)

#added line for additional output summary `
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


process.schedule = cms.Schedule(*[ process.pathALCARECOPromptCalibProdPCC, process.ALCARECOStreamPromptCalibProdOutPath ])
