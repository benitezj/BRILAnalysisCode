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

C1=ROOT.TCanvas("C1","",1600,1200)

ROOT.gROOT.Macro("/afs/cern.ch/user/a/asehrawa/reprocessing_PCC_ZB_data_27May2023/CMSSW_10_6_30/src/BRILAnalysisCode/PCCAnalysis/test/rootlogon.C")

def addCMSLabel(C1, cms_text="CMS Preliminary", extra_text="2018", lumi_text="#sqrt{s} = 13 TeV"):
    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextFont(61)  # helvetica bold
    latex.SetTextSize(0.03)  # adjust text size here
    latex.DrawLatex(0.13, 0.93, cms_text + " " + extra_text)  # adjust text position here (X, Y)
    
    latex.SetTextFont(52)  # helvetica italic
    latex.SetTextSize(0.04)  # adjust text size here
    latex.DrawLatex(0.74, 0.93, lumi_text)  # adjust text position here (X, Y)
    C1.Update()

#ROOT.gApplication.Run()
# Usage:
#canvas = ROOT.TCanvas("canvas", "canvas", 800, 600)
#setCMSStyle()
#addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")

#ROOT.gROOT.ProcessLine('.L /afs/cern.ch/user/a/asehrawa/reprocessing_PCC_RD_data_27May2023/CMSSW_10_2_2/src/BRILAnalysisCode/PCCAnalysis/plots/plotting_style.C')
#ROOT.setTDRStyle()

#C1=ROOT.TCanvas("C1","",800,600)
ROOT.gStyle.SetOptStat("nemri")
h_ratio=ROOT.TH2F("h_ratio", "PCC/HFOC vs lumi section histogram ", 6000, 0.0, 300000, 6000, 0.96, 1.04)
h_ratiovsHF=ROOT.TH2F("h_ratiovsHF", "PCC/HFOC vs HFOC Histogram", 600, 0.0, 30000, 600, 0.96, 1.04)
PCCvsHFOC=ROOT.TH2F("h_PCCvsHFOC", "PCC vs HFOC Histogram", 600, 0.0, 30000, 600, 0.0, 30000)
PCC_perls=ROOT.TGraph()
HFOC_perls=ROOT.TGraph()
ProfX_h_ratiovsHF_residual=ROOT.TGraphErrors()
ProfX_PCCvsHF_residual=ROOT.TGraphErrors()
ProfX_PCC_HFvsHF_residual=ROOT.TGraphErrors()
PCC_HFOCvsHFOC=ROOT.TGraphErrors()
ProjY_ProfX_h_ratio=ROOT.TH1F("ProjY_ProfX","Y Projection of Profile", 200, 0.96, 1.04)
ProjY_ProfX_h_ratiovsHF=ROOT.TH1F("ProjY_ProfXvsHF","Y Projection of ProfilevsHF", 200, 0.96, 1.04)
ProfX_h_residual=ROOT.TH1F("Profile_histogram_PCCvsHF","ProfileX_PCCvsHFOC", 200, -0.01, 0.01)
ProfX_h_HF_residual=ROOT.TH1F("Profile_histogram_PCC_HFvsHF","ProfileX_PCC_HFvsHFOC", 200, 0.96, 1.04)
hist_HFOC_perls=ROOT.TH1F("hL"," histogram of HFOC Lumi", 6000, 0, 300000)
hist_h_ratio = ROOT.TGraph()
lumisec_count=0
lumisec_all=0
lumisec_good=0
lumisec_good1=0
lumisec_good2=0

