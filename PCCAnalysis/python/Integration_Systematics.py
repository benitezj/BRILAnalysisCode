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
h_ratio = ROOT.TH2F("h_ratio", "PCC/HFOC vs lumi section histogram ", 500, 0.0, 50000, 500, 0, 2)                  
h_ratiovsHF = ROOT.TH2F("h_ratiovsHF", "PCC/HFOC vs HFOC Histogram", 200, 0.0, 23000, 200, 0.0, 2)
PCCvsHFOC=ROOT.TH2F("h_PCCvsHFOC", "PCC vs HFOC Histogram", 200, 0.0, 50000, 200, 0.0, 50000)
ProfX_h_ratiovsHF_residual=ROOT.TGraph()
ProfX_PCCvsHF_residual=ROOT.TGraph()
lumisec_count=0
lumisec_all=0
lumisec_good=0

with open("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_hfoc_plots/EXPRESS_datasets/Run2018_ZB_test/Run2018B/ls.dat", "r") as datFile:
        with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_pcc.json', "r") as PCC_JSON:
                json_data1 = json.load(PCC_JSON)
        with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json', "r") as HFOC_JSON:
                json_data = json.load(HFOC_JSON)
        for data in datFile:      
                run=data.split()[0]
                ls=data.split()[1]
                PCC_count=data.split()[2]
                HFOC_count=data.split()[3]
                lumisec_all=lumisec_all+1
                goodls=False
                for line1 in json_data1:
                        for run2 in line1[1]:
                                for line in json_data:
                                        if not line1[0].startswith("pcc") and line[0].startswith("hfoc"):
                                                continue
                                        for run1 in line[1]:
                                                if int(run)==int(run1)==int(run2):
                                                        ##print ('lsdat', run, 'pcc', int(run2), 'hfoc', int(run1)) 
                                                        ##print line[1][run1], line1[1][run2]
                                                        ##if 317653<=int(run)<=317663:                                ##Fill 6774
                                                        ##if int(run2)==317663: 
                                                        ##if int(run1)==int(run)==int(run2):
                                                        for ls3, ls4 in line1[1][run2]:
                                                                for ls1, ls2 in line[1][run1]:                            
                                                                        if int(ls1)<=int(ls)<=int(ls2) or int(ls3)<=int(ls)<=int(ls4):
                                                                                lumisec_good=lumisec_good+1
                                                                                goodls=True
                                                                                print int(run), int(run1), int(run2), int(ls), int(ls1), int(ls2), int(ls3), int(ls4)
        if goodls==True and float(HFOC_count) !=0: 
        ##and float(PCC_count)>=8000 and float(HFOC_count)>=8000:
                h_ratiovsHF.Fill(float(HFOC_count), float(PCC_count)/float(HFOC_count))
                h_ratio.Fill(lumisec_count+1, float(PCC_count)/float(HFOC_count))
                lumisec_count=lumisec_count+1                          
                PCCvsHFOC.Fill(float(PCC_count), float(HFOC_count))                
                ##print lumisec_count, int(ls), int(ls1), int(ls2), float(PCC_count)/float(HFOC_count)
                ##print int(run), int(run1), ls, ls1, ls2 
print ("all ls ", lumisec_all, "good ls ", lumisec_good)
ProjY_h_ratio=h_ratio.ProjectionY("Y Projection of PCC/HFOC vs ls", 0, 43989)    
ProfX_h_ratiovsHF=h_ratiovsHF.ProfileX()
fitfn = ROOT.TF1("fitfn","[0]*x+[1]",0,50000);
ProfX_h_ratiovsHF.Fit("fitfn")
ProfX_PCCvsHFOC=PCCvsHFOC.ProfileX()
fitfn1 = ROOT.TF1("fitfn1","[0]*x+[1]",0,50000);
ProfX_PCCvsHFOC.Fit("fitfn1")

