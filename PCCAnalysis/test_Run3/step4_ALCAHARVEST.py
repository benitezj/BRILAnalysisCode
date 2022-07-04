# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step4 -s ALCAHARVEST:LumiPCC --conditions auto:run3_data --era Run3 --scenario pp --datatier DQM --eventcontent DQM --data --filein file:/eos/cms/store/express/Run2022A/StreamALCALumiPixelsCountsExpress/ALCAPROMPT/PromptCalibProdLumiPCC-Express-v1/000/352/416/00000/c96bf8ed-4934-4ff9-8560-bf836494a7cb.root -n 100 --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process('ALCAHARVEST',Run3)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.AlCaHarvesting_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

# Input source
import os,sys
inputlist=cms.untracked.vstring()
inputfile=os.getenv('INPUTFILE')
if inputfile != None :
    print('reading from input file: '+inputfile)
    infile = open(inputfile,'r')
    inputlist=cms.untracked.vstring(infile.readlines())
    infile.close()
if len(inputlist) == 0 : 
    sys.exit("No input files")
process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)


process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    SkipEvent = cms.untracked.vstring(),
    accelerators = cms.untracked.vstring('*'),
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


process.MessageLogger.cerr.FwkSummary.reportEvery = cms.untracked.int32(10000)
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10000)


# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step4 nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
process.PoolDBOutputService.toPut.append(process.ALCAHARVESTLumiPCC_dbOutput)
process.pclMetadataWriter.recordsToMap.append(process.ALCAHARVESTLumiPCC_metadata)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run3_data', '')

# Path and EndPath definitions
process.ALCAHARVESTDQMSaveAndMetadataWriter = cms.Path(process.dqmSaver+process.pclMetadataWriter)
process.BeamSpotByLumi = cms.Path(process.ALCAHARVESTBeamSpotByLumi)
process.BeamSpotByRun = cms.Path(process.ALCAHARVESTBeamSpotByRun)
process.BeamSpotHPByLumi = cms.Path(process.ALCAHARVESTBeamSpotHPByLumi)
process.BeamSpotHPByRun = cms.Path(process.ALCAHARVESTBeamSpotHPByRun)
process.BeamSpotHPLowPUByLumi = cms.Path(process.ALCAHARVESTBeamSpotHPLowPUByLumi)
process.BeamSpotHPLowPUByRun = cms.Path(process.ALCAHARVESTBeamSpotHPLowPUByRun)
process.EcalPedestals = cms.Path(process.ALCAHARVESTEcalPedestals)
process.LumiPCC = cms.Path(process.ALCAHARVESTLumiPCC)
process.PPSAlignment = cms.Path(process.ALCAHARVESTPPSAlignment)
process.PPSDiamondSampicTimingCalibration = cms.Path(process.ALCAHARVESTPPSDiamondSampicTimingCalibration)
process.PPSTimingCalibration = cms.Path(process.ALCAHARVESTPPSTimingCalibration)
process.SiPixelAli = cms.Path(process.ALCAHARVESTSiPixelAli)
process.SiPixelLA = cms.Path(process.ALCAHARVESTSiPixelLorentzAngle)
process.SiPixelQuality = cms.Path(process.dqmEnvSiPixelQuality+process.ALCAHARVESTSiPixelQuality)
process.SiStripGains = cms.Path(process.ALCAHARVESTSiStripGains)
process.SiStripGainsAAG = cms.Path(process.ALCAHARVESTSiStripGainsAAG)
process.SiStripHitEff = cms.Path(process.ALCAHARVESTSiStripHitEfficiency)
process.SiStripQuality = cms.Path(process.ALCAHARVESTSiStripQuality)

# Schedule definition
process.schedule = cms.Schedule(process.LumiPCC,process.ALCAHARVESTDQMSaveAndMetadataWriter)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)



# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
