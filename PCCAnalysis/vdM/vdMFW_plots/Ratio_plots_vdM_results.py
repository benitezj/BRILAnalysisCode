import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt

#mpl.use("Agg")
#mpl.style.use('classic')
import mplhep as hep                                                                                                          
hep.style.use("CMS")

##2022
path1="/eos/user/l/lcuevasp/2022_Analisys/2022_output/vdm22_rerun_20230811/v2_new_tunes_rod_ls_all_sem"
path2="/eos/user/l/lcuevasp/2022_Analisys/2022_output/vdm22_rerun_20230811/v2_new_tunes_rod_ls_all_sem"

outpath='/eos/user/l/lcuevasp/'


corrTemp1="OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta_ResidualOrbitDrift_LengthScale"
corrTemp2="Background_OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta_ResidualOrbitDrift_LengthScale"

fill = "8381"
year = "2022"
fit1='DGConst'
fit2='DG'
det1="pcchd5"
det2="HFOC"
#######################2018(6868)#####################
"""
scans = ["6868_30Jun18_132834_30Jun18_135235", #vdM1   
         "6868_30Jun18_151040_30Jun18_153416", #img1              
         "6868_01Jul18_002854_01Jul18_005345", #img2 
         "6868_01Jul18_012042_01Jul18_014645", #img3              
         "6868_01Jul18_030039_01Jul18_032641", #vdM2      
         "6868_01Jul18_035146_01Jul18_041104", #vdM3                                         
         "6868_01Jul18_075134_01Jul18_081132"] #vdM4

text_values=['VdM1','Img1','Img2',Img3','VdM2','VdM3','VdM4']
BCIDS = [265,865,1780,2192,3380]
"""
#####################2017(6016)#######################         
"""                                                  
scans = ["6016_28Jul17_100134_28Jul17_102201",  #"VdM1"                 
         "6016_28Jul17_104434_28Jul17_110623", #"VdM2
         "6016_28Jul17_115005_28Jul17_121320", #"Img1"  
         "6016_28Jul17_124433_28Jul17_130718", #"Img2"          
         "6016_28Jul17_144025_28Jul17_150150", #"vdM3"              
         "6016_28Jul17_212416_28Jul17_214320"] #"VdM4"

text_values=['VdM1','VdM2','Img1','Img2','VdM3','VdM4']
"""
###############2022(8381)###########################

scans = ['8381_11Nov22_004152_11Nov22_010424',
        '8381_11Nov22_012659_11Nov22_015155',
        '8381_11Nov22_021738_11Nov22_024314',
        '8381_11Nov22_095321_11Nov22_101617',
        '8381_11Nov22_114759_11Nov22_121408']
#'8381_11Nov22_081031_11Nov22_083511',

text_values=['VdM1','Img1','Img2','VdM3','VdM4']                                                                 
BCIDS = [282,822,2944,3123,3302]
#######################2023(8999)#####################                                                                
""" 
                                                   
scans = ["8999_28Jun23_230143_28Jun23_232943", #vdM1                
         "8999_29Jun23_004658_29Jun23_011220", #BI1                    
         "8999_29Jun23_013851_29Jun23_020425", #BI2           
         "8999_29Jun23_023227_29Jun23_025502", #vdM2   
         "8999_29Jun23_073830_29Jun23_080352", #vdM3    
         "8999_29Jun23_092415_29Jun23_094738", #vdM4              
         "8999_29Jun23_110004_29Jun23_112226", #vdM5
         "8999_29Jun23_123257_29Jun23_125514"] #vdM6                             
                                                                           
text_values=['vdM1','BI1','BI2','vdM2','VdM3','VdM4','VdM5','vdM6']                                                   """       
                                                         



a = np.arange(1,len(text_values)*10+1,1)                                
x0 = np.arange(1,len(text_values)*5+1,1)
#print(x0)
x0_covX = []
x0_covY = []

count = 0
for l in a:
    count +=1
    if count <= 5:x0_covX.append(l)
    else: x0_covY.append(l)
    if count == 10:count=0


data_columns = ["CapSigma","peak","chi2","covStatus","xsec"]

