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

######################################
###search files for all streams in the run period
######################################
if [ "$period" == "" ]; then
	echo "no run period specified."
	return
fi

eospath=/eos/cms/store/data/$period
echo $eospath

##options
#eos=/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select
eos=''
specialRun=1  #0 -> AlCaLumiPixels  ,  1 -> merge AlCaLumiPixels0-N , not set merges all including Express

#ZBorRdm=ZeroBias-PromptReco
#ZBorRdm=AlCaPCCRandom-PromptReco
ZBorRdm=AlCaPCCRandom-02May2018


##all run files are cleaned out
rm -rf ./$period
mkdir ./$period
rm -f ./$period/*.txt


###################################
##search files for a given stream in a given period
###################################
search(){

    local stream=$1
    local path=$eospath/$stream/ALCARECO

    for v in `${eos} ls ${path} | grep ${ZBorRdm}`; do 
	local pathv=$path/$v
	#echo $pathv

	for vN in `${eos} ls ${pathv}`; do
	    local path1=$pathv/$vN 
	    #echo $path1

	    for r1 in `${eos} ls -d ${path1} | grep -v .`; do 
		local path2=$r1
		for r2 in `${eos} ls -d ${path2}`; do 
		    #echo "$r2" 
		    local path3=$r2/00000
		    for f in `${eos} ls ${path3} | grep .root`; do 
 	            ###files will be added to the run file
			echo "file:$path3/$f" >> ./$period/$r1$r2.txt  
		    done
		done
	    done


	    for f in `${eos} ls ${path1} | grep .root`; do 
		echo "file:$path1/$f" >> ./${period}/${ZBorRdm}.txt  
	    done


	done

    done
}



### loop over the streams
for s in `/bin/ls $eospath | grep AlCaLumiPixels`; do 

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
	search $s

done    


