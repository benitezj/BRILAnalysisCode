import ROOT

# Open the input file
input_file = ROOT.TFile.Open("root://cms-xrd-global.cern.ch//store/data/Run2018B/AlCaLumiPixels/ALCARECO/AlCaPCCZeroBias-27Oct2022_UL2018_PCC-v1/60000/980BE645-E9A7-7145-B26D-5459E678C3F0.root")

# Print the names of all the TTrees in the file
for key in input_file.GetListOfKeys():
    obj = key.ReadObj()
    if obj.InheritsFrom("TTree"):
        print("TTree name:", obj.GetName())

        # Print the names of all the branches in the TTree
        for branch in obj.GetListOfBranches():
            print("  Branch name:", branch.GetName())

        # Print the data stored in all the branches of the TTree
        tree = input_file.Get(obj.GetName())
        for event in tree:
            for branch in tree.GetListOfBranches():
                branch_name = branch.GetName()
                branch_value = getattr(event, branch_name)
                print("  {0}: {1}".format(branch_name, branch_value))

# Close the input file
input_file.Close()
