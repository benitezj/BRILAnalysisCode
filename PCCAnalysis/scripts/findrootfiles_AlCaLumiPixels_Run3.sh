#!/bin/bash

period=$1 #Run2022A
type=$2  #ZB, RD, LUM, RDX

## where is the data located
eospath=/eos/cms/store/data
#eospath=/eos/cms/tier0/store/data/
#eospath=/eos/cms/store/express

########
if [ "$type" == "ZB" ] ; then
    ##/eos/cms/store/data/Run2022C/AlCaLumiPixelsCountsPrompt/ALCARECO/AlCaPCCZeroBias-PromptReco-v1/000/355/862/00000/87fecadb-45ce-4701-bb7f-6489b5689e0a.root
    dataset=AlCaPCCZeroBias-PromptReco
fi

if [ "$type" == "RD" ] ; then
    #/eos/cms/store/data/Run2023A/AlCaLumiPixelsCountsPrompt/ALCARECO/AlCaPCCRandom-PromptReco-v1/000/365/753/00000/39f9fc82-5f50-489a-894c-6e48bd54c31f.root
    dataset=AlCaPCCRandom-PromptReco
fi

if [ "$type" == "LUM" ] ; then
    #/eos/cms/store/data/Run2022C/AlCaLumiPixelsCountsPrompt/ALCARECO/RawPCCProducer-PromptReco-v1
    dataset=RawPCCProducer-PromptReco
fi

if [ "$type" == "RDX" ] ; then
    #/eos/cms/store/express/Run2022A/StreamALCALumiPixelsCountsExpress/ALCARECO/AlCaPCCRandom-Express-v1/000/352/416/00000/fb9a0530-481e-416e-b621-1d15b74a9720.root
    eospath=/eos/cms/store/express
    dataset=AlCaPCCRandom-Express
fi


echo $period
echo $dataset



#################
##create directory for output
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


