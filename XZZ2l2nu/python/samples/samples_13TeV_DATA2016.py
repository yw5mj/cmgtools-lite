import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR

from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()


# Hengne:
json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-280385_13TeV_PromptReco_Collisions16_JSON_NoL1T_v2.txt'
run_range = (271036,280385)
#json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-280385_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt'
#run_range = (271036,280385)
#json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-279931_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt'
#run_range=(271036,279931)
#json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-279588_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt'
#run_range=(271036,279588)
#json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt'
#run_range=(271036,276811)
#json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-276384_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt'
#run_range=(271036,276384)
#json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-276097_13TeV_PromptReco_Collisions16_JSON_NoL1T_v2.txt'
#run_range=(271036,276097)
#json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-275783_13TeV_PromptReco_Collisions16_JSON.txt'
#run_range=(271036,275783)
#json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-274443_13TeV_PromptReco_Collisions16_JSON.txt'
#json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt'
#run_range = (271036,275125)
#json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-273730_13TeV_PromptReco_Collisions16_JSON.txt'

### ----------------------------- Zero Tesla run  ----------------------------------------

#dataDir = "$CMSSW_BASE/src/CMGTools/TTHAnalysis/data"  # use environmental variable, useful for instance to run on CRAB
#json=dataDir+'/json/Cert_246908-256869_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
#https://hypernews.cern.ch/HyperNews/CMS/get/physics-validation/2496.html
#golden JSON 166.37/pb 


#jetHT_0T = cfg.DataComponent(
#    name = 'jetHT_0T',
#    files = kreator.getFilesFromEOS('jetHT_0T',
#                                    'firstData_JetHT_v2',
#                                    '/store/user/pandolf/MINIAOD/%s'),
#    intLumi = 4.0,
#    triggers = [],
#    json = None #json
#    )


# PromptReco-v1 for run > 251561
#run_range = (251643, 251883)
#label = "_runs%s_%s"%(run_range[0], run_range[1])

### ----------------------------- Run2016B PromptReco v1 ----------------------------------------

