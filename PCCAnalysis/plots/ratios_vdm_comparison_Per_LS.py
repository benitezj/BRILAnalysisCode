# -*- coding: utf-8 -*-
import ROOT
from array import array
import pandas as pd
import numpy as np
det_dir = 'ratios'
ROOT.gROOT.SetBatch(ROOT.kTRUE)


myStyle = ROOT.TStyle("myStyle", "My Style")

myStyle.SetPadColor(0)
myStyle.SetCanvasColor(0)
myStyle.SetPadBorderSize(10)
myStyle.SetPadBorderMode(0)
myStyle.SetPadBottomMargin(0.15)
myStyle.SetPadTopMargin(0.08)
myStyle.SetPadLeftMargin(0.15)
myStyle.SetPadRightMargin(0.1)  
myStyle.SetPadGridX(0)
myStyle.SetPadGridY(0)
myStyle.SetPadTickX(1)
myStyle.SetPadTickY(1)


ROOT.gROOT.SetStyle("myStyle")


ROOT.gROOT.ForceStyle()

archivo_csv = 'merged_df_all_luminometers_bx_new_veto.csv'
df = pd.read_csv(archivo_csv)
            
MINLUMI = 0
MAXLUMI = 1
MINLUMICUT = -5
MINTIME = 0
MAXTIME = 25

ran_inf = .9                                                                                                               
ran_sup = 1.1

NDIV  = 1    ## change here how many lumisection want to  merge
NLS = 3781
NTIME = 3781/NDIV#196
ratiorange = 1
            
GLumi = ROOT.TGraph()
GLumiHF = ROOT.TGraph()
GLumiRatio = ROOT.TGraph()

HPCC = ROOT.TH1F("HPCC", "", NLS, MINTIME, MAXTIME)
HHFET = ROOT.TH1F("HHFET", "", NLS, MINTIME, MAXTIME)
HHFOC = ROOT.TH1F("HHFOC", "", NLS, MINTIME, MAXTIME)
HPLT = ROOT.TH1F("HPLT", "", NLS, MINTIME, MAXTIME)
HBCM1f = ROOT.TH1F("HBCM1f", "", NLS, MINTIME, MAXTIME)
HBCM1fUTCA = ROOT.TH1F("HBCM1fUTCA", "", NLS, MINTIME, MAXTIME)
#test = ROOT.TH1F("test", "", NLS, MINTIME, MAXTIME)

Ratio_HFET = ROOT.TH1F("Ratio_HFET", "", NTIME, MINTIME, MAXTIME)
Ratio_HFOC = ROOT.TH1F("Ratio_HFOC", "", NTIME, MINTIME, MAXTIME)
Ratio_PLT = ROOT.TH1F("Ratio_PLT", "", NTIME, MINTIME, MAXTIME) 
Ratio_BCM1f = ROOT.TH1F("Ratio_BCM1f", "", NTIME, MINTIME, MAXTIME)
Ratio_BCM1fUTCA = ROOT.TH1F("Ratio_BCM1fUTCA", "", NTIME, MINTIME, MAXTIME)


for index, row in df.iterrows():
    timestamp = row['timestamp']
    t = row['time']
    pcctotal = row['PCCTOTAL']
    bxdelivered_sum_bcm1f = row['bxdelivered_sum_bcm1f']
    bxdelivered_sum_bcm1futca = row['bxdelivered_sum_bcm1futca']
    bxdelivered_sum_hfet = row['bxdelivered_sum_hfet']
    bxdelivered_sum_hfoc = row['bxdelivered_sum_hfoc']
    bxdelivered_sum_plt = row['bxdelivered_sum_plt']
    
                
    if(bxdelivered_sum_bcm1futca>MINLUMICUT and MINTIME<t and t<MAXTIME):  
