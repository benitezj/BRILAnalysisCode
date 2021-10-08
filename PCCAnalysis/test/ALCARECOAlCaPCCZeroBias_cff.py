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
        inputPccLabel = cms.string("alcaPCCEventProducer"),
        trigstring = cms.untracked.string("alcaPCCEvent"),##input instance
        ProdInst = cms.string("alcaPCCZeroBias") ##output instance
    ),
)

# Sequence #
#seqALCARECOAlCaPCCZeroBias = cms.Sequence(ALCARECOZeroBiasHLT + alcaPCCIntegratorZeroBias)
seqALCARECOAlCaPCCZeroBias = cms.Sequence(alcaPCCIntegratorZeroBias)
