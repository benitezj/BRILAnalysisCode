#!/bin/bash                                                                                                                                                                                                     

BASE_DIR="/afs/cern.ch/user/a/asehrawa/reprocessing_PCC_ZB_data_27May2023/CMSSW_10_6_30/src"
OUTPUT="$BASE_DIR/combined_lumi.csv"
TEMP_DIR="$BASE_DIR/temp_lumi_dir"

                                                                                                                                                            
mkdir -p $TEMP_DIR
rm -rf $TEMP_DIR/*

echo -e "Run    LumiSection    Luminosity" > $OUTPUT

LAST_RUN=362760

process_runs_chunk() {
    local START=$1
    local END=$2
    local TEMP_OUTPUT="$TEMP_DIR/temp_lumi_$START-$END.csv"

    for RUN in $(seq $START $END); do
        echo "Processing RUN: $RUN"
        brilcalc lumi -c offline --datatag 23v1 --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_pcc.json -u /fb --byls --output-style csv --type pxl -r $RUN | \
        awk -F, 'NR>1 {split($1,run,":"); split($2,ls,":"); print run[1]"    "ls[1]"    "$6}' >> $TEMP_OUTPUT
    done
}

CHUNK_SIZE=500                                                                                                                                     
START=355862

while [ $START -le $LAST_RUN ]; do
    END=$((START + CHUNK_SIZE - 1))
    [ $END -gt $LAST_RUN ] && END=$LAST_RUN

    process_runs_chunk $START $END

    START=$((START + CHUNK_SIZE))
done

                                                                                                                                                         
for file in $TEMP_DIR/*.csv; do
    sed -e '/Run    LumiSection    Luminosity/d' \
        -e '/ls    delivered(\/fb)/d' \
        -e '/nrun    totrecorded(\/fb)/d' \
        -e '/^,$/d' \
        -e '/^$/d' \
        -e '/^#/d' $file >> $OUTPUT
done

                                                                                                                                                                                                       
rm -rf $TEMP_DIR

echo "Processed up to the last run number: $LAST_RUN. Exiting..."
