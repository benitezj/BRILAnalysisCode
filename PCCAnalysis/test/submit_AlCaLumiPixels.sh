#!/bin/bash

submitdir=$1
if [ "$submitdir" == "" ]; then
    echo "invalid submitdir"
    return
fi
#get the absolute path
fullsubmitdir=`readlink -f $submitdir`
echo "fullsubmitdir: $fullsubmitdir"

## what to do : 0=create scripts, 1=submit, 2=check
action=$2


normtagdir=/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags
ref=hfoc
INSTALLATION=${CMSSW_BASE}/src
echo "INSTALLATION: $INSTALLATION"

###################
###  options
###################
## only a few runs
TEST=0

#jobtype=corr
jobtype=lumi
echo "job type: $jobtype"

#########################################################
### define output directory
#########################################################
#outputdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/ZeroBias/$submitdir
#outputdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/$submitdir  
#outputdir=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/$submitdir
#outputdir=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/$submitdir
baseoutdir=""
if [ "$jobtype" == "lumi" ]; then
#baseoutdir=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias
baseoutdir=/eos/user/b/benitezj/BRIL/PCC/ZeroBias
fi

if [ $jobtype == "corr" ]; then
#baseoutdir=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom
baseoutdir=/eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom
fi

outputdir=$baseoutdir/$submitdir
echo "output: $outputdir"

##############################################################
## in case of jobtype=lumi: directory containing the PCC corrections to be applied
## set to "" to use FrontierConditions
#########################################################
#DBDIR=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels011_AlCaPCCRandom_Nov22/Commissioning2018
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-Express/Run2018E
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-Express/Run2018E_10LS_F3P2
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-PromptReco/Run2017G
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-PromptReco/Run2017G_v4
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-17Nov2017/Run2017G_v4
#DBDIR=/eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom/Run2018B_dynamicVeto
#DBDIR=/eos/user/b/benitezj/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-PromptReco/Run2017G_v4
if [ "$DBDIR" != "" ]; then
   echo "corections: $DBDIR"
fi



########################
### lxbatch submit
submit(){
    local run=$1
    rm -f $fullsubmitdir/${run}.log
    rm -f ${outputdir}/${run}.root
    rm -f ${outputdir}/${run}.db
    rm -f ${outputdir}/${run}.txt
    rm -f ${outputdir}/${run}.csv
    #bsub -q 1nd -o $fullsubmitdir/${run}.log -J $run < $fullsubmitdir/${run}.sh    
    condor_submit $fullsubmitdir/${run}.sub 
}

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
fi


##### loop over the runs
counter=0
for f in `/bin/ls $fullsubmitdir | grep .txt | grep -v "~" `; do
    run=`echo $f | awk -F".txt" '{print $1}'`
    #echo $run

    if [ "$TEST" == "1" ] && [ "$counter" == "2" ]; then break; fi
    ##if [ "$TEST" == "1" ] && [ "$run" != "324418" ]; then continue; fi        

    ###create the scripts
    if [ "$action" == "0" ]; then
	rm -f $fullsubmitdir/${run}.sh
	rm -f $fullsubmitdir/${run}.sub

	echo "export X509_USER_PROXY=${HOME}/x509up_u55361 " >> $fullsubmitdir/${run}.sh
	echo "cd ${INSTALLATION} " >> $fullsubmitdir/${run}.sh
	echo "eval \`scramv1 runtime -sh\` " >> $fullsubmitdir/${run}.sh
	echo "cd \$TMPDIR  "   >> $fullsubmitdir/${run}.sh
	echo "pwd  "   >> $fullsubmitdir/${run}.sh
	if [ "$DBDIR" != "" ]; then
	    echo "export DBFILE=${DBDIR}/${run}.db" >> $fullsubmitdir/${run}.sh
	fi
	echo "export INPUTFILE=${fullsubmitdir}/${run}.txt" >> $fullsubmitdir/${run}.sh

	###if json exist for each run  
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
#	    echo "cmsRun  ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/python/pcc_LumiInfoRead_cfg.py " >> $fullsubmitdir/${run}.sh
#	    echo "cp PCCLumiByBX.csv  $outputdir/${run}.csv " >> $fullsubmitdir/${run}.sh
#	    echo "cp PCCLumiByBX.err  $outputdir/${run}.err " >> $fullsubmitdir/${run}.sh
#	    echo "cp PCCLumiByBX.mod  $outputdir/${run}.mod " >> $fullsubmitdir/${run}.sh

	    echo "cp rawPCC.csv  $outputdir/${run}.csv " >> $fullsubmitdir/${run}.sh
	    echo "cp rawPCC.err  $outputdir/${run}.err " >> $fullsubmitdir/${run}.sh
	    echo "cp moduleFractionOutputLabel.csv  $outputdir/${run}_frac.csv " >> $fullsubmitdir/${run}.sh
	fi



	##create condor jdl
	echo "Universe   = vanilla" >>  $fullsubmitdir/${run}.sub
