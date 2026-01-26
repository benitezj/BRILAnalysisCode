#########################
import FWCore.ParameterSet.Config as cms

process = cms.Process("PCCLumiAnalyzer")

process.TFileService = cms.Service("TFileService", fileName = cms.string("pcctuple.root") )
  
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )


#### Here read the  desired list of input files for each job/run
#inputlist=cms.untracked.vstring('file:/eos/cms/store/data/Run2025D/AlCaLumiPixelsCountsGated/RAW/v1/000/394/468/00000/006174ff-519f-4a04-8b73-9ae6dff51388.root') #root1, for test
inputlist=cms.untracked.vstring() #test here
inputfile=os.getenv('INPUTFILE') #This is for the jobs
print(inputfile) #Test for log files
if inputfile != None :
    print('reading from input file: '+inputfile)
    infile = open(inputfile,'r')
    inputlist=cms.untracked.vstring(infile.readlines())
    infile.close()                 

if len(inputlist) == 0 : sys.exit("No input files")
print(inputlist)
process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)
 
#############################    init the reader   #######################################
process.PCCEventToTuple = cms.EDAnalyzer("PCCEventToTuple", 
        cms.PSet(
           #Check root file for event tags. Should look something like this:
           #recoPixelClusterCountsInEvent_hltAlcaPixelClusterCountsGated_alcaPCCEvent_HLT.present
           inputTag = cms.InputTag("hltAlcaPixelClusterCountsGated","alcaPCCEvent"),
           modVeto=cms.vint32(),
           bcidSelection=cms.vint32(),
        )
)


### select the bcids (L1 convention, +1 to the normal hlt convention)
process.PCCEventToTuple.bcidSelection.extend([73,136,540,740,1038,1154,1354,2045,2129,2329,2743,3069,3427])
print(process.PCCEventToTuple.bcidSelection)


## select the modules #DISABLED
vetofilename="" 
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/veto_2024/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2_+Block2_Sty1_Sty2+Block3_Sty1_Sty2+Block4_Sty1_Sty2+Block5_Sty1_Sty2+Block6_Sty1_Sty2+All_Blocks_Sty_Lty.txt'
if vetofilename !="":
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
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))


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

