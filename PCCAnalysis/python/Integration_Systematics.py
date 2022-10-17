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
ROOT.gStyle.SetOptStat(111111)
h_ratio = ROOT.TH2F("h_ratio", "PCC/RAMSES vs lumi section histogram ", 1080, 0.0, 54032, 1080, 0.94, 1.06)
#h_ratio = ROOT.TH2F("h_ratio", "PCC/HFOC vs lumi section histogram ", 30000, 0.0, 60000, 30000, 0.8, 1.2)
#h_ratio = ROOT.TH2F("h_ratio", "PCC/HFOC vs lumi section histogram ", 30000, 0.0, 60000, 30000, 0.8, 1.2)                  
#h_ratiovsHF = ROOT.TH2F("h_ratiovsHF", "PCC/HFOC vs HFOC Histogram", 30000, 0.0, 30000, 30000, 0, 2)
#h_ratio = ROOT.TH2F("h_ratio", "PCC/HFOC vs lumi section histogram ", 400, 0.0, 2000, 400, 0, 2)
#h_ratio=ROOT.TH2F("h_ratio", "PCC/HFOC vs lumi section histogram ", 30000, 0.0, 60000, 30000, 0, 2)
h_ratiovsHF=ROOT.TH2F("h_ratiovsHF", "PCC/HFOC vs HFOC Histogram", 500, 0.0, 25000, 500, 0.0, 2)
PCCvsHFOC=ROOT.TH2F("h_PCCvsHFOC", "PCC vs HFOC Histogram", 500, 0.0, 60000, 500, 0.0, 60000)
PCC_perls=ROOT.TGraph()
HFOC_perls=ROOT.TGraph()
ProfX_h_ratiovsHF_residual=ROOT.TGraphErrors()
ProfX_PCCvsHF_residual=ROOT.TGraphErrors()
PCC_HFOCvsHFOC=ROOT.TGraphErrors()
ProjY_ProfX_h_ratio=ROOT.TH1F("ProjY_ProfX","Y Projection of Profile", 1080, 0.99, 1.02)
lumisec_count=0
lumisec_all=0
lumisec_good=0
lumisec_good1=0
lumisec_good2=0

