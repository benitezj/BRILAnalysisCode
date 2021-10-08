import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi
ALCARECOZeroBiasHLT = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone(
    HLTPaths = cms.vstring("*ZeroBias*"),
    eventSetupPathsKey='',
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    andOr = cms.bool(True), # choose logical OR between Triggerbits
    throw = cms.bool(False) # tolerate triggers stated above, but not available
)

from EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi import siPixelDigis
siPixelDigisForLumiZB = siPixelDigis.cpu.clone(
    InputLabel = "hltFEDSelectorLumiPixels"
)

from RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizerPreSplitting_cfi import siPixelClustersPreSplitting
siPixelClustersForLumiZB = siPixelClustersPreSplitting.cpu.clone(
    src = "siPixelDigisForLumiZB"
)

alcaPCCProducerZeroBias = cms.EDProducer("AlcaPCCIntegrator",
    AlcaPCCIntegratorParameters = cms.PSet(
        inputPccLabel = cms.string("siPixelClustersForLumiZB"),
        trigstring = cms.untracked.string(""),
        ProdInst = cms.string("alcaPCCZeroBias")
    ),
)

# Sequence #
seqALCARECOAlCaPCCZeroBias = cms.Sequence(ALCARECOZeroBiasHLT + siPixelDigisForLumiZB + siPixelClustersForLumiZB + alcaPCCProducerZeroBias)
