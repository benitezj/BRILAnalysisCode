#!/bin/bash


#########################################
## * script to search for files of this form:
##     /eos/cms/store/data/Run2018A/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-PromptReco-v2/000/316/559/00000/3A1948E7-B85C-E811-BE2E-FA163E2965C4.root
##     /eos/cms/store/data/Run2018A/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-PromptReco-v1/000/316/060/00000/1AAF584E-0956-E811-B8CF-FA163E73FFA8.root
##     /eos/cms/store/data/Run2018D/AlCaLumiPixels0/ALCARECO/AlCaPCCZeroBias-PromptReco-v2/000/323/513/00000
## * this script takes no arguments
## * output are files with runnumber.txt with list of root files 
#######################################

period=$1 #Run2018A

##all run files are cleaned out
mkdir ./$period
rm -f ./$period/*.txt


##options
eos=/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select
specialRun=0  #0 -> AlCaLumiPixels  ,  1 -> merge AlCaLumiPixels0-N , not set merges all including Express
ZBorRdm=ZeroBias-PromptReco
#ZBorRdm=AlCaPCCRandom-PromptReco



###################################
##search files for a given stream in a given period
###################################
search(){
    local period=$1
    local stream=$2  
    local path=/store/data/$period/$stream/ALCARECO

    for v in `${eos} ls ${path} | grep ${ZBorRdm}`; do 
	local path1=$path/$v/000
	for r1 in `${eos} ls ${path1}`; do 
	    local path2=$path1/$r1
	    for r2 in `${eos} ls ${path2}`; do 
		echo "$r1$r2" 
		local path3=$path2/$r2/00000
		for f in `${eos} ls ${path3} | grep .root`; do 
	            ###files will be added to the run file
		    echo "file:/eos/cms/$path3/$f" >> ./$period/$r1$r2.txt  
		done
	    done
	done
    done
}


######################################
###search files for all streams in the run period
######################################
if [ "$period" == "" ]; then
	echo "no run period specified."
	return
fi

### loop over the streams
for s in `${eos} ls /store/data/$period/ | grep AlCaLumiPixels`; do 
	idx=`echo $s  | awk -F"AlCaLumiPixels" '{print $2}'`

	if [ "$specialRun" == "1" ] && [ "$idx" == "" ]; then
	# only process "AlCaLumiPixels[0-N]" 
	    continue
	fi

	if [ "$specialRun" == "0" ] && [ "$idx" != "" ]; then
	# only process "AlCaLumiPixels"
	    continue
	fi

	echo $s
	search $period $s
done    


