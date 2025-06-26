#!/bin/bash

submitdir=$1 ## path to submission directory
action=$2 ## option for: 0=create scripts, 1=submit, 2=check, ..
cfg=$3  ## only for action=0

##output paths
baseoutdir=/eos/user/b/benitezj/BRIL/PCC
plotsdir=/eos/user/b/benitezj/www/plots/BRIL/PCC_lumi/$submitdir

##########################
####### Options
jobtype=ZB ##RD (Randoms), ZB (ZeroBias) 

condorqueue=workday  #microcentury , workday, testmatch,  local (lxplus jobs in series, not condor), 

CAF=0 # Use the T0 cluster : https://batchdocs.web.cern.ch/local/specifics/CMS_CAF_tzero.html  , need to:  module load lxbatch/tzero

## afterglow corrections
DBFILE=
#DBFILE=/eos/user/b/benitezj/BRIL/PCC_Run3/Reprocess2023/Random/Run2023D
#DBFILE=/eos/user/b/benitezj/BRIL/PCC/28Aug24_UL2017_PCCZeroBias/Random_v8/Run2017F/merged.db
#DBFILE=/eos/user/b/benitezj/BRIL/PCC/28Aug24_UL2017_PCCZeroBias_vetoModules_2017_fixed_W0_FPIXOnly_SSBkg25/Random_v2/Run2017F/merged.db
#DBFILE=/eos/user/b/benitezj/BRIL/PCC/28Aug24_UL2017_PCCZeroBias_vetoModules_2017_fixed_W0_FPIXD05_Bkg25/Random/Run2017H/merged.db
DBFILE=/eos/user/b/benitezj/BRIL/PCC/28Aug24_UL2017_PCCZeroBias_vetoModules_2017_fixed_W0_FPIXD05_Bkg25_lowPU_W0_Stab0p02/Random_v2/Run2017H/merged.db

## options for brilcalc lumi
#brilcalc='/usr/bin/singularity -s exec  --env PYTHONPATH=/home/bril/.local/lib/python3.10/site-packages /cvmfs/unpacked.cern.ch/gitlab-registry.cern.ch/cms-cloud/brilws-docker:latest brilcalc lumi -u hz/ub --byls --output-style csv -c offline'
brilcalc='/usr/bin/singularity -s exec  --env PYTHONPATH=/home/bril/.local/lib/python3.10/site-packages /cvmfs/unpacked.cern.ch/gitlab-registry.cern.ch/cms-cloud/brilws-docker:latest brilcalc lumi -u hz/ub --xing --output-style csv -c offline'
#brilcalc='brilcalc lumi -u hz/ub --byls --output-style csv -c offline'
BRILCALCDATATAG=
#BRILCALCDATATAG=online 
#BRILCALCDATATAG=23v1

BRILCALCTYPE=
BRILCALCTYPE=hfet
#BRILCALCTYPE=pxl

BRILCALCNORM=
#BRILCALCNORM=/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_BRIL.json
#BRILCALCNORM=/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfet.json
BRILCALCNORM=hfet17New_v5
#BRILCALCNORM=pcc17_run2_legacy_v6_pccvtx
#BRILCALCNORM=NoCorr

BRILCALCREFDETNAME=
BRILCALCREFDETNAME=$BRILCALCNORM


BCIDS=
#BCIDS="282,822,1881,2453,2474,2579,2632,2653,2716,2758,2944,3123,3302"



###########################################################
### test valid params
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

#########################
##Print job config
echo "output: $outputdir"
echo "condor queue: $condorqueue"
echo "job type: $jobtype"
echo "plots dir: $plotsdir"
if [ "$jobtype" == "ZB" ]; then
    echo "afterglow corrections: $DBFILE $DBROOTDIR"
fi


#####################################################
## copy the cfg
if [ "$action" == "0" ]; then 
    if [ "$cfg" == "" ]; then
	echo "No cfg provided\n"
	return 0
    fi
    /bin/cp $cfg $fullsubmitdir/cfg.py
    echo "mkdir -p $outputdir"
    mkdir -p $outputdir
    /bin/ls $outputdir
fi

submit(){
    local run=$1
    rm -f ${fullsubmitdir}/${run}.log
    rm -f ${outputdir}/${run}.root
    rm -f ${outputdir}/${run}.db
    rm -f ${outputdir}/${run}.txt
    rm -f ${outputdir}/${run}.csv
    rm -f ${outputdir}/${run}.mod
    rm -f ${outputdir}/${run}.frac

    #bsub -q 1nd -o $fullsubmitdir/${run}.log -J $run < $fullsubmitdir/${run}.sh    
    if [ "$condorqueue" == "local" ] ; then
	source $fullsubmitdir/${run}.sh
    else
	condor_submit $fullsubmitdir/${run}.sub 
    fi
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

    ## for ReReco samples need specify the run number for the CorrPCCProducerReReco
    if [ "$jobtype" == "RD" ] ; then
	echo "export RUNNUMBER=$run" >> $fullsubmitdir/${run}.sh
    fi
    
    ###if DBFILE is set this will override the Afterglow corrections 
    if [ "$jobtype" == "ZB" ] ; then
	if [ "$DBFILE" != "" ]; then
	    echo "export DBFILE=${DBFILE}" >> $fullsubmitdir/${run}.sh
	fi
    fi
    
    ##print all environment into log file right before executing
    echo "env" >> $fullsubmitdir/${run}.sh

    ### cmsRun 
    echo "cmsRun  ${fullsubmitdir}/cfg.py" >> $fullsubmitdir/${run}.sh

    ## copy output
    if [ "$jobtype" == "ZB" ] ; then
	echo "cp rawPCC.csv  $outputdir/${run}.csv " >> $fullsubmitdir/${run}.sh
    fi

    if [ "$jobtype" == "RD" ] ; then
        echo "cp PCC_Corr.db  $outputdir/${run}.db " >> $fullsubmitdir/${run}.sh
        echo "cp CorrectionHisto.root  $outputdir/${run}.root " >> $fullsubmitdir/${run}.sh
    fi

}    
    

