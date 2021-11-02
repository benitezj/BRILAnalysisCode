import FWCore.ParameterSet.Config as cms
import os,sys

process = cms.Process("LUMI")

#process.maxEvents = cms.untracked.PSet(
#    input = cms.untracked.int32(-1),
#    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
#)


inputlist=cms.untracked.vstring()

inputfile=os.getenv('INPUTFILE')
if inputfile != None :
    print('reading from input file: '+inputfile)
    infile = open(inputfile,'r')
    inputlist=cms.untracked.vstring(infile.readlines())
    infile.close()
else :
    inputlist=cms.untracked.vstring('file:/eos/user/b/benitezj/BRIL/PCC/ZeroBias/Commissioning2021/AlCaLumiPixelsCountsPrompt/ALCARECO/346174.root')


if len(inputlist) == 0 : sys.exit("No input files")
process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)

process.LumiPCCAnalyzer = cms.EDAnalyzer("LumiPCCAnalyzer",
    LumiInfoName = cms.string("ALCARECORawPCCProd"), #input
    LumiInfoInst = cms.string("rawPCCProd"), #input
)

process.p = cms.Path(process.LumiPCCAnalyzer)