####change timestamp ranges of the fill you want to plot

        if  (timestamp >=1687983631 and timestamp <=1687983841)| (timestamp >=1688002095 and timestamp <=1688016290)|(timestamp >=1688033447 and timestamp <=1688034682)\
           |(timestamp >=1688037153 and timestamp <=1688056921)|(timestamp >=1688067714 and timestamp <=1688069952):
            GLumi.SetPoint(GLumi.GetN(), t, pcctotal)
            GLumiHF.SetPoint(GLumiHF.GetN(), t, bxdelivered_sum_bcm1futca)

            if bxdelivered_sum_bcm1futca != 0 :
                GLumiRatio.SetPoint(GLumiRatio.GetN(), t, pcctotal / bxdelivered_sum_bcm1futca)
            else:
                pass


            HPCC.Fill(t, pcctotal)
            HHFET.Fill(t, bxdelivered_sum_hfet)
            HHFOC.Fill(t, bxdelivered_sum_hfoc)
            HPLT.Fill(t, bxdelivered_sum_plt)
            HBCM1f.Fill(t, bxdelivered_sum_bcm1f)
            HBCM1fUTCA.Fill(t, bxdelivered_sum_bcm1futca)

 
C = ROOT.TCanvas("canvas", "My Canvas", 1200, 800)
label = ROOT.TLatex()
label.SetTextSize(0.04)
plotlabel = "Fill 8999(vdM) 2023, #sqrt{s}=13.6 TeV"


label1 = ROOT.TLatex()
label1.SetTextSize(0.04)

#############################################
MarkerSize  = .8
markerstyle = 8
LineWidth = 2
###########################################
C.Clear()
lineBCM1fUTCA = ROOT.TLine()
lineBCM1fUTCA.SetLineColor(ROOT.kBlue)  # Set the color of line1 to blue                                                     
lineBCM1fUTCA.SetLineWidth(LineWidth)

lineHFET = ROOT.TLine()                                                                                                      
lineHFET.SetLineColor(ROOT.kGreen)  # Set the color of line1 to blue                                                         
lineHFET.SetLineWidth(LineWidth)

lineHFOC = ROOT.TLine()                                                                                                      
lineHFOC.SetLineColor(ROOT.kOrange)  # Set the color of line1 to blue                                                        
lineHFOC.SetLineWidth(LineWidth)

lineBCM1f = ROOT.TLine()                                                                                                     
lineBCM1f.SetLineColor(ROOT.kRed)  # Set the color of line1 to blue                                                          
lineBCM1f.SetLineWidth(LineWidth)

linePLT = ROOT.TLine()                                                                                                       
linePLT.SetLineColor(ROOT.kViolet)  # Set the color of line1 to blue                                                         
linePLT.SetLineWidth(LineWidth)

################################################################################################

#ran_inf = 0.995                                                                                                        
#ran_sup = 1.005

HBCM1fUTCA.Divide(HPCC)
        
for i in range(1, HBCM1fUTCA.GetNbinsX() + 1, NDIV):
    valores_intervalo = [HBCM1fUTCA.GetBinContent(j) for j in range(i, i +NDIV)]
    valores_no_cero = [valor for valor in valores_intervalo if ran_inf <= valor <= ran_sup]
    promedio_valor = sum(valores_no_cero) / len(valores_no_cero) if len(valores_no_cero) > 0 else 0
    promedio_x = sum([HBCM1fUTCA.GetBinCenter(j) for j in range(i, i + NDIV)]) / NDIV
    bin_tprofile = Ratio_BCM1fUTCA.FindBin(promedio_x)
    Ratio_BCM1fUTCA.SetBinContent(bin_tprofile, promedio_valor)
    print(bin_tprofile, promedio_valor)
meanBCM1fUTCAfinal =np.mean([Ratio_BCM1fUTCA.GetBinContent(bin_number) for bin_number in range(1,  Ratio_BCM1fUTCA.GetNbinsX()+ 1) if Ratio_BCM1fUTCA.GetBinContent(bin_number) != 0]) 


HHFET.Divide(HPCC)
for i in range(1, HHFET.GetNbinsX() + 1,NDIV):                                                                            
    valores_intervalo = [HHFET.GetBinContent(j) for j in range(i, i + NDIV)]                                               
    valores_no_cero = [valor for valor in valores_intervalo if ran_inf <= valor <= ran_sup]                                   
    promedio_valor = sum(valores_no_cero) / len(valores_no_cero) if len(valores_no_cero) > 0 else 0                           
    promedio_x = sum([HHFET.GetBinCenter(j) for j in range(i, i + NDIV)]) / NDIV                                             
    bin_tprofile = Ratio_HFET.FindBin(promedio_x)                                                                      
    Ratio_HFET.SetBinContent(bin_tprofile, promedio_valor)                                                             
meanHFETfinal =np.mean([Ratio_HFET.GetBinContent(bin_number) for bin_number in range(1,  Ratio_HFET.GetNbinsX()+ 1) if Ratio_HFET.GetBinContent(bin_number) != 0])

