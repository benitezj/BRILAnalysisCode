#!/bin/bash

#/eos/cms/tier0/store/data/Commissioning2021/AlCaLumiPixelsCountsPrompt
#eospath=/eos/cms/store/data/Commissioning2021/AlCaLumiPixelsCountsExpress
eospath=/eos/cms/store/data/Commissioning2021/AlCaLumiPixelsCountsPrompt
#eospath=$1
if [ "$eospath" == "" ]; then
	echo "no run period specified."
	return
fi
echo $eospath


##all run files are cleaned out
rm -f ./*.txt


###################################
##search files for a given stream in a given period
###################################
search(){
    local path=$eospath/RAW/v1/000
    for r1 in `/bin/ls  ${path} `; do
	local path2=$path/$r1
	for r2 in `/bin/ls  ${path2}`; do 
	    local path3=$path2/$r2/00000
	    for f in `/bin/ls  ${path3} | grep .root`; do 
		echo "file:$path3/$f" >> ./$r1$r2.txt  
	    done
	done
    done

}


search 

