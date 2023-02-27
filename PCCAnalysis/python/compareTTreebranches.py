import ROOT

def get_branches(ttree):
    branches = []
    for branch in ttree.GetListOfBranches():
        branches.append(branch.GetName())
    return branches

# Open the first file and get the list of trees and their branches
file1 = ROOT.TFile.Open("root://cms-xrd-global.cern.ch//store/data/Run2018C/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-PromptReco-v3/000/319/840/00000/B6F319F3-D78B-E811-9D84-FA163E96E589.root")
trees1 = file1.GetListOfKeys()
branches1 = {}
for tree in trees1:
    obj = tree.ReadObj()
    if obj.InheritsFrom("TTree"):
        branches1[obj.GetName()] = get_branches(obj)
        #print("Tree:", obj.GetName())
        #print("Branches:", branches1[obj.GetName()])

# Open the second file and get the list of trees and their branches
file2 = ROOT.TFile.Open("root://cms-xrd-global.cern.ch//store/data/Run2018C/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/70000/9FC6DA64-8572-E045-B90D-309D8D5753A3.root")
trees2 = file2.GetListOfKeys()
branches2 = {}
for tree in trees2:
    obj = tree.ReadObj()
    if obj.InheritsFrom("TTree"):
        branches2[obj.GetName()] = get_branches(obj)
        #print("Tree:", obj.GetName())
        #print("Branches:", branches2[obj.GetName()])

# Find the branches present in both files
common_branches = []
for tree in branches1:
    if tree in branches2:
        for branch in branches1[tree]:
            if branch in branches2[tree]:
                common_branches.append(branch)

# Find the branches present in the first file but not the second
unique_branches1 = []
for tree in branches1:
    if tree not in branches2:
        for branch in branches1[tree]:
            if branch not in unique_branches1:
                unique_branches1.append(branch)
    else:
        for branch in branches1[tree]:
            if branch not in branches2[tree] and branch not in unique_branches1:
                unique_branches1.append(branch)

# Find the branches present in the second file but not the first
unique_branches2 = []
for tree in branches2:
    if tree not in branches1:
        for branch in branches2[tree]:
            if branch not in unique_branches2:
                unique_branches2.append(branch)
    else:
        for branch in branches2[tree]:
            if branch not in branches1[tree] and branch not in unique_branches2:
                unique_branches2.append(branch)

# Print the results
print("Common branches:")
for branch in common_branches:
    print(branch)
print("Branches present in the first file but not the second:")
for branch in unique_branches1:
    print(branch)
print("Branches present in the second file but not the first:")
for branch in unique_branches2:
    print(branch)
