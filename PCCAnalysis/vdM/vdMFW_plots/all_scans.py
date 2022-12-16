### Important note ####
#Code used within the vdM framework in  branch 'auto_analyses_17_18' (also 'even_more_fits' and 'pcc_fits' branches); usually run from /AnalysisVisualisationTools/Sxec/
#It should work in other branches
#Before running this code it is necessary to run 'source cmsenv.sh'
#######################

import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt
mpl.use("Agg")
mpl.style.use('classic')

#Input path
path="/eos/user/j/jmorenoc/Results_v1/NewVeto/Period_B/Tests_Poly246G/No_Limits_Poly246G_SuperG/Output/Analysed_Data"

#Output path:
outpath='/eos/user/j/jmorenoc/thesis_plots/'
#--------------------------------------------#

fit='Poly2G'

#Corrections
corrTemp='Background_OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta_LengthScale_PeakToPeak'

#Text positions (vdM1, img1,...,vdM4):
y_text=9030000

#Detector:
det='PLT' #PLT is PCC in this context


x=np.asarray([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35])

text_values=['','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','']


error=np.zeros(35)

data_column='xsec'

data_column_err='xsecErr'


#***************** vdM1  **************************
fill_name='6868_30Jun18_132834_30Jun18_135235'

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+"LumiCalibration_"+det+"_"+fit+"_6868.csv"


print("!-------------------vdM1-----------------------!")
print("Data Path:", fullpath)


data=pd.read_csv(fullpath)


##attX = np.asarray(data[data['Type']=='X']['chi2'])
##attY=np.asarray(data[data['Type']=='Y']['chi2'])
##con=np.concatenate((attX,attY))
##print("con",con)

array_BaseScan=np.asarray(data[data_column])
print("vdM1 xsec Array:",array_BaseScan)
print("vdM1 xsec Mean :",np.mean(array_BaseScan) )
print("vdM1 xsec Std :",np.std(array_BaseScan) )


array_BaseScan_err=np.asarray(data[data_column_err])

#print("array_BaseScan",array_BaseScan)


#for sigma_vis
#+"LumiCalibration_"+det+"_"+fit+"_6868.csv"


# +fit+"_FitResults.csv

#*****************------img1 DATA--------*******************#

fill_name="6868_30Jun18_151040_30Jun18_153416"

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+"LumiCalibration_"+det+"_"+fit+"_6868.csv"

print("!-------------------img1-----------------------!")
print("Data Path:", fullpath)

#Open the data and gettin next scan
data=pd.read_csv(fullpath)

array_NextScan=np.asarray(data[data_column])

print("img1 xsec Array:",array_NextScan)
print("img1 xsec Mean :",np.mean(array_NextScan) )
print("img1 xsec Std :",np.std(array_NextScan) )



#print("array_NextScan",array_NextScan)
array_BaseScan=np.concatenate((array_BaseScan,array_NextScan))


array_NextScan_err=np.asarray(data[data_column_err])
array_BaseScan_err=np.concatenate((array_BaseScan_err,array_NextScan_err))

#************* img_2 DATA*****************+

fill_name="6868_01Jul18_002854_01Jul18_005345"

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+"LumiCalibration_"+det+"_"+fit+"_6868.csv"

print("!-------------------img2-----------------------!")
print("Data Path:", fullpath)

#Open the data and gettin next scan
data=pd.read_csv(fullpath)

array_NextScan=np.asarray(data[data_column])

print("img2 xsec Array:",array_NextScan)
print("img2 xsec Mean :",np.mean(array_NextScan) )
print("img2 xsec Std :",np.std(array_NextScan) )


#print("array_NextScan",array_NextScan)
array_BaseScan=np.concatenate((array_BaseScan,array_NextScan))

array_NextScan_err=np.asarray(data[data_column_err])
array_BaseScan_err=np.concatenate((array_BaseScan_err,array_NextScan_err))


#************* img_3 DATA*****************+

fill_name="6868_01Jul18_012042_01Jul18_014645"

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+"LumiCalibration_"+det+"_"+fit+"_6868.csv"

print("!-------------------img3-----------------------!")
print("Data Path:", fullpath)

#Open the data and gettin next scan
data=pd.read_csv(fullpath)

array_NextScan=np.asarray(data[data_column])
#print("array_NextScan",array_NextScan)
print("img3 xsec Array:",array_NextScan)
print("img3 xsec Mean :",np.mean(array_NextScan) )
print("img3 xsec Std :",np.std(array_NextScan) )
array_BaseScan=np.concatenate((array_BaseScan,array_NextScan))

array_NextScan_err=np.asarray(data[data_column_err])
array_BaseScan_err=np.concatenate((array_BaseScan_err,array_NextScan_err))


#*************** vdM 2 DATA************************+
fill_name="6868_01Jul18_030039_01Jul18_032641"

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+"LumiCalibration_"+det+"_"+fit+"_6868.csv"

print("!-------------------vdM2-----------------------!")
print("Data Path:", fullpath)

#Open the data and gettin next scan
data=pd.read_csv(fullpath)

