#! /usr/bin/env python3

import tables as t
import glob
from pcchd5_lib import Lumitable

##read the input files
input_dir='/eos/user/j/jmorenoc/samples_hd5/hd5-1_100fctr'
inputfiles=glob.glob(input_dir+"/hd5_ZB*.hd5")
print 'Number of input files: '+str(len(inputfiles))


##create the output
outfilename='./pcchd5_merge.hd5'
outfile = t.open_file(outfilename,mode='w')
outtablename = 'pltlumizero'
compr_filter = t.Filters(complevel=9, complib='blosc')
chunkshape=(100,)
outtable = outfile.create_table('/',outtablename,Lumitable,filters=compr_filter,chunkshape=chunkshape)
rownew = outtable.row

##iterate over the files and copy the rows into new table
rowcount=0
for f in inputfiles:
  print f
  hd5file = t.open_file(f,mode='r')
  table = hd5file.root.pltlumizero
  print 'Number of rows: '+str(len(table))
  for row in table.iterrows(): 
    #print row['fillnum'], row['runnum'], row['lsnum'], row['nbnum'], row['timestampsec'], row['avg']
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
  hd5file.close()
  outtable.flush()


print 'number of outtable rows: ' +str(len(outtable)) 
print 'row counter: ' +str(rowcount) 
print 'merged file: ' + outfilename
outfile.close()