#with open("/eos/user/a/asehrawa/PCC_HFOC_emittance_scan_root_files/PCC/EXPRESS_datasets/Run2018_ZB_test/Run2018/ls.dat", "r") as datFile:
#with open("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_veto_1701/EXPRESS_datasets/Run2018C/ls.dat", "r") as datFile:
with open("/afs/cern.ch/user/a/asehrawa/reprocessing_PCC_ZB_data_27May2023/CMSSW_10_6_30/src/PCC_newafterglowparameters_27May2023/Run2018_27May2023/ls.dat", "r") as datFile:
#with open("/afs/cern.ch/user/a/asehrawa/reprocessing_PCC_ZB_data_27May2023/CMSSW_10_6_30/src/PCC_newafterglowparameters_27May2023/Run2018A_27May2023/ls_test.dat", "r") as datFile:
#with open("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_veto_1615/EXPRESS_datasets/Run2018/ls.dat", "r") as datFile:
#with open("/eos/user/a/asehrawa/PCC_PLT_plots/EXPRESS_datasets/Run2018_ZB_test/Run2018/ls.dat", "r") as datFile:
#with open("/afs/cern.ch/user/a/asehrawa/Reprocessed_PCC_2018_data/CMSSW_10_2_2/src/PCC_oldveto/EXPRESS_datasets/Run2018/ls.dat", "r") as datFile:
#with open("/eos/user/a/asehrawa/PCC_BCM1F_plots/EXPRESS_datasets/Run2018_ZB_test/Run2018/ls.dat", "r") as datFile:
#with open("/eos/user/a/asehrawa/PCC_PLT_plots/EXPRESS_datasets/Run2018_ZB_test/Run2018/ls.dat", "r") as datFile:
        with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json', "r") as HFOC_JSON:
                json_data = json.load(HFOC_JSON)
        #with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_bcm1f.json', "r") as HFOC_JSON:
         #       json_data = json.load(HFOC_JSON)
        #with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_pltzero.json', "r") as HFOC_JSON:
         #       json_data = json.load(HFOC_JSON)
        with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_pcc.json', "r") as PCC_JSON:
                json_data1 = json.load(PCC_JSON)
        with open('/afs/cern.ch/user/a/asehrawa/reprocessing_PCC_ZB_data_27May2023/CMSSW_10_6_30/src/BRILAnalysisCode/PCCAnalysis/test/emittance_scan_2018.json', "r") as EMITSCAN_JSON:
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
                                                        ##print int(run), int(run1), int(ls), "ls1 ", int(ls1), "ls2 ", int(ls2)
                for line1 in json_data1:
                        if not line1[0].startswith("pcc"):
                                continue
                        for run2 in line1[1]:
                                if int(run2)==int(run):
                                        ##print ('lsdat', run, 'pcc', int(run2), 'hfoc', int(run1)) 
                                        ##print line[1][run1], line1[1][run2]
                                        ##if 317653<=int(run)<=317663:                                ##Fill 6774
                                        ##if int(run2)==317663: 
                                        ##if int(run1)==int(run)==int(run2):
                                        for ls3, ls4 in line1[1][run2]:
                                                if int(ls3)<=int(ls)<=int(ls4):
                                                        goodls1=True                                                        
                                                        lumisec_good1=lumisec_good1+1
                                                        ##print int(run), int(run2), int(ls), "ls3 ", int(ls3), "ls4 ", int(ls4)
                for line2 in json_data2:
                        if not line2[0].startswith("emitscans"):
                                continue
                        for run3 in line2[1]:
                                if int(run3)==int(run):
                                        for ls5, ls6 in line2[1][run3]:
                                                if int(ls5)<=int(ls)<=int(ls6):
                                                        goodls2=True
                                                        lumisec_good2=lumisec_good2+1
                                                        ##print int(run), int(run3), int(ls), "ls5 ", int(ls5), "ls6 ", int(ls6)   
                if float(HFOC_count) !=0 and goodls==True and goodls1==True and goodls2!=True and float(PCC_count)>=6000 and float(HFOC_count)>=6000:
                        h_ratiovsHF.Fill(float(HFOC_count), float(PCC_count)/float(HFOC_count))
                        h_ratio.Fill(lumisec_count+1, float(PCC_count)/float(HFOC_count))  # make TH1 object and fill it with hfoc lumi having same bins as TH2 object, use it to fill 
                        PCCvsHFOC.Fill(float(HFOC_count), float(PCC_count)) 
                        PCC_perls.SetPoint(PCC_perls.GetN(), lumisec_count, float(PCC_count))
                        HFOC_perls.SetPoint(HFOC_perls.GetN(), lumisec_count, float(HFOC_count))
                        #hist_HFOC_perls.Fill(lumisec_count+1, float(HFOC_count))
                        hist_HFOC_perls.Fill(float(HFOC_count))
                        ##lumisec_count=lumisec_count+1 
                        ##print lumisec_count, float(PCC_count)*PCC_scaling_B, float(HFOC_count), (float(PCC_count)/float(HFOC_count))*PCC_scaling_B              
                        ##print lumisec_count, int(ls), int(ls1), int(ls2), float(PCC_count)/float(HFOC_count)
                        ##print int(run), int(run1), ls, ls1, ls2 
                        print run, lumisec_count, float(HFOC_count), float(PCC_count)/float(HFOC_count)
                        ##print run
