#!/bin/bash

#########################################
## * script to search for files in eos
## * output are files with runnumber.txt with list of root files 
## * this script takes no arguments, need to set options below
#######################################

#example: /eos/cms/tier0/store/data/Run2025C/AlCaLumiPixelsCountsPrompt/RAW/v1/000/392/538/00000/

period=$1 # Run2025C
eospath=/eos/cms/tier0/store/data
echo $period

#################
##clean out old files
#rm -rf ./$period
mkdir ./$period
rm -f ./$period/*.txt


###################################
##search files for a given stream in a given period
###################################

path1=${eospath}/${period}/AlCaLumiPixelsCountsPrompt/RAW/v1/000
echo "path1: $path1"
for r1 in `/bin/ls ${path1}`; do
    path2=$path1/$r1
    for r2 in `/bin/ls ${path2}`; do 
	path3=$path2/$r2/00000
	#echo "path3: $path3" 
	for f in `/bin/ls ${path3} | grep .root`; do 
	    echo "file:$path3/$f" >> ./$period/$r1$r2.txt  
	done
    done
done

/bin/ls ./$period/
