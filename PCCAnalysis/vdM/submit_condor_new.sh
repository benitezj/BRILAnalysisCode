#!/bin/bash

## path to submission directory
submitdir=$1 

## option for: 0=create scripts, 1=submit, 2=check
action=$2

## fixed options
type=hd5 #hd5:vdMFW per NB4, csv:per module, per LS

baseoutdir=/eos/user/b/benitezj/BRIL/PCC/VDM

#moduleveto=BRILAnalysisCode/PCCAnalysis/test/vetoModules_2017_fixed.txt
#moduleveto=BRILAnalysisCode/PCCAnalysis/test/vetoModules_2017_fixed_W0_SSBkg5.txt
moduleveto=BRILAnalysisCode/PCCAnalysis/test/vetoModules_2017_fixed_W0_FPIXOnly_SSBkg25.txt

modules=BRILAnalysisCode/PCCAnalysis/plots/modules.txt ## used for type=csv 


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

condorqueue=workday  #microcentury , workday, testmatch
echo "condor queue: $condorqueue"


outputdir=$baseoutdir/$submitdir
echo "output: $outputdir"


#####################################################
## copy the cfg
if [ "$action" == "0" ]; then 
    cfg=$3
    if [ "$cfg" == "" ]; then
	echo "No cfg provided\n"
	exit;
    fi
    /bin/cp $cfg $fullsubmitdir/cfg.py
    echo "mkdir -p $outputdir"
    mkdir -p $outputdir
    /bin/ls $outputdir
fi



##################################################
#### helper functions
submit(){
    local run=$1
    rm -f $fullsubmitdir/${run}.log
    rm -f ${outputdir}/${run}.hd5
    rm -f ${outputdir}/${run}.csv

    condor_submit $fullsubmitdir/${run}.sub 
}

make_sh_script(){
    local run=$1

    rm -f $fullsubmitdir/${run}.sh

    echo "cd ${INSTALLATION} " >> $fullsubmitdir/${run}.sh
    echo "eval \`scramv1 runtime -sh\` " >> $fullsubmitdir/${run}.sh
    echo "cd \$TMPDIR  "   >> $fullsubmitdir/${run}.sh
    echo "pwd  "   >> $fullsubmitdir/${run}.sh
    echo "env" >> $fullsubmitdir/${run}.sh

    if [ "$type" == "hd5" ]; then
	echo "python3  ${fullsubmitdir}/cfg.py --vetoModules=${INSTALLATION}/${moduleveto} --inputfile=${fullsubmitdir}/${run}.txt" >> $fullsubmitdir/${run}.sh
	echo "cp pcc_ZB.hd5  $outputdir/${run}.hd5 " >> $fullsubmitdir/${run}.sh
    fi

    if [ "$type" == "csv" ]; then
	echo "python3  ${fullsubmitdir}/cfg.py --modules=${INSTALLATION}/${modules} --inputfile=${fullsubmitdir}/${run}.txt" >> $fullsubmitdir/${run}.sh
	echo "cp tuples_to_csv_modules.csv  $outputdir/${run}.csv " >> $fullsubmitdir/${run}.sh
    fi
    
}    
    

make_sub_script(){
    local run=$1
    rm -f $fullsubmitdir/${run}.sub

    echo "Universe   = vanilla" >>  $fullsubmitdir/${run}.sub
    echo "+JobFlavour = \"${condorqueue}\" " >> $fullsubmitdir/${run}.sub
#    echo "requirements = (OpSysAndVer =?= \"CentOS7\")" >> $fullsubmitdir/${run}.sub
#    echo "+AccountingGroup = \"group_u_CMS.CAF.COMM\"" >> $fullsubmitdir/${run}.sub  
    echo "Executable = /bin/bash" >> $fullsubmitdir/${run}.sub 
    echo "Arguments  = ${fullsubmitdir}/${run}.sh" >> $fullsubmitdir/${run}.sub 
    echo "Log        = ${fullsubmitdir}/${run}.log" >> $fullsubmitdir/${run}.sub 
    echo "Output     = ${fullsubmitdir}/${run}.log" >> $fullsubmitdir/${run}.sub 
    echo "Error      = ${fullsubmitdir}/${run}.log" >> $fullsubmitdir/${run}.sub 
    echo "Queue  " >> $fullsubmitdir/${run}.sub 
}


check_log(){
    local run=$1
    fail=0
    
    if [ ! -f $fullsubmitdir/${run}.log ]; then
	echo "no log"
	fail=1
    fi

    if [ "$type" == "hd5" ]; then
	if [ ! -f ${outputdir}/${run}.hd5 ]; then
	    echo "no hd5"
	    fail=1
	fi
    fi

    if [ "$type" == "csv" ]; then
	if [ ! -f ${outputdir}/${run}.csv ]; then
	    echo "no csv"
	    fail=1
	fi
    fi

    if [ "$fail" == "0" ]; then
	success=`cat $fullsubmitdir/${run}.log | grep "exit-code 0."`
	if [ "$success" == "" ]; then
	    echo "no Success"
	    fail=1
	fi
    fi
    
    if [ "$fail" == "0" ]; then
	fatal=`cat $fullsubmitdir/${run}.log | grep Fatal`
	if [ "$fatal" != "" ]; then
	    echo "Fatal"
	    fail=1
	fi
    fi
    
}



###################################################################
##### loop over the runs
##################################################################
export RUNLIST=""
counter=0
counter_fail=0
for f in `/bin/ls $fullsubmitdir | grep .txt | grep -v "~" `; do
    run=`echo $f | awk -F".txt" '{print $1}'`
    #echo $run

    ##create the scripts
    if [ "$action" == "0" ]; then
	make_sh_script $run
	make_sub_script $run
    fi

    ##submit to lxbatch
    if [ "$action" == "1" ]; then
	submit $run
    fi

    ##check failed jobs
    if [ "$action" == "2" ] ; then
	check_log $run
	if [ "$fail" == "1" ]; then
	    echo $fullsubmitdir/${run}.log
	    counter_fail=`echo $counter_fail | awk '{print $1+1}'`
	fi   
    fi

    if [ "$action" == "3" ]; then
	check_log $run
	if [ "$fail" == "1" ]; then
	    submit $run
	fi   
    fi


    counter=`echo $counter | awk '{print $1+1}'`
done

echo "Total runs: $counter"
if [ "$action" == "2" ]; then
    echo "Total runs failed: $counter_fail"
fi

#run the merging
if [ "$action" == "4" ]; then
    if [ "$type" == "hd5" ]; then
	python3 ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/vdM/pcchd5_merger.py --inputdir=${outputdir}
    fi
fi

