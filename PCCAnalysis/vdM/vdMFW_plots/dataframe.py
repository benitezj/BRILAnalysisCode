import pandas as pd
import json, csv, pickle, yaml
import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import os, re, sys, math, bisect
from io import StringIO

import time

import datetime as dt

from scipy.interpolate import interp1d


from pprint import pprint

res = {}
fit_df_list   = []
calib_df = None


#rootdir = "/eos/user/l/lcuevasp/2017_Analisys/output_Luis_vs_Peter"
#rootdir = "/afs/cern.ch/user/l/lcuevasp/main_reproc_feature_qg_old/VdMFramework"
#rootdir = "/eos/user/p/pmajor/pccFitsForLuis"
#rootdir = "/afs/cern.ch/user/l/lcuevasp/vdMFW_ppRef_2024/VdMFramework"

rootdir = "/afs/cern.ch/user/l/lcuevasp/vdMFW_ppRef_2024/VdMFramework"
#rootdir = "/eos/user/l/lcuevasp/2023_Analisys"
for outdir in os.listdir(rootdir):
    if not outdir.startswith("output"):
        continue
    print(outdir)
   # outdirParts = outdir.split("__")
    path1  = os.path.join(rootdir, outdir, "analysed_data")
    for scan in os.listdir(path1):
        print(path1)
        for det in ["pcchd5","HFET","PLT"]:
            path2 = os.path.join(path1, scan, det)
            print(path2)
            if not os.path.exists(os.path.join(path1, scan, det)):
                continue
          #  print(path2)
            detstr = det
                           
            path3 = os.path.join(path2, "results")
            for origianl_corr_str in os.listdir(path3):
                print(path3)
                # Background_OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta_LengthScale_ResidualOrbitDrift_PeakToPeak
                corr_list = []
                if "noCorr" in origianl_corr_str:
                    corr_list.append("noCorr")
                if "Background" in origianl_corr_str:
                    corr_list.append("BG")
                if "ResidualOrbitDrift" in origianl_corr_str:
                    corr_list.append("OD3")
                elif "OrbitDriftRate" in origianl_corr_str:
                    corr_list.append("OD2")
                elif "OrbitDriftSep" in origianl_corr_str:
                    corr_list.append("OD1")
                if "BeamBeam" in origianl_corr_str:
                    corr_list.append("BB")
                if "DynamicBeta" in origianl_corr_str:
                    corr_list.append("DB") 
                if "LengthScale" in origianl_corr_str:
                    corr_list.append("LS") 
                if "PeakToPeak" in origianl_corr_str:
                    corr_list.append("P2P")   
                corr_str = "_".join(corr_list)
                                
                for fitresultname in os.listdir(os.path.join(path3, origianl_corr_str)):
                    
                    if fitresultname.endswith("FitResults.csv"):
                        
                        columns2read=["Type","BCID",
                                     "CapSigma","CapSigmaErr","peak","peakErr",
                                     "fitStatus","chi2","ndof","covStatus"]
                        if "Const" in fitresultname:
                            columns2read.extend(["Const","ConstErr"])
                
                        # print("reading", os.path.join(path3, origianl_corr_str, fitresultname))
                        # start = time.time()
                        fp = os.path.join(path3, origianl_corr_str, fitresultname)
                        try:
                            table = pd.read_csv(
                                fp, 
                                usecols = columns2read
                                )
                        except pd.errors.EmptyDataError as e:
                            print("empty",fp)
                            continue
                        except Exception as e:
                            raise e
                        table.columns = table.columns.str.replace(' ', '')  
                        
                        # print(time.time()-start) 
                        # start = time.time()
                        
                        # rename_dict = {}
                        # for name in table.columns:
                        #     if not name in ["Scan","Type","BCID"]:
                        #         rename_dict[name] = ":".join([corr_str, fitresultname.split("_")[0], name])                                
                        # table = table.rename(columns=rename_dict)     
                        table["Corr"] = corr_str
                        table["Scan"] = scan
                        table["Func"] = fitresultname.split("_")[0]
                        table["Det"]  = detstr

                       # if det == 'PLT':
                        #    table["Tag"]  = "limC"
                       # if det == 'Vtx_hd5':
                        #    table["Tag"]  = "limC"
                       # elif det == 'Vtx_hd5_':
                        #    table["Tag"]  = "nolimC"
                       # elif det == 'pcchd5_C':
                        #    table["Tag"]  = "nolimC"
                       # else:
                        #    table["Tag"]  = "none"
                        table["Tag"]  = "none"
                      #  table["bkg"]  = outdirParts[2]
                      #  table["rod"]  = outdirParts[1]
                      #  table["Tag"]  = "__".join(outdirParts[1:])
                        taglist = []
                            
                        # if scan=="6016_28Jul17_212416_28Jul17_214320" and detstr=="pcchd5" and len(taglist)==0 and  corr_str=="OD3_BB_DB_LS_P2P" and fitresultname.split("_")[0] == "SuperGConst":
                        #     print(os.path.join(path3, origianl_corr_str, fitresultname))
                                          
                        table.reset_index(drop=True, inplace=True)
                
                        table.round({"chi2":2})
                        
                        # print(time.time()-start)
                        # start = time.time()
                        
                        # Never call DataFrame.append or pd.concat inside a for-loop. It leads to quadratic copying
                        fit_df_list.append(table)
                            
                        # print(time.time()-start)
                        
