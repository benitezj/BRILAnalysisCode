import ROOT
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, Queue, csv
import struct
import os,sys


input_file = './pcc.hd5'
h5in = t.open_file(input_file,mode='r')
table = h5in.root.pcclumi

for row in table.iterrows(): 
 print row['fillnum'], row['runnum'], row['lsnum'], row['nbnum'], row['timestampsec'], row['bx']

h5in.close()

