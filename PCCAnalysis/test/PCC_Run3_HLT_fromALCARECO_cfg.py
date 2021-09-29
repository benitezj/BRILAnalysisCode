#########################
## standalone config to test PCC modules
## Jose Benitez, Attila Radl
#########################

import FWCore.ParameterSet.Config as cms

process = cms.Process("PCC")

process.source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring('file:/eos/cms/store/data/Run2015D/AlCaLumiPixels/ALCARECO/LumiPixels-PromptReco-v4/000/260/039/00000/1CF2A210-5B7E-E511-8F4F-02163E014145.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
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
        saveCSVFile=cms.untracked.bool(False),
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
###sequences/paths
#process.seqALCARECOPromptCalibProdPCC = cms.Sequence(process.alcaPCCEventProducer+process.alcaPCCIntegrator)
process.seqALCARECOPromptCalibProdPCC = cms.Sequence(process.alcaPCCEventProducer+process.alcaPCCIntegrator+process.rawPCCProd)
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
