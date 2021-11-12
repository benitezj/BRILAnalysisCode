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


f_LHC = 11245.6
t_LS=math.pow(2,18)/f_LHC
xsec_mb=80000. #microbarn


def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        return False


weightThreshold=1e-5

def AverageWithWeight(list):
    sumValue=0
    sumWeight=0
    for value,weight in list:
        sumValue=sumValue+value
        sumWeight=sumWeight+weight

    if sumWeight>0:
        return float(sumValue)/sumWeight

def GetWeightedValues(list):
    count=0
    sumOfWeights=0
    sumOfWeights2=0
    weightedSum=0

    for value,weight in list:
        #print value,weight
        if weight<weightThreshold:
            continue
        count=count+1
        sumOfWeights=sumOfWeights+weight
        sumOfWeights2=sumOfWeights2+math.pow(weight,2)
        weightedSum=weightedSum+weight*value

    return count,sumOfWeights,sumOfWeights2,weightedSum


def GetMean(list):
    #print "list length",len(list)
    count,sumOfWeights,sumOfWeights2,weightedSum=GetWeightedValues(list)
    mean=GetMeanFromWeightedValues(sumOfWeights,weightedSum)
    return mean


def GetMeanFromWeightedValues(sumOfWeights,weightedSum):
    mean=0
    if sumOfWeights>0:
        mean=weightedSum/sumOfWeights
    return mean


def GetMeanAndMeanError(list):
    count,sumOfWeights,sumOfWeights2,weightedSum=GetWeightedValues(list)
    if sumOfWeights2==0:
        return -99,-99
    neff=math.pow(sumOfWeights,2)/sumOfWeights
    mean=GetMeanFromWeightedValues(sumOfWeights,weightedSum)

    #print neff,count,sumOfWeights
    
    weightedSumDiffFromAve2=0
    for value,weight in list:
        if weight<weightThreshold:
            continue
        weightedSumDiffFromAve2=weightedSumDiffFromAve2+weight*math.pow(value-mean,2) 

    stddev=0
    meanError=0
    if count>2:
        stddev=math.sqrt( weightedSumDiffFromAve2 / (sumOfWeights))
        meanError=stddev/math.sqrt(neff)

    #print "stddev",stddev

    return mean,meanError



#######################
#  Setup PCC Ntuples  #
#######################
#if args.pccfile=="":
#    print "pccfile is not given"
#    sys.exit(1)
#
#filename=args.pccfile
#
##if filename.find("/store")==0: # file is in eos
##    filename="/eos/cms"+filename
#
#try:
#    tfile=ROOT.TFile.Open(filename)
#except:
#    print filename,"failed to open properly"
#    sys.exit(1)
#
#tree=tfile.Get("lumi/tree")


tree=TChain("lumi/tree")
with open(args.inputfile) as f:
    for line in f:
        if ".root" in line:
            #print(line)
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
tree.SetBranchStatus("layer*",1)
tree.SetBranchStatus("bunchCrossing",1)
tree.SetBranchStatus("timeStamp_begin",1)
tree.SetBranchStatus("timeStamp_end",1)

###
#Vtx Stuff
###
tree.SetBranchStatus("nVtx",1)
tree.SetBranchStatus("vtx_isGood",1)
tree.SetBranchStatus("vtx_isFake",1)
tree.SetBranchStatus("vtx_nTrk",1)

tree.SetBranchStatus("vtx_isValid",1)
tree.SetBranchStatus("vtx_x",1)
tree.SetBranchStatus("vtx_y",1)
tree.SetBranchStatus("vtx_z",1)
tree.SetBranchStatus("vtx_xError",1)
tree.SetBranchStatus("vtx_yError",1)

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
#timeStamp       = array.array( 'l', [ 0 ] )
timeStamp_begin = array.array( 'l', [ 0 ] )
timeStamp_end   = array.array( 'l', [ 0 ] )
#BXid            = array.array( 'l', [ 0 ] )
bunchCrossing   = array.array( 'l', [ 0 ] )
nCluster        = array.array( 'd', [ 0 ] )
nClusterPerLayer= array.array( 'd', 6*[ 0 ] )

