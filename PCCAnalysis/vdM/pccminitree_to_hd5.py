import ROOT
from ROOT import TFile, TTree, TChain
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, Queue, csv
import struct
import os,sys

output_path = './'
numBunchesLHC = 3564
k = 11246.

fill = 6868
input_file = '/eos/user/g/gkrintir/ZeroBias/ZeroBias.root'


class Lumitable(t.IsDescription):
 fillnum = t.UInt32Col(shape=(), dflt=0, pos=0)
 runnum = t.UInt32Col(shape=(), dflt=0, pos=1)
 lsnum = t.UInt32Col(shape=(), dflt=0, pos=2)
 nbnum = t.UInt32Col(shape=(), dflt=0, pos=3)
 timestampsec = t.UInt32Col(shape=(), dflt=0, pos=4)
 timestampmsec = t.UInt32Col(shape=(), dflt=0, pos=5)
 totsize = t.UInt32Col(shape=(), dflt=0, pos=6)
# publishnnb = t.UInt8Col(shape=(), dflt=0, pos=7)
# avg = t.Float32Col(shape=(), dflt=0.0, pos=14)
 bx = t.Float32Col(shape=(3564,), dflt=0.0, pos=7)


##open the output file
if not os.path.exists(output_path):
 os.makedirs(output_path)
file='pcc.hd5'
h5out = t.open_file(output_path+file,mode='w')

## create the table
outtablename = 'pcclumi'
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
LS_prev = 0
NB_prev= 0
pcc = 0

#j,i ; NB[lumi_slice][bunch_num]
PCC_NB= [numpy.zeros(3564) for i in range(64)] #integral por cada NB
PCC_NB4= [numpy.zeros(3564) for i in range(16)] #integral por cada NB4


#
#j,i
#PCC_NB= [numpy.zeros(64) for n in range(3564)] #
#PCC_NB4= [numpy.zeros(16) for n in range(3564)] #
#loop indices: [numpy.zeros(j) for n in range(i)] ; i=numb. bunch
#e.g.: [numpy.zeros(2) for i in range(3)] -> [[0,0] , [0,0], [0,0] ]


for iev in range(10000):
 if iev%1000==0:
  print "Processing event ",iev
 
 tree.GetEntry(iev)
 #print iev, tree.run, tree.LS, tree.LN, tree.BXid, tree.timeStamp, tree.nCluster
 
 PCC_NB[tree.LN][tree.BXid]=  tree.nCluster + PCC_NB[tree.LN][tree.BXid]

 #pcc+=tree.nCluster

 #step 0: create pcc_LN[64] que va integrar nCluster para cada LN

 if LS_prev!=tree.LS:
  #step 1: creat pcc_NB4[16] ,  loop sobre pcc_LN and fill pcc_NB4 
  
  for j in range(3564): #loop numb. bunch; j= 0,1,2,..., 3562,3563.
   x=0
   for i in range(64): #loop LN ; i=0,1,2,..., 62,63.
    x= x + PCC_NB[i][j]
    
    if (i+1)%4==0: #block of code applied when i=3, 7,..,63.
     PCC_NB4[int(i/4)][j] = x #when i=3: i/4=0 ; when i=7: i/4=1; ... ; when i=63: i/4=15.  
     x=0  
    
  
  
  #step 2: loop over pcc_NB4 and fill rownew and write to file
  #loop 16 dentro de LS; 
  for m in range(16):  
   rownew['fillnum'] = fill
   rownew['runnum'] = tree.run
   rownew['lsnum'] = tree.LS
   rownew['nbnum'] = NB_prev
   rownew['timestampsec'] = tree.timeStamp
   #rownew['bx'] = pcc #y un indice de 1 a 3564# meter vector de val de la matriz; de 0-3564;
   rownew['bx'] = PCC_NB4[m]
   rownew.append()
   NB_prev=tree.LN
  

h5out.close()

