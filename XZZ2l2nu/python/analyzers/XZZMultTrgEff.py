from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import *

class TrgObj(object):
    def __init__(self, pt=-100, eta=-100, phi=-100, dR=100):
        self.pt=pt
        self.eta=eta
        self.phi=phi
        self.dR=dR
        self.HLT=0

class XZZMultTrgEff(Analyzer):
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(XZZMultTrgEff, self).__init__(cfg_ana, cfg_comp, looperName)
        self.HLTlist=cfg_ana.HLTlist if hasattr(cfg_ana,'HLTlist') else []

    def declareHandles(self):
        super(XZZMultTrgEff, self).declareHandles()
        self.handles['trgresults']=AutoHandle(("TriggerResults","","HLT"),"edm::TriggerResults")
        self.handles['selectedtrg']=AutoHandle('selectedPatTrigger','std::vector<pat::TriggerObjectStandAlone>')

    def process(self,event):
        self.readCollections( event.input )
        Zll = event.LLNuNu[0]['pair'].leg1
        names = event.input.object().triggerNames(self.handles['trgresults'].product())
        trgoblist=self.handles['selectedtrg'].product()
        Zll.leg1.triggerob=TrgObj()
        Zll.leg2.triggerob=TrgObj()
        for i in trgoblist:
            lepdrs=[deltaR(Zll.leg1.eta(),Zll.leg1.phi(),i.eta(),i.phi()),deltaR(Zll.leg2.eta(),Zll.leg2.phi(),i.eta(),i.phi())]
            if lepdrs[0]<lepdrs[1]:
                if lepdrs[0]<min(0.3,Zll.leg1.triggerob.dR): 
                    Zll.leg1.triggerob=TrgObj(i.pt(),i.eta(),i.phi(), lepdrs[0])
                    Zll.leg1.triggerobj=i
            else:
                if lepdrs[1]<min(0.3,Zll.leg2.triggerob.dR): 
                    Zll.leg2.triggerob=TrgObj(i.pt(),i.eta(),i.phi(), lepdrs[1])
                    Zll.leg2.triggerobj=i
        if Zll.leg1.triggerob.dR!=100:
            Zll.leg1.triggerobj.unpackPathNames(names)
            pNames=list(Zll.leg1.triggerobj.pathNames())
            for n,i in enumerate(self.HLTlist):
                if [pN for pN in pNames if i in pN]: Zll.leg1.triggerob.HLT|=1<<n
        if Zll.leg2.triggerob.dR!=100:
            Zll.leg2.triggerobj.unpackPathNames(names)
            pNames=list(Zll.leg2.triggerobj.pathNames())
            for n,i in enumerate(self.HLTlist):
                if [pN for pN in pNames if i in pN]: Zll.leg2.triggerob.HLT|=1<<n
        return True


