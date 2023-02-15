#!/bin/bash


# Set the EOS area path and the target directory for the output files
EOS_PATH="/eos/cms/store/data/Run2018B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/2520000/"
OUTPUT_DIR="/afs/cern.ch/user/a/asehrawa/PCC_27Oct2022_UL2018/CMSSW_10_6_30/src/zerobias_27Oct2022_Run2018B"

# Find all root files in the EOS area and loop over them
find "$EOS_PATH" -name "*.root" -type f | while read root_file; do
    # Extract the filename without the path or extension
    filename=$(basename "$root_file" .root)

    # Create a text file with the filename in the output directory
    touch "$OUTPUT_DIR/$filename.txt"

    # Write the root file path to the text file
    echo "$root_file" >> "$OUTPUT_DIR/$filename.txt"

    # Copy the root file to the folder
    cp "$root_file" "$OUTPUT_DIR/$filename"
done
