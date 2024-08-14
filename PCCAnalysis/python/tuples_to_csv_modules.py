import  ROOT
from ROOT import TFile, TTree, TChain
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, csv
import math 
import struct
import argparse

numMods = 1856
inputmodsfile='/afs/cern.ch/user/b/benitezj/public/BRIL/PCC/modules.txt'
output_path = '.'

parser = argparse.ArgumentParser(description='Process entries in event-based trees to produce pixel cluster counts')             
parser.add_argument('--inputfile', type=str, default="", help='The pccfile to input (pixel clusters and vertices)')                            

args = parser.parse_args()                                               


################# open file #################
tree=TChain("lumi/tree")                                                                                                                        
with open(args.inputfile) as f:                                                                                                                 
    for line in f:                                                                                                                              
        if ".root" in line:                                                                                                                     
            print(line)                                                                                                                         
            tree.Add(line.rstrip('\n'))                                                                                                        
                
tree.SetBranchStatus("*",0)                                                                                                                      
tree.SetBranchStatus("run",1)                                                                                                                    
tree.SetBranchStatus("LS",1)                                                                                                                     
#tree.SetBranchStatus("LN",1)                                                                                                                     
tree.SetBranchStatus("event",1)        
tree.SetBranchStatus("bunchCrossing",1)                                                                                                          
tree.SetBranchStatus("nPixelClusters",1)
tree.SetBranchStatus("timeStamp_begin",1)                                                                                                   


################ open outputfile #####################
if not os.path.exists(output_path):
    print("output path does not exist")
    sys.exit()

of = open(output_path+'/pcc_mod.csv', "a")

#######################################################
## get the Pixel detector module list
print("opening:"+inputmodsfile)
modf = open(inputmodsfile, "r")
DEFAULTMODS={}
#for m in range(numMods):
while True:
    line=modf.readline()
    if not line:
        break
    modid=int(line)
    if modid>0:
        DEFAULTMODS[modid]=0
print(f'Number of modules: {len(DEFAULTMODS):d}')

##############################################################
################## Loop over events ###########################                                                                            
nentries = tree.GetEntries()
time_c=0
event_count=0
LS_prev=0
ROOTMODS={}
for iev in range(nentries):
    tree.GetEntry(iev)
    if iev%10000==0:
        print("event: ",tree.run,tree.LS,iev)                                                                                            
    if tree.LS == -99:                                                                                                        
        continue      
    if tree.nPixelClusters.size()==0:
        continue

    event_count+=1
    time_c+=tree.timeStamp_begin

    # integrate the module counts
    for item in tree.nPixelClusters:                                                                                
        module=int(item[0][1])
        ROOTMODS[module]=1
        DEFAULTMODS[module]+=int(item[1])
        
    #if this event is the begginning of next LS then write the module counts to output
    if (LS_prev!=tree.LS or iev==nentries-1) and event_count>1000: 
        if iev==nentries-1: 
            print("last entry")
        
        of.write(f'{tree.run:d},{LS_prev:d},{time_c/event_count:.1f}')
        for key in DEFAULTMODS:
            of.write(f',{DEFAULTMODS[key]/event_count:.6f}')
            DEFAULTMODS[key]=0
        of.write("\n")
        time_c=0
        event_count=0
        
    LS_prev=tree.LS

of.close()



##verify the module list
print(f'{inputmodsfile}: number of modules {len(DEFAULTMODS):d}')
DEFAULTMODSKeys = list(DEFAULTMODS.keys())
DEFAULTMODSKeys.sort()
DEFAULTMODS_sorted= {i: DEFAULTMODS[i] for i in DEFAULTMODSKeys}

print(f'root file  : number of modules {len(ROOTMODS):d}')
ROOTMODSKeys = list(ROOTMODS.keys())
ROOTMODSKeys.sort()
ROOTMODS_sorted= {i: ROOTMODS[i] for i in ROOTMODSKeys}

modsfile = open(output_path+'/modules_not_in_root.txt', "a")
for key in DEFAULTMODS_sorted:
    if key not in ROOTMODS_sorted:
        print(f'{key:d} not in root file')
    modsfile.write(f'{key:d}\n')
modsfile.close()

modsrootfile = open(output_path+'/modules_not_in_default.txt', "a")
for key in ROOTMODS_sorted:
    if key not in DEFAULTMODS_sorted:
        print(f'{key:d} not in {inputmodsfile}')
    modsrootfile.write(f'{key:d}\n')
modsrootfile.close()

        
print("Done")
