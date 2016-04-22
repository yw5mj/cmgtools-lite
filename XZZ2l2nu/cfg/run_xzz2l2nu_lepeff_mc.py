##########################################################
##      configuration for XZZ2l2nu 
##########################################################

import CMGTools.XZZ2l2nu.fwlite.Config as cfg
from CMGTools.XZZ2l2nu.fwlite.Config import printComps
from CMGTools.XZZ2l2nu.RootTools import *
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption


#Load all common analyzers
from CMGTools.XZZ2l2nu.analyzers.coreXZZ_cff import *
from CMGTools.XZZ2l2nu.analyzers.XZZLeptonEffTree import *

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.XZZ2l2nu.samples.loadSamples76x import *

#-------- Analyzer
from CMGTools.XZZ2l2nu.analyzers.treeXZZ_cff import *

#-------- SEQUENCE
#sequence = cfg.Sequence(coreSequence+[vvSkimmer,vvTreeProducer])
lepeffAna = cfg.Analyzer(
    XZZLeptonEffTree,
    name='leptonEffTree',
    genfilter=True,
    eithercharge=False,
    )

lepAna.applyIso=False
lepAna.applyID=False
vertexAna.keepFailingEvents=True
leptonType.variables.extend([
    # Extra muon ID working points
    NTupleVariable("softMuonId", lambda x : x.muonID("POG_ID_Soft") if abs(x.pdgId())==13 else -100, int, help="Muon POG Soft id"),
    NTupleVariable("pfMuonId",   lambda x : x.muonID("POG_ID_Loose") if abs(x.pdgId())==13 else -100, int, help="Muon POG Loose id"),
    # Extra electron ID working points
    NTupleVariable("eleCutId2012_full5x5",     lambda x : (1*x.electronID("POG_Cuts_ID_2012_full5x5_Veto") + 1*x.electronID("POG_Cuts_ID_2012_full5x5_Loose") + 1*x.electronID("POG_Cuts_ID_2012_full5x5_Medium") + 1*x.electronID("POG_Cuts_ID_2012_full5x5_Tight")) if abs(x.pdgId()) == 11 else -1, int, help="Electron cut-based id (POG 2012, full5x5 shapes): 0=none, 1=veto, 2=loose, 3=medium, 4=tight"),
]) 
sequence = [
    jsonAna,
    pileUpAna,
    vertexAna,
    lepAna,
    lepeffAna,
    lepeffTreeProducer
]
    

#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    selectedComponents = [DYJetsToLL_M50,ZZTo2L2Nu,DYJetsToLL_M50_Ext]
    #selectedComponents = mcSamples
    #selectedComponents = [SingleMuon_Run2015D_Promptv4,SingleElectron_Run2015D_Promptv4]
    #[SingleElectron_Run2015D_Promptv4,SingleElectron_Run2015D_05Oct]
    #selectedComponents = [RSGravToZZToZZinv_narrow_800]
    #selectedComponents = [DYJetsToLL_M50]
    #selectedComponents = [BulkGravToZZ_narrow_800]
    #selectedComponents = [BulkGravToZZToZlepZhad_narrow_800]
    for c in selectedComponents:
        #c.files = c.files[0]
        c.splitFactor = (len(c.files)/10 if len(c.files)>10 else 1)
        #c.splitFactor = 1
        #c.triggers=triggers_1mu_noniso
        #c.triggers=triggers_1e_noniso

## output histogram
outputService=[]
from PhysicsTools.HeppyCore.framework.services.tfile import TFileService
output_service = cfg.Service(
    TFileService,
    'outputfile',
    name="outputfile",
    fname='vvTreeProducer/tree.root',
    option='recreate'
    )
outputService.append(output_service)

from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
from CMGTools.TTHAnalysis.tools.EOSEventsWithDownload import EOSEventsWithDownload
event_class = EOSEventsWithDownload
event_class = Events
if getHeppyOption("nofetch"):
    event_class = Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],
                     events_class = event_class)




