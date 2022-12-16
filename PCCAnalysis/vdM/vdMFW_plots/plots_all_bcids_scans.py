import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt
mpl.use("Agg")
mpl.style.use('classic')

#---------------------------------------- SETINGS -------------------------------------------#

##Inputh path:
path="/eos/user/j/jmorenoc/Results_v1/NewVeto/Period_B/Tests_Poly246G/No_Limits_Poly246G_SuperG/Output/Analysed_Data"

##Corrections:
corrTemp='Background_OrbitDriftSep_OrbitDriftRate_BeamBeam_DynamicBeta_LengthScale_PeakToPeak'

##Output path:
OutPathPlots='/eos/user/j/jmorenoc/thesis_plots/'

##X label : text position (vdM1, img1, ..., vdM4): 
y_text=84
##See line 54 to add text in x-axis (in each point)

##Fit model:
fit='Poly2G'

##Fit results column to read:
data_column='peak'
#data columns:

##Set var_error: 
#1: yes (peak,CapSigma,...); 
#0: no (covStatus, chi2/ndof)
var_error=1

##Set yet_limits=1 for manual limits in Y axis:
y_limits=0
#Y axis limits_
y_lim1=-0.003
y_lim2=0.005

#------------------------------------------------------------------------#

if var_error==1:
 data_column_err=str(data_column)+'Err'
else:
 data_column_err='peakErr'

print("data_column_err",data_column_err)

#Horizontal axis array:
x=np.asarray([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70]) 


#---------------------- X axis text values--------------------------------#

##To add x axis text values set as 1

x_txt_val=1

#text_values=['265','865','1780','2192','3380','265','865','1780','2192','3380','265','865','1780','2192','3380','265','865','1780','2192','3380','265','865','1780','2192','3380','265','865','1780','2192','3380','265','865','1780','2192','3380','265','865','1780','2192','3380','265','865','1780','2192','3380','265','865','1780','2192','3380','265','865','1780','2192','3380','265','865','1780','2192','3380','265','865','1780','2192','3380','265','865','1780','2192','3380']

text_values=['','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','']

#-----------------------------------------------------------------------------#

#Array to assign errors:
error=np.zeros(70)


#-------------------------Setting scan names text-----------------------------------#

#####vdM1
fill_name='6868_30Jun18_132834_30Jun18_135235'

det='PLT'

data_column_ndof='ndof'


fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+fit+"_FitResults.csv"

data=pd.read_csv(fullpath)


##attX = np.asarray(data[data['Type']=='X']['chi2'])
##attY=np.asarray(data[data['Type']=='Y']['chi2'])
##con=np.concatenate((attX,attY))
##print("con",con)

array_BaseScan=np.asarray(data[data_column])

array_BaseScan_err=np.asarray(data[data_column_err])

array_BaseScan_ndof=np.asarray(data[data_column_ndof])


print("array_BaseScan",array_BaseScan)


#*****************------img1 DATA--------*******************#

fill_name="6868_30Jun18_151040_30Jun18_153416"

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+fit+"_FitResults.csv"


#Open the data and gettin next scan
data=pd.read_csv(fullpath)

array_NextScan=np.asarray(data[data_column])


print("array_NextScan",array_NextScan)
array_BaseScan=np.concatenate((array_BaseScan,array_NextScan))


array_NextScan_err=np.asarray(data[data_column_err])
array_BaseScan_err=np.concatenate((array_BaseScan_err,array_NextScan_err))


array_NextScan_ndof=np.asarray(data[data_column_ndof])
array_BaseScan_ndof=np.concatenate((array_BaseScan_ndof,array_NextScan_ndof))

#************* img_2 DATA*****************+

fill_name="6868_01Jul18_002854_01Jul18_005345"

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+fit+"_FitResults.csv"


#Open the data and gettin next scan
data=pd.read_csv(fullpath)

array_NextScan=np.asarray(data[data_column])
print("array_NextScan",array_NextScan)
array_BaseScan=np.concatenate((array_BaseScan,array_NextScan))

