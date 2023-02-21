#!/bin/bash                                                                                                                                            

#To create the input file, run the following command in terminal for period A,B and C
#dasgoclient -query="file dataset=/AlCaLumiPixels/Run2018A-AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/ALCARECO instance=prod/global | grep file.name | grep '.root'" 
#dasgoclient -query="file dataset=/AlCaLumiPixels/Run2018B-AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/ALCARECO instance=prod/global | grep file.name | grep '.root'" 
#dasgoclient -query="file dataset=/AlCaLumiPixels/Run2018C-AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/ALCARECO instance=prod/global | grep file.name | grep '.root'" 

echo "Please select the data period: "
echo "1. Period A (Run2018A)"
echo "2. Period B (Run2018B)"
echo "3. Period C (Run2018C)"

read period_choice

case $period_choice in
    1)
        input_file="Run2018A.txt"
        ;;
    2)
        input_file="Run2018B.txt"
        ;;
    3)
        input_file="Run2018C.txt"
        ;;
    *)
        echo "Invalid choice. Please enter 1, 2, or 3."
        exit 1
        ;;
esac

echo "Selected input file: $input_file"

# Get the input file name from user                                                                                                                    
#echo "Enter the name of the input file:"
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