HHFOC.Divide(HPCC)
for i in range(1, HHFOC.GetNbinsX() + 1, NDIV):                                                                               
    valores_intervalo = [HHFOC.GetBinContent(j) for j in range(i, i + NDIV)]                                                  
    valores_no_cero = [valor for valor in valores_intervalo if ran_inf <= valor <= ran_sup]                                   
    promedio_valor = sum(valores_no_cero) / len(valores_no_cero) if len(valores_no_cero) > 0 else 0                           
    promedio_x = sum([HHFOC.GetBinCenter(j) for j in range(i, i + NDIV)]) / NDIV                                              
    bin_tprofile = Ratio_HFOC.FindBin(promedio_x)                                                                             
    Ratio_HFOC.SetBinContent(bin_tprofile, promedio_valor)                                                                    
meanHFOCfinal =np.mean([Ratio_HFOC.GetBinContent(bin_number) for bin_number in range(1,  Ratio_HFOC.GetNbinsX()+ 1) if Ratio_HFOC.GetBinContent(bin_number) != 0])

HBCM1f.Divide(HPCC)
for i in range(1, HBCM1f.GetNbinsX() + 1, NDIV):                                                                              
    valores_intervalo = [HBCM1f.GetBinContent(j) for j in range(i, i + NDIV)]                                                 
    valores_no_cero = [valor for valor in valores_intervalo if ran_inf <= valor <= ran_sup]                                   
    promedio_valor = sum(valores_no_cero) / len(valores_no_cero) if len(valores_no_cero) > 0 else 0                           
    promedio_x = sum([HBCM1f.GetBinCenter(j) for j in range(i, i + NDIV)]) / NDIV                                             
    bin_tprofile = Ratio_BCM1f.FindBin(promedio_x)                                                                            
    Ratio_BCM1f.SetBinContent(bin_tprofile, promedio_valor)                                                                   
meanBCM1ffinal =np.mean([Ratio_BCM1f.GetBinContent(bin_number) for bin_number in range(1,  Ratio_BCM1f.GetNbinsX()+ 1) if Ratio_BCM1f.GetBinContent(bin_number) != 0])

HPLT.Divide(HPCC) 
for i in range(1, HPLT.GetNbinsX() + 1, NDIV):                                                                                
    valores_intervalo = [HPLT.GetBinContent(j) for j in range(i, i + NDIV)]                                                   
    valores_no_cero = [valor for valor in valores_intervalo if ran_inf <= valor <= ran_sup]                                   
    promedio_valor = sum(valores_no_cero) / len(valores_no_cero) if len(valores_no_cero) > 0 else 0                           
    promedio_x = sum([HPLT.GetBinCenter(j) for j in range(i, i + NDIV)]) / NDIV                                               
    bin_tprofile = Ratio_PLT.FindBin(promedio_x)                                                                            
    Ratio_PLT.SetBinContent(bin_tprofile, promedio_valor)                                                                   
meanPLTfinal =np.mean([Ratio_PLT.GetBinContent(bin_number) for bin_number in range(1,  Ratio_PLT.GetNbinsX()+ 1) if Ratio_PLT.GetBinContent(bin_number) != 0])
##########################################################################################
C.Clear()
miny = .985 
maxy = 1.015

Ratio_BCM1fUTCA.SetStats(0)
Ratio_BCM1fUTCA.SetMarkerStyle(markerstyle)
Ratio_BCM1fUTCA.SetMarkerSize(MarkerSize)
Ratio_BCM1fUTCA.SetMarkerColor(ROOT.kBlue)
Ratio_BCM1fUTCA.GetYaxis().SetTitle("Luminosity ratio (9 BCID's, "+str(NDIV)+" LS)")
Ratio_BCM1fUTCA.GetXaxis().SetTitle("Time [hr]")
Ratio_BCM1fUTCA.GetYaxis().SetRangeUser(miny,maxy)
Ratio_BCM1fUTCA.Draw("histp")

Ratio_HFET.SetStats(0)
Ratio_HFET.SetMarkerStyle(markerstyle)
Ratio_HFET.SetMarkerSize(MarkerSize)
Ratio_HFET.SetMarkerColor(ROOT.kGreen)
#HHFET.GetYaxis().SetTitle("HFET Lumi / "+ det+ " Lumi per 20 LS")
Ratio_HFET.GetXaxis().SetTitle("Time [hr]")
Ratio_HFET.GetYaxis().SetRangeUser(miny,maxy)
Ratio_HFET.Draw("histp same")

