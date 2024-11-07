import os
import re
import fnmatch
import argparse
import sys
from collections import defaultdict, OrderedDict

import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt
from scipy.stats import tmean, scoreatpercentile, sem

import plotting_dependencies.plot as ipt

mpl.use("Agg")
mpl.style.use("classic")
plt.rcParams["legend.numpoints"] = 1  # Show only one point in legend (defaut is 2)


# Define global variables

XY_CORR = "xyFactorization"

global capData, CapDict, CapAllDict, peakDict, peakAllDict, sigData, SigDict, SigAllDict

capData = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
CapDict = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
CapAllDict = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))

peakDict = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
peakAllDict = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
MeanDict = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
MeanAllDict = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
rchi2Dict = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
rchi2AllDict = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))

sigData = defaultdict(lambda: defaultdict(lambda: defaultdict(float)))
SigDict = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
SigAllDict = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))


# Define functions
def trimmean(arr, percent):
    lower_limit = scoreatpercentile(arr, percent / 2)
    upper_limit = scoreatpercentile(arr, 100 - percent / 2)
    return tmean(arr, limits=(lower_limit, upper_limit), inclusive=(False, False))


def identicalXAxis(axisList: np.array) -> bool:
    if len(axisList) > 0:
        axis0 = axisList[0]
        for axis in axisList:
            if len(axis0) != len(axis):
                return False
            if not (axis0 == axis).all():
                return False
    return True


def normpdf(x, mean, sd):
    var = float(sd) ** 2
    denom = (2 * np.pi * var) ** 0.5
    num = np.exp(-((float(x) - float(mean)) ** 2) / (2 * var))
    return num / denom


# Define here the way you want the output of the figures and csv files to be
# For exceptions, just manually change the code and do not call these functions
# Default is calling ipt.makeFileName(path, name, fill, scanName, corr, det, fit, xy)
# ipt.makeFileName code should never be changed!!!


def makeFigName(path: str, fill: str, scanName: str, var: str = "", corr: str = "", det: str = "", fit: str = "", xy: str = ""):
    return ipt.makeFileName(path, fill, scanName, var, corr, det, fit, xy)


def makeCSVName(path: str, fill: str, scanName: str, var: str = "", corr: str = "", det: str = "", fit: str = "", xy: str = ""):
    return ipt.makeFileName(path, fill, scanName, var, corr, det, fit, xy)


def getRefCorr(corr: str, corrs:dict):
    corrRefBool = True
    corrTmp = corr
    while corrRefBool:
        if len(corrTmp.split("_")) != 1:
            corrRef = corrTmp.replace("_" + corrTmp.split("_")[len(corrTmp.split("_")) - 1], "")
        else:
            corrRef = "noCorr"
        if corrRef in list(corrs.keys()):
            corrRefBool = False
        else:
            corrTmp = corrRef
        if (corrRef == "noCorr") and ("noCorr" not in list(corrs.keys())):
            print("noCorr is not in correction dictionary")
            print("Impossible to make correction effect plot")
            print("Please add noCorr to correction dictionary")
            print("Exit!")
            sys.exit()

    return corrRef


def getValueError(arr: np.array, bcid: float, scan: str = "X"):
    """arr = np.array[1][3][] or np.array[1][6][]"""
    value = None
    err = None
    if scan == "X":
        ixy = 0
    elif scan == "Y":
        ixy = 3
    else:
        print("Unknown scan. Only 'X' and 'Y' accepted.")
        print("Exit!")
        sys.exit()
    for i in range(len(arr[0][2 + ixy])):
        if arr[0][2 + ixy][i] == bcid:
            value = arr[0][0 + ixy][i]
            err = arr[0][1 + ixy][i]
            break
    return (value, err)


def allyear(path: str, Fit: str, dets: list, corrs: dict, FOM: bool = False):
    fint = len(path.split("/")) - 1
    print("All year data reading started!")
    (isFOM, freq) = ("FOM_", 11245.5) if FOM else ("", 1)
    for root, _, files in os.walk(path):
        for det in dets:
            for corr in list(corrs.keys()):
                if fnmatch.fnmatch(root, "*" + det + "/results/" + corr):
                    fill_name = root.split("/")[fint]
                    fit, covS, _ = Fit, 3, 0
                    c = ["k", "r", "g", "b", "y", "m", "c"]

                    filesearch = isFOM + fit + "_FitResults.csv"
                    for rfile in fnmatch.filter(files, filesearch):
                        fullpath = os.path.join(root, rfile)
                        data = pd.read_csv(fullpath)
                        bcidfilterX = np.asarray(data[(data["covStatus"]  == covS) & (data["Type"] == "X")]["BCID"])
                        bcidfilterY = np.asarray(data[(data["covStatus"]  == covS) & (data["Type"] == "Y")]["BCID"])

                        bcidfilter = np.intersect1d(bcidfilterX, bcidfilterY)

                    filesearch = isFOM + "LumiCalibration_*" + fit + "_*.csv"

                    for rfile in fnmatch.filter(files, filesearch):
                        fullpath = os.path.join(root, rfile)
                        data = pd.read_csv(fullpath)
                        if len(data["xsec"]) < 1000:
                            continue
                        att = np.asarray(data[(data["BCID"].isin(bcidfilter))]["xsec"]) / freq
                        attErr = np.asarray(data[(data["BCID"].isin(bcidfilter))]["xsecErr"]) / freq

                        if len(attErr) == 0:
                            continue
                        if FOM:
                            sigData[corr][det][fill_name] = np.around(float(trimmean(att, 30)), decimals=3)
                        else:
                            sigData[corr][det][fill_name] = np.around(float(np.average(att, weights=attErr)), decimals=3)
    if FOM:
        print("FOM analysis plots are in progress!")
        for corr in list(corrs.keys()):
            cc = 0
            for det in dets:
                y, _, x = ([] for i in range(3))
                for f, v in list(sigData[corr][det].items()):
                    if v > 1.1:
                        print(det, corr, f, v)
                    y.append(v)
                    # e.append( v[0][0].std())
                    x.append(int(f.split("_")[0]))
                plt.errorbar(x, y, yerr=0, fmt="o", color=c[cc])
                cc += 1

                plt.grid()
                plt.title(det + ", " + corrs[corr])
                plt.xlabel("Fill Numbers")
                plt.ylabel("FOM", fontsize=12)
                plt.ticklabel_format(useOffset=False, axis="y")  # Prevent plt from adding offset to y axis
                plt.figtext(0.04, 0.96, "CMS", fontsize=14, fontweight="bold", backgroundcolor="white")
                plt.figtext(0.11, 0.96, r"Preliminary 2018, $\sqrt{s}$=13 TeV", fontsize=14, style="italic", backgroundcolor="white")
                # plt.legend(loc='best', fontsize=12)
                folderpath = "./plots/FOM/"
                if not os.path.exists(folderpath):
                    os.makedirs(folderpath)
                plt.savefig(folderpath + det + "_" + Fit + "_" + corr + "_.png")
                plt.clf()


