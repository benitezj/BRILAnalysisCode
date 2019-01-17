#!/bin/bash

inputdir=$1
if [ "$inputdir" == "" ]; then
echo "invalid inputdir"
return
fi
#get the absolute path
inputdir=`readlink -f $inputdir`
extdir=$1 #relative path

## what to do : 0=create scripts, 1=submit, 2=check
action=$2


###################
##### options
###################
## only a few runs
TEST=0

#eos=/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select
eos=''

INSTALLATION=${CMSSW_BASE}/src

jobtype=lumi  #options: corr, lumi
echo "job type: $jobtype"

## in case of jobtype=lumi: directory containing the PCC corrections to be applied
## set to "" to use FrontierConditions
#DBDIR=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom
#DBDIR=/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/AlCaPCCRandom/AlCaLumiPixels011_AlCaPCCRandom_Nov22/Commissioning2018
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
    rm -f $inputdir/${run}.log
    $eos rm ${outputdir}/${run}.root
    $eos rm ${outputdir}/${run}.db
    $eos rm ${outputdir}/${run}.txt
    $eos rm ${outputdir}/${run}.csv
    bsub -q 1nd -o $inputdir/${run}.log -J $run < $inputdir/${run}.sh    
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

    /bin/cp $cfg $inputdir/cfg.py


    echo "mkdir -p $outputdir"
    mkdir -p $outputdir
    /bin/ls $outputdir

fi

## clean up the runs file
if [ "$action" == "5" ] ; then
    rm -f $inputdir/runs.dat
    rm -f $inputdir/ls.dat
fi


##### loop over the runs
counter=0
for f in `/bin/ls $inputdir | grep .txt | grep -v "~" `; do
    #if [ "$TEST" == "1" ] && [ "$counter" == "10" ]; then break; fi

    run=`echo $f | awk -F".txt" '{print $1}'`
    #echo $run

    ##if [ "$TEST" == "1" ] && [ "$run" != "318982" ]; then continue; fi    
    ##if [ "$TEST" == "1" ] && [ "$run" != "318982" ] && [ "$run" != "318983" ] && [ "$run" != "318984" ] ; then continue; fi    
    ##if [ "$TEST" == "1" ] && [ "$run" != "316766" ]; then continue; fi        
    if [ "$TEST" == "1" ] && [ "$run" != "323700" ]; then continue; fi        

    ###create the scripts
    if [ "$action" == "0" ]; then
	rm -f $inputdir/${run}.sh
	echo "export X509_USER_PROXY=${HOME}/x509up_u55361 " >> $inputdir/${run}.sh
	echo "cd ${INSTALLATION} " >> $inputdir/${run}.sh
	echo "eval \`scramv1 runtime -sh\` " >> $inputdir/${run}.sh
	echo "cd \$TMPDIR  "   >> $inputdir/${run}.sh
	echo "pwd  "   >> $inputdir/${run}.sh
	if [ "$DBDIR" != "" ]; then
	    echo "export DBFILE=${DBDIR}/${run}.db" >> $inputdir/${run}.sh
	fi
	echo "export INPUTFILE=${inputdir}/${run}.txt" >> $inputdir/${run}.sh
	echo "env" >> $inputdir/${run}.sh

	echo "cmsRun  ${inputdir}/cfg.py" >> $inputdir/${run}.sh

	if [ "$jobtype" == "lumi" ] ; then
	    echo "cmsRun  ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/python/pcc_LumiInfoRead_cfg.py " >> $inputdir/${run}.sh
	    echo "${eos} cp PCCLumiByBX.csv  $outputdir/${run}.csv " >> $inputdir/${run}.sh
	    echo "${eos} cp PCCLumiByBX.err  $outputdir/${run}.err " >> $inputdir/${run}.sh
	fi

	if [ "$jobtype" == "corr" ] ; then
	    echo "${eos} cp PCC_Corr.db $outputdir/${run}.db " >> $inputdir/${run}.sh
	    echo "${eos} cp CorrectionHisto.root $outputdir/${run}.root " >> $inputdir/${run}.sh
	fi

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
	
	if [ "$fail" == "0" ]; then
	    success=`cat $inputdir/${run}.log | grep "Successfully completed."`
	    if [ "$success" == "" ]; then
		echo "no Success"
		fail=1
	    fi
	fi

	if [ "$fail" == "0" ]; then
	    fatal=`cat $inputdir/${run}.log | grep Fatal`
	    if [ "$fatal" != "" ]; then
		echo "Fatal"
	    fail=1
	    fi
	fi

	
	if [ "$fail" == "0" ]; then
            ###### error check for lumi jobs
	    if [ "$jobtype" == "lumi" ] ; then
	       #rootf=`$eos ls $outputdir/${run}.root | grep err `
	       #if [ "$rootf" != "" ]; then
	       #	echo "no root"
	       #	fail=1
	       #fi
		
	        #csvf=`$eos ls $outputdir/${run}.csv | grep err `
		if [  ! -f  $outputdir/${run}.csv ]; then
		    echo "no csv"
		    fail=1
		fi
	    fi
	fi


	#### error check for corrections jobs
	if [ "$jobtype" == "corr" ] && [ "$fail" == "0" ] ; then
	    dbf=`$eos ls $outputdir/${run}.db | grep err `
	    if [ "$dbf" != "" ]; then
		echo "no db"
		fail=1
	    fi
	fi
	

	if [ "$fail" == "1" ]; then
	    echo $inputdir/${run}.log
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

	    /bin/rm -f $inputdir/${run}.$ref
	    
#	    if [ "$ref" == "normtag_BRIL" ]; then
#		brilcalc lumi --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_BRIL.json -r $run --byls --output-style csv | grep ${run}: | sed -e 's/,/ /g' | sed -e 's/:/ /g' >> $inputdir/${run}.$ref
#	    fi 
#
#	    if [ "$ref" != "normtag_BRIL" ]; then
#		brilcalc lumi -r $run --byls --type $ref --output-style csv | grep ${run}: | grep $ref | sed -e 's/,/ /g' | sed -e 's/:/ /g' >> $inputdir/${run}.$ref
#		#brilcalc lumi -r $run --xing --type $ref --output-style csv | grep ${run}: | grep $ref | sed -e 's/,/ /g' | sed -e 's/:/ /g'  | sed -e 's/\[//g'  | sed -e 's/\]//g' > $inputdir/${run}.$ref
#	    fi

	    brilcalc lumi -c web --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_${ref}.json -r $run --byls --output-style csv | grep ${run}: | sed -e 's/,/ /g' | sed -e 's/:/ /g' >> $inputdir/${run}.$ref
	fi

        # run plotting macro
	root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCcsv.C\(\"${outputdir}\",${run},\"${inputdir}\",\"${ref}\"\)

    fi


    counter=`echo $counter | awk '{print $1+1}'`
done

echo "Total runs: $counter"

