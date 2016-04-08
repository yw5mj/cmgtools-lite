import ROOT
import os, types
from math import *
from PhysicsTools.HeppyCore.utils.deltar import *

class JetResolution:
    def __init__(self,globalTag="",jetFlavour="",jerPath="" ,debug=False):
        """Create resolution and scalefactor objects to read the payloads from the text dumps from global tag under
            CMGTools/XZZ2l2nu/data/jer  (provided from https://github.com/cms-jet/JRDatabase/tree/master/textFiles/Fall15_25nsV2_MC).
           It will jet PtResolution and resolution scale factors for jets.
           Receipt from 
           https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyResolution
           """
        if globalTag=="" and jetFlavour=="" and jerPath=="": raise RuntimeError, 'Configuration not supported'
            
        self.globalTag = globalTag # "Summer15_25nsV6_DATA" or "Summer15_25nsV6_MC"
        self.jetFlavour = jetFlavour # "AK4PFchs" 
        self.jerPath = jerPath #"%s/src/CMGTools/XZZ2l2nu/data/jer" % os.environ['CMSSW_BASE'];
        path = os.path.expandvars(jerPath) 
        if debug:
            print '[Debug] I am JetResolution.py: ',path
            print "[Debug] %s/%s_PtResolution_%s.txt" % (path,globalTag,jetFlavour)
            print "[Debug] %s/%s_SF_%s.txt" % (path,globalTag,jetFlavour)

        # Step1 : Construct a FactorizedJetResObject object
        self.JetResObject = ROOT.JME.JetResolutionObject("%s/%s_PtResolution_%s.txt" % (path,globalTag,jetFlavour))
        self.JetSFObject = ROOT.JME.JetResolutionObject("%s/%s_SF_%s.txt" % (path,globalTag,jetFlavour))
        if debug : 
            self.JetResObject.dump() 
            self.JetSFObject.dump()

        # Step2 : initialize JetParameters object for pt and eta (rho if it is sf)
        self.resPar = ROOT.JME.JetParameters()
        self.sfPar = ROOT.JME.JetParameters()
       

    def getResolution(self,jet,rho,resolution=None):
        if not resolution: resolution = self.JetResObject
        if resolution != self.JetResObject : raise RuntimeError, 'Configuration not supported'
        self.resPar.setJetPt(jet.pt())
        self.resPar.setJetEta(jet.eta())
        self.resPar.setRho(rho)
#        print '[Debug] I am in JetResolution.py to getResolution for jet(pT, eta) = (',jet.pt() ,jet.eta(),'), ', 'rho = ', rho  
        res = 1.0
        if resolution.getRecord(self.resPar):
            res = resolution.evaluateFormula(resolution.getRecord(self.resPar), self.resPar)
        else:
            print '[Info] [JetResolution.py] no resolution record for [jet (pT, eta), rho] = [(%.2f, %.2f), %.2f]' % (jet.pt(), jet.eta(),rho)
#        print '[Debug] I am in JetResolution.py: res = ',res
        return res

    def getScaleFactor(self,jet,variation='norminal',scalefactor=None):

        Variation={'norminal':0,'down':1,'up':2} 

        if not scalefactor: scalefactor = self.JetSFObject
        if scalefactor != self.JetSFObject : raise RuntimeError, 'Configuration not supported'
        self.sfPar.setJetEta(jet.eta())
#        print '[Debug] I am in JetResolution.py to getScaleFactor for jet(eta) = (',jet.eta(),')'
        res_sf = 1.0
        if scalefactor.getRecord(self.sfPar):
            res_sf = scalefactor.getRecord(self.sfPar).getParametersValues().at(Variation[variation])
        else:
            print '[Info] [JetResolution.py] no scale factor record for jet(pT, eta) = (%.2f, %.2f)' % (jet.pt(), jet.eta()) 
#            print '[Debug] I am in JetResolution.py: res_sf = ',res_sf
        return res_sf


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

