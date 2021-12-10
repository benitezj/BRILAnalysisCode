#!/nfshome0/lumipro/brilconda/bin/python
#The output files should go in /localdata/fromeo/

import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, Queue, csv
import struct
import os,sys
from matplotlib import pyplot as plt

### --- Goal ---
#This script re-calculates bxraw, bx, avgraw, avg, in pltlumizero applying weights (eff, slope) on per channels values of luminosity read from pltaggzero
#In general, a time unit corresponds to an event. It is consistent with 
#pltaggzero: iterating 16 times, 1 for each channel of the plt
#pltlumizero: iterating 1 time (which takes into account (sum or average) the 16 channels of the plt)
#pltaggzero info
#sbil is mu (instantaneous luminosity per 4NB per bx (though it is a number withouth unit of measurement))
#3564 is the num of bunches in the LHC ring (the sum of their luminosity gives the integrated lumi per orbit)
#sbil_ch and sbil_avg are re-updated every time unit (which basically is over a orbit and iterating over the 16 channels)
#lumi_orbit is the instantaneous luminosity per orbit (the instant is the orbit time). It is the sum of sbil over all bunches in LHC
#NB
#It is possible that during the data taking some effects affect the stored data (raw['data']). Some examples are:
#1. Shifts in element positions 
#raw['data'] is corrected through rawDataFixed = numpy.roll(raw['data'],n)]), meaning its entries are shifted in position by n.
#n should normally be set to 0, unless a pathological behaviour is found during the reprocessing that points to a shift in the entries
#2. data are float at binary level, but are treated as int
#This happens in 2017 (until August), the data was being declared as int in one place but treated as a float in another place, so the results in the hd5 file are slightly garbled. 
#This requires their reinterpretation (a simply recast is not enough).
#binaries = [struct.pack('i', i) for i in raw['data']]
#rawDataFixed = [struct.unpack('f', b)[0] for b in binaries]

### --- Declare initial variables ---
#Input parameters
years = ['17']
#fills = [
#5633, 5691, 5719, 5745, 5838, 5862, 5880, 5900, 5934, 5958, 5985, 6019, 6039, 6061, 6096, 6116, 6146, 6162, 6177, 6193, 6217, 6239, 6258, 6277, 6295, 6309, 6325, 6351, 6370, 
#5663, 5692, 5720, 5746, 5839, 5864, 5882, 5902, 5940, 5960, 5996, 6020, 6041, 6072, 6097, 6119, 6147, 6164, 6179, 6194, 6218, 6240, 6259, 6278, 6296, 6311, 6336, 6352, 6371, 
#5676, 5697, 5722, 5747, 5840, 5865, 5883, 5904, 5942, 5962, 5997, 6021, 6044, 6073, 6098, 6123, 6148, 6165, 6180, 6195, 6221, 6241, 6260, 6279, 6297, 6312, 6337, 6353, 
#5677, 5698, 5730, 5748, 5841, 5866, 5884, 5905, 5943, 5963, 6001, 6024, 6046, 6079, 6100, 6130, 6150, 6166, 6181, 6196, 6226, 6243, 6261, 6283, 6298, 6313, 6338, 6355, 
#5680, 5699, 5733, 5749, 5842, 5868, 5885, 5906, 5944, 5965, 6005, 6026, 6047, 6082, 6101, 6132, 6152, 6167, 6182, 6197, 6227, 6244, 6262, 6284, 6300, 6314, 6340, 6356, 
#5681, 5700, 5735, 5750, 5843, 5869, 5887, 5919, 5946, 5966, 6006, 6030, 6048, 6084, 6104, 6136, 6153, 6168, 6183, 6199, 6228, 6245, 6263, 6285, 6301, 6315, 6341, 6357, 
#5682, 5704, 5736, 5822, 5845, 5870, 5888, 5920, 5947, 5970, 6008, 6031, 6050, 6086, 6105, 6138, 6154, 6169, 6184, 6200, 6230, 6247, 6266, 6287, 6302, 6316, 6343, 6358, 
#5683, 5707, 5737, 5824, 5848, 5871, 5891, 5927, 5950, 5971, 6009, 6033, 6052, 6089, 6106, 6140, 6155, 6170, 6185, 6201, 6231, 6252, 6268, 6288, 6303, 6317, 6344, 6360, 
#5684, 5708, 5738, 5825, 5849, 5872, 5893, 5928, 5952, 5974, 6012, 6034, 6053, 6090, 6109, 6141, 6156, 6171, 6186, 6202, 6232, 6253, 6269, 6289, 6304, 6318, 6346, 6361, 
#5685, 5709, 5739, 5830, 5856, 5873, 5895, 5929, 5953, 5976, 6014, 6035, 6054, 6091, 6110, 6142, 6158, 6173, 6189, 6210, 6233, 6254, 6271, 6290, 6305, 6319, 6347, 6362, 
#5686, 5710, 5740, 5833, 5859, 5874, 5896, 5930, 5954, 5979, 6015, 6036, 6055, 6092, 6111, 6143, 6159, 6174, 6190, 6211, 6234, 6255, 6272, 6291, 6306, 6321, 6348, 6363, 
#5689, 5717, 5742, 5834, 5860, 5876, 5898, 5932, 5956, 5980, 6016, 6037, 6057, 6093, 6112, 6144, 6160, 6175, 6191, 6212, 6236, 6256, 6275, 6293, 6307, 6323, 6349, 6364, 
#5690, 5718, 5744, 5837, 5861, 5878, 5899, 5933, 5957, 5984, 6018, 6038, 6060, 6094, 6114, 6145, 6161, 6176, 6192, 6216, 6238, 6257, 6276, 6294, 6308, 6324, 6350, 6365, 
#] #Fills to be processed 
fills = [6191]
ini = 0 #Initial number of events to be parsed
entries = -1 #16000 #Final number of events to be parsed (-1 means all entries) (must be multiple of 16)
#Output parameters (save in brildev2 at a first stage)
output_path = '/brildata/17_v2/pltzero/' #'/nfshome0/fromeo/newhd5HI/'

