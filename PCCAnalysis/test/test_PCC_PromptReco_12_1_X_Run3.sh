cmsDriver.py step5 \
--process RECO \
--conditions auto:run2_data \
--scenario pp \
-s ALCAPRODUCER:AlCaPCCZeroBias+RawPCCProducer \
--datatier ALCARECO \
--filein filelist:/afs/cern.ch/user/b/benitezj/public/BRIL/PCC/Run3Dev/step1_dasquery_Run3.log \
--data \
--eventcontent ALCARECO \
--era Run2_2018 \
-n 100 