with open("/eos/user/a/asehrawa/PCC_HFOC_emittance_scan_root_files/PCC/EXPRESS_datasets/Run2018_ZB_test/Run2018A/ls.dat", "r") as datFile:
#with open("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC/EXPRESS_datasets/Run2018_ZB_test/Run2018A/ls.dat", "r") as datFile:
#with open("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_dt_plots/EXPRESS_datasets/Run2018_ZB_test/Run2018A/ls.dat", "r") as datFile:
#with open("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_oldveto/EXPRESS_datasets/Run2018A/ls.dat", "r") as datFile:
        #with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_ramses.json', "r") as HFOC_JSON:
         #       json_data = json.load(HFOC_JSON)
        with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json', "r") as HFOC_JSON:
                json_data = json.load(HFOC_JSON)
        #with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_pltzero.json', "r") as HFOC_JSON:
         #       json_data = json.load(HFOC_JSON)
        with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_pcc.json', "r") as PCC_JSON:
                json_data1 = json.load(PCC_JSON)
        with open('/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/test/emittance_scan_2018.json', "r") as EMITSCAN_JSON:
                json_data2 = json.load(EMITSCAN_JSON)
        for data in datFile:      
                run=data.split()[0]
                ls=data.split()[1]
                PCC_count=data.split()[2]
                HFOC_count=data.split()[3]
                lumisec_all=lumisec_all+1
                lumisec_count=lumisec_count+1
                goodls=False
                goodls1=False
                goodls2=False
                for line in json_data:
                        if not line[0].startswith("hfoc"):
                                continue
                        for run1 in line[1]:
                                if int(run1)==int(run):
                                        for ls1, ls2 in line[1][run1]:
                                                if int(ls1)<=int(ls)<=int(ls2):
                                                        goodls=True
                                                        lumisec_good=lumisec_good+1
                                                        #print int(run), int(run1), int(ls), "ls1 ", int(ls1), "ls2 ", int(ls2)
                for line1 in json_data1:
                        if not line1[0].startswith("pcc"):
                                continue
                        for run2 in line1[1]:
                                if int(run2)==int(run):
                                        #print ('lsdat', run, 'pcc', int(run2), 'hfoc', int(run1)) 
                                        #print line[1][run1], line1[1][run2]
                                        #if 317653<=int(run)<=317663:                                ##Fill 6774
                                        #if int(run2)==317663: 
                                        #if int(run1)==int(run)==int(run2):
                                        for ls3, ls4 in line1[1][run2]:
                                                if int(ls3)<=int(ls)<=int(ls4):
                                                        goodls1=True
                                                        lumisec_good1=lumisec_good1+1
                                                        #print int(run), int(run2), int(ls), "ls3 ", int(ls3), "ls4 ", int(ls4)
                for line2 in json_data2:
                        if not line2[0].startswith("emitscans"):
                                continue
                        for run3 in line2[1]:
                                if int(run3)==int(run):
                                        for ls5, ls6 in line2[1][run3]:
                                                if int(ls5)<=int(ls)<=int(ls6):
                                                        goodls2=True
                                                        lumisec_good2=lumisec_good2+1
                                                        #print int(run), int(run3), int(ls), "ls5 ", int(ls5), "ls6 ", int(ls6)      
                if float(HFOC_count) !=0 and float(PCC_count)>=6000 and float(HFOC_count)>=6000 and (float(PCC_count)/float(HFOC_count))!=0 and goodls==True and goodls2!=True and goodls1==True:
                        #if int(run)==317291 or int(run)==317292 or int(run)==317295 or int(run)==317296 or int(run)==317297:
                        h_ratiovsHF.Fill(float(HFOC_count), float(PCC_count)/float(HFOC_count))
                        h_ratio.Fill(lumisec_count+1, float(PCC_count)/float(HFOC_count))
                        PCCvsHFOC.Fill(float(PCC_count), float(HFOC_count)) 
                        PCC_perls.SetPoint(PCC_perls.GetN(), lumisec_count, float(PCC_count))
                        HFOC_perls.SetPoint(HFOC_perls.GetN(), lumisec_count, float(HFOC_count))
                        #lumisec_count=lumisec_count+1 
                        #print lumisec_count, float(PCC_count)*PCC_scaling_B, float(HFOC_count), (float(PCC_count)/float(HFOC_count))*PCC_scaling_B              
                        #print lumisec_count, int(ls), int(ls1), int(ls2), float(PCC_count)/float(HFOC_count)
                        #print int(run), int(run1), ls, ls1, ls2 
                        print run, lumisec_count, float(PCC_count)/float(HFOC_count)
print ("all ls ", lumisec_all, "HFOC good ls ", lumisec_good, "PCC good ls ", lumisec_good1, "PCC and HFOC good ls ", lumisec_count)
ProjY_h_ratio=h_ratio.ProjectionY("Y Projection of PCC/HFOC vs ls", 0, 1080)
ProjY_h_ratiovsHF=h_ratiovsHF.ProjectionY("Y Projection of PCC/HFOC vs HF", 0, 60000)
fitfn2 = ROOT.TF1("fitfn2","gaus", 0, 2);
fitfn2.SetParameters(0, 0, 1000000);
fitfn2.SetParameters(1, 0.6, 0.8);
fitfn2.SetParameters(2, 0.0009, 0.009);
ProjY_h_ratiovsHF.Fit("fitfn2")
ProfX_h_ratiovsHF=h_ratiovsHF.ProfileX()
fitfn = ROOT.TF1("fitfn","[0]*x+[1]",0,60000);
ProfX_h_ratiovsHF.Fit("fitfn")
ProfX_PCCvsHFOC=PCCvsHFOC.ProfileX()
fitfn1 = ROOT.TF1("fitfn1","[0]*x+[1]",0,60000);
ProfX_PCCvsHFOC.Fit("fitfn1")
ProfX_h_ratio=h_ratio.ProfileX()
num_bin=ProfX_h_ratio.GetNbinsX()
for i in range(num_bin):
        ProjY_ProfX_h_ratio.Fill(float(ProfX_h_ratio.GetBinContent(int(i))))
        print float(ProfX_h_ratio.GetBinContent(int(i)))

