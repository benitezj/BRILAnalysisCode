#!/bin/bash

inputdir=$1
if [ "$inputdir" == "" ]; then
echo "invalid inputdir"
return
fi
#get the absolute path
inputdir=`readlink -f $inputdir`
extdir=$1 #relative path

## what to do : create scripts, submit, check
action=$2

## only a few runs
TEST=0


###################
###### options
###################
eos=/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select

#INSTALLATION=/afs/cern.ch/user/b/benitezj/scratch0/BRIL_PCC/CMSSW_10_1_0/src
INSTALLATION=${CMSSW_BASE}/src

### which script to run
jobtype=lumi  #options: corr, lumi, lumi_nocorr

## directory containing the corrections in case of jobtype=lumi jobs
## set to "" to use FrontierConditions
DBDIR=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/$extdir


###define output directory
#eosoutdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaLumiPixels_ZB/$extdir
eosoutdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/ZeroBias/$extdir
#eosoutdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/ZeroBias_noCorr/$extdir
#eosoutdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/AlCaPCCRandom/$extdir  
#eosoutdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/ZeroBias_19Aug/$extdir
#eosoutdir=/eos/cms/store/cmst3/user/benitezj/BRIL/PCC/ZeroBias_21Aug/$extdir


###lxbatch submit
submit(){
    local run=$1

    rm -f $inputdir/${run}.log

    $eos rm ${eosoutdir}/${run}.root
    $eos rm ${eosoutdir}/${run}.db
    $eos rm ${eosoutdir}/${run}.txt
    $eos rm ${eosoutdir}/${run}.csv

    bsub -q 1nd -o $inputdir/${run}.log -J $run < $inputdir/${run}.sh    
}


##clean up the runs file
if [ "$action" == "5" ] ; then
    rm -f $inputdir/runs.txt
fi


##### loop over the runs
counter=0
for f in `/bin/ls $inputdir | grep .txt `; do
    #if [ "$TEST" == "1" ] && [ "$counter" == "10" ]; then break; fi

    run=`echo $f | awk -F".txt" '{print $1}'`
    echo $run

    if [ "$TEST" == "1" ] && [ "$run" != "316766" ]; then continue; fi    
    
    ###create the scripts
    if [ "$action" == "0" ]; then
	rm -f $inputdir/${run}.sh
	echo "cd ${INSTALLATION} " >> $inputdir/${run}.sh
	echo "eval \`scramv1 runtime -sh\` " >> $inputdir/${run}.sh
	echo "cd \$TMPDIR  "   >> $inputdir/${run}.sh
	echo "pwd  "   >> $inputdir/${run}.sh
	if [ "$DBDIR" != "" ]; then
	    echo "export DBFILE=${DBDIR}/${run}.db" >> $inputdir/${run}.sh
	fi
	echo "export INPUTFILE=${inputdir}/${run}.txt" >> $inputdir/${run}.sh
	echo "env" >> $inputdir/${run}.sh

	######################
	#### lumi calculation 
	if [ "$jobtype" == "lumi" ]; then
	    echo "cmsRun ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/test/lumi_alcaZB_cfg.py" >> $inputdir/${run}.sh
	fi

	if [ "$jobtype" == "lumi_nocorr" ]; then
	    echo "cmsRun ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/test/lumi_alcaZB_noCorr_cfg.py " >> $inputdir/${run}.sh
	fi

	if [ "$jobtype" == "lumi" ] || [ "$jobtype" == "lumi_nocorr" ] ; then
	    echo "${eos} cp PCC_ZB.root $eosoutdir/${run}.root " >> $inputdir/${run}.sh
	    
	    ##produce the csv in the same batch job
	    echo "cmsRun  ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/test/pcc_LumiInfoRead_cfg.py " >> $inputdir/${run}.sh
	    echo "${eos} cp PCCLumiByBX.csv  $eosoutdir/${run}.csv " >> $inputdir/${run}.sh
	fi

	#####################
	### corrections
	if [ "$jobtype" == "corr" ] ; then
	    echo "cmsRun ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/test/raw_corr_Random_cfg.py " >> $inputdir/${run}.sh
	    echo "${eos} cp PCC_Corr.db $eosoutdir/${run}.db " >> $inputdir/${run}.sh
	    echo "${eos} cp CorrectionHisto.root $eosoutdir/${run}.root " >> $inputdir/${run}.sh
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
	
	success=`cat $inputdir/${run}.log | grep "Successfully completed."`
	if [ "$success" == "" ]; then
	    echo "no Success"
	    fail=1
	fi

	fatal=`cat $inputdir/${run}.log | grep Fatal`
	if [ "$fatal" != "" ]; then
	    echo "Fatal"
	    fail=1
	fi
	 
	###### error check for lumi jobs
	if [ "$jobtype" == "lumi" ] || [ "$jobtype" == "lumi_nocorr" ] ; then
	    rootf=`$eos ls $eosoutdir/${run}.root | grep err `
	    if [ "$rootf" != "" ]; then
		echo "no root"
		fail=1
	    fi
	    
	    csvf=`$eos ls $eosoutdir/${run}.csv | grep err `
	    if [ "$csvf" != "" ]; then
		echo "no csv"
		fail=1
	    fi
	fi


	#### error check for corrections jobs
	if [ "$jobtype" == "corr" ] ; then
	    dbf=`$eos ls $eosoutdir/${run}.db | grep err `
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

	#create reference csv for comparison
	# note brilcalc must be setup 
	ref=HFET
	/bin/rm -f $inputdir/${run}.$ref
	brilcalc lumi -r $run  --byls --type $ref --output-style csv | grep $ref | awk -F"," '{split($1,r,":"); split($2,ls,":"); print r[1]","ls[1]","$7}' >> $inputdir/${run}.$ref

	##run plotting macr
	root -b -q ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/test/plotPCCcsv.C\(\"$eosoutdir\",$run,\"$inputdir\",\"$ref\"\)
    fi


    counter=`echo $counter | awk '{print $1+1}'`
done

echo "Total runs: $counter"

