import ROOT
import os, types
from math import *
from PhysicsTools.HeppyCore.utils.deltar import *

class JetResolution:
    def __init__(self,globalTag="",jetFlavour="",jerPath="" ):
        """Create a corrector object that reads the payloads from the text dumps of a global tag under
            CMGTools/RootTools/data/jec  (see the getJec.py there to make the dumps).
           It will apply the L1,L2,L3 and possibly the residual corrections to the jets.
           If configured to do so, it will also compute the type1 MET corrections."""
        debug=True

        if globalTag=="" and jetFlavour=="" and jerPath=="":
            globalTag="Summer15_25nsV6_MC"
            jetFlavour="AK4PFchs"
            jerPath="../../data/jer/Summer15_25nsV6_MC"
            
        self.globalTag = globalTag # "Summer15_25nsV6_DATA" or "Summer15_25nsV6_MC"
        self.jetFlavour = jetFlavour # "AK4PFchs" 
        self.jerPath = jerPath 
#        self.type1METParams  = type1METParams
        path = os.path.expandvars(jerPath) #"%s/src/CMGTools/XZZ2l2nu/data/jer" % os.environ['CMSSW_BASE'];

        print '[Debug] I am JetResolution.py: ',path
        print "[Debug] %s/%s_PtResolution_%s.txt" % (path,globalTag,jetFlavour)
     # Step1 : Construct a FactorizedJetResObject object
        self.JetResObject = ROOT.JME.JetResolutionObject("%s/%s_PtResolution_%s.txt" % (path,globalTag,jetFlavour))
        self.JetSFObject = ROOT.JME.JetResolutionObject("%s/%s_SF_%s.txt" % (path,globalTag,jetFlavour))
        
        if debug : 
            self.JetResObject.dump() 
        # Step2 : initialize JetParameters object for pt and eta (rho if it is sf)
        
        self.resPar = ROOT.JME.JetParameters()
        self.sfPar = ROOT.JME.JetParameters()
#        self.L1JetPar  = ROOT.JetCorrectorParameters("%s/%s_L1FastJet_%s.txt" % (path,globalTag,jetFlavour),"");
#        self.L2JetPar  = ROOT.JetCorrectorParameters("%s/%s_L2Relative_%s.txt" % (path,globalTag,jetFlavour),"");
#        self.L3JetPar  = ROOT.JetCorrectorParameters("%s/%s_L3Absolute_%s.txt" % (path,globalTag,jetFlavour),"");
#        self.vPar = ROOT.vector(ROOT.JetCorrectorParameters)()
        

    def getResolution(self,jet,rho,delta=0,resolution=None):
        if not resolution: resolution = self.JetResObject
        if resolution != self.JetResObject and delta!=0: raise RuntimeError, 'Configuration not supported'
        self.resPar.setJetPt(jet.pt())
        self.resPar.setJetEta(jet.eta())
        self.resPar.setRho(rho)
        print '[Debug] I am in JetResolution.py to getResolution for jet(pT, eta) = (',jet.pt() ,jet.eta(),'), ', 'rho = ', rho  
        if not resolution.getRecord(self.resPar):
            res=1
        else:
            res=resolution.evaluateFormula(resolution.getRecord(self.resPar), self.resPar)
        print '[Debug] I am in JetResolution.py: res = ',res
        return res

    def getResolution(self,jet,rho,delta=0,resolution=None):
        if not resolution: resolution = self.JetResObject
        if resolution != self.JetResObject and delta!=0: raise RuntimeError, 'Configuration not supported'
        self.resPar.setJetPt(jet.pt())
        self.resPar.setJetEta(jet.eta())
        self.resPar.setRho(rho)
        print '[Debug] I am in JetResolution.py to getResolution for jet(pT, eta) = (',jet.pt() ,jet.eta(),'), ', 'rho = ', rho  
        if not resolution.getRecord(self.resPar):
            res=1
        else:
            res=resolution.evaluateFormula(resolution.getRecord(self.resPar), self.resPar)
        print '[Debug] I am in JetResolution.py: res = ',res
        return res


    # def rawP4forType1MET_(self, jet):
    #     """Return the raw 4-vector, after subtracting the muons (if requested),
    #        or None if the jet fails the EMF cut."""
    #     p4 = jet.p4() * jet.rawFactor()
    #     emf = ( jet.physObj.neutralEmEnergy() + jet.physObj.chargedEmEnergy() )/p4.E()
    #     if emf > self.type1METParams['skipEMfractionThreshold']:
    #         return None
    #     if self.type1METParams['skipMuons']:
    #         for idau in xrange(jet.numberOfDaughters()):
    #             pfcand = jet.daughter(idau)
    #             if pfcand.isGlobalMuon() or pfcand.isStandAloneMuon(): 
    #                 p4 -= pfcand.p4()
    #     return p4