## run loop, lumi section loop
with open("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_hfoc_plots/EXPRESS_datasets/Run2018_ZB_test/Run2018B/ls.dat", "r") as datFile:
    for data in datFile:
        run=data.split()[0]
        ls=data.split()[1]
        ##if int(run)==317663:
        if fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls)))!=0 and ProfX_h_ratiovsHF.GetBinContent(int(ls))!=0:
            ProfX_h_ratiovsHF_residual.SetPoint(ProfX_h_ratiovsHF_residual.GetN(), ProfX_h_ratiovsHF.GetBinCenter(int(ls)), ((ProfX_h_ratiovsHF.GetBinContent(int(ls)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))
            ProfX_h_ratiovsHF_residual.SetPointError(ProfX_h_ratiovsHF_residual.GetN(), 0, ProfX_h_ratiovsHF.GetBinError(int(ls))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))
            ##print int(run), int(ls), ProfX_h_ratiovsHF.GetBinCenter(int(ls)), ProfX_h_ratiovsHF.GetBinCenter(int(ls)), ProfX_h_ratiovsHF.GetBinContent(int(ls)), fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))), ((ProfX_h_ratiovsHF.GetBinContent(int(ls)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls)))
            ##print int(run), int(ls), ((ProfX_h_ratiovsHF.GetBinContent(int(ls)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls)))
            ##print ProfX_h_ratiovsHF.GetBinCenter(int(ls)), ((ProfX_h_ratiovsHF.GetBinContent(int(ls)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls)))  
            if fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls)))!=0 and ProfX_PCCvsHFOC.GetBinContent(int(ls))!=0:         
                ProfX_PCCvsHF_residual.SetPoint(ProfX_PCCvsHF_residual.GetN(), ProfX_PCCvsHFOC.GetBinCenter(int(ls)), ((ProfX_PCCvsHFOC.GetBinContent(int(ls)))-fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls))))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls))))
                ProfX_PCCvsHF_residual.SetPointError(ProfX_PCCvsHF_residual.GetN(), 0, ProfX_PCCvsHFOC.GetBinError(int(ls))/fitfn.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls))))
                ##print int(run), int(ls), ((ProfX_PCCvsHFOC.GetBinContent(int(ls)))-fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls))))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls)))    
                ##print ProfX_PCCvsHFOC.GetBinCenter(int(ls)), ((ProfX_PCCvsHFOC.GetBinContent(int(ls)))-fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls))))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls)))

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
h_ratio.SetStats(1)
h_ratio.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsls.png')
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
ProfX_h_ratiovsHF_residual.GetYaxis().SetTitle("(PCC-Fit)/Fit")
ProfX_h_ratiovsHF_residual.GetXaxis().SetTitle("HFOC")
ProfX_h_ratiovsHF_residual.SetTitle("Stability Systematics")
ProfX_h_ratiovsHF_residual.Draw("AP")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC_ProfileX_residuals.png')
ProfX_PCCvsHF_residual.SetMarkerStyle(20)
ProfX_PCCvsHF_residual.SetMarkerColor(46)
ProfX_PCCvsHF_residual.GetYaxis().SetRangeUser(-0.01, 0.01)
ProfX_PCCvsHF_residual.GetYaxis().SetTitle("(PCC-Fit)/Fit")
ProfX_PCCvsHF_residual.GetXaxis().SetTitle("HFOC")
ProfX_PCCvsHF_residual.SetTitle("Linearity systematics")
ProfX_PCCvsHF_residual.Draw("AP")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC_ProfileX_residuals.png')
ProjY_h_ratio.SetMarkerStyle(20)
ProjY_h_ratio.SetMarkerColor(46)
ProjY_h_ratio.GetXaxis().SetTitle("ProjectionY")
ProjY_h_ratio.GetYaxis().SetTitle("Entries")
ProjY_h_ratio.SetStats(1)
ProjY_h_ratio.Draw()
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC_ProjectionY.png')

## Selection for low lumi ls PCC_count>=3000000 and HFOC_count>=4000000