JetHT_Run2016B_PromptReco          = kreator.makeDataComponent("JetHT_Run2016B_PromptReco"         , "/JetHT/Run2016B-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016B_PromptReco          = kreator.makeDataComponent("HTMHT_Run2016B_PromptReco"         , "/HTMHT/Run2016B-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016B_PromptReco            = kreator.makeDataComponent("MET_Run2016B_PromptReco"           , "/MET/Run2016B-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016B_PromptReco = kreator.makeDataComponent("SingleElectron_Run2016B_PromptReco", "/SingleElectron/Run2016B-PromptReco-v1/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016B_PromptReco     = kreator.makeDataComponent("SingleMuon_Run2016B_PromptReco"    , "/SingleMuon/Run2016B-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016B_PromptReco   = kreator.makeDataComponent("SinglePhoton_Run2016B_PromptReco"  , "/SinglePhoton/Run2016B-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016B_PromptReco       = kreator.makeDataComponent("DoubleEG_Run2016B_PromptReco"      , "/DoubleEG/Run2016B-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016B_PromptReco         = kreator.makeDataComponent("MuonEG_Run2016B_PromptReco"       , "/MuonEG/Run2016B-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016B_PromptReco     = kreator.makeDataComponent("DoubleMuon_Run2016B_PromptReco"    , "/DoubleMuon/Run2016B-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)


### ----------------------------- Run2016B PromptReco v2 ----------------------------------------

JetHT_Run2016B_PromptReco_v2          = kreator.makeDataComponent("JetHT_Run2016B_PromptReco_v2"         , "/JetHT/Run2016B-PromptReco-v2/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016B_PromptReco_v2          = kreator.makeDataComponent("HTMHT_Run2016B_PromptReco_v2"         , "/HTMHT/Run2016B-PromptReco-v2/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016B_PromptReco_v2            = kreator.makeDataComponent("MET_Run2016B_PromptReco_v2"           , "/MET/Run2016B-PromptReco-v2/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016B_PromptReco_v2 = kreator.makeDataComponent("SingleElectron_Run2016B_PromptReco_v2", "/SingleElectron/Run2016B-PromptReco-v2/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016B_PromptReco_v2     = kreator.makeDataComponent("SingleMuon_Run2016B_PromptReco_v2"    , "/SingleMuon/Run2016B-PromptReco-v2/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016B_PromptReco_v2   = kreator.makeDataComponent("SinglePhoton_Run2016B_PromptReco_v2"  , "/SinglePhoton/Run2016B-PromptReco-v2/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016B_PromptReco_v2       = kreator.makeDataComponent("DoubleEG_Run2016B_PromptReco_v2"      , "/DoubleEG/Run2016B-PromptReco-v2/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016B_PromptReco_v2        = kreator.makeDataComponent("MuonEG_Run2016B_PromptReco_v2"        , "/MuonEG/Run2016B-PromptReco-v2/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016B_PromptReco_v2     = kreator.makeDataComponent("DoubleMuon_Run2016B_PromptReco_v2"    , "/DoubleMuon/Run2016B-PromptReco-v2/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)


### ----------------------------- Run2016C PromptReco v2 ----------------------------------------

JetHT_Run2016C_PromptReco_v2          = kreator.makeDataComponent("JetHT_Run2016C_PromptReco_v2"         , "/JetHT/Run2016C-PromptReco-v2/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016C_PromptReco_v2          = kreator.makeDataComponent("HTMHT_Run2016C_PromptReco_v2"         , "/HTMHT/Run2016C-PromptReco-v2/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016C_PromptReco_v2            = kreator.makeDataComponent("MET_Run2016C_PromptReco_v2"           , "/MET/Run2016C-PromptReco-v2/MINIAOD"           , "CMS", ".*root", json,  useAAA=False,jsonFilter=False)
SingleElectron_Run2016C_PromptReco_v2 = kreator.makeDataComponent("SingleElectron_Run2016C_PromptReco_v2", "/SingleElectron/Run2016C-PromptReco-v2/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016C_PromptReco_v2     = kreator.makeDataComponent("SingleMuon_Run2016C_PromptReco_v2"    , "/SingleMuon/Run2016C-PromptReco-v2/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016C_PromptReco_v2   = kreator.makeDataComponent("SinglePhoton_Run2016C_PromptReco_v2"  , "/SinglePhoton/Run2016C-PromptReco-v2/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016C_PromptReco_v2       = kreator.makeDataComponent("DoubleEG_Run2016C_PromptReco_v2"      , "/DoubleEG/Run2016C-PromptReco-v2/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016C_PromptReco_v2        = kreator.makeDataComponent("MuonEG_Run2016C_PromptReco_v2"        , "/MuonEG/Run2016C-PromptReco-v2/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016C_PromptReco_v2     = kreator.makeDataComponent("DoubleMuon_Run2016C_PromptReco_v2"    , "/DoubleMuon/Run2016C-PromptReco-v2/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)

### ----------------------------- Run2016D PromptReco v2 ----------------------------------------

JetHT_Run2016D_PromptReco_v2          = kreator.makeDataComponent("JetHT_Run2016D_PromptReco_v2"         , "/JetHT/Run2016D-PromptReco-v2/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016D_PromptReco_v2          = kreator.makeDataComponent("HTMHT_Run2016D_PromptReco_v2"         , "/HTMHT/Run2016D-PromptReco-v2/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016D_PromptReco_v2            = kreator.makeDataComponent("MET_Run2016D_PromptReco_v2"           , "/MET/Run2016D-PromptReco-v2/MINIAOD"           , "CMS", ".*root", json,  useAAA=False,jsonFilter=False)
SingleElectron_Run2016D_PromptReco_v2 = kreator.makeDataComponent("SingleElectron_Run2016D_PromptReco_v2", "/SingleElectron/Run2016D-PromptReco-v2/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016D_PromptReco_v2     = kreator.makeDataComponent("SingleMuon_Run2016D_PromptReco_v2"    , "/SingleMuon/Run2016D-PromptReco-v2/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016D_PromptReco_v2   = kreator.makeDataComponent("SinglePhoton_Run2016D_PromptReco_v2"  , "/SinglePhoton/Run2016D-PromptReco-v2/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016D_PromptReco_v2       = kreator.makeDataComponent("DoubleEG_Run2016D_PromptReco_v2"      , "/DoubleEG/Run2016D-PromptReco-v2/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016D_PromptReco_v2        = kreator.makeDataComponent("MuonEG_Run2016D_PromptReco_v2"        , "/MuonEG/Run2016D-PromptReco-v2/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016D_PromptReco_v2     = kreator.makeDataComponent("DoubleMuon_Run2016D_PromptReco_v2"    , "/DoubleMuon/Run2016D-PromptReco-v2/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)


### ----------------------------- Run2016E PromptReco v2 ----------------------------------------

JetHT_Run2016E_PromptReco_v2          = kreator.makeDataComponent("JetHT_Run2016E_PromptReco_v2"         , "/JetHT/Run2016E-PromptReco-v2/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016E_PromptReco_v2          = kreator.makeDataComponent("HTMHT_Run2016E_PromptReco_v2"         , "/HTMHT/Run2016E-PromptReco-v2/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016E_PromptReco_v2            = kreator.makeDataComponent("MET_Run2016E_PromptReco_v2"           , "/MET/Run2016E-PromptReco-v2/MINIAOD"           , "CMS", ".*root", json,  useAAA=False,jsonFilter=False)
SingleElectron_Run2016E_PromptReco_v2 = kreator.makeDataComponent("SingleElectron_Run2016E_PromptReco_v2", "/SingleElectron/Run2016E-PromptReco-v2/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016E_PromptReco_v2     = kreator.makeDataComponent("SingleMuon_Run2016E_PromptReco_v2"    , "/SingleMuon/Run2016E-PromptReco-v2/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016E_PromptReco_v2   = kreator.makeDataComponent("SinglePhoton_Run2016E_PromptReco_v2"  , "/SinglePhoton/Run2016E-PromptReco-v2/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016E_PromptReco_v2       = kreator.makeDataComponent("DoubleEG_Run2016E_PromptReco_v2"      , "/DoubleEG/Run2016E-PromptReco-v2/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016E_PromptReco_v2        = kreator.makeDataComponent("MuonEG_Run2016E_PromptReco_v2"        , "/MuonEG/Run2016E-PromptReco-v2/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016E_PromptReco_v2     = kreator.makeDataComponent("DoubleMuon_Run2016E_PromptReco_v2"    , "/DoubleMuon/Run2016E-PromptReco-v2/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)


### ----------------------------- Run2016F PromptReco v1 ----------------------------------------

JetHT_Run2016F_PromptReco_v1          = kreator.makeDataComponent("JetHT_Run2016F_PromptReco_v1"         , "/JetHT/Run2016F-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016F_PromptReco_v1          = kreator.makeDataComponent("HTMHT_Run2016F_PromptReco_v1"         , "/HTMHT/Run2016F-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016F_PromptReco_v1            = kreator.makeDataComponent("MET_Run2016F_PromptReco_v1"           , "/MET/Run2016F-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json, useAAA=True,jsonFilter=False)
SingleElectron_Run2016F_PromptReco_v1 = kreator.makeDataComponent("SingleElectron_Run2016F_PromptReco_v1", "/SingleElectron/Run2016F-PromptReco-v1/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016F_PromptReco_v1     = kreator.makeDataComponent("SingleMuon_Run2016F_PromptReco_v1"    , "/SingleMuon/Run2016F-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=True,jsonFilter=False)
SinglePhoton_Run2016F_PromptReco_v1   = kreator.makeDataComponent("SinglePhoton_Run2016F_PromptReco_v1"  , "/SinglePhoton/Run2016F-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=True,jsonFilter=False)
DoubleEG_Run2016F_PromptReco_v1       = kreator.makeDataComponent("DoubleEG_Run2016F_PromptReco_v1"      , "/DoubleEG/Run2016F-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016F_PromptReco_v1        = kreator.makeDataComponent("MuonEG_Run2016F_PromptReco_v1"        , "/MuonEG/Run2016F-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016F_PromptReco_v1     = kreator.makeDataComponent("DoubleMuon_Run2016F_PromptReco_v1"    , "/DoubleMuon/Run2016F-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)



### ----------------------------- Run2016G PromptReco v1 ----------------------------------------

JetHT_Run2016G_PromptReco_v1          = kreator.makeDataComponent("JetHT_Run2016G_PromptReco_v1"         , "/JetHT/Run2016G-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016G_PromptReco_v1          = kreator.makeDataComponent("HTMHT_Run2016G_PromptReco_v1"         , "/HTMHT/Run2016G-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016G_PromptReco_v1            = kreator.makeDataComponent("MET_Run2016G_PromptReco_v1"           , "/MET/Run2016G-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016G_PromptReco_v1 = kreator.makeDataComponent("SingleElectron_Run2016G_PromptReco_v1", "/SingleElectron/Run2016G-PromptReco-v1/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016G_PromptReco_v1     = kreator.makeDataComponent("SingleMuon_Run2016G_PromptReco_v1"    , "/SingleMuon/Run2016G-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016G_PromptReco_v1   = kreator.makeDataComponent("SinglePhoton_Run2016G_PromptReco_v1"  , "/SinglePhoton/Run2016G-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016G_PromptReco_v1       = kreator.makeDataComponent("DoubleEG_Run2016G_PromptReco_v1"      , "/DoubleEG/Run2016G-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016G_PromptReco_v1        = kreator.makeDataComponent("MuonEG_Run2016G_PromptReco_v1"        , "/MuonEG/Run2016G-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016G_PromptReco_v1     = kreator.makeDataComponent("DoubleMuon_Run2016G_PromptReco_v1"    , "/DoubleMuon/Run2016G-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)


### ----------------------------- summary ----------------------------------------


### ---------------------------------------------------------------------
