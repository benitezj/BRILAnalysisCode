#! python

import sys
import tables
from ROOT import *
from array import array
import time

NBX = 3564

#sigma_pcc  = 11245.5/5910000*23.311
sigma_hfoc = 11245.5/797.5
sigma_hfet = 11245.5/2431
sigma_bcm = 11245.5/203.2
#sigma_plt = 11245.5/252.1  ##sigma from vdm
sigma_plt = 11245.5/305  ##sigma from emmittance scan
#sigma_pcc  = 11245.5/5910000*23.311  ##23.3 comes from Georgios for pccminitrees
#sigma_pcc  = 11245.5/5910000
sigma_pcc  = 11245.5/3140000

 ##########################
 ####Fill 7358 (mu scan)
h5file = tables.open_file('/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/7358_1810260704_1810260726.hd5','r')
 #hist_pcc = TFile.Open( '/nfshome0/benitezj/vdmframework_final/VdmFramework/merged_7358.root')
 #hist_pcc = open('/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/325309.csv','r')
 #hist_pcc = open('/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/325309_RunDModVeto_NoCorr.csv','r')
#hist_pcc = open('/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_NoCorr/325309.csv','r')
hist_pcc = open('/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto/325309.csv','r')
hist_pccB = open('/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_Bpix/325309.csv','r')
hist_pccF = open('/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_Bpix/325309.csv','r')
tmin=1540537800
tmax=1540538550
BXLeading = [750,1644]
NBXTrain = 12
NTrain = 2
BXLIST =  array( 'i', (NTrain*NBXTrain+2)*[ 0 ] )
for i in range(NTrain):
    for j in range(NBXTrain):
        BXLIST[i*NBXTrain+j] = BXLeading[i] + j 
BXLIST[NTrain*NBXTrain]=11
BXLIST[NTrain*NBXTrain+1]=536
print BXLIST



# #############################
# #### Fill 6847
#h5file = tables.open_file('/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/6847_1806261047_1806261133.hd5','r')
##hist_pcc = open('/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/318675.csv','r')
##hist_pcc = open('/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/318675_RunDModVeto_NoCorr.csv','r')
#hist_pcc = open('/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018B_RunDModVeto/318675.csv','r')
#BXLeading = [686,816,2591,2612,2633]
#NBXTrain = 1
#NTrain = 5
#BXLIST =  array( 'i', NTrain*NBXTrain*[ 0 ] )
#for i in range(NTrain):
#    for j in range(NBXTrain):
#        BXLIST[i*NBXTrain+j] = BXLeading[i] + j 
#print BXLIST
#tmin=1530010510
#tmax=tmin+1940 #940


##############################
##### Fill 6854
#h5file = tables.open_file('/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/6854_1806272231_1806272311.hd5','r')
##hist_pcc = open('/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/318817.csv','r')
#hist_pcc = open('/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/318817_RunDModVeto_NoCorr.csv','r')
#BXLeading = [62,149,443,1043,1337,1631,1937,2231,2831,3125]
#NBXTrain = 10
#NTrain = 10
#BXLIST =  array( 'i', (NTrain*NBXTrain + 4)*[ 0 ] )
#for i in range(NTrain):
#    for j in range(NBXTrain):
#        BXLIST[i*NBXTrain+j] = BXLeading[i] + j 
#
#BXLIST[NTrain*NBXTrain]=1651
#BXLIST[NTrain*NBXTrain+1]=1678
#BXLIST[NTrain*NBXTrain+2]=2321
#BXLIST[NTrain*NBXTrain+3]=3255 
#print BXLIST
#tmin=1530139000
#tmax=tmin+1800



