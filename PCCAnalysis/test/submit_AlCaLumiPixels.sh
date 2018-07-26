#!/bin/bash

inputdir=$1
if [ "$inputdir" == "" ]; then
echo "invalid inputdir"
return
fi
#get the absolute path
inputdir=`readlink -f $inputdir`

##what to do : create scripts, submit, check
action=$2

## only a few runs
TEST=0


## aliases
eos=/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select


###### where is the code:
INSTALLATION=/afs/cern.ch/user/b/benitezj/scratch0/BRIL_PCC/CMSSW_10_1_0/src

### which script to run
script=lumi_alcaZB_cfg.py
#script=lumi_alcaZB_noCorr_cfg.py

####define output directory
#eosoutdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaLumiPixels_ZB
eosoutdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/ZeroBias
#eosoutdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/ZeroBias_noCorr

###batch submit
submit(){
    local run=$1

    rm -f $inputdir/${run}.log
    $eos rm $eosoutdir/${run}.root
    $eos rm $eosoutdir/${run}.csv
    bsub -q 1nd -o $inputdir/${run}.log -J $run < $inputdir/${run}.sh    
}



##### loop over the runs
counter=0
for f in `/bin/ls $inputdir | grep .txt `; do
    if [ "$TEST" == "1" ] && [ "$counter" == "5" ]; then break; fi

    run=`echo $f | awk -F".txt" '{print $1}'`
    echo $run
    

    ###create the scripts
    if [ "$action" == "0" ]; then
	rm -f $inputdir/${run}.sh
	echo "cd ${INSTALLATION} " >> $inputdir/${run}.sh
	echo "eval \`scramv1 runtime -sh\` " >> $inputdir/${run}.sh
	echo "mkdir /tmp/benitezj " >> $inputdir/${run}.sh
	echo "cd /tmp/benitezj " >> $inputdir/${run}.sh
	echo "export INPUTFILE=${inputdir}/${run}.txt" >> $inputdir/${run}.sh
	echo "cmsRun ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/test/${script} " >> $inputdir/${run}.sh
	echo "${eos} cp PCC_ZB.root $eosoutdir/${run}.root " >> $inputdir/${run}.sh
	echo "${eos} cp rawPCC.csv  $eosoutdir/${run}.csv " >> $inputdir/${run}.sh
    fi
    

    ##submit to lxbatch
    if [ "$action" == "1" ]; then
	submit $run
    fi


    ####check failed jobs
    if [ "$action" == "2" ] || [ "$action" == "3" ]; then
	fail=0
	
	if [ ! -f $inputdir/${run}.log ]; then
	    echo "no log"
	    fail=1
	fi
	
	success=`cat $inputdir/${run}.log | grep "Successfully completed."`
	if [ "$success" == "" ]; then
	    echo "no Success"
	    fail=1
	fi
	 
	rootf=`$eos ls $eosoutdir/${run}.root | grep err `
	if [ "$rootf" != "" ]; then
	    echo "no root"
	    fail=1
	fi
	

	if [ "$fail" == "1" ]; then
	    echo $inputdir/${run}.log
	    if [ "$action" == "3" ]; then
		submit $run
	    fi
	fi

    fi


    counter=`echo $counter | awk '{print $1+1}'`
done

echo "Total runs: $counter"

