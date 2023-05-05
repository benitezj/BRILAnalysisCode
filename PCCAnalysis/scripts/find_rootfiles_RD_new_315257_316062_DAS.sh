#!/bin/bash                                                                                                                                                                                                  \
                                                                                                                                                                                                              

total_files=0

for run in 315257 315258 315259 315264 315265 315267 315270 315322 315339 315357 315361 315363 315365 315366 315420 315488 315489 315490 315506 315509 315510 315512 315543 315555 315556 315557 315640 31564\
1 315642 315644 315645 315646 315647 315648 315689 315690 315702 315703 315704 315705 315713 315721 315741 315764 315770 315784 315785 315786 315788 315789 315790 315800 315801 315840 315973 315974 316058 \
316059 316060 316061 316062
do
    echo "Files for run number $run..."
    file_list=$(dasgoclient -query="file dataset=/AlCaLumiPixels/Run2018A-AlCaPCCRandom-15Apr2023_UL2018_315252_316062_PCCRandom_-v1/ALCARECO run=$run")
    file_count=$(echo $file_list | wc -w)

    if [ $file_count -eq 0 ]
    then
        echo "No files found for run $run"
    else
        total_files=$((total_files + file_count))
        echo "Number of files for run $run: $file_count"
        echo $file_list | tr ' ' '\n'
        echo $file_list | sed "s|^|root://cms-xrd-global.cern.ch/|" > "${run}.txt"
    fi
done

echo "Total number of files: $total_files"
