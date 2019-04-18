#!/bin/bash

submitdir=$1
if [ "$submitdir" == "" ]; then
echo "invalid submitdir"
return
fi
#get the absolute path
submitdir=`readlink -f $submitdir`
extdir=$1 #relative path

## what to do : 0=create scripts, 1=submit, 2=check
action=$2


normtagdir=/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags

###################
###  options
###################
## only a few runs
TEST=0

INSTALLATION=${CMSSW_BASE}/src

### options:
#corr= PCC corrections from Random triggers
#lumi= lumi from ZeroBias 
jobtype=lumi
echo "job type: $jobtype"

## in case of jobtype=lumi: directory containing the PCC corrections to be applied
## set to "" to use FrontierConditions
#DBDIR=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels011_AlCaPCCRandom_Nov22/Commissioning2018
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels_AlCaPCCRandom-Express/Run2018E
if [ "$DBDIR" != "" ]; then
   echo "corections: $DBDIR"
fi


#########################################################################################
########################################################################################
### define output directory
#outputdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/ZeroBias/$extdir
#outputdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/$extdir  
#outputdir=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/$extdir
#outputdir=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/$extdir
baseoutdir=""
if [ "$jobtype" == "lumi" ]; then
baseoutdir=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias
fi

if [ $jobtype == "corr" ]; then
baseoutdir=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom
fi

outputdir=$baseoutdir/$extdir
echo "output: $outputdir"



########################
### lxbatch submit
submit(){
    local run=$1
    rm -f $submitdir/${run}.log
    rm -f ${outputdir}/${run}.root
    rm -f ${outputdir}/${run}.db
    rm -f ${outputdir}/${run}.txt
    rm -f ${outputdir}/${run}.csv
    #bsub -q 1nd -o $submitdir/${run}.log -J $run < $submitdir/${run}.sh    
    condor_submit $submitdir/${run}.sub 
}

## copy the proxy in case of xrootd access:
/bin/cp /tmp/x509up_u55361 ${HOME}/

## copy the cfg
if [ "$action" == "0" ]; then 
    cfg=$3
    if [ "$cfg" == "" ]; then
	echo "No cfg provided\n"
	exit;
    fi

    /bin/cp $cfg $submitdir/cfg.py


    echo "mkdir -p $outputdir"
    mkdir -p $outputdir
    /bin/ls $outputdir

fi

## clean up the runs file
if [ "$action" == "5" ] ; then
    rm -f $submitdir/runs.dat
    rm -f $submitdir/ls.dat
    rm -f $submitdir/slope.dat
fi


