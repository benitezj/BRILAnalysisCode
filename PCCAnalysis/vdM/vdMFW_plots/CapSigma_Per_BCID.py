import pandas as pd
import json, csv, pickle, yaml
import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import os, re, sys, math, bisect
from io import StringIO

import datetime as dt

from scipy.interpolate import interp1d

import itertools

import mplhep as hep
hep.style.use("CMS")

sigmaVis_df = pd.read_csv("/eos/user/l/lcuevasp/output_ppRef/10299_SigmaVis_Alca13bcids.csv")

studyStatus = "Work in Progress"
studyStatus = ""
plotExt = "pdf"


#det  = "pcchd5"
det  = "HFET"
emittanceChangeCorr = {
 '6016_28Jul17_100134_28Jul17_102201': 0.0021245428112078094, 
 '6016_28Jul17_104434_28Jul17_110623': -0.0022608502571140185, 
 '6016_28Jul17_115005_28Jul17_121320': 0.0024402888451126353, 
 '6016_28Jul17_124433_28Jul17_130718': 0.0023827515207309075, 
 '6016_28Jul17_144025_28Jul17_150150': 0.0022729729690006698, 
 '6016_28Jul17_212416_28Jul17_214320': 0.0020973626535918277,
 '6868_30Jun18_132834_30Jun18_135235': 0.0021853149547631792, 
 '6868_30Jun18_151040_30Jun18_153416': 0.0021681572078689237, 
 '6868_01Jul18_002854_01Jul18_005345': 0.0023739486988298145, 
 '6868_01Jul18_012042_01Jul18_014645': 0.0024811037397572216, 
 '6868_01Jul18_030039_01Jul18_032641': 0.0025091321705555913, 
 '6868_01Jul18_035146_01Jul18_041104': 0.0018673885921181196, 
 '6868_01Jul18_075134_01Jul18_081132': 0.001973928910163246
}


scanlabels = {
    "6016_28Jul17_100134_28Jul17_102201": "vdM1",
    "6016_28Jul17_104434_28Jul17_110623": "vdM2",
    "6016_28Jul17_115005_28Jul17_121320": "Img1",
    "6016_28Jul17_124433_28Jul17_130718": "Img2",
    "6016_28Jul17_144025_28Jul17_150150": "vdM3",
    "6016_28Jul17_212416_28Jul17_214320": "vdM4",
    "6868_30Jun18_132834_30Jun18_135235": "vdM1",        
    "6868_30Jun18_151040_30Jun18_153416": "Img1",                      
    "6868_01Jul18_002854_01Jul18_005345": "Img3",                
    "6868_01Jul18_012042_01Jul18_014645": "Img4",       
    "6868_01Jul18_030039_01Jul18_032641": "vdM2",                                                                             
    "6868_01Jul18_035146_01Jul18_041104": "vdM3",                                                    
    "6868_01Jul18_075134_01Jul18_081132": "vdM4",
    "8999_29Jun23_123257_29Jun23_125514": "1",
    "8999_29Jun23_110004_29Jun23_112226": "2",
    "8999_29Jun23_092415_29Jun23_094738": "3",
    "8999_29Jun23_073830_29Jun23_080352": "4",
    "8999_29Jun23_023227_29Jun23_025502": "5",
    "8999_29Jun23_013851_29Jun23_020425": "6",
    "8999_29Jun23_004658_29Jun23_011220": "7",
    "8999_28Jun23_000143_28Jun23_232943": "8",
    "10299_29Oct24_032349_29Oct24_033328":"vdM1",                                                                             
    "10299_29Oct24_070902_29Oct24_072405":"vdM3",                                                                             
    "10299_29Oct24_083237_29Oct24_081733":"vdM5",                                                                             
    "10299_29Oct24_100235_29Oct24_101736":"vdM8"
}

