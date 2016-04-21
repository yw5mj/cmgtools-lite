#!/usr/bin/env python

import ROOT
import os
from python.TreePlotter import TreePlotter
from python.MergedPlotter import MergedPlotter
from python.myStackPlotter import StackPlotter
from python.mylib import *

Channel=raw_input("Please choose a channel (inclusive, el or mu): \n")
pdgID={'el':'11','mu':'13'}
metcut=raw_input("Please choose a met cut (no cut applied if you skip): \n")
MetCut='&&met_pt>'+metcut  if metcut else ''

xMax=50
xMin=0
nbins=25
outdir='./plots/aux'
indir="../AnalysisRegion"
lumi=2.169126704526
sepSig=True
LogY=True
doRatio=True
if not os.path.exists(outdir): os.system('mkdir '+outdir)

#######----------- Prepare samples to plot:
zjetsPlotters=[]
zjetsSamples = ['DYJetsToLL_M50_BIG'] # M50_BIG = M50 + M50_Ext

for sample in zjetsSamples:
    zjetsPlotters.append(TreePlotter(indir+'/'+sample+'.root','tree'))
    zjetsPlotters[-1].addCorrectionFactor('1./SumWeights','tree')
    zjetsPlotters[-1].addCorrectionFactor('xsec','tree')
    zjetsPlotters[-1].addCorrectionFactor('genWeight','tree')
    zjetsPlotters[-1].addCorrectionFactor('puWeight','tree')
    
ZJets = MergedPlotter(zjetsPlotters)
ZJets.setFillProperties(1001,ROOT.kGreen+2)
    
#######----------- Start Plotting:
ROOT.gROOT.ProcessLine('.x tdrstyle.C')

if Channel=='inclusive':
    factor_cuts='(abs(llnunu_l1_mass-91.1876)<20.0&&llnunu_l1_pt>200.0'+MetCut+')'
else:
    factor_cuts='((abs(llnunu_l1_mass-91.1876)<20.0&&llnunu_l1_pt>200.0)&&abs(llnunu_l1_l1_pdgId)=='+pdgID[Channel]+MetCut+')'


print '[info] cuts used here: ', factor_cuts

h2_dR_dphi=ZJets.drawTH2("llnunu_l1_deltaR:llnunu_deltaPhi",factor_cuts,str(lumi*1000),
                         32,0,3.2, 32,0,3.2,
                         titlex = "#Delta#Phi_{Z,MET}",unitsx = "",
                         titley = "#Delta R_{l,l}",unitsy = "",drawStyle = "COLZ")

ROOT.gStyle.SetPadBottomMargin(0.15);
ROOT.gStyle.SetPadLeftMargin(0.15);
ROOT.gStyle.SetPadRightMargin(0.12);
ROOT.gStyle.SetTitleXOffset(0.5);
ROOT.gStyle.SetTitleYOffset(0.5);
c1=ROOT.TCanvas("c1","c1",1)

h2_dR_dphi.Draw("COLZ")
print "llnunu_l1_deltaR:llnunu_deltaPhi, correlation factor:", h2_dR_dphi.GetCorrelationFactor()
c1.SaveAs(outdir+"/h2_dRll_dPhiZMet_"+Channel+".eps")
