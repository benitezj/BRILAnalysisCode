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
ProfX_PCCvsHF_residual=ROOT.TGraph()
lumisec_count=0
LS=0

with open("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_hfoc_plots/EXPRESS_datasets/Run2018_ZB_test/Run2018B/ls.dat", "r") as datFile:
    with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json', "r") as HFOC_JSON:
        json_data = json.load(HFOC_JSON)
    for data in datFile:      
        run=data.split()[0]
        ls=data.split()[1]
        PCC_count=data.split()[2]
        HFOC_count=data.split()[3]
        for line in json_data:
            if not line[0].startswith("hfoc"):
                continue
            for run1 in line[1]:
                ##print (run1)
                ##print(line[1][run1])
                if 317653<=int(run)<=317663:                                ## Fill 6774
                    if int(run1)==int(run):                            
                        for ls1, ls2 in line[1][run1]:
                            if int(ls1)<=int(ls)<=int(ls2):
                                ##print(run, run1, ls, ls1, ls2)
                                LS=int(ls)+lumisec_count
                                lumisec_count=lumisec_count+1                          
                                if float(HFOC_count) !=0:
                                    h_ratiovsHF.Fill(float(HFOC_count), float(PCC_count)/float(HFOC_count))
                                    h_ratio.Fill(lumisec_count, float(PCC_count)/float(HFOC_count))
                                    PCCvsHFOC.Fill(float(PCC_count), float(HFOC_count))                
                                    ##print(lumisec_count, float(PCC_count)/float(HFOC_count))
                                    print int(run), int(run1), ls, ls1, ls2 
    
ProfX_h_ratiovsHF=h_ratiovsHF.ProfileX()
fitfn = ROOT.TF1("fitfn","[0]*x+[1]",0,25000);
ProfX_h_ratiovsHF.Fit("fitfn")
ProfX_PCCvsHFOC=PCCvsHFOC.ProfileX()
fitfn1 = ROOT.TF1("fitfn1","[0]*x+[1]",0,25000);
ProfX_PCCvsHFOC.Fit("fitfn1")

##for i in ls:
##    if fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(i)))!=0:
##        ProfX_h_ratiovsHF_residual.SetPoint(ProfX_h_ratiovsHF_residual.GetN(), ProfX_h_ratiovsHF.GetBinCenter(int(i)), ((ProfX_h_ratiovsHF.GetBinContent(int(i)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(i))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(i))));

##for i in ls:
##    if fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(i)))!=0:         
##        ProfX_PCCvsHF_residual.SetPoint(ProfX_PCCvsHF_residual.GetN(), ProfX_PCCvsHFOC.GetBinCenter(int(i)), ((ProfX_PCCvsHFOC.GetBinContent(int(i)))-fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(i))))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(i))));  

h_ratiovsHF.SetMarkerStyle(20)
h_ratiovsHF.SetMarkerColor(46)
h_ratiovsHF.GetXaxis().SetTitle("HF Inst. Lumi")
h_ratiovsHF.GetYaxis().SetTitle("PCC/HFOC")
h_ratiovsHF.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC.png')
h_ratio.SetMarkerStyle(20)
h_ratio.SetMarkerColor(46)
h_ratio.GetXaxis().SetTitle("Lumi section")
h_ratio.GetYaxis().SetTitle("PCC/HFOC")
h_ratio.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsls_true.png')
PCCvsHFOC.SetMarkerStyle(20)
PCCvsHFOC.SetMarkerColor(46)
PCCvsHFOC.GetXaxis().SetTitle("HF Inst. Lumi")
PCCvsHFOC.GetYaxis().SetTitle("PCC")
PCCvsHFOC.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC.png')
ProfX_h_ratiovsHF.SetMarkerStyle(20)
ProfX_h_ratiovsHF.SetMarkerColor(46)
ProfX_h_ratiovsHF.GetXaxis().SetTitle("HF Inst. Lumi")
ProfX_h_ratiovsHF.GetYaxis().SetTitle("PCC/HFOC")
ProfX_h_ratiovsHF.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC_ProfileX.png')
ProfX_PCCvsHFOC.SetMarkerStyle(20)
ProfX_PCCvsHFOC.SetMarkerColor(46)
ProfX_PCCvsHFOC.GetXaxis().SetTitle("HF Inst. Lumi")
ProfX_PCCvsHFOC.GetYaxis().SetTitle("PCC")
ProfX_PCCvsHFOC.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC_ProfileX.png')
ProfX_h_ratiovsHF_residual.SetMarkerStyle(20)
ProfX_h_ratiovsHF_residual.SetMarkerColor(46)
ProfX_h_ratiovsHF_residual.GetXaxis().SetTitle("(PCC-Fit)/Fit")
ProfX_h_ratiovsHF_residual.GetYaxis().SetTitle("Entries")
ProfX_h_ratiovsHF_residual.Draw("AP")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC_ProfileX_residuals.png')
ProfX_PCCvsHF_residual.SetMarkerStyle(20)
ProfX_PCCvsHF_residual.SetMarkerColor(46)
ProfX_PCCvsHF_residual.GetXaxis().SetTitle("(PCC-Fit)/Fit")
ProfX_PCCvsHF_residual.GetYaxis().SetTitle("Entries")
ProfX_PCCvsHF_residual.Draw("AP")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC_ProfileX_residuals.png')




