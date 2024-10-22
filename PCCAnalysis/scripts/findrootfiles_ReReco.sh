
INPUTPATH=$1
OUTPUTPATH=$2

#for f in `ls /eos/cms/store/data/Run2017B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/*/*`
for f1 in `ls $INPUTPATH/*/*`
do 
export f2=` echo $f1 | awk -F"/" '{print $11}'| sed -e 's/.root//g'`
#echo "file:$f1" >> AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1/Run2017B/${f2}.txt
echo "file:$f1" >> $OUTPUTPATH/${f2}.txt
done

