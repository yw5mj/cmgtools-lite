from CMGTools.XZZ2l2nu.analyzers.AutoFillTreeProducer  import * 
from CMGTools.XZZ2l2nu.analyzers.XZZTypes  import * 
from CMGTools.XZZ2l2nu.analyzers.Skimmer  import * 
import PhysicsTools.HeppyCore.framework.config as cfg

vvSkimmer = cfg.Analyzer(
    Skimmer,
    name='vvSkimmer',
    required = ['LLNuNu']
)

leptonSkimmer = cfg.Analyzer(
    Skimmer,
    name='leptonSkimmer',
    required = ['inclusiveLeptons']
)

jetTreeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='jetTreeProducer',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
     globalVariables = [
         NTupleVariable("nVert",  lambda ev: len(ev.goodVertices), int, help="Number of good vertices"), 
     ],
     globalObjects =  {
         "met" : NTupleObject("met", metType, help="PF E_{T}^{miss}, after full type 1 corrections"),
         "met_miniAod" : NTupleObject("met_raw", metType, help="PF E_{T}^{miss}, after type 1 corrections in miniAOD"),
         "met_JEC" : NTupleObject("met_JEC", metType, help="PF E_{T}^{miss}, after type 1 corrections with new 76X JEC added"),
         "met_JECJER" : NTupleObject("met_JECJER", metType, mcOnly=True, help="PF E_{T}^{miss}, after type 1 corrections with JEC+JER jets"),
     },

     collections = {
         "selectedLeptons" : NTupleCollection("lep",leptonType,10, help="selected leptons"),
         "genLeptons" : NTupleCollection("genLep", genParticleType, 10, help="Generated leptons (e/mu) from W/Z decays"),
         "genZBosons" : NTupleCollection("genZ", genParticleType, 10, help="Generated V bosons"),
         "genLeptons" : NTupleCollection("genLep", genParticleType, 10, help="Generated leptons (e/mu) from W/Z decays"),
         "genZBosons" : NTupleCollection("genZ", genParticleType, 10, help="Generated V bosons"),
         "jets"  : NTupleCollection("jet_corr",corrJetType,15, help="all jets with new JEC for 76X applied"),
         "jets_raw"  : NTupleCollection("jet",JetType,15, help="all jets from miniAOD"),
     }
)

vvTreeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='vvTreeProducer',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
     globalVariables = [
         NTupleVariable("nLL",lambda ev: len(ev.LL) , int),       
         NTupleVariable("nLLNuNu",lambda ev: len(ev.LLNuNu) , int),       
         NTupleVariable("nVert",  lambda ev: len(ev.goodVertices), int, help="Number of good vertices"), 
         NTupleVariable("triggersf",  lambda x : getattr(x,'trgsf',1), help="singleelectron/muon trigger sf"),
     ],
     globalObjects =  {
         "met" : NTupleObject("met", metType, help="PF E_{T}^{miss}, after type 1 corrections"),
     },

     collections = {
         "LL"  : NTupleCollection("Zll",LLType,5, help="Z to ll"),
         "selectedLeptons" : NTupleCollection("lep",leptonType,10, help="selected leptons"),
         "genLeptons" : NTupleCollection("genLep", genParticleType, 10, help="Generated leptons (e/mu) from W/Z decays"),
         "genZBosons" : NTupleCollection("genZ", genParticleType, 10, help="Generated V bosons"),
         "LLNuNu"     : NTupleCollection("llnunu",LLNuNuType ,5, help="VV candidate with di-lepton and MET"),
         "genLeptons" : NTupleCollection("genLep", genParticleType, 10, help="Generated leptons (e/mu) from W/Z decays"),
         "genZBosons" : NTupleCollection("genZ", genParticleType, 10, help="Generated V bosons"),
         "jets"       : NTupleCollection("jet_corr",corrJetType,15, help="all jets with new JEC for 76X applied (JER corrected if isMC)"),
         "jets_raw"   : NTupleCollection("jet",JetType,15, help="all jets from miniAOD"),
     }
)

leptonEffTreeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='leptonEffTreeProducer',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
     collections = {
         "llpair"  : NTupleCollection("llpair",llpairType ,5, help="lepton eff study"),
     }
)

leptonTreeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='leptonTreeProducer',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
#     PDFWeights = PDFWeights,
#     globalVariables = susyMultilepton_globalVariables,
#     globalObjects = susyMultilepton_globalObjects,
     collections = {
            "genleps"          : NTupleCollection("gen",     genParticleWithLinksType, 10, help="Generated leptons (e/mu) from W/Z decays"),                                                                                                
            "inclusiveLeptons" : NTupleCollection("l",    leptonTypeExtra, 10, help="Inclusive Leptons"),                                                                                                
     }
)
