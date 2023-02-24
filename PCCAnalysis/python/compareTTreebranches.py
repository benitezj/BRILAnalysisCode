import ROOT

# Open the first file and get the list of TTree names
file1 = ROOT.TFile.Open("root://cms-xrd-global.cern.ch//store/data/Run2018B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/60000/980BE645-E9A7-7145-B26D-5459E678C3F0.root", "READ")
trees1 = [key.GetName() for key in file1.GetListOfKeys() if key.GetClassName() == "TTree"]

# Open the second file and get the list of TTree names
file2 = ROOT.TFile.Open("root://cms-xrd-global.cern.ch//store/data/Run2018B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/2520000/288996FE-3259-0945-8BEA-FD6F56B348CB.root", "READ")
trees2 = [key.GetName() for key in file2.GetListOfKeys() if key.GetClassName() == "TTree"]

# Print the common TTree names between the two files
print("Common TTree names:")
for tree in set(trees1).intersection(trees2):
    print(tree)

# Print the TTree names in file1 that are not in file2
print("TTree names in file1 that are not in file2:")
for tree in set(trees1).difference(trees2):
    print(tree)

# Print the TTree names in file2 that are not in file1
print("TTree names in file2 that are not in file1:")
for tree in set(trees2).difference(trees1):
    print(tree)


# Close the files
file1.Close()
file2.Close()
