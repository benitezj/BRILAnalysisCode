import ROOT
import tables as t, pandas as pd, os, sys,argparse
ROOT.gROOT.SetBatch(True) ## run with batch mode always
NBX=3564

parser = argparse.ArgumentParser(description='Process entries in event-based trees to produce pixel cluster counts')
parser.add_argument('--inputfile', type=str, default="./pcc_ZB.hd5", help='full path to input file')
parser.add_argument('--outpath', type=str, default=".", help='full path to output dir')
args = parser.parse_args()

output_path=args.outpath
input_file=args.inputfile

##automatically determine parameters for this scan
run=0  # run number 
pcc_bx=[] # list of bcids in this file
LSmin=99999 # first LS
LSmax=0   # last LS
h5in = t.open_file(input_file,mode='r')
table = h5in.root.pcchd5
for row in table.iterrows():
  if LSmin>row['lsnum']:
    LSmin=row['lsnum']
  if LSmax<row['lsnum']:
    LSmax=row['lsnum']

  for b in range(NBX):
    if row['bxraw'][b] > 0 and pcc_bx.count(b)==0:
      pcc_bx.append(b)

  if run == 0:
    run = row['runnum']
  if run !=0 and run != row['runnum']:
    print('ERROR: this file has multiple runs !!')
    exit(0)
    
h5in.close()
print('Run: '+str(run))
print('LS: '+str(LSmin)+' - '+str(LSmax))

pcc_bx.sort()
print('bcids:',pcc_bx)



bcidliststr='Run: '+str(run)+', BCID: '
h1d=[]
for bx in pcc_bx:
  h1d.append(ROOT.TH1F('h1d_'+str(bx)," ",(LSmax-LSmin+1)*16,LSmin,LSmax+1))
  bcidliststr += str(bx)+','


c2=ROOT.TCanvas("c2","",800,600)
c2.Clear()

bxc=0
for bx in pcc_bx: 
 #h1d[bxc] = ROOT.TH1F('h1d_'+str(bx)," ",(LSmax-LSmin+1)*16,LSmin,LSmax+1)

 h5in = t.open_file(input_file,mode='r')
 table = h5in.root.pcchd5
 for row in table.iterrows():
   h1d[bxc].Fill(row['lsnum']+row['nbnum']/16,row['bxraw'][bx])  
 h5in.close()

 color=bxc+1
 if color >=10: # color 10-19 are white - grey
   color += 10
   
 h1d[bxc].SetMarkerStyle(8)
 h1d[bxc].SetMarkerColor(color)
 h1d[bxc].SetLineColor(color)
 h1d[bxc].SetLineWidth(2)
 
 if bxc==0 :
   h1d[bxc].SetTitle(bcidliststr)
   h1d[bxc].GetXaxis().SetTitle("lumi section")
   h1d[bxc].GetYaxis().SetTitle("<PCC>")
   h1d[bxc].GetYaxis().SetRangeUser(0,1000)
   h1d[bxc].SetStats(0)
   h1d[bxc].Draw("histl")
 else:
   h1d[bxc].Draw("histlsame")

 bxc+=1
   
c2.Print(output_path+'/pcchd5_reader_emmittance.png')
