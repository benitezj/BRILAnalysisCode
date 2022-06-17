#!/bin/bash

eospath=/eos/cms/store
#eospath=/eos/cms/tier0/store

#########################################
## * script to search for files in eos
## * output are files with runnumber.txt with list of root files 
## * this script takes no arguments, need to set options below
#######################################


#period=Run2018A
period=Run2022A

########
## choose type of data: 

# many streams -> /eos/cms/store/data/Run2018A/AlCaLumiPixels0/RAW/v1/000/316/524/00000/
#datatype=0
#dataset=v1

#1 -> /eos/cms/store/express/Run2018D/StreamALCALUMIPIXELSEXPRESS/ALCARECO/AlCaPCCRandom-Express-v1/000/320/500/00000/2C216013-EE93-E811-967D-FA163ECC5F9A.root
#datatype=1
#dataset=AlCaPCCRandom-Express 

#2 -> /eos/cms/store/data/Run2018A/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-PromptReco-v2/000/316/239/00000/0CEC4AA3-1259-E811-9384-02163E01A168.root
#datatype=2
#dataset=AlCaPCCZeroBias-PromptReco


#ZB RawPCCProducer -> /store/data/Run2022A/AlCaLumiPixelsCountsPrompt/ALCARECO/RawPCCProducer-PromptReco-v1/000/352/416/00000
#datatype=3
#dataset=RawPCCProducer-PromptReco

#RD PCCIntegrator -> /store/express/Run2022A/StreamALCALumiPixelsCountsExpress/ALCARECO/AlCaPCCRandom-Express-v1/000/352/416/00000/fb9a0530-481e-416e-b621-1d15b74a9720.root
#datatype=4
#dataset=AlCaPCCRandom-Express

#RD RawPCC -> /store/express/Run2022A/StreamALCALumiPixelsCountsExpress/ALCAPROMPT/PromptCalibProdLumiPCC-Express-v1/000/352/416/00000/c96bf8ed-4934-4ff9-8560-bf836494a7cb.root
datatype=5
dataset=PromptCalibProdLumiPCC-Express


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