print ("all ls ", lumisec_all, "HFOC good ls ", lumisec_good, "PCC good ls ", lumisec_good1, "PCC and HFOC good ls ", lumisec_count)
ProjY_h_ratio=h_ratio.ProjectionY("Y Projection of PCC/HFOC vs ls", 0, 6000)
ProjY_h_ratiovsHF=h_ratiovsHF.ProjectionY("Y Projection of PCC/HFOC vs HF", 0, 30000)
fitfn2 = ROOT.TF1("fitfn2","gaus", 0, 2)
fitfn2.SetParameters(0, 0, 1000000)
fitfn2.SetParameters(1, 0.6, 0.8)
fitfn2.SetParameters(2, 0.0009, 0.009)
ProjY_h_ratiovsHF.Fit("fitfn2")
ProfX_h_ratiovsHF=h_ratiovsHF.ProfileX()
fitfn = ROOT.TF1("fitfn","[0]*x+[1]",0,30000)
ProfX_h_ratiovsHF.Fit("fitfn")
ProfX_PCCvsHFOC=PCCvsHFOC.ProfileX()
fitfn1 = ROOT.TF1("fitfn1","[0]*x+[1]",0,30000)
ProfX_PCCvsHFOC.Fit("fitfn1")
ProfX_h_ratio=h_ratio.ProfileX()
num_bin=ProfX_h_ratio.GetNbinsX()
for i in range(num_bin):
        ProjY_ProfX_h_ratio.Fill(float(ProfX_h_ratio.GetBinContent(int(i))))
        #ProjY_ProfX_h_ratio.Fill(float(ProfX_h_ratio.GetBinContent(int(i))), hist_HFOC_perls.GetBinContent(int(i)))
        #print float(ProfX_h_ratio.GetBinContent(int(i)))
        hist_h_ratio.SetPoint(hist_h_ratio.GetN(), ProfX_h_ratio.GetBinContent(int(i)), hist_HFOC_perls.GetBinContent(int(i)))
        print float(ProfX_h_ratio.GetBinContent(int(i))), hist_HFOC_perls.GetBinContent(int(i))

num_bin2=ProfX_h_ratiovsHF.GetNbinsX()
for k in range(num_bin2):
        ProjY_ProfX_h_ratiovsHF.Fill(float(ProfX_h_ratiovsHF.GetBinContent(int(k))))