nVtx            = array.array('d',[0])
vtx_isGood      = array.array('l',[0])
vtx_isFake      = array.array('l',[0])
vtx_nTrk        = array.array('l',[0])
vtx_isValid     = array.array( 'l', [ 0 ] )
vtx_x           = array.array( 'd', [ 0 ] )
vtx_y           = array.array( 'd', [ 0 ] )
vtx_z           = array.array( 'd', [ 0 ] )
vtx_xError      = array.array( 'd', [ 0 ] )
vtx_yError      = array.array( 'd', [ 0 ] )


newtree.Branch("run",run,"run/I")
newtree.Branch("LS",LS,"LS/I")
newtree.Branch("LN",LN,"LN/I")
newtree.Branch("event",event,"event/i")
newtree.Branch("timeStamp",timeStamp_begin,"timeStamp/i")
#newtree.Branch("timeStamp_begin",timeStamp_begin,"timeStamp_begin/i")
#newtree.Branch("timeStamp_end",timeStamp_end,"timeStamp_end/i")
newtree.Branch("BXid",bunchCrossing,"BXid/I")
#newtree.Branch("bunchCrossing",bunchCrossing,"bunchCrossing/I")
newtree.Branch("nCluster",nCluster,"nCluster/D")
newtree.Branch("nClusterPerLayer",nClusterPerLayer,"nClusterPerLayer[5]/D")
newtree.Branch("nVtx",nVtx,"nVtx/D")
newtree.Branch("vtx_isGood",vtx_isGood,"vtx_isGood/I")
newtree.Branch("vtx_isFake",vtx_isFake,"vtx_isFake/I")
newtree.Branch("vtx_nTrk",vtx_nTrk,"vtx_nTrk/I")
newtree.Branch("vtx_isValid",vtx_isValid,"vtx_isValid/I")
newtree.Branch("vtx_x",vtx_x,"vtx_x/D")
newtree.Branch("vtx_y",vtx_y,"vtx_y/D")
newtree.Branch("vtx_z",vtx_z,"vtx_z/D")
newtree.Branch("vtx_xError",vtx_xError,"vtx_xError/D")
newtree.Branch("vtx_yError",vtx_yError,"vtx_yError/D")

#######################
#  Loop over events   #
#######################
nentries=tree.GetEntries()/100#00

print nentries
maxNBX=0
a = set()
for iev in range(nentries):
   tree.GetEntry(iev)
   if (True):#(tree.timeStamp_begin>=1530412500 and tree.timeStamp_end<=1530412800) or (tree.timeStamp_begin>=1530427080 and tree.timeStamp_end<=1530427440)):
    if iev%30000==0:
        print "iev,",iev
        print "(tree.run,tree.LS)",tree.LS
   
    pixelCount=[0]*26

    for item in tree.nPixelClusters:
        bxid=item[0][0]
        module=item[0][1]
        layer=tree.layers[module]
        clusters=item[1]
        #print module
        if module in vetoModules or module/1000000==303:
            #print module
            continue
        a.add(module)
        pixelCount[0]=pixelCount[0]+clusters

    nVtx[0]=0
    for i in range(0,tree.nVtx):
        #print tree.vtx_isGood[i], tree.nVtx
        if tree.vtx_isGood[i]==True and tree.vtx_isFake[i]==False:# and tree.vtx_nTrk[i]>14:
            nVtx[0]=nVtx[0]+1	
            vtx_isGood[0]=tree.vtx_isGood[i]
            vtx_isValid[0]=tree.vtx_isValid[i]
            vtx_isFake[0]=tree.vtx_isFake[i]
            vtx_nTrk[0]=tree.vtx_nTrk[i]
            vtx_x[0]=tree.vtx_x[i]
            vtx_y[0]=tree.vtx_y[i]
            vtx_z[0]=tree.vtx_z[i]
            vtx_xError[0]=tree.vtx_xError[i]
            vtx_yError[0]=tree.vtx_yError[i]
    
    run[0]=tree.run
    LS[0]=tree.LS
    LN[0]=tree.LN
    event[0]=tree.event
    #timeStamp[0]=tree.timeStamp_begin
    timeStamp_begin[0]=tree.timeStamp_begin
    timeStamp_end[0]=tree.timeStamp_end
    #BXid[0]=tree.bunchCrossing
    bunchCrossing[0]=bxid#tree.bunchCrossing
    nCluster[0]=pixelCount[0]
    #for layer in range(1,6):
    #    nClusterPerLayer[layer-1]=pixelCount[layer]

    newtree.Fill()

print a

newfile.Write()
newfile.Close()

