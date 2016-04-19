from CMGTools.XZZ2l2nu.analyzers.XZZEventInterpretationBase import *
from CMGTools.XZZ2l2nu.tools.Pair import Pair
import itertools
import ROOT
from array import array

class XZZMultiFinalState( XZZEventInterpretationBase ):
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(XZZMultiFinalState,self).__init__(cfg_ana, cfg_comp, looperName)
        eeff=ROOT.TF1('trigeff', '50*[0]*(1+TMath::Erf((x-[1])/(sqrt(2)*[2])))*(1. + TMath::Erf((x-[3])/(sqrt(2)*[4])))')
        ep1=array('d',[4.88895e-01,1.20166e+02,1.17300e+01,1.52207e+02,3.01742e+01])
        ep2=array('d',[4.91171e-01,1.15409e+02,1.03195e+01,1.50434e+02,2.85255e+01])
        self.etrgsf=lambda x:eeff.EvalPar(array('d',[x]),ep1)/eeff.EvalPar(array('d',[x]),ep2) if eeff.EvalPar(array('d',[x]),ep2) else 1
        #msff=ROOT.gROOT.GetFunction("pol5")
        #mp=array('d',[0.98395,-4.05067e-05,1.74509e-06,-1.1423e-08,2.67736e-11,-2.00434e-14])
        self.mtrgsf=lambda x:0.985759

    def gettrigersf(self,zpt,pdgid):
        if abs(pdgid)==11:return self.etrgsf(zpt)
        if abs(pdgid)==13:return self.mtrgsf(zpt)

    def process(self, event):
        super(XZZMultiFinalState,self).process(event)
        self.counters.counter('events').inc('all events')

        LLNuNu=[]

        # do LL+MET combination
        if len(event.LL)>0:
            #take the Z->ll  nearest to the Z mass and the highest pt jets
            bestZ = min(event.LL,key = lambda x: abs(x.M()-91.118))
            VV=Pair(bestZ,event.met)
            if self.selectPairLLNuNu(VV):
                selected = {'pair':VV}
                LLNuNu.append(selected)
                if self.cfg_comp.isMC:
                    event.trgsf = self.gettrigersf(bestZ.pt(),bestZ.leg1.pdgId())

        if len(LLNuNu)>0: 
             self.counters.counter('events').inc('pass events')

        setattr(event,'LLNuNu'+self.cfg_ana.suffix,LLNuNu)