## run loop, lumi section loop
#with open("/eos/user/a/asehrawa/PCC_HFOC_emittance_scan_root_files/PCC/EXPRESS_datasets/Run2018_ZB_test/Run2018/ls.dat", "r") as datFile:
 #   for data in datFile:
  #      run=data.split()[0]
   #     ls=data.split()[1]
    #    PCC_count=data.split()[2]
     #   HFOC_count=data.split()[3]
      #  PCC_HFOCvsHFOC.SetPoint(PCC_HFOCvsHFOC.GetN(), float(HFOC_count), float(ProfX_h_ratio.GetBinContent(int(ls))))
        #print float(HFOC_count), float(ProfX_h_ratio.GetBinContent(int(ls)))                                                                                                                              
        #ProjY_ProfX_h_ratio.Fill(float(ProfX_h_ratio.GetBinContent(int(ls))))
        #print float(ProfX_h_ratio.GetBinContent(int(ls)))
        ##if int(run)==317663:
        ##if fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls)))!=0 and ProfX_h_ratiovsHF.GetBinContent(int(ls))!=0:
          ##  ProfX_h_ratiovsHF_residual.SetPoint(ProfX_h_ratiovsHF_residual.GetN(), ProfX_h_ratiovsHF.GetBinCenter(int(ls)), ((ProfX_h_ratiovsHF.GetBinContent(int(ls)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))
            ##ProfX_h_ratiovsHF_residual.SetPointError(ProfX_h_ratiovsHF_residual.GetN(), 0, ProfX_h_ratiovsHF.GetBinError(int(ls))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))
            ##print int(run), int(ls), ProfX_h_ratiovsHF.GetBinCenter(int(ls)), ProfX_h_ratiovsHF.GetBinCenter(int(ls)), ProfX_h_ratiovsHF.GetBinContent(int(ls)), fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))), ((ProfX_h_ratiovsHF.GetBinContent(int(ls)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls)))
            ##print int(run), int(ls), ((ProfX_h_ratiovsHF.GetBinContent(int(ls)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls)))
            ##print ProfX_h_ratiovsHF.GetBinCenter(int(ls)), ((ProfX_h_ratiovsHF.GetBinContent(int(ls)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(ls))) 
num_bin1=ProfX_PCCvsHFOC.GetNbinsX()
for j in range(num_bin1): 
        if fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(j)))!=0 and ProfX_PCCvsHFOC.GetBinContent(int(j))!=0:         
                ProfX_PCCvsHF_residual.SetPoint(ProfX_PCCvsHF_residual.GetN(), ProfX_PCCvsHFOC.GetBinCenter(int(j)), ((ProfX_PCCvsHFOC.GetBinContent(int(j)))-fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(j))))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(j))))
                ProfX_PCCvsHF_residual.SetPointError(ProfX_PCCvsHF_residual.GetN(), 0, ProfX_PCCvsHFOC.GetBinError(int(j))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(j))))
                ProfX_h_residual.Fill(((ProfX_PCCvsHFOC.GetBinContent(int(j)))-fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(j))))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(j))))
                ##print int(run), int(ls), ((ProfX_PCCvsHFOC.GetBinContent(int(ls)))-fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls))))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls)))    
                ##print ProfX_PCCvsHFOC.GetBinCenter(int(ls)), ((ProfX_PCCvsHFOC.GetBinContent(int(ls)))-fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls))))/fitfn1.Eval(ProfX_PCCvsHFOC.GetBinCenter(int(ls)))
        #PCC_HFOCvsHFOC.SetPoint(PCC_HFOCvsPCC.GetN(), float(HFOC_count), float(ProfX_h_ratio.GetBinContent(int(ls))))
        #print float(HFOC_count), float(ProfX_h_ratio.GetBinContent(int(ls)))

num_bin2=ProfX_h_ratiovsHF.GetNbinsX()
for k in range(num_bin2):
        if fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(k)))!=0 and ProfX_h_ratiovsHF.GetBinContent(int(k))!=0:
                ProfX_PCC_HFvsHF_residual.SetPoint(ProfX_PCC_HFvsHF_residual.GetN(), ProfX_h_ratiovsHF.GetBinCenter(int(k)), ((ProfX_h_ratiovsHF.GetBinContent(int(k)))-fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(k))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(k))))
                ProfX_PCC_HFvsHF_residual.SetPointError(ProfX_PCC_HFvsHF_residual.GetN(), 0, ProfX_h_ratiovsHF.GetBinError(int(k))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(k))))
                ProfX_h_HF_residual.Fill(((ProfX_PCCvsHFOC.GetBinContent(int(k)))-fitfn1.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(k))))/fitfn.Eval(ProfX_h_ratiovsHF.GetBinCenter(int(k))))


