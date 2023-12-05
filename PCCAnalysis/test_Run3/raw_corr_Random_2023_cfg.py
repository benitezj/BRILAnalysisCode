import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
import os
import sys

process = cms.Process("corrRECO")
inputlist=cms.untracked.vstring()

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

vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/veto_2022/veto2022_FStab2p08pLin04p025p_CStab2p1pLin04p03p_EStab06pLin03p_GStab06pLin03p_DStab06pLin03p_vdmStab1p04pNoise05p.txt'


print('reading from veto file: '+vetofilename)
vetofile = open(vetofilename,'r')
with vetofile as f:
   for line in f:
       process.rawPCCProd.RawPCCProducerParameters.modVeto.extend([int(line.rstrip())])
vetofile.close()
print(process.rawPCCProd.RawPCCProducerParameters.modVeto)

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
DQMStore = cms.Service("DQMStore")

process.load("DQMServices.Core.DQM_cfg")

process.corrPCCProd = DQMEDAnalyzer("CorrPCCProducer",
    CorrPCCProducerParameters = cms.PSet(
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

process.load("CondCore.CondDB.CondDB_cfi")
process.CondDB.connect = "sqlite_file:PCC_Corr.db"
process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDB,
    toPut = cms.VPSet(
    cms.PSet(record = cms.string('LumiCorrectionsRcd'),tag = cms.string('TestCorrections'))),
    loadBlobStreamer = cms.untracked.bool(False),
    timetype   = cms.untracked.string('lumiid')
    )


process.path1 = cms.Path(process.rawPCCProd+process.corrPCCProd+process.dqmEnvLumiPCC)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('PCC_RD.root'),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_rawPCCProd_*_*',
        'keep *_corrPCCProd_*_*')
)
process.outpath = cms.EndPath(process.out)

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

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
    infos = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    statistics = cms.untracked.vstring('cerr_stats'),
    suppressDebug = cms.untracked.vstring(),
    suppressInfo = cms.untracked.vstring(),
    suppressWarning = cms.untracked.vstring(),
    warnings = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)
