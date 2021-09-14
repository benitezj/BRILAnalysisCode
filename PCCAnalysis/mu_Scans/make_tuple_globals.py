#! python

import os
#import sys
import StringIO
#import tables
from ROOT import TFile, TTree
from array import array
#import time
import copy

NBX = 3564

######################
#FILL = 0
#NBX = 3564
#RUN = 0
#tmin=0
#tmax=0

##################
#####  input data files
#h5file = None
#hist_pcc = None
#hist_pccB1 = None
#hist_pccB2 = None
#hist_pccB3 = None
#hist_pccF1P1 = None
#hist_pccF2P1 = None
#hist_pccF3P1 = None
#hist_pccF1P2 = None
#hist_pccF2P2 = None
#hist_pccF3P2 = None
## indices needed for reading input files


############ helper class
class DataFile:
    idx = 0
    file = StringIO.StringIO()
    Lumi = array( 'f', NBX*[ 0. ] )
    
    def open(self,inputf):
        if os.path.isfile(inputf):
            hist = open(inputf,'r')
            print(file)
    

    def getLUMIcsv(self,ls):
        if self.file.closed: 
            idx=0
            return
    
        self.file.seek(0)
        for i in range(self.idx):
            next(self.file)

        for line in self.file:
            self.idx+=1
            lumi = line.split(',')
            #print ls,lumi[0],lumi[1]
            if int(lumi[1]) == int(self.ls):
                #print "  hello"
                for j in range(NBX):
                    self.Lumi[j] = float(lumi[j+3])
                    #print j,LUMI[j]
            
                self.idx-=1
                return

        self.idx=0
        return


############### analysis object
class make_tuple:

    min=0
    tmax=0
    BXLIST = {}

    ##### HFOC residual corrections
    hfoc_corr = {} 
    hfoc_corr_nls = 20
    
    #### Visible crossections
    sigma_hfoc = 11245.5/797.5
    sigma_hfet = 11245.5/2431
    sigma_bcm = 11245.5/203.2
    #sigma_plt = 11245.5/252.1  ##sigma from vdm
    sigma_plt = 11245.5/305  ##sigma from emmittance scan
    #sigma_pcc  = 11245.5/5910000*23.311  ##23.3 comes from Georgios for pccminitrees
    #sigma_pcc  = 11245.5/5910000
    sigma_pcc  = 11245.5/3140000
    #sigma_pccB  = sigma_pcc/0.501
    #sigma_pccF  = sigma_pcc/0.499
    sigma_pccB1  = sigma_pcc/0.0117935
    sigma_pccB2  = sigma_pcc/0.20715
    sigma_pccB3  = sigma_pcc/0.280174
    sigma_pccF1P1  = sigma_pcc/0.0939883
    sigma_pccF2P1  = sigma_pcc/0.118907
    sigma_pccF3P1  = sigma_pcc/0.119082
    sigma_pccF1P2  = sigma_pcc/0.0320373
    sigma_pccF2P2  = sigma_pcc/0.056945
    sigma_pccF3P2  = sigma_pcc/0.0822348
    
    ##### arrays to hold the data
    HFOC = array( 'f', NBX*[ 0. ] )
    HFET = array( 'f', NBX*[ 0. ] )
    BCM = array( 'f', NBX*[ 0. ] )
    PLT = array( 'f', NBX*[ 0. ] )
#    PCC = array( 'f', NBX*[ 0. ] )
#    PCCB1 = array( 'f', NBX*[ 0. ] )
#    PCCB2 = array( 'f', NBX*[ 0. ] )
#    PCCB3 = array( 'f', NBX*[ 0. ] )
#    PCCF1P1 = array( 'f', NBX*[ 0. ] )
#    PCCF2P1 = array( 'f', NBX*[ 0. ] )
#    PCCF3P1 = array( 'f', NBX*[ 0. ] )
#    PCCF1P2 = array( 'f', NBX*[ 0. ] )
#    PCCF2P2 = array( 'f', NBX*[ 0. ] )
#    PCCF3P2 = array( 'f', NBX*[ 0. ] )