make_sub_script(){
    local run=$1
    rm -f $fullsubmitdir/${run}.sub

    echo "Universe   = vanilla" >>  $fullsubmitdir/${run}.sub
    echo "+JobFlavour = \"${condorqueue}\" " >> $fullsubmitdir/${run}.sub
    if [ $CAF == 1 ] ; then echo "+AccountingGroup = \"group_u_CMS.CAF.COMM\"" >> $fullsubmitdir/${run}.sub ; fi
    echo "Executable = /bin/bash" >> $fullsubmitdir/${run}.sub 
    echo "Arguments  = ${fullsubmitdir}/${run}.sh" >> $fullsubmitdir/${run}.sub 
    echo "Log        = ${fullsubmitdir}/${run}.log" >> $fullsubmitdir/${run}.sub 
    echo "Output     = ${fullsubmitdir}/${run}.log" >> $fullsubmitdir/${run}.sub 
    echo "Error      = ${fullsubmitdir}/${run}.log" >> $fullsubmitdir/${run}.sub 
    echo "Queue  " >> $fullsubmitdir/${run}.sub 

}


check_log(){

    if [ ! -f $fullsubmitdir/${run}.log ]; then
	echo "no log"
	fail=1
    fi
    
    ### error check for corrections jobs
    if [ "$jobtype" == "ZB" ] && [ "$fail" == "0" ] ; then
	if [ ! -f $outputdir/${run}.csv ]; then
	    echo "no csv"
	    fail=1
	fi
    fi

    ### error check for corrections jobs
    if [ "$jobtype" == "RD" ] && [ "$fail" == "0" ] ; then
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
counterbad=0
for f in `/bin/ls $fullsubmitdir | grep .txt | grep -v "~" `; do

    run=`echo $f | awk -F".txt" '{print $1}'`
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
    fail=0
    if [ "$action" == "2" ] ; then
	check_log
	if [ "$fail" == "1" ]; then
	    echo $fullsubmitdir/${run}.log
	    #cat $fullsubmitdir/${run}.log | grep sqlite_file
	    counterbad=`echo $counterbad | awk '{print $1+1}'`
	fi   
    fi

    if [ "$action" == "3" ]; then
	check_log
	if [ "$fail" == "1" ]; then
	    submit $run
	fi   
    fi


    ##get input lumi data for comparisons
    if [ "$action" == "4" ] ; then
	command="${brilcalc} -r ${run}"
	if [ ! -z "$BRILCALCDATATAG" ] ; then
	    command="${command} --datatag ${BRILCALCDATATAG}"
	fi
	if [ ! -z "$BRILCALCTYPE" ] ; then
	    command="${command} --type ${BRILCALCTYPE}"
	fi
	if [ ! -z "$BRILCALCNORM" ] ; then
	    if [ "$BRILCALCNORM" == "NoCorr" ] ; then
		command="${command} --without-correction"
	    else
		command="${command} --normtag ${BRILCALCNORM}"
	    fi
	fi
	if [ ! -z "$BCIDS" ] ; then
	    command="${command} --xing --xingId ${BCIDS}"
	fi
        echo $command
        #${command} | grep ${run}: | sed -e 's/,/ /g' | sed -e 's/:/ /g' | sed -e 's/\[//g'  | sed -e 's/\]//g' > $outputdir/${run}.ref
	#${command} | grep ${run}:  > $outputdir/${run}.ref
	${command} | grep ${run}:  > $outputdir/${run}.${BRILCALCREFDETNAME}

    fi 

    counter=`echo $counter | awk '{print $1+1}'`
done


if [ "$action" == "2" ] ; then
    echo "Total bad runs: $counterbad"
fi

echo "Total runs: $counter"
#echo ${RUNLIST:1}


mkdir -p $plotsdir
if [ "$action" == "5" ] ; then
    #rm -f $plotsdir/*
    root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C  ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotCSVList.C\(\"${outputdir}\",\"${plotsdir}\",\"${RUNLIST:1}\",\"${BRILCALCREFDETNAME}\"\)
fi

if [ "$action" == "6" ] ; then
    NLS=`cat ${plotsdir}/ls.dat | wc -l`
    root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCStability.C\(\"${plotsdir}\",0,${NLS},\"${BRILCALCREFDETNAME}\"\)
    #root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCruns.C\(\"${plotsdir}\"\)
fi

if [ "$action" == "7" ] ; then
    root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plot_afterglow_residual.C\(\"${outputdir}\",\"${plotsdir}\",\"${RUNLIST:1}\"\)
fi


if [ "$action" == "71" ] ; then
    ## this was needed only for the ReReco samples because the normal Corr module didn't work
    root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/test_afterglow_blockLSranges.C\(\"${outputdir}\",\"${RUNLIST:1}\"\)
fi
