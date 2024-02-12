import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt

#mpl.use("Agg")
#mpl.style.use('classic')
import mplhep as hep
hep.style.use("CMS")

path="/eos/user/l/lcuevasp/2023_Analisys/output_bgf.003/analysed_data"
#path="/eos/user/l/lcuevasp/2017_Analisys/new_ratefile_studys/output_allCorr/output_no_normalization/analysed_data"
#path = "/eos/user/l/lcuevasp/2023_Analisys/output/analysed_data"
#outpath='/eos/user/l/lcuevasp/2017_Analisys/new_ratefile_studys/output_allCorr/analysed_data/plots_Poly4G_bgPerB.1/'
#path="/eos/user/l/lcuevasp/2017_Analisys/new_ratefile_studys/output_allCorr/analysed_data"
#outpath='/eos/user/l/lcuevasp/2023_Analisys/output/plots/DG'

#path="/eos/user/l/lcuevasp/2017_Analisys/output_2017_newdata/analysed_data"    
outpath='/eos/user/l/lcuevasp/' 

corrTemp="Background_BeamBeam_DynamicBeta"
#corrTemp = "noCorr"
#'Background_OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta_LengthScale_PeakToPeak'
#OutPathPlots='/eos/user/j/jmorenoc/thesis_plots/'
fill = "8999"
fit='DG'
det="pcchd5"
ax = plt.gca()

# Utilizar transAxes en el objeto Axes
trans = ax.transAxes
#######################2018(6868)#####################
"""
scans = ["6868_30Jun18_132834_30Jun18_135235", #vdM1   
         "6868_30Jun18_151040_30Jun18_153416", #img1              
         "6868_41Jul18_002854_01Jul18_005345", #img2 
         "6868_41Jul18_012042_01Jul18_014645", #img3              
         "6868_41Jul18_030039_01Jul18_032641", #vdM2      
         "6868_41Jul18_035146_01Jul18_041104", #vdM3                                         
         "6868_41Jul18_075134_01Jul18_081132"] #vdM4

text_values=['vdM1','Img1','Img2','Img3','vdM2','vdM3','vdM4']
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

text_values=['vdM1','vdM2','Img1','Img2','VdM3','VdM4']
#text_values=['VdM1','VdM2','VdM3','VdM4']
BCIDS = [42,281,872,1783,2063]

"""
###############2022(8381)###########################

scans = ['8381_11Nov22_004152_11Nov22_010424',
        '8381_11Nov22_012659_11Nov22_015155',
        '8381_11Nov22_021738_11Nov22_024314',
        '8381_11Nov22_095321_11Nov22_101617',
        '8381_11Nov22_114759_11Nov22_121408']
#'8381_11Nov22_081031_11Nov22_083511',

text_values=['VdM1','Img1','Img2','VdM3','VdM4']
BCIDS = [282, 822, 2944, 3123, 3302]
#####################################################                                                                 

#######################2023(8999)#####################                                                                
"""
                                                   
scans = ["8999_28Jun23_000143_28Jun23_232943", #vdM1  #230143                
         "8999_29Jun23_004658_29Jun23_011220", #BI1                    
         "8999_29Jun23_013851_29Jun23_020425", #BI2           
         "8999_29Jun23_023227_29Jun23_025502", #vdM2   
         "8999_29Jun23_073830_29Jun23_080352", #vdM3    
         "8999_29Jun23_092415_29Jun23_094738", #vdM4              
         "8999_29Jun23_110004_29Jun23_112226", #vdM5
         "8999_29Jun23_123257_29Jun23_125514"] #vdM6                             
                                                                           
text_values=['vdM1','BI1','BI2','vdM2','VdM3','VdM4','VdM5','vdM6']
BCIDS = [208, 282, 548, 822, 1197, 1376, 2716, 2944, 3123]  
#text_values=['vdM1','vdM2','VdM3','VdM4','VdM5']
""" 
filll=8381
year=2022
a = np.arange(1,len(text_values)*len(BCIDS)*2+1,1)                                
x0 = np.arange(1,len(text_values)*len(BCIDS)+1,1)
#print(x0)
x0_covX = []
x0_covY = []

