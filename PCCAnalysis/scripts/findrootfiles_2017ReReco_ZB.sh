
PERIOD=Run2017B
DATASET=AlCaPCCZeroBias-28Aug24_UL2017_PCCZeroBias-v1

if [ -d ./$PERIOD ]; then
    echo "output dir exists, need to remove it first"
    return
fi
mkdir ./$PERIOD

find "/eos/cms/store/data/${PERIOD}/AlCaLumiPixels/ALCARECO/${DATASET}/" -name "*.root" -type f | while read file; do
    f2=` echo $file | awk -F"/" '{print $11}'| sed -e 's/.root//g'`
    echo "file:$file" > ./$PERIOD/${f2}.txt
    #echo "$file"
    #echo "$f2"
done
