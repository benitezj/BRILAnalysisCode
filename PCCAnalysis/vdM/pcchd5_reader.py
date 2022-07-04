import ROOT
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os, Queue, csv
import struct
import os,sys



#Input hd5 file:
input_file='/eos/user/j/jmorenoc/PCC_hd5_fill6868/NewVeto/Period_A/PLTreprocessed/6868/6868_319019_PCC_NewVeto_Period_A.hd5'


###h5in = t.open_file(input_file,mode='r')
#table = h5in.root.pcclumi #prev
###table = h5in.root.pltlumizero

#j,i
#data_bx=numpy.zeros(3564)
#data_time=numpy.zeros(3564)



#N=140048 #->For SS1

#N=140048

#N=2912

data_bx_tool=numpy.zeros(3564)


#bx_2f=numpy.zeros(N)
#data_bx=numpy.zeros(N)
data_bx=-10
#data_time=numpy.zeros(N)
#ls_2f=numpy.zeros(N)



 #1530413316 ->Start LSC1 constant

 #1530411064,1530412028
c2=ROOT.TCanvas("c2","",2000,1000)



pcc_bx=[265,865,1780,2192,3380]

for bx in pcc_bx:
 ######TH2:

 h5in = t.open_file(input_file,mode='r')
 #table = h5in.root.pcclumi #prev
 table = h5in.root.pltlumizero
 rownum=0

# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1530412200, 1530413200 , 300,0,50) #SS Period 1  #WIDE
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",10,1530412500, 1530412800 , 300,0,5) #SS Period 1
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1530426850, 1530427680 , 300,0,60) #SS Period 2 #WIDE
 h2d=ROOT.TH2F('h2d_'+str(bx)," ",10,1530427080, 1530427440 , 300,0,5) #SS Period 2
 h2d.GetXaxis().SetTitle("Time (s)")
 h2d.GetYaxis().SetTitle("PCC per NB4")

 for row in table.iterrows(): 

 #data_bx=row['bxraw']
  if row['timestampsec']>=1530427080 and row['timestampsec']<=1530427440:# CHANGE TIMES!!
  ##print row['fillnum'], row['runnum'], row['lsnum'], row['nbnum'], row['timestampsec'], row['bx'], row['bxraw'], row['avgraw'], row['avg']
   data_bx_tool=row['bxraw']
  #data_time_tool=row['bxraw']
   data_bx=data_bx_tool[int(bx)]
  #data_time[rownum]=row['timestampsec']
   h2d.Fill(row['timestampsec'],data_bx)
   data_bx_tool=numpy.zeros(3564)
   data_bx=-10
  #data_time=0#numpy.zeros(3564)
   rownum=rownum+1
 #rownum=rownum+1
  ##print "row numer ", rownum
 h5in.close()
#rownum=0


