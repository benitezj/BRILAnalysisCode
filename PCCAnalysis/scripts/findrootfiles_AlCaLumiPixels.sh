#!/bin/bash

eospath=/eos/cms/store

#########################################
## * script to search for files in eos
## * output are files with runnumber.txt with list of root files 
## * this script takes no arguments, need to set options below
#######################################

period=Run2018A

########
## choose type of data: 
#1 -> /eos/cms/store/express/Run2018D/StreamALCALUMIPIXELSEXPRESS/ALCARECO/AlCaPCCRandom-Express-v1/000/320/500/00000/2C216013-EE93-E811-967D-FA163ECC5F9A.root
datatype=1
dataset=AlCaPCCRandom-Express 


#2 -> /eos/cms/store/data/Run2018A/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-PromptReco-v2/000/316/239/00000/0CEC4AA3-1259-E811-9384-02163E01A168.root
#datatype=2
#dataset=AlCaPCCZeroBias-PromptReco


#3 -> /eos/cms/store/data/Run2018A/AlCaLumiPixels0/RAW/v1/000/316/524/00000/
#datatype=3
#dataset=v1

echo $period
echo $datatype
echo $dataset
######## ##########################################



#################
##clean out old files
rm -rf ./$period
mkdir ./$period
rm -f ./$period/*.txt


###################################
##search files for a given stream in a given period
###################################
search(){
    local path1=$1
    #echo "path1: $path1"
    for tag1 in `/bin/ls ${path1} | grep ${dataset}`; do 
	local path2=$path1/$tag1/000
        for r1 in `/bin/ls ${path2}`; do
	    local path3=$path2/$r1
	    for r2 in `/bin/ls ${path3}`; do 
		local path4=$path3/$r2/00000
		echo "path4: $path4" 
		for f in `/bin/ls ${path4} | grep .root`; do 
		    echo "file:$path4/$f" >> ./$period/$r1$r2.txt  
		done
	    done
	done
    done
}




if [ "$datatype" == "1" ] ; then
    search ${eospath}/express/${period}/StreamALCALUMIPIXELSEXPRESS/ALCARECO
fi

if [ "$datatype" == "2" ] ; then
    search ${eospath}/data/${period}/AlCaLumiPixels/ALCARECO
fi

if [ "$datatype" == "3" ] ; then
    for i in 0 1 2 3 4 5 6 7 8 9 10 11 ; do
	search ${eospath}/data/${period}/AlCaLumiPixels${i}/RAW
    done
fi


