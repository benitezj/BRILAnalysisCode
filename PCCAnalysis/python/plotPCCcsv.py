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

##run_number = [317080, 317087, 317088, 317089, 317170, 317182, 317212, 317213, 317239, 317279,317291, 317292, 317295, 317296, 317297, 317319, 317320,317338, 317339, 317340, 317382, 317383, 317391, 317392, 317434, 317435, 317438, 317475, 317478, 317479, 317480, 317481, 317482, 317484, 317488, 317509, 317510, 317511, 317512, 317527, 317580, 317591, 317626, 317640, 317641, 317648,317649, 317650, 317661, 317663, 317683, 317696, 318070, 318622, 318653, 318661, 318662, 318663, 318667, 318669, 318670, 318675, 318712,318714, 318733, 318734, 318785,318816, 318817, 318819, 318820, 318828, 318834, 318836, 318837, 318872, 318874, 318876, 318877, 318939, 318944, 318945, 318953, 318980, 318981, 318982, 318983, 318984, 318992, 319006, 319011, 319015, 319016, 319018, 319019, 319077, 319097, 319098, 319099, 319100, 319101, 319103, 319104, 319124, 319125, 319159, 319160, 319173, 319174, 319175, 319176, 319177, 319190, 319222, 319223, 319254, 319255, 319256, 319260, 319262, 319263, 319264, 319265, 319266, 319267, 319268, 319270, 319273, 319274, 319300, 319310, 319311]
run_number =[317080]
print len(run_number)

for i in range(len(run_number)):
      file = open(str("/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018B/")+str(run_number[i])+str(".csv"), "r")
data = list(csv.reader(file, delimiter=","))
for lines in data:
        print(lines)
file.close()
##print(data)

for i in range(len(run_number)):
      file1 = open(str("/eos/user/a/asehrawa/PCC/EXPRESS_datasets/ZeroBias/Run2018_ZB_test/Run2018B/")+str(run_number[i])+str(".hfoc"), "r")
data1 = list(csv.reader(file1, delimiter=","))
for lines1 in data1:
        print(lines1)
file1.close()
##print(data1)

with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json', "r") as f:
    data = json.load(f)
    for line in data:
        if not line[0].startswith("hfoc"):
            continue
        for run, ranges in line[1].items():
            for ls1, ls2 in ranges:
                print run, ls1, ls2


C1=ROOT.TCanvas("C1","",2000,1000)
h_ratiovsHF = ROOT.TH2F("h_ratiovsHF", "lumi ratio vs HF histogram", 28000, 0.0, 23000, 28000, 0.0, 1.4);
for i in range(len(run_number)):
      sigmavis=getSigmaVis(run_number[i]);
      h_ratiovsHF.Fill(refLumi[ls1], cluster_count[ls1]/(sigmavis*refLumi[ls1]))


h_ratiovsHF.SetMarkerStyle(20)
h_ratiovsHF.SetMarkerColor(46)
h_ratiovsHF.SetStats(0)
h_ratiovsHF.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC.png')
C1.Clear()




## run, ls, bx1, bx2,............, bx2556