#     def correct(self,jet,rho,delta=0,addCorr=False,addShifts=False, metShift=[0,0],type1METCorr=[0,0,0]):
#         """Corrects a jet energy (optionally shifting it also by delta times the JEC uncertainty)

#            If addCorr, set jet.corr to the correction.
#            If addShifts, set also the +1 and -1 jet shifts 

#            The metShift vector will accumulate the x and y changes to the MET from the JEC, i.e. the 
#            negative difference between the new and old jet momentum, for jets eligible for type1 MET 
#            corrections, and after subtracting muons. The pt cut is applied to the new corrected pt.
#            This shift can be applied on top of the *OLD TYPE1 MET*, but only if there was no change 
#            in the L1 corrections nor in the definition of the type1 MET (e.g. jet pt cuts).

#            The type1METCorr vector, will accumulate the x, y, sumEt type1 MET corrections, to be
#            applied to the *RAW MET* (if the feature was turned on in the constructor of the class).
#         """
# #        raw = jet.rawFactor()
#         corr = self.getCorrection(jet,rho,delta)
#         if addCorr: 
#             jet.corr = corr
#             for sepcorr in self.separateJetCorrectors.keys():
#                 setattr(jet,"CorrFactor_"+sepcorr,self.getCorrection(jet,rho,delta=0,corrector=self.separateJetCorrectors[sepcorr]))
#         if addShifts:
#             for cdelta,shift in [(1.0, "JECUp"), (-1.0, "JECDown")]:
#                 cshift = self.getCorrection(jet,rho,delta+cdelta)
#                 setattr(jet, "corr"+shift, cshift)
#         if corr <= 0:
#             return False
#         # newpt = jet.pt()*raw*corr
#         # if newpt > self.type1METParams['jetPtThreshold']:
#         #     rawP4forT1 = self.rawP4forType1MET_(jet)
#         #     if rawP4forT1 and rawP4forT1.Pt()*corr > self.type1METParams['jetPtThreshold']:
#         #         metShift[0] -= rawP4forT1.Px() * (corr - 1.0/raw)
#         #         metShift[1] -= rawP4forT1.Py() * (corr - 1.0/raw)
#         #         if self.calculateType1METCorr:
#         #             l1corr = self.getCorrection(jet,rho,delta=0,corrector=self.separateJetCorrectors["L1"])
#         #             #print "\tfor jet with raw pt %.5g, eta %.5g, dpx = %.5g, dpy = %.5g" % (
#         #             #            jet.pt()*raw, jet.eta(), 
#         #             #            rawP4forT1.Px()*(corr - l1corr), 
#         #             #            rawP4forT1.Py()*(corr - l1corr))
#         #             type1METCorr[0] -= rawP4forT1.Px() * (corr - l1corr) 
#         #             type1METCorr[1] -= rawP4forT1.Py() * (corr - l1corr) 
#         #             type1METCorr[2] += rawP4forT1.Et() * (corr - l1corr) 
#         jet.setCorrP4(jet.p4() * (corr * raw))
#         return True

    # def correctAll(self,jets,rho,delta=0, addCorr=False, addShifts=False, metShift=[0.,0.], type1METCorr=[0.,0.,0.]):
    #     """Applies 'correct' to all the jets, discard the ones that have bad corrections (corrected pt <= 0)"""
    #     badJets = []
    #     if metShift     != [0.,0.   ]: raise RuntimeError, "input metShift tuple is not initialized to zeros"
    #     if type1METCorr != [0.,0.,0.]: raise RuntimeError, "input type1METCorr tuple is not initialized to zeros"
    #     for j in jets:
    #         ok = self.correct(j,rho,delta,addCorr=addCorr,addShifts=addShifts,metShift=metShift,type1METCorr=type1METCorr)
    #         if not ok: badJets.append(j)
    #     if len(badJets) > 0:
    #         print "Warning: %d out of %d jets flagged bad by JEC." % (len(badJets), len(jets))
    #     for bj in badJets:
    #         jets.remove(bj)

