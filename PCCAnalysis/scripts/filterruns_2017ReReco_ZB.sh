#!/bin/bash

RUNLIST=$1 # file with list of runs in the dataset
EOS_PATH=$2 # path to the csv files of the entire dataset

echo $RUNLIST
echo $EOS_PATH

OUTPUT_DIR=${EOS_PATH}_runs
echo $OUTPUT_DIR
mkdir $OUTPUT_DIR

for run in `cat $RUNLIST`;do
    echo $run
    rm -f $OUTPUT_DIR/${run}.csv
    find "$EOS_PATH/" -name "*.csv" -type f | while read file; do
	cat $file | grep "${run}," | grep -v ".${run}" >> $OUTPUT_DIR/${run}.csv
    done
done

