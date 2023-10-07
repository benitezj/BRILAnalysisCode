#!/bin/bash

START_RUN=355862
LAST_RUN=362760
TEMP_FILE=$(mktemp)

for RUN in $(seq $START_RUN $LAST_RUN); do
    brilcalc lumi -c offline --datatag 23v1 --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_pcc.json -u /fb --byls --output-style csv --type pxl -b "STABLE BEAMS" -r $RUN >> $TEMP_FILE
done

awk -F, 'NR>1 && $1 !~ /#/ {delivered+=$6; recorded+=$7} END {print "Delivered:", delivered, "Recorded:", recorded}' $TEMP_FILE

rm -f $TEMP_FILE


