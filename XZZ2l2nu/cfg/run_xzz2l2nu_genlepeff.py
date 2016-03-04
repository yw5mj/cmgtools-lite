##########################################################
##      configuration for XZZ2l2nu 
##########################################################

import CMGTools.XZZ2l2nu.fwlite.Config as cfg
from CMGTools.XZZ2l2nu.fwlite.Config import printComps
from CMGTools.XZZ2l2nu.RootTools import *
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption
from CMGTools.XZZ2l2nu.analyzers.AutoFillTreeProducer  import *
from CMGTools.XZZ2l2nu.analyzers.XZZTypes  import *
from CMGTools.XZZ2l2nu.analyzers.XZZGenLep import *
from CMGTools.XZZ2l2nu.samples.loadSamples76x import *
from CMGTools.XZZ2l2nu.analyzers.coreXZZ_cff import *
#-------- SEQUENCE
vertexAna.keepFailingEvents=True
genLep=cfg.Analyzer(
    XZZGenLep,
    name="genleptoneffAnalyzer",
    muons='slimmedMuons',
    electrons='slimmedElectrons',
    packedCandidates = 'packedPFCandidates',
    rhoMuon= 'fixedGridRhoFastjetCentralNeutral',
    rhoElectron = 'fixedGridRhoFastjetCentralNeutral',
    mu_isoCorr = "rhoArea" ,
    ele_isoCorr = "rhoArea" ,
    mu_effectiveAreas = "Spring15_25ns_v1", 
    ele_effectiveAreas = "Spring15_25ns_v1",
    miniIsolationPUCorr = None, # Allowed options: 'rhoArea' (EAs for 03 cone scaled by R^2), 'deltaBeta', 
                                     # 'raw' (uncorrected), 'weights' (delta beta weights; not validated)
                                     # Choose None to just use the individual object's PU correction
    )

