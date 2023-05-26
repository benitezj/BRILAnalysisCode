import ROOT
from ROOT import TFile, TTree, TChain
import sys,os
import numpy
import array
import math
import argparse

parser = argparse.ArgumentParser(description='Process entries in event-based trees to produce pixel cluster counts')
parser.add_argument('--pccfile', type=str, default="", help='The pccfile to input (pixel clusters and vertices)')
parser.add_argument('--inputfile', type=str, default="", help='The pccfile to input (pixel clusters and vertices)')
parser.add_argument('--label', type=str, default="", help="Label for output file")
parser.add_argument('--vetoModules', type=str, default="", help="Text file containing list of pixel modules to veto (default: vetoModules.txt)")
parser.add_argument('--outputDir', type=str, default="pccVdmMiniTree.root", help="Directory to save file in (default:  local path)")

args = parser.parse_args()

tree=TChain("lumi/tree")
with open(args.inputfile) as f:
    for line in f:
        if ".root" in line:
            print(line)
            tree.Add(line.rstrip('\n'))

#tree.Print()
print 'tree entries: '
print tree.GetEntries()
#sys.exit(0);

tree.SetBranchStatus("*",0)
tree.SetBranchStatus("run",1)
tree.SetBranchStatus("LS",1)
tree.SetBranchStatus("LN",1)
tree.SetBranchStatus("event",1)
tree.SetBranchStatus("nPixelClusters",1)
tree.SetBranchStatus("timeStamp_begin",1)
tree.SetBranchStatus("nPixelClusters.first.first",1)
tree.SetBranchStatus("nPixelClusters.first.second",1)
tree.SetBranchStatus("nPixelClusters.second",1)

#######################
# Make mod veto list  #
#######################
vetoModules=[]

if os.path.isfile(args.vetoModules):
    vetoFile=open(args.vetoModules)
    lines = vetoFile.readlines()
    for line in lines:
        for mod in line.split(","):
            try:
                vetoModules.append(int(mod))
            except:
                print "Error reading: ",mod,"in",line

else:
    print "Veto list does not exist... not vetoing anything."

print vetoModules


#######################
# Setup new mini-tree #
#######################
#newfilename=filename.split("/")[-1].split(".")[0]+"_"+args.label+".root"

if args.outputDir != "":
    if args.outputDir.find("/store")==0:
        args.outputDir="root://eoscms//eos/cms"+args.outputDir
    newfilename=args.outputDir#+"/"+newfilename

newfile=ROOT.TFile.Open(newfilename,"recreate")
newtree=ROOT.TTree("pccminitree","pcc vdm scan data")

run             = array.array( 'l', [ 0 ] )
LS              = array.array( 'l', [ 0 ] )
LN              = array.array( 'l', [ 0 ] )
event           = array.array( 'l', [ 0 ] )
timeStamp       = array.array( 'l', [ 0 ] )
bunchCrossing   = array.array( 'l', [ 0 ] )
nCluster        = array.array( 'd', [ 0 ] )
nClusterPerLayer= array.array( 'd', 6*[ 0 ] )


newtree.Branch("run",run,"run/I")
newtree.Branch("LS",LS,"LS/I")
newtree.Branch("LN",LN,"LN/I")
newtree.Branch("event",event,"event/i")
newtree.Branch("timeStamp",timeStamp,"timeStamp/i")
newtree.Branch("BXid",bunchCrossing,"BXid/I")
newtree.Branch("nCluster",nCluster,"nCluster/D")

#######################
#  Loop over events   #
#######################
nentries=tree.GetEntries()

print nentries
maxNBX=0
a = set()

for iev in range(nentries):
    tree.GetEntry(iev)   
    if iev%30000==0:
        print (tree.run,tree.LS,iev)
    pixelCount=[0]*26
    if tree.nPixelClusters.size() == 0:
        bxid=0
    for item in tree.nPixelClusters:
        #print(type(tree.nPixelClusters))
        bxid=item[0][0]    #item.first.first 
        module=item[0][1]   #item.first.second
        clusters= item[1] #item.second
        print(bxid,module,clusters)
        if module in vetoModules or module/1000000==303:#when starts in 303 is  barrel layer 0
            continue
        a.add(module)
        pixelCount[0]=pixelCount[0]+clusters
    
    print(bxid)   
    run[0]=tree.run
    LS[0]=tree.LS #LS = 23s
    LN[0]=tree.LN #LN=0.3s
    event[0]=tree.event
    timeStamp[0]=tree.timeStamp_begin
    bunchCrossing[0]= bxid
    nCluster[0]=pixelCount[0]
    newtree.Fill()

print a

newfile.Write()
newfile.Close()
