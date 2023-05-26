import ROOT
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, Queue, csv
import struct
import os,sys



#Input hd5 file:
#input_file='./pcc_ZB.hd5'
#input_file='/eos/user/l/lcuevasp/BRIL/PCC/VDM_2022/PCC2022_8385/PCC_merge_2022_8385_veto_2022.hd5'
#input_file='/eos/user/l/lcuevasp/BRIL/PCC/VDM_2022/PCC2022_8381_new_veto/PCC_merge_2022_8381_veto_2022.hd5'
#input_file='/eos/user/l/lcuevasp/BRIL/PCC/VDM_2022/PCC2022_8385/ZB0/pcc_Data_PixVtx_Event_90X_50.hd5'
input_file='/eos/cms/store/group/dpg_bril/comm_bril/vdmdata/2022/PCCreprocessed/8385/PCC_merge_2022_8385_veto_2022.hd5'

 #1668123900,1668124500
data_bx_tool=numpy.zeros(3564)


data_bx=-10
c2=ROOT.TCanvas("c2","",2000,1000)



#pcc_bx=[265,865,1780,2192,3380]
#pcc_bx=[41,281,872,1783,2063]
#pcc_bx = [282,822,2944,3123,3302]
pcc_bx = [67,74,98,858,2643]
#pcc_bx = range(3564)
for bx in pcc_bx:


 h5in = t.open_file(input_file,mode='r')
 #table = h5in.root.pcclumi #prev
 table = h5in.root.pcchd5
 rownum=0

# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1530412200, 1530413200 , 300,0,50) #SS Period 1  #WIDE
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",10,1530412500, 1530412800 , 300,0,5) #SS Period 1
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1530426850, 1530427680 , 300,0,60) #SS Period 2 #WIDE
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",10,1530427080, 1530427440 , 300,0,5) #SS Period 2
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",10,1530359080, 1530366066 , 300,0,5)
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",10,1530358152, 1530424196, 300,0,5)
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1501241804,1501242110, 300,0,70) #2017 1501228804,1501272046
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1501228804,1501231000,300,0,70) #VdM1
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1501231500,1501233826,300,0,70) #VdM2
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1501235277,1501237885,300,0,70) #Img1
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1501238538,1501241112,300,0,70) #Img2
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1501245536,1501247956,300,0,70) #VdM3
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1501269766,1501272046,300,0,70) #VdM4
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1663979280,1663996793,300,0,100) #2022

#2022 (8178)

# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100, 1664020000,1664020380,300,0,100) #SS Period 1  #WIDE
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",10, 1664020038,1664020354,300,0,5) #SS Period 1
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100, 1664049780,1664050320,300,0,100) #SS Period 2 #WIDE
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",10, 1664049900,1664050200,300,0,5) #SS Period 2
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100, 1664121660,1664122020,300,0,100) #SS Period 3 #WIDE                    
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",10, 1664049900,1664050200,300,0,5) #SS Period 3

#2022 (8381)
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1668123000,1668123440,300,0,100) #SS Period 1  #WIDE 
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",10,1668123050,1668123370,300,0,5) #SS Period 1 
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1668167170,1668167590,300,0,100) #SS Period 2 #WIDE
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",10,1668167215,1668167550,300,0,5) #SS Period 2

# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1668229200,1668234600,300,0,50) #VdM1
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1668236400,1668246400,300,0,1)
#1668150570,1668153060 #VdM2           1668236400,1668246400
#1668122160,1668132060 #first
#1668150600,1668127860, #second
 h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1668228076,1668234746,300,0,3500)
# h1d=ROOT.TH1F ("h1d","clusters",100,50,50);
#1668228519
#1668230056   1668228076,1668234746

 h2d.GetXaxis().SetTitle("Time (s)")
 h2d.GetYaxis().SetTitle("PCC per NB4")
#1664010800,1664134000 total
#1663976311,1663978580
#1663979280,1663981400
#1663994501,1663996793
#1664005680,1664007960
#1664014560,1664016860
#1664030467,1664032667
#1664094242,1664096442
  
 a = []
 for row in table.iterrows(): 
  if row['timestampsec'] != 0:
   a.append(row['timestampsec'])
 #data_bx=row['bxraw']
  if row['timestampsec']>=1668228076 and row['timestampsec']<=1668234746:
 # if row['timestampsec']>=1501240000 and row['timestampsec']<=1501260073: #2017     # CHANGE TIMES!!
   #print row['fillnum'], row['runnum'], row['lsnum'], row['nbnum'], row['timestampsec'], row['bx'], row['bxraw'], row['avgraw'], row['avg']
   #print(row['bxraw'])
   data_bx_tool=row['bxraw']
   data_bx=data_bx_tool[int(bx)]
  # print(row['timestampsec'],data_bx)
  # h1d.Fill(data_bx)
   h2d.Fill(row['timestampsec'],data_bx)
   data_bx_tool=numpy.zeros(3564)
   data_bx=-10
   rownum=rownum+1
   
 h5in.close()

 #print min(a)
 #print max(a)



 c2.Clear()
 h2d.SetMarkerStyle(20)
 h2d.SetMarkerColor(46)
 h2d.SetStats(0)
# h2d.SetAxisRange(0., 40.,"Y")
 h2d.Draw("colz")#"colz")
 c2.Print('/eos/user/l/lcuevasp/BRIL/PCC/VDM_2022/PCC2022_8385/reader/BX_'+str(bx)+'_Rates_10p.png')
 
  
###
 c2.Clear()
 P=ROOT.TProfile
 c2.SetLogy(0)
 P=h2d.ProfileX()
 P.SetStats(0)
 P.SetMarkerStyle(16)
 P.SetMarkerColor(9)
 P.GetYaxis().SetTitle("Average PCC")
# P.SetAxisRange(0., 40.,"Y")
 P.Draw()
 c2.Print('/eos/user/l/lcuevasp/BRIL/PCC/VDM_2022/PCC2022_8385/reader/BX_'+str(bx)+'_RatesAvg_10p.png')
 

 c2.Clear()
 prjctn= h2d.ProjectionY() 
 c2.SetLogy()
 #prjctn.SetStats(0)
 prjctn.Draw()
 c2.Print('/eos/user/l/lcuevasp/BRIL/PCC/VDM_2022/PCC2022_8385/reader/BX_'+str(bx)+'_proj.png')
 c2.SetLogy(0)
 c2.Clear()

 #c2.Clear()                                                                                                          
 #h1d.Draw()                                                                                                        
 #c2.Print('/eos/user/l/lcuevasp/BRIL/PCC/VDM_2022/PCC2022_8385/reader/BY_'+str(bx)+'_proj.png')                
 #c2.Clear()   