count = 0
for l in a:
    count +=1
    if count <= len(BCIDS):x0_covX.append(l)
    else: x0_covY.append(l)
    if count == len(BCIDS)*2:count=0


data_columns = ["CapSigma","peak","chi2","covStatus","xsec"]
#Xscan  = np.array([])                                                                             
#Yscan  = np.array([])  
for col in data_columns:
    #Xsxan  = np.array([])                                                                                           
    #Yscan = np.array([])
    Xscan  = []
    Yscan  = []
    for scan in scans:
        if col =="xsec":
            fullpath=path+"/"+scan+"/"+det+"/results/"+corrTemp+"/"+"LumiCalibration_pcchd5_"+fit+"_"+fill+".csv"
            data=pd.read_csv(fullpath)                                                                 
            for i,j in zip(np.asarray(data.xsec),np.asarray(data.xsecErr)):
                Xscan.append(i)
                Yscan.append(j)
        else: 
            fullpath=path+"/"+scan+"/"+det+"/results/"+corrTemp+"/"+fit+"_FitResults.csv"
            data=pd.read_csv(fullpath)
            for i,j,k in zip(np.asarray(data.Type),np.asarray(data[col]),np.asarray(data.ndof)):
                if i == 'X':
                    if col == "chi2":
                        Xscan.append(j/k)
                    else:
                        Xscan.append(j)
                else:
                    if col == "chi2":
                        Yscan.append(j/k)      
                    else:
                        Yscan.append(j)

    if col =="covStatus":
        if len(text_values)>1:
            x1=len(BCIDS)*2+.5
            x2=len(BCIDS)*4+.5
            plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0)
        if len(text_values)>3:
            x1=len(BCIDS)*6+.5
            x2=len(BCIDS)*8+.5
            plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade
        if len(text_values)>5:
            x1=len(BCIDS)*10+.5                                                             
            x2=len(BCIDS)*12+.5                                                                                         
            plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0)
        if len(text_values)>7:
            x1=len(BCIDS)*14+.5                                              
            x2=len(BCIDS)*16+.5                   
            plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0)
    else:
        if len(text_values)>1:
            x1=len(BCIDS)+.5                          
            x2=len(BCIDS)*2+.5                                        
            plt.axvspan(x1, x2, color='gray', alpha=0.65, lw=0) 
        if len(text_values)>3 :   
            x1=len(BCIDS)*3+.5
            x2=len(BCIDS)*4+.5
            plt.axvspan(x1, x2, color='gray', alpha=0.65, lw=0) #shade
        if len(text_values)>5:    
            x1=len(BCIDS)*5+.5   
            x2=len(BCIDS)*6+.5                         
            plt.axvspan(x1, x2, color='gray', alpha=0.65, lw=0)
        if len(text_values)>7:    
            x1=len(BCIDS)*7+.5                                                
            x2=len(BCIDS)*8+.5                                               
            plt.axvspan(x1, x2, color='gray', alpha=0.65, lw=0) 
        
    if col == "covStatus":
        #print(len(x0_covX),len(Xscan))
       # print(x0_covX,Xscan)
        plt.errorbar(x0_covX, Xscan, fmt='o', color = 'red',markersize=12,label = 'X-Scans')                          
        plt.errorbar(x0_covY, Yscan, fmt='o', color = 'blue',markersize=12,label = 'Y-Scans')
        
    elif col == "xsec":
        
        colors_base = ['red', 'blue', 'green', 'orange', 'purple','yellow','black','magenta','brown']
        colors = np.tile(colors_base[:len(BCIDS)], len(text_values))


        for i in range(len(x0)):
            plt.errorbar(x0[i], Xscan[i]/1000, yerr=Yscan[i]/1000, fmt='o', color = colors[i],markersize=14)
            for j in range(len(BCIDS)):
                if i==j:
                    plt.errorbar(x0[i], Xscan[i]/1000, yerr=Yscan[i]/1000, fmt='o', color = colors[i],markersize=14,label="BCID "+str(BCIDS[j]),markeredgewidth=1)
        
        spread = round((np.std(Xscan)/np.mean(Xscan))*100,3) 
       # plt.title('SigmaVis [$mb$]',pad = 30,fontsize =40)
        rms = np.std(Xscan)/1000
        mean  = np.mean(Xscan)/1000
        sem = np.std(Xscan)/np.sqrt(len(Xscan))        
       # print("SEM:",np.std(Xscan)/np.sqrt(25))
        plt.axhline(mean , color = 'black', lw = 4,ls="--")
       # plt.text(0.05, 0.95, 'Label arriba izquierda', transform=plt.gca().transAxes, ha='left', va='top')
        plt.legend(loc='lower left',bbox_to_anchor=(0, 1))  
       # plt.figtext(x=0.895, y=0.89, ha="right", va="top", fontsize=25,                                     
        #            s="-- MEAN = %.2f mb\n  RMS = %.2f mb" %(mean,rms),                          
         #                       backgroundcolor="white").set_bbox(dict(boxstyle='round', facecolor='white', edgecolor='black', linewidth=2))
        #plt.figtext(x=0.895, y=0.89, ha="right", va="top", fontsize=25, s='Std(BunchSpread): \n'+ str(round(np.std(Xscan)/1000,3))+' mb  ' + '('+ str(spread) +'%)',backgroundcolor="white").set_bbox(dict(color='w', alpha=1))
       # print("std:",np.std(Xscan))
       # print("SEM:",np.std(Xscan)/np.sqrt(25))        
   # elif col == "peak":
    #    plt.errorbar(x0, np.array(Xscan)/np.array(Yscan), fmt='o', color = 'red',markersize=12,label = 'Xpeak/Ypeak')
   # elif col == "CapSigma":
