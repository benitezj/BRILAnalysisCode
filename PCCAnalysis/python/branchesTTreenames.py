import ROOT

# Open the file
file = ROOT.TFile.Open("root://cms-xrd-global.cern.ch//store/data/Run2018B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/60000/980BE645-E9A7-7145-B26D-5459E678C3F0.root")

# Get the list of trees in the file
trees = file.GetListOfKeys()

# Loop over the trees
for i in range(trees.GetEntries()):
    # Get the TTree object
    tree = file.Get(trees.At(i).GetName())

    # Get the list of branch names
    branches = tree.GetListOfBranches()

    # Print the tree name
    print("Tree:", tree.GetName())

    # Loop over the branches and print their names
    for j in range(branches.GetEntries()):
        branch = branches.At(j)
        print(branch.GetName())
