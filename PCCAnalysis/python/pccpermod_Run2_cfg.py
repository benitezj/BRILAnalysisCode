#########################
import FWCore.ParameterSet.Config as cms
#import FWCore.PythonUtilities.LumiList as LumiList
import os,sys

process = cms.Process("LumiInfo")
process.path1 = cms.Path()
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


######################## 
## PoolSource module

inputlist=cms.untracked.vstring()

inputfile=os.getenv('INPUTFILE')
if inputfile != None : 
    print('reading from input file: '+inputfile)
    infile = open(inputfile,'r')
    inputlist_tmp=cms.untracked.vstring(infile.readlines())
    infile.close()

    #remove the local file prefix, cmssw will any find the correct site. File lists can be made using dasgoclient, or the local script
    for f in inputlist_tmp:
        f=f.replace('file:/eos/cms','')
        inputlist.insert(-1,f)

else:    
    #inputlist.insert(-1,'file:step5_ALCAPRODUCER.root')
    #inputlist.insert(-1,'file:/eos/cms/store/data/Run2023C/AlCaLumiPixelsCountsPrompt/ALCARECO/AlCaPCCRandom-PromptReco-v4/000/369/802/00000/ffa38dbd-9341-4b85-8a0c-02f4f165bd5d.root')
    #inputlist.insert(-1,'file:/eos/cms/store/data/Run2023C/AlCaLumiPixelsCountsPrompt/ALCARECO/AlCaPCCZeroBias-PromptReco-v1/000/367/095/00000/eb36faf7-3503-4596-a863-704c30ea12f9.root')
    #inputlist.insert(-1,'file:/eos/cms/store/data/Run2017A/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v1/2820000/8F63121D-337C-DC4B-92CE-F28F42C0358C.root')
    #inputlist.insert(-1,'file:/eos/cms/store/data/Run2017A/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/130000/E9CE9535-DBFE-4F4A-91DC-ACEA94BDFE55.root')
    #inputlist.insert(-1,'file:/eos/user/b/benitezj/BRIL/PCC/Run2018B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-13Mar2023_UL2018_PCC-v1/2550000/C57E5A9D-A444-8B41-8CE7-747EBAFC6BF1.root')
    inputlist.insert(-1,'file:/eos/cms/store/data/Run2017B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/120000/E57DFC3F-F7D0-8B40-8067-C6512B09FE63.root')

if len(inputlist) == 0 : sys.exit("No input files")
print(inputlist)
process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)


#######################################
#####Setup the conditions database
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.GlobalTag.globaltag = '106X_dataRun2_v37'
#process.GlobalTag.DumpStat = cms.untracked.bool( False )



##################
# rawPCC producer
process.rawPCCProd = cms.EDProducer("RawPCCProducer_modveto",
    RawPCCProducerParameters = cms.PSet(
        #2017 RD: recoPixelClusterCounts_alcaPCCProducerRandom_alcaPCCRandom_ALCA
        #2018 ZB: recoPixelClusterCounts_alcaPCCProducerZeroBias_alcaPCCZeroBias_RECO
        #2023 RD: recoPixelClusterCounts_alcaPCCIntegratorRandom_alcaPCCRandom_RECO.present
        inputPccLabel = cms.string("alcaPCCProducerZeroBias"),
        ProdInst = cms.string("alcaPCCZeroBias"),
        saveCSVFile=cms.untracked.bool(True)
    )    
) 



#process.path1 = cms.Path(process.rawPCCProd)
process.path1 += process.rawPCCProd


#####################
#From the end path, this is where we specify format for our output.
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('PCC_Veto.root'),
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