for col in data_columns:
    Xscan_PCC  = []
    Yscan_PCC  = []
    Xscan_DET  = []  
    Yscan_DET  = []
    for scan in scans:
        if col =="xsec":
            fullpath_PCC=path1+"/"+scan+"/"+det1+"/results/"+corrTemp1+"/"+"LumiCalibration_pcchd5_"+fit1+"_"+fill+".csv"
            data1=pd.read_csv(fullpath_PCC)                                                 
            
            fullpath_DET=path2+"/"+scan+"/"+det2+"/results/"+corrTemp2+"/"+"LumiCalibration_HFOC_"+fit2+"_"+fill+".csv"
            data2=pd.read_csv(fullpath_DET) 
            data2 = data2[data2["BCID"].isin(["282","822","2944","3123","3302"])]

            for i,j,i_,j_ in zip(np.asarray(data1.xsec),np.asarray(data1.xsecErr),np.asarray(data2.xsec),np.asarray(data2.xsecErr)):
                Xscan_PCC.append(i)
                Yscan_PCC.append(j)
                Xscan_DET.append(i_)   
                Yscan_DET.append(j_) 
        else: 
            fullpath_PCC=path1+"/"+scan+"/"+det1+"/results/"+corrTemp1+"/"+fit1+"_FitResults.csv"
            data1=pd.read_csv(fullpath_PCC)

            fullpath_DET=path2+"/"+scan+"/"+det2+"/results/"+corrTemp2+"/"+fit2+"_FitResults.csv"                   
            data2=pd.read_csv(fullpath_DET)
            data2 = data2[data2["BCID"].isin(["282","822","2944","3123","3302"])]

            print(data2["BCID"])

            for i,j,k,i_,j_,k_ in zip(np.asarray(data1.Type),np.asarray(data1[col]),np.asarray(data1.ndof),
                                      np.asarray(data2.Type),np.asarray(data2[col]),np.asarray(data2.ndof)):


                if i == 'X':
                    if col == "chi2":
                        Xscan_PCC.append(j/k)
                        Xscan_DET.append(j_/k_)
                    else:
                        Xscan_PCC.append(j)
                        Xscan_DET.append(j_)
                else:
                    if col == "chi2":
                        Yscan_PCC.append(j/k)
                        Yscan_DET.append(j_/k_)      
                    else:
                        Yscan_PCC.append(j)
                        Yscan_DET.append(j_)

    if col =="covStatus":
        x1=10.5                   
        x2=20.5                                                                                                     
        plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0)
        x1=30.5
        x2=40.5
        plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade
        x1=50.5                                                                                                      
        x2=60.5                                                                                                      
        plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0)
    
    else:
        x1=5.5                          
        x2=10.5                                        
        plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) 
        x1=15.5
        x2=20.5
        plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade
        x1=25.5   
        x2=30.5                         
        plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0)
   # print(Xscan)
    if col == "covStatus":
        #print(len(x0_covX),len(Xscan))
       # print(x0_covX,Xscan)
        plt.errorbar(x0_covX,Xscan_PCC, fmt='o', color = 'red',markersize=12,label = 'PCC_X-Scans')          
        plt.errorbar(x0_covY,Yscan_PCC, fmt='o', color = 'blue',markersize=12,label = 'PCC_Y-Scans')
        plt.errorbar(x0_covX, Xscan_DET, fmt='o', color = 'green',markersize=12,label = 'HFET_X-Scans')   
        plt.errorbar(x0_covY, Yscan_DET, fmt='o', color = 'black',markersize=12,label = 'HFET_Y-Scans')        


    elif col == "xsec":
        plt.errorbar(x0, Xscan_PCC, yerr=Yscan_PCC, fmt='o', color = 'black',markersize=15,label = "xsec_PCC")
        plt.errorbar(x0, Xscan_DET, yerr=Yscan_DET, fmt='o', color = 'green',markersize=15,label = "xsec_HFET")

        spread = round((np.std(Xscan)/np.mean(Xscan))*100,3) 
        plt.title('SigmaVis [$\mu b$ ]',pad = 30,fontsize =40)
       # rms = np.std(Xscan)
       # mean  = np.mean(Xscan)
       # sem = np.std(Xscan)/np.sqrt(len(Xscan))        
       # print("SEM:",np.std(Xscan)/np.sqrt(25))  
       # plt.figtext(x=0.895, y=0.89, ha="right", va="top", fontsize=25,                                     
        #            s="MEAN = %.2f\nSEM  = %.2f\nRMS = %.2f" %(mean,sem,rms),                          
         #                       backgroundcolor="white").set_bbox(dict(color='w', alpha=1))
        #plt.figtext(x=0.895, y=0.89, ha="right", va="top", fontsize=25, s='Std(BunchSpread): \n'+ str(round(np.std(Xscan)/1000,3))+' mb  ' + '('+ str(spread) +'%)',backgroundcolor="white").set_bbox(dict(color='w', alpha=1))
       # print("std:",np.std(Xscan))
       # print("SEM:",np.std(Xscan)/np.sqrt(25))        
   # elif col == "peak":
    #    plt.errorbar(x0, np.array(Xscan)/np.array(Yscan), fmt='o', color = 'red',markersize=12,label = 'Xpeak/Ypeak')
   # elif col == "CapSigma":
