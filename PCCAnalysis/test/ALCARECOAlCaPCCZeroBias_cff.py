import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi
ALCARECOZeroBiasHLT = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone(
    HLTPaths = cms.vstring("*ZeroBias*"),
    eventSetupPathsKey='',
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    andOr = cms.bool(True), # choose logical OR between Triggerbits
    throw = cms.bool(False) # tolerate triggers stated above, but not available
)

alcaPCCIntegratorZeroBias = cms.EDProducer("AlcaPCCIntegrator",
    AlcaPCCIntegratorParameters = cms.PSet(
        inputPccLabel = cms.string("hltAlcaPixelClusterCounts"),
        trigstring = cms.untracked.string("ZeroBias"),# input path
        ProdInst = cms.string("ZeroBias") # output path
    ),
)

# Sequence #
#seqALCARECOAlCaPCCZeroBias = cms.Sequence(ALCARECOZeroBiasHLT + alcaPCCIntegratorZeroBias)
seqALCARECOAlCaPCCZeroBias = cms.Sequence(alcaPCCIntegratorZeroBias)
