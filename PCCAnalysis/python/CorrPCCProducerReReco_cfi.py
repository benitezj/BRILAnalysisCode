import FWCore.ParameterSet.Config as cms

corrPCCProd = cms.EDProducer("CorrPCCProducerReReco",
    CorrPCCProducerReRecoParameters = cms.PSet(
        inLumiObLabel = cms.string("rawPCCProd"),
        ProdInst = cms.string("rawPCCRandom"),
        approxLumiBlockSize=cms.int32(50),
        trigstring = cms.untracked.string("corrPCCRand"),
        type2_a= cms.double(0.00072),
        type2_b= cms.double(0.014),
        subSystemFolder=cms.untracked.string('AlCaReco'),
        run = cms.int32(0)
    )
)