#        Xscan_ = np.array(Xscan)
 #       Yscan_ = np.array(Yscan)
  #      print(x0,Xscan_,Yscan_)
      #  plt.errorbar(x0, np.array(Xscan)*np.array(Yscan), fmt='o', color = 'blue',markersize=13,label = 'CapX*CapY')
    else:
       # print(x0,Xscan)
       # print(col,len(x0),len(Xscan))
       # if  col == "CapSigma":
       # print(len(x0),len(Xscan))
           # plt.errorbar(x0, Xscan*Yscan, fmt='o', color = 'red',markersize=12,label = 'CapX*CapY')
        plt.errorbar(x0, Xscan, fmt='o', color = 'red',markersize=12,label = 'X-Scans')
        plt.errorbar(x0, Yscan, fmt='o', color = 'blue',markersize=12,label = 'Y-Scans')
       # print(col,len(x0),len(Xscan))
    if col == "chi2":
        avg = round(np.mean(Xscan+Yscan),5)
        plt.axhline(avg , color = 'green', lw = 4, ls='--',label=avg)
        plt.title('Fit Quality ('+col+'/ndof)',pad = 30,fontsize =40)
        plt.ylabel(col+'/ndof',fontsize = 30);
        plt.figtext(x=0.25, y=0.04, ha="right", va="top", fontsize=35, s="mean="+str(avg),backgroundcolor="white").set_bbox(dict(color='w', alpha=1))
    elif col == "Const":                                                                            
        avg_x = round(np.mean(Xscan),5)
        avg_y = round(np.mean(Yscan),5)                           
        plt.title(col,pad = 30,fontsize =40)                                            
        plt.ylabel(col,fontsize = 30);                                                                     
       # plt.ylim(0.20,0.60)
        plt.figtext(x=0.895, y=0.875, ha="right", va="top", fontsize=30, s="Avg_X = "+str(avg_x)).set_bbox(dict(color='w', alpha=1))
        plt.figtext(x=0.895, y=0.845, ha="right", va="top", fontsize=30, s="Avg_Y = "+str(avg_y)).set_bbox(dict(color='w', alpha=1))    
    elif col =="xsec":
        plt.legend(loc =4,scatterpoints =1,fontsize = 20); 
      #  plt.title('SigmaVis [$mb$ ]',pad = 30,fontsize =40) 
        plt.ylabel('$\sigma_{vis}$ [$mb$]',fontsize = 35);
    else:
        plt.title(col , pad = 30,fontsize =40)
        plt.ylabel(col,fontsize = 30);
       # plt.legend(loc ='upper left',scatterpoints =1,fontsize = 20);
    plt.xlabel('Scan',labelpad =35, fontsize =30)
   # plt.legend(loc =4,scatterpoints =1,fontsize = 20);
    plt.tick_params(axis='x',labelbottom = True,pad = 10 ,direction = 'in', length =1, labelsize=15)
    j = np.arange(len(BCIDS)/2,len(text_values)*len(BCIDS)+1,len(BCIDS))       
    
    if col == "covStatus":
        plt.xticks(np.arange(9,len(text_values)*len(BCIDS)*2+1,len(BCIDS)*2), text_values , fontsize = 23)
        plt.xlim(0,len(text_values)*len(BCIDS)*2+1)
        plt.ylim(0,4)        
    else:
        plt.xticks(j, text_values , fontsize = 23)
        plt.xlim(0,len(text_values)*len(BCIDS)+1)
    #hep.cms.text("Work in Progress", loc=0,fontsize=30)
    
                                                                
    plt.figtext(.14, 0.84, "CMS", fontsize=30, fontweight="bold", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))
    plt.figtext(.14 +0.05, 0.84, "Work in progress", fontsize=30, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))                                                                       
    plt.figtext(.14 , 0.80, "PCC Luminometer", fontsize=30, style="italic", backgroundcolor="white", ha="left").set_bbox(dict(alpha=0.0))                                                                                                             
   
 
   # plt.text(0.98, 1.01, "Fill {:d} ({:d}, $\sqrt{s} = 13 TeV$)".format(filll, year),  
    plt.text(0.98, 1.01, "Fill"+" "+str(filll)+"   "+str(year)+"("+"$\sqrt{s}$ = 13 TeV)",              
                horizontalalignment='right',                                                                                                  verticalalignment='bottom',                                                                                  
                transform=trans,#plt.transAxes,                                                                            
                fontname='sans-serif',                                                                                                        color="k",                                                                                                   
                fontsize=30)
  
    if col=='CapSigma':                           
        plt.ylim(.100,.135)
        plt.legend(loc =1,scatterpoints =1,fontsize = 20);
#plt.legend(loc=1,bbox_to_anchor=(0, 1))
    elif col=='peak':
        plt.ylim(13.5,18.5)  
        plt.legend(loc =1,scatterpoints =1,fontsize = 20)
    elif col=='Const': 
        plt.ylim(0,.040)
        plt.legend(loc =3,scatterpoints =1,fontsize = 20)
    elif col=='chi2':
        plt.ylim(0,3.5)
        plt.legend(loc =1,scatterpoints =1,fontsize = 20)
    elif col=='xsec':
        plt.ylim(1260,1345) 
        plt.legend(loc =1,scatterpoints =1,fontsize = 20)
    else:
        print("hola")
    plt.yticks(fontsize=23)
    plt.ticklabel_format(useOffset=False, axis='y')  # Prevent plt from adding offset to y axis
    fig = plt.gcf()
    fig.set_size_inches(25, 14)
    plt.grid(0)
    plt.savefig(str(outpath)+str(fit)+'_'+col+'.png', dpi=500, bbox_inches = None)
    Xscan = []                                            
    Yscan = []
    plt.clf()