array_NextScan=np.asarray(data[data_column])
#print("array_NextScan",array_NextScan)
print("vdM2 xsec Array:",array_NextScan)
print("vdM2 xsec Mean :",np.mean(array_NextScan) )
print("vdM2 xsec Std :",np.std(array_NextScan) )
array_BaseScan=np.concatenate((array_BaseScan,array_NextScan))

array_NextScan_err=np.asarray(data[data_column_err])
array_BaseScan_err=np.concatenate((array_BaseScan_err,array_NextScan_err))


#*************** vdM 3 DATA************************+
fill_name="6868_01Jul18_035146_01Jul18_041104"

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+"LumiCalibration_"+det+"_"+fit+"_6868.csv"

print("!-------------------vdM3-----------------------!")
print("Data Path:", fullpath)

#Open the data and gettin next scan
data=pd.read_csv(fullpath)

array_NextScan=np.asarray(data[data_column])
#print("array_NextScan",array_NextScan)
print("vdM3 xsec Array:",array_NextScan)
print("vdM3 xsec Mean :",np.mean(array_NextScan) )
print("vdM3 xsec Std :",np.std(array_NextScan) )
array_BaseScan=np.concatenate((array_BaseScan,array_NextScan))

array_NextScan_err=np.asarray(data[data_column_err])
array_BaseScan_err=np.concatenate((array_BaseScan_err,array_NextScan_err))


#*************** vdM 4 DATA************************+
fill_name="6868_01Jul18_075134_01Jul18_081132"

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+"LumiCalibration_"+det+"_"+fit+"_6868.csv"

print("!-------------------vdM4-----------------------!")
print("Data Path:", fullpath)

#Open the data and gettin next scan
data=pd.read_csv(fullpath)

array_NextScan=np.asarray(data[data_column])
#print("array_NextScan",array_NextScan)
print("vdM4 xsec Array:",array_NextScan)
print("vdM4 xsec Mean :",np.mean(array_NextScan) )
print("vdM4 xsec Std :",np.std(array_NextScan) )
array_BaseScan=np.concatenate((array_BaseScan,array_NextScan))

array_NextScan_err=np.asarray(data[data_column_err])
array_BaseScan_err=np.concatenate((array_BaseScan_err,array_NextScan_err))



##############to plot#############
y=array_BaseScan
print("********************************************")
print("y",y)

y_error=array_BaseScan_err
print("y_error",y_error)

print("Y_mean: ",np.mean(y), " Std: ",np.std(y))

fake_y=y
for i in range(len(y)):
 #print(i)
 if y[i]<=1374100:
     #print("small xsec!",i)
     fake_y[i]=0.0
#print("fake Y:",fake_y)
fake_sum=0

for i in range(len(fake_y)):
  fake_sum= fake_sum+fake_y[i]

y_array_small= fake_y

index=[3,18]
y_array_small_b=np.delete(y_array_small,index)

#print("y_array_small_b MEAN:",np.mean(y_array_small_b))
#print("y_array_small_b STD",np.std(y_array_small_b))


plt.errorbar(x, y, yerr=y_error, fmt='o', color = 'red',markersize=16 )
plt.grid(0)




#********************************************************************
#******************** For Plotting ***********************************
#*********************************************************************

#*****************Settingscan names and scan numbers******************


#-----------vdM1 shade----------------
x1=0
x2=5.5

plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade

plt.text(2,y_text, "vdM1",color="black",fontsize=30) #Scan Name

#----------------img1---------------------

#No shade 

plt.text(7,y_text, "img1",color="black",fontsize=30) #Scan name


#-----------img2 shade----------------
x1=10.5
x2=15.5

plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade

plt.text(12,y_text, "img2",color="black",fontsize=30) #Scan Name

#----------------img3---------------------

#No shade 

plt.text(17,y_text, "img3",color="black",fontsize=30) #Scan name


#-----------vdM2 shade----------------
x1=20.5
x2=25.5

plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade

plt.text(22.5,y_text, "vdM2",color="black",fontsize=30) #Scan Name

#----------------vdM3---------------------

#No shade 

plt.text(26.8,y_text, "vdM3",color="black",fontsize=30) #Scan name


#-----------vdM4 shade----------------
x1=30.5
x2=35.5


plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade

plt.text(31.8,y_text, "vdM4",color="black",fontsize=30) #Scan Name



#--------------- Last figure settings------------#

#plt.xticks(np.arange(np.min(x)-1, np.max(x)+1, 5))
plt.xticks(np.arange(np.min(x)-1, np.max(x)+1, 1), text_values)

plt.xticks(fontsize=23)
plt.yticks(fontsize=23)

#plt.xlabel('Bunch ID', fontsize=29)
plt.ylabel('xsec [$\mu b$ ]', fontsize=29)
plt.ticklabel_format(useOffset=False, axis='y')  # Prevent plt from adding offset to y axis

plt.xlim(0,37)
plt.ylim(9045000,9400000)

fig = plt.gcf()
fig.set_size_inches(27, 14)

plt.savefig(str(outpath)+str(fit)+'_xsec.png', dpi=500, bbox_inches='tight')