#        Xscan_ = np.array(Xscan)
 #       Yscan_ = np.array(Yscan)
  #      print(x0,Xscan_,Yscan_)
    #    plt.errorbar(x0, np.array(Xscan)*np.array(Yscan), fmt='o', color = 'blue',markersize=13,label = 'CapX*CapY')
    else:
       # print(x0,Xscan)
       # print(col,len(x0),len(Xscan))
       # if  col == "CapSigma":
        #    print(x0,Yscan)
           # plt.errorbar(x0, Xscan*Yscan, fmt='o', color = 'red',markersize=12,label = 'CapX*CapY')
        plt.errorbar(x0,np.abs(np.array(Xscan_PCC)/np.array(Xscan_DET)), fmt='o', color = 'red',markersize=12,label = "X-"+det1+"/"+det2)
        plt.errorbar(x0,np.abs(np.array(Yscan_PCC)/np.array(Yscan_DET)), fmt='o', color = 'blue',markersize=12,label = "Y-"+det1+"/"+det2 )
       # plt.errorbar(x0, Xscan_DET, fmt='o', color = 'green',markersize=12,label = str(det2)+'_X-Scans')
       # plt.errorbar(x0, Yscan_DET, fmt='o', color = 'black',markersize=12,label = str(det2)+'_Y-Scans')
  
       # print(col,len(x0),len(Xscan))
    if col == "chi2":
        avg = round(np.mean(Xscan+Yscan),5)
        plt.axhline(avg , color = 'green', lw = 4)
        plt.title('Fit Quality ('+col+'/ndof)',pad = 30,fontsize =40)
        plt.ylabel(col+'/ndof',fontsize = 30);
       # plt.figtext(x=0.89, y=0.04, ha="right", va="top", fontsize=35, s="Avg="+str(avg),backgroundcolor="white").set_bbox(dict(color='w', alpha=1))
    elif col == "Const":                                                                            
        avg_x = round(np.mean(Xscan),5)
        avg_y = round(np.mean(Yscan),5)                           
        plt.title(col,pad = 30,fontsize =40)                                            
        plt.ylabel(col,fontsize = 30);                                                                     
        plt.ylim(0.20,0.60)
        plt.figtext(x=0.895, y=0.895, ha="right", va="top", fontsize=30, s="Avg_X = "+str(avg_x),backgroundcolor="white").set_bbox(dict(color='w', alpha=1))
        plt.figtext(x=0.895, y=0.865, ha="right", va="top", fontsize=30, s="Avg_Y = "+str(avg_y),backgroundcolor="white").set_bbox(dict(color='w', alpha=1))    
    elif col =="xsec":
        plt.title('SigmaVis [$\mu b$ ]',pad = 30,fontsize =40) 
        plt.ylabel('xsec [$\mu b$ ]',fontsize = 30);
    else:
        plt.title(col+" ratios" , pad = 30,fontsize =40)
        plt.ylabel(col+" ratios",fontsize = 30);

    plt.xlabel('Scans and BCID',labelpad =30, fontsize =30)
    plt.legend(loc = 'best',scatterpoints =1,fontsize = 20);
    plt.tick_params(axis='x',labelbottom = True,pad = 10 ,direction = 'in', length =1, labelsize=15)
    j = np.arange(3,len(text_values)*5+1,5)       
    
    if col == "covStatus":
        plt.xticks(np.arange(5,len(text_values)*10+1,10), text_values , fontsize = 23)
        plt.xlim(0,len(text_values)*10+1)
        plt.ylim(0,4)        
    else:
        plt.xticks(j, text_values , fontsize = 23)
        plt.xlim(0,len(text_values)*5+1)
   
    plt.figtext(.9, .91, "Fill "+fill+" ("+year+", 13 TeV)", fontsize=35, style="normal", backgroundcolor="white", ha="right").set_bbox(dict(alpha=0.0))                                                                                                         
    plt.figtext(.14, 0.91, "CMS", fontsize=35, fontweight="bold", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))                                                                                                                             
    plt.figtext(.14 +0.05, 0.91, "Work in Progress ", fontsize=35, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))  

    plt.yticks(fontsize=23)
    plt.ticklabel_format(useOffset=False, axis='y')  # Prevent plt from adding offset to y axis
    fig = plt.gcf()
    fig.set_size_inches(25, 14)
    plt.grid(0)
    plt.savefig(str(outpath)+str(fit1)+'_'+col+'ratios.png', dpi=500, bbox_inches = None)
    Xscan = []                                            
    Yscan = []
    plt.clf()
