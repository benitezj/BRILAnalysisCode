#!/bin/bash

## path to submission directory
submitdir=$1 

## option for: 0=create scripts, 1=submit, 2=check
action=$2
## arg 3 needed for action 0 below

##corr: afterglow corrections with Random triggers,
##lumi: ZeroBias triggers luminosity 
#jobtype=lumi

jobtype=lumi

#baseoutdir=/eos/user/a/asehrawa/PCC_veto_1701/EXPRESS_datasets
#plotsdir=/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_veto_1701/EXPRESS_datasets

#baseoutdir=/eos/user/a/asehrawa/PCC_veto_1615/EXPRESS_datasets
#plotsdir=/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_veto_1615/EXPRESS_datasets

#baseoutdir=/eos/user/a/asehrawa/PCC_veto_1615_new_format/EXPRESS_datasets
#plotsdir=/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_veto_1615_new_format/EXPRESS_datasets

baseoutdir=/eos/user/a/asehrawa/PCC_newformat/EXPRESS_datasets
plotsdir=/afs/cern.ch/user/a/asehrawa/PCC_newformat/CMSSW_10_6_30/src/PCC_veto_1701_plots

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

plotsdir=${plotsdir}/$submitdir
#plotsdir=${plotsdir}/zerobias_27Oct2022_Run2018B
echo "plotsdir: $plotsdir"

### full path to output directory
if [ "$jobtype" == "lumi" ]; then
outputdir=$baseoutdir/ZeroBias/$submitdir
#outputdir=$baseoutdir/zerobias_27Oct2022_Run2018B
fi
if [ $jobtype == "corr" ]; then
outputdir=$baseoutdir/AlCaPCCRandom/$submitdir
fi
echo "output: $outputdir"

##############################################################
## directory containing the Afterglow corrections if computed privately
#DBDIR=/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/afterglow_veto_1615/AlCaPCCRandom/afterglow_merged
DBDIR=/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/afterglow_veto_1701/AlCaPCCRandom/afterglow_merged
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
    local rootfilename=$1
    rm -f $fullsubmitdir/${rootfilename}.log
    rm -f ${outputdir}/${rootfilename}.root
    rm -f ${outputdir}/${rootfilename}.db
    rm -f ${outputdir}/${rootfilename}.txt
    rm -f ${outputdir}/${rootfilename}.csv
    rm -f ${outputdir}/${rootfilename}.mod
    rm -f ${outputdir}/${rootfilename}.frac

    #bsub -q 1nd -o $fullsubmitdir/${run}.log -J $run < $fullsubmitdir/${run}.sh    
    condor_submit $fullsubmitdir/${rootfilename}.sub 
}

make_sh_script(){
    local rootfilename=$1

    rm -f $fullsubmitdir/${rootfilename}.sh

    echo "export X509_USER_PROXY=${HOME}/x509up_u55361 " >> $fullsubmitdir/${rootfilename}.sh
    echo "cd ${INSTALLATION} " >> $fullsubmitdir/${rootfilename}.sh
    echo "eval \`scramv1 runtime -sh\` " >> $fullsubmitdir/${rootfilename}.sh
    echo "cd \$TMPDIR  "   >> $fullsubmitdir/${rootfilename}.sh
    echo "pwd  "   >> $fullsubmitdir/${rootfilename}.sh

    ###if DBDIR is set this will override the Afterglow corrections 
    if [ "$DBDIR" != "" ]; then
	echo "export DBFILE=${DBDIR}/${rootfilename}.db" >> $fullsubmitdir/${rootfilename}.sh
    fi
    if [ "$DBROOTDIR" != "" ]; then
	echo "export DBROOT=${DBROOTDIR}/${rootfilename}.root" >> $fullsubmitdir/${rootfilename}.sh
    fi

    echo "export INPUTFILE=${fullsubmitdir}/${rootfilename}.txt" >> $fullsubmitdir/${rootfilename}.sh
    
    ###if run.json file is there, this will apply a json when processing  
    if [ -f ${fullsubmitdir}/${rootfilename}.json ]; then
	echo "export JSONFILE=${fullsubmitdir}/${rootfilename}.json" >> $fullsubmitdir/${rootfilename}.sh
    fi
    
    echo "env" >> $fullsubmitdir/${rootfilename}.sh
    
    echo "cmsRun  ${fullsubmitdir}/cfg.py" >> $fullsubmitdir/${rootfilename}.sh
    
    if [ "$jobtype" == "corr" ] ; then
	echo "cp PCC_Corr.db  $outputdir/${rootfilename}.db " >> $fullsubmitdir/${rootfilename}.sh
	echo "cp CorrectionHisto.root  $outputdir/${rootfilename}.root " >> $fullsubmitdir/${rootfilename}.sh
    fi
    
    if [ "$jobtype" == "lumi" ] ; then
	echo "cp rawPCC.csv  $outputdir/${rootfilename}.csv " >> $fullsubmitdir/${rootfilename}.sh
	echo "cp rawPCC.err  $outputdir/${rootfilename}.err " >> $fullsubmitdir/${rootfilename}.sh
	echo "cp moduleFraction.csv  $outputdir/${rootfilename}.frac " >> $fullsubmitdir/${rootfilename}.sh
	echo "cp modCount.txt  $outputdir/${rootfilename}.mod " >> $fullsubmitdir/${rootfilename}.sh

	##for ALCARECO jobs
	echo "cp step5_ALCAPRODUCER.root  $outputdir/${rootfilename}.root " >> $fullsubmitdir/${rootfilename}.sh
    fi

}    
    