#Declare initial quantities
nchannels = 16 #total number of channels in PLT (note that some channels might be disabled)
numBunchesLHC = 3564

##Weights and normalization values
#Note that the comparison of sbil_avg, lumi_orbit_avg with bxraw (bx), avgraw (avg) is valid given the same conditions of recording. This means:
#For 2018:
#disablech = [0,4]
#numdisabledch = 2
#For 2017:
#disabled channels and slopes are the same for the whole 2017
numdisabledch = 3
disablech = [0,3,4]
numeffectivech = nchannels-numdisabledch
slope_L = [-99, 0.0166,	0.0137, -99, -99, 0.0148, 0.0164, 0.0163, 0.0127, 0.0142, 0.0168, 0.0182, 0.0166, 0.0136, 0.0118, 0.0116]
slope_T = [-99, 0.0105, 0.0122, -99, -99, 0.0093, 0.0108, 0.0029, 0.0038, 0.0005, -0.0007, 0.0142, 0.0026, 0.0153, 0.0096, 0.0009]
#efficiencies vary per fill
fills_effs = {} #dictionary with fill as a key and an array of 16 values (1 each PLT channel, make sure the .csv files has columns with -99 for the missing channels)
shift=6 #shift in the column position that determines where PLT channel information starts
missing_fills_incsv = [5633, 5663, 5676, 5677, 5680, 5681, 5682, 5683, 5684, 5685, 5686, 5689, 5690, 5691, 5692, 5697] #for these fills, use the efficiency values calculated for the first fill
with open('2017PLTCorrections_Fills_5633_6371.csv') as csv_file:
 csv_reader = csv.reader(csv_file, delimiter=',')
 line_count = 0
 for row in csv_reader:
  #if 1 <= line_count and line_count < 5:
  if line_count>=1: #skip first file (line 0) has it contains the titles of the columns (check it on the .csv input file)
   effs = []
   for i in range(nchannels): effs.append(row[i+shift]) #Note that this assumes you have a column in the csv files for all 16 PLT channels, also those that are disable (have -99 in the column value)
   if line_count==1: #This is special for 2017, where David said that for the fills before the first fill in the csv file we can assign the weights of the first fills in the csv file
    for mf in missing_fills_incsv: fills_effs[int(mf)] = effs
   fills_effs[int(row[0])] = effs
  line_count += 1

