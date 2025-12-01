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
#data_Luis_oldROD.csv     data_Luis.csv
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

    6868:[
        "6868_30Jun18_132834_30Jun18_135235",
        "6868_30Jun18_151040_30Jun18_153416",
        "6868_01Jul18_002854_01Jul18_005345",
        "6868_01Jul18_012042_01Jul18_014645",
        "6868_01Jul18_030039_01Jul18_032641",
        "6868_01Jul18_035146_01Jul18_041104",
        "6868_01Jul18_075134_01Jul18_081132"
        ],

     10299:[                                                                                                                  
         "10299_29Oct24_032349_29Oct24_033328",                                                                               
         "10299_29Oct24_070902_29Oct24_072405",                                                                               
         "10299_29Oct24_083237_29Oct24_081733",                                                                               
         "10299_29Oct24_100235_29Oct24_101736"                                                                                
         ]  
}
def plot_CapSigmaRatio_graph_scan(reference_det_tag_corr_ff, det_tag_corr_ff_pairs, direction, fill):
    
    year = {6016:2017, 6868:2018, 10299:2024}[fill]
    
    
    fig = plt.figure()
    ax = fig.add_subplot()
    
    ax.set_ylabel("SigmaVis [$\mu$B]")
    
    hep.cms.text("Work in Progress", loc=0)
    
    xticklabels = [scanlabels[s] for s in scanorder[fill]]
    ax.set_xticks( range(len(xticklabels)) )
    ax.set_xticklabels(xticklabels, rotation=0, ha="center")
    
    if len(direction) == 1:
        ax.set_ylabel(f"$\Sigma_{direction}/\Sigma_{direction}^{{ref}}-1$ [%]")
    else:
        ax.set_ylabel("$\Sigma_X\Sigma_Y/(\Sigma_X^{ref}\Sigma_Y^{ref})-1$ [%]")
    hep.cms.text("Work in Progress", loc=0)
            
    det, tag, corr, ff = reference_det_tag_corr_ff
    corr_ = corr
    if corr.endswith("XY"):
        corr_ = corr[:-3]
      
    ref_df = sigmaVis_df[ 
                    (sigmaVis_df["Det"]  == det ) & 
                    (sigmaVis_df["Corr"] == corr_) & 
                    (sigmaVis_df["Func"] == ff  ) &
                    (sigmaVis_df["Tag"] == tag  ) 
                ]
    
    for i, (det, tag, corr, ff) in enumerate(det_tag_corr_ff_pairs):
        
        corr_ = corr
        if corr.endswith("XY"):
            corr_ = corr[:-3]
            
        df = sigmaVis_df[ 
                (sigmaVis_df["Det"]  == det ) & 
                (sigmaVis_df["Corr"] == corr_ ) & 
                (sigmaVis_df["Func"] == ff  ) &
                (sigmaVis_df["Tag"] == tag  ) 
            ]
      #  bcids = [265 , 865, 1780, 2192, 3380]  #6868
       # bcids = [41, 281, 872, 1783, 2063]  #6016
