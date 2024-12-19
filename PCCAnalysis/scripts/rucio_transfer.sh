
## setup rucio: https://twiki.cern.ch/twiki/bin/viewauth/CMS/Rucio
#source /cvmfs/cms.cern.ch/rucio/setup-py3.sh
#voms-proxy-init -voms cms -rfc -valid 192:00
#export RUCIO_ACCOUNT=`whoami`


#time=2592000 # one month
time=7776000 # 3 months
site=T2_CH_CERN 

#dataset=/AlCaLumiPixels/Run2017B-AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v2/ALCARECO
#dataset=/AlCaLumiPixels/Run2017C-AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v1/ALCARECO
#dataset=/AlCaLumiPixels/Run2017D-AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v1/ALCARECO
#dataset=/AlCaLumiPixels/Run2017E-AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v1/ALCARECO
#dataset=/AlCaLumiPixels/Run2017F-AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v1/ALCARECO
#
#dataset=/AlCaLumiPixels/Run2017B-AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/ALCARECO
#dataset=/AlCaLumiPixels/Run2017C-AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/ALCARECO
#dataset=/AlCaLumiPixels/Run2017D-AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/ALCARECO
#dataset=/AlCaLumiPixels/Run2017E-AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/ALCARECO
dataset=/AlCaLumiPixels/Run2017F-AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/ALCARECO



rucio add-rule \
      cms:${dataset} \
      1 \
      $site \
      --lifetime $time \
      --activity "User AutoApprove" \
      --ask-approval \
      --comment "Data needed for Run lumi paper"