# #############################
# #### Fill 7274
#h5file = tables.open_file('/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/7274_1810102346_1810110021.hd5','r')
##BXLeading = [62,117,196,251,306,385,440,495,574,629,684,767,822,901,956,1011,1090,1145,1200,1279,1334,1389,1468,1523,1578,1661,1716,1795,1850,1905,1984,2039,2094,2173,2228,2283,2362,2417,2472,2555,2610,2689,2744,2799,2878,2933,2988,3067,3122,3177,3256,3311,3366]
#BXLeading = [62,196,385,574,767,901,1090,1279,1468,1661,1795,1984,2173,2362,2555,2689]
#NBXTrain = 10
#NTrain = 16
#BXLIST =  array( 'i', NTrain*NBXTrain*[ 0 ] )
#for i in range(NTrain):
#    for j in range(NBXTrain):
#        BXLIST[i*NBXTrain+j] = BXLeading[i] + j 
#print BXLIST
#tmin=1539215350
#tmax=tmin+2000
####hist_pcc = TFile.Open( '/nfshome0/benitezj/vdmframework_final/VdmFramework/fromBrilcalc_delivered.root') #file from Yusuf but has wrong scaling
##hist_pcc = TFile.Open( './fromBrilcalc_delivered.root') #file from Georgios email March 4, 1pm
#hist_pcc = open('/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/324418.csv','r')



##############################



print(h5file)


# create TTree
outFile = TFile( 'bril.root', 'recreate' )
tree = TTree( 'lumi', 'tree with detector lumi' ) 

fill = array( 'i', [ 0 ] )
tree.Branch( 'fill', fill, 'fill/I' )

run = array( 'i', [ 0 ] )
tree.Branch( 'run', run, 'run/I' )

ls = array( 'i', [ 0 ] )
tree.Branch( 'ls', ls, 'ls/I' )

time = array( 'i', [ 0 ] )
tree.Branch( 'time', time, 'time/I' )

bx = array( 'i', [ 0 ] )
tree.Branch( 'bx', bx, 'bx/I' )


pcc = array( 'f', [ 0. ] )
tree.Branch( 'pcc', pcc, 'pcc/F' )

pccb = array( 'f', [ 0. ] )
tree.Branch( 'pccb', pccb, 'pccb/F' )
pccf = array( 'f', [ 0. ] )
tree.Branch( 'pccf', pccf, 'pccf/F' )

hfoc = array( 'f', [ 0. ] )
tree.Branch( 'hfoc', hfoc, 'hfoc/F' )

hfet = array( 'f', [ 0. ] )
tree.Branch( 'hfet', hfet, 'hfet/F' )

bcm = array( 'f', [ 0. ] )
tree.Branch( 'bcm', bcm, 'bcm/F' )

plt = array( 'f', [ 0. ] )
tree.Branch( 'plt', plt, 'plt/F' )

NPLT=16
plt_0 = array( 'f', [ 0. ] )
tree.Branch( 'plt_0', plt_0, 'plt_0/F' )
plt_1 = array( 'f', [ 0. ] )
tree.Branch( 'plt_1', plt_1, 'plt_1/F' )
plt_2 = array( 'f', [ 0. ] )
tree.Branch( 'plt_2', plt_2, 'plt_2/F' )
plt_3 = array( 'f', [ 0. ] )
tree.Branch( 'plt_3', plt_3, 'plt_3/F' )
plt_4 = array( 'f', [ 0. ] )
tree.Branch( 'plt_4', plt_4, 'plt_4/F' )
plt_5 = array( 'f', [ 0. ] )
tree.Branch( 'plt_5', plt_5, 'plt_5/F' )
plt_6 = array( 'f', [ 0. ] )
tree.Branch( 'plt_6', plt_6, 'plt_6/F' )
plt_7 = array( 'f', [ 0. ] )
tree.Branch( 'plt_7', plt_7, 'plt_7/F' )
plt_8 = array( 'f', [ 0. ] )
tree.Branch( 'plt_8', plt_8, 'plt_8/F' )
plt_9 = array( 'f', [ 0. ] )
tree.Branch( 'plt_9', plt_9, 'plt_9/F' )
plt_10 = array( 'f', [ 0. ] )
tree.Branch( 'plt_10', plt_10, 'plt_10/F' )
plt_11 = array( 'f', [ 0. ] )
tree.Branch( 'plt_11', plt_11, 'plt_11/F' )
plt_12 = array( 'f', [ 0. ] )
tree.Branch( 'plt_12', plt_12, 'plt_12/F' )
plt_13 = array( 'f', [ 0. ] )
tree.Branch( 'plt_13', plt_13, 'plt_13/F' )
plt_14 = array( 'f', [ 0. ] )
tree.Branch( 'plt_14', plt_14, 'plt_14/F' )
plt_15 = array( 'f', [ 0. ] )
tree.Branch( 'plt_15', plt_15, 'plt_15/F' )


