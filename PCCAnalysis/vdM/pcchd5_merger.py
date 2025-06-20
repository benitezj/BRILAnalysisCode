#! /usr/bin/env python3

import tables as t
import glob
import os,sys
from pcchd5_lib import Lumitable
import argparse


parser = argparse.ArgumentParser(description='Process entries in event-based trees to produce pixel cluster counts')
parser.add_argument('--inputdir', type=str, default="", help='The input directory')
args = parser.parse_args()

inpath= args.inputdir #'/eos/user/b/benitezj/BRIL/PCC/VDM/CapsigmaProblem_ModuleStudy_PerLayer/ZeroBias1'

inputfiles=glob.glob(inpath+'/*.hd5')
print('Number of input files: '+str(inpath))
print('Number of input files: '+str(len(inputfiles)))

outfilename=inpath+'_merge.hd5'
os.system('rm -f ${outfilename}')

outfile = t.open_file(outfilename,mode='w')
compr_filter = t.Filters(complevel=9, complib='blosc')
chunkshape=(100,)
outtable = outfile.create_table('/','pcchd5',Lumitable,filters=compr_filter,chunkshape=chunkshape)
rownew = outtable.row
rowcount=0
for f in inputfiles:
  #print(f)
  inputhd5 = t.open_file(f,mode='r')
  table = inputhd5.root.pcchd5 #pltlumizero
  #print('Number of rows: '+str(len(table)))
  for row in table.iterrows(): 
    rownew['fillnum'] = row['fillnum']
    rownew['runnum'] = row['runnum']
    rownew['lsnum'] = row['lsnum']
    rownew['nbnum'] = row['nbnum']
    rownew['timestampsec'] = row['timestampsec']
    rownew['avgraw'] = row['avgraw'] 
    rownew['avg'] = row['avg']
    rownew['bxraw'] = row['bxraw']
    rownew['bx'] = row['bx'] 
    rownew.append()
    rowcount+=1
  inputhd5.close()
  outtable.flush()

outfile.close()
print('row counter: ' +str(rowcount))
print('merged file: ' + outfilename)
