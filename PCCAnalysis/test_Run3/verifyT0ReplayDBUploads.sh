
FIRSTIOV=$1
LASTIOV=$2

STRING="'{\"start_iov\": \"${FIRSTIOV}\", \"end_iov\": \"${LASTIOV}\"}'"
echo ${STRING}

/cvmfs/cms.cern.ch/slc7_amd64_gcc10/cms/cmssw/CMSSW_12_3_5/bin/slc7_amd64_gcc10/getPayloadData.py \
--plugin pluginLumiCorrections_PayloadInspector \ 
--plot plot_LumiCorrectionsSummary \
--tag LumiPCC_Corrections_prompt \
--time_type Lumi \
--iovs ${STRING} \
--db Prod 

