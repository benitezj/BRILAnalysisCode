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
lumisec_count=0

with open("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_hfoc_plots/EXPRESS_datasets/Run2018_ZB_test/Run2018B/ls.dat", "r") as datFile:
      with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json', "r") as f:
            content = json.load(f)
            print(content)
            for data in datFile:
                  run=data.split()[0]
                  ls=data.split()[1]
                  PCC_count=data.split()[2]
                  HFOC_count=data.split()[3]
                  print(run, ls, PCC_count, HFOC_count)
                  with open("hfoc_json.txt", "w") as f:
                        for line in content:
                              if not line[0].startswith("hfoc"):
                                    continue
                                    for run1, ranges in line[1].items():
                                          for ls1, ls2 in ranges:
                                                f.write("{}; {}; {};\n".format(run1, ls1, ls2))
                                                print(int(run1), int(ls1), int(ls2))
                                                if float(HFOC_count) !=0:
                                                      if ((int(run)==int(run1)) or (int(ls1)<=int(ls)<=int(ls2))):
                                                            h_ratiovsHF.Fill(float(HFOC_count), float(PCC_count)/float(HFOC_count))    
                                                            h_ratio.Fill(int(lumisec_count), float(PCC_count)/float(HFOC_count))
                                                            lumisec_count=lumisec_count+1
                                                            print(run, run1, ls, ls1, ls2, lumisec_count, PCC_count, HFOC_count)
h_ratiovsHF.SetMarkerStyle(20)
h_ratiovsHF.SetMarkerColor(46)
h_ratiovsHF.SetStats(0)
h_ratiovsHF.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC.png')
h_ratio.SetMarkerStyle(20)
h_ratio.SetMarkerColor(46)
h_ratio.SetStats(0)
h_ratio.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsls.png')
C1.Clear()



