#!/bin/bash

submitdir=$1 
action=$2

wwwdir=/afs/cern.ch/user/b/benitezj/www/BRIL/PCC_lumi
normtagdir=/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags
ref="" #hfoc
baseoutdir=/eos/user/b/benitezj/BRIL/PCC_Run3
datadir=$baseoutdir/$submitdir
plotsdir=${wwwdir}/$submitdir
###########################################################
### 
if [ "$submitdir" == "" ]; then
    echo "invalid submitdir"
    return
fi
fullsubmitdir=`readlink -f $submitdir`
echo "fullsubmitdir: $fullsubmitdir"

INSTALLATION=${CMSSW_BASE}/src
if [ "$INSTALLATION" == "/src" ]; then
    echo "invalid INSTALLATION"
    return
fi
echo "INSTALLATION: $INSTALLATION"


echo "plotsdir: $plotsdir"

echo "datadir: $datadir"

#goldenjson="-i ~/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt"
echo "golden json: $goldenjson"

#######################################
## clean up the runs file
if [ "$action" == "1" ] ; then
    rm -f $fullsubmitdir/runs.dat
    rm -f $fullsubmitdir/ls.dat
    rm -f $fullsubmitdir/slope.dat
    rm -rf $plotsdir
    mkdir -p $plotsdir
fi

###################################################################
##### loop over the runs
##################################################################
export RUNLIST=""
counter=0
for f in `/bin/ls $fullsubmitdir | grep .txt | grep -v "~" `; do
    run=`echo $f | awk -F".txt" '{print $1}'`
    echo $run

    ##get ref data from brilcalc
    if [ "$action" == "0" ] && [ "$ref" != "" ] ; then
	command="brilcalc lumi -u hz/ub -r $run --byls  --output-style csv --normtag ${normtagdir}/normtag_${ref}.json "
	echo $command
	#${command} $goldenjson | grep ${run}: | sed -e 's/,/ /g' | sed -e 's/:/ /g' | sed -e 's/\[//g'  | sed -e 's/\]//g' > $datadir/${run}.$ref
    fi

    ## plot the lumi vs LS 
    if [ "$action" == "1" ] ; then
	root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCcsv.C\(\"${datadir}\",${run},\"${plotsdir}\",\"${ref}\",0\)
    fi


    ### plot Afterglow corrections (From RANDOM stream step 4)
    if [ "$action" == "3" ] && [ -f  $datadir/${run}.root ]; then
	root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/compareRandomRawCorr.C\(\"${datadir}\",${run},\"${plotsdir}\"\)
    fi


    RUNLIST=$RUNLIST,$run
    counter=`echo $counter | awk '{print $1+1}'`
done
echo "Total runs: $counter"


## plots for entire period
if [ "$action" == "2" ] ; then
    root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCStability.C\(\"${plotsdir}\",\"${ref}\"\)
    #root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCruns.C\(\"${plotsdir}\",\"${ref}\"\)
    #root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotModuleFrac.C\(\"${datadir}\",\"${RUNLIST}\",\"${plotsdir}\"\)
fi
