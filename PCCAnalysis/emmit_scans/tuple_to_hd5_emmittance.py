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
bcidselection=[57,67,92,919,951,961,986,1998,2727,3272]


parser = argparse.ArgumentParser(description='Process entries in event-based trees to produce pixel cluster counts')             
parser.add_argument('--inputfile', type=str, default="", help='The pccfile to input (pixel clusters and vertices)')
parser.add_argument('--moduleveto', type=str, default="", help='Module veto file')
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
for iev in range(nentries):
    tree.GetEntry(iev)                                                                                                        
    if iev%100==0:
        print(tree.run,tree.LS,iev,"/",nentries)                                                                                            

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

      
    ##here need to correct for afterglow
    ## TO DO
    

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



    
#    ### NOTE: bcid convention CMS HLT Datasets goes from 0 - 3563 !
#    ## See: https://cmsoms.cern.ch/cms/fills/bunch_info
#    ## but I think I may have found an event with value 3564, so need to protect
#    if BXid<0 or BXid>=NBX:
#     continue
#    if bcidselection.count(BXid+1) == 0:
#     continue

#    PCC_NB4[int(LN/4)][BXid] += tree.pcc
#    ev_countNB4[int(LN/4)][BXid] += 1
#    time_countNB4[int(LN/4)][BXid] += tree.timeStamp_begin    
    

#    ##this code assumes input data is organized by LS
#    if LS_prev!=LS or iev>=nentries-1: 
#        for l in range(NBX):
#            for k in range(NNB4):
#                if ev_countNB4[k][l]!=0:
#                    time_countNB4[k][l]/=ev_countNB4[k][l]
#                    PCC_NB4[k][l]/=ev_countNB4[k][l]
#
#        ## calculate avg time per NB4             
#        for r in range(NNB4):  
#            count_nonzero=0
#            for q in range(NBX): 
#                if ev_countNB4[r][q]!=0:
#                    count_nonzero += 1 
#                    time_count_NB4avg[r] += time_countNB4[r][q]
#            if count_nonzero!=0:
#                time_count_NB4avg[r]=time_count_NB4avg[r]/count_nonzero
#   
#        # fill the table for each NB4
#        for m in range(NNB4):   
#            rownew['fillnum'] = fill
#            rownew['runnum'] = run
#            rownew['lsnum'] = LS_prev
#            rownew['nbnum'] = m
#            rownew['timestampsec'] = time_count_NB4avg[m]  
#            rownew['bxraw'] = PCC_NB4[m]
#            rownew['bx'] = PCC_NB4[m]
#            bxsum=0
#            for b in range(NBX): 
#                bxsum += PCC_NB4[m][b]
#            rownew['avgraw'] = bxsum
#            rownew['avg'] = bxsum
#            rownew.append()
#            outtable.flush() 
#    
#
#        #reset for next LS 
#        for j in range(NBX):
#            for i in range(NNB4):
#                PCC_NB4[i][j]=0
#                time_countNB4[i][j]=0
#                ev_countNB4[i][j] =0
#
#        for p in range(NNB4):  
#            time_count_NB4avg[p]=0     
#   
#      
#    LS_prev=LS

    
h5out.close()
print("Done")