leptonType.variables.extend([
    NTupleVariable("isfromX", lambda x : x.xdaughter if hasattr(x,'xdaughter') else -100, help="is from X"),
#    NTupleVariable("isprobe", lambda x : x.isprobe if hasattr(x,'isprobe') else -100, help="is probe lepton"),
    # Extra isolation variables
    NTupleVariable("chargedHadRelIso03",  lambda x : x.chargedHadronIsoR(0.3)/x.pt(), help="PF Rel Iso, R=0.3, charged hadrons only"),
    NTupleVariable("chargedHadRelIso04",  lambda x : x.chargedHadronIsoR(0.4)/x.pt(), help="PF Rel Iso, R=0.4, charged hadrons only"),
    # Extra muon ID working points
    NTupleVariable("softMuonId", lambda x : x.muonID("POG_ID_Soft") if abs(x.pdgId())==13 else -999, int, help="Muon POG Soft id"),
    NTupleVariable("pfMuonId",   lambda x : x.muonID("POG_ID_Loose") if abs(x.pdgId())==13 else -999, int, help="Muon POG Loose id"),
    # Extra electron ID working points
    NTupleVariable("looseelectron",   lambda x : x.physObj.electronID("cutBasedElectronID-Spring15-25ns-V1-standalone-loose") if abs(x.pdgId())==11 else -999, int, help="electron POG Loose id"),
    NTupleVariable("eleCutId2012_full5x5",     lambda x : (1*x.electronID("POG_Cuts_ID_2012_full5x5_Veto") + 1*x.electronID("POG_Cuts_ID_2012_full5x5_Loose") + 1*x.electronID("POG_Cuts_ID_2012_full5x5_Medium") + 1*x.electronID("POG_Cuts_ID_2012_full5x5_Tight")) if abs(x.pdgId()) == 11 else -1, int, help="Electron cut-based id (POG 2012, full5x5 shapes): 0=none, 1=veto, 2=loose, 3=medium, 4=tight"),
    # Extra muon ID variables
    NTupleVariable("nStations",    lambda lepton : lepton.numberOfMatchedStations() if abs(lepton.pdgId()) == 13 else 4, help="Number of matched muons stations (4 for electrons)"),
    NTupleVariable("caloCompatibility",      lambda lepton : lepton.caloCompatibility() if abs(lepton.pdgId()) == 13 else 0, help="Calorimetric compatibility"), 
    NTupleVariable("trkKink",      lambda lepton : lepton.combinedQuality().trkKink if abs(lepton.pdgId()) == 13 else 0, help="Tracker kink-finder"), 
    NTupleVariable("segmentCompatibility", lambda lepton : lepton.segmentCompatibility() if abs(lepton.pdgId()) == 13 else 0, help="Segment-based compatibility"), 
    NTupleVariable("chi2LocalPosition",    lambda lepton : lepton.combinedQuality().chi2LocalPosition if abs(lepton.pdgId()) == 13 else 0, help="Tracker-Muon matching in position"), 
    NTupleVariable("chi2LocalMomentum",    lambda lepton : lepton.combinedQuality().chi2LocalMomentum if abs(lepton.pdgId()) == 13 else 0, help="Tracker-Muon matching in momentum"), 
    NTupleVariable("glbTrackProbability",  lambda lepton : lepton.combinedQuality().glbTrackProbability if abs(lepton.pdgId()) == 13 else 0, help="Global track pseudo-probability"), 
    # Extra electron ID variables
    NTupleVariable("sigmaIEtaIEta",  lambda x : x.full5x5_sigmaIetaIeta() if abs(x.pdgId())==11 else 0, help="Electron sigma(ieta ieta), with full5x5 cluster shapes"),
    NTupleVariable("hadronicOverEm", lambda x : x.hadronicOverEm() if abs(x.pdgId())==11 else 0, help="Electron hadronicOverEm"),
    NTupleVariable("eInvMinusPInv",  lambda x : ((1.0/x.ecalEnergy() - x.eSuperClusterOverP()/x.ecalEnergy()) if x.ecalEnergy()>0. else 9e9) if abs(x.pdgId())==11 else 0, help="Electron 1/E - 1/p  (without absolute value!)"),
#    new version used by EGM in Spring15, 7_4_14:
    NTupleVariable("etaSc", lambda x : x.superCluster().eta() if abs(x.pdgId())==11 else -100, help="Electron supercluster pseudorapidity"),
]
)
lepTree = cfg.Analyzer(
     AutoFillTreeProducer, name='lepTreeProducer',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
     collections = {
         "selectedLeptons"  : NTupleCollection("Lep",leptonType,20, help="selected leptons"),
     }
)
sequence = [
    vertexAna,
    genLep,
    lepTree
]

#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    #selectedComponents = [BulkGravToZZToZlepZhad_narrow_1600]
    selectedComponents = BulkGravToZZToZlepZhad
    #selectedComponents = [JetHT_Run2015D_Promptv4,JetHT_Run2015D_05Oct]
    #selectedComponents = [SingleMuon_Run2015D_05Oct]
    #selectedComponents = mcSamples
    #selectedComponents = [SingleMuon_Run2015D_Promptv4,SingleElectron_Run2015D_Promptv4]
    #[SingleElectron_Run2015D_Promptv4,SingleElectron_Run2015D_05Oct]
    #selectedComponents = [RSGravToZZToZZinv_narrow_800]
    #selectedComponents = [ZZTo4L]
    #selectedComponents = [BulkGravToZZ_narrow_800]
    #selectedComponents = [BulkGravToZZToZlepZhad_narrow_800]
    for c in selectedComponents:
        #c.files = c.files[0]
        c.splitFactor =  (len(c.files)/10 if len(c.files)>10 else 1)
        #c.splitFactor = 1
        c.triggers=[]
        c.vetoTriggers = []
        #c.triggers=triggers_1e_noniso

## output histogram
outputService=[]
from PhysicsTools.HeppyCore.framework.services.tfile import TFileService
output_service = cfg.Service(
    TFileService,
    'outputfile',
    name="outputfile",
    fname='lepTreeProducer/tree.root',
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




