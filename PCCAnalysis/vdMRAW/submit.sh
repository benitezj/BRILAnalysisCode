#!/bin/bash

submitdir=$1 ## path to submission directory
action=$2 ## option for: 0=create scripts, 1=submit, 2=check, ..
cfg=$3  ## only for action=0

condorqueue=workday  #microcentury , workday, testmatch


baseoutdir=/eos/user/o/oangulog/Analysis2025 ##outpath for
#plotsdir=/eos/user/o/oangulog/Lumi2025 


###########################################################
### 
if [ "$submitdir" == "" ]; then
    echo "invalid submitdir"
    return 0
fi
fullsubmitdir=`readlink -f $submitdir`
echo "fullsubmitdir: $fullsubmitdir"

INSTALLATION=${CMSSW_BASE}/src
if [ "$INSTALLATION" == "/src" ]; then
    echo "invalid INSTALLATION"
    return 0
fi
echo "INSTALLATION: $INSTALLATION"

outputdir=$baseoutdir/$submitdir
echo "output: $outputdir"
echo "condor queue: $condorqueue"
#echo "plots dir: $plotsdir"


#####################################################
## copy the cfg
if [ "$action" == "0" ]; then 
    if [ "$cfg" == "" ]; then
	echo "No cfg provided\n"
	return 0
    fi
    /bin/cp $cfg $fullsubmitdir/run.sh

    echo "mkdir -p $outputdir"
    mkdir -p $outputdir
    #mkdir -p $plotsdir
fi


##################################################
#### helper functions
submit(){
    local run=$1
    rm -f $fullsubmitdir/${run}.log
    rm -f ${outputdir}/${run}.root
    rm -f ${outputdir}/${run}.hd5
    #rm -f ${plotsdir}/${run}.png
    condor_submit $fullsubmitdir/${run}.sub 
}

make_sh_script(){
    local run=$1

    rm -f $fullsubmitdir/${run}.sh

    echo "export X509_USER_PROXY=${HOME}/x509up_u55361 " >> $fullsubmitdir/${run}.sh
    echo "cd ${INSTALLATION} " >> $fullsubmitdir/${run}.sh
    echo "eval \`scramv1 runtime -sh\` " >> $fullsubmitdir/${run}.sh
    echo "cd \$TMPDIR  "   >> $fullsubmitdir/${run}.sh
    echo "pwd  "   >> $fullsubmitdir/${run}.sh

    echo "export INPUTFILE=${fullsubmitdir}/${run}.txt" >> $fullsubmitdir/${run}.sh
    
    ##print all environment into log file right before executing
    echo "env" >> $fullsubmitdir/${run}.sh

    ### cmsRun 
    echo "source  ${fullsubmitdir}/run.sh" >> $fullsubmitdir/${run}.sh
    echo "cp pcctuple.root  $outputdir/${run}.root " >> $fullsubmitdir/${run}.sh
    echo "cp pcc_ZB.hd5  $outputdir/${run}.hd5 " >> $fullsubmitdir/${run}.sh
    
    #echo "cp pcchd5_reader_emmittance.png  $plotsdir/${run}.png " >> $fullsubmitdir/${run}.sh
}    
    

make_sub_script(){
    local run=$1
    rm -f $fullsubmitdir/${run}.sub

    echo "Universe   = vanilla" >>  $fullsubmitdir/${run}.sub
    echo "+JobFlavour = \"${condorqueue}\" " >> $fullsubmitdir/${run}.sub
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
    
    ### error check for lumi jobs	
    if [  ! -f  $outputdir/${run}.root ]; then
	echo "no tuple"
	fail=1
    fi

    ### error check for lumi jobs	
    if [  ! -f  $outputdir/${run}.hd5 ]; then
	echo "no hd5"
	fail=1
    fi

    
}



###################################################################
##### loop over the runs
##################################################################
export RUNLIST=""
counter=0
counterbad=0
for f in `/bin/ls $fullsubmitdir | grep .txt | grep -v "~" `; do ##Check here for path and stuff

    run=`echo $f | awk -F".txt" '{print $1}'`
    #echo $run
    RUNLIST=$RUNLIST,$run

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
	    cat $fullsubmitdir/${run}.log | grep sqlite_file
	    counterbad=`echo $counterbad | awk '{print $1+1}'`
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
echo "Total bad runs: $counterbad"

echo ${RUNLIST:1}

