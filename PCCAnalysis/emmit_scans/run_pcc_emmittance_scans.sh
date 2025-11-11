
##process the root files with pcc per event, integrate events per NB4
##output is a TTree with one entry per NB4. contains pcc per bx per module
## apply bcid selection to keep file small
cmsRun ${CMSSW_BASE}/src/BRILAnalysisCode/PCCAnalysis/emmit_scans/PCCEventToTuple_cfg.py

## process the Tuple and output HD5 table with pcc per NB4
## apply module veto and afterglow correction
python3 ${CMSSW_BASE}/src/BRILAnalysisCode/PCCAnalysis/emmit_scans/tuple_to_hd5_emmittance.py --inputfile=./pcctuple.root  --moduleveto=${CMSSW_BASE}/src/BRILAnalysisCode/PCCAnalysis/veto_2024/veto_B0+vdMBgdSS1_SS2_wights0+vdMSty+Block1_Sty1_Sty2_+Block2_Sty1_Sty2+Block3_Sty1_Sty2+Block4_Sty1_Sty2+Block5_Sty1_Sty2+Block6_Sty1_Sty2+All_Blocks_Sty_Lty.txt --timestamps=${CMSSW_BASE}/src/BRILAnalysisCode/PCCAnalysis/emmit_scans/emmit_scantimes_2025.txt

## create a plot of the time profile of the bcid's
python3  ${CMSSW_BASE}/src/BRILAnalysisCode/PCCAnalysis/emmit_scans/pcchd5_reader_emmittance.py --outpath=. #/eos/user/b//benitezj/www/plots/BRIL/PCC_lumi/EmmittanceScans/V0

