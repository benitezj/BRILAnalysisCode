#########################
import FWCore.ParameterSet.Config as cms

process = cms.Process("PCCLumiAnalyzer")

process.TFileService = cms.Service("TFileService", fileName = cms.string("pcctuple.root") )
  
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )


######
###Need to implement good run/LS list in cfg, currently hardcoded in the module
#goodrunlist = [[392382,40,76]]
#for x in goodrunlist:
#  for y in x:
#    print(x,y)
#exit()


#### DATA INPUT
process.source = cms.Source(
    "PoolSource",fileNames = cms.untracked.vstring(
        #'file:/eos/cms/store/group/comm_luminosity/PCC/ForLumiSystematics/2017/5Feb2018/AlCaLumiPixels/PCC_AlCaLumiPixels_Run2017B_5kLS/180205_190216/0000/raw_corr_PCC_RD_1.root',
        #'file:/eos/cms/tier0/store/data/Run2025B/AlCaLumiPixelsCountsPrompt/RAW/v1/000/391/874/00000/05a4b647-7901-48b0-81e8-fa7a6630c6a4.root',
       #'file:/eos/user/b/benitezj/BRIL/PCC_Run3/05a4b647-7901-48b0-81e8-fa7a6630c6a4.root',
       #'/eos/cms/tier0/store/data/Run2025C/AlCaLumiPixelsCountsPrompt/RAW/v1/000/392/382/00000/90ddcf6b-ee5d-45a4-a756-151e80489b5c.root
       #'file:/eos/user/b/benitezj/BRIL/PCC_Run3/90ddcf6b-ee5d-45a4-a756-151e80489b5c.root',
       'file:/eos/cms/tier0/store/data/Run2025C/AlCaLumiPixelsCountsPrompt/RAW/v1/000/392/382/00000/90ddcf6b-ee5d-45a4-a756-151e80489b5c.root',
       'file:/eos/cms/tier0/store/data/Run2025C/AlCaLumiPixelsCountsPrompt/RAW/v1/000/392/382/00000/73347719-4a98-4bfc-8248-2070b05102ca.root',
        #'file:./PCC_ZB.root'
        )                 
    )



#############################    init the reader   #######################################
process.PCCEventToTuple = cms.EDAnalyzer("PCCEventToTuple", 
        cms.PSet(
            #recoPixelClusterCountsInEvent_hltAlcaPixelClusterCounts_alcaPCCEvent_HLT.present
            inputTag = cms.InputTag("hltAlcaPixelClusterCounts","alcaPCCEvent"),
            modVeto=cms.vint32(),
        )
)

vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/veto_2024/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2_+Block2_Sty1_Sty2+Block3_Sty1_Sty2+Block4_Sty1_Sty2+Block5_Sty1_Sty2+Block6_Sty1_Sty2+All_Blocks_Sty_Lty.txt'
print('reading from veto file: '+vetofilename)
vetofile = open(vetofilename,'r')
with vetofile as f:
   for line in f:
       process.PCCEventToTuple.modVeto.extend([int(line.rstrip())])
vetofile.close()


process.p = cms.Path(process.PCCEventToTuple)


############################# MessageLogger #######################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000000
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
    statistics = cms.untracked.vstring('cerr_stats'),
    suppressDebug = cms.untracked.vstring(),
    suppressInfo = cms.untracked.vstring(),
    suppressWarning = cms.untracked.vstring(),
    warnings = cms.untracked.PSet(
        #placeholder = cms.untracked.bool(True)
        threshold = cms.untracked.string('WARNING')
    )
)

