set INSTALLATION=/afs/cern.ch/user/b/benitezj/scratch0/BRIL_PCC/CMSSW_10_1_0/src

cd $INSTALLATION
eval `scramv1 runtime -sh`

mkdir /tmp/benitezj
cd /tmp/benitezj

cmsRun $INSTALLATION/BRILAnalysisCode/PCCAnalysis/test/lumi_producer_cfg.py

/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select cp PCC_ZB.root /eos/cms/store/cmst3/user/benitezj/BRIL/PCC/Run316559/PCC_ZB_Average.root
/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select cp rawPCC.csv  /eos/cms/store/cmst3/user/benitezj/BRIL/PCC/Run316559/PCC_ZB_Average.csv
