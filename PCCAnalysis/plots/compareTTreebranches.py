import ROOT

# Open the first file
file1 = ROOT.TFile.Open("root://cms-xrd-global.cern.ch//store/data/Run2018B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/60000/980BE645-E9A7-7145-B26D-5459E678C3F0.root")

# Get the list of branch names for file1
branches_file1 = []
for key in file1.GetListOfKeys():
    if key.GetClassName() == "TTree":
        tree = key.ReadObj()
        for branch in tree.GetListOfBranches():
            branches_file1.append(branch.GetName())

# Open the second file
file2 = ROOT.TFile.Open("file2.root")

# Get the list of branch names for file2
branches_file2 = []
for key in file2.GetListOfKeys():
    if key.GetClassName() == "TTree":
        tree = key.ReadObj()
        for branch in tree.GetListOfBranches():
            branches_file2.append(branch.GetName())

# Compare the lists of branch names
if len(branches_file1) != len(branches_file2):
    print("The two files have different numbers of branches")
else:
    for i in range(len(branches_file1)):
        if branches_file1[i] != branches_file2[i]:
            print("The two files have different branch names")
            break
    else:
        print("The two files have the same branch names")
