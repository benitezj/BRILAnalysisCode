#!/bin/bash

#########################################
## * script to search for files in eos
## * output are files with runnumber.txt with list of root files 
## * this script takes no arguments, need to set options below
#######################################


period=$1 #Run2022A

########
## choose type of data: 

#ZeroBias RawPCCProducer -> /store/data/Run2022A/AlCaLumiPixelsCountsPrompt/ALCARECO/RawPCCProducer-PromptReco-v1/000/352/416/00000
datatype=3
eospath=/eos/cms/store
dataset=RawPCCProducer-PromptReco

#Random PCCIntegrator -> /store/express/Run2022A/StreamALCALumiPixelsCountsExpress/ALCARECO/AlCaPCCRandom-Express-v1/000/352/416/00000/fb9a0530-481e-416e-b621-1d15b74a9720.root
#datatype=4
#eospath=/eos/cms/store
#dataset=AlCaPCCRandom-Express

#Random RawPCC -> /store/express/Run2022A/StreamALCALumiPixelsCountsExpress/ALCAPROMPT/PromptCalibProdLumiPCC-Express-v1/000/352/416/00000/c96bf8ed-4934-4ff9-8560-bf836494a7cb.root
#datatype=5
#eospath=/eos/cms/store
#dataset=PromptCalibProdLumiPCC-Express


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



if [ "$datatype" == "0" ] ; then
    for i in 0 1 2 3 4 5 6 7 8 9 10 11 ; do
	search ${eospath}/data/${period}/AlCaLumiPixels${i}/RAW
    done
fi

if [ "$datatype" == "1" ] ; then
    search ${eospath}/express/${period}/StreamALCALUMIPIXELSEXPRESS/ALCARECO
fi

if [ "$datatype" == "2" ] ; then
    search ${eospath}/data/${period}/AlCaLumiPixels/ALCARECO
fi


if [ "$datatype" == "3" ] ; then
    search ${eospath}/data/${period}/AlCaLumiPixelsCountsPrompt/ALCARECO
fi

if [ "$datatype" == "4" ] ; then
    search ${eospath}/express/${period}/StreamALCALumiPixelsCountsExpress/ALCARECO
fi

if [ "$datatype" == "5" ] ; then
    search ${eospath}/express/${period}/StreamALCALumiPixelsCountsExpress/ALCAPROMPT
fi

