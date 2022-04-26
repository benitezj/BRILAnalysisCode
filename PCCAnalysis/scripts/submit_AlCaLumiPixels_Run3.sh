#!/bin/bash

submitdir=$1 ## path to submission directory
action=$2 ## option for: 0=create scripts, 1=submit, 2=check
cfg=$3  ## only for action=0
jobtype=step5 ##step2, step3, step4, step5 , csv


baseoutdir=/eos/user/b/benitezj/BRIL/PCC_Run3
condorqueue=workday  #microcentury , workday, testmatch

#DBDIR=/eos/user/b/benitezj/BRIL/PCC_Run3/Commissioning2021_v2/AlCaLumiPixelsCountsExpress/step4/

###########################################################
### 
if [ "$submitdir" == "" ]; then
    echo "invalid submitdir"
    exit
fi
fullsubmitdir=`readlink -f $submitdir`
echo "fullsubmitdir: $fullsubmitdir"

INSTALLATION=${CMSSW_BASE}/src
if [ "$INSTALLATION" == "/src" ]; then
    echo "invalid INSTALLATION"
    exit
fi
echo "INSTALLATION: $INSTALLATION"

echo "condor queue: $condorqueue"

### full path to output directory
outputdir=$baseoutdir/$submitdir
echo "output: $outputdir"


##############################################################
## directory containing the Afterglow corrections if computed privately
if [ "$DBDIR" != "" ] || [ "$DBROOTDIR" != "" ]; then
   echo "corections: $DBDIR $DBROOTDIR"
fi


#####################################################
## copy the cfg
if [ "$action" == "0" ]; then 
    if [ "$cfg" == "" ]; then
	echo "No cfg provided\n"
	exit;
    fi
    /bin/cp $cfg $fullsubmitdir/cfg.py
    echo "mkdir -p $outputdir"
    mkdir -p $outputdir
    /bin/ls $outputdir
fi

## clean up the runs file
if [ "$action" == "5" ] ; then
    rm -f $fullsubmitdir/runs.dat
    rm -f $fullsubmitdir/ls.dat
    rm -f $fullsubmitdir/slope.dat
    rm -rf $plotsdir
    mkdir -p $plotsdir
fi


##################################################
#### helper functions
submit(){
    local run=$1
    rm -f $fullsubmitdir/${run}.log
    rm -f ${outputdir}/${run}.root
    rm -f ${outputdir}/${run}.db
    rm -f ${outputdir}/${run}.txt
    rm -f ${outputdir}/${run}.csv
    rm -f ${outputdir}/${run}.mod
    rm -f ${outputdir}/${run}.frac

    #bsub -q 1nd -o $fullsubmitdir/${run}.log -J $run < $fullsubmitdir/${run}.sh    
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

    ###if DBDIR is set this will override the Afterglow corrections 
    if [ "$DBDIR" != "" ]; then
	echo "export DBFILE=${DBDIR}/${run}.db" >> $fullsubmitdir/${run}.sh
    fi
    if [ "$DBROOTDIR" != "" ]; then
	echo "export DBROOT=${DBROOTDIR}/${run}.root" >> $fullsubmitdir/${run}.sh
    fi
    
    ###if run.json file is there, this will apply a json when processing  
    if [ -f ${fullsubmitdir}/${run}.json ]; then
	echo "export JSONFILE=${fullsubmitdir}/${run}.json" >> $fullsubmitdir/${run}.sh
    fi
    
    ##print all environment into log file right before executing
    echo "env" >> $fullsubmitdir/${run}.sh

    ### cmsRun 
    echo "cmsRun  ${fullsubmitdir}/cfg.py" >> $fullsubmitdir/${run}.sh

    ###product that should be saved to output
    if [ "$jobtype" == "step2" ] ; then
	echo "cp step2_ALCAPRODUCER.root $outputdir/${run}.root " >> $fullsubmitdir/${run}.sh
    fi

    if [ "$jobtype" == "step3" ] ; then
	echo "cp PromptCalibProdLumiPCC.root $outputdir/${run}.root " >> $fullsubmitdir/${run}.sh
    fi
    
    if [ "$jobtype" == "step4" ] ; then
	echo "cp promptCalibConditions.db $outputdir/${run}.db " >> $fullsubmitdir/${run}.sh
	echo "cp CorrectionHisto.root $outputdir/${run}.root " >> $fullsubmitdir/${run}.sh
    fi

    if [ "$jobtype" == "step5" ] ; then
	echo "cp step5_ALCAPRODUCER.root  $outputdir/${run}.root " >> $fullsubmitdir/${run}.sh
    fi

    if [ "$jobtype" == "csv" ] ; then
	echo "cp rawPCC.csv  $outputdir/${run}.csv " >> $fullsubmitdir/${run}.sh
    fi
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
    
    if [ "$fail" == "0" ]; then
	success=`cat $fullsubmitdir/${run}.log | grep "Normal termination"`
	if [ "$success" == "" ]; then
	    echo "no Success"
	    fail=1
	fi
    fi
    
#    if [ "$fail" == "0" ]; then
#	fatal=`cat $fullsubmitdir/${run}.log | grep Fatal`
#	if [ "$fatal" != "" ]; then
#	    echo "Fatal"
#	    fail=1
#	fi
#    fi
    
    
    ### error check for lumi jobs	
#    if [ "$jobtype" == "lumi" ] && [ "$fail" == "0" ]; then
#	if [  ! -f  $outputdir/${run}.csv ]; then
#	    echo "no csv"
#	    fail=1
#	fi
#    fi
    
    
    ### error check for corrections jobs
    if [ "$jobtype" == "step4" ] && [ "$fail" == "0" ] ; then
	if [ ! -f $outputdir/${run}.db ]; then
	    echo "no db"
	    fail=1
	fi
    fi
    
}



###################################################################
##### loop over the runs
##################################################################
export RUNLIST=""
counter=0
for f in `/bin/ls $fullsubmitdir | grep .txt | grep -v "~" `; do
    run=`echo $f | awk -F".txt" '{print $1}'`
    echo $run

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