#ROOT.setTDRStyle()
#ROOT.gStyle.SetOptStat(0)
hist_h_ratio.SetMarkerStyle(20)
hist_h_ratio.SetMarkerColor(46)
hist_h_ratio.SetMarkerStyle(8)
hist_h_ratio.SetMarkerSize(1)
hist_h_ratio.SetFillColor(4)
#hist_h_ratio.GetXaxis().SetRangeUser(0.94, 1.06)
#hist_h_ratio.SetStats(1)
hist_h_ratio.GetXaxis().SetTitle("PCC/HFOC ratio per 50 LS")
#hist_h_ratio.GetXaxis().SetTitle("time (s)")                                                                                                                                                             
hist_h_ratio.GetYaxis().SetTitle("Luminosity")
#hist_h_ratio.GetYaxis().SetTitle("PCC/PLT")                                                                                                                                                             
#hist_h_ratio.GetYaxis().SetRangeUser(0, 400)                                                                                                                                                             
#hist_h_ratio.SetTitle("PCC/HFOC vs lumi section histogram")                                                                                                                                              
hist_h_ratio.SetTitle("ProfileX vs HFOC lumi")
hist_h_ratio.Draw("AB")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'ProfileXvsHFOClumi_all.png')

ProfX_h_residual.SetMarkerStyle(20)
ProfX_h_residual.SetMarkerColor(46)
ProfX_h_residual.GetYaxis().SetTitle("Entries")
ProfX_h_residual.GetXaxis().SetTitle("(PCC-Fit)/Fit")
ProfX_h_residual.GetXaxis().SetRangeUser(-0.01, 0.01)
ProfX_h_residual.SetStats(1)
ProfX_h_residual.SetFillColor(2)
ProfX_h_residual.Draw("hist")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'h_ProfileX_PCCvsHFOC_all.png')

ProfX_h_HF_residual.SetMarkerStyle(20)
ProfX_h_HF_residual.SetMarkerColor(46)
ProfX_h_HF_residual.GetYaxis().SetTitle("Entries")
ProfX_h_HF_residual.GetXaxis().SetTitle("(PCC-Fit)/Fit")
#ProfX_h_HF_residual.GetXaxis().SetRangeUser(-0.01, 0.01)
ProfX_h_HF_residual.SetStats(1)
ProfX_h_HF_residual.SetFillColor(2)
ProfX_h_HF_residual.Draw("hist")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'h_ProfileX_PCC_HFOCvsHFOC_all.png')

ProjY_ProfX_h_ratio.SetMarkerStyle(20)
ProjY_ProfX_h_ratio.SetMarkerColor(46)
ProjY_ProfX_h_ratio.GetXaxis().SetTitle("PCC/HFOC")
ProjY_ProfX_h_ratio.GetYaxis().SetTitle("Entries")
#ProjY_ProfX_h_ratio.GetYaxis().SetTitle("PCC/PLT")                                                                                                                                                       
ProjY_ProfX_h_ratio.SetStats(1)
ProjY_ProfX_h_ratio.SetFillColor(2)
#ProjY_ProfX_h_ratio.GetYaxis().SetRangeUser(0, 300)
ProjY_ProfX_h_ratio.Draw("hist")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'ProjY_ProfileX_h_ratio_all.png')

ProjY_ProfX_h_ratiovsHF.SetMarkerStyle(20)
ProjY_ProfX_h_ratiovsHF.SetMarkerColor(46)
ProjY_ProfX_h_ratiovsHF.GetXaxis().SetTitle("PCC/HFOC")
ProjY_ProfX_h_ratiovsHF.GetYaxis().SetTitle("Entries")
#ProjY_ProfX_h_ratiovsHF.GetYaxis().SetTitle("PCC/PLT")                                                                                                                                                                    
ProjY_ProfX_h_ratiovsHF.SetStats(1)
ProjY_ProfX_h_ratiovsHF.SetFillColor(2)
#ProjY_ProfX_h_ratio.GetYaxis().SetRangeUser(0, 300)                                                                                                                                                                      
ProjY_ProfX_h_ratiovsHF.Draw("hist")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'ProjY_ProfileX_h_ratiovsHF_all.png')