#    ##input files
#    hist_pcc = StringIO.StringIO()
#    hist_pccB1 = StringIO.StringIO()
#    hist_pccB2 = StringIO.StringIO()
#    hist_pccB3 = StringIO.StringIO()
#    hist_pccF1P1 = StringIO.StringIO()
#    hist_pccF2P1 = StringIO.StringIO()
#    hist_pccF3P1 = StringIO.StringIO()
#    hist_pccF1P2 = StringIO.StringIO()
#    hist_pccF2P2 = StringIO.StringIO()
#    hist_pccF3P2 = StringIO.StringIO()
#    idx_hfet  = 0
#    idx_bcm  = 0
#    idx_plt  = 0
#    idx_pcc  = 0
#    idx_pccB1  = 0
#    idx_pccB2  = 0
#    idx_pccB3  = 0
#    idx_pccF1p1  = 0
#    idx_pccF2p1  = 0
#    idx_pccF3p1  = 0
#    idx_pccF1p2  = 0
#    idx_pccF2p2  = 0
#    idx_pccF3p2  = 0


    ###PCC Data files
    PCC = DataFile()
    PCCB1 = DataFile()
    PCCB2 = DataFile()
    PCCB3 = DataFile()
    PCCF1P1 = DataFile()
    PCCF2P1 = DataFile()
    PCCF3P1 = DataFile()
    PCCF1P2 = DataFile()
    PCCF2P2 = DataFile()
    PCCF3P2 = DataFile()


    
    ###### output TTree branches
    outFile = None
    tree = None
    fill = array( 'i', [ 0 ] )
    run = array( 'i', [ 0 ] )
    ls = array( 'i', [ 0 ] )
    time = array( 'i', [ 0 ] )
    bx = array( 'i', [ 0 ] )
    pcc = array( 'f', [ 0. ] )
    pccB = array( 'f', [ 0. ] )
    pccF = array( 'f', [ 0. ] )
    pccB1 = array( 'f', [ 0. ] )
    pccB2 = array( 'f', [ 0. ] )
    pccB3 = array( 'f', [ 0. ] )
    pccF1p1 = array( 'f', [ 0. ] )
    pccF2p1 = array( 'f', [ 0. ] )
    pccF3p1 = array( 'f', [ 0. ] )
    pccF1p2 = array( 'f', [ 0. ] )
    pccF2p2 = array( 'f', [ 0. ] )
    pccF3p2 = array( 'f', [ 0. ] )
    hfoc = array( 'f', [ 0. ] )
    hfet = array( 'f', [ 0. ] )
    bcm = array( 'f', [ 0. ] )
    plt = array( 'f', [ 0. ] )



    def createOutputTree(self):
        self.outFile = TFile( 'bril.root', 'recreate' )
        self.tree = TTree( 'lumi', 'tree with detector lumi' ) 
        self.tree.Branch( 'fill', self.fill, 'fill/I' )
        self.tree.Branch( 'run', self.run, 'run/I' )
        self.tree.Branch( 'ls', self.ls, 'ls/I' )
        self.tree.Branch( 'time', self.time, 'time/I' )
        self.tree.Branch( 'bx', self.bx, 'bx/I' )
        self.tree.Branch( 'pcc', self.pcc, 'pcc/F' )
        self.tree.Branch( 'pccB1', self.pccB1, 'pccB1/F' )
        self.tree.Branch( 'pccB2', self.pccB2, 'pccB2/F' )
        self.tree.Branch( 'pccB3', self.pccB3, 'pccB3/F' )
        self.tree.Branch( 'pccF1p1', self.pccF1p1, 'pccF1p1/F' )
        self.tree.Branch( 'pccF2p1', self.pccF2p1, 'pccF2p1/F' )
        self.tree.Branch( 'pccF3p1', self.pccF3p1, 'pccF3p1/F' )
        self.tree.Branch( 'pccF1p2', self.pccF1p2, 'pccF1p2/F' )
        self.tree.Branch( 'pccF2p2', self.pccF2p2, 'pccF2p2/F' )
        self.tree.Branch( 'pccF3p2', self.pccF3p2, 'pccF3p2/F' )
        self.tree.Branch( 'hfoc', self.hfoc, 'hfoc/F' )
        self.tree.Branch( 'hfet', self.hfet, 'hfet/F' )
        self.tree.Branch( 'bcm', self.bcm, 'bcm/F' )
        self.tree.Branch( 'plt', self.plt, 'plt/F' )
        

    def fillTree(self):
        for b in self.BXLIST:
            self.bx[0]   = b

            ##apply HFOC residual correction
            if int((self.ls[0]-1)/self.hfoc_corr_nls) in self.hfoc_corr :
                self.hfoc[0] = self.HFOC[b-1]*self.sigma_hfoc*self.hfoc_corr[int((self.ls[0]-1)/self.hfoc_corr_nls)].GetBinContent(b+1)
            else :
                self.hfoc[0] = self.HFOC[b-1]*self.sigma_hfoc
                
            self.pcc[0]    = self.PCC.Lumi[b-1]*self.sigma_pcc