scanorder = {

    10299:[                                                                                                              
         "10299_29Oct24_032349_29Oct24_033328",                                                                               
         "10299_29Oct24_070902_29Oct24_072405",                                                                               
         "10299_29Oct24_083237_29Oct24_081733",                                                                               
         "10299_29Oct24_100235_29Oct24_101736"                                                                                
         ],
    
    6016:[
        "6016_28Jul17_100134_28Jul17_102201",
        "6016_28Jul17_104434_28Jul17_110623",
        "6016_28Jul17_115005_28Jul17_121320",
        "6016_28Jul17_124433_28Jul17_130718",
        "6016_28Jul17_144025_28Jul17_150150",
        "6016_28Jul17_212416_28Jul17_214320"
        ],

    6868:[                                                                                                                                       
        "6868_30Jun18_132834_30Jun18_135235",                                                                                                    
        "6868_30Jun18_151040_30Jun18_153416",                                                                                                    
        "6868_01Jul18_002854_01Jul18_005345",                                                                                                    
        "6868_01Jul18_012042_01Jul18_014645",                                                                                                    
        "6868_01Jul18_030039_01Jul18_032641",                                                                                                    
        "6868_01Jul18_035146_01Jul18_041104",                                                                                                    
        "6868_01Jul18_075134_01Jul18_081132"                                                                                                     
        ],

    8999:[                   
        "8999_29Jun23_123257_29Jun23_125514",                                                                                
        "8999_29Jun23_110004_29Jun23_112226",                                                                                
        "8999_29Jun23_092415_29Jun23_094738",                                                                                
        "8999_29Jun23_073830_29Jun23_080352",                                                                                
        "8999_29Jun23_023227_29Jun23_025502",                                                                                
        "8999_29Jun23_013851_29Jun23_020425",                                                                                
        "8999_29Jun23_004658_29Jun23_011220",                                                                                
        "8999_28Jun23_000143_28Jun23_232943",
        ]
}

