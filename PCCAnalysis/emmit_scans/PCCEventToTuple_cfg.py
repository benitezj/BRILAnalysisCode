#########################
import FWCore.ParameterSet.Config as cms

process = cms.Process("PCCLumiAnalyzer")

process.TFileService = cms.Service("TFileService", fileName = cms.string("pcctuple.root") )
  
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )


#### Here read the  desired list of input files for each job/run
inputlist=cms.untracked.vstring()
inputfile=os.getenv('INPUTFILE')
if inputfile != None :
    print('reading from input file: '+inputfile)
    infile = open(inputfile,'r')
    inputlist=cms.untracked.vstring(infile.readlines())
    infile.close()

#inputlist=cms.untracked.vstring(
#'file:/eos/user/b/benitezj/BRIL/PCC_Run3/392538/1a0dc589-1da6-46d9-8c2c-2468e9959cf7.root',
#'file:/eos/user/b/benitezj/BRIL/PCC_Run3/392538/34d69768-df78-42a4-9379-0efe2dc03cc1.root',
#'file:/eos/user/b/benitezj/BRIL/PCC_Run3/392538/3f5c4a57-203a-49cd-9e3b-601f2b96a2b0.root',
#'file:/eos/user/b/benitezj/BRIL/PCC_Run3/392538/caa64211-a72a-42d6-b934-a571a5de045d.root',
#'file:/eos/user/b/benitezj/BRIL/PCC_Run3/392538/d26b18e8-2f89-4af2-996f-cbe450e9d8ea.root',
#'file:/eos/user/b/benitezj/BRIL/PCC_Run3/392538/e7cb51bd-e0ac-479c-b5cb-f3d96305901c.root',
#'file:/eos/user/b/benitezj/BRIL/PCC_Run3/392538/ebb014da-509d-4884-85cf-7c95d5ad8970.root',)                 

if len(inputlist) == 0 : sys.exit("No input files")
print(inputlist)
process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)


####
## apply selection on the LS's corresponding to the scan
#process.source.lumisToProcess = cms.untracked(cms.VLuminosityBlockRange())
#process.source.lumisToProcess.extend(["392382:40-392382:76"])
#process.source.lumisToProcess.extend(["392524:32-392524:60"])
#process.source.lumisToProcess.extend(["392538:46-392538:65"]) 




#############################    init the reader   #######################################
process.PCCEventToTuple = cms.EDAnalyzer("PCCEventToTuple", 
        cms.PSet(
           #recoPixelClusterCountsInEvent_hltAlcaPixelClusterCounts_alcaPCCEvent_HLT.present
           inputTag = cms.InputTag("hltAlcaPixelClusterCounts","alcaPCCEvent"),
           modVeto=cms.vint32(),
           bcidSelection=cms.vint32(),
        )
)


### select the bcids (L1 convention )
process.PCCEventToTuple.bcidSelection.extend([57,67,92,919,951,961,986,1998,2727,3272])
print(process.PCCEventToTuple.bcidSelection)


## select the modules
vetofilename="" ##currently disabled, to be done in the tuple -> hd5 code
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

