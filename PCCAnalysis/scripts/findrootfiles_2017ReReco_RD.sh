#!/bin/bash

RUNLIST=$1 # file with list of runs
PERIOD=$2 # eg. Run2017B

DATASET=AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v2
DASDATASET=/AlCaLumiPixels/${PERIOD}-${DATASET}/ALCARECO

#eg. /eos/cms/store/data/Run2017B/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-28Aug24_UL2017_PCCRandom-v2
#EOS_PATH=/eos/cms/store/data/$PERIOD/AlCaLumiPixels/ALCARECO/$DATASET/

echo $RUNLIST
echo $PERIOD
echo $DASDATASET

if [ -d ./$PERIOD ]; then
    echo "output dir exists, need to remove it first"
    return
fi
mkdir ./$PERIOD

####method searchin for files in eos, puts all files into all runs
#find "$EOS_PATH/" -name "*.root" -type f | while read root_file; do
#    for r in `cat $RUNLIST`;do
#	# in this code all files will be used in all runs, run number are filtered in the CorrPCCProducer
#	echo "file:${root_file}" >> $PERIOD/${r}.txt
#    done
#done

##method using dasgoclient to select files by run    
for r in `cat $RUNLIST`; do
    source /cvmfs/cms.cern.ch/common/dasgoclient --query="file run=${r} dataset=${DASDATASET}" > $PERIOD/${r}.txt
    
    sed -i -e 's/\/store\/data/file:\/eos\/cms\/store\/data/g' $PERIOD/${r}.txt
    
done