## run loop, lumi section loop
#with open("/eos/user/a/asehrawa/PCC_HFOC_emittance_scan_root_files/PCC/EXPRESS_datasets/Run2018_ZB_test/Run2018A/ls.dat", "r") as datFile:
 #   for data in datFile:
  #      run=data.split()[0]
   #     ls=data.split()[1]
    #    PCC_count=data.split()[2]
     #   HFOC_count=data.split()[3]
      #  PCC_HFOCvsHFOC.SetPoint(PCC_HFOCvsHFOC.GetN(), float(HFOC_count), float(ProfX_h_ratio.GetBinContent(int(ls))))
        #print float(HFOC_count), float(ProfX_h_ratio.GetBinContent(int(ls)))                                                                                                                            
       # ProjY_ProfX_h_ratio.Fill(float(ProfX_h_ratio.GetBinContent(int(ls))))
       # print float(ProfX_h_ratio.GetBinContent(int(ls)))
        #if int(run)==317663:
        #if fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls)))!=0 and ProfX_h_ratiovsHF.GetBinContent(int(ls))!=0:
          #ProfX_h_ratiovsHF_residual.SetPoint(ProfX_h_ratiovsHF_residual.GetN(), ProfX_h_ratiovsHF.GetBinCenter(int(ls)), ((ProfX_h_ratiovsHF.GetBinContent(int(ls)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))
            #ProfX_h_ratiovsHF_residual.SetPointError(ProfX_h_ratiovsHF_residual.GetN(), 0, ProfX_h_ratiovsHF.GetBinError(int(ls))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))
            #print int(run), int(ls), ProfX_h_ratiovsHF.GetBinCenter(int(ls)), ProfX_h_ratiovsHF.GetBinCenter(int(ls)), ProfX_h_ratiovsHF.GetBinContent(int(ls)), fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))), ((ProfX_h_ratiovsHF.GetBinContent(int(ls)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls)))
            #print int(run), int(ls), ((ProfX_h_ratiovsHF.GetBinContent(int(ls)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls)))
            #print ProfX_h_ratiovsHF.GetBinCenter(int(ls)), ((ProfX_h_ratiovsHF.GetBinContent(int(ls)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls)))

num_bin1=ProfX_PCCvsHFOC.GetNbinsX()
for j in range(num_bin1):
        if fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls)))!=0 and ProfX_PCCvsHFOC.GetBinContent(int(ls))!=0:         
                ProfX_PCCvsHF_residual.SetPoint(ProfX_PCCvsHF_residual.GetN(), ProfX_PCCvsHFOC.GetBinCenter(int(ls)), ((ProfX_PCCvsHFOC.GetBinContent(int(ls)))-fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls))))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls))))
                ProfX_PCCvsHF_residual.SetPointError(ProfX_PCCvsHF_residual.GetN(), 0, ProfX_PCCvsHFOC.GetBinError(int(ls))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls))))
                #print int(run), int(ls), ((ProfX_PCCvsHFOC.GetBinContent(int(ls)))-fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls))))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls)))    
                #print ProfX_PCCvsHFOC.GetBinCenter(int(ls)), ((ProfX_PCCvsHFOC.GetBinContent(int(ls)))-fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls))))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls)))
        #if float(HFOC_count) !=0 and float(PCC_count)>=6000 and float(HFOC_count)>=6000 and (float(PCC_count)/float(HFOC_count))!=0 and goodls==True and goodls1==True and goodls2!=True:
        #PCC_HFOCvsHFOC.SetPoint(PCC_HFOCvsHFOC.GetN(), float(HFOC_count), float(ProfX_h_ratio.GetBinContent(int(ls))))
        #print float(HFOC_count), float(ProfX_h_ratio.GetBinContent(int(ls)))
        #ProjY_ProfX_h_ratio.Fill(float(ProfX_h_ratio.GetBinContent(int(ls))))
        #print float(ProfX_h_ratio.GetBinContent(int(ls)))

