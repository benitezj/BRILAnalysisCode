#! python

import os
import sys
import StringIO
import tables
from ROOT import TFile, TTree
from array import array
import time
import copy

from make_tuple_globals import *

FILL = 0
RUN = 0 
if len(sys.argv)>1:
    FILL = int(sys.argv[1])
if len(sys.argv)>2:
    RUN = int(sys.argv[2])

print("fill,run:",FILL,RUN)

TEST = 1



########################
def configFILL(analysis):
    #global RUN
    #global tmin
    #global tmax

    BXLeading = {}
    NBXTrain = 0
    BXExtra = {}
    
    file_hfoc_corr = ''

    hd5input = ''
    pccinput = ''
    pccinputB = ''
    pccinputF = ''
    pccinputB1 = ''
    pccinputB2 = ''
    pccinputB3 = ''
    pccinputF1P1 = ''
    pccinputF2P1 = ''
    pccinputF3P1 = ''
    pccinputF1P2 = ''
    pccinputF2P2 = ''
    pccinputF3P2 = ''

    if FILL == 6847 :
        RUN = 318675
        hd5input = '/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/6847_1806261047_1806261133.hd5'
        pccinput = '/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018B_RunDModVeto/318675.csv'
        #pccinput = '/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018B_RunDModVeto_NoCorr/318675.csv'
        file_hfoc_corr = '/afs/cern.ch/user/j/jingyu/public/LUMI/for_Jose/Overall_Correction_HFOC_2018_bt_corr_fills_6847.root'
        BXLeading = [686,816,2591,2612,2633]
        NBXTrain = 1
        tmin=1530010510
        tmax=tmin+1940 

    elif FILL == 6854 :
        RUN = 318817
        hd5input='/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/6854_1806272231_1806272311.hd5'
        pccinput='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018B_RunDModVeto/318817.csv'
        #pccinput='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018B_RunDModVeto_NoCorr/318817.csv'
        file_hfoc_corr = '/afs/cern.ch/user/j/jingyu/public/LUMI/for_Jose/Overall_Correction_HFOC_2018_bt_corr_fills_6854.root'
        BXLeading = [62,149,443,1043,1337,1631,1937,2231,2831,3125]
        NBXTrain = 10
        BXExtra = [1651,1678,2321,3255]
        tmin=1530139000
        tmax=tmin+1800

    elif FILL == 7270:
        RUN = 324293
        hd5input='/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/7274_1810102346_1810110021.hd5'
        #file_hfoc_corr = '/afs/cern.ch/user/j/jingyu/public/LUMI/for_Jose/Overall_Correction_HFOC_2018_bt_corr_fills_7274.root'
        analysis.sigma_hfoc /= 0.988  ### HF radiation correction
        pccinput  ='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels_Dec21_LateRunD/Run2018D/324293.csv'
        BXLeading = [62]
        NBXTrain = 20
        tmin=0
        tmax=9999999999

    elif FILL == 7274:
        RUN = 324418
        hd5input='/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/7274_1810102346_1810110021.hd5'
        file_hfoc_corr = '/afs/cern.ch/user/j/jingyu/public/LUMI/for_Jose/Overall_Correction_HFOC_2018_bt_corr_fills_7274.root'
        analysis.sigma_hfoc /= 0.988  ### HF radiation correction
        pccinput  ='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018D_RunDModVeto/324418.csv'
        pccinputB1='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018D_RunDModVeto_B1/324418.csv'
        pccinputB2='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018D_RunDModVeto_B2/324418.csv'
        pccinputB3='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018D_RunDModVeto_B3/324418.csv'
        pccinputF1P1='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018D_RunDModVeto_F1P1/324418.csv'
        pccinputF2P1='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018D_RunDModVeto_F2P1/324418.csv'
        pccinputF3P1='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018D_RunDModVeto_F3P1/324418.csv'
        pccinputF1P2='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018D_RunDModVeto_F1P2/324418.csv'
        pccinputF2P2='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018D_RunDModVeto_F2P2/324418.csv'
        pccinputF3P2='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018D_RunDModVeto_F3P2/324418.csv'

        ### settings for mu scan
        BXLeading = [62,196,385,574,767,901,1090,1279,1468,1661,1795,1984,2173,2362,2555,2689]
        NBXTrain = 10
        tmin=1539215450
        tmax=tmin+1600

        ## settings for full Fill
        #BXLeading = [62,196]
        #NBXTrain = 20
        #tmin=0
        #tmax=9999999999

    elif FILL == 7358 :
        RUN = 325309
        hd5input = '/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/7358_1810260704_1810260726.hd5'
        pccinput = '/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto/325309.csv'
        #pccinput = '/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_NoCorr/325309.csv'
        file_hfoc_corr = '/afs/cern.ch/user/j/jingyu/public/LUMI/for_Jose/Overall_Correction_HFOC_2018_bt_corr_fills_7358.root'
        analysis.sigma_hfoc /= 0.987  ### HF radiation correction
        tmin=1540537800
        tmax=1540538550
        BXLeading = [750,1644]
        NBXTrain = 12
        BXExtra = [11,536]
        #pccinputB='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_Bpix/325309.csv'
        #pccinputF='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_Fpix/325309.csv'
        pccinputB1='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_B1/325309.csv'
        pccinputB2='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_B2/325309.csv'
        pccinputB3='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_B3/325309.csv'
        pccinputF1P1='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_F1P1/325309.csv'
        pccinputF2P1='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_F2P1/325309.csv'
        pccinputF3P1='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_F3P1/325309.csv'
        pccinputF1P2='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_F1P2/325309.csv'
        pccinputF2P2='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_F2P2/325309.csv'
        pccinputF3P2='/afs/cern.ch/work/b/benitezj/public/BRIL/PCC/ZeroBias/AlCaLumiPixels0-N_ZeroBias-PromptReco_23Mar2019/Run2018E_RunDModVeto_F3P2/325309.csv'

    else : 
        print('Unknown fill ',FILL)
        exit()

    ## set some things
    analysis.fillBXList(NBXTrain,BXLeading,BXExtra)
    analysis.fillHFOCResidualCorr(file_hfoc_corr,FILL,RUN)

    ## open input files
    global h5file 
    h5file = tables.open_file(hd5input,'r')
    print(h5file)

    global hist_pcc 
    global hist_pccB1
    global hist_pccB2
    global hist_pccB3
    global hist_pccF1P1
    global hist_pccF2P1
    global hist_pccF3P1
    global hist_pccF1P2
    global hist_pccF2P2
    global hist_pccF3P2
    hist_pcc = openPCCFile(pccinput)
    hist_pccB1 = openPCCFile(pccinputB2) # swaped on purpose due to bug, swaped back below, tuple is ok
    hist_pccB2 = openPCCFile(pccinputB1)
    hist_pccB3 = openPCCFile(pccinputB3)
    hist_pccF1P1 = openPCCFile(pccinputF1P1)
    hist_pccF2P1 = openPCCFile(pccinputF2P1)
    hist_pccF3P1 = openPCCFile(pccinputF3P1)
    hist_pccF1P2 = openPCCFile(pccinputF1P2)
    hist_pccF2P2 = openPCCFile(pccinputF2P2)
    hist_pccF3P2 = openPCCFile(pccinputF3P2)
    