#################################
idx_hfet  = 0
idx_bcm  = 0
idx_plt  = 0
idx_plt_ = NPLT*[ 0 ] 
idx_pcc  = 0
idx_pccb  = 0
idx_pccf  = 0


def getLUMI(time,idx,hist):
    #print time,idx,hist    

    if hist == None or idx>=len(hist) :
        return len(hist),array( 'f', NBX*[ 0. ] )

    r=idx
    for i in range(idx,len(hist)):
        #print hist[i]['timestampsec']
        r=i
        if hist[i]['timestampsec'] >= time: break
     
    if r<len(hist): 
        return r,hist[r]['bxraw']
    
    return len(hist),array( 'f', NBX*[ 0. ] )



def getLUMIroot(time,idx,hist):
    LUMI = array( 'f', NBX*[ 0. ] )
    for i in range(idx,idx+hist.pccminitree.GetEntries()):
        hist.pccminitree.GetEntry(i)
        #print time,getattr(hist.pccminitree,'timeStamp')
        if getattr(hist.pccminitree,'timeStamp')<(time-23): continue
        if getattr(hist.pccminitree,'timeStamp')-(time-23) > 23: continue
        #for j in range(NBX):
        for j in BXLIST:
            LUMI[j] = getattr(hist.pccminitree,'BXid_'+str(j-1))
        return i,LUMI 
        #exit()
    return 0,LUMI 


def getLUMIcsv(ls,idx,hist):
    LUMI = array( 'f', NBX*[ 0. ] )
    hist.seek(0)

    for i in range(idx):
        next(hist)

    for line in hist:
        idx+=1
        lumi = line.split(',')
        #print ls,lumi[0],lumi[1]
        if int(lumi[1]) == int(ls):
            #print "  hello"
            for j in range(NBX):
                LUMI[j] = float(lumi[j+3])
                #print j,LUMI[j]
            
            return idx-2,LUMI 
        
    return 0,LUMI 





#exit()
##########################################
# Start the main loop
#t0 = time.time()
for row in h5file.root.hfoclumi.iterrows():

    #if row['lsnum'] != 22: continue
    #if row['timestampsec'] != 1540539309: continue
    #print str(row['fillnum']), str(row['runnum']), str(row['lsnum']), str(row['nbnum']), str(row['timestampsec']) # , str(row['bxraw'][0])

    time[0] = row['timestampsec']

    if time[0] < tmin: continue
    if time[0] > tmax: continue

    progress = int(100*(time[0]-tmin)/(tmax-tmin))
    if progress % 10 == 0 : 
        print progress, 
        sys.stdout.flush()

    fill[0] = row['fillnum']
    run[0]  = row['runnum']
    ls[0]   = row['lsnum']
    HFOC    = row['bxraw']

    #idx_pcc, PCC   = getLUMIroot(time[0],idx_pcc,hist_pcc)
    idx_pcc, PCC   = getLUMIcsv(ls[0],idx_pcc,hist_pcc)
    idx_pccb, PCCB   = getLUMIcsv(ls[0],idx_pcc,hist_pccB)
    idx_pccf, PCCF   = getLUMIcsv(ls[0],idx_pcc,hist_pccF)

    idx_hfet, HFET = getLUMI(time[0],idx_hfet,h5file.root.hfetlumi)
    idx_bcm, BCM   = getLUMI(time[0],idx_bcm,h5file.root.bcm1flumi )
    idx_plt, PLT   = getLUMI(time[0],idx_plt,h5file.root.pltlumizero)
    