####opening 2nd file
###h5in = t.open_file(input_file_7f,mode='r')
###table = h5in.root.pltlumizero
###bx_7f=numpy.zeros(N)
###ls_7f=numpy.zeros(N)
###
###data_bx=numpy.zeros(3564)
###for row in table.iterrows(): 
###
### #if row['timestampsec']>= 1530358114 and row['timestampsec']<= 1530360519: 
### data_bx=row['bxraw']
### #print row['fillnum'], row['runnum'], row['lsnum'], row['nbnum'], row['timestampsec'], row['bx'], row['bxraw'], row['avgraw'], row['avg']
### bx_7f[rownum]=data_bx[3380]
### ls_7f[rownum]=row['lsnum']
### ##h2d.Fill(data_time[3380],data_bx[3380])
### data_bx=numpy.zeros(3564)
### ##data_time=numpy.zeros(3564)
### rownum=rownum+1
### #rownum=rownum+1
### print "row numer ", rownum
###h5in.close()
###rownum=0
###
###data_bx=numpy.zeros(3564)
####opnening 3rd file:
###h5in = t.open_file(input_file_com,mode='r')
###table = h5in.root.pltlumizero
###bx_c=numpy.zeros(N)
###ls_c=numpy.zeros(N)
###
###for row in table.iterrows(): 
###
### #if row['timestampsec']>= 1530358114 and row['timestampsec']<= 1530360519: 
### data_bx=row['bxraw']
### #print row['fillnum'], row['runnum'], row['lsnum'], row['nbnum'], row['timestampsec'], row['bx'], row['bxraw'], row['avgraw'], row['avg']
### bx_c[rownum]=data_bx[3380]
### ls_c[rownum]=row['lsnum']
### ##h2d.Fill(data_time[3380],data_bx[3380])
### data_bx=numpy.zeros(3564)
### ##data_time=numpy.zeros(3564)
### rownum=rownum+1
### #rownum=rownum+1
### print "row numer ", rownum
###h5in.close()
###rownum=0
###
###
###
###
####CREAR TGRAPH
###
#outpng_Test='/eos/user/j/jmorenoc/DataToPlot_csv/plots-nCluster_vs_LS/Test_SS.png'
###
###c2=ROOT.TCanvas("c2","",3700,2300)
###mg = ROOT.TMultiGraph()
###
###
####1st file: 2files:
###tgraph_2files = ROOT.TGraph(N, ls_2f, bx_2f)
###tgraph_2files.SetName("tgraph_2files")
###tgraph_2files.SetTitle("tgraph_2files")
###tgraph_2files.SetMarkerStyle(20)
###tgraph_2files.SetMarkerColor(3)
###tgraph_2files.SetLineWidth(0)
###tgraph_2files.SetFillStyle(0)
###
###
####2nd file: 7files:
###tgraph_7files = ROOT.TGraph(N, ls_7f, bx_7f)
###tgraph_7files.SetName("tgraph_7files")
###tgraph_7files.SetTitle("tgraph_7files")
###tgraph_7files.SetMarkerStyle(21)
###tgraph_7files.SetMarkerColor(4)
###tgraph_7files.SetLineWidth(0)
###tgraph_7files.SetFillStyle(0)
###
####3rd file: complete file:
###tgraph_com = ROOT.TGraph(N, ls_c, bx_c)
###tgraph_com.SetName("tgraph_com")
###tgraph_com.SetTitle("tgraph_com")
###tgraph_com.SetMarkerStyle(23)
###tgraph_com.SetMarkerColor(2)
###tgraph_com.SetLineWidth(0)
###tgraph_com.SetFillStyle(0)
###
###mg.SetTitle(" BX 3380 ; LS ; Rates")
###mg.Add(tgraph_2files)
###mg.Add(tgraph_7files)
###mg.Add(tgraph_com)
###
####graph.Draw("AC*")
###mg.GetXaxis().SetRangeUser(1400008110,1600000510)
###mg.Draw("ALP")
###
####to add legend
###legend =  ROOT.TLegend(0.8,0.8,0.9,0.9)
###legend.AddEntry("tgraph_7files","7 files","p") 
###legend.AddEntry("tgraph_2files","2 Files","p") 
###legend.AddEntry("tgraph_com","Complete","p") 
###legend.SetBorderSize(1)
###legend.SetTextFont(42)
###legend.SetTextSize(0.03)
###legend.Draw()
###
###
###c2.Print(outpng_Test)
###c2.Clear()
###
####################################################################
 h2d.SetMarkerStyle(20)
 h2d.SetMarkerColor(46)
 h2d.SetStats(0)
 h2d.Draw("colz")#"colz")
 c2.Print('/eos/user/j/jmorenoc/Results_v1/NewVeto/Period_A/Background_Estimation/SS2/BX_'+str(bx)+'_Rates_10p.png')
###
 c2.Clear()
###
 P=ROOT.TProfile
###print "type of P1: ",type(P)
 P=h2d.ProfileX()
###print "type of P2 ", type(P)
 P.SetStats(0)
 P.SetMarkerStyle(16)
 P.SetMarkerColor(9)
 P.GetYaxis().SetTitle("Average PCC per NB4")
 P.Draw()
 c2.Print('/eos/user/j/jmorenoc/Results_v1/NewVeto/Period_A/Background_Estimation/SS2/BX_'+str(bx)+'_RatesAvg_10p.png')

 c2.Clear()
 prjctn= h2d.ProjectionY() 
 c2.SetLogy()
 #prjctn.SetStats(0)
 prjctn.Draw()
 c2.Print('/eos/user/j/jmorenoc/Results_v1/NewVeto/Period_A/Background_Estimation/SS2/BX_'+str(bx)+'_proj.png')
 c2.SetLogy(0)
 c2.Clear()

##To get min and max timestamp value
###print "max timestamp value:"
###print max(data_time)
###print "*****************"
######print "min timestampvalue"
###sasasas
######print min(data_time)

