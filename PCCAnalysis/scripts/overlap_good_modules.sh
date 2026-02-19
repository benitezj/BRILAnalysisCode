#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 total_modules.txt veto1.txt veto2.txt"
    exit 1
fi

TOTAL="$1"
VETO1="$2"
VETO2="$3"

GOODveto1="good_veto1.txt"
GOODveto2="good_veto2.txt"
OVERLAP="good_overlap_veto1_veto2.txt"

# Sort lists
sort -u "$TOTAL" > total_sorted.txt
sort -u "$VETO1" > veto1_sorted.txt
sort -u "$VETO2" > veto2_sorted.txt

# Good modules = total - veto
comm -23 total_sorted.txt veto1_sorted.txt > "$GOODveto1"
comm -23 total_sorted.txt veto2_sorted.txt > "$GOODveto2"

# Overlap of good modules
comm -12 <(sort "$GOODveto1") <(sort "$GOODveto2") > "$OVERLAP"

# Counts
N_TOTAL=$(wc -l < total_sorted.txt)
N_GOODveto1=$(wc -l < "$GOODveto1")
N_GOODveto2=$(wc -l < "$GOODveto2")
N_OVERLAP=$(wc -l < "$OVERLAP")

echo "================ SUMMARY ================"
echo "Total modules: $N_TOTAL"
echo ""
echo "Good modules veto1: $N_GOODveto1"
echo "Good modules veto2: $N_GOODveto2"
echo "Common good modules (overlap): $N_OVERLAP"
echo ""
echo "Files created:"
echo "  $GOODveto1"
echo "  $GOODveto2"
echo "  $OVERLAP"
