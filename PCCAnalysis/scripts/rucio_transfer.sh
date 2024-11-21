#time=2592000 # one month
time=7776000 # 3 months
site=T2_CH_CERN 
#dataset=/AlCaLumiPixels/Run2017B-AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v2/ALCARECO
dataset=/AlCaLumiPixels/Run2017B-AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/ALCARECO

rucio add-rule \
      cms:${dataset} \
      1 \
      $site \
      --lifetime $time \
      --activity "User AutoApprove" \
      --ask-approval \
      --comment "Data needed for Run lumi paper"

