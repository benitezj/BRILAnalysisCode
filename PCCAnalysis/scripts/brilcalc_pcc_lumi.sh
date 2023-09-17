#!/bin/bash                                                                                                                                                                                                                          

OUTPUT="combined_lumi.csv"
TEMP_OUTPUT="temp_lumi.csv"

echo -e "Run    LumiSection    Luminosity" > $OUTPUT

for RUN in $(seq 355862 362760); do
#for RUN in $(seq 355862 355868); do                                                                                                                                                                                                 
    brilcalc lumi -c offline --datatag 23v1 --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_pcc.json -u /fb --byls --output-style csv --type pxl -r $RUN | \
    awk -F, 'NR>1 {split($1,run,":"); split($2,ls,":"); print run[1]"    "ls[1]"    "$6}' >> $TEMP_OUTPUT
done

sed -e '/Run    LumiSection    Luminosity/d' \
    -e '/ls    delivered(\/fb)/d' \
    -e '/nrun    totrecorded(\/fb)/d' \
    -e '/^,$/d' \
    -e '/^$/d' \
    -e '/^#/d' $TEMP_OUTPUT >> $OUTPUT

rm $TEMP_OUTPUT
