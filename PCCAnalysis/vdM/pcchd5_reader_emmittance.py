import ROOT
import tables as t, pandas as pd, os, sys,argparse
ROOT.gROOT.SetBatch(True) ## run with batch mode always


parser = argparse.ArgumentParser(description='Process entries in event-based trees to produce pixel cluster counts')
parser.add_argument('--inputfile', type=str, default="./pcc_ZB.hd5", help='full path to input file')
parser.add_argument('--outpath', type=str, default=".", help='full path to output dir')
args = parser.parse_args()

output_path=args.outpath
input_file=args.inputfile

#pcc_bx=[57,67,92,919,951,961,986,1998,2727,3272] #2025 2400b fill gated bcids
pcc_bx=[56]

##find the min and max LS
LSmin=99999
LSmax=0
h5in = t.open_file(input_file,mode='r')
table = h5in.root.pcchd5
for row in table.iterrows():
  if LSmin>row['lsnum']:
    LSmin=row['lsnum']
  if LSmax<row['lsnum']:
    LSmax=row['lsnum']
h5in.close()
print('LS range: '+str(LSmin)+' - '+str(LSmax))


c2=ROOT.TCanvas("c2","",800,600)

bxc=0
for bx in pcc_bx:
 bxc+=1
 
 h2d=ROOT.TH2F('h2d_'+str(bx)," ",(LSmax-LSmin+1)*16,LSmin,LSmax+1,100,0,2000)
 h5in = t.open_file(input_file,mode='r')
 table = h5in.root.pcchd5
 for row in table.iterrows():
   #print(row['lsnum']+row['nbnum']/16,row['bxraw'][bx])
   h2d.Fill(row['lsnum']+row['nbnum']/16,row['bxraw'][bx])  
 h5in.close()

 h2d.GetXaxis().SetTitle("lumi section")
 h2d.GetYaxis().SetTitle("<PCC>")
 h2d.SetMarkerStyle(8)
 #h2d.SetStats(0)
 c2.Clear()
 h2d.Draw("histscat")
 c2.Print(output_path+'/BX_'+str(bx)+'.png')

 P=h2d.ProfileX()
 P.SetStats(0)
 P.SetMarkerStyle(8)
 P.SetMarkerColor(bxc)
 P.SetLineColor(bxc)
 P.GetYaxis().SetTitle("<PCC>")
 c2.Clear()
 P.Draw('histpe')
 c2.Print(output_path+'/BX_'+str(bx)+'_RatesAvg.png')
 
