#! /usr/bin/env python3

import tables as t
import glob
from pcchd5_lib import Lumitable

##read the input files


#new veto:
#inputfiles=glob.glob("/eos/user/j/jmorenoc/BRIL/PCC/VDM/vdMMiniTreesJobs/hd5FilesJobs/newveto2-fullstat/ZB*/pcc_Data*.hd5")


Period='Period_D_4' #Only used for output information

#new veto Preiod A-D
inputfiles=glob.glob("/eos/user/j/jmorenoc/BRIL/PCC/VDM/vdMMiniTreesJobs/hd5FilesJobs/Veto_Period_D_4/ZB*/pcc_Data*.hd5")





#inputfiles2=glob.glob(input_dir2+"/pcc_Data*.hd5")

#inputfiles=inputfiles1+inputfiles2

#oldveto files: merged_ZB
print "Input files: ", inputfiles
print 'Number of input files: '+str(len(inputfiles))


##create the output
#outfilename='./pcchd5_merge.hd5'
#outfilename='/eos/user/j/jmorenoc/BRIL/PCC/VDM/vdMMiniTreesJobs/hd5FilesJobs/merge_oldveto-1_100fctr/merge.hd5'


#run numbers: 318983, 318984, 319018,319019

run_number= 319019


#new veto all periods (A-D):
outfilename='/eos/user/j/jmorenoc/PCC_hd5_fill6868/NewVeto/Period_D_4/PLTreprocessed/6868/6868_'+str(run_number)+'_PCC_NewVeto_Period_D_4.hd5'




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
   if row['runnum']== run_number: #new
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

print 'Period: '+Period
print 'Number of input files: '+str(len(inputfiles))
print 'Run Number:', run_number
