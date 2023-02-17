#!/bin/bash                                                                                                                                                        

# Set the path to the directory containing the root files                                                                                                          
DIRECTORY="/store/test/xrootd/T1_US_FNAL/store/data/Run2018B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/2530000/"

# Use the "xrdfs" command to list the files in the directory                                                                                                       
# and filter the output to show only the files ending with ".root"                                                                                                 
FILES=$(xrdfs root://cmsxrootd.fnal.gov/ ls -l $DIRECTORY | awk '{print $5, $9}' | grep -E '\.root$' | awk '{print $2}')

# Loop through the files and print their names                                                                                                                     
for FILE in $FILES
do
    echo $FILE
done
