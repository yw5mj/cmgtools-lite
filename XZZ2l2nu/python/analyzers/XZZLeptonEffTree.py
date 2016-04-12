import ROOT
import random
import math
from  itertools import combinations
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import *
import PhysicsTools.HeppyCore.framework.config as cfg
from CMGTools.XZZ2l2nu.tools.Pair import *


class XZZLeptonEffTree( Analyzer ):
    def process(self, event):
        self.readCollections( event.input )
        event.llpair=[]
        if len(event.selectedElectrons)>2 and event.selectedElectrons[0].pdgId()==-event.selectedElectrons[1].pdgId():
            event.llpair.append(Pair(event.selectedElectrons[0],event.selectedElectrons[1],23))
        if len(event.selectedMuons)>2 and event.selectedMuons[0].pdgId()==-event.selectedMuons[1].pdgId():
            event.llpair.append(Pair(event.selectedMuons[0],event.selectedMuons[1],23))
        if not event.llpair: return False
        return True




        
            

        


                
                
