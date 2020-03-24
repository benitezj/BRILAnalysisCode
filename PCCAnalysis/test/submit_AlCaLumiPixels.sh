#!/bin/bash

## path to submission directory
submitdir=$1 

## option for: 0=create scripts, 1=submit, 2=check
action=$2
## arg 3 needed for action 0 below

##corr: afterglow corrections with Random triggers,
##lumi: ZeroBias triggers luminosity 
jobtype=lumi 


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

normtagdir=/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags
ref=hfoc
echo "reference: $ref"
#goldenjson="-i ~/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt"
echo "golden json: $goldenjson"
plotsdir=/afs/cern.ch/user/b/benitezj/www/BRIL/PCC_lumi/$submitdir
echo "plotsdir: $plotsdir"



### full path to output directory
baseoutdir=""
if [ "$jobtype" == "lumi" ]; then
baseoutdir=/eos/user/b/benitezj/BRIL/PCC/ZeroBias
fi
if [ $jobtype == "corr" ]; then
baseoutdir=/eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom
fi
outputdir=$baseoutdir/$submitdir
echo "output: $outputdir"


##############################################################
## directory containing the Afterglow corrections if computed privately
#DBDIR=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels011_AlCaPCCRandom_Nov22/Commissioning2018
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-Express/Run2018E
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-Express/Run2018E_10LS_F3P2
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-PromptReco/Run2017G
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-PromptReco/Run2017G_v4
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-17Nov2017/Run2017G_v4
#DBDIR=/eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom/Run2018B_dynamicVeto
#DBDIR=/eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-PromptReco/Run2017G_v4
#DBDIR=/eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-17Nov2017/Run2017G_v4
#DBROOTDIR=/eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-PromptReco/Run2017G_v4
#DBROOTDIR=/eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-17Nov2017/Run2017G_v4
if [ "$DBDIR" != "" ] || [ "$DBROOTDIR" != "" ]; then
   echo "corections: $DBDIR $DBROOTDIR"
fi



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

    ###if DBDIR is set this will override the Afterglow corrections 
    if [ "$DBDIR" != "" ]; then
	echo "export DBFILE=${DBDIR}/${run}.db" >> $fullsubmitdir/${run}.sh
    fi
    if [ "$DBROOTDIR" != "" ]; then
	echo "export DBROOT=${DBROOTDIR}/${run}.root" >> $fullsubmitdir/${run}.sh
    fi

    echo "export INPUTFILE=${fullsubmitdir}/${run}.txt" >> $fullsubmitdir/${run}.sh
    
    ###if run.json file is there, this will apply a json when processing  
    if [ -f ${fullsubmitdir}/${run}.json ]; then
	echo "export JSONFILE=${fullsubmitdir}/${run}.json" >> $fullsubmitdir/${run}.sh
    fi
    
    echo "env" >> $fullsubmitdir/${run}.sh
    
    echo "cmsRun  ${fullsubmitdir}/cfg.py" >> $fullsubmitdir/${run}.sh
    
    if [ "$jobtype" == "corr" ] ; then
	echo "cp PCC_Corr.db $outputdir/${run}.db " >> $fullsubmitdir/${run}.sh
	echo "cp CorrectionHisto.root $outputdir/${run}.root " >> $fullsubmitdir/${run}.sh
    fi
    
    if [ "$jobtype" == "lumi" ] ; then
	echo "cp rawPCC.csv  $outputdir/${run}.csv " >> $fullsubmitdir/${run}.sh
	echo "cp rawPCC.err  $outputdir/${run}.err " >> $fullsubmitdir/${run}.sh
	echo "cp moduleFraction.csv  $outputdir/${run}.frac " >> $fullsubmitdir/${run}.sh
	echo "cp modCount.txt  $outputdir/${run}.mod " >> $fullsubmitdir/${run}.sh
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
    
    if [ "$fail" == "0" ]; then
	fatal=`cat $fullsubmitdir/${run}.log | grep Fatal`
	if [ "$fatal" != "" ]; then
	    echo "Fatal"
	    fail=1
	fi
    fi
    
    
    ### error check for lumi jobs	
    if [ "$jobtype" == "lumi" ] && [ "$fail" == "0" ]; then
	if [  ! -f  $outputdir/${run}.csv ]; then
	    echo "no csv"
	    fail=1
	fi
    fi
    
    
    ### error check for corrections jobs
    if [ "$jobtype" == "corr" ] && [ "$fail" == "0" ] ; then
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

    #if [ "$counter" == "2" ]; then break; fi
    #if [ "$run" != "306550" ]; then continue; fi        

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

    ##get ref data from brilcalc
    if [ "$action" == "4" ] && [ "$ref" != "" ] ; then
	command="brilcalc lumi -u hz/ub -r $run --byls  --output-style csv --normtag ${normtagdir}/normtag_${ref}.json "
	#echo $command
	${command} $goldenjson | grep ${run}: | sed -e 's/,/ /g' | sed -e 's/:/ /g' | sed -e 's/\[//g'  | sed -e 's/\]//g' > $outputdir/${run}.$ref
    fi

    ## make plots with ROOT
    if [ "$action" == "5" ] && [ -f  $outputdir/${run}.csv ] && [ -f  $outputdir/${run}.${ref} ]; then
	root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCcsv.C\(\"${outputdir}\",${run},\"${plotsdir}\",\"${ref}\",0\)
    fi

    RUNLIST=$RUNLIST,$run
    counter=`echo $counter | awk '{print $1+1}'`
done
echo "Total runs: $counter"


## plots for entire period
if [ "$action" == "6" ] ; then
    root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCStability.C\(\"${plotsdir}\",\"${ref}\"\)
    root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCruns.C\(\"${plotsdir}\",\"${ref}\"\)
    root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotModuleFrac.C\(\"${outputdir}\",\"${RUNLIST}\",\"${plotsdir}\"\)
fi
