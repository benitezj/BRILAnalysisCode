import FWCore.ParameterSet.Config as cms
import os,sys

process = cms.Process("LUMI")

#process.maxEvents = cms.untracked.PSet(
#    input = cms.untracked.int32(-1),
#    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
#)

process.MessageLogger.cerr.FwkSummary.reportEvery = cms.untracked.int32(100000)
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100000)

inputlist=cms.untracked.vstring()

inputfile=os.getenv('INPUTFILE')
if inputfile != None :
    print('reading from input file: '+inputfile)
    infile = open(inputfile,'r')
    inputlist=cms.untracked.vstring(infile.readlines())
    infile.close()
else :
    #inputlist=cms.untracked.vstring('file:/eos/user/b/benitezj/BRIL/PCC/ZeroBias/Commissioning2021/AlCaLumiPixelsCountsPrompt/ALCARECO/346174.root')
    #inputlist=cms.untracked.vstring('file:/eos/cms//store/backfill/1/data/Tier0_REPLAY_2022/AlCaLumiPixelsCountsPrompt/ALCARECO/RawPCCProducer-PromptReco-v17/000/346/512/00000/c4607885-1e01-443e-b51c-0e0df2fe2468.root')
    inputlist=cms.untracked.vstring('file:/afs/cern.ch/user/b/benitezj/output/BRIL/test_Tier0_REPLAY_2022-AlCaPCCZeroBias-PromptReco-v17/CMSSW_12_3_0_pre6/src/step5_onlyRawPCCProducer_ALCAPRODUCER-correctGT-fromT0ReplayALCARECO.root')

if len(inputlist) == 0 : sys.exit("No input files")
process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)

process.LumiPCCAnalyzer = cms.EDAnalyzer("LumiPCCAnalyzer",
                                         LumiInfoName = cms.string("ALCARECORawPCCProd"), 
                                         LumiInfoInst = cms.string("rawPCCProd"), 
)

process.p = cms.Path(process.LumiPCCAnalyzer)