array_NextScan_err=np.asarray(data[data_column_err])
array_BaseScan_err=np.concatenate((array_BaseScan_err,array_NextScan_err))


array_NextScan_ndof=np.asarray(data[data_column_ndof])
array_BaseScan_ndof=np.concatenate((array_BaseScan_ndof,array_NextScan_ndof))

#************* img_3 DATA*****************+

fill_name="6868_01Jul18_012042_01Jul18_014645"

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+fit+"_FitResults.csv"


#Open the data and gettin next scan
data=pd.read_csv(fullpath)

array_NextScan=np.asarray(data[data_column])
print("array_NextScan",array_NextScan)
array_BaseScan=np.concatenate((array_BaseScan,array_NextScan))

array_NextScan_err=np.asarray(data[data_column_err])
array_BaseScan_err=np.concatenate((array_BaseScan_err,array_NextScan_err))

array_NextScan_ndof=np.asarray(data[data_column_ndof])
array_BaseScan_ndof=np.concatenate((array_BaseScan_ndof,array_NextScan_ndof))

#*************** vdM 2 DATA************************+
fill_name="6868_01Jul18_030039_01Jul18_032641"

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+fit+"_FitResults.csv"


#Open the data and gettin next scan
data=pd.read_csv(fullpath)

array_NextScan=np.asarray(data[data_column])
print("array_NextScan",array_NextScan)
array_BaseScan=np.concatenate((array_BaseScan,array_NextScan))

array_NextScan_err=np.asarray(data[data_column_err])
array_BaseScan_err=np.concatenate((array_BaseScan_err,array_NextScan_err))

array_NextScan_ndof=np.asarray(data[data_column_ndof])
array_BaseScan_ndof=np.concatenate((array_BaseScan_ndof,array_NextScan_ndof))

#*************** vdM 3 DATA************************+
fill_name="6868_01Jul18_035146_01Jul18_041104"

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+fit+"_FitResults.csv"


#Open the data and gettin next scan
data=pd.read_csv(fullpath)

array_NextScan=np.asarray(data[data_column])
print("array_NextScan",array_NextScan)
array_BaseScan=np.concatenate((array_BaseScan,array_NextScan))

array_NextScan_err=np.asarray(data[data_column_err])
array_BaseScan_err=np.concatenate((array_BaseScan_err,array_NextScan_err))

array_NextScan_ndof=np.asarray(data[data_column_ndof])
array_BaseScan_ndof=np.concatenate((array_BaseScan_ndof,array_NextScan_ndof))

#*************** vdM 4 DATA************************+
fill_name="6868_01Jul18_075134_01Jul18_081132"

fullpath=path+"/"+fill_name+"/"+det+"/results/"+corrTemp+"/"+fit+"_FitResults.csv"


#Open the data and gettin next scan
data=pd.read_csv(fullpath)

array_NextScan=np.asarray(data[data_column])
print("array_NextScan",array_NextScan)
array_BaseScan=np.concatenate((array_BaseScan,array_NextScan))

array_NextScan_err=np.asarray(data[data_column_err])
array_BaseScan_err=np.concatenate((array_BaseScan_err,array_NextScan_err))


array_NextScan_ndof=np.asarray(data[data_column_ndof])
array_BaseScan_ndof=np.concatenate((array_BaseScan_ndof,array_NextScan_ndof))


#----------------------------------------------------------------------------------------#


####################################to plot#####################################3
y=array_BaseScan

y_2=array_BaseScan_ndof
print("********************************************")
print("y",y)

y_error=array_BaseScan_err
print("y_error",y_error)

print("types:")
print("x:",type(x))
print("y:",type(y))
print("y_err:",type(y_error))

#chi2/ndof= y/y_2
if data_column=='chi2':
 print("chi2/ndof= ",y/y_2)
 print("chi2 Mean= ",np.mean(y/y_2))
 plt.errorbar(x, y/y_2, yerr=error, fmt='o', color = 'red',markersize=12 )
 plt.grid(0)

