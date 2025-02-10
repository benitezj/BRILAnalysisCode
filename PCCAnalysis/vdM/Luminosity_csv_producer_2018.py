import tables as t
import pandas as pd
import numpy as np


input_file = '/eos/user/l/lcuevasp/BRIL/PCC/VDM_2018/VDM/PCC2018Jobs_veto_common/merge/PCC_common_veto_2018.hd5'
rate_data = {
    'RUN': [],
    'LS': [],
    'timestamp':[],
    #'PCCTOTAL': [],
    'PCC_BCID1': [],
    'PCC_BCID2': [],
    'PCC_BCID3': [],
    'PCC_BCID4': [],
    'PCC_BCID5': [],
}  

# BCID values you are interested in
#pcc_bx = [282, 822, 2944, 3123, 3302] #2022
#pcc_bx = [41,  281,  872, 1783, 2063]  
#pcc_bx = [208, 282, 548, 822, 1197, 1376, 2716, 2944, 3123]
pcc_bx = [265,  865,  1780, 2192, 3380] #2018
# Open the hd5 file 
h5in = t.open_file(input_file, mode='r')
table = h5in.root.pcchd5

for row in table.iterrows():
    if row['timestampsec']==0:
        continue
    run = row['runnum']
    ls = row['lsnum']
    bx = row['bxraw'][pcc_bx]  # Extract the BCID rates for the specified BCIDs
#    print(row['bxraw'][pcc_bx])
    timestamp=row['timestampsec']
    
    rate_data['RUN'].append(run)
    rate_data['LS'].append(ls)
    rate_data['PCC_BCID1'].append(bx[0])
    rate_data['PCC_BCID2'].append(bx[1])
    rate_data['PCC_BCID3'].append(bx[2])
    rate_data['PCC_BCID4'].append(bx[3])
    rate_data['PCC_BCID5'].append(bx[4])
    rate_data['timestamp'].append(timestamp)


h5in.close()
# dic to df
df = pd.DataFrame(rate_data)
 
# Rate for each  Run and LS
df_grouped = df.groupby(['RUN', 'LS']).agg({
   # 'PCCTOTAL': 'mean',
    'PCC_BCID1': 'mean',
    'PCC_BCID2': 'mean',
    'PCC_BCID3': 'mean',
    'PCC_BCID4': 'mean',
    'PCC_BCID5': 'mean',
    'timestamp': 'median'
}).reset_index()


df_grouped['background'] = 0.02757   ##  <<-- Background value
#print(df_grouped['PCC_BCID1'][100:110])


df_grouped['PCC_BCID1'] = df_grouped['PCC_BCID1'] - df_grouped['background']
df_grouped['PCC_BCID2'] = df_grouped['PCC_BCID2'] - df_grouped['background']  
df_grouped['PCC_BCID3'] = df_grouped['PCC_BCID3'] - df_grouped['background']  
df_grouped['PCC_BCID4'] = df_grouped['PCC_BCID4'] - df_grouped['background']  
df_grouped['PCC_BCID5'] = df_grouped['PCC_BCID5'] - df_grouped['background']  

df_grouped['PCCTOTAL'] = df_grouped[['PCC_BCID1', 'PCC_BCID2', 'PCC_BCID3', 'PCC_BCID4', 'PCC_BCID5']].sum(axis=1)  
 

flhc_SigVis = 1

df_grouped['PCCTOTAL'] =df_grouped['PCCTOTAL']*flhc_SigVis #0.002425423 #11245/4629520    #4636304 Actual Peter   # #2017
#df_grouped['PCCTOTAL'] =df_grouped['PCCTOTAL']*0.011679598 #11245/962790 Actual Peter #2018
#Vtx 4621819.84 all corr


 
df_grouped['PCC_BCID1'] = df_grouped['PCC_BCID1']* flhc_SigVis   
df_grouped['PCC_BCID2'] = df_grouped['PCC_BCID2']* flhc_SigVis                                          
df_grouped['PCC_BCID3'] = df_grouped['PCC_BCID3']* flhc_SigVis                                           
df_grouped['PCC_BCID4'] = df_grouped['PCC_BCID4']* flhc_SigVis                                           
df_grouped['PCC_BCID5'] = df_grouped['PCC_BCID5']* flhc_SigVis 
columnas_a_eliminar = ['background']#,'PCC_BCID1', 'PCC_BCID2', 'PCC_BCID3', 'PCC_BCID4', 'PCC_BCID5']

# Elimina las columnas especificadas
df_grouped = df_grouped.drop(columnas_a_eliminar, axis=1)


column_order = ['RUN', 'LS','timestamp','PCC_BCID1', 'PCC_BCID2', 'PCC_BCID3', 'PCC_BCID4', 'PCC_BCID5','PCCTOTAL']  
df_grouped = df_grouped[column_order]  


output_file = '6868_SigV_960026.csv
'
#output_file = 'pcchd5_CapS_Vtx_noCorr_miss.csv'
#output_file = 'pcchd5_allCorr_miss.csv'
#output_file = 'pcchd5_noCorr_miss.csv'

df_grouped.to_csv(output_file, index=False)

print('Data saved to output_file')
