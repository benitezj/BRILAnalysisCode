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
    #inputlist.insert(-1, 'file:/eos/user/a/asehrawa/zerobias_27Oct2022_Run2018B/0C05753C-304A-F041-BB5E-6CA209E77721.root')
    #inputlist.insert(-1, 'file:/eos/user/a/asehrawa/zerobias_27Oct2022_Run2018B/0CAF3C5B-40B2-4C4B-A556-DB05343195EA.root')
    inputlist.insert(-1, 'file:/eos/user/a/asehrawa/zerobias_27Oct2022_Run2018B/24EF8507-316C-3F4C-9F0A-8D49084B6FAE.root')
    #inputlist.insert(-1, 'file:/eos/user/a/asehrawa/zerobias_27Oct2022_Run2018B/288996FE-3259-0945-8BEA-FD6F56B348CB.root')
    #inputlist.insert(-1, 'file:/eos/user/a/asehrawa/zerobias_27Oct2022_Run2018B/2E6B6FAE-C6C5-CA48-B743-0E157DB69C3D.root')
    #inputlist.insert(-1, 'file:/eos/user/a/asehrawa/zerobias_27Oct2022_Run2018B/34D28B26-EDE7-2746-B27E-55866DBB0368.root')
    #inputlist.insert(-1, 'file:/eos/user/a/asehrawa/zerobias_27Oct2022_Run2018B/4022C153-25F6-EF4A-8FB8-329A131D857.root')
    #inputlist.insert(-1, 'file:/eos/user/a/asehrawa/zerobias_27Oct2022_Run2018B/BD8DA275-4B36-3448-BBCD-66AB9572101B.root')
    #inputlist.insert(-1, 'file:/eos/user/a/asehrawa/zerobias_27Oct2022_Run2018B/BEDAB030-3FE8-A848-BEE8-318140C11F20.root')
    #inputlist.insert(-1, 'file:/eos/user/a/asehrawa/zerobias_27Oct2022_Run2018B/C00DEB2A-C113-7047-816C-16E24CCB7C8D.root')
    
    #inputlist.insert(-1,'file:/eos/cms/store/data/Run2018A/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-PromptReco-v3/000/316/766/00000/12C5ED41-3A66-E811-B070-FA163EFF00DA.root')
    #inputlist.insert(-1,'root://xrootd-cms.infn.it//store/data/Commissioning2018/AlCaLumiPixels0/ALCARECO/AlCaPCCZeroBias-02May2018-v1/70000/AA1D32AA-CE57-E811-B631-FA163E09BABD.root')
    #inputlist.insert(-1,'root://xrootd-cms.infn.it//store/data/Commissioning2018/AlCaLumiPixels2/ALCARECO/AlCaPCCZeroBias-02May2018-v1/30000/F451E0E9-C553-E811-8AB7-FA163E4D19DB.root')
    
if len(inputlist) == 0 : sys.exit("No input files")
print(inputlist)
process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)

jsonfile=os.getenv('JSONFILE')
if jsonfile != '' :
   #print('will apply json file: '+jsonfile)
   process.source.lumisToProcess = LumiList.LumiList(filename = jsonfile).getVLuminosityBlockRange()
   print('LumisToProcess: ')
   print( process.source.lumisToProcess)

#######################################
#####Setup the conditions database
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '101X_dataRun2_Prompt_v9'
process.GlobalTag.DumpStat = cms.untracked.bool( False )

###replace the Conditions database if DBFILE is set in enviroment
dbfile=os.getenv('DBFILE')
#dbfile='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-17Nov2017/Run2017G/AlCaPCCRandom-17Nov2017.db'
dbfile='/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/afterglow_veto_1615/afterglow_merged/B.db' 

if dbfile : 
    print('will override DB file: '+ dbfile)
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




##################
# rawPCC producer
process.rawPCCProd = cms.EDProducer("RawPCCProducer",
    RawPCCProducerParameters = cms.PSet(
        #Mod factor to count lumi and the string to specify output 
        inputPccLabel = cms.string("alcaPCCProducerZeroBias"),
        ProdInst = cms.string("alcaPCCZeroBias"),
#        inputPccLabel = cms.string("alcaPCCProducerRandom"),
#        ProdInst = cms.string("alcaPCCRandom"),
        outputProductName = cms.untracked.string("lumiInfo"), 
        ApplyCorrections=cms.untracked.bool(True),  
        saveCSVFile=cms.untracked.bool(True),
        modVeto=cms.vint32(),
        OutputValue = cms.untracked.string("Average"),
    )    
) 
print('RawPCCProducerParameters.ApplyCorrections: '+ str(process.rawPCCProd.RawPCCProducerParameters.ApplyCorrections))


###### veto list  
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/test/veto_master_VdM_ABCD_2018_newcuts.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/test/mergedModuleList.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/test/veto_master_VdM_ABCD_2018_newcuts_SamTest_Lo.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/test/veto_master_VdM_ABCD_2018_newcuts_SamTest_Hi.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/test/veto_lateRunD_lowcut_tight.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/test/veto_lateRunD_lowcut_tight_F3P2.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/PCCTools/VetoModules/vetoModules_2017.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/plots/veto_Run2018A.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/plots/veto_2018_vdM.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/plots/veto_Run2018C.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/plots/veto_Run2018D1.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/plots/veto_Run2018D2.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/plots/veto_Run2018D3.txt'
#vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/plots/veto_Run2018D4.txt'
vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/test/veto_common_1615.txt'

print('reading from veto file: '+vetofilename)
vetofile = open(vetofilename,'r')
with vetofile as f:
   for line in f:
       process.rawPCCProd.RawPCCProducerParameters.modVeto.extend([int(line.rstrip())])
vetofile.close()
print(process.rawPCCProd.RawPCCProducerParameters.modVeto)


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
