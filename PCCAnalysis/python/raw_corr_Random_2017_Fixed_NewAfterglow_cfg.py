#########################
import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
import os
import sys
from BRILAnalysisCode.PCCAnalysis.message_logger_cfi import MessageLogger

process = cms.Process("corrRECO")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
process.MessageLogger = MessageLogger
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )


############################33333
#### INPUT
inputlist=cms.untracked.vstring()
#inputlist.insert(-1,'file:/eos/cms/store/data/Run2017B/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v2/2520000/B786C70B-3FB3-7142-ACF5-57305F6FC368.root')
#inputlist.insert(-1,'file:/eos/cms/store/data/Run2017B/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v2/2520000/F9C45FD9-3A2C-2B40-8CEE-04F6AFAB04A8.root')
#inputlist.insert(-1,'file:/eos/cms/store/data/Run2017B/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v2/2520000/6B35BD08-4539-C447-A939-99A165A92959.root')
#inputlist.insert(-1,'file:/eos/cms/store/data/Run2017B/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v2/2520000/448F8125-E5CE-1447-9946-AFE3F5F26702.root')
#inputlist.insert(-1,'file:/eos/cms/store/data/Run2017B/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v2/2520000/F9585FE0-44B4-9340-896C-55921BAB4BF4.root')

## read from file, environment variable must be set
if len(inputlist) == 0 :
   inputfile=os.getenv('INPUTFILE')
   if inputfile == '' : sys.exit('invalid INPUTFILE')
   print('reading from input file: '+inputfile)
   infile = open(inputfile,'r')
   inputlist=cms.untracked.vstring(infile.readlines())
   infile.close()
   
print(inputlist)
process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)


##############################################
# RawPCCProducer config and module veto
process.rawPCCProd = cms.EDProducer("RawPCCProducer",
    RawPCCProducerParameters = cms.PSet(
        #Mod factor to count lumi and the string to specify output 
        inputPccLabel = cms.string("alcaPCCProducerRandom"),
        ProdInst = cms.string("alcaPCCRandom"),
        OutputValue = cms.untracked.string("Total"),
        outputProductName = cms.untracked.string("rawPCCRandom"),
        saveCSVFile = cms.untracked.bool(False),
        modVeto=cms.vint32()
    )    
)

vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/test/vetoModules_2017_fixed.txt'
if vetofilename == '' : sys.exit('invalid veto file')
print('reading from veto file: '+vetofilename)
vetofile = open(vetofilename,'r')
with vetofile as f:
   for line in f:
       process.rawPCCProd.RawPCCProducerParameters.modVeto.extend([int(line.rstrip())])
vetofile.close()
print(process.rawPCCProd.RawPCCProducerParameters.modVeto)


#### DQM EDAnalyzer
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
DQMStore = cms.Service("DQMStore")
process.load("DQMServices.Core.DQM_cfg")

process.corrPCCProd = DQMEDAnalyzer("CorrPCCProducerReReco",
      CorrPCCProducerReRecoParameters = cms.PSet(
         inLumiObLabel = cms.string("rawPCCProd"),
         ProdInst = cms.string("rawPCCRandom"),
         approxLumiBlockSize=cms.int32(50),
         trigstring = cms.untracked.string("corrPCCRand"), 
         type2_a= cms.double(0.0008228),
         type2_b= cms.double(0.01316),
         subSystemFolder=cms.untracked.string('AlCaReco'),
         minimumNumberOfEvents=cms.int32(1000),
         applyPedestal=cms.bool(True),
    )
)

runnumber=os.getenv('RUNNUMBER')
if runnumber == '' : sys.exit('invalid run number')
process.corrPCCProd.CorrPCCProducerReRecoParameters.run = cms.int32(int(runnumber))

### schedule the modules
process.dqmEnvLumiPCC = DQMEDAnalyzer('DQMEventInfo',subSystemFolder=cms.untracked.string('AlCaRecoEventInfo'))
process.path1 = cms.Path(process.rawPCCProd+process.corrPCCProd+process.dqmEnvLumiPCC)

##### Output DB 
process.load("CondCore.CondDB.CondDB_cfi")
process.CondDB.connect = "sqlite_file:PCC_Corr.db"
process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDB,
    toPut = cms.VPSet(
    cms.PSet(record = cms.string('LumiCorrectionsRcd'),tag = cms.string('TestCorrections'))),
    loadBlobStreamer = cms.untracked.bool(False),
    timetype   = cms.untracked.string('lumiid')
    #timetype   = cms.untracked.string('runnumber')
    )

###Output ROOT file
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('PCC_RD.root'),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_rawPCCProd_*_*',
        'keep *_corrPCCProd_*_*')
)
process.outpath = cms.EndPath(process.out)