##### loop over the runs
counter=0
for f in `/bin/ls $submitdir | grep .txt | grep -v "~" `; do
    #if [ "$TEST" == "1" ] && [ "$counter" == "10" ]; then break; fi

    run=`echo $f | awk -F".txt" '{print $1}'`
    #echo $run

    ##if [ "$TEST" == "1" ] && [ "$run" != "318982" ]; then continue; fi    
    ##if [ "$TEST" == "1" ] && [ "$run" != "318982" ] && [ "$run" != "318983" ] && [ "$run" != "318984" ] ; then continue; fi    
    ##if [ "$TEST" == "1" ] && [ "$run" != "316766" ]; then continue; fi        
    ##if [ "$TEST" == "1" ] && [ "$run" != "323700" ]; then continue; fi        
    if [ "$TEST" == "1" ] && [ "$run" != "323696" ]; then continue; fi        

    ###create the scripts
    if [ "$action" == "0" ]; then
	rm -f $submitdir/${run}.sh
	rm -f $submitdir/${run}.sub

	echo "export X509_USER_PROXY=${HOME}/x509up_u55361 " >> $submitdir/${run}.sh
	echo "cd ${INSTALLATION} " >> $submitdir/${run}.sh
	echo "eval \`scramv1 runtime -sh\` " >> $submitdir/${run}.sh
	echo "cd \$TMPDIR  "   >> $submitdir/${run}.sh
	echo "pwd  "   >> $submitdir/${run}.sh
	if [ "$DBDIR" != "" ]; then
	    echo "export DBFILE=${DBDIR}/${run}.db" >> $submitdir/${run}.sh
	fi
	echo "export INPUTFILE=${submitdir}/${run}.txt" >> $submitdir/${run}.sh
	echo "env" >> $submitdir/${run}.sh

	echo "cmsRun  ${submitdir}/cfg.py" >> $submitdir/${run}.sh

	if [ "$jobtype" == "lumi" ] ; then
	    echo "cmsRun  ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/python/pcc_LumiInfoRead_cfg.py " >> $submitdir/${run}.sh
	    echo "cp PCCLumiByBX.csv  $outputdir/${run}.csv " >> $submitdir/${run}.sh
	    echo "cp PCCLumiByBX.err  $outputdir/${run}.err " >> $submitdir/${run}.sh
	fi

	if [ "$jobtype" == "corr" ] ; then
	    echo "cp PCC_Corr.db $outputdir/${run}.db " >> $submitdir/${run}.sh
	    echo "cp CorrectionHisto.root $outputdir/${run}.root " >> $submitdir/${run}.sh
	fi


	##create condor jdl
	echo "Universe   = vanilla" >>  $submitdir/${run}.sub
	echo "+JobFlavour = \"workday\" " >> $submitdir/${run}.sub
	echo "Executable = /bin/bash" >> $submitdir/${run}.sub 
	echo "Arguments  = ${submitdir}/${run}.sh" >> $submitdir/${run}.sub 
	echo "Log        = ${submitdir}/${run}.log" >> $submitdir/${run}.sub 
	echo "Output     = ${submitdir}/${run}.log" >> $submitdir/${run}.sub 
	echo "Error      = ${submitdir}/${run}.log" >> $submitdir/${run}.sub 
	echo "Queue  " >> $submitdir/${run}.sub 

    fi
    

    ##submit to lxbatch
    if [ "$action" == "1" ]; then
	submit $run
    fi


    ####check failed jobs
    if [ "$action" == "2" ] || [ "$action" == "3" ]; then
	fail=0
	
	if [ ! -f $submitdir/${run}.log ]; then
	    echo "no log"
	    fail=1
	fi
	
	if [ "$fail" == "0" ]; then
	    success=`cat $submitdir/${run}.log | grep "Successfully completed."`
	    if [ "$success" == "" ]; then
		echo "no Success"
		fail=1
	    fi
	fi

	if [ "$fail" == "0" ]; then
	    fatal=`cat $submitdir/${run}.log | grep Fatal`
	    if [ "$fatal" != "" ]; then
		echo "Fatal"
	    fail=1
	    fi
	fi

	
	if [ "$fail" == "0" ]; then
            ###### error check for lumi jobs
	    if [ "$jobtype" == "lumi" ] ; then
		if [  ! -f  $outputdir/${run}.csv ]; then
		    echo "no csv"
		    fail=1
		fi
	    fi
	fi


	#### error check for corrections jobs
	if [ "$jobtype" == "corr" ] && [ "$fail" == "0" ] ; then
	    dbf=`/bin/ls $outputdir/${run}.db | grep err `
	    if [ "$dbf" != "" ]; then
		echo "no db"
		fail=1
	    fi
	fi
	

	if [ "$fail" == "1" ]; then
	    echo $submitdir/${run}.log
	    if [ "$action" == "3" ]; then
		submit $run
	    fi
	fi

    fi



    ## produce the plots for lumi jobs
    if [ "$action" == "5" ] ; then

	# create reference csv for comparison
	# note brilcalc must be setup 
#	ref=hfet
	ref=hfoc
#	ref=dt
#	ref=pltzero
# ref=bcm1f
#	ref=BRIL
	if [ "$ref" != "" ]; then

	    /bin/rm -f $submitdir/${run}.$ref
	    
#	    if [ "$ref" == "normtag_BRIL" ]; then
#		brilcalc lumi --normtag ${normtagdir}/normtag_BRIL.json -r $run --byls --output-style csv | grep ${run}: | sed -e 's/,/ /g' | sed -e 's/:/ /g' >> $submitdir/${run}.$ref
#	    fi 
#	    if [ "$ref" != "normtag_BRIL" ]; then
#		brilcalc lumi -r $run --byls --type $ref --output-style csv | grep ${run}: | grep $ref | sed -e 's/,/ /g' | sed -e 's/:/ /g' >> $submitdir/${run}.$ref
#		#brilcalc lumi -r $run --xing --type $ref --output-style csv | grep ${run}: | grep $ref | sed -e 's/,/ /g' | sed -e 's/:/ /g'  | sed -e 's/\[//g'  | sed -e 's/\]//g' > $submitdir/${run}.$ref
#	    fi
	    
	    #brilcalc lumi -c web --normtag ${normtagdir}/normtag_${ref}.json -r $run --byls --output-style csv | grep ${run}: | sed -e 's/,/ /g' | sed -e 's/:/ /g' >> $submitdir/${run}.$ref
	    
	    command="brilcalc lumi -c web --normtag ${normtagdir}/normtag_${ref}.json -r $run --xing --output-style csv"
	    echo $command
	    brilcalc lumi -c web --normtag ${normtagdir}/normtag_${ref}.json -r $run --xing --output-style csv | grep ${run}: | sed -e 's/,/ /g' | sed -e 's/:/ /g' | sed -e 's/\[//g'  | sed -e 's/\]//g' >> $outputdir/${run}.$ref
	fi

        # run plotting macro
	root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCcsv.C\(\"${outputdir}\",${run},\"${submitdir}\",\"${ref}\",1\)
	/bin/rm -f $submitdir/${run}.$ref
    fi


    counter=`echo $counter | awk '{print $1+1}'`
done

echo "Total runs: $counter"