fit_df = pd.concat(fit_df_list)
fit_df.reset_index(drop=True, inplace=True)    
                   
sorted_col = ["Scan", "Corr", "Det", "Tag", "Func", "Type", "BCID"] 
sorted_col+= [n for n in fit_df.columns if not n in sorted_col]
fit_df = fit_df.reindex(columns=sorted_col) 


print(fit_df["Tag"].unique())
print(fit_df["Det"].unique())


xyCorr = {
        "6016_28Jul17_100134_28Jul17_102201": 0.358, #1.00358,
        "6016_28Jul17_104434_28Jul17_110623": 0.453, #1.00453,
        "6016_28Jul17_115005_28Jul17_121320": 0.501, #1.00501,
        "6016_28Jul17_124433_28Jul17_130718": 0.492, #1.00492,
        "6016_28Jul17_144025_28Jul17_150150": 0.387, #1.00387,
        "6016_28Jul17_212416_28Jul17_214320":-0.010, #0.99989,

        "6868_30Jun18_132834_30Jun18_135235": 0.355, #1.00355,
        "6868_30Jun18_151040_30Jun18_153416": 0.393, #1.00393,
        "6868_01Jul18_002854_01Jul18_005345": 0.011, #1.00012,
        "6868_01Jul18_012042_01Jul18_014645":-0.037, #0.99962,
        "6868_01Jul18_030039_01Jul18_032641":-0.086, #0.99913,
        "6868_01Jul18_035146_01Jul18_041104":-0.076, #0.99924,
        "6868_01Jul18_075134_01Jul18_081132":-0.284, #0.99715,
}


print(len(fit_df[fit_df["Type"]=="X"]))
temp = fit_df[fit_df["Type"]=="X"][["Scan", "Det", "Tag", "Corr", "Func", "BCID"]].value_counts(dropna=False).reset_index(name='size')
temp = temp.sort_values(["size", "Scan", "Det", "Tag", "Corr", "Func", "BCID"])

print(len(temp))
with pd.option_context('display.max_rows', 30, 'display.max_columns', None, 'display.width', 1200):
    print(temp[temp["size"]>1])

temp["size"]



sigmaVis_df = pd.merge(fit_df[fit_df["Type"]=="X"], fit_df[fit_df["Type"]=="Y"], 
                       how="inner", on=["Scan", "Det", "Tag", "Corr", "Func", "BCID"],
                       suffixes=['_X', '_Y'])
sigmaVis_df.reset_index()
sigmaVis_df["SigmaVis"] = 2*np.pi*(sigmaVis_df["peak_X"]+sigmaVis_df["peak_Y"])/2*sigmaVis_df["CapSigma_X"]*sigmaVis_df["CapSigma_Y"]
sigmaVis_df["SigmaVis"] *= 10**6

relErr2 = (sigmaVis_df["CapSigmaErr_X"] / sigmaVis_df["CapSigma_X"] )**2 + (sigmaVis_df["CapSigmaErr_X"] / sigmaVis_df["CapSigma_X"] )**2 + (sigmaVis_df["peakErr_X"] ** 2 + sigmaVis_df["peakErr_Y"] ** 2) / (sigmaVis_df["peak_X"]+sigmaVis_df["peak_Y"]) ** 2
sigmaVis_df["SigmaVis_e"] = relErr2**0.5 * sigmaVis_df["SigmaVis"]


#def f(row):
 #   return (1 + xyCorr[row["Scan"]]*0.01)*row["SigmaVis"]
#sigmaVis_df["SigmaVis_xycorr"] = sigmaVis_df.apply(f, axis=1)

print(sigmaVis_df.columns)


assert len(fit_df)/len(sigmaVis_df) == 2.0

print(len(fit_df), len(sigmaVis_df), len(fit_df)/len(sigmaVis_df))
print(sys.getsizeof(fit_df)/10**6)
print(sys.getsizeof(sigmaVis_df)/10**6)



sigmaVis_df.to_csv("/eos/user/l/lcuevasp/output_ppRef/10299_SigmaVis_Alca13bcids.csv")
