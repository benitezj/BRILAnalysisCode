#recipe to run the analyzer:


cmsrel CMSSW_12_2_1
cd CMSSW_12_2_1/src
cmsenv
git cms-addpkg CondTools/BeamSpot
scram b -j 8
cd CondTools/BeamSpot/test


# edit BeamSpotRcdRead_cfg.py in:
# line 15: change starting run (Period A, C, early D and late D)
# line 16: change starting LS
# line 36: put the correct tag name (BeamSpotObjects_2016_2017_2018UL_SpecialRuns_LumiBased_v1)
# lines 45 and 51 to change the names of the output files

# edit BeamSpotRcdReader.cc Analyzer in:
# line 129: write only run and lumi section
# line 148: write only objects mybeamspot->GetX(), mybeamspot->GetY(), mybeamspot->GetZ()
# output file will contain run number, lumi section, beam position X, Y and Z
# Run the python code using 

cmsRun BeamSpotRcdRead_cfg.py

#The python code will run till you press ctrl+c and will dumpt the content of the beamspot tag into a text file and a root file.
