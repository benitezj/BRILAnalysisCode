import CRABClient                                                                                       
from WMCore.Configuration import Configuration                                                                                                                                                               
config = Configuration()                                                                   
import os                                                                                                                                                                                                              
config.section_("General")                                                                                                                                                                                                                                                                                                   
config.General.requestName = "CMSSW_10_6_29_ZeroBias7" #change for each dataset     
#config.General.instance = "prod"                                                         
config.General.transferLogs = True                                                                        
                                                                                             
config.section_("JobType")                                                                   
config.JobType.pluginName = "Analysis"                                                        
config.JobType.psetName = "Run_PixVertex_Event.py"                                             
config.JobType.allowUndistributedCMSSW = True                                                                                                                                                      
## to include local file in the sendbox, this will put the file in the directory where cmsRun runs                                                                                                                                                                                                                           
#config.JobType.inputFiles   = [ os.environ['CMSSW_BASE'] + '/src/'+ 'flashgg/MetaData/data/PY8_RunIISpring15DR74_bx50_MC.db' ]                                                                                                                                                                                  
## incrase jobs time wall, maximum 2750 minutes (~46 hours)                                                                                                                                                                                                                                                                  
config.JobType.maxJobRuntimeMin = 2750                                                                                                                                                             
## config.JobType.maxMemoryMB = 3000 # For memory leaks. NB. will block jobs on many sites                                                                                                                                                                                                                                   
## config.JobType.scriptExe = "cmsWrapper.sh"                                                                                                                                                                                                                                                                                
#config.JobType.pyCfgParams = ['datasetName=/ZeroBias7/Run2015E-PromptReco-v1/AOD', 'globalTag=75X_dataRun2_Prompt_ppAt5TeV_v0', 'processType=data']                                                                                                                                                                         
#config.JobType.sendPythonFolder = True                                                                                                                                                                     
config.section_("Data")                                                                                                                                                                       
config.Data.inputDataset = "/ZeroBias7/Run2017C-LumiPixelsMinBias-18Oct2022_UL2017_VdM-v1/ALCARECO"
config.Data.inputDBS = 'global'                                                 
config.Data.splitting = "FileBased"                                
config.Data.unitsPerJob = 1                                                               
config.Data.publication = True                                                     
config.Data.publishDBS = 'phys03'                   
config.Data.outLFNDirBase = "/store/group/comm_luminosity/lcuevasp/tuples_2017/"                                                                                                                                                                                                                                                              
config.Data.ignoreLocality = True                                                           
config.Data.allowNonValidInputDataset = True                                                                                                                                                         
config.section_("Site")                                                                                                                                                                      
config.Site.storageSite = "T2_CH_CERN"   #"T2_IT_Pisa"          
config.Site.whitelist=["T2_CH_CERN"] #"T2_IT_Pisa"                                                                                                                                                            
config.Data.runRange = "300019,300029,300050"  #change runs                                                                                                                                                                                                                                                                        
                                                        
