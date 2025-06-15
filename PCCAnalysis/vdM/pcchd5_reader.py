import ROOT
import tables as t, pandas as pd, pylab as py, sys, numpy, math, os
import struct
import os,sys
import argparse
ROOT.gROOT.SetBatch(True) ## run with batch mode always

parser = argparse.ArgumentParser(description='Process entries in event-based trees to produce pixel cluster counts')
parser.add_argument('--inputfile', type=str, default="", help='The input hd5 file')
parser.add_argument('--outpath', type=str, default=".", help='The path to  write output')
args = parser.parse_args()

input_file=args.inputfile 

#2017
pcc_bx=[41] #,281,872,1783,2063]
#tmin=1501226983 
#tmax=tmin+8000 #cover vdM scans only
tmin=1501226983+1950 #first X scan
tmax=tmin+900 


#2018
#pcc_bx=[265,865,1780,2192,3380]

#2022
#pcc_bx = [282,822,2944,3123,3302]
#tmin=1668123000
#tmax=1668123440

#pcc_bx = [67,74,98,858,2643]

#2018
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1530412200, 1530413200 , 300,0,50) #SS Period 1  #WIDE
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",10,1530412500, 1530412800 , 300,0,5) #SS Period 1
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1530426850, 1530427680 , 300,0,60) #SS Period 2 #WIDE
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",10,1530427080, 1530427440 , 300,0,5) #SS Period 2

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
# h2d=ROOT.TH2F('h2d_'+str(bx)," ",100,1668228076,1668234746,300,0,3500)


####################
TMIN=1e11
TMAX=0
data_bx_tool=numpy.zeros(3564)
c2=ROOT.TCanvas("c2","",1200,600)

for bx in pcc_bx:
 h5in = t.open_file(input_file,mode='r')
 table = h5in.root.pcchd5
 
 h2d=ROOT.TH2F('h2d_'+str(bx)," ",int((tmax-tmin)/5),0,tmax-tmin,1000,0,100)

 for row in table.iterrows():
  if row['timestampsec'] <= 0:
   continue;
  
  if row['timestampsec'] < TMIN:
   TMIN = row['timestampsec']
  if row['timestampsec'] > TMAX:
   TMAX = row['timestampsec']
   
  if row['timestampsec']>=tmin and row['timestampsec']<=tmax:
   data_bx_tool=row['bxraw']
   h2d.Fill(row['timestampsec']-tmin,data_bx_tool[int(bx)])
   data_bx_tool=numpy.zeros(3564)
   
 h5in.close()


# h2d.GetXaxis().SetTitle("Time [s]")
# h2d.GetYaxis().SetTitle("PCC")
# c2.Clear()
# h2d.SetMarkerStyle(20)
# h2d.SetMarkerColor(46)
# h2d.SetStats(0)
# h2d.Draw("colz")
# c2.Print('./BX_'+str(bx)+'_Rates_10p.png')
 
# c2.Clear()
# prjctn= h2d.ProjectionY() 
# c2.SetLogy()
# prjctn.Draw()
# c2.Print('./BX_'+str(bx)+'_proj.png')
# c2.SetLogy(0)
# c2.Clear()


 P=h2d.ProfileX()
 P.SetStats(0)
 P.SetMarkerStyle(8)
 P.SetMarkerSize(.6)
 P.SetMarkerColor(1)
 P.SetTitle('BCID: '+str(bx))
 P.GetYaxis().SetTitle("PCC")
 P.GetXaxis().SetTitle("Time [s]")
 for b in range(1,P.GetXaxis().GetNbins()+1):
  if P.GetBinContent(b) > 0.  and P.GetBinError(b)/P.GetBinContent(b) > 0.2:
   P.SetBinError(b,0)
   P.SetBinContent(b,0)
   
 c2.Clear()
 c2.SetLogy(0)
 P.Draw("histp")


 fit=ROOT.TF1("fit","[0]*exp(-0.5*(x-[1])**2/[2]**2)+[3]",0,900)
 fit.SetParameters(P.GetMaximum(),P.GetMean(),P.GetRMS())
 #fit.SetParLimits(0,P.GetMaximum()*0.9,P.GetMaximum()*1.2)
 #fit.SetParLimits(1,P.GetMean()-500,P.GetMean()+500)
 #fit.SetParLimits(2,P.GetRMS()*0.1,P.GetRMS()*2)

 #calculate the background from the tail
 bkg=0.
 bkgc=0
 for b in range(1,P.GetXaxis().GetNbins()+1):
  if P.GetBinCenter(b)< 50 or P.GetBinCenter(b)>850:
   bkg+=P.GetBinContent(b)
   bkgc+=1
 bkg/=bkgc
 fit.FixParameter(3,bkg)

 
 P.Fit(fit,"LQ","l",0,900)
 fit.Draw("lsame")
 txt=ROOT.TText()
 txt.DrawTextNDC(0.7,0.85,'Mean {:.1f}'.format(fit.GetParameter(1)))
 txt.DrawTextNDC(0.7,0.80,'Sigma {:.1f}'.format(fit.GetParameter(2)))
 txt.DrawTextNDC(0.7,0.75,'Const {:.3f}'.format(fit.GetParameter(3)))
 c2.Print(args.outpath+'/BX_'+str(bx)+'_ProfX.png')
 


print("T min = ",TMIN)
print("T max = ",TMAX)
