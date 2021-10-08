import FWCore.ParameterSet.Config as cms



#Make sure that variables match in producer.cc and .h
ALCARECORawPCCProd = cms.EDProducer("RawPCCProducer",
    RawPCCProducerParameters = cms.PSet(
        #Mod factor to count lumi and the string to specify output
        inputPccLabel = cms.string("alcaPCCIntegratorZeroBias"),
        ProdInst = cms.string("alcaPCCZeroBias"),
        OutputValue = cms.untracked.string("Average"),
        outputProductName = cms.untracked.string("rawPCCProd"),
        #Below is a list of module IDs that will be ignored in calculation of luminosity
        ApplyCorrections = cms.untracked.bool(True),
        modVeto=cms.vint32()
    )
)

seqALCARECORawPCCProducer = cms.Sequence(ALCARECORawPCCProd)
