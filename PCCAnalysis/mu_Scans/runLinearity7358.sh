#path=/brildata/vdmoutput18/Analysed_Data/7242_01Oct18_093547_01Oct18_093833/*
#path='/brildata/vdmoutput18/Analysed_Data/7358_26Oct18_090951_26Oct18_092136/*'
for file in /brildata/vdmoutput18/Analysed_Data/7358_26Oct18_090951_26Oct18_092136/*
 	do 
 		echo Trying $file
 		python ./linearityOverPCC.py $file AllVersus
 	done
