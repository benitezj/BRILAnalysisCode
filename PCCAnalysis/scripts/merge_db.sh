#!/bin/bash                                              
# Define input database directory
#db_dir="/eos/user/a/asehrawa/PCC_newafterglowparameters_23May2023/AlCaPCCRandom/Run2018D/"
#db_dir=/eos/user/b/benitezj/BRIL/PCC/28Aug24_UL2017_PCCZeroBias/Random_v4/Run2017B
db_dir=$1

#dest_db="/afs/cern.ch/user/a/asehrawa/PCC_25May2023/CMSSW_10_2_2/src/Run2018_afterglow/Run2018D/dest.db"
dest_db=${db_dir}/merged.db

echo $dest_db

for db_file in "${db_dir}/"*.db; do
    if [[ -f $db_file ]]; then
	yes | conddb --db "${db_file}" copy --destdb "${dest_db}" TestCorrections
	#echo "${db_file}"
    fi
done
