#!/bin/bash

# Args
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 file1.txt file2.txt"
    exit 1
fi

FILE1="$1"
FILE2="$2"
OUTFILE="Veto_Overlap.txt"

# Sort and Intersections
comm -12 <(sort -u "$FILE1") <(sort -u "$FILE2") > "$OUTFILE"

echo "Outfile in: $OUTFILE"
echo "Number of common elements: $(wc -l < "$OUTFILE")"
