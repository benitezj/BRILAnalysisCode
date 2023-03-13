#!/bin/bash                                                                                                                                                                                                   

export LC_ALL=C
export LANG=en_US.UTF-8

# Define the two datasets to compare                                                                                                                                                                          
dataset1="/AlCaLumiPixels/Run2018A-AlCaPCCZeroBias-PromptReco-v3/ALCARECO"
dataset2="/AlCaLumiPixels/Run2018C-AlCaPCCZeroBias-PromptReco-v3/ALCARECO"
dataset3="/AlCaLumiPixels/Run2018D-AlCaPCCZeroBias-PromptReco-v2/ALCARECO"
dataset4="/AlCaLumiPixels/Run2018A-AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/ALCARECO"
dataset5="/AlCaLumiPixels/Run2018B-AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/ALCARECO"
dataset6="/AlCaLumiPixels/Run2018C-AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/ALCARECO"


# Function to print information for a given dataset                                                                                                                                                           
print_info() {
  dataset=$1
  echo "Information for dataset: $dataset"
  dataset_info=$(dasgoclient -query="dataset=$dataset" -json | grep -E 'size|nevents|num_lumi' | sed 's/,$//')
  size=$(echo $dataset_info | jq -r '.dataset[0].size')
  nevents=$(echo $dataset_info | jq -r '.dataset[0].nevents')
  num_lumi=$(echo $dataset_info | jq -r '.dataset[0].num_lumi')
  size_per_event=$(echo "scale=2; $size / $nevents" | bc)
  size_per_lumi=$(echo "scale=2; $size / $num_lumi" | bc)

  echo "Number of events: $nevents"
  echo "Dataset size: $size bytes"
  echo "Number of lumi sections: $num_lumi"
  echo "Dataset size per event: $size_per_event bytes"
  echo "Dataset size per lumi section: $size_per_lumi bytes"
}

# Call the function for both datasets                                                                                                                                                                         
print_info $dataset1
print_info $dataset2
print_info $dataset3
print_info $dataset4
print_info $dataset5
print_info $dataset6
