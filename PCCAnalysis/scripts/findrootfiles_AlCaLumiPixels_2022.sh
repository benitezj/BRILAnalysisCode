#!/bin/bash

#########################################
## * script to search for files in eos
## * output are files with runnumber.txt with list of root files 
## * this script takes no arguments, need to set options below
#######################################


period=$1 #Run2022A

eospath=/eos/cms/store/data
#eospath=/eos/cms/tier0/store/data/

########
## choose type of data: 

#ZerboBias Integrator:  
##/eos/cms/tier0/store/data/Run2022B/AlCaLumiPixelsCountsPrompt/ALCARECO/AlCaPCCZeroBias-PromptReco-v1/000/
#eospath=/eos/cms/tier0/store/data
##/eos/cms/store/data/Run2022C/AlCaLumiPixelsCountsPrompt/ALCARECO/AlCaPCCZeroBias-PromptReco-v1/000/355/862/00000/87fecadb-45ce-4701-bb7f-6489b5689e0a.root
dataset=AlCaPCCZeroBias-PromptReco


#Integrator Random
#DAS: /AlCaLumiPixelsCountsPrompt/Run2023A-AlCaPCCRandom-PromptReco-v1/ALCARECO
#EOS: /store/data/Run2023A/AlCaLumiPixelsCountsPrompt/ALCARECO/AlCaPCCRandom-PromptReco-v1/000/365/753/00000/39f9fc82-5f50-489a-894c-6e48bd54c31f.root
#     /store/data/Run2024A/AlCaLumiPixelsCountsPrompt/ALCARECO/AlCaPCCRandom-PromptReco-v1/000/378/238/00000/
#dataset=AlCaPCCRandom-PromptReco


#ZeroBias RawPCCProducer: 
#/eos/cms/tier0/store/data/Run2022B/AlCaLumiPixelsCountsPrompt/ALCARECO/RawPCCProducer-PromptReco-v1/
#eospath=/eos/cms/tier0/store/data
#/eos/cms/store/data/Run2022C/AlCaLumiPixelsCountsPrompt/ALCARECO/RawPCCProducer-PromptReco-v1
#eospath=/eos/cms/store/data
#dataset=RawPCCProducer-PromptReco

#Random PCCIntegrator  
#/store/express/Run2022A/StreamALCALumiPixelsCountsExpress/ALCARECO/AlCaPCCRandom-Express-v1/000/352/416/00000/fb9a0530-481e-416e-b621-1d15b74a9720.root
#/store/express/Run2022F/StreamALCALumiPixelsCountsExpress/ALCARECO/AlCaPCCRandom-Express-v1/000/*/*/*/*.root
#eospath=/eos/cms/store/express
#dataset=AlCaPCCRandom-Express

#Random RawPCC -> /store/express/Run2022A/StreamALCALumiPixelsCountsExpress/ALCAPROMPT/PromptCalibProdLumiPCC-Express-v1/000/352/416/00000/c96bf8ed-4934-4ff9-8560-bf836494a7cb.root
#eospath=/eos/cms/store/express
#dataset=PromptCalibProdLumiPCC-Express

echo $period
echo $dataset
######## #############################################################################
### END OF OPTIONS
########################################################################################




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



#if [ "$datatype" == "0" ] ; then
#    for i in 0 1 2 3 4 5 6 7 8 9 10 11 ; do
#	search ${eospath}/data/${period}/AlCaLumiPixels${i}/RAW
#    done
#fi
#
#if [ "$datatype" == "1" ] ; then
#    search ${eospath}/express/${period}/StreamALCALUMIPIXELSEXPRESS/ALCARECO
#fi
#
#if [ "$datatype" == "2" ] ; then
#    search ${eospath}/data/${period}/AlCaLumiPixels/ALCARECO
#fi


if [ "$dataset" == "AlCaPCCZeroBias-PromptReco" ] ; then
    search ${eospath}/${period}/AlCaLumiPixelsCountsPrompt/ALCARECO
fi
if [ "$dataset" == "AlCaPCCRandom-PromptReco" ] ; then
    search ${eospath}/${period}/AlCaLumiPixelsCountsPrompt/ALCARECO
fi

if [ "$dataset" == "RawPCCProducer-PromptReco" ] ; then
    search ${eospath}/${period}/AlCaLumiPixelsCountsPrompt/ALCARECO
fi

if [ "$dataset" == "AlCaPCCRandom-Express" ] ; then
    search ${eospath}/${period}/StreamALCALumiPixelsCountsExpress/ALCARECO
fi

if [ "$dataset" == "PromptCalibProdLumiPCC-Express" ] ; then
    search ${eospath}/${period}/StreamALCALumiPixelsCountsExpress/ALCAPROMPT
fi