PCC_HFOCvsHFOC.SetMarkerStyle(20)
PCC_HFOCvsHFOC.SetMarkerColor(46)
#PCC_HFOCvsHFOC.GetYaxis().SetRangeUser(0.94, 1.06)
PCC_HFOCvsHFOC.GetXaxis().SetTitle("PCC Inst. Lumi")
PCC_HFOCvsHFOC.GetYaxis().SetTitle("PCC/HFOC")
PCC_HFOCvsHFOC.Draw("AP")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC_all.png')
h_ratiovsHF.SetMarkerStyle(20)
h_ratiovsHF.SetMarkerColor(46)
h_ratiovsHF.GetXaxis().SetTitle("HF Inst. Lumi")
h_ratiovsHF.GetYaxis().SetTitle("PCC/HFOC")
h_ratiovsHF.Draw("colz")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC_all.png')

h_ratio.SetMarkerStyle(20)
h_ratio.SetMarkerColor(46)
#h_ratio.GetYaxis().SetRangeUser(0.95, 1.05)
h_ratio.GetXaxis().SetTitle("Lumi section")
#h_ratio.GetXaxis().SetTitle("time (s)")
h_ratio.GetYaxis().SetTitle("PCC/HFOC")
h_ratio.SetTitle("PCC/HFOC vs lumi section histogram")
#h_ratio.SetStats(1)
h_ratio.Draw("colz")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsls_all.png')

PCCvsHFOC.SetMarkerStyle(20)
PCCvsHFOC.SetMarkerColor(46)
PCCvsHFOC.GetXaxis().SetTitle("HF Inst. Lumi")
PCCvsHFOC.GetYaxis().SetTitle("PCC")
PCCvsHFOC.Draw("colz")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC_all.png')

ProfX_h_ratio.SetMarkerStyle(20)
ProfX_h_ratio.SetMarkerColor(46)
ProfX_h_ratio.GetXaxis().SetTitle("lumi section")
ProfX_h_ratio.GetYaxis().SetTitle("PCC/HFOC")
ProfX_h_ratio.GetYaxis().SetRangeUser(0.96, 1.04)
ProfX_h_ratio.SetStats(0)
ProfX_h_ratio.Draw("p")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsls_ProfileX_all.png')

ProfX_h_ratiovsHF.SetMarkerStyle(20)
ProfX_h_ratiovsHF.SetMarkerColor(46)
ProfX_h_ratiovsHF.GetYaxis().SetRangeUser(0.96, 1.04)
ProfX_h_ratiovsHF.GetXaxis().SetTitle("HF Inst. Lumi")
ProfX_h_ratiovsHF.GetYaxis().SetTitle("PCC/HFOC")
ProfX_h_ratiovsHF.Draw("colz")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC_ProfileX_all.png')

ProfX_PCCvsHFOC.SetMarkerStyle(20)
ProfX_PCCvsHFOC.SetMarkerColor(46)
ProfX_PCCvsHFOC.GetXaxis().SetRangeUser(0, 30000)
ProfX_PCCvsHFOC.GetYaxis().SetRangeUser(0, 30000)
ProfX_PCCvsHFOC.GetXaxis().SetTitle("HF Inst. Lumi")
ProfX_PCCvsHFOC.GetYaxis().SetTitle("PCC")
ProfX_PCCvsHFOC.Draw("colz")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC_ProfileX_all.png')

#ProfX_h_ratiovsHF_residual.SetMarkerStyle(20)
#ProfX_h_ratiovsHF_residual.SetMarkerColor(46)
#ProfX_h_ratiovsHF_residual.GetYaxis().SetTitle("(PCC-Fit)/Fit")
#ProfX_h_ratiovsHF_residual.GetXaxis().SetTitle("HFOC")
#ProfX_h_ratiovsHF_residual.SetTitle("Stability Systematics")
#ProfX_h_ratiovsHF_residual.Draw("AP")
#C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC_ProfileX_residuals.png')
ProfX_PCCvsHF_residual.SetMarkerStyle(20)
ProfX_PCCvsHF_residual.SetMarkerColor(46)
ProfX_PCCvsHF_residual.GetYaxis().SetRangeUser(-0.01, 0.01)
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
#line1.Draw("SAME")
#line2.Draw("SAME")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC_ProfileX_residuals_all.png')

