
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, Queue, csv
import struct
import os,sys

output_path = './'
numBunchesLHC = 3564
k = 11246.


class Lumitable(t.IsDescription):
 fillnum = t.UInt32Col(shape=(), dflt=0, pos=0)
 runnum = t.UInt32Col(shape=(), dflt=0, pos=1)
 lsnum = t.UInt32Col(shape=(), dflt=0, pos=2)
 nbnum = t.UInt32Col(shape=(), dflt=0, pos=3)
 timestampsec = t.UInt32Col(shape=(), dflt=0, pos=4)
 timestampmsec = t.UInt32Col(shape=(), dflt=0, pos=5)
 totsize = t.UInt32Col(shape=(), dflt=0, pos=6)
 publishnnb = t.UInt8Col(shape=(), dflt=0, pos=7)
 avg = t.Float32Col(shape=(), dflt=0.0, pos=14)
 bx = t.Float32Col(shape=(3564,), dflt=0.0, pos=16)



if not os.path.exists(output_path):
 os.makedirs(output_path)
file='pcc.hd5'
h5out = t.open_file(output_path+file,mode='w')

outtablename = 'pcclumi'
compr_filter = t.Filters(complevel=9, complib='blosc')
chunkshape=(100,)
outtable = h5out.create_table('/',outtablename,Lumitable,filters=compr_filter,chunkshape=chunkshape)
rownew = outtable.row


# rownew['fillnum'] = oldrow['fillnum'] 
# rownew['runnum'] = oldrow['runnum']
# rownew['lsnum'] = row['lsnum']
# rownew['nbnum'] = row['nbnum']
# rownew['timestampsec'] = oldrow['timestampsec']
# rownew['timestampmsec'] = oldrow['timestampmsec']
# rownew['totsize'] = oldrow['totsize'] 
# rownew['publishnnb'] = oldrow['publishnnb'] 
# rownew['avg'] = lumi_orbit_avg_w_ 
# rownew['bx'] = sbil_avg_w
# rownew.append()
 

h5out.close()