Ratio_HFOC.SetStats(0)
Ratio_HFOC.SetMarkerStyle(markerstyle)
Ratio_HFOC.SetMarkerSize(MarkerSize)
Ratio_HFOC.SetMarkerColor(ROOT.kOrange)
#HHFOC.GetYaxis().SetTitle("HFOC Lumi / "+ det+ " Lumi per 20 LS")
Ratio_HFOC.GetXaxis().SetTitle("Time [hr]")
Ratio_HFOC.GetYaxis().SetRangeUser(miny,maxy)
Ratio_HFOC.Draw("histp same")

Ratio_BCM1f.SetStats(0)
Ratio_BCM1f.SetMarkerStyle(markerstyle)
Ratio_BCM1f.SetMarkerSize(MarkerSize)
Ratio_BCM1f.SetMarkerColor(ROOT.kRed)
#HBCM1F.GetYaxis().SetTitle("BCM1F Lumi / "+ det+ " Lumi per 20 LS")
Ratio_BCM1f.GetXaxis().SetTitle("Time [hr]")
Ratio_BCM1f.GetYaxis().SetRangeUser(miny,maxy)
Ratio_BCM1f.Draw("histp same")

Ratio_PLT.SetStats(0)
Ratio_PLT.SetMarkerStyle(markerstyle)
Ratio_PLT.SetMarkerSize(MarkerSize)
Ratio_PLT.SetMarkerColor(ROOT.kViolet)
#HPLT.GetYaxis().SetTitle("PLT Lumi / "+ det+ " Lumi per 20 LS")
Ratio_PLT.GetXaxis().SetTitle("Time [hr]")
Ratio_PLT.GetYaxis().SetRangeUser(miny,maxy)
Ratio_PLT.Draw("histp same")

lineBCM1fUTCA.DrawLine(0, meanBCM1fUTCAfinal, 25, meanBCM1fUTCAfinal)
lineHFET.DrawLine(0, meanHFETfinal, 25, meanHFETfinal)
lineHFOC.DrawLine(0, meanHFOCfinal, 25, meanHFOCfinal)
lineBCM1f.DrawLine(0, meanBCM1ffinal, 25, meanBCM1ffinal)
linePLT.DrawLine(0, meanPLTfinal, 25, meanPLTfinal)


label1.SetTextColor(ROOT.kBlue)
label1.DrawLatexNDC(0.25, 0.82, "#bullet")
label1.SetTextColor(ROOT.kBlack)                                                                                              
label1.DrawLatexNDC(0.265, 0.82, "BCM1Fu/PCC")

label1.SetTextColor(ROOT.kGreen)
label1.DrawLatexNDC(0.45, 0.82, "#bullet")
label1.SetTextColor(ROOT.kBlack)                                                                                              
label1.DrawLatexNDC(0.465, 0.82, "HFET/PCC")

label1.SetTextColor(ROOT.kRed)
label1.DrawLatexNDC(0.65, 0.82, "#bullet")
label1.SetTextColor(ROOT.kBlack)                                                                                              
label1.DrawLatexNDC(0.665, 0.82, "HFOC/PCC")

label1.SetTextColor(ROOT.kOrange)
label1.DrawLatexNDC(0.25, 0.77, "#bullet")
label1.SetTextColor(ROOT.kBlack)                                                                                             
label1.DrawLatexNDC(0.265, 0.77, "BCM1F/PCC")

label1.SetTextColor(ROOT.kViolet)
label1.DrawLatexNDC(0.45, 0.77, "#bullet")
label1.SetTextColor(ROOT.kBlack)                                                                                             
label1.DrawLatexNDC(0.465, 0.77, "PLT/PCC")


label1.SetTextColor(ROOT.kBlack)
label1.DrawLatexNDC(0.53, 0.935, plotlabel)
label1.DrawLatexNDC(0.15, 0.935,"CMS Preliminary")
#C.SetResolution(300)

C.Print('/eos/user/l/lcuevasp/ratios/'+det_dir+'/ratio_comparisons_PCC_Correct'+str(NDIV)+'LS.png','png:300')
