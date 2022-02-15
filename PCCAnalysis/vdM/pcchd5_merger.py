#! /usr/bin/env python3

import ROOT
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, Queue, csv
import struct
import os,sys

input_dir='/eos/user/j/jmorenoc/samples_hd5/hd5-1_100fctr'
input_file = input_dir+'/'+'hd5_ZB1.hd5'


h5in = t.open_file(input_file,mode='r')
#table = h5in.root.pcclumi
table = h5in.root.pltlumizero


for row in table.iterrows(): 

 print row['fillnum'], row['runnum'], row['lsnum'], row['nbnum'], row['timestampsec'], row['bx'], row['bxraw'], row['avgraw'], row['avg']
 
 
h5in.close()