#    idx_plt_[0], PLT_0   = getLUMI(time[0],idx_plt_[0],h5file.root.pltlumizero_0)
#    idx_plt_[1], PLT_1   = getLUMI(time[0],idx_plt_[1],h5file.root.pltlumizero_1)
#    idx_plt_[2], PLT_2   = getLUMI(time[0],idx_plt_[2],h5file.root.pltlumizero_2)
#    idx_plt_[3], PLT_3   = getLUMI(time[0],idx_plt_[3],h5file.root.pltlumizero_3)
#    idx_plt_[4], PLT_4   = getLUMI(time[0],idx_plt_[4],h5file.root.pltlumizero_4)
#    idx_plt_[5], PLT_5   = getLUMI(time[0],idx_plt_[5],h5file.root.pltlumizero_5)
#    idx_plt_[6], PLT_6   = getLUMI(time[0],idx_plt_[6],h5file.root.pltlumizero_6)
#    idx_plt_[7], PLT_7   = getLUMI(time[0],idx_plt_[7],h5file.root.pltlumizero_7)
#    idx_plt_[8], PLT_8   = getLUMI(time[0],idx_plt_[8],h5file.root.pltlumizero_8)
#    idx_plt_[9], PLT_9   = getLUMI(time[0],idx_plt_[9],h5file.root.pltlumizero_9)
#    idx_plt_[10], PLT_10   = getLUMI(time[0],idx_plt_[10],h5file.root.pltlumizero_10)
#    idx_plt_[11], PLT_11   = getLUMI(time[0],idx_plt_[11],h5file.root.pltlumizero_11)
#    idx_plt_[12], PLT_12   = getLUMI(time[0],idx_plt_[12],h5file.root.pltlumizero_12)
#    idx_plt_[13], PLT_13   = getLUMI(time[0],idx_plt_[13],h5file.root.pltlumizero_13)
#    idx_plt_[14], PLT_14   = getLUMI(time[0],idx_plt_[14],h5file.root.pltlumizero_14)
#    idx_plt_[15], PLT_15   = getLUMI(time[0],idx_plt_[15],h5file.root.pltlumizero_15)
    
    #print fill[0],run[0],ls[0],time[0],HFOC[BXLIST[0]-1]*sigma_hfoc

    ##loop over bx's
    #for i in range(0,NBX)
    for b in BXLIST:
        bx[0]   = b 
        pcc[0]  = PCC[b-1]*sigma_pcc 
        pccb[0]  = PCCB[b-1]*sigma_pcc 
        pccf[0]  = PCCF[b-1]*sigma_pcc 
        hfoc[0] = HFOC[b-1]*sigma_hfoc
        hfet[0] = HFET[b-1]*sigma_hfet
        bcm[0]  = BCM[b-1]*sigma_bcm
        plt[0]  = PLT[b-1]*sigma_plt

#        plt_0[0]  = PLT_0[b-1]*sigma_plt
#        plt_1[0]  = PLT_1[b-1]*sigma_plt
#        plt_2[0]  = PLT_2[b-1]*sigma_plt
#        plt_3[0]  = PLT_3[b-1]*sigma_plt
#        plt_4[0]  = PLT_4[b-1]*sigma_plt
#        plt_5[0]  = PLT_5[b-1]*sigma_plt
#        plt_6[0]  = PLT_6[b-1]*sigma_plt
#        plt_7[0]  = PLT_7[b-1]*sigma_plt
#        plt_8[0]  = PLT_8[b-1]*sigma_plt
#        plt_9[0]  = PLT_9[b-1]*sigma_plt
#        plt_10[0]  = PLT_10[b-1]*sigma_plt
#        plt_11[0]  = PLT_11[b-1]*sigma_plt
#        plt_12[0]  = PLT_12[b-1]*sigma_plt
#        plt_13[0]  = PLT_13[b-1]*sigma_plt
#        plt_14[0]  = PLT_14[b-1]*sigma_plt
#        plt_15[0]  = PLT_15[b-1]*sigma_plt
    
        tree.Fill() 
        #print fill[0],run[0],ls[0],time[0],bx[0],hfoc[0],pcc[0]

outFile.Write()
outFile.Close()

#t1 = time.time()
#print "run time = (t1-t0) seconds"