ProfX_PCC_HFvsHF_residual.SetMarkerStyle(20)
ProfX_PCC_HFvsHF_residual.SetMarkerColor(46)
#ProfX_PCC_HFvsHF_residual.GetYaxis().SetRangeUser(-0.01, 0.01)
ProfX_PCC_HFvsHF_residual.GetYaxis().SetTitle("(PCC-Fit)/Fit")
ProfX_PCC_HFvsHF_residual.GetXaxis().SetTitle("HFOC")
ProfX_PCC_HFvsHF_residual.SetTitle("Linearity systematics")
line1 = ROOT.TLine(0, -0.01, 35000, -0.01);
line1.SetLineColor(1);
line1.SetLineStyle(2);
line2 = ROOT.TLine(0, 0.01, 35000, 0.01);
line2.SetLineColor(1);
line2.SetLineStyle(2);
ProfX_PCC_HFvsHF_residual.Draw("AP")
#line1.Draw("SAME")                                                                                                                                                                                                    #line2.Draw("SAME")                                                                
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")                                                                                                                                             
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOCvsHFOC_ProfileX_residuals_all.png')
ProjY_h_ratio.SetMarkerStyle(20)
ProjY_h_ratio.SetMarkerColor(46)
#ProjY_h_ratio.GetXaxis().SetRangeUser(1.4, 1.8)
#ProjY_h_ratio.GetYaxis().SetRangeUser(0, 300)
ProjY_h_ratio.GetXaxis().SetTitle("ProjectionY")
ProjY_h_ratio.GetYaxis().SetTitle("Entries")
ProjY_h_ratio.SetTitle("Stability Systematics") 
#ProjY_h_ratio.SetStats(1)
#ProjY_h_ratio.GetXaxis().SetRangeUser(0.95, 1.05)
ProjY_h_ratio.Draw("histp")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCCvsHFOC_ProjectionY_all.png')

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
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOC_vsHFOC_ProjectionY_all.png')

#C2=ROOT.TCanvas("C2","",800,600)
#C2.SetLogy()
PCC_perls.SetMarkerStyle(20)
PCC_perls.SetMarkerColor(1)
PCC_perls.SetMarkerSize(0.5)
PCC_perls.GetXaxis().SetTitle("ls")
PCC_perls.GetYaxis().SetTitle("PCC")
#PCC_perls.GetYaxis().SetRangeUser(0.0001, 100000000)
PCC_perls.Draw("AP")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_perls_all.png')

HFOC_perls.SetMarkerStyle(20)
HFOC_perls.SetMarkerColor(2)
HFOC_perls.SetMarkerSize(0.5)
HFOC_perls.GetXaxis().SetTitle("ls")
HFOC_perls.GetYaxis().SetTitle("HFOC")
#HFOC_perls.GetYaxis().SetRangeUser(0.0001, 100000000)
HFOC_perls.Draw("AP")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'HFOC_perls_all.png')

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
#PCC_perls.GetYaxis().SetRangeUser(0.0001, 100000000)
#HFOC_perls.GetYaxis().SetRangeUser(0.0001, 100000000)
PCC_perls.Draw("AP")
HFOC_perls.Draw("PSAME")
legend = ROOT.TLegend(0.75, 0.75, 0.88, 0.88)
legend.AddEntry(PCC_perls, "PCC", "l")
legend.AddEntry(HFOC_perls, "HFOC", "l")
legend.SetFillColor(0)
legend.SetLineColor(0)
legend.SetFillColor(0)
legend.Draw("SAME")
addCMSLabel(C1, "CMS Preliminary", "2018", "#sqrt{s} = 13 TeV")
C1.Print('/eos/user/a/asehrawa/BRIL-new/'+'PCC_HFOC_perls_all.png')


