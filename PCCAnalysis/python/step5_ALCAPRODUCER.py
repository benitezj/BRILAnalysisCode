# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step5 --process RECO --conditions auto:run2_data --scenario pp -s ALCAPRODUCER:AlCaPCCZeroBias+RawPCCProducer --datatier ALCARECO --filein file:/eos/cms/tier0/store/data/Commissioning2021/AlCaLumiPixelsCountsPrompt/RAW/v1/000/346/304/00000/abfcd5ff-d534-4f0c-8dcf-96e11a20613f.root --data --eventcontent ALCARECO --era Run2_2018 -n 100
import FWCore.ParameterSet.Config as cms
import os,sys

from Configuration.Eras.Era_Run2_2018_cff import Run2_2018

process = cms.Process('RECO',Run2_2018)

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

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

process.MessageLogger.cerr.FwkSummary.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# Input source
#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('file:/eos/cms/tier0/store/data/Commissioning2021/AlCaLumiPixelsCountsPrompt/RAW/v1/000/346/304/00000/abfcd5ff-d534-4f0c-8dcf-96e11a20613f.root'),
#    secondaryFileNames = cms.untracked.vstring()
#)

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
    annotation = cms.untracked.string('step5 nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.ALCARECOoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('ALCARECO'),
        filterName = cms.untracked.string('StreamALCACombined')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('step5_ALCAPRODUCER.root'),
    outputCommands = process.ALCARECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.ALCARECOEventContent.outputCommands.extend(process.OutALCARECOAlCaPCCZeroBias_noDrop.outputCommands)
process.ALCARECOEventContent.outputCommands.extend(process.OutALCARECORawPCCProducer_noDrop.outputCommands)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# Path and EndPath definitions
process.endjob_step = cms.EndPath(process.endOfProcess)
process.ALCARECOoutput_step = cms.EndPath(process.ALCARECOoutput)

# Schedule definition
process.schedule = cms.Schedule(process.pathALCARECOAlCaPCCZeroBias,process.pathALCARECORawPCCProducer,process.endjob_step,process.ALCARECOoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)



# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
