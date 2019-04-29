#!/bin/bash

run=$1 # 324293
normtag=$2 #PHYSICS, hfoc, pcc,..
normtagdir=/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags
goldenjson=$CMSSW_BASE/src/BRILAnalysisCode/PCCAnalysis/test/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt

command="brilcalc lumi -c web --output-style csv --normtag ${normtagdir}/normtag_${normtag}.json -r ${run} -i ${goldenjson}"
echo $command

/bin/rm -f ${run}_${normtag}.txt

brilcalc lumi -c web --output-style csv --byls \
-r ${run}  \
--normtag ${normtagdir}/normtag_${normtag}.json \
-i ${goldenjson} \
| grep ${run}: \
| sed -e 's/,/ /g' \
| sed -e 's/:/ /g' \
>> ${run}_${normtag}.txt
