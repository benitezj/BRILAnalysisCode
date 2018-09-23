#########################
#Author: Sam Higginbotham
#Purpose: To investigate the AlCaPCCProducer input and output. 
#########################
import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("LumiInfo")
process.path1 = cms.Path()


#####Setup the conditions database
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '101X_dataRun2_Prompt_v9'
process.GlobalTag.DumpStat = cms.untracked.bool( True )

###replace the Conditions database if DBFILE is set in enviroment
dbfile=os.getenv('DBFILE')
if dbfile : 
    print 'will override DB file: ', dbfile
    process.load("CondCore.CondDB.CondDB_cfi")
    #process.CondDB.connect = 'sqlite_file:/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/316060.db'
    process.CondDB.connect = 'sqlite_file:'+dbfile
    process.GlobalTag = cms.ESSource("PoolDBESSource",
                                     process.CondDB,                                                                                                  
                                     DumpStat=cms.untracked.bool(True),                                                                                                
                                     toGet = cms.VPSet(cms.PSet(record = cms.string('LumiCorrectionsRcd'),                                                             
                                                                tag = cms.string('TestCorrections'))),)    


#process.load("CondCore.CondDB.CondDB_cfi")
#process.PoolDBESSource = cms.ESSource("PoolDBESSource",
#    DumpStat=cms.untracked.bool(True),
#    toGet = cms.VPSet(cms.PSet(
#        record = cms.string('LumiCorrectionsRcd'),
#        tag = cms.string("TestCorrections")
#    )),
#    #connect = cms.string('sqlite_file:Global.db')
#    connect = cms.string('sqlite_file:/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/316060.db')
#)


######################## 
## create PoolSource module

# setup the input files manually
#inputlist=cms.untracked.vstring()
#inputlist.insert(-1,'file:/eos/cms/store/data/Run2018A/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-PromptReco-v3/000/316/766/00000/12C5ED41-3A66-E811-B070-FA163EFF00DA.root')
#inputlist.insert(-1,'file:/eos/cms/store/data/Run2018A/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-PromptReco-v3/000/316/766/00000/2C0D3126-3366-E811-991A-FA163EC01FA2.root')
#inputlist.insert(-1,'file:/eos/cms//store/data/Run2018A/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-PromptReco-v1/000/316/060/00000/B8EF7674-0956-E811-81E0-FA163E29A824.root')


### read from file, environment variable must be set
inputfile=os.getenv('INPUTFILE')
if inputfile == '' : sys.exit('invalid INPUTFILE')
print 'reading from input file: ', inputfile
infile = open(inputfile,'r')
inputlist=cms.untracked.vstring(infile.readlines())
infile.close()

print inputlist
process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)


##################
#recoPixelClusterCounts_alcaPCCProducerZeroBias_alcaPCCZeroBias_RECO.obj
#Make sure that variables match in producer.cc and .h
process.rawPCCProd = cms.EDProducer("RawPCCProducer",
    RawPCCProducerParameters = cms.PSet(
        #Mod factor to count lumi and the string to specify output 
        inputPccLabel = cms.string("alcaPCCProducerZeroBias"),
        ProdInst = cms.string("alcaPCCZeroBias"),
        outputProductName = cms.untracked.string("lumiInfo"), 
        ApplyCorrections=cms.untracked.bool(True),  
        saveCSVFile=cms.untracked.bool(True),
        modVeto=cms.vint32(),
        OutputValue = cms.untracked.string("Average"),
    )    
) 


###### veto list  
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/test/vetoModules_2017_2018_VdM.txt'
vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/test/veto_master_VdM_ABCD_2018_newcuts.txt'
print 'reading from veto file: ', vetofilename
vetofile = open(vetofilename,'r')
with vetofile as f:
   for line in f:
       process.rawPCCProd.RawPCCProducerParameters.modVeto.extend([int(line.rstrip())])
vetofile.close()
print process.rawPCCProd.RawPCCProducerParameters.modVeto


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
    fwkJobReports = cms.untracked.vstring('FrameworkJobReport'),
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
