#!/usr/bin/env python

import ROOT
import os

### Please keep the keys same in tex_dic={}  and cuts={}

Tex_dic={'SR': 'Region A', 'CRb': 'Region B','CRc': 'Region C','CRd': 'Region D'}

def Cuts(channel, whichRegion="", isPreSelect=False):
    if whichRegion=="": whichRegion=raw_input("Please choose a benchmarck Region (SR or VR): \n")
    if whichRegion=='SR':
        preSelection='(abs(llnunu_l1_mass-91.1876)<20.0&&llnunu_l1_pt>200.0&&met_pt>50)'
    elif whichRegion=='VR':
        preSelection='(abs(llnunu_l1_mass-91.1876)<20.0&&llnunu_l1_pt>200.0&&met_pt<50)'
    else:
        print "I do not understand your benchmark Region, should be either 'SR' for MET>50GeV or 'VR' for MET<50GeV\n"
        sys.exit(0)
        
    pdgID={'el':'11','mu':'13' }
    if isPreSelect:
        cuts='('+preSelection+'&&abs(llnunu_l1_l1_pdgId)=='+pdgID[channel]+')'
    else:
        dR_ll={'el':'0.6','mu':'0.6'}
        dPhi_ZMET='2.5'
        var1='llnunu_deltaPhi'
        var2='llnunu_l1_deltaR'
        
        cuts_a='('+preSelection+'&&abs(llnunu_l1_l1_pdgId)=='+pdgID[channel]+'&&'+var1+'>'+dPhi_ZMET+'&&'+var2+'<'+dR_ll[channel]+')'
        cuts_c='('+preSelection+'&&abs(llnunu_l1_l1_pdgId)=='+pdgID[channel]+'&&'+var1+'>'+dPhi_ZMET+'&&'+var2+'>'+dR_ll[channel]+')'
        cuts_b='('+preSelection+'&&abs(llnunu_l1_l1_pdgId)=='+pdgID[channel]+'&&'+var1+'<'+dPhi_ZMET+'&&'+var2+'<'+dR_ll[channel]+')'
        cuts_d='('+preSelection+'&&abs(llnunu_l1_l1_pdgId)=='+pdgID[channel]+'&&'+var1+'<'+dPhi_ZMET+'&&'+var2+'>'+dR_ll[channel]+')'
        cuts={'SR':cuts_a, 'CRb':cuts_b, 'CRc':cuts_c, 'CRd':cuts_d}
    
    #cuts_test='1'
    return cuts

def PreSelections(channel):
    whichRegion=raw_input("Please choose a benchmarck Region (SR or VR): \n")
    if whichRegion=='SR':
        preSelection='(abs(llnunu_l1_mass-91.1876)<20.0&&llnunu_l1_pt>200.0&&met_pt>50)'
    elif whichRegion=='VR':
        preSelection='(abs(llnunu_l1_mass-91.1876)<20.0&&llnunu_l1_pt>200.0&&met_pt<50)'
    else:
        print "I do not understand your benchmark Region, should be either 'SR' for MET>50GeV or 'VR' for MET<50GeV\n"
        sys.exit(0)

    pdgID={'el':'11','mu':'13' }    
    cuts='('+preSelection+'&&abs(llnunu_l1_l1_pdgId)=='+pdgID[channel]+')'
    
    return cuts
    