########################################
## start the main loop
analysis = make_tuple()

#configFILL(analysis)

config(analysis,FILL,RUN)
hd5input = '/afs/cern.ch/user/b/benitezj/output/public/BRIL/brildata/vdmdata18/7358_1810260704_1810260726.hd5'
h5file = tables.open_file(hd5input,'r')


analysis.createOutputTree()


for row in h5file.root.hfoclumi.iterrows():

    #if row['lsnum'] != 22: continue
    #if row['timestampsec'] != 1540539309: continue
    #print str(row['fillnum']), str(row['runnum']), str(row['lsnum']), str(row['nbnum']), str(row['timestampsec']) # , str(row['bxraw'][0])

    analysis.time[0] = row['timestampsec']
    if analysis.time[0] < analysis.tmin: continue
    if analysis.time[0] > analysis.tmax: continue

    progress = int(100*(analysis.time[0]-analysis.tmin)/(analysis.tmax-analysis.tmin))
    if progress % 10 == 0 : 
        print(progress),
        sys.stdout.flush()
    
    if TEST != None and progress == 10 :
        break
    

    #analysis.fill[0] = row['fillnum']
    #analysis.run[0]  = row['runnum']
    analysis.ls[0]   = row['lsnum']

    analysis.HFOC    = row['bxraw']

    #idx_hfet,HFET = getLUMI(time[0],idx_hfet,h5file.root.hfetlumi)
    #idx_bcm,BCM   = getLUMI(time[0],idx_bcm,h5file.root.bcm1flumi )
    #idx_plt,PLT   = getLUMI(time[0],idx_plt,h5file.root.pltlumizero)

#    analysis.getLUMIcsv(analysis.idx_pcc,analysis.hist_pcc,analysis.PCC)
#    analysis.getLUMIcsv(analysis.idx_pccB1,analysis.hist_pccB2,analysis.PCCB1) # swaped on purpose due to bug
#    analysis.getLUMIcsv(analysis.idx_pccB2,analysis.hist_pccB1,analysis.PCCB2)
#    analysis.getLUMIcsv(analysis.idx_pccB3,analysis.hist_pccB3,analysis.PCCB3)
#    analysis.getLUMIcsv(analysis.idx_pccF1p1,analysis.hist_pccF1P1,analysis.PCCF1P1)
#    analysis.getLUMIcsv(analysis.idx_pccF2p1,analysis.hist_pccF2P1,analysis.PCCF2P1)
#    analysis.getLUMIcsv(analysis.idx_pccF3p1,analysis.hist_pccF3P1,analysis.PCCF3P1)
#    analysis.getLUMIcsv(analysis.idx_pccF1p2,analysis.hist_pccF1P2,analysis.PCCF1P2)
#    analysis.getLUMIcsv(analysis.idx_pccF2p2,analysis.hist_pccF2P2,analysis.PCCF2P2)
#    analysis.getLUMIcsv(analysis.idx_pccF3p2,analysis.hist_pccF3P2,analysis.PCCF3P2)
 
    analysis.PCC.getLUMIcsv(analysis.ls[0])
    analysis.PCCB1.getLUMIcsv(analysis.ls[0])
    analysis.PCCB2.getLUMIcsv(analysis.ls[0])
    analysis.PCCB3.getLUMIcsv(analysis.ls[0])
    analysis.PCCF1P1.getLUMIcsv(analysis.ls[0])
    analysis.PCCF2P1.getLUMIcsv(analysis.ls[0])
    analysis.PCCF3P1.getLUMIcsv(analysis.ls[0])
    analysis.PCCF1P2.getLUMIcsv(analysis.ls[0])
    analysis.PCCF2P2.getLUMIcsv(analysis.ls[0])
    analysis.PCCF3P2.getLUMIcsv(analysis.ls[0])

    analysis.fillTree()

###finish writing and close
analysis.outFile.Write()
#outFile.Print()
analysis.outFile.Close()

