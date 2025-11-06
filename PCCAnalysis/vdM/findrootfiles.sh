
#INPATH=/eos/cms/store/group/comm_luminosity/lcuevasp/Run2/6016/ZeroBias1/crab_CMSSW_10_6_29_ZeroBias1/221021_224816/0000
#OUTPATH=ZeroBias1

INPATH=$1
OUTPATH=$2

mkdir $OUTPATH
rm -f $OUTPATH/*
##create one job .txt per root file
for f in `ls ${INPATH} | grep .root | awk -F".root" '{print $1}'`;do
    echo  ${INPATH}/${f}.root > ${OUTPATH}/${f}.txt
done
