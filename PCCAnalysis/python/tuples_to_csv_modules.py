import  ROOT
from ROOT import TFile, TTree, TChain
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, csv
import math 
import struct
import argparse

numMods = 1856
inputmodsfile='BRILAnalysisCode/PCCAnalysis/plots/modules.txt'
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
#tree.SetBranchStatus("timeStamp_begin",1)                                                                                                   



#######################################################
## get the Pixel detector module list
print("opening:"+inputmodsfile)
modf = open(inputmodsfile, "r")
DEFAULTMODS={}
while True:
    line=modf.readline()
    if not line:
        break
    modid=int(line)
    if modid>0:
        DEFAULTMODS[modid]=0
print(f'Number of modules in module file: {len(DEFAULTMODS):d}, number expected: {numMods}')

#check the order of the keys is the same as in the input file
modfile_tmp = open('./modules_tmp.txt', "a")
for key in DEFAULTMODS:
    modfile_tmp.write(f'{key:d}\n')
modfile_tmp.close()
print("Module order diff check:")
print(os.system("diff "+inputmodsfile+" ./modules_tmp.txt")) 


################ open outputfile #####################
if not os.path.exists(output_path):
    print("output path does not exist")
    sys.exit()

of = open(output_path+'/tuples_to_csv_modules.csv', "a")

##############################################################
################## Loop over events ###########################                                                                            
nentries = tree.GetEntries()
time_c=0
event_count=0
LS_prev=0
#ROOTMODS={}
for iev in range(nentries):
    tree.GetEntry(iev)
    if iev%10000==0:
        print("event: ",tree.run,tree.LS,iev)                                                                                            
    if tree.LS == -99:                                                                                                        
        continue      
    if tree.nPixelClusters.size()==0:
        continue

    event_count+=1
    #time_c+=tree.timeStamp_begin

    # integrate the module counts
    for item in tree.nPixelClusters:                                                                                
        module=int(item[0][1])
        #ROOTMODS[module]=1
        DEFAULTMODS[module]+=int(item[1])
        
    #if this event is the begginning of next LS then write the module counts to output
    if (LS_prev!=tree.LS or iev==nentries-1) and event_count>1000: 
        if iev==nentries-1: 
            print("last entry")
            
        #of.write(f'{tree.run:d},{LS_prev:d},{time_c/event_count:.1f}')
        of.write(f'{tree.run:d},{LS_prev:d}')
        for key in DEFAULTMODS:
            #of.write(f',{DEFAULTMODS[key]/event_count:.6f}')
            of.write(f',{DEFAULTMODS[key]:d}')
            DEFAULTMODS[key]=0
        of.write("\n")
        time_c=0
        event_count=0
        
    LS_prev=tree.LS

of.close()



        
print("Done")
