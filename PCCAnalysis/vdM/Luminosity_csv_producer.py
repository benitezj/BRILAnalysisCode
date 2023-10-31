import tables as t
import pandas as pd
import numpy as np

# Input hd5 file:
#input_file = "/afs/cern.ch/user/l/lcuevasp/public/PCC/Ratefile/new_2017/6016/6016_PCC_2017.hd5"
input_file = '/eos/user/l/lcuevasp/BRIL/PCC/VDM_2022/PCC2022_8381_veto2022_Benitez/8381_PCC_vdM_2022.hd5'  # 2022 last veto (1693 bad modules)

# Crear un diccionario para almacenar los datos por cada combinacinnn de Run y LS
rate_data = {
    'RUN': [],
    'LS': [],
    'timestamp':[],
    #'PCCTOTAL': [],
    'PCC_BCID1': [],
    'PCC_BCID2': [],
    'PCC_BCID3': [],
    'PCC_BCID4': [],
    'PCC_BCID5': []
}

# BCID values you are interested in
pcc_bx = [282, 822, 2944, 3123, 3302] #2022
#pcc_bx = [41,  281,  872, 1783, 2063]  

# Open the hd5 file 
h5in = t.open_file(input_file, mode='r')
table = h5in.root.pcchd5

for row in table.iterrows():
    if row['timestampsec']==0:
        continue
    run = row['runnum']
    ls = row['lsnum']
    bx = row['bxraw'][pcc_bx]  # Extract the BCID rates for the specified BCIDs
    timestamp=row['timestampsec']
    # Agregar los valores a las listas correspondientes en el diccionario
    rate_data['RUN'].append(run)
    rate_data['LS'].append(ls)
    rate_data['PCC_BCID1'].append(bx[0])
    rate_data['PCC_BCID2'].append(bx[1])
    rate_data['PCC_BCID3'].append(bx[2])
    rate_data['PCC_BCID4'].append(bx[3])
    rate_data['PCC_BCID5'].append(bx[4])
   # rate_data['RATETOTAL'].append(0)
    rate_data['timestamp'].append(timestamp)


h5in.close()
#b=a.sort()
#c  = set(b)
#print(c)
# Convertir el diccionario en un DataFrame de Pandas
df = pd.DataFrame(rate_data)

#df['timestamp'].replace(0, np.nan, inplace=True)
#df['PCC_BCID1'].replace(0, np.nan, inplace=True) 
#df['PCC_BCID2'].replace(0, np.nan, inplace=True) 
#df['PCC_BCID3'].replace(0, np.nan, inplace=True) 
#df['PCC_BCID4'].replace(0, np.nan, inplace=True) 
#df['PCC_BCID5'].replace(0, np.nan, inplace=True)

 
# Calcular el promedio de RATETOTAL para cada combinacinnn de Run y LS
df_grouped = df.groupby(['RUN', 'LS']).agg({
   # 'PCCTOTAL': 'mean',
    'PCC_BCID1': 'mean',
    'PCC_BCID2': 'mean',
    'PCC_BCID3': 'mean',
    'PCC_BCID4': 'mean',
    'PCC_BCID5': 'mean',
    'timestamp': 'mean'
}).reset_index()

#m = (0.07777400000000001 - 0.0411288)/(1668167375-1668123203) 
#b=-(m*1668167375-0.0777740000)
#y = -1325.06  + (7.94367e-07)*x 
df_grouped['background'] = (df_grouped['timestamp']*(7.94367e-07))-1325.06
#df_grouped.loc[0:568, 'background'] = 0.0411288

df_grouped['PCC_BCID1'] = df_grouped['PCC_BCID1'] - df_grouped['background']
df_grouped['PCC_BCID2'] = df_grouped['PCC_BCID2'] - df_grouped['background']  
df_grouped['PCC_BCID3'] = df_grouped['PCC_BCID3'] - df_grouped['background']  
df_grouped['PCC_BCID4'] = df_grouped['PCC_BCID4'] - df_grouped['background']  
df_grouped['PCC_BCID5'] = df_grouped['PCC_BCID5'] - df_grouped['background']  

df_grouped['PCCTOTAL'] = df_grouped[['PCC_BCID1', 'PCC_BCID2', 'PCC_BCID3', 'PCC_BCID4', 'PCC_BCID5']].sum(axis=1)  
df_grouped['PCCTOTAL'] =df_grouped['PCCTOTAL']*0.010451


columnas_a_eliminar = ['background']#,'PCC_BCID1', 'PCC_BCID2', 'PCC_BCID3', 'PCC_BCID4', 'PCC_BCID5']
# Elimina las columnas especificadas
df_grouped = df_grouped.drop(columnas_a_eliminar, axis=1)


column_order = ['RUN', 'LS','timestamp','PCC_BCID1', 'PCC_BCID2', 'PCC_BCID3', 'PCC_BCID4', 'PCC_BCID5', 'PCCTOTAL']  
df_grouped = df_grouped[column_order]  

# Guardar el DataFrame agrupado en un archivo CSV
output_file = 'rates_8381_2022.csv'
df_grouped.to_csv(output_file, index=False)

print('Data saved to output_file')
