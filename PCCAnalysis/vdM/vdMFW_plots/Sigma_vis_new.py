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


det  = "pcchd5"

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
    "10299_29Oct24_100235_29Oct24_101736":"vdM8",
}

scanorder = {
    6016:[
        "6016_28Jul17_100134_28Jul17_102201",
        "6016_28Jul17_104434_28Jul17_110623",
        "6016_28Jul17_115005_28Jul17_121320",
        "6016_28Jul17_124433_28Jul17_130718",
        "6016_28Jul17_144025_28Jul17_150150",
        "6016_28Jul17_212416_28Jul17_214320"
        ],
    10299:[
         "10299_29Oct24_032349_29Oct24_033328",
         "10299_29Oct24_070902_29Oct24_072405",                                                                             
         "10299_29Oct24_083237_29Oct24_081733",                                                                             
         "10299_29Oct24_100235_29Oct24_101736"
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

def plot_SigVis_graph(detectors, tag_corr_ff, normalize = False, fill=6016, namePostfix=None, ylim=None):
    
    year = {6016:2017, 6868:2018, 10299:2024}[fill]
    
    
    fig = plt.figure()
    ax = fig.add_subplot()
    
    ax.set_ylabel(r"$\sigma_{vis}$ [$\mu$b]")    
    scale = 1
    if (not detectors is None) and len(detectors)==1 and detectors[0]=="pcchd5":
        ax.set_ylabel(r"$\sigma_{vis}$ [mb]")
        scale = 1e-3
        #  scale = 1
    if "pcchd5" in [x[0] for x in tag_corr_ff]:
        ax.set_ylabel(r"$\sigma_{vis}$ [mb]")
        scale = 1e-3
      #  scale = 1 
    if normalize:
        ax.set_ylabel(r"$\sigma_{vis}^{scan}/\sigma_{vis}-1$ [%]")
    else:
        pass
        # print(detectors[0], tag_corr_ff[0])
    
    hep.cms.text(studyStatus, loc=0)
    
    xticklabels = [scanlabels[s] for s in scanorder[fill]]
    ax.set_xticks( range(len(xticklabels)) )
    ax.set_xticklabels(xticklabels, rotation=0, ha="center")
    
    if detectors is None:
        det_tag_corr_ff = tag_corr_ff
    else:
        det_tag_corr_ff = []
        for detector in detectors:
            for tag, correction, fitfunction in tag_corr_ff:
                det_tag_corr_ff.append([detector, tag, correction, fitfunction])
        

    N = len(det_tag_corr_ff)
    detectors_ = list(set([v[0] for v in det_tag_corr_ff]))
    tags = list(set([v[1] for v in det_tag_corr_ff]))
    corrections = list(set([v[2] for v in det_tag_corr_ff]))
    fitfunctions = list(set([v[3] for v in det_tag_corr_ff]))
        
    
    
    i=-1
    for detector, tag, correction, fitfunction in det_tag_corr_ff:
            
        i+=1
        
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
                       # & (sigmaVis_df["SigmaVis_e"]<0.2*sigmaVis_df["SigmaVis"])
                       # & ((sigmaVis_df["SigmaVis_e"] > 0.01 * sigmaVis_df["SigmaVis"]) & (sigmaVis_df["SigmaVis_e"] < 0.2 * sigmaVis_df["SigmaVis"]))
            #&(sigmaVis_df["SigmaVis"]>600000)
                        & ( (sigmaVis_df["Func"] == "CS") | ((sigmaVis_df["fitStatus_X"] < 2) & (sigmaVis_df["fitStatus_Y"] < 2 )) )
                        & (sigmaVis_df["covStatus_X"] == 3) & (sigmaVis_df["covStatus_Y"] == 3) & (sigmaVis_df["fitStatus_Y"] < 2) & ((sigmaVis_df["chi2_X"]/sigmaVis_df["ndof_X"]) < 5) & ((sigmaVis_df["chi2_Y"]/sigmaVis_df["ndof_Y"]) < 5) 
                        ]
        
        # if not detector.lower().startswith("pcc"):
        #     df = df[(df["covStatus_X"] == 3) & (df["covStatus_Y"] == 3) ]
                
        if len(df)==0:
            print("WARNING", detector, correction, fitfunction)
        
        n = []
        y  = []
        ye = []
        for scan in scanorder[fill]:
            temp = df[(df["Scan"] == scan)]
            
            assert len(temp) == len(temp["BCID"].unique())
            
            if len(temp)==0:
                print("WARNING", tag, detector, correction, fitfunction, scan, "len(df)", len(df))
                print(df["Scan"].unique())
            if "XY" in correction:
                v = (temp["SigmaVis_xycorr"]).values
            else:
                v = (temp["SigmaVis"]).values
                
            # computing the sigVis for HF is mod4! 
            # this is to prevent bias due to potentially non-convergent fits. 
            if detector.startswith("HF"):
                bcid = (temp["BCID"]).values
                w = []
                for imod in range(4):
                    v1 = np.mean(v[bcid%4==imod])
                    w.append(v1)
                    
                n.append(len(v))
                y.append( np.mean(w) )
                ye.append( np.std(v, ddof=1) )
                # ye.append( np.std(v)/len(v)**0.5 )
                
            else:
                
                n.append(len(v))
                y.append( np.mean(v) )
                ye.append( np.std(v, ddof=1) )
                # ye.append( np.std(v)/len(v)**0.5 )
               
            if "EC" in correction:
                y[-1]*=(1+emittanceChangeCorr[scan])
                ye[-1]*=(1+emittanceChangeCorr[scan])
                
        label_list = []
        if len(detectors_)>1: 
            label_list.append(detector)
        if len(corrections)>1:  
            label_list.append(correction)
        if len(fitfunctions)>1:  
            label_list.append(fitfunction)
        if len(tags)>1:
            if not tag == "none":  
                label_list.append(tag)
        # if len(corrections)>1:  
        #     label_list.append(correction)
               
        # if tag == "none": 
        #     label_list = ["originalROD"]
        # else:
        #     label_list = [tag]
           
        if detectors is None and not normalize: 
            label = f"{det} "
            if det == "pcchd5":
                label = "PCC"
            if tag.endswith("VTX"):
                label = f"PCC+VTX "
            if True or year == 2024:
                if det == "pcchd5" and tag.endswith("bgfbun"):
                    label = f"{label} Bg Per BCID "
                if det == "pcchd5" and tag.endswith("bgf"):
                    label = f"{label} const. bg "
                if det == "pcchd5" and tag.endswith("none"):
                    label = f"{label}  "
            
            #label_list=[label]
            label = f"{label} {fitfunction} (mean={y[-1]:.2f}, std={ye[-1]:.2f})"
        
        y = np.array(y)
        ye = np.array(ye)
        y *= scale
        ye*= scale
        n = np.array(n)
        n_valid = n[np.logical_not( np.isnan( y ) )]
        y_valid = y[np.logical_not( np.isnan( y ) )]
        ye_valid = ye[np.logical_not( np.isnan( y ) )]
        n = n_valid
        y = y_valid
        ye= ye_valid
        # wm = np.average(y, weights=n)
        # wstd = np.average((y-wm)**2, weights=n)**0.5
        # wm = np.average(y)
        # wstd = np.average((y-wm)**2)**0.5
        wm = np.mean(y)
        wstd = np.std(y, ddof=1)
        
        if normalize:
            y = (y/wm -1)*100
            ye= (ye/wm)*100
            
            # wm2 = np.average(y, weights=n)
            # wstd2 = np.average((y-wm2)**2, weights=n)**0.5
            wstd2 = np.std(y, ddof=1)
            detector_ = detector.upper().replace("HD5", "").replace("PCVD", "")
            if tag.endswith("VTX"):
                detector_ += "+VTX"
            label_list = [ detector_, f"\tstd: {wstd2:.2f}%" ]
        else:
            if detector=="pcchd5":
                # label_list.append(f"\t{np.mean(wm):.4f}$\pm${wstd/len(y)**0.5:.4f}")
                # label_list.append(f"\t{np.mean(wm)/1000:.1f}$\pm${wstd/1000:.2f}")
                label_list.append(f"  {np.mean(wm):.4f}$\pm${wstd:.4f}")
            else:
                # label_list.append(f"\t{np.mean(wm):.2f}$\pm${wstd/len(y)**0.5:.2f}")
                label_list.append(f"  {np.mean(wm):.2f}$\pm${wstd:.2f}")
        label = " ".join(label_list)
            
        print( label,"aqui" )
            
        d = 0.08*(i-(N-1)/2)
        # ax.plot([], [], lw=0, color="k", label=f"{np.mean(y):.2f}")
       # label = "964.9$\pm$2.2 mb"
       # label = "4666.4$\pm$14.7 mb"
       # label = " 1178.8262$\pm$10.89 mb"
       # mean_ = np.mean(wm)
        mean_ = 1178.8
       # ax.axhline(y=np.mean(wm), linestyle='--', color='red',label=label)
        p = ax.errorbar(np.arange(len(y))+d, y, yerr=ye, fmt="o", markersize=6,label=label)
        #p =  ax.errorbar(np.arange(len(y))+d, y, yerr=ye, fmt="o", markersize=6, label=f"{fitfunction} {np.mean(y):.2f}$\pm${np.std(y):.2f}")
       # p =  ax.errorbar(np.arange(len(y))+d, y, yerr=ye, fmt="o", markersize=6, label=label)
        # if True or i == 2:
        #     m= np.mean( [y[0]] + y[2:] )
        #     m= np.mean( y )
        #     ax.plot([0, len(y)], [m, m], "--", color=p[0].get_color())
       #               

    
   
    ax.set_ylim([ax.get_ylim()[0], ax.get_ylim()[1]+(ax.get_ylim()[1]-ax.get_ylim()[0])*0.35])
    # handles, labels = ax.get_legend_handles_labels()
    # handles = handles[::2] + handles[1::2]
    # labels  = labels[::2] + labels[1::2]
    # legend  = ax.legend(handles, labels, ncol=2, columnspacing=-0.5, handletextpad=0.1)
    # for i, lh in enumerate(legend.legend_handles):
    #     if i % 2 == 1:
    #         lh.set_visible(False)
    ax.legend( fontsize=30 )
    #ax.legend(  )
    # ax.legend(ncol=2, fontsize=13 )
    # if N > 6:
    #     ax.legend(ncol=2, columnspacing=0.4, fontsize=16//(1+(N-1)//4))
        
    ax.text(0.98, 1.01, "Fill{:d} ({:d}, 5.36 TeV)".format(fill, year),
                horizontalalignment='right',
                verticalalignment='bottom',
                transform=ax.transAxes,
                fontname='sans-serif',
                color="k",
                fontsize=20)
    
    label_list = []
    if len(detectors_)==1:  
        label_list.append(detector)
    # if len(tags)==1 and not tag == "none":  
    #     label_list.append(tag)
    # if len(fitfunctions)==1:  
    #     label_list.append(fitfunction)
    # if len(corrections)==1:  
    #     label_list.append(correction)
    label = "\n".join(label_list).replace("__oldPLT", "")
    
    if not normalize:
        ax.text(0.05, 0.97, label,
                horizontalalignment='left',
                verticalalignment='top',
                transform=ax.transAxes,
                fontname='sans-serif',
                color="k",
                fontsize=18)    
    
    ax.figure.canvas.draw()
    ax.yaxis.offsetText.set_visible(False)
    if len( ax.yaxis.get_offset_text().get_text() )>0:
        ax.yaxis.set_label_text( ax.yaxis.get_label_text()+ r" / "+ ax.yaxis.get_offset_text().get_text() ) 
        
    if not ylim is None:
        ax.set_ylim(*ylim)
        
    namePostfix_=""
    if not namePostfix is None:
            namePostfix_ = "_" + namePostfix
        
    fig.subplots_adjust(bottom=0.1, left=0.15)
    fig.savefig("/eos/user/l/lcuevasp/output_ppRef/plots/10299_SigmaVis_Alca13bcids_Poly4G_BG.png", dpi=400)
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


plot_SigVis_graph(None, [
#    ("HFET", "none","BG", "QG"), 
#    ("HFET_Sruthy_SS", "none","BG", "QG"),
#    ("PLT", "none","BG", "QG"),
#    ("PLT_Sruthy_SS", "none","BG","QG"),
     ("pcchd5", "none","BG","Poly4G"),
#     ("pcchd5", "none","noCorr","QGConst")    
], normalize=False, fill=10299, namePostfix="_pcc")
