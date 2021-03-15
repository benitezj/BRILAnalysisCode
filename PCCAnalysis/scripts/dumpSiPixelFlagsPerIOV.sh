#!/bin/bash

if [ "$1" == "" ]; then
echo "no input file provided";
return;
fi

INFILE=$1

#the tag is determined from the input file name
TAG=`echo $INFILE | sed 's/.txt//'`
echo "$TAG.mod"
#return;


rm -f ./${TAG}.mod

while read line; do
RUN=`echo $line | awk -F" " '{print $1}'`
LUM=`echo $line | awk -F" " '{print $2}'`
IOV=`echo $line | awk -F" " '{print $3}'`

## get the module list
COMMAND="getPayloadData.py  --plugin pluginSiPixelQuality_PayloadInspector --plot plot_SiPixelQualityBadRocsSummary --tag $TAG --time_type Lumi --iovs '{\"start_iov\": \"$IOV\", \"end_iov\": \"$IOV\"}' --db Prod"
eval $COMMAND >  "${TAG}.tmp"

LIST=`cat ${TAG}.tmp | grep detId | awk -F" " '{print $2}' | tr '\n' ' '` 

echo "$RUN $LUM $IOV $LIST" >>  "${TAG}.mod"

/bin/rm -f ./*.png 

done < $INFILE

