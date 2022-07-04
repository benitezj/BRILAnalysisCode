# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 -s ALCAOUTPUT:AlCaPCCRandom,ALCA:PromptCalibProdLumiPCC --conditions auto:run3_data --era Run3 --datatier ALCARECO --eventcontent ALCARECO --triggerResultsProcess RECO --filein file:/afs/cern.ch/user/b/benitezj/output/BRIL/CMSSW_12_1_0_pre4/src/step2_Express_PCCRandom_ALCAPRODUCER.root -n 100 --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process('ALCA',Run3)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.AlCaRecoStreams_cff')
process.load('Configuration.StandardSequences.AlCaRecoStreams_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

# Input source
#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/b/benitezj/output/BRIL/CMSSW_12_1_0_pre4/src/step2_Express_PCCRandom_ALCAPRODUCER.root'),
#    secondaryFileNames = cms.untracked.vstring()
#)

import os,sys
process.MessageLogger.cerr.FwkSummary.reportEvery = cms.untracked.int32(10000)
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10000)

inputlist=cms.untracked.vstring()
inputfile=os.getenv('INPUTFILE')
if inputfile != None :
    print('reading from input file: '+inputfile)
    infile = open(inputfile,'r')
    inputlist=cms.untracked.vstring(infile.readlines())
    infile.close()
if len(inputlist) == 0 : sys.exit("No input files")
process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)

process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    SkipEvent = cms.untracked.vstring(),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    deleteNonConsumedUnscheduledModules = cms.untracked.bool(True),
    dumpOptions = cms.untracked.bool(False),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(
            allowAnyLabel_=cms.required.untracked.uint32
        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(0)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(0),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step3 nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition
process.ALCARECOStreamAlCaPCCRandom = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOAlCaPCCRandom:RECO')
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('ALCARECO'),
        filterName = cms.untracked.string('AlCaPCCRandom')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('AlCaPCCRandom.root'),
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_alcaPCCIntegratorRandom_alcaPCCRandom_*'
    )
)
process.ALCARECOStreamPromptCalibProdLumiPCC = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOPromptCalibProdLumiPCC')
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('ALCARECO'),
        filterName = cms.untracked.string('PromptCalibProdLumiPCC')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('PromptCalibProdLumiPCC.root'),
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_rawPCCProd_*_*',
        'keep *_corrPCCProd_*_*'
    )
)

# Other statements
process.ALCARECOEventContent.outputCommands.extend(process.OutALCARECOAlCaPCCRandom_noDrop.outputCommands)
process.ALCARECOEventContent.outputCommands.extend(process.OutALCARECOPromptCalibProdLumiPCC_noDrop.outputCommands)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run3_data', '')

# Path and EndPath definitions
process.ALCARECOStreamAlCaPCCRandomOutPath = cms.EndPath(process.ALCARECOStreamAlCaPCCRandom)
process.ALCARECOStreamPromptCalibProdLumiPCCOutPath = cms.EndPath(process.ALCARECOStreamPromptCalibProdLumiPCC)

# Schedule definition
process.schedule = cms.Schedule(process.pathALCARECOPromptCalibProdLumiPCC,process.ALCARECOStreamAlCaPCCRandomOutPath,process.ALCARECOStreamPromptCalibProdLumiPCCOutPath)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)



# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