#        bcids = [96,117,138,159,191,212,233,254,286,307,328,349,381,402,423,444,476,497,518,539,571,592,613,634,666,687,708,729,761,782,803,824,987,1008,1029,1050,1082,1103,1124,1145,1180,1201,1222,1243,1275,1296,1317,1338,1370,1391,1412,1433,1465,1486,1507,1528,1560,1581,1602,1623,1655,1676,1697,1718,1875,1896,1917,1938,1970,1991,2012,2033,2071,2092,2113,2134,2166,2187,2208,2229,2261,2282,2303,2324,2356,2377,2398,2419,2451,2472,2493,2514,2546,2567,2588,2609,2766,2787,2808,2829,2861,2882,2903,2924,2956,2977,2998,3019,3051,3072,3093,3114,3149,3170,3191,3212,3244,3265,3286,3307]
      #  ref_df_filtered = ref_df[ref_df['BCID'].isin(bcids)]
       # df_filtered = df[df['BCID'].isin(bcids)]
       # df = pd.merge(ref_df_filtered, df_filtered, on=["BCID", "Scan"], how="inner", suffixes=['_ref', '_new'])
        df = pd.merge(ref_df, df, on=["BCID", "Scan"], how="inner", suffixes=['_ref', '_new'])
        
        print(df["BCID"].unique(),"###")
        # if len(df)==0:
        #     print("WARNING", det, corr, ff)
        #     continue
        # print( df["Scan"].unique() )
        
        y = []
        ye = []
        for scan in scanorder[fill]:
            temp = df[(df["Scan"] == scan)]
            assert len(temp) == len(temp["BCID"].unique())
                        
            if len(temp)==0:
                print("WARNING2", det, corr, ff)
                continue
            
            if len(direction) == 1:
                d = direction
               # print(temp["BCID"])
                temp = temp[(temp[f"covStatus_{d}_ref"] == 3) & (temp[f"covStatus_{d}_new"] == 3)]
                v  = (temp[f"CapSigma_{d}_new"] / temp[f"CapSigma_{d}_ref"]).values
                ve = ((temp[f"CapSigmaErr_{d}_new"]/temp[f"CapSigma_{d}_new"])**2 + (temp[f"CapSigmaErr_{d}_ref"] / temp[f"CapSigma_{d}_ref"])**2).values**0.5*v
                                                                            
            else:
               # print(temp["BCID"])
                temp = temp[(temp["covStatus_X_ref"] == 3) & (temp["covStatus_X_new"] == 3) & (temp["covStatus_Y_ref"] == 3) & (temp["covStatus_Y_new"] == 3) & ((temp["chi2_X_ref"]/temp["ndof_X_ref"]) < 5) & ((temp["chi2_Y_ref"]/temp["ndof_Y_ref"]) < 5)]
                v  = (temp["CapSigma_X_new"] * temp["CapSigma_Y_new"] / temp["CapSigma_X_ref"] / temp["CapSigma_Y_ref"]).values
                ve = (
                    (temp[f"CapSigmaErr_X_new"]/temp[f"CapSigma_X_new"])**2 + 
                    (temp[f"CapSigmaErr_X_ref"]/temp[f"CapSigma_X_ref"])**2 +
                    (temp[f"CapSigmaErr_Y_new"]/temp[f"CapSigma_Y_new"])**2 + 
                    (temp[f"CapSigmaErr_Y_ref"]/temp[f"CapSigma_Y_ref"])**2
                    ).values**0.5*v
                
            print(len(v), scan, det)
            
            v -= 1
            v *= 100
            ve*=100
            
            y.append(np.mean(v))
            ye.append(np.std(v))
        
       # print(ff,type(ff),"####")
       # if ff == "SimCapSigma":
        #    ff == "Poly4G"
            
        label = f"{det}, {corr}, {ff}, "
       # if label == "SimFit_PCC_Vtx, BG_OD3_BB_DB_LS_P2P, SimCapSigma, ":
        #    label == "SimFit_PCC_Vtx, BG_OD3_BB_DB_LS_P2P, Poly4G," 
        if tag != "none":
            label += f"{tag}, "
        label +=f"{np.mean(y):.2f}$\pm${np.std(y)/len(y):.2f} %"
    
        d = 0.1*(i-(len(det_tag_corr_ff_pairs)-1)/2)
        ax.errorbar(np.arange(len(y))+d, y, yerr=ye, fmt="o", markersize=6, label=label)
                
      
                       
    ax.set_ylim([ax.get_ylim()[0], ax.get_ylim()[1]+(ax.get_ylim()[1]-ax.get_ylim()[0])*0.5])
  #  ax.set_ylim(-1.5, 5.5)
    # handles, labels = ax.get_legend_handles_labels()
    # handles = handles[::2] + handles[1::2]
    # labels  = labels[::2] + labels[1::2]
    # legend  = ax.legend(handles, labels, ncol=2, columnspacing=-0.5, handletextpad=0.1)
    # for i, lh in enumerate(legend.legend_handles):
    #     if i % 2 == 1:
    #         lh.set_visible(False)
    
    ax.legend(fontsize=20, loc=(.01, .7))
        
    ax.text(0.98, 1.01, "Fill {:d} ({:d}, 5.36 TeV)".format(fill, year),
                horizontalalignment='right',
                verticalalignment='bottom',
                transform=ax.transAxes,
                fontname='sans-serif',
                color="k",
                fontsize=20)
    
    fig.subplots_adjust(bottom=0.1, left=0.15)
    fig.savefig("/eos/user/l/lcuevasp/output_ppRef/plots/CapSigmas_XY_Alca13bcids_Chi2.png", dpi=400)

plot_CapSigmaRatio_graph_scan(
    ("HFET", "none", "BG", "Poly4G"),
   # ("PLT", "none", "BG", "Poly4G"), 
    # ("BCM1FPCVD", "none", "BG_OD3_BB_DB_LS_P2P", "Poly4G"),  
    [
        ("HFET",  "none",  "BG",    "Poly4G"), 
#        ("PLT", "none", "BG", "Poly4G"),
#        ("PLT",  "none",  "BG_OD3_BB_DB_LS_P2P", "Poly4G"), 
        ("pcchd5", "none", "BG", "Poly4G"), 
      #  ("pcchd5_bgfAvg", "none", "BG_OD3_BB_DB_LS_P2P", "Poly4G"),
      #  ("pcchd5_bgfbun", "none", "BG_OD3_BB_DB_LS_P2P", "Poly4G"),
      #  ("vtxhd5", "none",  "OD3_BB_DB_LS_P2P",    "Poly4G"),
      #  ("pcchd5", "none", "BG_OD3_BB_DB_LS_P2P", "SimCapSigma"),
      #  ("pcchd5_BPIXL4", "none", "OD3_BB_DB_LS_P2P", "Poly4GConst"),
      #  ("pcchd5_C", "none", "OD3_BB_DB_LS_P2P", "Poly4GConst"),
      #   ("pcchd5_PartZB1", "none", "BG_OD3_BB_DB_LS_P2P", "Poly4G"),
      #   ("pcchd5_PartZB1", "none", "OD3_BB_DB_LS_P2P", "Poly4GConst")
         

    ],  
    "XY", fill=10299)