#	echo "+JobFlavour = \"microcentury\" " >> $fullsubmitdir/${run}.sub
	echo "+JobFlavour = \"workday\" " >> $fullsubmitdir/${run}.sub
#	echo "+JobFlavour = \"testmatch\" " >> $fullsubmitdir/${run}.sub
	echo "Executable = /bin/bash" >> $fullsubmitdir/${run}.sub 
	echo "Arguments  = ${fullsubmitdir}/${run}.sh" >> $fullsubmitdir/${run}.sub 
	echo "Log        = ${fullsubmitdir}/${run}.log" >> $fullsubmitdir/${run}.sub 
	echo "Output     = ${fullsubmitdir}/${run}.log" >> $fullsubmitdir/${run}.sub 
	echo "Error      = ${fullsubmitdir}/${run}.log" >> $fullsubmitdir/${run}.sub 
	echo "Queue  " >> $fullsubmitdir/${run}.sub 

    fi
    

    ##submit to lxbatch
    if [ "$action" == "1" ]; then
	submit $run
    fi


    ####check failed jobs
    if [ "$action" == "2" ] || [ "$action" == "3" ]; then
	fail=0
	
	if [ ! -f $fullsubmitdir/${run}.log ]; then
	    echo "no log"
	    fail=1
	fi
	
	if [ "$fail" == "0" ]; then
	    #success=`cat $fullsubmitdir/${run}.log | grep "Successfully completed."`
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

	
        ###### error check for lumi jobs
	if [ "$jobtype" == "lumi" ] && [ "$fail" == "0" ]; then
	    if [  ! -f  $outputdir/${run}.csv ]; then
		echo "no csv"
		fail=1
	    fi
	fi


	#### error check for corrections jobs
	if [ "$jobtype" == "corr" ] && [ "$fail" == "0" ] ; then
	    #dbf=`/bin/ls $outputdir/${run}.db | grep err `	    
	    if [ ! -f $outputdir/${run}.db ]; then
		echo "no db"
		fail=1
	    fi
	fi
	

	if [ "$fail" == "1" ]; then
	    echo $fullsubmitdir/${run}.log
	    if [ "$action" == "3" ]; then
		submit $run
	    fi
	fi

    fi


    ################################################################################
    ## action 5 is for producing plots with brilcalc and ROOT
    if [ "$action" == "5" ] && [ -f  $outputdir/${run}.csv ]; then
	# first create reference csv with brilcalc
	if [ "$ref" != "" ]; then
	    /bin/rm -f $outputdir//${run}.$ref
	    command="brilcalc lumi -c web --normtag ${normtagdir}/normtag_${ref}.json -r $run --xing --output-style csv"
	    ${command} | grep ${run}: | sed -e 's/,/ /g' | sed -e 's/:/ /g' | sed -e 's/\[//g'  | sed -e 's/\]//g' >> $outputdir/${run}.$ref
	fi
	# now run ROOT
	root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCcsv.C\(\"${outputdir}\",${run},\"${fullsubmitdir}\",\"${ref}\",1\)
    fi
    #####################################################################################

    counter=`echo $counter | awk '{print $1+1}'`
done


##for the plots run the lumisections and runs comparisons plots
if [ "$action" == "5" ] ; then
    root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCStability.C\(\"${fullsubmitdir}\",\"${ref}\"\)
    root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCruns.C\(\"${fullsubmitdir}\",\"${ref}\"\)
fi



echo "Total runs: $counter"

