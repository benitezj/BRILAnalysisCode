# ----------------------------------------------------------------------
import os
import FWCore.ParameterSet.Config as cms
process = cms.Process("Lumi")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.categories.append('L1GtTrigReport')
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# -- Database configuration
process.load("CondCore.CondDB.CondDB_cfi")

# -- Conditions

process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff") #
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag

#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_v14', '')
process.GlobalTag = GlobalTag(process.GlobalTag,'106X_dataRun2_v36', '') #'90X_upgrade2023_realistic_v1'

process.load("Configuration.StandardSequences.Reconstruction_cff") # 
# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -- skimming
process.zerobiasfilter = cms.EDFilter("HLTHighLevel",
   TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
  # HLTPaths = cms.vstring("HITZeroBias_part*"),
   HLTPaths = cms.vstring("*ZeroBias*"),
   eventSetupPathsKey = cms.string(""),
   andOr = cms.bool(True),
   throw = cms.bool(False)
    )

# the main Analyzer
process.lumi = cms.EDAnalyzer(
    "PCCNTupler",
    verbose                      = cms.untracked.int32(0),
    #rootFileName                 = cms.untracked.string(rootFileName),
    #type                         = cms.untracked.string(getDataset(process.source.fileNames[0])),
    globalTag                    = process.GlobalTag.globaltag,
    dumpAllEvents                = cms.untracked.int32(0),
    vertexCollLabel              = cms.untracked.InputTag('offlinePrimaryVertices'),
    pixelClusterLabel            = cms.untracked.InputTag('siPixelClusters'), # even in Phase2, for now.
    saveType                     = cms.untracked.string('Event'), # LumiSect, LumiNib, Event
    sampleType                   = cms.untracked.string('DATA'), # MC, DATA
    includeVertexInformation     = cms.untracked.bool(True),
    includePixels                = cms.untracked.bool(True),
    splitByBX                    = cms.untracked.bool(True),
    L1GTReadoutRecordLabel       = cms.untracked.InputTag('gtDigis'), 
    hltL1GtObjectMap             = cms.untracked.InputTag('hltL1GtObjectMap'), 
    HLTResultsLabel              = cms.untracked.InputTag('TriggerResults::HLT'),
    pixelPhase2Geometry          = cms.untracked.bool(False),
    )

# -- Path
process.p = cms.Path(
    process.zerobiasfilter*
    process.lumi
    )


outFile = 'pcc_Data_PixVtx_Event_90X.root'
process.TFileService = cms.Service("TFileService",fileName = cms.string(outFile)) 
readFiles = cms.untracked.vstring() 
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles) 
readFiles.extend([                                                          
# '/store/data/Run2018B/ZeroBias4/ALCARECO/LumiPixelsMinBias-10Dec2018-v1/00000/148E33D1-5B5D-AD4A-95DF-FED9DEE9548D.root'                        
'/store/data/Run2018B/ZeroBias4/ALCARECO/LumiPixelsMinBias-10Dec2018-v1/00000/6104DDC1-4E1D-634C-B5B9-278DC7D18B08.root'   
])
#import FWCore.PythonUtilities.LumiList as LumiList
#process.source.lumisToProcess = LumiList.LumiList(filename = 'goodList.json').getVLuminosityBlockRange()
