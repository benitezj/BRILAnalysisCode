import CRABClient                                                                                       
from WMCore.Configuration import Configuration                                                                                                                                                               
config = Configuration()                                                                   
import os                                                                                                                                                                                                              
config.section_("General")                                                                                                                                                                                                                                                                                                   
config.General.requestName = "8385_CMSSW_12_4_9_LumiPixelsMinBias_splitPerBXTrue19"     
#config.General.instance = "prod"                                                         
config.General.transferLogs = True                                                                        
                                                                                             
config.section_("JobType")                                                                   
config.JobType.pluginName = "Analysis"                                                        
config.JobType.psetName = "Run_PixVertex_Event.py"                                             
config.JobType.allowUndistributedCMSSW = True                                                                                                                                                      
## to include local file in the sendbox, this will put the file in the directory where cmsRun runs                    #Data.inputBlocks                                                                                                                                                                                                       
#config.JobType.inputFiles   = [ os.environ['CMSSW_BASE'] + '/src/'+ 'flashgg/MetaData/data/PY8_RunIISpring15DR74_bx50_MC.db' ]                                                                                                                                                                                  
## incrase jobs time wall, maximum 2750 minutes (~46 hours)                                                                                                                                                                                                                                                                  
config.JobType.maxJobRuntimeMin = 2750                                                                                                                                                             
## config.JobType.maxMemoryMB = 3000 # For memory leaks. NB. will block jobs on many sites                                                                                                                                                                                                                                   
## config.JobType.scriptExe = "cmsWrapper.sh"                                                                                                                                                                                                                                                                                
#config.JobType.pyCfgParams = ['datasetName=/ZeroBias7/Run2015E-PromptReco-v1/AOD', 'globalTag=75X_dataRun2_Prompt_ppAt5TeV_v0', 'processType=data']                                                                                                                                                                         
#config.JobType.sendPythonFolder = True                                                                                                                                                                     
config.section_("Data")                                                                                                                                                                       
config.Data.inputDataset = "/SpecialHLTPhysics19/Run2022F-LumiPixelsMinBias-PromptReco-v1/ALCARECO"
config.Data.inputDBS = 'global'                                                 
config.Data.splitting = "FileBased"                                
config.Data.unitsPerJob = 1                                                               
config.Data.publication = True                                                     
config.Data.publishDBS = 'phys03'                   
config.Data.outLFNDirBase = "/store/group/comm_luminosity/lcuevasp/Run3/8385/PromptReco/Nov2022_VdM-v1/"
config.Data.inputBlocks = True                                                                 
config.Data.ignoreLocality = True                                                           
#config.Data.allowNonValidInputDataset = True  
config.Data.allowNonValidInputDataset=True                                                                                            
config.section_("Site")                                                                                                                                                                      
config.Site.storageSite = "T2_CH_CERN"   #"T2_IT_Pisa"          
config.Site.whitelist=["T2_CH_CERN"] #"T2_IT_Pisa"                                                                                                                                                            
config.Data.runRange = "361955"

#"361880-361955"                                                                                        
                                                        