ProjY_ProfX_h_ratio.SetMarkerStyle(20)
ProjY_ProfX_h_ratio.SetMarkerColor(46)
ProjY_ProfX_h_ratio.GetXaxis().SetTitle("PCC/HFOC")
ProjY_ProfX_h_ratio.GetYaxis().SetTitle("Entries")
#ProjY_ProfX_h_ratio.GetYaxis().SetTitle("PCC/PLT")                                                                                                                                       
ProjY_ProfX_h_ratio.SetStats(1)
ProjY_ProfX_h_ratio.SetFillColor(2)
ProjY_ProfX_h_ratio.GetYaxis().SetRangeUser(0, 200)
ProjY_ProfX_h_ratio.Draw("histp")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'ProjY_ProfileX_h_ratio.png')
PCC_HFOCvsHFOC.SetMarkerStyle(20)
PCC_HFOCvsHFOC.SetMarkerColor(46)
PCC_HFOCvsHFOC.GetYaxis().SetRangeUser(0.94, 1.06)
PCC_HFOCvsHFOC.GetXaxis().SetTitle("HFOC Inst. Lumi")
PCC_HFOCvsHFOC.GetYaxis().SetTitle("PCC/HFOC")
PCC_HFOCvsHFOC.Draw("AP")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC.png')
h_ratiovsHF.SetMarkerStyle(20)
h_ratiovsHF.SetMarkerColor(46)
h_ratiovsHF.GetXaxis().SetTitle("HF Inst. Lumi")
h_ratiovsHF.GetYaxis().SetTitle("PCC/HFOC")
h_ratiovsHF.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC.png')
h_ratio.SetMarkerStyle(20)
h_ratio.SetMarkerColor(46)
h_ratio.SetStats(1)
h_ratio.GetXaxis().SetTitle("Lumi section")
#h_ratio.GetXaxis().SetTitle("time (s)")
h_ratio.GetYaxis().SetTitle("PCC/HFOC")
#h_ratio.GetYaxis().SetTitle("PCC/PLT")
h_ratio.GetYaxis().SetRangeUser(0.94, 1.06)
#h_ratio.SetTitle("PCC/HFOC vs lumi section histogram")
h_ratio.SetTitle("PCC/RAMSES vs lumi section histogram")
#h_ratio.SetStats(1)
h_ratio.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsls.png')
PCCvsHFOC.SetMarkerStyle(20)
PCCvsHFOC.SetMarkerColor(46)
PCCvsHFOC.GetXaxis().SetTitle("HF Inst. Lumi")
PCCvsHFOC.GetYaxis().SetTitle("PCC")
PCCvsHFOC.Draw("colz")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC.png')
ProfX_h_ratio.SetMarkerStyle(20)
ProfX_h_ratio.SetMarkerColor(46)
ProfX_h_ratio.GetXaxis().SetTitle("lumi section")
ProfX_h_ratio.GetYaxis().SetTitle("PCC/HFOC")
#ProfX_h_ratio.GetYaxis().SetTitle("PCC/PLT")
ProfX_h_ratio.SetStats(0)
ProfX_h_ratio.GetYaxis().SetRangeUser(0.94, 1.06)
ProfX_h_ratio.Draw("p")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsls_ProfileX.png')
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
#ProfX_h_ratiovsHF_residual.SetMarkerStyle(20)
#ProfX_h_ratiovsHF_residual.SetMarkerColor(46)
#ProfX_h_ratiovsHF_residual.GetYaxis().SetTitle("(PCC-Fit)/Fit")
#ProfX_h_ratiovsHF_residual.GetXaxis().SetTitle("HFOC")
#ProfX_h_ratiovsHF_residual.SetTitle("Stability Systematics")
#ProfX_h_ratiovsHF_residual.Draw("AP")
#C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC_ProfileX_residuals.png')
ProfX_PCCvsHF_residual.SetMarkerStyle(20)
ProfX_PCCvsHF_residual.SetMarkerColor(46)
ProfX_PCCvsHF_residual.GetYaxis().SetRangeUser(-0.1, 0.1)
ProfX_PCCvsHF_residual.GetYaxis().SetTitle("(PCC-Fit)/Fit")
ProfX_PCCvsHF_residual.GetXaxis().SetTitle("HFOC")
ProfX_PCCvsHF_residual.SetTitle("Linearity systematics")
line1 = ROOT.TLine(0, -0.01, 35000, -0.01);
line1.SetLineColor(1);
line1.SetLineStyle(2);
line2 = ROOT.TLine(0, 0.01, 35000, 0.01);
line2.SetLineColor(1);
line2.SetLineStyle(2);
ProfX_PCCvsHF_residual.Draw("AP")
line1.Draw("SAME")
line2.Draw("SAME")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC_ProfileX_residuals.png')
ProjY_h_ratio.SetMarkerStyle(20)
ProjY_h_ratio.SetMarkerColor(46)
#ProjY_h_ratio.GetXaxis().SetRangeUser(1.4, 1.8)
#ProjY_h_ratio.GetYaxis().SetRangeUser(0, 300)
ProjY_h_ratio.GetXaxis().SetTitle("ProjectionY")
ProjY_h_ratio.GetYaxis().SetTitle("Entries")
#ProjY_h_ratio.GetXaxis().SetRangeUser(0.95, 1.05)
ProjY_h_ratio.SetTitle("Stability Systematics") 
ProjY_h_ratio.SetStats(1)
ProjY_h_ratio.Draw("histp")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC_ProjectionY.png')
ProjY_h_ratiovsHF.SetMarkerStyle(20)
ProjY_h_ratiovsHF.SetMarkerColor(46)
#ProjY_h_ratiovsHF.GetXaxis().SetRangeUser(1.4, 1.8)
#ProjY_h_ratiovsHF.GetYaxis().SetRangeUser(0, 300)
ProjY_h_ratiovsHF.GetXaxis().SetTitle("ProjectionY")
ProjY_h_ratiovsHF.GetYaxis().SetTitle("Entries")
ProjY_h_ratiovsHF.SetTitle("Stability Systematics")
ProjY_h_ratiovsHF.SetStats(1)
ProjY_h_ratiovsHF.Draw("histp")
fitfn2.SetLineColor(1)
fitfn2.SetMarkerColor(1)
#fitfn2.Draw("SAME")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOC_vsHFOC_ProjectionY.png')
C2=ROOT.TCanvas("C2","",2000,1000)
C2.SetLogy()
PCC_perls.SetMarkerStyle(20)
PCC_perls.SetMarkerColor(1)
PCC_perls.SetMarkerSize(0.5)
PCC_perls.GetXaxis().SetTitle("ls") 
PCC_perls.GetYaxis().SetTitle("PCC")
#PCC_perls.GetYaxis().SetRangeUser(0.0001, 100000000)
PCC_perls.GetYaxis().SetRangeUser(100, 100000)
PCC_perls.Draw("AP")
C2.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_perls.png')
HFOC_perls.SetMarkerStyle(20)
HFOC_perls.SetMarkerColor(2)
HFOC_perls.SetMarkerSize(0.5)
HFOC_perls.GetXaxis().SetTitle("ls")
HFOC_perls.GetYaxis().SetTitle("HFOC")
#HFOC_perls.GetYaxis().SetRangeUser(0.0001, 100000000)
HFOC_perls.GetYaxis().SetRangeUser(100, 100000)
HFOC_perls.Draw("AP")
C2.Print('/eos/user/a/asehrawa/BRIL-new/'+'HFOC_perls.png')
PCC_perls.SetMarkerStyle(20)
HFOC_perls.SetMarkerStyle(20)
PCC_perls.SetMarkerColor(1)
PCC_perls.SetLineColor(1)
PCC_perls.SetMarkerSize(0.5)
HFOC_perls.SetMarkerColor(2)
HFOC_perls.SetLineColor(2)
HFOC_perls.SetMarkerSize(0.5)
PCC_perls.GetXaxis().SetTitle("ls")
PCC_perls.GetYaxis().SetTitle("PCC or HFOC")
PCC_perls.SetTitle("Run 2018A")
#PCC_perls.SetTitle("Run 317626")
#PCC_perls.GetYaxis().SetRangeUser(0.0001, 100000000)
#HFOC_perls.GetYaxis().SetRangeUser(0.0001, 100000000)
PCC_perls.GetYaxis().SetRangeUser(100, 100000)                                                                                       
HFOC_perls.GetYaxis().SetRangeUser(100, 100000)
#PCC_perls.GetXaxis().SetRangeUser(0, 2200)
#HFOC_perls.GetXaxis().SetRangeUser(0, 2200)
#PCC_perls.GetYaxis().SetRangeUser(0, 30000)
#HFOC_perls.GetYaxis().SetRangeUser(0, 30000)
PCC_perls.Draw("AP")
HFOC_perls.Draw("PSAME")
legend = ROOT.TLegend(0.75, 0.75, 0.88, 0.88)
legend.AddEntry(PCC_perls, "PCC", "l")
legend.AddEntry(HFOC_perls, "HFOC", "l")
legend.SetFillColor(0)
legend.SetLineColor(0)
legend.SetFillColor(0)
legend.Draw("SAME")
C2.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOC_perls.png')










