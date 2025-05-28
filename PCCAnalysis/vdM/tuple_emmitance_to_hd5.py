import  ROOT
from ROOT import TFile, TTree, TChain
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, csv
import math 
import struct
import argparse

numBunchesLHC = 3564                                                                                                                            
k = 11246.                                                                                                                                       

parser = argparse.ArgumentParser(description='Process entries in event-based trees to produce pixel cluster counts')             
parser.add_argument('--inputfile', type=str, default="", help='The pccfile to input (pixel clusters and vertices)')                            

args = parser.parse_args()                                               
output_path = './' 

################# open file #################
tree=TChain("PCCEventToTuple/tree")                                                                                                                        
with open(args.inputfile) as f:                                                                                                                 
    for line in f:                                                                                                                              
        if ".root" in line:                                                                                                                     
            print(line)                                                                                                                         
            tree.Add(line.rstrip('\n'))                                                                                                        
                
tree.SetBranchStatus("*",0)                                                                                                                      
tree.SetBranchStatus("run",1)                                                                                                                    
tree.SetBranchStatus("LS",1)                                                                                                                     
tree.SetBranchStatus("LN",1)                                                                                                                     
tree.SetBranchStatus("bunchCrossing",1)                                                                                                          
tree.SetBranchStatus("pcc",1)
tree.SetBranchStatus("timeStamp_begin",1)                                                                                                   



##################### Hd5 table #######################
class Lumitable(t.IsDescription):
 fillnum = t.UInt32Col(shape=(), dflt=0, pos=0)
 runnum = t.UInt32Col(shape=(), dflt=0, pos=1)
 lsnum = t.UInt32Col(shape=(), dflt=0, pos=2)
 nbnum = t.UInt32Col(shape=(), dflt=0, pos=3)  
 timestampsec = t.UInt32Col(shape=(), dflt=0, pos=4)
 avgraw = t.Float32Col(shape=(), dflt=0.0, pos=5)
 avg = t.Float32Col(shape=(), dflt=0.0, pos=6) 
 bxraw = t.Float32Col(shape=(3564,), dflt=0.0, pos=7) 
 bx = t.Float32Col(shape=(3564,), dflt=0.0, pos=8) 

 
################ open outputfile #####################
if not os.path.exists(output_path):
 os.makedirs(output_path)

h5out = t.open_file(output_path+'pcc_ZB.hd5',mode='w')
outtablename = 'pcchd5'
compr_filter = t.Filters(complevel=9, complib='blosc')
chunkshape=(100,)
outtable = h5out.create_table('/',outtablename,Lumitable,filters=compr_filter,chunkshape=chunkshape)
rownew = outtable.row


##############################################################
PCC_NB= [numpy.zeros(3564) for i in range(64)]
PCC_NB4= [numpy.zeros(3564) for i in range(16)] 
time_count= [numpy.zeros(3564) for i in range(64)]
time_countNB4= [numpy.zeros(3564) for i in range(16)]
ev_count= [numpy.zeros(3564) for i in range(64)]
ev_countNB4= [numpy.zeros(3564) for i in range(16)]
time_count_NB4avg= numpy.zeros(16)
fill = 1  #2018->6868 #2017->6016                                                                                                        


################## Loop over events ###########################                                                                            
nentries = tree.GetEntries()
tree.GetEntry(0)                                                                    
LS_prev=tree.LS                                                                                                       
bcids = set()
for iev in range(nentries):
    tree.GetEntry(iev)                                                                                                        
    if iev%30000==0:
        print(tree.run,tree.LS,iev)                                                                                            

    run=tree.run                                                                                                     
    LS=tree.LS
    LN=tree.LN
    BXid=tree.bunchCrossing

    PCC_NB[LN][BXid]+= tree.pcc
    time_count[LN][BXid]+= tree.timeStamp_begin    
    ev_count[LN][BXid]+=1 

    ##this code assumes input data is organized by LS
    if LS_prev!=LS or iev==nentries-1: 
        if iev==nentries-1: 
            print("last entry in tuple")

        #sum the LN's in each NB4
        for j in range(3564):
            for i in range(64):
                PCC_NB4[int(i/4)][j] += PCC_NB[i][j]
                time_countNB4[int(i/4)][j] += time_count[i][j] 
                ev_countNB4[int(i/4)][j] += ev_count[i][j] 
        
        #calculate average per NB4
        for l in range(3564):
            for k in range(16):
                if ev_countNB4[k][l]!=0:
                    time_countNB4[k][l]/=ev_countNB4[k][l]
                    PCC_NB4[k][l]/=ev_countNB4[k][l]
     

        ## orbit avg time per NB4             
        for r in range(16):  
            count_nonzero=0
            for q in range(3564): 
                if ev_countNB4[r][q]!=0:
                    count_nonzero += 1 
                    time_count_NB4avg[r] += time_countNB4[r][q]
   
            if count_nonzero!=0:
                time_count_NB4avg[r]=time_count_NB4avg[r]/count_nonzero
   
        # fill the table for each NB4
        for m in range(16):   
            rownew['fillnum'] = fill
            rownew['runnum'] = run
            rownew['lsnum'] = LS_prev
            rownew['nbnum'] = m
            rownew['timestampsec'] = time_count_NB4avg[m]  
            rownew['bxraw'] = PCC_NB4[m]
            rownew['bx'] = PCC_NB4[m]
            bxsum=0
            for b in range(3564): 
                bxsum += PCC_NB4[m][b]
            rownew['avgraw'] = bxsum
            rownew['avg'] = bxsum


            rownew.append()
            outtable.flush() 
    

        #reset for next LS 
        for j in range(3564):
            for i in range(64):
                PCC_NB[i][j]= 0.
                time_count[i][j]= 0
                ev_count[i][j]=0
            for i in range(16):
                PCC_NB4[i][j]=0
                time_countNB4[i][j]=0
                ev_countNB4[i][j] =0

        for p in range(16):  
            time_count_NB4avg[p]=0     
   
      
    LS_prev=LS

    
h5out.close()
print("Done")