make_sub_script(){
    local rootfilename=$1
    rm -f $fullsubmitdir/${rootfilename}.sub

    echo "Universe   = vanilla" >>  $fullsubmitdir/${rootfilename}.sub
    echo "+JobFlavour = \"${condorqueue}\" " >> $fullsubmitdir/${rootfilename}.sub
    echo "Executable = /bin/bash" >> $fullsubmitdir/${rootfilename}.sub 
    echo "Arguments  = ${fullsubmitdir}/${rootfilename}.sh" >> $fullsubmitdir/${rootfilename}.sub 
    echo "Log        = ${fullsubmitdir}/${rootfilename}.log" >> $fullsubmitdir/${rootfilename}.sub 
    echo "Output     = ${fullsubmitdir}/${rootfilename}.log" >> $fullsubmitdir/${rootfilename}.sub 
    echo "Error      = ${fullsubmitdir}/${rootfilename}.log" >> $fullsubmitdir/${rootfilename}.sub 
    echo "Queue  " >> $fullsubmitdir/${rootfilename}.sub 
}


check_log(){
    local rootfilename=$1
    fail=0
    
    if [ ! -f $fullsubmitdir/${rootfilename}.log ]; then
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
    #fatal=`cat $fullsubmitdir/${run}.log | grep Fatal`
    #if [ "$fatal" != "" ]; then
    #    echo "Fatal"
    #    fail=1
    #fi
#    fi
    
    
    ### error check for lumi jobs
    if [ "$jobtype" == "lumi" ] && [ "$fail" == "0" ]; then
	if [  ! -f  $outputdir/${rootfilename}.csv ]; then
	                echo "no csv"
			        fail=1
				    fi
    fi
    
    
    ### error check for corrections jobs
    if [ "$jobtype" == "corr" ] && [ "$fail" == "0" ] ; then
	if [ ! -f $outputdir/${rootfilename}.db ]; then
	                echo "no db"
			        fail=1
				    fi
    fi
    
}

###################################################################
##### loop over the rootfile names
##################################################################
export ROOTFILENAME=""
counter=0
for f in `/bin/ls $fullsubmitdir | grep .txt | grep -v "~" `; do
    rootfilename=`echo $f | awk -F".txt" '{print $1}'`
    echo $rootfilename

    #if [ "$counter" == "2" ]; then break; fi
    #if [ "$run" != "306550" ]; then continue; fi        

    ##create the scripts
    if [ "$action" == "0" ]; then
	make_sh_script $rootfilename
	make_sub_script $rootfilename
    fi

    ##submit to lxbatch
    if [ "$action" == "1" ]; then
	submit $rootfilename
    fi

    ##check failed jobs
    if [ "$action" == "2" ] ; then
	check_log $rootfilename
	if [ "$fail" == "1" ]; then
	                echo $fullsubmitdir/${rootfilename}.log
			    fi   
    fi

    if [ "$action" == "3" ]; then
	check_log $rootfilename
	if [ "$fail" == "1" ]; then
	                submit $rootfilename
			    fi   
    fi

    ##get ref data from brilcalc
    if [ "$action" == "4" ] && [ "$ref" != "" ] ; then
	command="brilcalc lumi -u hz/ub -r ${rootfilename} --byls  --output-style csv --normtag ${normtagdir}/normtag_${ref}.json "
	echo $command
	${command} $goldenjson | grep ${rootfilename}: | sed -e 's/,/ /g' | sed -e 's/:/ /g' | sed -e 's/\[//g'  | sed -e 's/\]//g' > $outputdir/${rootfilename}.$ref
    fi

    ## make plots with ROOT
    #if [ "$action" == "5" ] && [ -f  $outputdir/${run}.csv ] && [ -f  $outputdir/${run}.${ref} ]; then
    if [ "$action" == "5" ] ; then
	root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCcsv.C\(\"${outputdir}\",${rootfilename},\"${plotsdir}\",\"${ref}\",0\)
    fi

    if [ "$action" == "7" ] && [ -f  $outputdir/${rootfilename}.root ]; then
	root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/compareRandomRawCorr.C\(\"${outputdir}\",${rootfilename},\"${plotsdir}\"\)
    fi

    ROOTFILENAME=$ROOTFILENAME,$rootfilename
    counter=`echo $counter | awk '{print $1+1}'`
done
echo "Total rootfiles: $counter"


## plots for entire period
if [ "$action" == "6" ] ; then
    root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCStability.C\(\"${plotsdir}\",\"${ref}\"\)
    #root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotPCCruns.C\(\"${plotsdir}\",\"${ref}\"\)
    #root -b -q -l ${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/plots/plotModuleFrac.C\(\"${outputdir}\",\"${RUNLIST}\",\"${plotsdir}\"\)
fi
