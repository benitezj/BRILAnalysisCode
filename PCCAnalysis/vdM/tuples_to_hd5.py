import  ROOT
from ROOT import TFile, TTree, TChain
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, Queue, csv
import math 
import struct
import argparse


parser = argparse.ArgumentParser(description='Process entries in event-based trees to produce pixel cluster counts')             
parser.add_argument('--inputfile', type=str, default="", help='The pccfile to input (pixel clusters and vertices)')                            
parser.add_argument('--vetoModules', type=str, default="", help="Text file containing list of pixel modules to veto (default: vetoModules.txt)")

args = parser.parse_args()                                               
output_path = './' 

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
tree.SetBranchStatus("LN",1)                                                                                                                     
tree.SetBranchStatus("event",1)                                                                                                                  
tree.SetBranchStatus("nPixelClusters",1)
#uncoment for tuples 2022                                                                             
tree.SetBranchStatus("nPixelClusters.first.first",1)
tree.SetBranchStatus("nPixelClusters.first.second",1) 
tree.SetBranchStatus("nPixelClusters.second",1)                          
tree.SetBranchStatus("timeStamp_begin",1)                                                                                                   

################### Make mod veto list ################                                                                                                                            
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

file='pcc_ZB.hd5' 
h5out = t.open_file(output_path+file,mode='w')

outtablename = 'pcchd5'
compr_filter = t.Filters(complevel=9, complib='blosc')
chunkshape=(100,)
outtable = h5out.create_table('/',outtablename,Lumitable,filters=compr_filter,chunkshape=chunkshape)
rownew = outtable.row

##############################################################
nentries = tree.GetEntries()

PCC_NB= [numpy.zeros(3564) for i in range(64)]
PCC_NB4= [numpy.zeros(3564) for i in range(16)] 

time_count= [numpy.zeros(3564) for i in range(64)]
time_countNB4= [numpy.zeros(3564) for i in range(16)]

ev_count= [numpy.zeros(3564) for i in range(64)]
ev_countNB4= [numpy.zeros(3564) for i in range(16)]

time_count_NB4avg= numpy.zeros(16)
event_count = numpy.zeros(16)

numBunchesLHC = 3564                                                                                                                            
k = 11246.                                                                                                                                       
fill = 8385  #2018->6868 #2017->6016                                                                                                        
                                                                                                        
################## Loop over events ###########################                                                                            

tree.GetEntry(0)                                                                    
LS_prev=tree.LS                                                                                                       
a = set()
for iev in range(nentries):                                                                                     
    tree.GetEntry(iev)                                                                                                         
    if iev%30000==0:                                                                                                           
        print (tree.run,tree.LS,iev)                                                                                            
    pixelCount=[0]*26                                                                                                          
    if tree.nPixelClusters.size() == 0:                                                                                           
        continue                                                                                                 
    for item in tree.nPixelClusters:                                                                                
        bxid=item.first.first            #item[0][0]                                             
        module=item.first.second         #item[0][1]                                                    
        clusters=item.second             #item[1] 
        print(bxid,module,clusters)           
        if module in vetoModules or module/1000000==303:#when starts in 303 is  barrel layer 0                  
            continue                                                                                         
        a.add(module)                                                                                           
        pixelCount[0]=pixelCount[0]+clusters      

    run=tree.run                                                                                                     
    LS=tree.LS #LS = 23s                                                             
    LN=tree.LN #LN=0.3s                                                                                         
    event=tree.event                                                                                         
    timeStamp = tree.timeStamp_begin    
    BXid = bxid                                                                                                
    nCluster=pixelCount[0]                                                                              
                    
    PCC_NB[LN][BXid]+= nCluster
    time_count[LN][BXid]+= timeStamp 
    ev_count[LN][BXid]+=1 
     
    if LS_prev!=LS or iev==nentries-1: 
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
            rownew['runnum'] = run
            rownew['lsnum'] = LS_prev
            #rownew['nevent'] = 
            rownew['nbnum'] = m #0 to 15  
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
    
   #reset array
   
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
      
    LS_prev=LS
      

h5out.close()
print "Done"