def plot_SigVis_graph_bcid(detector, tag, correction, fitfunction, fill=6016, separatemod4= False):
    
    year = {6016:2017, 6868:2018, 10299:2024}[fill]
    
    
    #fig = plt.figure()
    #ax = fig.add_subplot()
   # fig, ax = plt.subplots(figsize=(9, 6))
    fig, ax = plt.subplots(figsize=(9, 6))
    ax.set_xlabel("BCID")
    ax.set_ylabel("CapSigma_Y [mm]")
    scale = 1
    if detector=="pcchd5":
        ax.set_ylabel(r"CapSigma_Y [mm]")
        scale = 1
    
    hep.cms.text(studyStatus, loc=0)
        
    scanlabel = None
                
    if correction.endswith("XY_EC"):
        correction_ = correction[:-6]
    elif correction.endswith("XY"):
        correction_ = correction[:-3]
    elif correction.endswith("EC"):
        correction_ = correction[:-3]
    else:
        correction_ = correction
        
    df = sigmaVis_df[ 
                    (sigmaVis_df["Det"] == detector) & 
                    (sigmaVis_df["Corr"] == correction_) & 
                    (sigmaVis_df["Func"] == fitfunction) &
                    (sigmaVis_df["Tag"] == tag) 
                    & (sigmaVis_df["SigmaVis_e"]<0.1*sigmaVis_df["SigmaVis"])
                    ]
    
   
    
    
    if len(df)==0:
        print("WARNING", detector, tag, correction, fitfunction)
    else:
        print(len(df))
    
    if not separatemod4:
        y  = []
        ye = []
        for iscan, scan in enumerate(scanorder[fill]):
            temp = df[(df["Scan"] == scan)]
            assert len(temp) == len(temp["BCID"].unique())
            
           # mask = (temp["Func"] == "CS") | ((sigmaVis_df["SigmaVis_e"]>0.01*sigmaVis_df["SigmaVis"])& ((temp["fitStatus_X"] < 2) & (temp["fitStatus_Y"] < 2 )& (temp["fitStatus_Y"] < 2) & ((temp["chi2_X"]/temp["ndof_X"]) < 3.5) & ((temp["chi2_Y"]/temp["ndof_Y"]) < 3.5))
           # mask = (temp["Func"] == "CS") | ((sigmaVis_df["SigmaVis_e"] > 0.01 * sigmaVis_df["SigmaVis"])
                                             #&(sigmaVis_df["SigmaVis"] > 600000)
            mask =                           ( (temp["fitStatus_X"] < 2) & (temp["fitStatus_Y"] < 2) & ((temp["chi2_X"] / temp["ndof_X"]) < 5) & ((temp["chi2_Y"] / temp["ndof_Y"]) < 5))

             
            
            print(f"bcids excluded:", temp[~mask].shape[0])
            temp = temp[mask]
            #& (sigmaVis_df["SigmaVis_e"]<0.2*sigmaVis_df["SigmaVis"])
            #((sigmaVis_df["SigmaVis_e"])!=0)&
            if "XY" in correction:
                y = (temp["SigmaVis_xycorr"]).values
            else:
                y = (temp["CapSigma_Y"]).values
            ye = (temp["CapSigmaErr_Y"]).values
            x = (temp["BCID"]).values
            print(y)
            print(ye)
            print(x)
            y = np.array(y)*scale
            ye= np.array(ye)*scale

            if "EC" in correction:
                y*=(1+emittanceChangeCorr[scan])
                ye*=(1+emittanceChangeCorr[scan])
                
            # y = (temp["chi2_X"] / temp["ndof_X"]).values
            # y = (temp["chi2_Y"] / temp["ndof_Y"]).values
        #    ye = 0
        
            label = scanlabels[scan]

            ax.errorbar(x+iscan*20, y, yerr=ye, fmt="o", markersize=6, label=label)
            
    else:
        
        print("=========")
        
        sigVis = []
        for j in range(4):
            
            x  = []
            y  = []
            ye = []
            for scan in scanorder[fill][0:]:
                
                temp = df[(df["Scan"] == scan)]
                temp = temp[(temp["BCID"] % 4 == j)]
                
                mask = (temp["Func"] == "CS") | ((temp["fitStatus_X"] < 2) & (temp["fitStatus_Y"] < 2 )& (temp["fitStatus_Y"] < 2) & ((temp["chi2_X"]/temp["ndof_X"]) < 3.5) & ((temp["chi2_Y"]/temp["ndof_Y"]) < 3.5))
                print(f"bcids excluded {j} cov:", temp[~mask].shape[0])
                temp = temp[mask]
                
                if "XY" in correction:
                    y.extend( (temp["SigmaVis_xycorr"]).values.tolist() )
                else:
                    y.extend( (temp["CapSigma_Y"]).values.tolist() )
                ye.extend( (temp["CapSigmaErr_Y"]).values.tolist() )
                x.extend( (temp["BCID"]).values )
                
                scanlabel = scanlabels[scan]
                
                break
                
            if j>0:
                label = f"$4k+${j}"
            else:
                label = "$4k$"
                
            sigVis.append( np.mean(y) )
            
            y = np.array(y)*scale
            ye= np.array(ye)*scale
            
            if "EC" in correction:
                y*=(1+emittanceChangeCorr[scan])
                ye*=(1+emittanceChangeCorr[scan])
                
            label += f"\t{np.mean(y):.2f}$\pm${np.std(y)/len(y)**0.5:.2f}"
            ax.errorbar(x, y, yerr=ye, fmt="o", markersize=6, label=label)
                        
        print(det, fill)    
        avgSigVis = np.mean(sigVis)
        for j in range(4):
            s = (avgSigVis/sigVis[j])
            print(f"mod4: {j} sigVis: {sigVis[j]:.2f} avgSigVis/sigVis: {s:.5f}" )
            
    #ax.set_ylim(600,950)                   
    ax.set_ylim([ax.get_ylim()[0], ax.get_ylim()[1]+(ax.get_ylim()[1]-ax.get_ylim()[0])*0.35])
    # handles, labels = ax.get_legend_handles_labels()
    # handles = handles[::2] + handles[1::2]
    # labels  = labels[::2] + labels[1::2]
    # legend  = ax.legend(handles, labels, ncol=2, columnspacing=-0.5, handletextpad=0.1)
    # for i, lh in enumerate(legend.legend_handles):
    #     if i % 2 == 1:
    #         lh.set_visible(False)
    
    if not separatemod4:
        ax.legend(ncol=2, columnspacing=0.4, fontsize=20)
    else:
        ax.legend(fontsize=20)
        
    ax.text(0.98, 1.01, "Fill {:d} ({:d}, 5.36 TeV)".format(fill, year),
                horizontalalignment='right',
                verticalalignment='bottom',
                transform=ax.transAxes,
                fontname='sans-serif',
                color="k",
                fontsize=20)
    
    if not scanlabel is None:
        scanlabel = ", "+scanlabel
    else:
        scanlabel = ""
    label_list = [ detector+scanlabel, fitfunction]
    # label_list = [ detector+scanlabel, fitfunction, correction]
    # if not tag == "none":
    #     label_list.append(tag)
    label = "\n".join(label_list).replace("__oldPLT", "")
    
    ax.text(0.03, 0.97, label,
                horizontalalignment='left',
                verticalalignment='top',
                transform=ax.transAxes,
                fontname='sans-serif',
                color="k",
                fontsize=18)

    offset = ax.yaxis.get_offset_text().get_text()
    
    ax.figure.canvas.draw()

  #  ax.yaxis.offsetText.set_visible(False)
    if offset:  # si existe offset
    # Agregarlo al ylabel
        ax.set_ylabel(ax.get_ylabel() + " " + offset)
    # Y ocultar el offset flotante
        ax.yaxis.offsetText.set_visible(False)

    
    if len( ax.yaxis.get_offset_text().get_text() )>0:
        ax.yaxis.set_label_text( ax.yaxis.get_label_text()+ r" / "+ ax.yaxis.get_offset_text().get_text() ) 

   # ax.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax.yaxis.get_offset_text().set_x(-0.15)
    # ax.set_ylim(0.91, 0.99)
    fig.tight_layout()
   # fig.subplots_adjust(bottom=0.15, left=0.2)
    fig.savefig("/eos/user/l/lcuevasp/output_ppRef/plots/CapSigma_Y_10299_Alca_Per_BCID_QG.png", dpi=400)
    #if not normalize:
    #    plotPath = f'analyzePlots/sigVis/sigVisPerScan_fill{fill}_{detector}_{correction}_{tag}{namePostfix_}.{plotExt}'
   # else:
        
    #    plotPath = f'analyzePlots/sigVis/sigVisPerScanNormalized_fill{fill}{namePostfix_}.{plotExt}'
        
   # dirPath = os.path.dirname(plotPath)
   # if not os.path.exists(dirPath):
   #     os.makedirs(dirPath)
   # fig.savefig(plotPath)            

#plot_SigVis_graph(None, [
        # ("pcchd5", "none",    "BG_OD3_BB_DB_LS",    "DG"),
 #        ("pcchd5", "none",    "BG_OD3_BB_DB_LS_P2P_XY",    "Poly4G"),
       #  ("pcchd5_BPIXL4", "none", "OD3_BB_DB_LS_P2P_XY",    "Poly4GConst"), 
        # ("Vtx_hd5", "none",   "OD3_BB_DB_LS_P2P_XY",    "Poly4G"),
        # ("pcchd5_CapS_Vtx", "none",   "BG",    "Poly4G"), 
#         (det, "none",   "BG",    "Poly4G"), 
  #   ], 6016)


fill=10299
ff = "Poly4G"
det = "pcchd5"
corr = "BG"
tag = 'none'#f'CommonLSBB_fill6016_arc__bgf'
plot_SigVis_graph_bcid(det, tag, corr, ff, fill, separatemod4 = False)
  
#plot_SigVis_graph(None, [ 
 #   ("pcchd5", "none","BG_OD3_BB_DB_LS_P2P_XY_EC", "Poly4G"),
#], normalize=False, fill=6016, namePostfix="_pccversions_XY_EC")



