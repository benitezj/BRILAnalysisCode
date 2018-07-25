#!/bin/bash


#########################################
##  search for files of this form:
## /eos/cms/store/data/Run2018A/AlCaLumiPixels0/ALCARECO/AlCaPCCZeroBias-PromptReco-v2/000/316/559/00000/3A1948E7-B85C-E811-BE2E-FA163E2965C4.root
#######################################


eos=/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select


###################################
##search files for a given stream in a given period
###################################
search(){
local period=$1
local stream=$2  
local path=/store/data/$period/$stream/ALCARECO/AlCaPCCZeroBias-PromptReco-v2/000
for r1 in `${eos} ls ${path}`; do 
    for r2 in `${eos} ls ${path}/$r1`; do 
	echo "$r1$r2" 
	for f in `${eos} ls ${path}/$r1/$r2/00000 | grep .root`; do 
	    ###files will be added to the run file
	    echo "file:/eos/cms/$path/$r1/$r2/00000/$f" >> ./$period/$r1$r2.txt  
	done
    done
done
}


######################################
###search files for all streams in the run period
######################################
seachPeriod(){

period=$1 #Run2018A
if [ "$period" == "" ]; then
    echo "no run period specified."
    return
fi

##all run files are cleaned out
mkdir ./$period
rm -f ./$period/*.txt


### loop over the streams
for s in `${eos} ls /store/data/$period/ | grep AlCaLumiPixels`; do 
    local idx=`echo $s  | awk -F"AlCaLumiPixels" '{print $2}'`
    if [ "$idx" != "" ]; then
	# only process "AlCaLumiPixels[0-N] not "AlCaLumiPixels" stream
	echo $s
	search $period $s
    fi
done

}

######################################
### search all run periods:
######################################
seachPeriod Run2018A
seachPeriod Run2018B
seachPeriod Run2018C
