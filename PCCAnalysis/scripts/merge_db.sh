#!/bin/bash                                                                                                                                                                                                                   

# Define input database directory                                                                                                                                                                                             
db_dir="/eos/user/a/asehrawa/PCC_newafterglowparameters_23May2023/AlCaPCCRandom/Run2018D/"

# Define destination database file                                                                                                                                                                                            
dest_db="/afs/cern.ch/user/a/asehrawa/PCC_25May2023/CMSSW_10_2_2/src/Run2018_afterglow/Run2018D/dest.db"

# Loop through input database files in the directory and copy them to the destination database                                                                                                                                
for db_file in "$db_dir"*.db
do
  if [[ -f $db_file ]]; then # Ensure it's a file, not a directory                                                                                                                                                            
    yes | conddb --db "${db_file}" copy --destdb "${dest_db}" TestCorrections
  fi
done
