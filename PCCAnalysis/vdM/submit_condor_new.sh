#!/bin/bash

## path to submission directory
submitdir=$1 

## option for: 0=create scripts, 1=submit, 2=check
action=$2


#baseoutdir=/eos/user/b/benitezj/BRIL/PCC/VDM
baseoutdir=/eos/user/l/lcuevasp/BRIL/PCC/VDM_2017

#python makeVdMMiniTree.py --pccfile=/eos/cms/store/group/comm_luminosity/PCC/ForLumiComputation/2018/NormalFills/6847_And_6854_And_6868/ZeroBias8/crab_CMSSW_10_3_2_ZeroBias8_splitPerBXTrue/211021_070146/0000/pcc_Data_PixVtx_Event_90X_3.root --vetoModules=vetoModules_2018.txt --outputDir=skimPCCjobs_test/output/Data_3.root

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
    rm -f ${outputdir}/${run}.root

    condor_submit $fullsubmitdir/${run}.sub 
}

make_sh_script(){
    local run=$1

    rm -f $fullsubmitdir/${run}.sh

    #echo "export X509_USER_PROXY=${HOME}/x509up_u55361 " >> $fullsubmitdir/${run}.sh
    echo "cd ${INSTALLATION} " >> $fullsubmitdir/${run}.sh
    echo "eval \`scramv1 runtime -sh\` " >> $fullsubmitdir/${run}.sh
  #  echo "export LD_LIBRARY_PATH=/path/to/libs:\$LD_LIBRARY_PATH" >> $fullsubmitdir/${run}.sh
    echo "cd \$TMPDIR  "   >> $fullsubmitdir/${run}.sh
    echo "pwd  "   >> $fullsubmitdir/${run}.sh

    echo "env" >> $fullsubmitdir/${run}.sh
    
    echo "python3  ${fullsubmitdir}/cfg.py --vetoModules=${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/test/vetoModules_2017.txt --inputfile=${fullsubmitdir}/${run}.txt" >> $fullsubmitdir/${run}.sh

   # echo "python  /afs/cern.ch/user/l/lcuevasp/CMSSW/CMSSW_10_6_29/src/BRILAnalysisCode/PCCAnalysis/vdM/pccminitree_to_hd5_vtx.py --minitreefile=./pccVdmMiniTree.root" >> $fullsubmitdir/${run}.sh
 
#Using new veto:   
    #echo "python  ${fullsubmitdir}/cfg.py --vetoModules=${INSTALLATION}/BRILAnalysisCode/PCCAnalysis/test/Veto-2018-B-vdM.txt --inputfile=${fullsubmitdir}/${run}.txt" >> $fullsubmitdir/${run}.sh
    
    #echo "cp pccVdmMiniTree.root  $outputdir/${run}.root " >> $fullsubmitdir/${run}.sh
    echo "cp pcc_ZB.hd5  $outputdir/${run}.hd5 " >> $fullsubmitdir/${run}.sh
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


    RUNLIST=$RUNLIST,$run
    counter=`echo $counter | awk '{print $1+1}'`
done
echo "Total runs: $counter"