# class Type1METCorrector:
#     def __init__(self, old74XMiniAODs):
#         """Object to apply type1 corrections computed by the JetReCalibrator to the MET.
#            old74XMiniAODs should be True if using inputs produced with CMSSW_7_4_11 or earlier."""
#         self.oldMC = old74XMiniAODs
#     def correct(self,met,type1METCorrections):
#         oldpx, oldpy = met.px(), met.py()
#         #print "old met: px %+10.5f, py %+10.5f" % (oldpx, oldpy)
#         if self.oldMC:
#             raw  = met.shiftedP2_74x(12,0);
#             rawsumet =  met.shiftedSumEt_74x(12,0);
#         else:
#             raw = met.uncorP2()
#             rawsumet =  met.uncorSumEt();
#         rawpx, rawpy = raw.px, raw.py
#         #print "raw met: px %+10.5f, py %+10.5f" % (rawpx, rawpy)
#         corrpx = rawpx + type1METCorrections[0]
#         corrpy = rawpy + type1METCorrections[1]
#         corrsumet = rawsumet  + type1METCorrections[2]
#         #print "done met: px %+10.5f, py %+10.5f\n" % (corrpx,corrpy)
#         met.setP4(ROOT.reco.Particle.LorentzVector(corrpx,corrpy,0,hypot(corrpx,corrpy)))
#         ## workaround for missing setSumEt in reco::MET and pat::MET
#         met._sumEt = corrsumet
#         met.sumEt = types.MethodType(lambda myself : myself._sumEt, met, met.__class__) 
#         if not self.oldMC:
#             met.setCorShift(rawpx, rawpy, rawsumet, met.Raw)
#         else: 
#             # to avoid segfauls in pat::MET, I need a more ugly solution
#             setFakeRawMETOnOldMiniAODs(met, rawpx,rawpy, rawsumet)

# def setFakeRawMETOnOldMiniAODs(met, rawpx, rawpy, rawsumet):
#         met._rawSumEt = rawsumet
#         met._rawP4    = ROOT.reco.Particle.LorentzVector(rawpx,rawpy,0,hypot(rawpx,rawpy))
#         met.uncorPt  = types.MethodType(lambda myself : myself._rawP4.Pt(), met, met.__class__)
#         met.uncorPx  = types.MethodType(lambda myself : myself._rawP4.Px(), met, met.__class__)
#         met.uncorPy  = types.MethodType(lambda myself : myself._rawP4.Py(), met, met.__class__)
#         met.uncorPhi = types.MethodType(lambda myself : myself._rawP4.Phi(), met, met.__class__)
#         met.uncorP4  = types.MethodType(lambda myself : myself._rawP4, met, met.__class__)
#         met.uncorSumEt = types.MethodType(lambda myself : myself._rawSumEt, met, met.__class__)
#         # the two below are a bit more tricky, but probably less needed, but something dummy
#         met.uncorP2 = types.MethodType(lambda myself : None, met, met.__class__)
#         met.uncorP3 = types.MethodType(lambda myself : None, met, met.__class__)