#            self.pccB1[0]  = self.PCCB1[b-1]*self.sigma_pccB1
#            self.pccB2[0]  = self.PCCB2[b-1]*self.sigma_pccB2
#            self.pccB3[0]  = self.PCCB3[b-1]*self.sigma_pccB3
#            self.pccF1p1[0]  = self.PCCF1P1[b-1]*self.sigma_pccF1P1
#            self.pccF2p1[0]  = self.PCCF2P1[b-1]*self.sigma_pccF2P1
#            self.pccF3p1[0]  = self.PCCF3P1[b-1]*self.sigma_pccF3P1
#            self.pccF1p2[0]  = self.PCCF1P2[b-1]*self.sigma_pccF1P2
#            self.pccF2p2[0]  = self.PCCF2P2[b-1]*self.sigma_pccF2P2
#            self.pccF3p2[0]  = self.PCCF3P2[b-1]*self.sigma_pccF3P2
            self.hfet[0]     = self.HFET[b-1]*self.sigma_hfet
            self.bcm[0]      = self.BCM[b-1]*self.sigma_bcm
            self.plt[0]      = self.PLT[b-1]*self.sigma_plt
            
            self.tree.Fill()
            #print fill[0],run[0],ls[0],time[0],bx[0],hfoc[0],pcc[0]
            #print fill[0],run[0],ls[0],':',idx_pccB1,pccB1[0],idx_pccF3p1,pccF3p1[0]


    def fillBXList(self,NBXTrain,BXLeading,BXExtra):
        print('NBXTrain,BXLeading,BXExtra:',NBXTrain,BXLeading,BXExtra)
        self.BXLIST =  array( 'i', (len(BXLeading)*NBXTrain+len(BXExtra))*[ 0 ] )
        for i in range(len(BXLeading)):
            for j in range(NBXTrain):
                self.BXLIST[i*NBXTrain+j] = BXLeading[i] + j
        for i in range(len(BXExtra)):
            self.BXLIST[len(BXLeading)*NBXTrain + i] = BXExtra[i] 
        print('BXLIST:',self.BXLIST)




    def fillHFOCResidualCorr(self,resid_hfoc_corr,FILL,RUN):
        print('fillHFOCResidualCorr:',resid_hfoc_corr,FILL,RUN)
        file_hfoc_corr = TFile.Open(resid_hfoc_corr,'read')

        if file_hfoc_corr == None : 
            print('No resid_hfoc_corr')
            return
            #file_hfoc_corr.ls()
 
        for l in range(int(1000/self.hfoc_corr_nls)):
            hname= 'After_Corr_'+str(RUN)+'_LS'+str(l*self.hfoc_corr_nls+1)+'_LS'+str((l+1)*self.hfoc_corr_nls)+'_Fill'+str(FILL)
            hist = getattr(file_hfoc_corr,hname,None)
            #print hname, hist
            if hist != None :
                self.hfoc_corr[l] = copy.copy(hist) 
                hname_denom = 'Before_Corr_'+str(RUN)+'_LS'+str(l*self.hfoc_corr_nls+1)+'_LS'+str((l+1)*self.hfoc_corr_nls)+'_Fill'+str(FILL)
                hist_denom = getattr(file_hfoc_corr,hname_denom)
                if hist_denom == None: 
                    print hist_denom, "not found in ",file_hfoc_corr
                    return
                self.hfoc_corr[l].Divide(hist_denom)
                print(self.hfoc_corr[l])




######################################
#### aux functions
def openPCCFile(pccinput):
    hist = StringIO.StringIO()
    if os.path.isfile(pccinput):
        hist = open(pccinput,'r')
        print(hist)
    return hist

                               

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



##################################################################
#### run by run configurations
def config(analysis,FILL,RUN) :
    analysis.fill[0] = FILL
    analysis.run[0] = RUN

    if FILL == 7358 and RUN == 325309 :
        analysis.fillHFOCResidualCorr('/afs/cern.ch/user/j/jingyu/public/LUMI/for_Jose/Overall_Correction_HFOC_2018_bt_corr_fills_7358.root',FILL,RUN)
        analysis.sigma_hfoc /= 0.987  ### HF radiation correction
        analysis.tmin=1540537800
        analysis.tmax=1540538550
        analysis.fillBXList(12,[750,1644],[11,536])

        analysis.PCC.open('/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto/325309.csv')
        analysis.PCCB1.open('/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_B1/325309.csv')
        analysis.PCCB2.open('/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_B2/325309.csv')
        analysis.PCCB3.open('/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_B3/325309.csv')

