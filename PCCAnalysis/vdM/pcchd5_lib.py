import tables as t
# pandas as pd, pylab as py, sys, numpy, math, os, Queue, csv
#import struct
#import os,sys


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


