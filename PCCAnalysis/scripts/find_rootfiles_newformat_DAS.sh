#!/bin/bash                                                                                                                                            

#To create the input file, run the following command in terminal for period A,B and C
#dasgoclient -query="file dataset=/AlCaLumiPixels/Run2018A-AlCaPCCZeroBias-13Mar2023_UL2018_PCC-v1/ALCARECO instance=prod/global | grep file.name | grep '.root'" > Run2018A.txt 
#dasgoclient -query="file dataset=/AlCaLumiPixels/Run2018B-AlCaPCCZeroBias-13Mar2023_UL2018_PCC-v1/ALCARECO instance=prod/global | grep file.name | grep '.root'" > Run2018B.txt
#dasgoclient -query="file dataset=/AlCaLumiPixels/Run2018C-AlCaPCCZeroBias-13Mar2023_UL2018_PCC-v1/ALCARECO instance=prod/global | grep file.name | grep '.root'" > Run2018C.txt

# Get the input file name from user  Run2018A.txt, Run2018B.txt, Run2018C.txt                                                                                                                  
echo "Enter the name of the input file:"
read input_file

# Check if the input file exists                                                                                                                       
if [ ! -f $input_file ]; then
    echo "File $input_file not found."
    exit 1
fi

# Loop through each line in the input file                                                                                                             
while read line; do
    # Get the root file name                                                                                                                           
    root_file=$(basename $line)

    # Create a new text file with the root file name                                                                                                   
    echo $line > "${root_file%.*}.txt"

done < $input_file

echo "Separate text files created successfully."

# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookXrootdService
