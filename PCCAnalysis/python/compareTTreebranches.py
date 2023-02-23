import ROOT

# Open the first file and get the list of TTree names
file1 = ROOT.TFile.Open("file1.root", "READ")
trees1 = [key.GetName() for key in file1.GetListOfKeys() if key.GetClassName() == "TTree"]

# Open the second file and get the list of TTree names
file2 = ROOT.TFile.Open("file2.root", "READ")
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