##Normalization values
sigvis = 292.8
k = 11246./sigvis 

class Lumitable(t.IsDescription):
 fillnum = t.UInt32Col(shape=(), dflt=0, pos=0)
 runnum = t.UInt32Col(shape=(), dflt=0, pos=1)
 lsnum = t.UInt32Col(shape=(), dflt=0, pos=2)
 nbnum = t.UInt32Col(shape=(), dflt=0, pos=3)
 timestampsec = t.UInt32Col(shape=(), dflt=0, pos=4)
 timestampmsec = t.UInt32Col(shape=(), dflt=0, pos=5)
 totsize = t.UInt32Col(shape=(), dflt=0, pos=6)
 publishnnb = t.UInt8Col(shape=(), dflt=0, pos=7)
 datasourceid = t.UInt8Col(shape=(), dflt=0, pos=8)
 algoid = t.UInt8Col(shape=(), dflt=0, pos=9)
 channelid = t.UInt8Col(shape=(), dflt=0, pos=10)
 payloadtype = t.UInt8Col(shape=(), dflt=0, pos=11)
 calibtag = t.StringCol(itemsize=32, shape=(), dflt='', pos=12)
 avgraw = t.Float32Col(shape=(), dflt=0.0, pos=13)
 avg = t.Float32Col(shape=(), dflt=0.0, pos=14)
 bxraw = t.Float32Col(shape=(3564,), dflt=0.0, pos=15)
 bx = t.Float32Col(shape=(3564,), dflt=0.0, pos=16)
 maskhigh = t.UInt32Col(shape=(), dflt=0, pos=17)
 masklow = t.UInt32Col(shape=(), dflt=0, pos=18)