def perfill(
    fill: str,
    filedict: dict,
    dets: list,
    Fit: str,
    corrs: list,
    year: str,
    energy: str,
    suffix: str = "",
    SpecDict: dict = None,
    xyCorr: dict = None,
    CapRatio: bool = False,
    CapNormal: bool = False,
    CapEffect: bool = False,
    CapEvol: bool = False,
    peakNormal: bool = True,
    peakEffect: bool = False,
    SigEffect: bool = False,
    PeakPeakEffect: bool = False,
    SigNormal: bool = True,
    SigAll: bool = True,
    SigCorrelation: bool = False,
   # outputPlotsDirBase: str = "/eos/user/l/lcuevasp/2017_Analisys/new_ratefile_studys/output_allCorr/plots_Per_avg",
   # outputCsvDirBase: str = "/eos/user/l/lcuevasp/2017_Analisys/new_ratefile_studys/output_allCorr/output",
   # path: str = "/eos/user/l/lcuevasp/2022_Analisys/2022_output/vdm22_rerun_20230811/v2_new_tunes_rod_ls_all_sem",
    path: str = "/eos/user/l/lcuevasp/2023_Analisys/output_PCC/analysed_data",
    outputPlotsDirBase: str ="/eos/user/l/lcuevasp/2023_Analisys/output_PCC",                        
    outputCsvDirBase: str = "/eos/user/l/lcuevasp/2023_Analisys/output",
   # path: str = "/eos/user/l/lcuevasp/2017_Analisys/new_ratefile_studys/output_allCorr/analysed_data", 
    figureExtension: str = "pdf",
    bcidStart: int = 0,
    bcidEnd: int = 3564,
    xlim: list = None,
    refDet: str = "HFET",
    makeLatexTable: bool = True,
    colorList: list = None,
    chi2Scaling: bool =False,
    peak2peakCorr: bool = False,
    peak2peak: str = "perBX",
) -> None:

    """Plots quantities from vdM Framework results

    Parameters
    ----------
    fill : str
        Fill number(s). Used for txt plotting purpose and make output path name.
        Authorised values:

        * "": fill number is not written
        * "fill1 fill2 fill3" etc... e.g "7442" or "7442 7443"

    filedict : OrderedDict`
        keys are names of the scan files (vdm output) values are a short name for the scan to put in titles and labels.
        Order scans as you want them to appear in evol plots
    dets : list
        List of luminometer data to analyse
    Fit : str
        Fit function
    corrs : dict[str]:str
        Keys are the names of the corrections to read, as they appear in the vdm output directory (full name).
        Values are a short name for the corrections to put in titles and labels.
        For xy factorisation bias correction, the correction must be called "xyFactorization"
    year : str
        Year. Only used for text plotting purpose
    energy : str
        Center of mass energy. Only used for text plotting purpose
    suffix : str, optional
        Text identifier added to filenames, by default ""
    SpecDict : dict, optional
        Deprecated, by default None
    xyCorr : dict, optional
        Dictionary of the xy correction value per scan.
        If you asked for xxxx_xyFactorization correction (see above corrsPF), you must provide the xy factorisation bias correction dictionary.
        Set to None if no xyFactorization correction, by default None
    CapRatio : bool, optional
        Plots CapSigma_detD/CapSigma_refDet ratio, by default False
    CapNormal : bool, optional
        Plots CapSigma as a function of BCID, by default False
    CapEffect : bool, optional
        Plots effect of a correction on CapSigma as a function of BCID, by default False
    CapEvol : bool, optional
        Plots average CapSigma as a function of scan, by default False
    peakNormal : bool, optional
        Plots peak as a function of BCID, by default False
    peakEffect : bool, optional
        Plots effect of a correction on peak as a function of BCID, by default False
    SigEffect : bool, optional
        Plots effect of a correction on sigma visible as a function of BCID, by default False
    PeakPeakEffect : bool, optional
        Plots peak to peak effect on sigma visible as a function of BCID (sigma vis PP / sigma vis wo PP - 1), by default False
    SigNormal : bool, optional
        Plots sigma visible as a function of BCID, by default False
    SigAll : bool, optional
        Plots average sigma visible as a function of scan, by default False
    SigCorrelation : bool, optional
        Prints correlation between sigma vis of the different detectors, by default False
    outputPlotsDirBase : str, optional
        Path for output plots, by default "../output/plots"
    outputCsvDirBase : str, optional
        Path for csv files (csv files contain x,y coordinate of all points in plots), by default "../output/csv"
    path : str, optional
        Path to vdm output data for the perfill function, by default "../output/analysed_data"
    figureExtension : str, optional
        "png" or "pdf", by default "pdf"
    bcidStart : int, optional
        BCID range that will be read is [bcidStart, bcidEnd]. Attention: affects statistics, for zoom, use TODO, by default 0
    bcidEnd : int, optional
        BCID range that will be read is [bcidStart, bcidEnd]. Attention: affects statistics, for zoom, use TODO, by default 3564
    xlim : list, optional
        Upper and lower x limit, by default None
    refDet : str, optional
        Reference detector for CapRatio plots, by default "PLT"
    makeLatexTable : bool, optional
        * For SigNormal: make bunch-to-bunch systematic uncertainty table.
        * For SigAll: make table of average sigma visible for each correction (rows) with their effect, for each detector (columns)

        This might require the user some changes in sxsec.py to work correctly, by default True
    colorList : list, optional
        List of colors defining the color associated with a detector, by default None
    chi2Scaling : bool, optional
        If True, the error on fitted parameters (CapSigmas, peaks) is multiplied by max(1,sqrt(chi2/ndof)), by default True
    peak2peakCorr : bool, optional
        If True, the peak to peak correction is used, by default False
    peak2peak : str, optional
        Correction per beam or per BCID. Authorised values: "perBX" "beam", by default "perBX"
    """

    colorList = colorList or ["k", "r", "g", "b", "m", "c", "y"]
    SpecDict = SpecDict or {}
    xyCorr = xyCorr or {}

    if xlim:
        xlimBool = True
        xlimMin, xlimMax = xlim
    else:
        xlimBool = False

    # If fill is "fill1 fill2 fill3 etc..." then replace space by _ for output directory
    fillD = fill.replace(" ", "_")

    ## Define output directories for plots and csv files (more few lines below)
    OUTPUT_PLOTS_DIR_FILL = outputPlotsDirBase + fill
    OUTPUT_PLOTS_DIR_SCAN_NAME = {}
    OUTPUT_CSV_DIR_FILL = outputCsvDirBase + fill
    OUTPUT_CSV_DIR_SCAN_NAME = {}

    # Read fitResults and LumiCalibration files
    for dirr in os.listdir(path):
        regexFillName = r"[0-9]{4}(_[0-9]{2}[a-zA-Z]{3}[0-9]{2}_[0-9]{6}){2}"  # 7442_13Nov18_210836_13Nov18_211753/
        reSearch = re.search(regexFillName, dirr)
        if reSearch is None:
            continue
        if dirr in list(filedict.keys()):
            fill_name = dirr
            print(("\nFound scan " + fill_name))
            fillN = fill_name.split("_")[0]

            ## Define output directories for plots and csv files
            OUTPUT_PLOTS_DIR_SCAN_NAME[fill_name] = OUTPUT_PLOTS_DIR_FILL + "/" + fill_name
            OUTPUT_CSV_DIR_SCAN_NAME[fill_name] = OUTPUT_CSV_DIR_FILL + "/" + fill_name

            # scanno=int(filedict[fill_name].split('_')[0].split('n')[1])
            # if scanno==0: continue #Emittance scan skip  ## This number depends on the scan program !!!
            for det in dets:
                for corr in list(corrs.keys()):
                    if corr[-len(XY_CORR) :] == XY_CORR:
                        continue
                    fit, corrTemp = Fit, corr
                    # if (det in SpecDict.keys()) and (scanno in SpecDict[det][1]): fit=SpecDict[det][0]
                    if "Const" in fit:
                        corrTemp = "noCorr" if corr == "Background" else corr.replace("Background_", "")
                    ## Read FitResults.csv file
                    fullpath = path + "/" + fill_name + "/" + det + "/results/" + corrTemp + "/" + fit + "_FitResults.csv"
                    if not os.path.exists(fullpath):
                        print((fullpath + " file does not exist. Exit"))
                        sys.exit(1)
                    else:
                        ## Read CapSigma values
                        data = pd.read_csv(fullpath)
                        Ninit = len(data)

                        # Keep only converged fits
                        goodCovStatus = 3
                        goodBCIDsInX = data[
                            (data["covStatus"] == goodCovStatus)
                            & (data["Type"] == "X")
                        ]["BCID"].to_numpy()
                        goodBCIDsInY = data[
                            (data["covStatus"] == goodCovStatus)
                            & (data["Type"] == "Y")
                        ]["BCID"].to_numpy()
                        goodBCIDs = goodBCIDsInX

                        if goodBCIDsInX.shape != goodBCIDsInY.shape or np.any(goodBCIDsInX != goodBCIDsInY):
                            print("INFO: BCIDs in X and Y are not the same after covStatus cut.")
                            print("INFO: Keeping only BCIDs that are in both X and Y.")
                            goodBCIDs = np.intersect1d(goodBCIDsInX, goodBCIDsInY)

                        data = data[data["BCID"].isin(goodBCIDs)]

                        if data["covStatus"].unique() != goodCovStatus:
                            print(f"!=!=!WARNING!=!=!: covStatus not unique for {dirr} | {fill_name} | {det} | {corr} | {fit}")

                        # Keep only interesting bcids
                        data = data[(data["BCID"] <= bcidEnd) & (data["BCID"] >= bcidStart)]

                        print("Reading %s %s %s" % (det, corr, fit))
                        Ncuts = len(data)
                        frac = float(Ncuts) / Ninit
                        print(("\tFraction of dataset remaining after cuts: %.2f" % (frac)))
                        if frac < 0.5:
                            print("\tWARNING: Dataset has been considerably reduced!!!")

                        attX = np.asarray(data[data["Type"] == "X"]["CapSigma"])
                        attY = np.asarray(data[data["Type"] == "Y"]["CapSigma"])
                        attErrX = np.asarray(data[data["Type"] == "X"]["CapSigmaErr"])
                        attErrY = np.asarray(data[data["Type"] == "Y"]["CapSigmaErr"])
                        bcidfilterX = np.asarray(data[data["Type"] == "X"]["BCID"])
                        bcidfilterY = np.asarray(data[data["Type"] == "Y"]["BCID"])
                        bcidfilter = np.intersect1d(bcidfilterX, bcidfilterY)

                        CapAllDict[corr][det][fill_name].append([attX, attErrX, bcidfilterX, attY, attErrY, bcidfilterY])
                        CapDict[corr][fill_name][det].append([attX, attErrX, bcidfilterX, attY, attErrY, bcidfilterY])

                        # print 'CAP!!', fill_name, corr, det, fit
                        if len(attErrX) == 0 or len(attErrY) == 0:
                            continue
                        capData[corr][det][filedict[fill_name]].append(
                            [np.around(float(np.average(attX, weights=attErrX)), decimals=3), np.around(float(np.average(attY, weights=attErrY)), decimals=3)]
                        )
                        capData[corr][det][filedict[fill_name] + "_STD"].append(
                            [np.around(float(attX.std()), decimals=4), np.around(float(attY.std()), decimals=4)]
                        )
                        capData[corr][det][filedict[fill_name] + "_#BCID"].append([len(attX), len(attY)])

                        ## Read peak values
                        attX = np.asarray(data[data["Type"] == "X"]["peak"])
                        attY = np.asarray(data[data["Type"] == "Y"]["peak"])
                        attErrX = np.asarray(data[data["Type"] == "X"]["peakErr"])
                        attErrY = np.asarray(data[data["Type"] == "Y"]["peakErr"])
                        bcidfilterX = np.asarray(data[data["Type"] == "X"]["BCID"])
                        bcidfilterY = np.asarray(data[data["Type"] == "Y"]["BCID"])
                        bcidfilter = np.intersect1d(bcidfilterX, bcidfilterY)
                        peakAllDict[corr][det][fill_name].append([attX, attErrX, bcidfilterX, attY, attErrY, bcidfilterY])
                        peakDict[corr][fill_name][det].append([attX, attErrX, bcidfilterX, attY, attErrY, bcidfilterY])

                        ## Read Mean values
                        attX = np.asarray(data[data["Type"] == "X"]["Mean"])
                        attY = np.asarray(data[data["Type"] == "Y"]["Mean"])
                        attErrX = np.asarray(data[data["Type"] == "X"]["MeanErr"])
                        attErrY = np.asarray(data[data["Type"] == "Y"]["MeanErr"])
                        MeanAllDict[corr][det][fill_name].append([attX, attErrX, bcidfilterX, attY, attErrY, bcidfilterY])
                        MeanDict[corr][fill_name][det].append([attX, attErrX, bcidfilterX, attY, attErrY, bcidfilterY])

                        ## Read chi2 values
                        attX = np.asarray(data[data["Type"] == "X"]["chi2"])
                        attY = np.asarray(data[data["Type"] == "Y"]["chi2"])
                        ndofX = np.asarray(data[data["Type"] == "X"]["ndof"])
                        ndofY = np.asarray(data[data["Type"] == "Y"]["ndof"])
                        rchi2AllDict[corr][det][fill_name].append([attX / ndofX, attY / ndofY])
                        rchi2Dict[corr][fill_name][det].append([attX / ndofX, attY / ndofY])

                    ## Read LumiCalibration*.csv file#######################################################
                    fullpath = path + "/" + fill_name + "/"+det+"/results/" + corrTemp + "/" + "LumiCalibration_pcchd5_" + fit + "_" + fillN + ".csv"
                    if not os.path.exists(fullpath):
                        print(fullpath + " file does not exist. Exit")
                        sys.exit(1)
                    else:
                        data = pd.read_csv(fullpath)
                        att = np.asarray(data[(data["BCID"].isin(bcidfilter))]["xsec"])
                        attErr = np.asarray(data[(data["BCID"].isin(bcidfilter))]["xsecErr"])

                        SigAllDict[corr][det][fill_name].append([att, attErr, bcidfilter])
                        SigDict[corr][fill_name][det].append([att, attErr, bcidfilter])
                        if len(attErr) == 0:
                            continue

                        sigData[corr][det][filedict[fill_name]] = np.around(float(np.average(att, weights=attErr)), decimals=3)
                        sigData[corr][det][filedict[fill_name] + "_STD"] = np.around(float(att.std()), decimals=4)
                        sigData[corr][det][filedict[fill_name] + "_#BCID"] = len(att)

    # CapSigma plots
    if CapRatio:
        print(("Capsigma ratio plots are in progress with reference detector as: %s" % refDet))
        for xy in ["X", "Y"]:
            ei = 0 if xy == "X" else 3
            for corr in sorted(list(corrs.keys()), key=len):
                # if corr not in corrs: continue
                for f in sorted(list(CapDict[corr].keys()), key=lambda name: int(name[0:4] + name[13:17])):#0:4
                    print(("\n=== Scan %s ===" % f))
                    print(("Correction: %s" % corr))
                    plt.figure()
                    vref = CapDict[corr][f][refDet]
                    cc = 0
                    xaxisList = []
                    CSV_DICT = {}
                    ntabmax = max((len("Average for %s/%s:" % (det, refDet)) // 8 for det in dets))
                    for det in dets:
                        if det == refDet:
                            cc += 1
                            continue
                        # Make graph for detector det
                        v = CapDict[corr][f][det]
                        diff = np.setdiff1d(v[0][2 + ei], vref[0][2 + ei])
                        diffref = np.setdiff1d(vref[0][2 + ei], v[0][2 + ei])
                        ind = [np.where(v[0][2 + ei] == a) for a in diff]
                        indref = [np.where(vref[0][2 + ei] == a) for a in diffref]
                        yaxis = np.true_divide(np.delete(v[0][0 + ei], ind), np.delete(vref[0][0 + ei], indref))
                        yerr = yaxis * np.sqrt(
                            (np.power(np.delete(v[0][1 + ei], ind) / np.delete(v[0][0 + ei], ind), 2))
                            + (np.power(np.delete(vref[0][1 + ei], indref) / np.delete(vref[0][0 + ei], indref), 2))
                        )
                        xaxis = np.delete(v[0][2 + ei], ind)
                        xaxisList.append(xaxis)

                        plt.errorbar(xaxis, yaxis, yerr=yerr, fmt="o", color=colorList[cc], label=det + "/" + refDet)
                        CSV_DICT.update({det + " X": xaxis, det + " Y": yaxis, det + " Y error": yerr})

                        # Print mean and standard dev
                        yMean = np.mean(yaxis)
                        ystd = np.std(yaxis)
                        ysem = sem(yaxis)
                        ntab = 1 + ntabmax - len("Average for %s/%s:" % (det, refDet)) // 8
                        tab = ntab * "\t"
                        ntab2 = 1 + ntabmax - len("Standard deviation is:") // 8
                        tab2 = ntab2 * "\t"
                        print(("Average for %s/%s:%s%.3f +- %.3f" % (det, refDet, tab, yMean, ysem)))
                        print(("Standard deviation   :%s%.3f" % (tab2, ystd)))
                        print(("SEM                  :%s%.3f" % (tab2, ysem)))
                        print(("Distance of mean to 1:%s%.1f SEM" % (tab2, (yMean - 1.0) / ysem)))

                        plt.plot([plt.xlim()[0], plt.xlim()[1]], [yMean, yMean], ls="--", color=colorList[cc])

                        cc += 1

                    if xlimBool:
                        plt.xlim(xlimMin, xlimMax)

                    # Make CMS preliminary, year, fill
                    ipt.makeCMSPreli(year=year, fill=fill, energy=energy)

                    # Set title, labels, ...
                    ipt.makeTitle(ipt.makeTitleText(scanName=filedict[f], var=r"Ratio of $\Sigma_{" + xy + "}$", corr=corrs[corr], fit=fit))
                    plt.grid()
                    plt.xlabel("Bunch ID")
                    plt.ylabel(r"Ratio of $\Sigma_{" + xy + "}$" + ", detectorX/" + refDet, fontsize=12)
                    plt.ticklabel_format(useOffset=False, axis="y")  # Prevent plt from adding offset to y axis
                    plt.legend(loc="upper right", fontsize=12)

                    # Save figure
                    figName = makeFigName(path=outputPlotsDirBase, fill=fillD, scanName=f, var="capsigma/ratio", corr=corrs[corr], fit=fit, xy=xy)
                    figName = figName + (suffix != "") * "_" + suffix
                    folderpath = ipt.getFolders(figName)
                    if not os.path.exists(folderpath):
                        os.makedirs(folderpath)
                    ipt.savefig(figName, figureExtension)

                    # Save csv file
                    if identicalXAxis(xaxisList):
                        csvName = makeCSVName(path=outputCsvDirBase, fill=fillD, scanName=f, var="capsigma/ratio", corr=corrs[corr], fit=fit, xy=xy)
                        csvName = csvName + (suffix != "") * "_" + suffix
                        folderpath = ipt.getFolders(csvName)
                        if not os.path.exists(folderpath):
                            os.makedirs(folderpath)
                        PLOT_DF = pd.DataFrame(data=CSV_DICT)
                        ipt.toCSV(PLOT_DF, csvName)

                    plt.close()

    if CapNormal:
        print("Capsigma normal plots are in progress!")
        for xy in ["X", "Y"]:
            ei = 0 if xy == "X" else 3
            for corr in sorted(list(corrs.keys()), key=len):
                print(("\n=== Corr %s ===" % corr))
                for f in sorted(list(CapDict[corr].keys()), key=lambda name: int(name[0:4] + name[13:17])):
                    print(("\n=== Scan %s ===" % f))
                    plt.figure()
                    cc = 0
                    xaxisList = []
                    CSV_DICT = {}
                    ntabmax = max((len("Average CapSigma %s for %s:" % (xy, det)) // 8 for det in dets))
                    for det in dets:
                        # Make graph for detector det

                        v = CapDict[corr][f][det]
                        yaxis = v[0][0 + ei]
                        yerr = v[0][1 + ei]
                        xaxis = v[0][2 + ei]
                        xaxisList.append(xaxis)

                        # Print mean and standard dev
                        yMean = yaxis.mean()
                        ystd = yaxis.std()
                        ysem = sem(yaxis)
                        ntab = 1 + ntabmax - len("Average CapSigma %s for %s:" % (xy, det)) // 8
                        tab = ntab * "\t"
                        ntab2 = 1 + ntabmax - len("Standard deviation is:") // 8
                        tab2 = ntab2 * "\t"
                        print(("Average CapSigma %s for %s:%s%.3f +- %.3f" % (xy, det, tab, yMean, ysem)))
                        print(("Standard deviation is:%s%.3f" % (tab2, ystd)))

                        plt.errorbar(xaxis, yaxis, yerr=yerr, fmt="o", color=colorList[cc], label=det)
                        CSV_DICT.update({det + " X": xaxis, det + " Y": yaxis, det + " Y error": yerr})
                        cc += 1

                    # Make CMS preliminary, year, fill
                    ipt.makeCMSPreli(year=year, fill=fill, energy=energy)

                    # Set title, labels, ...
                    ipt.makeTitle(ipt.makeTitleText(scanName=filedict[f], var=r"$\Sigma_{" + xy + "}$", corr=corrs[corr], fit=fit))
                    plt.grid()
                    plt.xlabel("Bunch ID")
                    plt.ylabel(r"$\Sigma_{" + xy + "}$ [mm]", fontsize=12)
                    plt.ticklabel_format(useOffset=False, axis="y")  # Prevent plt from adding offset to y axis
                    plt.legend(loc="best", fontsize=12)

                    # Save figure
                    figName = makeFigName(path=outputPlotsDirBase, fill=fillD, scanName=f, var="capsigma/normal", corr=corrs[corr], fit=fit, xy=xy)
                    figName = figName + (suffix != "") * "_" + suffix
                    folderpath = ipt.getFolders(figName)
                    if not os.path.exists(folderpath):
                        os.makedirs(folderpath)
                    ipt.savefig(figName, figureExtension)

                    # Save csv file
                    if identicalXAxis(xaxisList):
                        csvName = makeCSVName(path=outputCsvDirBase, fill=fillD, scanName=f, var="capsigma/normal", corr=corrs[corr], fit=fit, xy=xy)
                        csvName = csvName + (suffix != "") * "_" + suffix
                        folderpath = ipt.getFolders(csvName)
                        if not os.path.exists(folderpath):
                            os.makedirs(folderpath)
                        PLOT_DF = pd.DataFrame(data=CSV_DICT)
                        ipt.toCSV(PLOT_DF, csvName)

                    plt.close()

    if CapEffect:
        print("CapSigma effect plots are in progress!")
        for xy in ["X", "Y"]:
            ei = 0 if xy == "X" else 3
            for corr in sorted(list(corrs.keys()), key=len):
                if corr == "noCorr":
                    continue
                # Find reference correction
                corrRef = getRefCorr(corr, corrs)

                for f in sorted(list(CapDict[corr].keys()), key=lambda name: int(name[0:4] + name[13:17])):
                    print(("\n=== Scan %s ===" % f))
                    print(("corrRef: %s" % corrRef))
                    plt.figure()
                    cc = 0
                    xaxisList = []
                    CSV_DICT = {}
                    ntabmax = max((len("Average effect of %s for %s:" % (corr, det)) // 8 for det in dets))
                    for det in dets:
                        # Make graph for detector det
                        v = CapDict[corr][f][det]
                        vref = CapDict[corrRef][f][det]

                        # print det, corr, f
                        diff = np.setdiff1d(v[0][2 + ei], vref[0][2 + ei])
                        diffref = np.setdiff1d(vref[0][2 + ei], v[0][2 + ei])
                        ind = [np.where(v[0][2 + ei] == a) for a in diff]
                        indref = [np.where(vref[0][2 + ei] == a) for a in diffref]
                        try:
                            yaxis = np.true_divide(np.delete(v[0][0 + ei], ind), np.delete(vref[0][0 + ei], indref)) - 1
                        except:
                            continue
                        if len(yaxis) == 0:
                            continue
                        yerr = yaxis * np.sqrt(
                            (np.power(np.delete(v[0][1 + ei], ind) / np.delete(v[0][0 + ei], ind), 2))
                            + (np.power(np.delete(vref[0][1 + ei], indref) / np.delete(vref[0][0 + ei], indref), 2))
                        )

                        # Use % units:
                        yaxis *= 100
                        yerr *= 100
                        xaxis = np.delete(v[0][2 + ei], ind)
                        xaxisList.append(xaxis)

                        plt.errorbar(xaxis, yaxis, yerr=yerr, fmt="o", color=colorList[cc], label=det)

                        # Print mean and standard dev
                        yMean = yaxis.mean()
                        ystd = yaxis.std()
                        ysem = sem(yaxis)
                        ntab = 1 + ntabmax - len("Average effect of %s for %s:" % (corr, det)) // 8
                        tab = ntab * "\t"
                        ntab2 = 1 + ntabmax - len("Standard deviation is:") // 8
                        tab2 = ntab2 * "\t"
                        print(("Average effect of %s for %s:%s%.3f +- %.3f" % (corr, det, tab, yMean, ysem)))
                        print(("Standard deviation is:%s%.3f" % (tab2, ystd)))

                        plt.plot([plt.xlim()[0], plt.xlim()[1]], [yMean, yMean], ls="--", color=colorList[cc])

                        CSV_DICT.update({det + " X": xaxis, det + " Y": yaxis, det + " Y error": yerr})
                        cc += 1

                    # Make CMS preliminary, year, fill
                    ipt.makeCMSPreli(year=year, fill=fill, energy=energy)

                    # Set title, labels, ...
                    ipt.makeTitle(
                        ipt.makeTitleText(
                            scanName=filedict[f], var="Effect of " + corr.split("_")[len(corr.split("_")) - 1] + r" on $\Sigma_{" + xy + "}$", fit=fit
                        )
                    )

                    plt.grid()
                    plt.xlabel("Bunch ID")
                    plt.ylabel("Effect of " + corr.split("_")[len(corr.split("_")) - 1] + r" on $\Sigma_{" + xy + "}$ [%]", fontsize=12)
                    plt.ticklabel_format(useOffset=False, axis="y")  # Prevent plt from adding offset to y axis
                    plt.legend(loc="best", fontsize=12)

                    # Save figure
                    figName = makeFigName(path=outputPlotsDirBase, fill=fillD, scanName=f, var="capsigma/correffect", corr=corrs[corr], fit=fit, xy=xy)
                    figName = figName + (suffix != "") * "_" + suffix
                    folderpath = ipt.getFolders(figName)
                    if not os.path.exists(folderpath):
                        os.makedirs(folderpath)
                    ipt.savefig(figName, figureExtension)

                    # Save csv file
                    if identicalXAxis(xaxisList):
                        csvName = makeCSVName(path=outputCsvDirBase, fill=fillD, scanName=f, var="capsigma/correffect", corr=corrs[corr], fit=fit, xy=xy)
                        csvName = csvName + (suffix != "") * "_" + suffix
                        folderpath = ipt.getFolders(csvName)
                        if not os.path.exists(folderpath):
                            os.makedirs(folderpath)
                        PLOT_DF = pd.DataFrame(data=CSV_DICT)
                        ipt.toCSV(PLOT_DF, csvName)

                    plt.close()

    if CapEvol:
        print("CapSigma evolution plots are in progress!")
        for xy in ["X", "Y"]:
            ei = 0 if xy == "X" else 3
            for corr in sorted(list(corrs.keys()), key=len):
                plt.figure()
                cc = 0
                CSV_DICT = {}
                for det in dets:
                    # Make graph for detector det
                    y, e, x = ([] for i in range(3))
                    scanno = 1
                    for f in list(filedict.keys()):
                        if f in list(CapAllDict[corr][det].keys()):
                            v = CapAllDict[corr][det][f]
                            y.append(v[0][0 + ei].mean())
                            e.append(v[0][0 + ei].std())
                            x.append(scanno)
                            scanno += 1

                    plt.errorbar(x, y, yerr=e, fmt="o", color=colorList[cc], label=det)
                    CSV_DICT.update({det + " X": x, det + " Y": y, det + " Y error": e})
                    cc += 1

                # Make CMS preliminary, year, fill
                ipt.makeCMSPreli(year=year, fill=fill, energy=energy)

                # Set title, labels, ...
                ipt.makeTitle(ipt.makeTitleText(scanName="", var=r"$\Sigma_{" + xy + "}$", corr=corrs[corr], fit=fit))

                plt.xlim(0, len(list(filedict.keys())) + 1)
                plt.grid()
                labels = []
                for key in list(filedict.keys()):
                    if key in list(CapAllDict[corr][det].keys()):
                        labels.append(filedict[key])
                plt.xticks(x, labels, rotation=0, fontsize=10)
                plt.xlabel("Scan Names")
                plt.ylabel(r"$\Sigma_{" + xy + "}$ [mm]", fontsize=12)
                plt.ticklabel_format(useOffset=False, axis="y")  # Prevent plt from adding offset to y axis
                plt.legend(loc="best", fontsize=12)

                # Save figure
                figName = makeFigName(path=outputPlotsDirBase, fill=fillD, scanName="", var="capsigma/evol", corr=corrs[corr], fit=fit, xy=xy)
                figName = figName + (suffix != "") * "_" + suffix
                folderpath = ipt.getFolders(figName)
                if not os.path.exists(folderpath):
                    os.makedirs(folderpath)
                ipt.savefig(figName, figureExtension)
                os.system("cp index.php %s" % folderpath)

                # Save csv file
                csvName = makeCSVName(path=outputCsvDirBase, fill=fillD, scanName="", var="capsigma/evol", corr=corrs[corr], fit=fit, xy=xy)
                csvName = csvName + (suffix != "") * "_" + suffix
                folderpath = ipt.getFolders(csvName)
                if not os.path.exists(folderpath):
                    os.makedirs(folderpath)
                PLOT_DF = pd.DataFrame(data=CSV_DICT)
                ipt.toCSV(PLOT_DF, csvName)

                plt.close()

    ##   Peak plots
    if peakNormal:
        print("peak normal plots are in progress!")
        for xy in ["X", "Y"]:
            ei = 0 if xy == "X" else 3
            for corr in sorted(list(corrs.keys()), key=len):
                for f in sorted(list(peakDict[corr].keys()), key=lambda name: int(name[0:4] + name[13:17])):
                    plt.figure()
                    cc = 0
                    xaxisList = []
                    CSV_DICT = {}
                    for det in dets:
                        # Make graph for detector det

                        v = peakDict[corr][f][det]
                        yaxis = v[0][0 + ei]
                        yerr = v[0][1 + ei]
                        xaxis = v[0][2 + ei]
                        xaxisList.append(xaxis)

                        plt.errorbar(xaxis, yaxis, yerr=yerr, fmt="o", color=colorList[cc], label=det)
                        CSV_DICT.update({det + " X": xaxis, det + " Y": yaxis, det + " Y error": yerr})
                        cc += 1

                    # Make CMS preliminary, year, fill
                    ipt.makeCMSPreli(year=year, fill=fill, energy=energy)

                    # Set title, labels, ...
                    ipt.makeTitle(ipt.makeTitleText(scanName=filedict[f], var="peak$_{" + xy + "}$", corr=corrs[corr], fit=fit))
                    plt.grid()
                    plt.xlabel("Bunch ID")
                    plt.ylabel("peak$_{" + xy + "}$", fontsize=12)
                    plt.ticklabel_format(useOffset=False, axis="y")  # Prevent plt from adding offset to y axis
                    plt.legend(loc="best", fontsize=12)

                    # Save figure
                    figName = makeFigName(path=outputPlotsDirBase, fill=fillD, scanName=f, var="peak/normal", corr=corrs[corr], fit=fit, xy=xy)
                    figName = figName + (suffix != "") * "_" + suffix
                    folderpath = ipt.getFolders(figName)
                    if not os.path.exists(folderpath):
                        os.makedirs(folderpath)
                    ipt.savefig(figName, figureExtension)

                    # Save csv file
                    if identicalXAxis(xaxisList):
                        csvName = makeCSVName(path=outputCsvDirBase, fill=fillD, scanName=f, var="peak/normal", corr=corrs[corr], fit=fit, xy=xy)
                        csvName = csvName + (suffix != "") * "_" + suffix
                        folderpath = ipt.getFolders(csvName)
                        if not os.path.exists(folderpath):
                            os.makedirs(folderpath)
                        PLOT_DF = pd.DataFrame(data=CSV_DICT)
                        ipt.toCSV(PLOT_DF, csvName)

                    plt.close()

        if peakEffect:
            print("peak effect plots are in progress!")
            for xy in ["X", "Y"]:
                ei = 0 if xy == "X" else 3
                for corr in sorted(list(corrs.keys()), key=len):
                    if corr == "noCorr":
                        continue
                    # Find reference correction
                    corrRef = getRefCorr(corr, corrs)

                    for f in sorted(list(peakDict[corr].keys()), key=lambda name: int(name[0:4] + name[13:17])):
                        print(("\n=== Scan %s ===" % f))
                        print(("corrRef: %s" % corrRef))
                        plt.figure()
                        cc = 0
                        xaxisList = []
                        CSV_DICT = {}
                        ntabmax = max((len("Average effect of %s for %s:" % (corr, det)) // 8 for det in dets))
                        for det in dets:
                            # Make graph for detector det
                            v = peakDict[corr][f][det]
                            vref = peakDict[corrRef][f][det]

                            diff = np.setdiff1d(v[0][2 + ei], vref[0][2 + ei])
                            diffref = np.setdiff1d(vref[0][2 + ei], v[0][2 + ei])
                            ind = [np.where(v[0][2 + ei] == a) for a in diff]
                            indref = [np.where(vref[0][2 + ei] == a) for a in diffref]
                            try:
                                yaxis = np.true_divide(np.delete(v[0][0 + ei], ind), np.delete(vref[0][0 + ei], indref)) - 1
                            except:
                                continue
                            if len(yaxis) == 0:
                                continue
                            yerr = yaxis * np.sqrt(
                                (np.power(np.delete(v[0][1 + ei], ind) / np.delete(v[0][0 + ei], ind), 2))
                                + (np.power(np.delete(vref[0][1 + ei], indref) / np.delete(vref[0][0 + ei], indref), 2))
                            )

                            # Use % units:
                            yaxis *= 100
                            yerr *= 100
                            xaxis = np.delete(v[0][2 + ei], ind)
                            xaxisList.append(xaxis)

                            plt.errorbar(xaxis, yaxis, yerr=yerr, fmt="o", color=colorList[cc], label=det)

                            # Print mean and standard dev
                            yMean = yaxis.mean()
                            ystd = yaxis.std()
                            ysem = sem(yaxis)
                            ntab = 1 + ntabmax - len("Average effect of %s for %s:" % (corr, det)) // 8
                            tab = ntab * "\t"
                            ntab2 = 1 + ntabmax - len("Standard deviation is:") // 8
                            tab2 = ntab2 * "\t"
                            print(("Average effect of %s for %s:%s%.3f +- %.3f" % (corr, det, tab, yMean, ysem)))
                            print(("Standard deviation is:%s%.3f" % (tab2, ystd)))

                            plt.plot([plt.xlim()[0], plt.xlim()[1]], [yMean, yMean], ls="--", color=colorList[cc])

                            CSV_DICT.update({det + " X": xaxis, det + " Y": yaxis, det + " Y error": yerr})
                            cc += 1

                        # Make CMS preliminary, year, fill
                        ipt.makeCMSPreli(year=year, fill=fill, energy=energy)

                        # Set title, labels, ...
                        ipt.makeTitle(
                            ipt.makeTitleText(
                                scanName=filedict[f], var="Effect of " + corr.split("_")[len(corr.split("_")) - 1] + " on peak$_{" + xy + "}$", fit=fit
                            )
                        )

                        plt.grid()
                        plt.xlabel("Bunch ID")
                        plt.ylabel("Effect of " + corr.split("_")[len(corr.split("_")) - 1] + " on peak$_{" + xy + "}$ [%]", fontsize=12)
                        plt.ticklabel_format(useOffset=False, axis="y")  # Prevent plt from adding offset to y axis
                        plt.legend(loc="best", fontsize=12)

                        # Save figure
                        figName = makeFigName(path=outputPlotsDirBase, fill=fillD, scanName=f, var="peak/correffect", corr=corrs[corr], fit=fit, xy=xy)
                        figName = figName + (suffix != "") * "_" + suffix
                        folderpath = ipt.getFolders(figName)
                        if not os.path.exists(folderpath):
                            os.makedirs(folderpath)
                        ipt.savefig(figName, figureExtension)

                        # Save csv file
                        if identicalXAxis(xaxisList):
                            csvName = makeCSVName(path=outputCsvDirBase, fill=fillD, scanName=f, var="peak/correffect", corr=corrs[corr], fit=fit, xy=xy)
                            csvName = csvName + (suffix != "") * "_" + suffix
                            folderpath = ipt.getFolders(csvName)
                            if not os.path.exists(folderpath):
                                os.makedirs(folderpath)
                            PLOT_DF = pd.DataFrame(data=CSV_DICT)
                            ipt.toCSV(PLOT_DF, csvName)

                        plt.close()

        ##   Sigma visible plots
        if SigEffect:
            print("SigmaVis effect plots are in progress!")
            for corr in sorted(list(corrs.keys()), key=len):
                if corr == "noCorr":
                    continue
                # Find reference correction
                corrRef = getRefCorr(corr, corrs)

                for f in sorted(list(SigDict[corr].keys()), key=lambda name: int(name[0:4] + name[13:17])):
                    print(("\n=== Scan %s ===" % f))
                    print(("corrRef: %s" % corrRef))
                    plt.figure()
                    cc = 0
                    xaxisList = []
                    CSV_DICT = {}
                    ntabmax = max((len("Average effect of %s for %s:" % (corr, det)) // 8 for det in dets))
                    for det in dets:
                        # Make graph for detector det
                        v = SigDict[corr][f][det]
                        vref = SigDict[corrRef][f][det]

                        # print det, corr, f
                        diff = np.setdiff1d(v[0][2], vref[0][2])
                        diffref = np.setdiff1d(vref[0][2], v[0][2])
                        ind = [np.where(v[0][2] == a) for a in diff]
                        indref = [np.where(vref[0][2] == a) for a in diffref]
                        yaxis = np.true_divide(np.delete(v[0][0], ind), np.delete(vref[0][0], indref)) - 1
                        if len(yaxis) == 0:
                            continue
                        yerr = yaxis * np.sqrt(
                            (np.power(np.delete(v[0][1], ind) / np.delete(v[0][0], ind), 2))
                            + (np.power(np.delete(vref[0][1], indref) / np.delete(vref[0][0], indref), 2))
                        )

                        # Use % units:
                        yaxis *= 100
                        yerr *= 100
                        xaxis = np.delete(v[0][2], ind)
                        xaxisList.append(xaxis)

                        plt.errorbar(xaxis, yaxis, yerr=yerr, fmt="o", color=colorList[cc], label=det)
                        CSV_DICT.update({det + " X": xaxis, det + " Y": yaxis, det + " Y error": yerr})

                        # Print mean and standard dev
                        yMean = np.mean(yaxis)
                        ystd = np.std(yaxis)
                        ysem = sem(yaxis)
                        ntab = 1 + ntabmax - len("Average effect of %s for %s:" % (corr, det)) // 8
                        tab = ntab * "\t"
                        ntab2 = 1 + ntabmax - len("Standard deviation is:") // 8
                        tab2 = ntab2 * "\t"
                        print(("Average effect of %s for %s:%s%.3f +- %.3f" % (corr, det, tab, yMean, ysem)))
                        print(("Standard deviation is:%s%.3f" % (tab2, ystd)))

                        plt.plot([plt.xlim()[0], plt.xlim()[1]], [yMean, yMean], ls="--", color=colorList[cc])

                        cc += 1

                    # Make CMS preliminary, year, fill
                    ipt.makeCMSPreli(year=year, fill=fill, energy=energy)

                    # Set title, labels, ...
                    ipt.makeTitle(
                        ipt.makeTitleText(scanName=filedict[f], var="Effect of " + corr.split("_")[len(corr.split("_")) - 1] + " on $\sigma_{vis}$", fit=fit)
                    )

                    plt.grid()
                    plt.xlabel("Bunch ID")
                    plt.ylabel("Effect of " + corr.split("_")[len(corr.split("_")) - 1] + " on $\sigma_{vis}$ [%]", fontsize=12)
                    plt.ticklabel_format(useOffset=False, axis="y")  # Prevent plt from adding offset to y axis
                    plt.legend(loc="best", fontsize=12)

                    # Save figure
                    figName = makeFigName(path=outputPlotsDirBase, fill=fillD, scanName=f, var="sigmavis/correffect", corr=corrs[corr], fit=fit)
                    figName = figName + (suffix != "") * "_" + suffix
                    folderpath = ipt.getFolders(figName)
                    if not os.path.exists(folderpath):
                        os.makedirs(folderpath)
                    ipt.savefig(figName, figureExtension)

                    # Save csv file
                    if identicalXAxis(xaxisList):
                        csvName = makeCSVName(path=outputCsvDirBase, fill=fillD, scanName=f, var="sigmavis/correffect", corr=corrs[corr], fit=fit)
                        csvName = csvName + (suffix != "") * "_" + suffix
                        folderpath = ipt.getFolders(csvName)
                        if not os.path.exists(folderpath):
                            os.makedirs(folderpath)
                        PLOT_DF = pd.DataFrame(data=CSV_DICT)
                        ipt.toCSV(PLOT_DF, csvName)

                    plt.close()

        if SigNormal:
            print("SigmaVis normal plots are in progress!")

            for corr in sorted(list(corrs.keys()), key=len):
                if corr[-len(XY_CORR) :] == XY_CORR:
                    corrRead = corr.replace("_" + XY_CORR, "")
                else:
                    corrRead = corr
                print(("\n=== Corr %s ===" % corr))

                scanList = sorted(list(SigDict[corrRead].keys()), key=lambda name: int(name[5:7] + name[13:17]))

                if makeLatexTable:
                    bx2bxDict = {}
                    for det in dets:
                        bx2bxDict[det] = {}
                        for f in scanList:
                            bx2bxDict[f] = 0.0
                data_scans={}
                for f in scanList:
                    print(("\n=== Scan %s ===" % f))
                    plt.figure()
                    cc = 0
                    xaxisList = []
                    CSV_DICT = {}
                    ntabmax = max((len("Average sigmaVis for %s:" % (det)) // 8 for det in dets))
                    ntabmax3 = max((len("bx-to-bx syst. unc. %s [%%]:" % (det)) // 8 for det in dets))
                    for det in dets:
                        ## Make graph for detector det

                        ## Get per bunch CapSigma, Peak, Mean and reduced chi2 values
                        xaxis = CapAllDict[corrRead][det][f][0][2]
                        CX = CapAllDict[corrRead][det][f][0][0] * 1000
                        CXErr = CapAllDict[corrRead][det][f][0][1] * 1000
                        CY = CapAllDict[corrRead][det][f][0][3] * 1000
                        CYErr = CapAllDict[corrRead][det][f][0][4] * 1000
                        PX = peakAllDict[corrRead][det][f][0][0]
                        PXErr = peakAllDict[corrRead][det][f][0][1]
                        PY = peakAllDict[corrRead][det][f][0][3]
                        PYErr = peakAllDict[corrRead][det][f][0][4]
                        MX = MeanAllDict[corrRead][det][f][0][0] * 1000
                        MXErr = MeanAllDict[corrRead][det][f][0][1] * 1000
                        MY = MeanAllDict[corrRead][det][f][0][3] * 1000
                        MYErr = MeanAllDict[corrRead][det][f][0][4] * 1000
                        rchi2X = rchi2AllDict[corrRead][det][f][0][0]
                        rchi2Y = rchi2AllDict[corrRead][det][f][0][1]
                        nbcid = len(xaxis)

                        if chi2Scaling:
                            CXErr = np.array([CXErr[i] * max(1.0, np.sqrt(rchi2X[i])) for i in range(nbcid)])
                            PXErr = np.array([PXErr[i] * max(1.0, np.sqrt(rchi2X[i])) for i in range(nbcid)])
                            MXErr = np.array([MXErr[i] * max(1.0, np.sqrt(rchi2X[i])) for i in range(nbcid)])
                            CYErr = np.array([CYErr[i] * max(1.0, np.sqrt(rchi2Y[i])) for i in range(nbcid)])
                            PYErr = np.array([PYErr[i] * max(1.0, np.sqrt(rchi2Y[i])) for i in range(nbcid)])
                            MYErr = np.array([MYErr[i] * max(1.0, np.sqrt(rchi2Y[i])) for i in range(nbcid)])

                        if peak2peakCorr:
                            ## Get average MX and MY
                            if peak2peak == "beam":
                                avgMX = np.average(MX, weights=1.0 / (MXErr ** 2))
                                avgMY = np.average(MY, weights=1.0 / (MYErr ** 2))

                            ## Compute peak, peakErr and sigVis
                            P = []
                            PErr = []
                            for ibcid in range(nbcid):
                                if peak2peak == "perBX":
                                    DX = normpdf(0.0, 0.0, CX[ibcid]) / normpdf(MX[ibcid], 0.0, CX[ibcid])
                                    DY = normpdf(0.0, 0.0, CY[ibcid]) / normpdf(MY[ibcid], 0.0, CY[ibcid])
                                elif peak2peak == "beam":
                                    DX = normpdf(0.0, 0.0, CX[ibcid]) / normpdf(avgMX, 0.0, CX[ibcid])
                                    DY = normpdf(0.0, 0.0, CY[ibcid]) / normpdf(avgMY, 0.0, CY[ibcid])
                                P.append((DY * PX[ibcid] + DX * PY[ibcid]) / 2.0)
                                PErr.append(np.sqrt((DY * PXErr[ibcid]) ** 2 + (DX * PYErr[ibcid]) ** 2))
                            P = np.array(P)
                            PErr = np.array(PErr)
                            sigVis = 2.0 * np.pi * CX * CY * P
                            sigVisErr = 2.0 * np.pi * np.sqrt((CY * P * CXErr) ** 2 + (CX * P * CYErr) ** 2 + (CX * CY * PErr) ** 2)

                        else:
                            ## Compute sigVis
                            print(CX.shape,CY.shape)
                            sigVis = np.pi * CX * CY * (PX + PY)/1000  ### <-- XY Corr 
                            sigVisErr = np.pi * np.sqrt(
                                (CY * (PX + PY) * CXErr) ** 2 + (CX * (PX + PY) * CYErr) ** 2 + (CX * CY * PXErr) ** 2 + (CX * CY * PYErr) ** 2)/1000 ### <-- XY Corr
                         #   print("aqui:",sigVis)
                        ## Apply xy correction
                        if corr[-len(XY_CORR) :] == XY_CORR:
                            sigVis *= 1.0 + 0.01 * xyCorr[f]
                            sigVisErr *= 1.0 + 0.01 * xyCorr[f]

                        ## Fit a constant to get sigma vis for the given scan
                        yMean = np.average(sigVis, weights=1.0 / (sigVisErr ** 2))
                        yMeanErr = 1.0 / np.sqrt(sum(1.0 / sigVisErr ** 2))
                        data_scans[f] = {'sigVis': sigVis, 'sigVisErr': sigVisErr}
                        print("here",sigVis,sigVisErr) 
                        ystd = sigVis.std()
                        ysem = sem(sigVis)
                        rchi2 = sum(((sigVis - yMean) / sigVisErr) ** 2) / (len(sigVis) - 1)
                        if ysem > yMeanErr:
                            bx2bxSyst = 100 * np.sqrt((ysem / yMean) ** 2 - (yMeanErr / yMean) ** 2)
                        else:
                            bx2bxSyst = 0.0
                       # print("if ysem > yMeanErr:\n bx2bxSyst = 100 * np.sqrt((ysem / yMean) ** 2 - (yMeanErr / yMean) ** 2)\n else:\n bx2bxSyst = 0.0" )  
                       # print("mean:"+str(yMean),"meanErr:"+str(yMeanErr),"std:"+str(ystd),"SEM:"+str(ysem))
                        ## Print mean and standard dev
                        ntab = 1 + ntabmax3 - len("Average sigmaVis for %s:" % (det)) // 8
                        tab = ntab * "\t"
                        ntab2 = 1 + ntabmax3 - len("Standard deviation:") // 8
                        tab2 = ntab2 * "\t"
                        ntab3 = 1 + ntabmax3 - len("bx-to-bx syst. unc. %s [%%]:" % det) // 8
                        tab3 = ntab3 * "\t"
                        print("Average sigmaVis of %s:%s%.2f +- %.2f" % (det, tab, yMean, yMeanErr))
                        print("Standard deviation:%s%.2f" % (tab2, ystd))
                        print("Reduced chisquare :%s%.2f" % (tab2, rchi2))
                        # print("std / avg  %s:         %s %.3f" %(det, tab, ystd / yMean))
                        print("bx-to-bx syst. unc. %s [%%]:%s%.2f" % (det, tab3, bx2bxSyst))
                        if makeLatexTable:
                            bx2bxDict[det][f] = bx2bxSyst

                        plt.errorbar(xaxis, sigVis, yerr=sigVisErr, fmt="o", color=colorList[cc], label=det)
                        # CSV_DICT.update({det + ' X':xaxis, det + ' Y':sigVis, det + ' Y error':sigVisErr})

                        ## Plot mean and error bars
                        plt.plot([plt.xlim()[0], plt.xlim()[1]], [yMean, yMean], ls="--", color=colorList[cc])
                        # plt.fill_between( [plt.xlim()[0],plt.xlim()[1]], y1=[yMean-ysem,yMean-ysem], y2=[yMean+ysem,yMean+ysem], color=colorList[cc], alpha=0.5)
                        plt.fill_between(
                            [plt.xlim()[0], plt.xlim()[1]], y1=[yMean - ystd, yMean - ystd], y2=[yMean + ystd, yMean + ystd], color=colorList[cc], alpha=0.5
                        )

                        cc += 1

                    if xlimBool:
                        plt.xlim(xlimMin, xlimMax)

                    # Make CMS preliminary, year, fill
                    ipt.makeCMSPreli(year=year, fill=fill, energy=energy)

                    # Set title, labels, ...
                    ipt.makeTitle(ipt.makeTitleText(scanName=filedict[f], var="$\sigma_{vis}$", corr=corrs[corr], fit=fit))

                    plt.grid()
                    plt.xlabel("Bunch ID")
                    if det == "PCC":
                        plt.ylabel("$\sigma_{vis}$ [$b$]", fontsize=14)
                    else:
                        plt.ylabel("$\sigma_{vis}$ [$\mu b$]", fontsize=14)
                    plt.ticklabel_format(useOffset=False, axis="y")  # Prevent plt from adding offset to y axis
                    plt.legend(loc="best", fontsize=12)

                    # Save figure
                    figName = makeFigName(path=outputPlotsDirBase, fill=fillD, scanName=f, var="sigmavis/normal", corr=corrs[corr], fit=fit)
                    figName = figName + (suffix != "") * "_" + suffix
                    folderpath = ipt.getFolders(figName)
                    if not os.path.exists(folderpath):
                        os.makedirs(folderpath)
                    ipt.savefig(figName, figureExtension)

                    # Save csv file
                    if identicalXAxis(xaxisList):
                        csvName = makeCSVName(path=outputCsvDirBase, fill=fillD, scanName=f, var="sigmavis/normal", corr=corrs[corr], fit=fit)
                        csvName = csvName + (suffix != "") * "_" + suffix
                        folderpath = ipt.getFolders(csvName)
                        if not os.path.exists(folderpath):
                            os.makedirs(folderpath)
                        PLOT_DF = pd.DataFrame(data=CSV_DICT)
                        ipt.toCSV(PLOT_DF, csvName)

                    plt.close()
#################################################################################################

#                print(data_scans)
            #    plt.figure()                                                                                                                                
                plt.figure(figsize=(10, 6))    
                # Colores para los puntos de cada scan                                                                                                          
               # if fill == "8381":
                colors = ['red', 'blue', 'green', 'orange', 'purple','yellow','black','brown','gray']                                                         
               # bx =["BCID 265","BCID 865","BCID 1780","BCID 2192","BCID 3380"]   #2018
               # bx =["BCID 282","BCID 822","BCID 2944","BCID 3123","BCID 3302"]   #2022      
                bx =["BCID 208","BCID 282","BCID 548","BCID 822","BCID 1197","BCID 1376","BCID 2716","BCID 2944","BCID 3123"]   #2023                        
                # Posiciones x de los puntos de cada scan                                                                                        
              #  x_positions = np.arange(1.0, 6.0)                                                                                                                
                scans = 8
                posi = {str(i): np.arange(i - 0.32, i + 0.4, 0.08) for i in range(1,scans+1)}
                print(posi)
               # posi = {str(i): np.arange(i - 0.2, i + 0.3, 0.1) for i in range(1,scans+1)}
               # posi  = {"1":np.arange(0.8,1.3,.1),             
                #         "2":np.arange(1.8,2.2,.1),                                                                          
                 #        "3":np.arange(2.8,3.2,.1),                                                                           
                  #       "4":np.arange(3.8,4.2,.1),                                                                          
                   #      "5":np.arange(4.8,5.2,.1)  }                                                                                                     
                sv = np.array([])                                                                         
                sverr=np.array([])                                                                                        
                for scan_name, scan_data,h in zip(data_scans.keys(),data_scans.values(),np.arange(1,scans+1)):                       
                    sigVis = scan_data['sigVis']                                                                                 
                    sigVisErr = scan_data['sigVisErr']                                                            
                    # Trazar los puntos del scan actual en las posiciones x correspondientes              
                    for x,y,z,c,b in zip(posi[str(h)],sigVis,sigVisErr,colors,bx):                                
                        sv=np.append(sv,y)                                                                                     
                        sverr=np.append(sverr,z)                                                                       #
                        if scan_name=="8381_11Nov22_004152_11Nov22_010424" or scan_name=="6868_30Jun18_132834_30Jun18_135235" or scan_name=="8999_28Jun23_000143_28Jun23_232943":
                            print("x,y",x,y)                                        
                            plt.errorbar(x, y, yerr=z, fmt='o',label=b, color=c)                
                        else:                  
                            print("x,y",x,y)
                            plt.errorbar(x, y, yerr=z, fmt='o', color=c)  
                            
                print("sv,sverr",sv,sverr)                                                                                                
                yMean_z =  np.average(sv, weights=1./(sverr**2))                          
                yzstd=sv.std(ddof=1)                                                                                         
                plt.plot( [plt.xlim()[0],plt.xlim()[1]], [yMean_z,yMean_z], ls='-', color=colorList[cc])  
                plt.fill_between( [plt.xlim()[0],plt.xlim()[1]], y1=[yMean_z-yzstd,yMean_z-yzstd], y2=[yMean_z+yzstd,yMean_z+yzstd], color="gray",alpha=0.5)
               # labels = ["vdM1","BI1","BI2","BI3","vdM2","vdM3","vdM4"] #2018
               # labels = ["vdM1","BI1","BI2","vdM3","vdM4"] #2022
                labels = ["vdM1","BI1","BI2","vdM2","vdM3","vdM4","vdM5","vdM6"] #2023
                xaxis = [ i+1 for i in range(len(labels)) ]       
                plt.xticks(xaxis, labels)
               # plt.legend(loc='lower right', bbox_to_anchor=(0.7, 0.3), fontsize=8)
                if fill  == "8381":
                    plt.figtext(.9, .91, "Fill 8381 (2022, 13.6 TeV)", fontsize=13, style="normal", backgroundcolor="white", ha="right").set_bbox(dict(alpha=0.0))
                    plt.figtext(.15, 0.84, "CMS", fontsize=15, fontweight="bold", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))                                                                                                          
                    plt.figtext(.15 +0.07, 0.84, "Preliminary ", fontsize=13, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))                                                                                               
                    plt.figtext(.15 , 0.80, "PCC", fontsize=13, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))          
                   # ipt.makeCMSPreli(year="Fill    2022 ",fill=fill,energy=energy + " TeV")
                    plt.ylim(1055,1095)
                elif fill  == "8999":
                    plt.figtext(.9, .91, "Fill 8999 (2023, 13.6 TeV)", fontsize=13, style="normal", backgroundcolor="white", ha="right").set_bbox(dict(alpha=0.0))                                                                                                        
                    plt.figtext(.15, 0.84, "CMS", fontsize=15, fontweight="bold", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))                                                                                                                  
                    plt.figtext(.15 +0.07, 0.84, "Preliminary ", fontsize=13, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))                                                
                    plt.figtext(.15 , 0.80, "PCC", fontsize=13, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))    
                   # ipt.makeCMSPreli(year="Fill 6868   2018 ",fill=fill,energy=energy + " TeV")
                    plt.ylim(1250,1315)
                plt.ylabel('$\sigma_{vis}$ [$mb$]', fontsize=18)                                              
   
                plt.ticklabel_format(useOffset=False, axis='y')  # Prevent plt from adding offset to y axis                
                plt.legend(loc='lower right', fontsize=10)                    
               # plt.ylim(1055,1095)                      
                plt.xlim(0.5,scans +0.5)                                          
               # plt.legend(fontsize=10)
               # plt.legend(loc='upper right', bbox_to_anchor=(1.0, 0.5), fontsize=10) 
               # plt.xlabel('Scan',fontsize=18)                                                                            #                   
                plt.ylabel('$\sigma_{vis}$ [$mb$]', fontsize=19)                                               
                plt.grid(True)                                                                             
                plt.savefig('/eos/user/l/lcuevasp/2023_Analisys/output_PCC/all_scans_BCID.pdf') 
               # print("yMean_z",yMean_z)
                
#############################################################################################################

                ## Make latex table        
                if makeLatexTable:                                                                                                                                           
                    print("")                                    
                    table = "\hline\n"                             
                    table = table + "            & \multicolumn{5}{c|}{Bunch-to-bunch systematic uncertainty [\%]}  \\\\\n"           
                    table = table + "\hline\n"          
                    table = table + "Luminometer"                               
                    for idx, f in enumerate(scanList):                
                        table = table + " & ~vdM" + str(idx + 1) + "~"                   
                    table = table + " \\\\\n"                                     
                    table = table + "\hline\n"                                                                                                                               
                                                                                                                                                                             
                    for det in dets:                                             
                        table = table + det + (11 - len(det)) * " "                                                                                                          
                        for f in scanList:                                                                                                                                   
                            table = table + " & " + ("%.2f" % bx2bxDict[det][f])                                                                                             
                        table = table + " \\\\\n"                                                                                                                            
                    table = table + "\hline\n"                                                                                                                               
                                                                                                                                                                             
                    print(table) 


        if SigAll:
            print("SigmaVis all corrections plots are in progress!")

            sigVisDict = {}
            if makeLatexTable:
                for det in dets:
                    sigVisDict[det] = {}
                    for corr in sorted(list(corrs.keys()), key=len):
                        sigVisDict[det][corrs[corr][-2:]] = [0.0, 0.0]

            for corr in sorted(list(corrs.keys()), key=len):
                print(("\n\n=== %s ===" % corr))
                if corr[-len(XY_CORR) :] == XY_CORR:
                    corrRead = corr.replace("_" + XY_CORR, "")
                else:
                    corrRead = corr

                for det in dets:
                    print("")
                    sigmaVisPerScan = []
                    sigmaVisErrPerScan = []
                    rmsPerScan = []
                    scanList = sorted(list(SigAllDict[corrRead][det].keys()), key=lambda name: int(name[5:7] + name[13:17]))
                    labels = [filedict[key] for key in scanList]
                    xaxis = [i + 1 for i in range(len(labels))]

                    for f in scanList:
                        ## Get per bunch CapSigma, Peak, Mean and reduced chi2 values
                        CX = CapAllDict[corrRead][det][f][0][0] * 1000
                        CXErr = CapAllDict[corrRead][det][f][0][1] * 1000
                        CY = CapAllDict[corrRead][det][f][0][3] * 1000
                        CYErr = CapAllDict[corrRead][det][f][0][4] * 1000
                        PX = peakAllDict[corrRead][det][f][0][0]
                        PXErr = peakAllDict[corrRead][det][f][0][1]
                        PY = peakAllDict[corrRead][det][f][0][3]
                        PYErr = peakAllDict[corrRead][det][f][0][4]
                        MX = MeanAllDict[corrRead][det][f][0][0] * 1000
                        MXErr = MeanAllDict[corrRead][det][f][0][1] * 1000
                        MY = MeanAllDict[corrRead][det][f][0][3] * 1000
                        MYErr = MeanAllDict[corrRead][det][f][0][4] * 1000
                        rchi2X = rchi2AllDict[corrRead][det][f][0][0]
                        rchi2Y = rchi2AllDict[corrRead][det][f][0][1]
                        nbcid = len(CX)

                        if chi2Scaling:
                            CXErr = np.array([CXErr[i] * max(1.0, np.sqrt(rchi2X[i])) for i in range(nbcid)])
                            PXErr = np.array([PXErr[i] * max(1.0, np.sqrt(rchi2X[i])) for i in range(nbcid)])
                            MXErr = np.array([MXErr[i] * max(1.0, np.sqrt(rchi2X[i])) for i in range(nbcid)])
                            CYErr = np.array([CYErr[i] * max(1.0, np.sqrt(rchi2Y[i])) for i in range(nbcid)])
                            PYErr = np.array([PYErr[i] * max(1.0, np.sqrt(rchi2Y[i])) for i in range(nbcid)])
                            MYErr = np.array([MYErr[i] * max(1.0, np.sqrt(rchi2Y[i])) for i in range(nbcid)])

                        if peak2peakCorr:
                            ## Get average MX and MY
                            if peak2peak == "beam":
                                avgMX = np.average(MX, weights=1.0 / (MXErr ** 2))
                                avgMY = np.average(MY, weights=1.0 / (MYErr ** 2))

                            ## Compute peak, peakErr and sigVis
                            P = []
                            PErr = []
                            for ibcid in range(nbcid):
                                if peak2peak == "perBX":
                                    DX = normpdf(0.0, 0.0, CX[ibcid]) / normpdf(MX[ibcid], 0.0, CX[ibcid])
                                    DY = normpdf(0.0, 0.0, CY[ibcid]) / normpdf(MY[ibcid], 0.0, CY[ibcid])
                                elif peak2peak == "beam":
                                    DX = normpdf(0.0, 0.0, CX[ibcid]) / normpdf(avgMX, 0.0, CX[ibcid])
                                    DY = normpdf(0.0, 0.0, CY[ibcid]) / normpdf(avgMY, 0.0, CY[ibcid])
                                P.append((DY * PX[ibcid] + DX * PY[ibcid]) / 2.0)
                                PErr.append(np.sqrt((DY * PXErr[ibcid]) ** 2 + (DX * PYErr[ibcid]) ** 2))
                            P = np.array(P)
                            PErr = np.array(PErr)
                            sigVis = 2.0 * np.pi * CX * CY * P
                            sigVisErr = 2.0 * np.pi * np.sqrt((CY * P * CXErr) ** 2 + (CX * P * CYErr) ** 2 + (CX * CY * PErr) ** 2)

                        else:
                            ## Compute sigVis
                            sigVis = np.pi * CX * CY * (PX + PY)#/1000
                            sigVisErr = np.pi * np.sqrt(
                                (CY * (PX + PY) * CXErr) ** 2 + (CX * (PX + PY) * CYErr) ** 2 + (CX * CY * PXErr) ** 2 + (CX * CY * PYErr) ** 2)#/1000
                        
                        ## Fit a constant to get sigma vis for the given scan
                        sigmaVisPerScan.append(np.average(sigVis, weights=1.0 / (sigVisErr ** 2)))
                        sigmaVisErrPerScan.append(1.0 / np.sqrt(sum(1.0 / sigVisErr ** 2)))
                       # rmsPerScan.append(np.sqrt(np.mean(np.square(sigVis))))
                        rmsPerScan.append(np.std(sigVis,ddof=1)) 
                        ## Apply xy correction
                        if corr[-len(XY_CORR) :] == XY_CORR:
                            sigmaVisPerScan[-1] *= 1.0 + 0.01 * xyCorr[f]
                            sigmaVisErrPerScan[-1] *= 1.0 + 0.01 * xyCorr[f]

                    sigmaVisPerScan = np.array(sigmaVisPerScan)/1000   #   <--1.0098 refers to XY correction
                   # sigmaVisErrPerScan = np.array(sigmaVisErrPerScan)*1.0098/1000 
                    sigmaVisErrPerScan = np.array(rmsPerScan)/1000

                    ## Fit a constant to get sigma vis for all scans
                    sigmaVis = np.average(sigmaVisPerScan, weights=1.0 / (sigmaVisErrPerScan ** 2))
                    sigmaVisErr = 1.0 / np.sqrt(sum(1.0 / sigmaVisErrPerScan ** 2))
                    
                    rchi2 = sum(((sigmaVisPerScan - sigmaVis) / sigmaVisErrPerScan) ** 2) / (len(sigmaVisPerScan) - 1)

                    print("Average xsec for %s:\t%.2f +- %.2f" % (det, sigmaVis, sigmaVisErr))
                    print("Reduced chisquare:\t%.2f" % rchi2)
                    print(
                        (
                            "Scan-to-scan max(max-avg,avg-min)/avg syst unc [%%]:\t%.1f"
                            % (100.0 * (max(max(sigmaVisPerScan) - sigmaVis, sigmaVis - min(sigmaVisPerScan))) / sigmaVis)
                        )
                    )
                    # print("Std/Avg [%%]: %.2f" %(100.*np.std(sigmaVisPerScan)/sigmaVis))

                    if makeLatexTable:
                        sigVisDict[det][corrs[corr][-2:]] = [sigmaVis, sigmaVisErr]

                    plt.figure()

                  
                    plt.errorbar(xaxis, sigmaVisPerScan, yerr=sigmaVisErrPerScan, fmt="o", color="b")
                    plt.xlim(0, len(labels) + 1)
                    if fill =="8381":
                        plt.ylim(1055,1095)
                    elif fill =="8999":
                        plt.ylim(1260,1320)
                 #   plt.ylim(3290,3380)#HFET

                    #yMean_z =  np.average(sv, weights=1./(sverr**2))    
                    sv_std=sigmaVisPerScan.std(ddof=1) 
                    print(sv_std)
                   # print("RMS:",np.sqrt(1/len(sigmaVisPerScan)*np.sum((sigmaVisPerScan)**2))
                   # print(sigmaVisPerScan)
                   # print(sigmaVisPerScan**2)
                   # print("RMS:",np.sqrt(np.sum(sigmaVisPerScan**2)/len(sigmaVisPerScan)))
                   # print("RMS:",np.sqrt(np.mean(np.square(sigmaVisPerScan))))
                    plt.fill_between( [plt.xlim()[0],plt.xlim()[1]], y1=[sigmaVis-sv_std,sigmaVis-sv_std], y2=[sigmaVis+sv_std,sigmaVis+sv_std], color='gray',alpha=0.5)


                    #print("RMS:",np.sqrt(1/len(sigmaVisPerScan)*(sigmaVisPerScan)**2)   

                    plt.plot(plt.xlim(), [sigmaVis, sigmaVis], ls="-", color="r")
                    plt.xticks(xaxis, labels, fontsize = 12)
                   # plt.xlabel('Scan',fontsize=18) 
                    plt.ylabel("$\sigma_{vis}$ [$mb$]", fontsize=19)
                    plt.grid()
                    plt.ticklabel_format(useOffset=False, axis="y")  # Prevent plt from adding offset to y axis
                    if  fill == "8381":
                        sigmaVis = 1076.00 
                    else:
                        sigmaVis = sigmaVis
                    plt.figtext(
                    x=0.88,
                    y=0.87,
                    ha="right",
                    va="top",
                    fontsize=12,
                        s="$\sigma_{vis}$ = %.2f $\pm$ %.2f (stat) mb" % (sigmaVis, sigmaVisErr),
                    backgroundcolor="white",
                    ).set_bbox(dict(color="w", alpha=1))
                   # print("RMS:",np.sqrt(1/len(sigmaVisPerScan)*(sigmaVisPerScan)**2) 
                    # Set title, labels, ...
                   # ipt.makeTitle(ipt.makeTitleText(var="$\sigma_{vis}$", fit=fit, corr=corrs[corr], det=det))

                    # Make CMS preliminary, year, fill
                    if fill == "8381":
                        plt.figtext(.9, .91, "Fill 8381 (2022, 13.6 TeV)", fontsize=13, style="normal", backgroundcolor="white", ha="right").set_bbox(dict(alpha=0.0))
                        plt.figtext(.15, 0.84, "CMS", fontsize=15, fontweight="bold", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0)) 
                        plt.figtext(.15 +0.07, 0.84, "Preliminary ", fontsize=13, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))
                        plt.figtext(.15 , 0.80, "PCC", fontsize=13, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0)) 
                       # ipt.makeCMSPreli(year="Fill "+ fill+"  "+year, fill=fill, energy= energy+" TeV")
                    elif fill == "6868":
                        plt.figtext(.9, .91, "Fill 6868 (2018, 13 TeV)", fontsize=13, style="normal", backgroundcolor="white", ha="right").set_bbox(dict(alpha=0.0))    
                        plt.figtext(.15, 0.84, "CMS", fontsize=15, fontweight="bold", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))                     
                        plt.figtext(.15 +0.07, 0.84, "Preliminary ", fontsize=13, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))      
                        plt.figtext(.15 , 0.80, "PCC", fontsize=13, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))
                    elif fill == "8999":                                                                                                                                    
                        plt.figtext(.9, .91, "Fill 8999 (2023, 13.6 TeV)", fontsize=13, style="normal", backgroundcolor="white", ha="right").set_bbox(dict(alpha=0.0))      
                        plt.figtext(.15, 0.84, "CMS", fontsize=15, fontweight="bold", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))                        
                        plt.figtext(.15 +0.07, 0.84, "Preliminary ", fontsize=13, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))            
                        plt.figtext(.15 , 0.80, "PCC", fontsize=13, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))    
                       # ipt.makeCMSPreli(year="Fill "+ fill+"  "+year, fill=fill, energy= energy+" TeV")
                    #ipt.makeCMSPreli(year=year, fill=fill, energy=energy, y=0.475)

                    # Save figure
                    figName = makeFigName(path=outputPlotsDirBase, fill=fillD, scanName="", var="sigmavis/allscans", corr=corrs[corr], det=det, fit=fit)
                    figName = figName + (suffix != "") * "_" + suffix
                    folderpath = ipt.getFolders(figName)
                    if not os.path.exists(folderpath):
                        os.makedirs(folderpath)
                    ipt.savefig(figName, figureExtension)
                    print("figname",figName, figureExtension)
                    plt.close()

            if makeLatexTable:
                print("\n")

                detName = OrderedDict()
                detName["BCM1FPCVD"] = "BCM1F"
                detName["PLT"] = "PLT"
                detName["HFOC"] = "HFOC"

                corrName = OrderedDict()
                corrName["NC"] = " No correction              "
                corrName["GS"] = " Ghost and satellite charge "
                corrName["BG"] = " Noncollision rate          "
                corrName["OD"] = " Orbit drift                "
                corrName["LS"] = " Length scale               "
                corrName["XY"] = " $x$-$y$ factorization      "

                corrList = []
                detList = []

                table = "\hline\n"
                table = table + "                       &  Correction                 "
                for det in list(detName.keys()):
                    if det in list(sigVisDict.keys()):
                        if det not in detList:
                            detList.append(det)
                        table = table + "&  " + det + (19 - len(det)) * " "
                table = table + "\\\\\n"
                table = table + "\hline\n"
                table = table + "\hline\n"
                table = table + "\multirow{6}{*}{\shortstack{Visible cross\\\\section \sigmaVis} [\mub]}\n"

                for corr in list(corrName.keys()):
                    if corr in (key[-2:] for key in list(sigVisDict[dets[0]].keys())):
                        corrList.append(corr)
                        table = table + "                       & " + corrName[corr]
                        for det in list(detName.keys()):
                            if det in list(sigVisDict.keys()):
                                table = table + "&  $ " + ("%.1f" % sigVisDict[det][corr][0]) + " \pm " + ("%.1f" % sigVisDict[det][corr][1]) + " $  "
                        table = table + "\\\\\n"

                table = table + "\hline\n"
                table = table + "\hline\n"
                table = table + "\multirow{6}{*}{\shortstack{Average effect\\\\on \sigmaVis [\%]}}\n"

                for icorr, corr in enumerate(corrList[1:]):
                    table = table + "                       & " + corrName[corr]
                    for det in detList:
                        effect = (sigVisDict[det][corr][0] / sigVisDict[det][corrList[icorr]][0] - 1) * 100
                        table = table + "&  $ " + ("%+.1f" % effect) + "          $  "
                    table = table + "\\\\\n"
                print(table)

        if SigCorrelation:
            print("\n")
            print("=====================================================================================")
            print("SigmaVis correlation coefficients are in progress!")
            for corr in sorted(list(corrs.keys()), key=len):
                print(("\n=== Corr %s ===" % corr))
                for f in sorted(list(SigDict[corr].keys()), key=lambda name: int(name[0:4] + name[13:17])):
                    print(("\n=== Scan %s ===" % f))
                    cc = 0
                    xaxisList = []
                    CSV_DICT = {}
                    ntabmax = max((len("Sigma vis correlation for %s %s:" % (det1, det2)) // 8 for det1 in dets for det2 in dets if det1 != det2))
                    for idet1 in range(len(dets) - 1):
                        for idet2 in range(idet1 + 1, len(dets)):
                            ## Get sigma vis values
                            det1 = dets[idet1]
                            det2 = dets[idet2]
                            v1 = SigDict[corr][f][det1]
                            v2 = SigDict[corr][f][det2]
                            bcidV1 = list(v1[0][2])
                            bcidV2 = list(v2[0][2])
                            v1C = []
                            v2C = []
                            for ibcid in range(len(bcidV1)):
                                if bcidV1[ibcid] in bcidV2:
                                    v1C.append(v1[0][0][ibcid])
                                    v2C.append(v2[0][0][bcidV2.index(bcidV1[ibcid])])
                            ntab = 1 + ntabmax - len("Sigma vis correlation for %s %s:" % (det1, det2)) // 8
                            tab = ntab * "\t"
                            corrCoef = np.corrcoef(v1C, v2C)[0, 1]
                            print(("Sigma vis correlation for %s %s:%s%.3f" % (det1, det2, tab, corrCoef)))

        if PeakPeakEffect:
            print("Peak to Peak correction plots are in progress!")
            sigmaNCnoPP = {}
            sigmaPP = {}

            for corr in sorted(list(corrs.keys()), key=len):
                sigmaPP[corr] = {}
                for f in sorted(list(SigDict[corr].keys()), key=lambda name: int(name[0:4] + name[13:17])):
                    print(("\n=== Scan %s ===" % f))
                    sigmaPP[corr][f] = {}
                    if not f in list(sigmaNCnoPP.keys()):
                        sigmaNCnoPP[f] = {}
                    plt.figure()
                    cc = 0
                    xaxisList = []
                    CSV_DICT = {}
                    for det in dets:
                        # Make graph for detector det
                        sigma0 = SigDict[corr][f][det]
                        peak0 = peakDict[corr][f][det]
                        capSigma0 = CapDict[corr][f][det]

                        # Get peak, capsigma and sigma arrays corresponding to common bcid
                        xaxis = []
                        sigma = [[], []]  # [ [values], [errors] ]
                        peakX = [[], []]
                        peakY = [[], []]
                        capSigmaX = [[], []]
                        capSigmaY = [[], []]
                        for bcid in range(bcidStart, bcidEnd + 1):
                            s = getValueError(sigma0, bcid)
                            pX = getValueError(peak0, bcid, "X")
                            pY = getValueError(peak0, bcid, "Y")
                            cX = getValueError(capSigma0, bcid, "X")
                            cY = getValueError(capSigma0, bcid, "Y")
                            if s != (None, None) and pX != (None, None) and pY != (None, None) and cX != (None, None) and cY != (None, None):
                                xaxis.append(bcid)
                                for i in range(2):
                                    sigma[i].append(s[i])
                                    peakX[i].append(pX[i])
                                    peakY[i].append(pY[i])
                                    capSigmaX[i].append(cX[i])
                                    capSigmaY[i].append(cY[i])

                        xaxis = np.array(xaxis)
                        sigma = np.array(sigma)
                        peakX = np.array(peakX)
                        peakY = np.array(peakY)
                        capSigmaX = np.array(capSigmaX)
                        capSigmaY = np.array(capSigmaY)

                        # sigma visible = 2*pi * Cx * Cy * (Px + Py)/2
                        # In units of calculateCalibrationConstant.py: Ci -> Ci*1000  Pi -> Pi
                        sigmaNoPP = np.pi * capSigmaX[0] * capSigmaY[0] * (peakX[0] + peakY[0]) * 10 ** 6
                        sigmaNoPPErr = sigmaNoPP * np.sqrt(
                            capSigmaX[1] ** 2 / capSigmaX[0] ** 2
                            + capSigmaY[1] ** 2 / capSigmaY[0] ** 2
                            + (peakX[1] ** 2 + peakY[1] ** 2) / (peakX[0] + peakY[0]) ** 2
                        )

                        if corr == "noCorr":
                            sigAvg = sigmaNoPP.mean()
                            print(("Average sigma vis for NC without PP for %s: %.1f" % (det, sigAvg)))
                            if not det in list(sigmaNCnoPP[f].keys()):
                                sigmaNCnoPP[f][det] = sigAvg
                        else:
                            sigAvg = sigma[0].mean()
                            print(("Average sigma vis for %s with PP for %s: %.1f" % (corr, det, sigAvg)))
                            sigmaPP[corr][f][det] = sigAvg

                        xaxisList.append(xaxis)
                        yaxis = (sigma[0] / sigmaNoPP) - 1
                        yerr = yaxis * np.sqrt(sigma[1] ** 2 / sigma[0] ** 2 + sigmaNoPPErr ** 2 // sigmaNoPP ** 2)

                        # Use % units
                        yaxis *= 100
                        yerr *= 100

                        plt.errorbar(xaxis, yaxis, yerr=yerr, fmt="o", color=colorList[cc], label=det)
                        CSV_DICT.update({det + " X": xaxis, det + " Y": yaxis, det + " Y error": yerr})

                        # Print mean and standard dev
                        yMean = yaxis.mean()
                        ystd = yaxis.std()
                        print(("Average effect of Peak to Peak corr for %s %s: %.3f" % (corr, det, yMean)))
                        print(("Standard deviation is: %.3f" % (ystd)))
                        plt.plot([plt.xlim()[0], plt.xlim()[1]], [yMean, yMean], ls="--", color=colorList[cc])

                        cc += 1

                    # Make CMS preliminary, year, fill
                    ipt.makeCMSPreli(year=year, fill=fill, energy=energy)

                    # Set title, labels, ...
                    ipt.makeTitle(ipt.makeTitleText(scanName=filedict[f], var="Effect of Peak to Peak corr on $\sigma_{vis}$", corr=corrs[corr], fit=fit))

                    plt.grid()
                    plt.xlabel("Bunch ID")
                    plt.ylabel("Effect of Peak to Peak corr on $\sigma_{vis}$ [%]", fontsize=14)
                    plt.ticklabel_format(useOffset=False, axis="y")  # Prevent plt from adding offset to y axis
                    plt.legend(loc="best", fontsize=12)

                    # Save figure
                    figName = makeFigName(path=outputPlotsDirBase, fill=fillD, scanName=f, var="sigmavis/correffectPP", corr=corrs[corr], fit=fit)
                    figName = figName + (suffix != "") * "_" + suffix
                    folderpath = ipt.getFolders(figName)
                    if not os.path.exists(folderpath):
                        os.makedirs(folderpath)
                    ipt.savefig(figName, figureExtension)

                    # Save csv file
                    if identicalXAxis(xaxisList):
                        csvName = makeCSVName(path=outputCsvDirBase, fill=fillD, scanName=f, var="sigmavis/correffectPP", corr=corrs[corr], fit=fit)
                        csvName = csvName + (suffix != "") * "_" + suffix
                        folderpath = ipt.getFolders(csvName)
                        if not os.path.exists(folderpath):
                            os.makedirs(folderpath)
                        PLOT_DF = pd.DataFrame(data=CSV_DICT)
                        ipt.toCSV(PLOT_DF, csvName)

                    plt.close()

            if "noCorr" in list(corrs.keys()):
                for corr in list(corrs.keys()):
                    if corr != "noCorr":
                        for f in list(SigDict[corr].keys()):
                            print(("\n=== File %s ===" % f))
                            for det in dets:
                                print(
                                    (
                                        "Effect of %s + Peak to Peak corr with respect to no corr on sigma vis for %s: %.2f %%"
                                        % (corr, det, ((sigmaPP[corr][f][det] / sigmaNCnoPP[f][det]) - 1) * 100)
                                    )
                                )


def main():
    """This method should be the only one used by the user for making plots (no need for modifying or running directly other methods)"""
    parser = argparse.ArgumentParser(description="sxsec", formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        "--preset",
        help="""capsigmaratio: Pre-defined CapSigma ratio plots,
    sigma_vis: Pre-defined sigma visible plots,
    sigma_vis_correlation: Correlation between the sigma vis as a function of BCID of the different luminometers""",
        choices=["capsigmaratio", "sigma_vis", "sigma_vis_correlation"],
        type=str,
    )

    parser.add_argument("--zoom", help="", choices=[1, 2], type=int)

    parser.add_argument("--suffix", help="", default="", type=str)

    parser.add_argument("--fit", help="Fit function", default="SG", type=str)

    parser.add_argument("--luminometers", default="BCM1FPCVD_PLT", type=str)

    parser.add_argument("--year", default="", type=str)

    parser.add_argument("--energy", default="xxx TeV", type=str)

    parser.add_argument("--fill", default="", type=str)

    args = parser.parse_args()

    dets = args.luminometers.split("_")


    # Scan information:
    # Define an ordered dictionary key:value full_scan_name:short_scan_name
    # In cap evol and all scans plots, the order of the scans is used, and the x axis is the value
    #  associated with the scan name
    # The short scan names are used in the title of all the plots
    filedict = OrderedDict()

    # Example for 2018 PbPb:
  #  filedict["6868_30Jun18_132834_30Jun18_135235"] = "vdM1"
  #  filedict["6868_30Jun18_151040_30Jun18_153416"] = "BI1"
  #  filedict["6868_41Jul18_002854_01Jul18_005345"] = "BI3"
  #  filedict["6868_41Jul18_012042_01Jul18_014645"] = "BI4"
  #  filedict["6868_41Jul18_030039_01Jul18_032641"] = "vdM2"
  #  filedict["6868_41Jul18_035146_01Jul18_041104"] = "vdM3"
  #  filedict["6868_41Jul18_075134_01Jul18_081132"] = "vdM4"
  
    # 2022 pp 13 TeV
  #  filedict["8381_11Nov22_004152_11Nov22_010424"] = "vdM1"
  #  filedict["8381_11Nov22_095321_11Nov22_101617"] = "vdM3"
  #  filedict["8381_11Nov22_114759_11Nov22_121408"] = "vdM4"
  #  filedict["8381_11Nov22_012659_11Nov22_015155"] = "BI1"
  #  filedict["8381_11Nov22_021738_11Nov22_024314"] = "BI2"

    filedict["8999_28Jun23_000143_28Jun23_232943"] = "vdM1"
    filedict["8999_29Jun23_004658_29Jun23_011220"] = "BI1"
    filedict["8999_29Jun23_013851_29Jun23_020425"] = "BI2"
    filedict["8999_29Jun23_023227_29Jun23_025502"] = "vdM2"
    filedict["8999_29Jun23_073830_29Jun23_080352"] = "vdM3"
    filedict["8999_29Jun23_092415_29Jun23_094738"] = "vdM4"
    filedict["8999_29Jun23_110004_29Jun23_112226"] = "vdM5"
    filedict["8999_29Jun23_123257_29Jun23_125514"] = "vdM6"

   # filedict["6016_28Jul17_100134_28Jul17_102201"] = "vdM1"                                                        
   # filedict["6016_28Jul17_104434_28Jul17_110623"] = "vdM2"  
   # filedict["6016_28Jul17_115005_28Jul17_121320"] = "Img1"  
   # filedict["6016_28Jul17_124433_28Jul17_130718"] = "Img2"  
   # filedict["6016_28Jul17_144025_28Jul17_150150"] = "vdM3"  
   # filedict["6016_28Jul17_212416_28Jul17_214320"] = "vdM4"

    ## xy factorisation bias correction
    # dict full_scan_name:correction
    # If you asked for xxxx_xyFactorization correction, you must provide this dictionary
    # Example for 2018 PbPb:
   # xyCorr = {
    #    "8381_11Nov22_004152_11Nov22_010424": 1.0098,
     #   "8381_11Nov22_012659_11Nov22_015155": 1.0098,
      #  "8381_11Nov22_021738_11Nov22_024314": 1.0098,
       # "8381_11Nov22_095321_11Nov22_101617": 1.0098,
       # "8381_11Nov22_114759_11Nov22_121408": 1.0098
   # }

    # Initialisation of the modules for the perfill function
    if args.preset == "capsigmaratio":
        if args.zoom == 1:
            xlim = [0, 3500]
        elif args.zoom == 2:
            xlim = [300, 800]
        else:
            xlim = None

        corrs = {"noCorr": "NC"}#, "Background": "BG", "Background_OrbitDrift": "BG_OD", "Background_OrbitDrift_LengthScale": "BG_OD_LS"}

        perfill(args.fill, filedict, dets, args.fit, corrs, args.year, args.energy, CapRatio=True, xlim=xlim)

    elif args.preset == "sigma_vis":
        if args.zoom == 1:
            xlim = [200, 3500]
            makeLatexTable = False
        elif args.zoom == 2:
            xlim = [300, 800]
            makeLatexTable = False
        else:
            makeLatexTable = True
     #   corrs ={"OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta": "All Corr"}
        if args.fill == "8381": 
            corrs ={"OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta_ResidualOrbitDrift_LengthScale": "All Corr"}
        elif args.fill == "6868": 
            corrs ={"Background_OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta_LengthScale_PeakToPeak": "All Corr"}
        elif args.fill == "8999":
            corrs ={"Background_OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta_ResidualOrbitDrift_LengthScale": "All Corr"}
        perfill(args.fill, filedict, dets, args.fit, corrs, args.year, args.energy, SigNormal=True, SigAll=True, xlim=xlim, makeLatexTable=makeLatexTable)

    elif args.preset == "sigma_vis_correlation":
        corrs = {
            "noCorr": "NC"
          #  "GhostSatellite": "GS",
          #  "GhostSatellite_Background": "GS_BG",
          #  "GhostSatellite_Background_OrbitDrift": "GS_BG_OD",
          #  "GhostSatellite_Background_OrbitDrift_LengthScale": "GS_BG_OD_LS",
        }
        perfill(args.fill, filedict, dets, args.fit, corrs, args.year, args.energy, SigCorrelation=True)

    else: # Playground
        #corrs = {"Background_OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta_LengthScale_PeakToPeak": "BG_OD_LS"}
        corrs = {"noCorr": "NC"}

        perfill(args.fill, filedict, dets, args.fit, corrs, args.year, args.energy,
            CapRatio=True, CapNormal=True, peakNormal=True, SigEffect=True, PeakPeakEffect=True,
            CapEvol=True, SigAll=True, SigCorrelation=True)

    # ----------------------------------------------------------------------------
    # SpecialCases Dictionary
    # For any special case of a detector this dictionary should be filled by structure as:
    # {'detector':['fitname', [scan numbers that this exception will be applied]]}
    # Example: SpecDict={'HFOC':[fit+'Const',[1,2,5,7]]}
    # SpecDict={'HFOC':[fit+'Const',[1,2,5,7]]}

    # ----------------------------------------------------------------------------
    # Settings for allyear function
    #fitAY = "SG"
    #detsAY = ["HFOC", "HFET"]
    #corrsAY = {"noCorr": "NC", "BeamBeam": "BB"}

    # ----------------------------------------------------------------------------
    # Modules for allyear function
    #FOM = True
    #Emittance = False

    # * Here you would call allyear *


if __name__ == "__main__":
    main()
