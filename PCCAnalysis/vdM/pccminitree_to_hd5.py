import ROOT
from ROOT import TFile, TTree, TChain
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, Queue, csv
import struct
import os,sys
import argparse

parser = argparse.ArgumentParser(description='Process entries in event-based trees to produce pixel cluster counts')

parser.add_argument('--minitreefile', type=str, default="", help='The pccfile to input (pixel clusters and vertices)')


args = parser.parse_args()

output_path = './'

numBunchesLHC = 3564
k = 11246.

fill = 6868


input_file=args.minitreefile

class Lumitable(t.IsDescription):
 fillnum = t.UInt32Col(shape=(), dflt=0, pos=0)
 runnum = t.UInt32Col(shape=(), dflt=0, pos=1)
 lsnum = t.UInt32Col(shape=(), dflt=0, pos=2)
 nbnum = t.UInt32Col(shape=(), dflt=0, pos=3)
 #timestampsec = t.Float32Col(shape=(3564), dflt=0, pos=4) 
 #timestampsec = t.UInt32Col(shape=(3564,), dflt=0, pos=4)  
 timestampsec = t.UInt32Col(shape=(), dflt=0, pos=4)
 #totsize = t.UInt32Col(shape=(), dflt=0, pos=6)
 #publishnnb = t.UInt8Col(shape=(), dflt=0, pos=7)
 #avg = t.Float32Col(shape=(), dflt=0.0, pos=14)

 avgraw = t.Float32Col(shape=(), dflt=0.0, pos=5) #new
 avg = t.Float32Col(shape=(), dflt=0.0, pos=6) #new
 bxraw = t.Float32Col(shape=(3564,), dflt=0.0, pos=7) #new
 bx = t.Float32Col(shape=(3564,), dflt=0.0, pos=8) #new


##open the output file
if not os.path.exists(output_path):
 os.makedirs(output_path)

file='pcc_ZB.hd5' 

h5out = t.open_file(output_path+file,mode='w')

## create the table
#outtablename = 'pcclumi'
outtablename = 'pltlumizero'
compr_filter = t.Filters(complevel=9, complib='blosc')
chunkshape=(100,)
outtable = h5out.create_table('/',outtablename,Lumitable,filters=compr_filter,chunkshape=chunkshape)
rownew = outtable.row

####Open the input file and get the tree
tree=TChain("pccminitree")
tree.Add(input_file)
print 'tree entries: ', tree.GetEntries()

tree.SetBranchStatus("*",0)
tree.SetBranchStatus("run",1)
tree.SetBranchStatus("LS",1)
tree.SetBranchStatus("LN",1)
tree.SetBranchStatus("event",1)
tree.SetBranchStatus("timeStamp",1)
tree.SetBranchStatus("nCluster",1)
tree.SetBranchStatus("BXid",1)


nentries=tree.GetEntries()



#j,i ; NB[lumi_slice][bunch_num]
PCC_NB= [numpy.zeros(3564) for i in range(64)] #NB
PCC_NB4= [numpy.zeros(3564) for i in range(16)] # NB4

time_count= [numpy.zeros(3564) for i in range(64)]
time_countNB4= [numpy.zeros(3564) for i in range(16)]

ev_count= [numpy.zeros(3564) for i in range(64)]
ev_countNB4= [numpy.zeros(3564) for i in range(16)]

time_count_NB4avg= numpy.zeros(16)

#loop indices: [numpy.zeros(j) for n in range(i)] ; i=numb. bunch
#e.g.: [numpy.zeros(2) for i in range(3)] -> [[0,0] , [0,0], [0,0] ]


tree.GetEntry(0)
LS_prev=tree.LS

for iev in range(nentries):
 if iev%100000==0:
  print "Processing event ",iev ," Total events: ",nentries ," ; Remaining events: ", nentries-iev
 
 tree.GetEntry(iev)
 #if tree.timeStamp>=1530358110 and tree.timeStamp<=1530360523: #vdM1

 if iev%100000==0:
  print "ievent to hd5:", iev, "  Total events: ",nentries ," ; Remaining events: ", nentries-iev
 PCC_NB[tree.LN][tree.BXid]+=  tree.nCluster
  
 time_count[tree.LN][tree.BXid]+= tree.timeStamp
  
 ev_count[tree.LN][tree.BXid]+=1
  

 if LS_prev!=tree.LS or iev==nentries-1:
  if iev==nentries-1:
   print("last entry")

  for j in range(3564):
    for i in range(64):
     PCC_NB4[int(i/4)][j] +=PCC_NB[i][j]
     time_countNB4[int(i/4)][j]+=time_count[i][j]
     ev_countNB4[int(i/4)][j] += ev_count[i][j]
     
      
  for l in range(3564):
    for k in range(16):
     if ev_countNB4[k][l]!=0:
      time_countNB4[k][l]/=ev_countNB4[k][l]
      PCC_NB4[k][l]/=ev_countNB4[k][l]

  for r in range(16):
    count_nonzero=0
    for q in range(3564):
      if ev_countNB4[r][q]!=0:
       count_nonzero+=1
       time_count_NB4avg[r]+=time_countNB4[r][q]

    if count_nonzero!=0:
       time_count_NB4avg[r]=time_count_NB4avg[r]/count_nonzero

  for m in range(16):  
    rownew['fillnum'] = fill
    rownew['runnum'] = tree.run
    rownew['lsnum'] = LS_prev 
    rownew['nbnum'] = m #0 to 15
    #rownew['timestampsec'] = time_countNB4[m] 
    #rownew['timestampsec'] = time_countNB4[m][3380]  
    rownew['timestampsec'] = time_count_NB4avg[m] 
    bxsum=0
    for b in range(3564):
     bxsum= bxsum+PCC_NB4[m][b]
    rownew['avgraw'] = bxsum
    rownew['avg'] = bxsum
    rownew['bxraw'] = PCC_NB4[m]
    rownew['bx'] = PCC_NB4[m]
    rownew.append()
    outtable.flush() 
  for j in range(3564):
    for i in range(64):
     PCC_NB[i][j]= 0.
  
     time_count[i][j]= 0
  
     ev_count[i][j]=0
  for p in range(16):  
     time_count_NB4avg[p]=0     

  for i in range(16):
    for j in range(3564):
     PCC_NB4[i][j]=0
     time_countNB4[i][j]=0
     ev_countNB4[i][j] =0

 LS_prev=tree.LS
   

h5out.close()
print "Done"