count_cs3=0
count_cs2=0
count_cs1=0
if data_column=='covStatus':
 plt.errorbar(x, y, yerr=error, fmt='o', color = 'red',markersize=12 )
 plt.grid(0)
 count_cs3=0
 count_cs2=0
 count_cs1=0
 for i in range(70):
  if y[i]==int(3):
   count_cs3+=1
  if y[i]==int(2):
   count_cs2+=1
  if y[i]==int(1):
   count_cs1+=1
  #print("i: ",i)


if var_error==1:
 plt.errorbar(x, y, yerr=y_error, fmt='o', color = 'red',markersize=12 )
 plt.grid(0)


print("# covStatus=3 : ",count_cs3," ; ",(count_cs3*100)/70.0,"%" )
print("# covStatus=2 : ",count_cs2," ; ",(count_cs2*100)/70.0,"%" )
print("# covStatus=1 : ",count_cs1,"  ; ",(count_cs1*100)/70.0,"%" )

print("Sanity check for CovStatus # of data: ",count_cs3+count_cs2+count_cs1)

#********************************************************************
#******************** For Plotting ***********************************
#*********************************************************************

#*****************Settingscan names and scan numbers******************

maxy=np.max(y)
miny=np.min(y)


#-----------vdM1 shade----------------
x1=0
x2=10.5

plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade

plt.text(4.8,y_text, "vdM1",color="black",fontsize=30) #Scan Name

plt.axvline(x = 5.25, color = 'blue') #Scan number line (X,Y Scan division)

#----------------img1---------------------

#No shade 

plt.text(14.8,y_text, "img1",color="black",fontsize=30) #Scan name

plt.axvline(x = 15.25, color = 'blue') #Scan number line division



#-----------img2 shade----------------
x1=20.5
x2=30.5

plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade

plt.text(24.8,y_text, "img2",color="black",fontsize=30) #Scan Name

plt.axvline(x = 25.25, color = 'blue') #Scan number line (X,Y Scan division)


#----------------img3---------------------

#No shade 

plt.text(34.8,y_text, "img3",color="black",fontsize=30) #Scan name

plt.axvline(x = 35.25, color = 'blue') #Scan number line division


#-----------vdM2 shade----------------
x1=40.5
x2=50.5

plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade

plt.text(44.8,y_text, "vdM2",color="black",fontsize=30) #Scan Name

plt.axvline(x = 45.25, color = 'blue') #Scan number line (X,Y Scan division)


#----------------vdM3---------------------

#No shade 

plt.text(54.8,y_text, "vdM3",color="black",fontsize=30) #Scan name

plt.axvline(x = 55.25, color = 'blue') #Scan number line division

#-----------vdM4 shade----------------
x1=60.5
x2=70.5


plt.axvspan(x1, x2, color='gray', alpha=0.75, lw=0) #shade

plt.text(64.8,y_text, "vdM4",color="black",fontsize=30) #Scan Name

plt.axvline(x = 65.25, color = 'blue') #Scan number line (X,Y Scan division)

#--------------- Last figure settings

#adding text values in X:
if x_txt_val==1:
 plt.xticks(np.arange(np.min(x), np.max(x), 5), text_values)   #IMPORTANT
else:
 plt.xticks(np.arange(np.min(x), np.max(x), 5), text_values)

plt.xticks(fontsize=23)

plt.yticks(fontsize=23)


#plt.xlabel('Bunch ID', fontsize=29)
plt.ylabel(str(data_column), fontsize=29)

if data_column=='chi2':
 plt.ylabel(str(data_column)+'/ndof', fontsize=29)


plt.ticklabel_format(useOffset=False, axis='y')  # Prevent plt from adding offset to y axis

##Xlimits:
plt.xlim(0,72)


if y_limits==1:
 plt.ylim(y_lim1,y_lim2)

fig = plt.gcf()

fig.set_size_inches(25, 14)


plt.savefig(str(OutPathPlots)+str(data_column)+'_'+str(fit)+'.png', dpi=500, bbox_inches='tight')
