#########################
import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
import os,sys
from BRILAnalysisCode.PCCAnalysis.message_logger_cfi import MessageLogger

process = cms.Process("LumiInfo")
process.path1 = cms.Path()
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
process.MessageLogger = MessageLogger

######################## 
inputlist=cms.untracked.vstring()
inputfile=os.getenv('INPUTFILE')
if inputfile != None : 
    print('reading from input file: '+inputfile)
    infile = open(inputfile,'r')
    inputlist=cms.untracked.vstring(infile.readlines())
    infile.close()
else:
    inputlist.insert(-1, 'file:/eos/user/b/benitezj/BRIL/LUMPOG/data/Run2017B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/00280FAD-0361-244F-AC51-B0DBA00A3744.root')

    
if len(inputlist) == 0 : sys.exit("No input files")
print(inputlist)

process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)


##################
process.rawPCCProd = cms.EDProducer("RawPCCProducer",
    RawPCCProducerParameters = cms.PSet(
        inputPccLabel = cms.string("alcaPCCProducerZeroBias"),
        ProdInst = cms.string("alcaPCCZeroBias"),
        outputProductName = cms.untracked.string("lumiInfo"), 
        ApplyCorrections=cms.untracked.bool(True),  
        saveCSVFile=cms.untracked.bool(True),
        modVeto=cms.vint32(),
        OutputValue = cms.untracked.string("Average"),
    )    
) 
print('RawPCCProducerParameters.ApplyCorrections: '+ str(process.rawPCCProd.RawPCCProducerParameters.ApplyCorrections))


###### veto list  
vetofilename = os.getenv('CMSSW_BASE')+'/src/BRILAnalysisCode/PCCAnalysis/test/vetoModules_2017_fixed.txt'
print('reading from veto file: '+vetofilename)
vetofile = open(vetofilename,'r')
with vetofile as f:
   for line in f:
       process.rawPCCProd.RawPCCProducerParameters.modVeto.extend([int(line.rstrip())])
vetofile.close()
print(process.rawPCCProd.RawPCCProducerParameters.modVeto)


process.path1 += process.rawPCCProd


###Conditions database
dbfile=os.getenv('DBFILE')
#dbfile='/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/afterglow_veto_1615/afterglow_merged/B.db' 
if dbfile : 
    print('will override DB file: '+ dbfile)
    process.load("CondCore.CondDB.CondDB_cfi")
    process.CondDB.connect = 'sqlite_file:'+dbfile
    process.GlobalTag = cms.ESSource("PoolDBESSource",process.CondDB,DumpStat=cms.untracked.bool(True),toGet = cms.VPSet(cms.PSet(record = cms.string('LumiCorrectionsRcd'),tag = cms.string('TestCorrections'))),)    


#####################
#From the end path, this is where we specify format for our output.
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('PCC_ZB.root'),
                               outputCommands = cms.untracked.vstring('drop *','keep *_rawPCCProd_*_*'),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('!*')
                                                                )
                               )

process.outpath = cms.EndPath(process.out)
