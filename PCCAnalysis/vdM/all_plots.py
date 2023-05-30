import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt

mpl.use("Agg")
mpl.style.use('classic')

path="/eos/user/l/lcuevasp/2022_Analisys/output/analysed_data"
outpath='/eos/user/l/lcuevasp/2022_Analisys/output/plots/'
#path="/eos/user/l/lcuevasp/2017_Analisys/output_2017_newdata/analysed_data"    
#outpath='/eos/user/l/lcuevasp/2017_Analisys/output_2017_newdata/plots/' 

#corrTemp="noCorr"
corrTemp = "OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta_LengthScale"
#'Background_OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta_LengthScale_PeakToPeak'
#OutPathPlots='/eos/user/j/jmorenoc/thesis_plots/'
fill = "8381"
fit='DGConst'
det="pcchd5"

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
###############2022(8183)###########################

scans = ['8381_11Nov22_004152_11Nov22_010424',
        '8381_11Nov22_012659_11Nov22_015155',
        '8381_11Nov22_021738_11Nov22_024314',
        '8381_11Nov22_095321_11Nov22_101617',
        '8381_11Nov22_114759_11Nov22_121408']
#'8381_11Nov22_081031_11Nov22_083511',

text_values=['VdM1','Img1','Img2','VdM3','VdM4']

#####################################################                                                                 

a = np.arange(1,len(text_values)*10+1,1)                                
x0 = np.arange(1,len(text_values)*5+1,1)
x0_covX = []
x0_covY = []

count = 0
for l in a:
    count +=1
    if count <= 5:x0_covX.append(l)
    else: x0_covY.append(l)
    if count == 10:count=0


data_columns = ["CapSigma","peak","chi2","covStatus","xsec","Const"]

for col in data_columns:
    Xscan = []                                                                                             
    Yscan = []
    for scan in scans:
        if col =="xsec":
            fullpath=path+"/"+scan+"/"+det+"/results/"+corrTemp+"/"+"LumiCalibration_"+det+"_"+fit+"_"+fill+".csv"
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
        x1=10.5                   
        x2=20.5                                                                                                     
        plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0)
        x1=30.5
        x2=40.5
        plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade
       # x1=50.5                                                                                                      
        #x2=60.5                                                                                                      
       # plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0)
    
    else:
        x1=5.5                          
        x2=10.5                                        
        plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) 
        x1=15.5
        x2=20.5
        plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade
        #x1=25.5   
      #  x2=30.5                         
       # plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0)

    if col == "covStatus":
        #print(len(x0_covX),len(Xscan))
        plt.errorbar(x0_covX, Xscan, fmt='o', color = 'red',markersize=12,label = 'X-Scans')                          
        plt.errorbar(x0_covY, Yscan, fmt='o', color = 'blue',markersize=12,label = 'Y-Scans')
        
    elif col == "xsec":
        plt.errorbar(x0, Xscan, yerr=Yscan, fmt='o', color = 'black',markersize=15,label = "xsec")
        spread = round((np.std(Xscan)/np.mean(Xscan))*100,3) 
        plt.title('SigmaVis [$\mu b$ ]',pad = 30,fontsize =40)
        rms = np.std(Xscan)
        mean  = np.mean(Xscan)
        sem = np.std(Xscan)/np.sqrt(len(Xscan))        
        print("SEM:",np.std(Xscan)/np.sqrt(25))  
        plt.figtext(x=0.895, y=0.89, ha="right", va="top", fontsize=25,                                     
                    s="MEAN = %.2f\nSEM  = %.2f\nRMS = %.2f" %(mean,sem,rms),                          
                                backgroundcolor="white").set_bbox(dict(color='w', alpha=1))
        #plt.figtext(x=0.895, y=0.89, ha="right", va="top", fontsize=25, s='Std(BunchSpread): \n'+ str(round(np.std(Xscan)/1000,3))+' mb  ' + '('+ str(spread) +'%)',backgroundcolor="white").set_bbox(dict(color='w', alpha=1))
        print("std:",np.std(Xscan))
        print("SEM:",np.std(Xscan)/np.sqrt(25))        
    else:
        print(col,len(x0),len(Xscan))
        plt.errorbar(x0, Xscan, fmt='o', color = 'red',markersize=12,label = 'X-Scans')
        plt.errorbar(x0, Yscan, fmt='o', color = 'blue',markersize=12,label = 'Y-Scans')
        print(col,len(x0),len(Xscan))
    if col == "chi2":
        avg = round(np.mean(Xscan+Yscan),5)
        plt.axhline(avg , color = 'green', lw = 4)
        plt.title('Fit Quality ('+col+'/ndof)',pad = 30,fontsize =40)
        plt.ylabel(col+'/ndof',fontsize = 30);
        plt.figtext(x=0.89, y=0.04, ha="right", va="top", fontsize=35, s="Avg="+str(avg),backgroundcolor="white").set_bbox(dict(color='w', alpha=1))
    elif col == "Const":                                                                            
        avg_x = round(np.mean(Xscan),5)
        avg_y = round(np.mean(Yscan),5)                           
        plt.title(col,pad = 30,fontsize =40)                                            
        plt.ylabel(col,fontsize = 30);                                                                
        plt.figtext(x=0.895, y=0.895, ha="right", va="top", fontsize=30, s="Avg_X = "+str(avg_x),backgroundcolor="white").set_bbox(dict(color='w', alpha=1))
        plt.figtext(x=0.895, y=0.865, ha="right", va="top", fontsize=30, s="Avg_Y = "+str(avg_y),backgroundcolor="white").set_bbox(dict(color='w', alpha=1))    
    elif col =="xsec":
        plt.title('SigmaVis [$\mu b$ ]',pad = 30,fontsize =40) 
        plt.ylabel('xsec [$\mu b$ ]',fontsize = 30);
    else:
        plt.title(col , pad = 30,fontsize =40)
        plt.ylabel(col,fontsize = 30);

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
 
    plt.yticks(fontsize=23)
    plt.ticklabel_format(useOffset=False, axis='y')  # Prevent plt from adding offset to y axis
    fig = plt.gcf()
    fig.set_size_inches(25, 14)
    plt.grid(0)
    plt.savefig(str(outpath)+str(fit)+'_'+col+'.png', dpi=500, bbox_inches = None)
    Xscan = []                                            
    Yscan = []
    plt.clf()
