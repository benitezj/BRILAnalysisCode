
installation=/afs/cern.ch/user/b/benitezj/output/BRIL/2025_EmmittanceScans/CMSSW_15_0_6/src/
period=Run2025C
version=EmmittanceScans_v1
outpath=/eos/user/b/benitezj/BRIL/PCC_Run3/$version
plotspath=/eos/user/b/benitezj/www/plots/BRIL/PCC_lumi/$version
python=/cvmfs/cms.cern.ch/el9_amd64_gcc12/external/python3/3.9.14-2cc3edc74f38772096f1e8978a9716da/bin/python3

if [ ! -d $outpath ]; then
    mkdir $outpath
    echo "created outpath"
fi
if [ ! -d $plotspath ]; then
    mkdir $plotspath
    echo "created plots path"
fi

### step0: setup environment
cd $installation
`/cvmfs/cms.cern.ch/common/scramv1 runtime -sh`

### step1: look for new runs in tier0 eos
rm -rf ./$period
`${installation}/BRILAnalysisCode/PCCAnalysis/emmit_scans/findrootfiles_emmittance.sh ${period}`


### step2: loop over the new runs and check if each run is already processed
echo "starting to process new runs:"
for r in `/usr/bin/ls ${installation}/${period}/ | grep .txt | awk -F".txt" '{print $1}'`; do
    echo $r
    if [ -f ${installation}/$version/${r}.txt ]; then
	echo "processed"
    else
	echo "not processed"
	cp ${installation}/${period}/${r}.txt ${installation}/$version/
	export INPUTFILE=${installation}/$version/${r}.txt

	## RAW -> TUPLE
	`cmsRun ${installation}/BRILAnalysisCode/PCCAnalysis/emmit_scans/PCCEventToTuple_cfg.py`

	## TUPLE -> HD5
	`${python} ${installation}/BRILAnalysisCode/PCCAnalysis/emmit_scans/tuple_to_hd5_emmittance.py --inputfile=./pcctuple.root  --moduleveto=${installation}/BRILAnalysisCode/PCCAnalysis/veto_2024/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2_+Block2_Sty1_Sty2+Block3_Sty1_Sty2+Block4_Sty1_Sty2+Block5_Sty1_Sty2+Block6_Sty1_Sty2+All_Blocks_Sty_Lty.txt`

	## simple plot
	`${python}  ${installation}/BRILAnalysisCode/PCCAnalysis/emmit_scans/pcchd5_reader_emmittance.py --outpath=. --lsmax=200 --bcid=56`
	
	## save outputs
	/usr/bin/cp -f pcctuple.root  $outpath/${r}.root
	/usr/bin/cp -f pcc_ZB.hd5  $outpath/${r}.hd5
	/usr/bin/cp -f pcchd5_reader_emmittance.png  $plotspath/${r}.png
	break
    fi
done

