#!/bin/bash

run=$1 # 324293
normtag=$2 #PHYSICS, hfoc, pcc,..
normtagdir=/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags

command="brilcalc lumi -c web --output-style csv --normtag ${normtagdir}/normtag_${normtag}.json -r ${run} --byls"
echo $command

brilcalc lumi -c web --output-style csv --normtag ${normtagdir}/normtag_${normtag}.json -r ${run} --byls \
| grep ${run}: \
| sed -e 's/,/ /g' \
| sed -e 's/:/ /g' \
>> ${run}_${normtag}.txt
