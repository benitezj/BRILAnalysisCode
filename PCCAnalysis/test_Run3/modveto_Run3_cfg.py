#########################
import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
import os,sys

process = cms.Process("LumiInfo")
process.path1 = cms.Path()




######################## 
## PoolSource module

inputlist=cms.untracked.vstring()

inputfile=os.getenv('INPUTFILE')
if inputfile != None : 
    print('reading from input file: '+inputfile)
    infile = open(inputfile,'r')
    inputlist=cms.untracked.vstring(infile.readlines())
    infile.close()
else:    
    #inputlist.insert(-1,'file:step5_ALCAPRODUCER.root')
    inputlist.insert(-1,'file:/eos/cms//store/data/Run2022C/AlCaLumiPixelsCountsPrompt/ALCARECO/AlCaPCCZeroBias-PromptReco-v1/000/355/862/00000/87fecadb-45ce-4701-bb7f-6489b5689e0a.root')
    

if len(inputlist) == 0 : sys.exit("No input files")
print(inputlist)
process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)

jsonfile=os.getenv('JSONFILE')
if jsonfile != None :
   print('will apply json file: '+ jsonfile)
   process.source.lumisToProcess = LumiList.LumiList(filename = jsonfile).getVLuminosityBlockRange()
   print('LumisToProcess: ')
   print(process.source.lumisToProcess)

#######################################
#####Setup the conditions database
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = '101X_dataRun2_Prompt_v9'
process.GlobalTag.globaltag = '113X_dataRun3_Prompt_Candidate_2021_06_21_16_59_39'
process.GlobalTag.DumpStat = cms.untracked.bool( False )



##################
# rawPCC producer
process.rawPCCProd = cms.EDProducer("RawPCCProducer",
    RawPCCProducerParameters = cms.PSet(
        inputPccLabel = cms.string("alcaPCCIntegratorZeroBias"),
        ProdInst = cms.string("alcaPCCZeroBias"),
        outputProductName = cms.untracked.string("lumiInfo"), 
        ApplyCorrections=cms.untracked.bool(False),  
        saveCSVFile=cms.untracked.bool(True),
        modVeto=cms.vint32(),
        OutputValue = cms.untracked.string("Average"),
    )    
) 



#process.path1 = cms.Path(process.rawPCCProd)
process.path1 += process.rawPCCProd


#####################
#From the end path, this is where we specify format for our output.
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('PCC_ZB.root'),
                               outputCommands = cms.untracked.vstring('drop *', 
                                                                      'keep *_rawPCCProd_*_*'),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('!*')
                                                                 )
                               )

process.outpath = cms.EndPath(process.out)



#####################################
####Configure MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000000
#added line for additional output summary 
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

#For message summary 
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
 #       'infos', 
        'debugs', 
        'cout', 
        'cerr'),
    errors = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
#    fwkJobReports = cms.untracked.vstring('FrameworkJobReport'),
#    infos = cms.untracked.PSet(
#        #Root_NoDictionary = cms.untracked.PSet(
#        #    limit = cms.untracked.int32(0),
#        #    optionalPSet = cms.untracked.bool(True)
#        #),
#        #optionalPSet = cms.untracked.bool(True),
#        #placeholder = cms.untracked.bool(True)
#        threshold = cms.untracked.string('INFO')
#    ),
    statistics = cms.untracked.vstring('cerr_stats'),
    suppressDebug = cms.untracked.vstring(),
    suppressInfo = cms.untracked.vstring(),
    suppressWarning = cms.untracked.vstring(),
    warnings = cms.untracked.PSet(
        #placeholder = cms.untracked.bool(True)
        threshold = cms.untracked.string('WARNING')
    )
)
