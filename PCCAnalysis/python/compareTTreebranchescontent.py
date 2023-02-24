import ROOT

# Open the first file
file1 = ROOT.TFile.Open("root://cms-xrd-global.cern.ch//store/data/Run2018B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/60000/980BE645-E9A7-7145-B26D-5459E678C3F0.root")

# Open the second file
file2 = ROOT.TFile.Open("root://cms-xrd-global.cern.ch//store/data/Run2018B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/2520000/288996FE-3259-0945-8BEA-FD6F56B348CB.root")

# Get a list of the TTrees in each file
trees1 = [key.GetName() for key in file1.GetListOfKeys() if key.GetClassName() == "TTree"]
trees2 = [key.GetName() for key in file2.GetListOfKeys() if key.GetClassName() == "TTree"]

# Loop over the common TTrees in the two files
common_trees = list(set(trees1).intersection(trees2))
for tree_name in common_trees:

    # Open the TTree in the first file
    tree1 = file1.Get(tree_name)

    # Open the TTree in the second file
    tree2 = file2.Get(tree_name)

    # Get a list of the branches in each TTree
    branches1 = [branch.GetName() for branch in tree1.GetListOfBranches()]
    branches2 = [branch.GetName() for branch in tree2.GetListOfBranches()]

    # Loop over the common branches in the two TTrees
    common_branches = list(set(branches1).intersection(branches2))
    for branch_name in common_branches:

        # Get the branch in the first TTree
        branch1 = tree1.GetBranch(branch_name)

        # Get the branch in the second TTree
        branch2 = tree2.GetBranch(branch_name)

        # Compare the number of entries in the two branches
        if branch1.GetEntries() != branch2.GetEntries():
            print("Number of entries in branch {} of tree {} differs: {} vs {}".format(branch_name, tree_name, branch1.GetEntries(), branch2.GetEntries()))
            continue

        # Compare the content of the two branches
        for i in range(branch1.GetEntries()):
            branch1.GetEntry(i)
            branch2.GetEntry(i)
            if branch1.GetValue(i, branch1.GetEntries()) != branch2.GetValue(i, branch1.GetEntries()):
                print("Content of branch {} in tree {} differs at entry {}".format(branch_name, tree_name, i))
                break

# Close the files
file1.Close()
file2.Close()
