import ROOT
import struct
import os,sys
from ROOT import TFile, TTree, TChain
import sys,os
import numpy
import array
import math
import argparse
import csv
import json

C1=ROOT.TCanvas("C1","",2000,1000)
h_ratio = ROOT.TH2F("h_ratio", "PCC/HFOC vs lumi section histogram ", 200, 0.0, 70000, 300, 0.3, 2)                  
h_ratiovsHF = ROOT.TH2F("h_ratiovsHF", "PCC/HFOC vs HFOC Histogram", 200, 0.0, 23000, 200, 0.0, 2)
PCCvsHFOC=ROOT.TH2F("h_PCCvsHFOC", "PCC vs HFOC Histogram", 200, 0.0, 30000, 200, 0.0, 30000)
ProfX_h_ratiovsHF_residual=ROOT.TGraph()
lumisec_count=0


with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json', "r") as HFOC_JSON:
    data = json.load(HFOC_JSON)
    ##print(data)
with open("hfoc_normtag.txt", "w") as HFOC_JSON:
    for line in data:
        if not line[0].startswith("hfoc"):
            continue
        for run1, ranges in line[1].items():
            for ls1, ls2 in ranges:
                HFOC_JSON.write("{}; {}; {};\n".format(run1, ls1, ls2))
                ##print(int(run1), int(ls1), int(ls2))                    

with open("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_hfoc_plots/EXPRESS_datasets/Run2018_ZB_test/Run2018B/ls.dat", "r") as datFile:
      for data in datFile:
            run=data.split()[0]
            ls=data.split()[1]
            PCC_count=data.split()[2]
            HFOC_count=data.split()[3]
            ##print(int(run), int(ls), float(PCC_count), float(HFOC_count))
            if float(HFOC_count) !=0:
                  for ls1, ls2 in ranges:
                      ##if ((int(run)==int(run1)) or (int(ls1)<=int(ls)<=int(ls2))):
                      h_ratiovsHF.Fill(float(HFOC_count), float(PCC_count)/float(HFOC_count))    
                      h_ratio.Fill(int(lumisec_count), float(PCC_count)/float(HFOC_count))
                      PCCvsHFOC.Fill(float(PCC_count), float(HFOC_count))
                      lumisec_count=lumisec_count+1
                      ##print(int(run), int(run1), int(ls), int(ls1), int(ls2), int(lumisec_count), float(PCC_count), float(HFOC_count))


ProfX_h_ratiovsHF=h_ratiovsHF.ProfileX()
fitfn = ROOT.TF1("fitfn","[0]*x+[1]",0,25000);
ProfX_h_ratiovsHF.Fit("fitfn")
ProfX_PCCvsHFOC=PCCvsHFOC.ProfileX()
fitfn1 = ROOT.TF1("fitfn1","[0]*x+[1]",0,25000);
ProfX_PCCvsHFOC.Fit("fitfn1")

##for i in ls:
  ##  if fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(i)))!=0:
    ##    ProfX_h_ratiovsHF_residual.SetPoint(ProfX_h_ratiovsHF_residual.GetN(), ProfX_h_ratiovsHF.GetBinCenter(int(i)), ((ProfX_h_ratiovsHF.GetBinContent(int(i)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(i))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(i))));

h_ratiovsHF.SetMarkerStyle(20)
h_ratiovsHF.SetMarkerColor(46)
h_ratiovsHF.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC.png')
h_ratio.SetMarkerStyle(20)
h_ratio.SetMarkerColor(46)
h_ratio.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsls.png')
PCCvsHFOC.SetMarkerStyle(20)
PCCvsHFOC.SetMarkerColor(46)
PCCvsHFOC.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC.png')
ProfX_h_ratiovsHF.SetMarkerStyle(20)
ProfX_h_ratiovsHF.SetMarkerColor(46)
ProfX_h_ratiovsHF.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC_ProfileX.png')
ProfX_PCCvsHFOC.SetMarkerStyle(20)
ProfX_PCCvsHFOC.SetMarkerColor(46)
ProfX_PCCvsHFOC.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC_ProfileX.png')
ProfX_h_ratiovsHF_residual.SetMarkerStyle(20)
ProfX_h_ratiovsHF_residual.SetMarkerColor(46)
ProfX_h_ratiovsHF_residual.Draw("AP")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC_ProfileX_residuals.png')
C1.Clear()



