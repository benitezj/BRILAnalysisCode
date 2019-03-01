#! python

import tables
from ROOT import *
from array import array
import time

NBX = 3564
BXLIST = [750,751,752,753,754,755,756,757,758,759,760,761,1644,1645,1646,1647,1648,1649,1650,1651,1652,1653,1654,1655]


h5file = tables.open_file('/brildata/vdmdata18/7358_1810260724_1810260821.hd5','r')
# print file details
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


HFOC = array( 'f', NBX*[ 0. ] )
hfoc = array( 'f', [ 0. ] )
tree.Branch( 'hfoc', hfoc, 'hfoc/F' )

#HFET = array( 'f', NBX*[ 0. ] )
hfet = array( 'f', [ 0. ] )
tree.Branch( 'hfet', hfet, 'hfet/F' )

#BCM = array( 'f', NBX*[ 0. ] )
bcm = array( 'f', [ 0. ] )
tree.Branch( 'bcm', bcm, 'bcm/F' )

#PLT = array( 'f', NBX*[ 0. ] )
plt = array( 'f', [ 0. ] )
tree.Branch( 'plt', plt, 'plt/F' )

#PCC = array( 'f', NBX*[ 0. ] )
pcc = array( 'f', [ 0. ] )
tree.Branch( 'pcc', pcc, 'pcc/F' )

#VTX = array( 'f', NBX*[ 0. ] )
vtx = array( 'f', [ 0. ] )
tree.Branch( 'vtx', vtx, 'vtx/F' )


#################################
# read detector
hist = h5file.root.hfoclumi 

idx_hfet = 0
hist_hfet = h5file.root.hfetlumi 

idx_bcm = 0
hist_bcm = h5file.root.bcm1flumi 

idx_plt = 0
hist_plt = h5file.root.pltlumizero

# PCC, VTX are root files with trees
idx_pcc = 0
hist_pcc = TFile.Open( '/nfshome0/benitezj/vdmframework_final/VdmFramework/merged_7358.root')


def getLUMI(time,idx,hist):
    for i in range(idx,len(hist)):
        if hist[i]['timestampsec'] < time :
            continue
        return i,hist[i]['bxraw']

def getLUMIroot(time,idx,hist):
    LUMI = array( 'f', NBX*[ 0. ] )
    for i in range(idx,idx+hist.pccminitree.GetEntries()):
        hist.pccminitree.GetEntry(i)
        #print time,getattr(hist.pccminitree,'timeStamp')
        if getattr(hist.pccminitree,'timeStamp')<(time-23): continue
        if getattr(hist.pccminitree,'timeStamp')-(time-23) > 23: continue
        #for j in range(NBX):
        for j in BXLIST:
            LUMI[j] = getattr(hist.pccminitree,'BXid_'+str(j))
        return i,LUMI 
        #exit()
    return 0,LUMI 



##########################################
# Start the main loop
#t0 = time.time()
for row in hist.iterrows():
    #if row['lsnum'] != 22: continue
    #if row['timestampsec'] != 1540539309: continue
    #print str(row['fillnum']), str(row['runnum']), str(row['lsnum']), str(row['nbnum']), str(row['timestampsec']) # , str(row['bxraw'][0])
    
    ####Fill 7358, VdmScan
    if row['timestampsec'] < 1540541400: continue
    if row['timestampsec'] > 1540541900: continue


    fill[0] = row['fillnum']
    run[0]  = row['runnum']
    ls[0]   = row['lsnum']
    time[0] = row['timestampsec']
    HFOC    = row['bxraw']

    idx_hfet, HFET = getLUMI(time[0],idx_hfet,hist_hfet)
    idx_bcm, BCM   = getLUMI(time[0],idx_bcm,hist_bcm)
    idx_plt, PLT   = getLUMI(time[0],idx_plt,hist_plt)
    idx_pcc, PCC   = getLUMIroot(time[0],idx_pcc,hist_pcc)

    ##loop over bx's
    #for i in range(0,NBX)
    for b in BXLIST:
        bx[0] = b
        hfoc[0] = HFOC[b]*11245.5/797.5
        hfet[0] = HFET[b]*11245.5/2431
        bcm[0]  = BCM[b]*11245.5/203.2
        plt[0]  = PLT[b]*11245.5/252.1
        pcc[0]  = PCC[b]*11245.5/5910000*23.311
        tree.Fill() 
        #print fill[0],run[0],ls[0],time[0],hfoc[0],hfet[0]

outFile.Write()
outFile.Close()

#t1 = time.time()
#print "run time = (t1-t0) seconds"