### --- Start the reprocessing ---
for year in years:
 print "The year is %s" % (year)
 for fill in fills:
  print "The fill number is %s" % (fill)
  #Prepare input files and output dirs
  input_path = '/brildata/'+str(year)+'/'+str(fill)+'/'
  files = os.listdir(input_path)
  print "files are %s " % (files)
  #For the output
  outtablename = 'pltlumizero'
  compr_filter = t.Filters(complevel=9, complib='blosc')
  chunkshape=(100,)
  output_path_ = output_path
  output_path_ += '/'+str(fill)+'/'
  if not os.path.exists(output_path_):
   os.makedirs(output_path_)
  #Parse files
  c = 0 #entries iterator, usefull if you want to test a small number of events
  stoparsing = False
  for file in files:
   if(stoparsing): break
   sbil_ch = numpy.zeros((nchannels,numBunchesLHC)) 
   sbil_avg = numpy.zeros(numBunchesLHC) #The average is upon the active channels and corresponds to the "bxraw" (or "bx") saved in pltlumizero (whic is mu or sbil) 
   lumi_orbit_ch = [[] for i in range(nchannels)]
   lumi_orbit_avg = [] #It corresponds to "avgraw" (or "avg") in pltlumizero
   sbil_ch_w = numpy.zeros((nchannels,numBunchesLHC)) 
   sbil_avg_w = numpy.zeros(numBunchesLHC) #The average is upon the active channels and corresponds to the 'sbil' saved in pltlumizero 
   lumi_orbit_ch_w = [[] for i in range(nchannels)]
   lumi_orbit_avg_w = [] 
   h5in = t.open_file(input_path+file,mode='r')
   if("/beam" not in h5in or "/pltaggzero" not in h5in or "/pltlumizero" not in h5in): #Sometime the fill never reaches STABLE BEAMS and these groups are not written
    print "Fill %s File %s has a problem (beam OR pltaggzero OR pltlumizero tables not in input file)" % (fill,file)
    h5in.close()
    continue
   if(h5in.root.beam.nrows==0 or h5in.root.pltaggzero.nrows==0 or h5in.root.pltlumizero.nrows==0): #Sometime tables exists, but with zero entries
    print "Fill %s File %s has a problem (nrow of beam OR pltaggzero OR pltlumizero is 0 in input file)" % (fill,file)
    h5in.close()
    continue
   print "The file is %s" % (file)
   table = h5in.root.pltaggzero
   h5out = t.open_file(output_path_+file,mode='w')
   outtable = h5out.create_table('/',outtablename,Lumitable,filters=compr_filter,chunkshape=chunkshape)
   rownew = outtable.row
   channel_counter = 0 #bxmask = h5in.root.beam[channel_counter/nchannels]['collidable'] needs it and it has to start from 0 for every file
   bxmask = h5in.root.beam[0]['collidable']
   leading = (py.logical_xor(bxmask, py.roll(bxmask,1)) & bxmask)
   train = py.logical_xor(leading,bxmask)
   maskhigh = 0
   masklow = 0
   for d in disablech: #Move to function at some point
    #masklow
    if(d==0):
     masklow += 0x1 #Use hexadecimal numbers
    elif(d==1):
     masklow += 0x2
    elif(d==2):
     masklow += 0x10
    elif(d==3):
     masklow += 0x20
    elif(d==4):
     masklow += 0x100
    elif(d==5):
     masklow += 0x200
    elif(d==6):
     masklow += 0x1000
    elif(d==7):
     masklow += 0x2000
    #maskhigh
    if(d==8):
     maskhigh += 0x1
    elif(d==9):
     maskhigh += 0x2
    elif(d==10):
     maskhigh += 0x10
    elif(d==11):
     maskhigh += 0x20
    elif(d==12):
     maskhigh += 0x100
    elif(d==13):
     maskhigh += 0x200
    elif(d==14):
     maskhigh += 0x1000
    elif(d==15):
     maskhigh += 0x2000
   masklow = 0x3333-masklow #You want the active channels, not the disabled ones
   maskhigh = 0x3333-maskhigh
   for row in table.iterrows(): #each row iterate over a channels. After 16 times, you go to a "new event"
    #Parse only n events
    if(c>=entries and entries!=-1):
     stoparsing = True
     break
    if((ini<=c and c<entries) or (entries==-1 and c>=ini)):
     isdisabledch = 0 #0 means is not disabled 
     for i in disablech:
      if(i==int(row['channelid'])):
       isdisabledch = 1
       break
     if(isdisabledch==0):
      #Fix row['data'], if needed
      rawDataFixed = row['data'] #Initially no changes
      if year=="17":
       if fill<=6156:
        binaries = [struct.pack('i', i) for i in rawDataFixed]
        rawDataFixed = [struct.unpack('f', b)[0] for b in binaries]
       if fill==5698:
        if(row['runnum']>=294947 and row['runnum']<294966): rawDataFixed = numpy.roll(rawDataFixed,828) 
       if fill==5839:
        if(row['runnum']==297050 and row['lsnum']>=8 and row['lsnum']<=69): rawDataFixed = numpy.roll(rawDataFixed,827)
       if fill==5883:
        if(row['runnum']<297670 or (row['runnum']==297670 and row['lsnum']<=31)): rawDataFixed = numpy.roll(rawDataFixed,2071)      
       if fill==6364:
        if((row['runnum']>306155 and row['runnum']<306169) or (row['runnum']==306155 and row['lsnum']>=1508) or (row['runnum']==306169 and row['lsnum']<=2)): rawDataFixed = numpy.roll(rawDataFixed,188)
      numorbits = math.ceil(float(max(rawDataFixed))/1024.)*1024.
      #Default measurements
      sbil_ch[int(row['channelid'])] = -1 * k * numpy.log([float(x)/numorbits for x in rawDataFixed])
      lumi_orbit_ch_ = sum(sbil_ch[int(row['channelid'])])
      lumi_orbit_ch[int(row['channelid'])].append(lumi_orbit_ch_)
      #Weighted measurements
      mu = -1 * k * numpy.log([float(x)/numorbits for x in rawDataFixed]) 
      eff = leading/float(fills_effs[fill][int(row['channelid'])])+train/float(fills_effs[fill][int(row['channelid'])])
      slope = leading*slope_L[int(row['channelid'])]/100+train*slope_T[int(row['channelid'])]/100        
      mu = mu*eff
      mu = mu-mu*mu*slope
      sbil_ch_w[int(row['channelid'])] = mu
      lumi_orbit_ch_w_ = sum(sbil_ch_w[int(row['channelid'])])
      lumi_orbit_ch_w[int(row['channelid'])].append(lumi_orbit_ch_w_)
     if(int(row['channelid'])==nchannels-1):
      sbil_avg = (sbil_ch.sum(axis=0))/numeffectivech
      lumi_orbit_avg_ = sum(sbil_avg)
      lumi_orbit_avg.append(lumi_orbit_avg_)
      sbil_avg_w = (sbil_ch_w.sum(axis=0))/numeffectivech
      lumi_orbit_avg_w_ = sum(sbil_avg_w)
      lumi_orbit_avg_w.append(lumi_orbit_avg_w_)
      #Write new data
      if(channel_counter/nchannels<h5in.root.pltlumizero.nrows):
       oldrow = h5in.root.pltlumizero[channel_counter/nchannels]
       rownew['fillnum'] = oldrow['fillnum'] 
       rownew['runnum'] = oldrow['runnum']
       rownew['lsnum'] = row['lsnum']
       rownew['nbnum'] = row['nbnum']
       rownew['timestampsec'] = oldrow['timestampsec']
       rownew['timestampmsec'] = oldrow['timestampmsec']
       rownew['totsize'] = oldrow['totsize'] 
       rownew['publishnnb'] = oldrow['publishnnb'] 
       rownew['datasourceid'] = oldrow['datasourceid'] 
       rownew['algoid'] = oldrow['algoid']  
       rownew['channelid'] = oldrow['channelid']
       rownew['payloadtype'] = oldrow['payloadtype'] 
       rownew['calibtag'] = oldrow['calibtag'] 
      else:
       oldrow = h5in.root.pltlumizero[h5in.root.pltlumizero.nrows-1]
       rownew['fillnum'] = row['fillnum'] 
       rownew['runnum'] = row['runnum']
       rownew['lsnum'] = row['lsnum']
       rownew['nbnum'] = row['nbnum']
       rownew['timestampsec'] = row['timestampsec']
       rownew['timestampmsec'] = row['timestampmsec']
       rownew['totsize'] = oldrow['totsize'] 
       rownew['publishnnb'] = row['publishnnb'] 
       rownew['datasourceid'] = row['datasourceid'] 
       rownew['algoid'] = row['algoid']  
       rownew['channelid'] = row['channelid']
       rownew['payloadtype'] = oldrow['payloadtype'] 
       rownew['calibtag'] = oldrow['calibtag'] 
      rownew['avgraw'] = lumi_orbit_avg_w_/k 
      rownew['avg'] = lumi_orbit_avg_w_ 
      rownew['bxraw'] = sbil_avg_w/k 
      rownew['bx'] = sbil_avg_w
      rownew['maskhigh'] = maskhigh
      rownew['masklow'] = masklow
      rownew.append()
    channel_counter = channel_counter+1
    c = c+1
    #Print entries to keep track of the execution
    if (c%4000 == 0):
     print c
   h5in.close()
   h5out.close()

   ### --- Plot the results ---
   #NB this part has been recently introduced and somethin went wrong as it seems that if you run over more fills it saves the luminosity of all the analyzed fills in the last produced plots
   #E.g. see newhd517pp/Lumi_6371_0_-1.png and newhd517pp/LumiWeighted_6371_0_-1.png
   #This has to be fixed and you might want to run over at least 2 fills to reproduce this effect and fix it, even though it might just be that you need to re-initialize/clear the quantities that store the plotted luminosity
   #Also change the name in something like Fill_6371_Lumi.png and Fill_6371_LumiWeighted.png 
   #colors = ["k.-","g.-","r.-","b.-","c.-","m.-","g^-","r^-","b^-","c^-","m^-","gs-","rs-","bs-","cs-","ms-"]
   colors = ["b","b","b","b","b","b","b","b","b","b","b","b","b","b","b","b"]
   py.style.use('seaborn-white')
   fig = py.figure(1)
   py.title("Fill %s, $\sqrt{s}$ = 13 TeV" % (fill), fontsize=60)
   ax = fig.add_subplot(111)
   py.text(0.05, 0.925,'CMS', ha='center', va='center', transform=ax.transAxes, fontsize=60, weight='bold') 
   py.text(0.180, 0.925,'Preliminary', ha='center', va='center', transform=ax.transAxes, fontsize=60, style='italic')
   py.text(0.31, 0.925,'2017', ha='center', va='center', transform=ax.transAxes, fontsize=60) #, weight='bold') 
   lumi_orbit_ActiveCh = [loac for loac in lumi_orbit_ch if loac]
   y_max = 1.5*max((max(m) for m in lumi_orbit_ActiveCh))
   py.ylim((0,y_max))
   isNotFirst=False
   for i in range(0,nchannels):
    if(i not in disablech):
     if isNotFirst: py.plot(range(int(len(lumi_orbit_ch[i]))),lumi_orbit_ch[i],colors[i],label="")
     else: py.plot(range(int(len(lumi_orbit_ch[i]))),lumi_orbit_ch[i],colors[i],label="PLT channels")
     isNotFirst=True
     py.legend(loc='upper right', frameon=False, prop={'size': 70})
     py.xlabel('Time (sec)',size=60)#, horizontalalignment='right', x=1.0)
     py.ylabel('PLT luminosity (Hz/$\mu$b)',size=60)#, verticalalignment='top')
     py.tick_params(axis='x', labelsize=40)
     py.tick_params(axis='y', labelsize=40)
     mng = py.get_current_fig_manager()
     mng.window.showMaximized() # QT backend
     #mng.resize(*mng.window.maxsize()) # TkAgg backend
     #mng.frame.Maximize(True) # WX backend
   name = "Lumi_"+str(fill)+"_"+str(ini)+"_"+str(entries)+".png"
   fig.set_size_inches((32, 18))#Number are W and H respectively, here adjusted to fit screen size
   fig.savefig(name, bbox_inches='tight')
   
   fig = py.figure(2)
   py.title("Fill %s, $\sqrt{s}$ = 13 TeV" % (fill), fontsize=60)
   ax = fig.add_subplot(111)
   py.text(0.05, 0.925,'CMS', ha='center', va='center', transform=ax.transAxes, fontsize=60, weight='bold') 
   py.text(0.180, 0.925,'Preliminary', ha='center', va='center', transform=ax.transAxes, fontsize=60, style='italic')
   py.text(0.31, 0.925,'2017', ha='center', va='center', transform=ax.transAxes, fontsize=60) #, weight='bold') 
   py.ylim((0,y_max))
   isNotFirst=False
   for i in range(0,nchannels):
    if(i not in disablech):
     if isNotFirst: py.plot(range(int(len(lumi_orbit_ch_w[i]))),lumi_orbit_ch_w[i],colors[i],label="")
     else: py.plot(range(int(len(lumi_orbit_ch_w[i]))),lumi_orbit_ch_w[i],colors[i],label="PLT channels")
     isNotFirst=True
     py.legend(loc='upper right', frameon=False, prop={'size': 70})
     py.xlabel('Time (sec)',size=60)#, horizontalalignment='right', x=1.0)
     py.ylabel('Reweighted PLT luminosity (Hz/$\mu$b)',size=60)#, verticalalignment='top')
     py.tick_params(axis='x', labelsize=40)
     py.tick_params(axis='y', labelsize=40)
     mng = py.get_current_fig_manager()
     mng.window.showMaximized() # QT backend
     #mng.resize(*mng.window.maxsize()) # TkAgg backend
     #mng.frame.Maximize(True) # WX backend
   name = "LumiWeighted_"+str(fill)+"_"+str(ini)+"_"+str(entries)+".png"
   fig.set_size_inches((32, 18))#Number are W and H respectively, here adjusted to fit screen size
   fig.savefig(name, bbox_inches='tight')
   
   #py.show()
