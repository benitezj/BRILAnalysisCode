import ROOT
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, Queue, csv
import struct
import os,sys

input_file = '/eos/user/j/jmorenoc/pcc_hd5/6868/PLTreprocessed/6868/6868_318984_1806301122_1806301215.hd5'
#input_file = '/afs/cern.ch/user/j/jmorenoc/localdata/PLTreprocessed/hd5files_brilvmlxa/PLTreprocessed/6868/6868_318984_1806301121_1806301813.hd5' #real PLT reprocessed file
h5in = t.open_file(input_file,mode='r')
#table = h5in.root.pcclumi #prev
table = h5in.root.pltlumizero

#j,i
data_bx=numpy.zeros(3564)
data_time=numpy.zeros(3564)

rownum=0


######TH2:
###h2d=ROOT.TH2F("h2d"," ",32,1530359555, 1530360519,52000,0,52000)
###c2=ROOT.TCanvas("c2","",2000,1000)
###h2d.GetXaxis().SetTitle("Time (s)")
###h2d.GetYaxis().SetTitle("PCC per event")

for row in table.iterrows(): 
 #if row['timestampsec']>= 1530358114 and row['timestampsec']<= 1530360519: 

 print row['fillnum'], row['runnum'], row['lsnum'], row['nbnum'], row['timestampsec'], row['bx'], row['bxraw'], row['avgraw'], row['avg']
 ##h2d.Fill(data_time[3380],data_bx[3380])
 ##data_bx=numpy.zeros(3564)
 ##data_time=numpy.zeros(3564)
 
 rownum=rownum+1
 print "row numer ", rownum
h5in.close()

###h2d.Draw("colz")
###c2.Print("/eos/user/j/jmorenoc/pcc_hd5/Plots/vdM1_hd5-1_10/vdM1_BCID3380_Y-time_clus.png")
###
###c2.Clear()
###
###P=ROOT.TProfile
###print "type of P1: ",type(P)
###P=h2d.ProfileX()
###print "type of P2 ", type(P)
####P.SetStats(0)
###P.GetYaxis().SetTitle("Average PCC per Event")
###P.Draw()
###c2.Print("/eos/user/j/jmorenoc/pcc_hd5/Plots/vdM1_hd5-1_10/vdM1_BCID3380_Y-time_clusprom.png")


##To get min and max timestamp value
###print "max timestamp value:"
###print max(data_time)
###print "*****************"
###print "min timestampvalue"
###print min(data_time)

