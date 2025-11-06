
##Step 1
## setup rucio: https://twiki.cern.ch/twiki/bin/viewauth/CMS/Rucio
voms-proxy-init --voms cms --valid 192:00
source /cvmfs/cms.cern.ch/rucio/setup-py3.sh
#voms-proxy-init -voms cms -rfc -valid 192:00
export RUCIO_ACCOUNT=`whoami`
rucio whoami

##Step 2: choose site and time
#time=2592000 # one month
time=7776000 # 3 months
site=T2_CH_CERN 

#Step 3: choose dataset and run command

###2017 reprocessing
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
#dataset=/AlCaLumiPixels/Run2017F-AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/ALCARECO

###For emmittance 2025 scans
#dataset=/AlCaLumiPixelsCountsPrompt/Run2025B-v1/RAW
#dataset=/AlCaLumiPixelsCountsPrompt/Run2025C-v1/RAW
#dataset=/AlCaLumiPixelsCountsPrompt/Run2025D-v1/RAW
#dataset=/AlCaLumiPixelsCountsPrompt/Run2025E-v1/RAW

### 2025 vdM
#dataset=/AlCaLumiPixelsCountsGated/Run2025D-v1/RAW

### 2025 Randoms and ZeroBias
#dataset=/AlCaLumiPixelsCountsPrompt/Run2025C-AlCaPCCRandom-PromptReco-v1/ALCARECO

#dataset=/AlCaLumiPixelsCountsPrompt/Run2025B-AlCaPCCZeroBias-PromptReco-v1/ALCARECO
#dataset=/AlCaLumiPixelsCountsPrompt/Run2025C-AlCaPCCZeroBias-PromptReco-v1/ALCARECO
#dataset=/AlCaLumiPixelsCountsPrompt/Run2025C-AlCaPCCZeroBias-PromptReco-v2/ALCARECO
#dataset=/AlCaLumiPixelsCountsPrompt/Run2025D-AlCaPCCZeroBias-PromptReco-v1/ALCARECO
#dataset=/AlCaLumiPixelsCountsPrompt/Run2025E-AlCaPCCZeroBias-PromptReco-v1/ALCARECO
#dataset=/AlCaLumiPixelsCountsPrompt/Run2025F-AlCaPCCZeroBias-PromptReco-v1/ALCARECO
dataset=/AlCaLumiPixelsCountsPrompt/Run2025G-AlCaPCCZeroBias-PromptReco-v1/ALCARECO 


## 2025 vdM
#dataset=/AlCaLumiPixelsCountsGated/Run2025D-v1/RAW

##ppRef 2024
#dataset=/AlCaLumiPixelsCountsPrompt/Run2024J-v1/RAW
#dataset=/AlCaLumiPixelsCountsPromptHighRate4/Run2024J-v1/RAW
#dataset=/AlCaLumiPixelsCountsGated/Run2024J-v1/RAW


 rucio add-rule \
       cms:${dataset} \
       1 \
       $site \
       --lifetime $time \
       --activity "User AutoApprove" \
       --ask-approval \
       --comment "Data needed for Run lumi paper"

#command="rucio rule add"
#command=$command" -d cms:"$dataset
#command=$command" --rses "$site
#command=$command" --copies 1 --lifetime 7776000 --ask-approval"
#command=$command" --comment \"Data needed for lumi measurement\""
#echo $command


#step 4: Update existing rule
 
