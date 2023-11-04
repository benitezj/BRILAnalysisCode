import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
import os
import sys

process = cms.Process("corrRECO")

inputlist=cms.untracked.vstring()
#inputlist.insert(-1,'file:/eos/cms/store/express/Run2022E/StreamALCALumiPixelsCountsExpress/ALCARECO/AlCaPCCRandom-Express-v1/000/360/327/00000/32c89930-ae66-4ed0-9ca2-01c62c8dbb1a.root')
#inputlist.insert(-1,'file:/eos/cms/store/express/Run2022F/StreamALCALumiPixelsCountsExpress/ALCARECO/AlCaPCCRandom-Express-v1/000/361/913/00000/4520a73b-b3d5-44f9-862a-a593742488d4.root')

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


#Make sure that variables match in producer.cc and .h
process.rawPCCProd = cms.EDProducer("RawPCCProducer",
    RawPCCProducerParameters = cms.PSet(
        inputPccLabel = cms.string("alcaPCCIntegratorRandom"),
        ProdInst = cms.string("alcaPCCRandom"),
        outputProductName = cms.untracked.string("rawPCCRandom"), 
        ApplyCorrections=cms.untracked.bool(False),
        OutputValue = cms.untracked.string(""), ##don't use "Average" , event averaging is done in the CorrPCCProducer
        modVeto=cms.vint32()
    )    
)

###### veto list
vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/veto_2022/veto2022_FStab2p08pLin04p025p_CStab2p1pLin04p03p_EStab06pLin03p_GStab06pLin03p_DStab06pLin03p_vdmStab1p04pNoise05p.txt'


print('reading from veto file: '+vetofilename)
vetofile = open(vetofilename,'r')
with vetofile as f:
   for line in f:
       process.rawPCCProd.RawPCCProducerParameters.modVeto.extend([int(line.rstrip())])
vetofile.close()
print(process.rawPCCProd.RawPCCProducerParameters.modVeto)


#Make sure that variables match in producer.cc and .h
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
DQMStore = cms.Service("DQMStore")

process.load("DQMServices.Core.DQM_cfg")
#process.load('HLTrigger.Timer.FastTimerService_cff')

process.corrPCCProd = DQMEDAnalyzer("CorrPCCProducer",
    CorrPCCProducerParameters = cms.PSet(
        #Mod factor to count lumi and the string to specify output 
        inLumiObLabel = cms.string("rawPCCProd"),
        ProdInst = cms.string("rawPCCRandom"),
        approxLumiBlockSize=cms.int32(50),
        trigstring = cms.untracked.string("corrPCCRand"), 
        type2_a= cms.double(0.0008406),
        type2_b= cms.double(0.01047),
        subSystemFolder=cms.untracked.string('AlCaReco')
    )
)

process.dqmEnvLumiPCC = DQMEDAnalyzer('DQMEventInfo',
                              subSystemFolder=cms.untracked.string('AlCaRecoEventInfo'))

#Output for the Database
process.load("CondCore.CondDB.CondDB_cfi")


process.CondDB.connect = "sqlite_file:PCC_Corr.db"

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDB,
    toPut = cms.VPSet(
    cms.PSet(record = cms.string('LumiCorrectionsRcd'),tag = cms.string('TestCorrections'))#,
     ),
    loadBlobStreamer = cms.untracked.bool(False),
    timetype   = cms.untracked.string('lumiid')
    #timetype   = cms.untracked.string('runnumber')
    )


process.path1 = cms.Path(process.rawPCCProd+process.corrPCCProd+process.dqmEnvLumiPCC)

#From the end path, this is where we specify format for our output.
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('PCC_RD.root'),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_rawPCCProd_*_*',
        'keep *_corrPCCProd_*_*')
)
process.outpath = cms.EndPath(process.out)


#added line for additional output summary `
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))


#
process.MessageLogger = cms.Service("MessageLogger",
    FrameworkJobReport = cms.untracked.PSet(
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        optionalPSet = cms.untracked.bool(True)
    ),
    categories = cms.untracked.vstring('FwkJob', 
        'FwkReport', 
        'FwkSummary', 
        'Root_NoDictionary'),
    cerr = cms.untracked.PSet(
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        FwkReport = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True),
            reportEvery = cms.untracked.int32(100000)
        ),
        FwkSummary = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True),
            reportEvery = cms.untracked.int32(10000)
        ),
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        Root_NoDictionary = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        noTimeStamps = cms.untracked.bool(False),
        optionalPSet = cms.untracked.bool(True),
        threshold = cms.untracked.string('INFO')
    ),
    cerr_stats = cms.untracked.PSet(
        optionalPSet = cms.untracked.bool(True),
        output = cms.untracked.string('cerr'),
        threshold = cms.untracked.string('WARNING')
    ),
    cout = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    debugModules = cms.untracked.vstring(),
    debugs = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    default = cms.untracked.PSet(

    ),
    destinations = cms.untracked.vstring('warnings', 
        'errors', 
        'infos', 
        'debugs', 
        'cout', 
        'cerr'),
    errors = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    #fwkJobReports = cms.untracked.vstring('FrameworkJobReport'),
    infos = cms.untracked.PSet(
        #Root_NoDictionary = cms.untracked.PSet(
        #    limit = cms.untracked.int32(0),
        #    optionalPSet = cms.untracked.bool(True)
        #),
        #optionalPSet = cms.untracked.bool(True),
        #placeholder = cms.untracked.bool(True)
        threshold = cms.untracked.string('INFO')
    ),
    statistics = cms.untracked.vstring('cerr_stats'),
    suppressDebug = cms.untracked.vstring(),
    suppressInfo = cms.untracked.vstring(),
    suppressWarning = cms.untracked.vstring(),
    warnings = cms.untracked.PSet(
        #placeholder = cms.untracked.bool(True)
        threshold = cms.untracked.string('WARNING')
    )
)
