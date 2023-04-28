import os
import sys
import ROOT

# Define the path to the dataset on CMS DAS                                                                                                                                                                         
dataset = "/AlCaLumiPixels/Run2018A-AlCaPCCRandom-15Apr2023_UL2018_315252_316062_PCCRandom_-v1/ALCARECO"

# Set up the DAS query to get a list of file paths                                                                                                                                                                  
query = "file dataset={0}".format(dataset)
das_output = os.popen("dasgoclient -query='{0}'".format(query)).read()
file_paths = das_output.strip().split("\n")

# Loop over the files and count the number of lumi sections                                                                                                                                                         
for file_path in file_paths:
    file_path = "root://cms-xrd-global.cern.ch//" + file_path
    # Open the file and check if it was successfully opened                                                                                                                                                         
    root_file = ROOT.TFile.Open(file_path)
    if not root_file or root_file.IsZombie():
        print("Error: failed to open file {0}".format(file_path))
        continue

    # Access the LuminosityBlocks TTree and count the number of entries                                                                                                                                             
    lumi_tree = root_file.Get("LuminosityBlocks")
    if not lumi_tree:
        print("Error: failed to access LuminosityBlocks tree in file {0}".format(file_path))
        continue

    n_lumis = lumi_tree.GetEntries()
    print("File {0} has {1} lumi sections".format(file_path, n_lumis))

    # Close the ROOT file                                                                                                                                                                                           
    root_file.Close()
