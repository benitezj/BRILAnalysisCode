import  ROOT
from ROOT import TFile, TTree
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, csv
import math 
import struct
import argparse

NBX = 3564                                                                                                                            
NNB4 = 16

##Gated bcids 2025 2400b fills values are in L1 bcid convention !
# L1 convention is +1 w.r.t HLT convention (?) see https://cmsoms.cern.ch/cms/fills/bunch_info
bcidselection=[75,115,969,1009,1049,1823,1863,1903,2757]


parser = argparse.ArgumentParser(description='Process entries in event-based trees to produce pixel cluster counts')             
parser.add_argument('--inputfile', type=str, default="", help='The pccfile to input (pixel clusters and vertices)')
parser.add_argument('--moduleveto', type=str, default="", help='Module veto file')
parser.add_argument('--timestamps', type=str, default="", help='File with the scan time windows')
args = parser.parse_args()

output_path = '.' 

if args.inputfile == "":
 print("no input file")
 exit(1)

## read the module veto
vetoModules=[]
if os.path.isfile(args.moduleveto):
 print("Module veto file:",args.moduleveto)
 vetoFile=open(args.moduleveto)
 for line in vetoFile.readlines():
  if line != "":
   vetoModules.append(int(line))
print("Module veto size: ",len(vetoModules))

## open the input file
file=ROOT.TFile(args.inputfile)
tree=file.Get("PCCEventToTuple/tree")
tree.SetBranchStatus("*",0)                                                                                                                      
tree.SetBranchStatus("run",1)                                                                                                                    
tree.SetBranchStatus("LS",1)                                                                                                                     
tree.SetBranchStatus("LN",1)                                                                                                                     
tree.SetBranchStatus("timeStamp_begin",1)
tree.SetBranchStatus("pcc_bx_mod.first.first",1)
tree.SetBranchStatus("pcc_bx_mod.first.second",1)
tree.SetBranchStatus("pcc_bx_mod.second",1)
tree.SetBranchStatus("nev_bx.first",1)
tree.SetBranchStatus("nev_bx.second",1)


##################### Hd5 table #######################
class Lumitable(t.IsDescription):
 fillnum = t.UInt32Col(shape=(), dflt=0, pos=0)
 runnum = t.UInt32Col(shape=(), dflt=0, pos=1)
 lsnum = t.UInt32Col(shape=(), dflt=0, pos=2)
 nbnum = t.UInt32Col(shape=(), dflt=0, pos=3)  
 timestampsec = t.UInt32Col(shape=(), dflt=0, pos=4)
 bxraw = t.Float32Col(shape=(NBX,), dflt=0.0, pos=5) 
 bx = t.Float32Col(shape=(NBX,), dflt=0.0, pos=6) 
 avgraw = t.Float32Col(shape=(), dflt=0.0, pos=7)
 avg = t.Float32Col(shape=(), dflt=0.0, pos=8) 

 
################ open outputfile #####################
if not os.path.exists(output_path):
 os.makedirs(output_path)

h5out = t.open_file(output_path+'/pcc_ZB.hd5',mode='w')

outtablename = 'pcchd5'
compr_filter = t.Filters(complevel=9, complib='blosc')
chunkshape=(100,)
outtable = h5out.create_table('/',outtablename,Lumitable,filters=compr_filter,chunkshape=chunkshape)
rownew = outtable.row


##############################################################


fill = 1  #2018->6868 #2017->6016                                                                                                        
################## Loop over events ###########################                                                                            
nentries = tree.GetEntries()
print(args.inputfile)
for iev in range(nentries):
    tree.GetEntry(iev)                                                                                                        
    if iev%100==0:
        print(tree.run,tree.LS,iev,"/",nentries)                                                                                           

    ## read basic info
    rownew['fillnum'] = fill
    rownew['runnum'] = tree.run                                                                                                     
    rownew['lsnum'] = tree.LS
    rownew['nbnum'] = tree.LN
    rownew['timestampsec'] = tree.timeStamp_begin

      
    ## here sum the modules
    PCC_NB4 = [0]*NBX
    if tree.pcc_bx_mod.size()!=0:
        for item in tree.pcc_bx_mod:
            bx=item.first.first

            ##here filter bcids
            if len(bcidselection)==0 or  (bx+1) in bcidselection:

             if item.first.second in vetoModules:             ## here filter modules 
              continue 

             PCC_NB4[bx] += item.second

            
    ## here calcualte average clusters per bx
    for bx in range(NBX):
     if tree.nev_bx[bx]>0:
      PCC_NB4[bx] /= tree.nev_bx[bx]
    

    ##fill row
    rownew['bxraw'] = PCC_NB4
    rownew['bx'] = PCC_NB4 ##just a copy for historical reason ??

    ## calculate orbit integrated pcc
    bxsum=0
    for b in range(NBX): 
      bxsum += PCC_NB4[b]
    rownew['avgraw'] = bxsum
    rownew['avg'] = bxsum ##just a copy for historical reason ??

    rownew.append()
    outtable.flush() 

    
h5out.close()

print("Closed inputfile")

