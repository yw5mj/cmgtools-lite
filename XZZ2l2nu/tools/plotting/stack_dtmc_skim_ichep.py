#!/usr/bin/env python

import ROOT
import os, string, math, pickle
from CMGTools.XZZ2l2nu.plotting.TreePlotter import TreePlotter
from CMGTools.XZZ2l2nu.plotting.MergedPlotter import MergedPlotter
from CMGTools.XZZ2l2nu.plotting.StackPlotter import StackPlotter


tag="Tight_DYDataYields_BigDY_NoRecoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="Tight_DYDataYields_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="TightZPt100_DYDataYields_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="TightZPtLt200_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="TightZPt100MET50_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="TightZPt100MET50_Norm_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="TightZPt100MET50_TrgUp_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="TightZPt100MET50_TrgDn_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="TightZPt100MET50_IDUp_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="TightZPt100MET50_IDDn_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="TightZPt100MET50_ZPtUp_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="TightZPt100MET50_ZPtDn_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="test_TgEfElFineBin_TightZPt100_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="TgEfElFineBin_TightZPt100_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="ResBos_TightZPt100_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="TightZPt100_BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="LODY_Sept1RecoilNoSmooth_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="LODY_Sept1RecoilSmooth_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="NoElTrgEff_Sept1RecoilNoSmooth_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="Sept1RecoilSmooth_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="LODY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="BigDY_Sept1RecoilNoSmooth_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="BigDY_Sept1RecoilSmooth_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="BigDY_Sept1Recoil_GenCutZPtwt_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_"
#tag="NewRecoilNoPUWtNoSmooth_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="BIGDY_NewRecoil_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="NewRecoilNoSmooth_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="LODY_NewRecoilNoSmooth_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="NoRecoil_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="LODY_NewRecoil_ElEscale_NewPuMuSf_MuPtScale_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="ElEscale_LOZJets_NewPuMuSf_MuPtScale_MCRecoilGraph_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="ElEscale_MergedBigZJets_NewPuMuSf_MuPtScale_MCRecoilGraph_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="TkHptId_NewPuMuSf_MuPtScale_MCRecoilGraph_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="HptId_NewPuMuSf_MuPtScale_MCRecoilGraph_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="BigZJets_NewPuMuSf_MuPtScale_MCRecoilGraph_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="Test_MCRecoilSmooth_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="BigZJetsMC_NewRecoil_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="BigZJetsMC_MCRecoilSmooth_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="BigZJetsMC_MCRecoilGraph_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="MCRecoilNoSmooth_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="Test_NewRecoil_NTgEf_80X_L12p9_HLTv2_ichepPU_ZPTwt_allSF_"
#tag="DataHLT_oldLumiJson_"
#tag="test_"
#tag=""
#cutChain='loosecut'
cutChain='tight'
#cutChain='tightzpt100'
#cutChain='tightzptlt200'
#cutChain='tightzpt100met50'
#cutChain='tightzpt100met100'
#cutChain='tightzpt100met200'

# can be el or mu or both
channel='el' 
LogY=True
test=False
DrawLeptons=True
doRhoScale=True
doSys=False

lepsf="trgsf*idisotrksf"
#lepsf="trgsf_up*idisotrksf"
#lepsf="trgsf_dn*idisotrksf"
#lepsf="trgsf*idisotrksf_up"
#lepsf="trgsf*idisotrksf_dn"

ZPtWeight="ZPtWeight"
#ZPtWeight="ZPtWeight_up"
#ZPtWeight="ZPtWeight_dn"


if test: DrawLeptons = False

#lepsf="(1)"
#lepsf="idsf"
#lepsf="trgsf"
#lepsf="isosf"
#lepsf="isosf*idsf"
#lepsf="trgsf*isosf*idsf*trksf"
#lepsf="(trgsf*isosf*idsf*((1.03128*(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11))+(0.908956*(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13))))"
#lepsf="(trgsf*((isosf*idsf)*(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11)+(highpt_dt_eff_m1/highpt_mc_eff_m1*highpt_dt_eff_m2/highpt_mc_eff_m2)*(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13)))"
#lepsf="(trgsf*trksf*((isosf*idsf)*(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11)+(tkhighpt_dt_eff_m1/tkhighpt_mc_eff_m1*tkhighpt_dt_eff_m2/tkhighpt_mc_eff_m2)*(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13)))"

#if channel=='el' : lepsf="isosf*idsf*trksf"

if doRhoScale: 
    tag+="RhoWt_"
    lepsf=lepsf+"*(0.602*exp(-0.5*pow((rho-8.890)/6.187,2))+0.829*exp(-0.5*pow((rho-21.404)/10.866,2)))"
    #lepsf=lepsf+"*(0.122360+0.180976*rho+-0.010879*pow(rho,2)+0.000226*pow(rho,3)-0.000002*pow(rho,4))"

outdir='plots'

indir='/home/heli/XZZ/80X_20160825_light_Skim'
#indir='/home/heli/XZZ/80X_20160818_light_Skim_EffSkim'
#indir='root://eoscms//store/caf/user/heli/XZZ/80X_20160721_EffSkim_v2'
#indir='/home/heli/XZZ/80X_20160721_SkimV2_EffSkim'
#indir='/home/heli/XZZ/80X_20160721_SkimV2_EffSkim'
#indir='/data/XZZ/80X_20160721_SkimV2_EffSkim'
#indir='/data/XZZ/80X_20160721_SkimV2_EffSkim'
#indir='/dataf/heli/XZZ/80X_20160721_EffSkim_v2'
#indir='/data2/XZZ2/80X_20160721_EffSkim_v2'
#indir='/datag/heli/XZZ/80X_20160705_L9p17_EffSkim'
#indir='/home/heli/XZZ/80X_20160721_EffSkim_v2'
lumi=12.9
sepSig=True
doRatio=True
Blind=True
FakeData=False
UseMETFilter=True
SignalAll1pb=True
puWeight='puWeight68075'
#puWeight='puWeight61651'
#puWeight='puWeight62118'
#puWeight='puWeight62154'
#puWeight='puWeight61665'
#puWeight='puWeight68715'
#puWeight='puWeight62525'
#puWeight='puWeight62127'
#puWeight='puWeight'
ZJetsZPtWeight=True
DataHLT=True
k=1 # signal scale

elChannel='(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11)'
muChannel='(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13)'

if not os.path.exists(outdir): os.system('mkdir '+outdir)

tag = tag+cutChain+'_'
tag = tag+puWeight+'_'

if doSys: tag = tag+"sys_"

if UseMETFilter: tag = tag+'metfilter_'

if not Blind: tag = tag+'unblind_'

tag = tag+channel+'_'
if LogY: tag = tag+'log_'
if SignalAll1pb:
    tag += '1pb'
else:
    tag += 'scale'+str(k)

#tag += '_'


paveText="#sqrt{s} = 13 TeV 2016 L = "+"{:.3}".format(float(lumi))+" fb^{-1}"

#metfilter='(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_CSCTightHalo2015Filter&&Flag_eeBadScFilter)'
metfilter='(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter)'

cuts_loose='(nllnunu)'
#cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4)"
cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99))"
#cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&llnunu_l1_l1_highPtID==1&&llnunu_l1_l2_highPtID==1)"
#cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&llnunu_l1_l1_trackerHighPtID==1&&llnunu_l1_l2_trackerHighPtID==1)"
cuts_lepaccept+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5))"
cuts_zmass="(llnunu_l1_mass>70&&llnunu_l1_mass<110)"
cuts_zpt100="(llnunu_l1_pt>100)"
cuts_zptlt200="(llnunu_l1_pt<200)"
cuts_met50="(llnunu_l2_pt>50)"
cuts_met100="(llnunu_l2_pt>100)"
cuts_met200="(llnunu_l2_pt>200)"
cuts_loose_z="("+cuts_lepaccept+"&&"+cuts_zmass+")"
cuts_loose_zll="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+")"
cuts_loose_zll_zptlt200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zptlt200+")"
cuts_loose_zll_met50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met50+")"
cuts_loose_zll_met100="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met100+")"
cuts_loose_zll_met200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met200+")"


if cutChain=='loosecut': cuts=cuts_loose
elif cutChain=='tight': cuts=cuts_loose_z
elif cutChain=='tightzpt100': cuts=cuts_loose_zll
elif cutChain=='tightzpt100met50': cuts=cuts_loose_zll_met50
elif cutChain=='tightzpt100met100': cuts=cuts_loose_zll_met100
elif cutChain=='tightzpt100met200': cuts=cuts_loose_zll_met200
elif cutChain=='tightzptlt200': cuts=cuts_loose_zll_zptlt200
else : cuts=cuts_loose


if channel=='el': cuts = cuts+'&&'+elChannel
elif channel=='mu': cuts = cuts+'&&'+muChannel

if UseMETFilter:
    cuts = '('+cuts+'&&'+metfilter+')'

ROOT.gROOT.ProcessLine('.x tdrstyle.C') 

#allSamples = ['WWTo2L2Nu','WWToLNuQQ','WZTo1L1Nu2Q',
#            'WZTo2L2Q','WZTo3LNu',
#'ZZTo2L2Nu',
#'ZZTo2L2Q','ZZTo4L',
#'DYJetsToLL_M50_BIG',
#'TTTo2L2Nu',
#'BulkGravToZZToZlepZinv_narrow_600',
#'BulkGravToZZToZlepZinv_narrow_800',
#'BulkGravToZZToZlepZinv_narrow_1000',
#'BulkGravToZZToZlepZinv_narrow_1200',
#'BulkGravToZZToZlepZinv_narrow_1400',
#'BulkGravToZZToZlepZinv_narrow_1600', 
#'BulkGravToZZToZlepZinv_narrow_1800', 
#'BulkGravToZZToZlepZinv_narrow_2000',
#'BulkGravToZZToZlepZinv_narrow_2500',
#'BulkGravToZZToZlepZinv_narrow_3000',
#'BulkGravToZZToZlepZinv_narrow_3500', 
#'BulkGravToZZToZlepZinv_narrow_4000', 
#'BulkGravToZZToZlepZinv_narrow_4500', 
#]

allPlotters = {}


wwPlotters=[]
#wwSamples = ['WWTo2L2Nu','WWToLNuQQ','WZTo1L1Nu2Q','WJetsToLNu']
wwSamples = ['WWTo2L2Nu','WWToLNuQQ_BIG','WZTo1L1Nu2Q']

for sample in wwSamples:
    wwPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wwPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wwPlotters[-1].addCorrectionFactor('xsec','xsec')
    wwPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wwPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wwPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    allPlotters[sample] = wwPlotters[-1]

WW = MergedPlotter(wwPlotters)
WW.setFillProperties(1001,ROOT.kOrange)


vvPlotters=[]
vvSamples = ['WZTo2L2Q','WZTo3LNu_AMCNLO',
'ZZTo2L2Nu',
'ZZTo2L2Q','ZZTo4L']

for sample in vvSamples:
    vvPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    vvPlotters[-1].addCorrectionFactor('1/SumWeights','norm')
    vvPlotters[-1].addCorrectionFactor('xsec','xsec')
    vvPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    vvPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    vvPlotters[-1].addCorrectionFactor(lepsf, 'lepsf')
    allPlotters[sample] = vvPlotters[-1]

VV = MergedPlotter(vvPlotters)
VV.setFillProperties(1001,ROOT.kMagenta)

#ggZZPlotters=[]
#ggZZSamples = ['ggZZTo2e2nu','ggZZTo2mu2nu']

#for sample in ggZZSamples:
#    ggZZPlotters.append(TreePlotter(indir+'/'+sample+'.root','tree'))
#    ggZZPlotters[-1].addCorrectionFactor('1./SumWeights','tree')
#    ggZZPlotters[-1].addCorrectionFactor('xsec','tree')
#    ggZZPlotters[-1].addCorrectionFactor('genWeight','genWeight')
#    ggZZPlotters[-1].addCorrectionFactor(puWeight,'tree')
#    ggZZPlotters[-1].addCorrectionFactor('(llnunu_l1_l1_lepsf*llnunu_l1_l2_lepsf)','tree')
#    ggZZPlotters[-1].addCorrectionFactor('triggersf','tree')
#    allPlotters[sample] = ggZZPlotters[-1]

#ggZZ = MergedPlotter(ggZZPlotters)
#ggZZ.setFillProperties(1001,ROOT.kRed)



wjetsPlotters=[]
wjetsSamples = ['WJetsToLNu']

for sample in wjetsSamples:
    wjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
    wjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')

WJets = MergedPlotter(wjetsPlotters)
WJets.setFillProperties(1001,ROOT.kBlue-6)


zjetsPlotters=[]
#zjetsSamples = ['DYJetsToLL_M50_RecoilNoSmooth','DYJetsToLL_M50_MGMLM_Ext1_RecoilNoSmooth']
#zjetsSamples = ['DYJetsToLL_M50_RecoilSmooth','DYJetsToLL_M50_MGMLM_Ext1_RecoilSmooth']
#zjetsSamples = ['DYJetsToLL_M50','DYJetsToLL_M50_MGMLM_Ext1']
zjetsSamples = ['DYJetsToLL_M50_NoRecoil','DYJetsToLL_M50_MGMLM_Ext1_NoRecoil']
#zjetsSamples = ['DYJetsToLL_M50_TgEfElFineBin','DYJetsToLL_M50_MGMLM_Ext1_TgEfElFineBin']
#zjetsSamples = ['DYJetsToLL_M50_Resbos','DYJetsToLL_M50_MGMLM_Ext1_Resbos']
#zjetsSamples = ['DYJetsToLL_M50'] # M50
#zjetsSamples = ['DYJetsToLL_M50_RecoilSmooth'] # M50
#zjetsSamples = ['DYJetsToLL_M50_RecoilNoSmooth'] # M50
#zjetsSamples = ['DYJetsToLL_M50_MGMLM_Ext1_RecoilSmooth'] # M50
#zjetsSamples = ['DYJetsToLL_M50_MGMLM_Ext1_RecoilNoSmooth'] # M50
#zjetsSamples = ['DYJetsToLL_M50_MGMLM_Ext1'] # M50
#zjetsSamples = ['DYJetsToLL_M50_NoRecoil'] # M50
#zjetsSamples = ['GJet_Pt_20toInf_DoubleEMEnriched'] 


for sample in zjetsSamples:
    zjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    #zjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    zjetsPlotters[-1].addCorrectionFactor('(1)','norm')
    if ZJetsZPtWeight: zjetsPlotters[-1].addCorrectionFactor(ZPtWeight,'ZPtWeight')
    #zjetsPlotters[-1].addCorrectionFactor('PhiStarWeight','PhiStarWeight')
    #zjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
    zjetsPlotters[-1].addCorrectionFactor('(1921.8*3)','xsec') # FEWZ NNLO.results_z_m50_nnlo_inclusive_NNPDF30_nlo_as_0118
    #zjetsPlotters[-1].addCorrectionFactor('(1907.0*3)','xsec') # FEWZ NNLO.results_z_m50_nnlo_fsrOn_lowstat_inclusive_NNPDF30_nlo_as_0118
    #zjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    zjetsPlotters[-1].addCorrectionFactor("ZJetsGenWeight",'genWeight')
    zjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    zjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    if channel=='el' :
        zjetsPlotters[-1].addCorrectionFactor('(14522.2806340000/13832.429603)','scale') #el
        #zjetsPlotters[-1].addCorrectionFactor('(14523.8/13576.479992)','scale') #el
    elif channel=='mu' :
        zjetsPlotters[-1].addCorrectionFactor('(2152385.6374089997/2132746.799724)','scale') #mu
        #zjetsPlotters[-1].addCorrectionFactor('(2152468.6819809997/2122399.011136)','scale') #mu
    else :
        zjetsPlotters[-1].addCorrectionFactor('(2166907.9180429997/2146579.229327)','scale') #all
        #zjetsPlotters[-1].addCorrectionFactor('(2166992.5014429996/2135975.491128)','scale') #all
    allPlotters[sample] = zjetsPlotters[-1]



ZJets = MergedPlotter(zjetsPlotters)
ZJets.setFillProperties(1001,ROOT.kGreen+2)

ttPlotters=[]
#ttSamples = ['TTTo2L2Nu']
ttSamples = ['TTTo2L2Nu','TTZToLLNuNu','TTWJetsToLNu']

for sample in ttSamples:
    ttPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    ttPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    ttPlotters[-1].addCorrectionFactor('xsec','xsec')
    ttPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    ttPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    ttPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    allPlotters[sample] = ttPlotters[-1]

TT = MergedPlotter(ttPlotters)
TT.setFillProperties(1001,ROOT.kAzure-9)

sigPlotters=[]
sigSamples = [
'BulkGravToZZToZlepZinv_narrow_600',
#'BulkGravToZZToZlepZinv_narrow_800',
'BulkGravToZZToZlepZinv_narrow_1000',
#'BulkGravToZZToZlepZinv_narrow_1200',
#'BulkGravToZZToZlepZinv_narrow_1400',
#'BulkGravToZZToZlepZinv_narrow_1600', 
#'BulkGravToZZToZlepZinv_narrow_1800', 
'BulkGravToZZToZlepZinv_narrow_2000',
#'BulkGravToZZToZlepZinv_narrow_2500',
#'BulkGravToZZToZlepZinv_narrow_3000',
#'BulkGravToZZToZlepZinv_narrow_3500', 
#'BulkGravToZZToZlepZinv_narrow_4000', 
#'BulkGravToZZToZlepZinv_narrow_4500', 
]


sigSampleNames = {
'BulkGravToZZToZlepZinv_narrow_600':str(k)+' x BulkG-600',
'BulkGravToZZToZlepZinv_narrow_800':str(k)+' x BulkG-800',
'BulkGravToZZToZlepZinv_narrow_1000':str(k)+' x BulkG-1000',
'BulkGravToZZToZlepZinv_narrow_1200':str(k)+' x BulkG-1200',
'BulkGravToZZToZlepZinv_narrow_1400':str(k)+' x BulkG-1400',
'BulkGravToZZToZlepZinv_narrow_1600':str(k)+' x BulkG-1600',
'BulkGravToZZToZlepZinv_narrow_1800':str(k)+' x BulkG-1800',
'BulkGravToZZToZlepZinv_narrow_2000':str(k)+' x BulkG-2000',
'BulkGravToZZToZlepZinv_narrow_2500':str(k)+' x BulkG-2500',
'BulkGravToZZToZlepZinv_narrow_3000':str(k)+' x BulkG-3000',
'BulkGravToZZToZlepZinv_narrow_3500':str(k)+' x BulkG-3500',
'BulkGravToZZToZlepZinv_narrow_4000':str(k)+' x BulkG-4000',
'BulkGravToZZToZlepZinv_narrow_4500':str(k)+' x BulkG-4500',
}

BulkGZZ2l2nuXsec = {
600:8.61578e-03,
800:1.57965e-03,
1000:4.21651e-04,
1200:1.39919e-04,
1400:5.32921e-05,
1600:2.24428e-05,
1800:1.01523e-05,
2000:4.86037e-06,
2500:9.08739e-07,
3000:1.98856e-07,
3500:4.87505e-08,
4000:1.25937e-08,
4500:1.0,
}

sigXsec = {
'BulkGravToZZToZlepZinv_narrow_600'  : BulkGZZ2l2nuXsec[600]*k,
'BulkGravToZZToZlepZinv_narrow_800'  : BulkGZZ2l2nuXsec[800]*k,
'BulkGravToZZToZlepZinv_narrow_1000' : BulkGZZ2l2nuXsec[1000]*k,
'BulkGravToZZToZlepZinv_narrow_1200' : BulkGZZ2l2nuXsec[1200]*k,
'BulkGravToZZToZlepZinv_narrow_1400' : BulkGZZ2l2nuXsec[1400]*k,
'BulkGravToZZToZlepZinv_narrow_1600' : BulkGZZ2l2nuXsec[1600]*k,
'BulkGravToZZToZlepZinv_narrow_1800' : BulkGZZ2l2nuXsec[1800]*k,
'BulkGravToZZToZlepZinv_narrow_2000' : BulkGZZ2l2nuXsec[2000]*k,
'BulkGravToZZToZlepZinv_narrow_2500' : BulkGZZ2l2nuXsec[2500]*k,
'BulkGravToZZToZlepZinv_narrow_3000' : BulkGZZ2l2nuXsec[3000]*k,
'BulkGravToZZToZlepZinv_narrow_3500' : BulkGZZ2l2nuXsec[3500]*k,
'BulkGravToZZToZlepZinv_narrow_4000' : BulkGZZ2l2nuXsec[4000]*k,
'BulkGravToZZToZlepZinv_narrow_4500' : BulkGZZ2l2nuXsec[4500]*k,
}

if SignalAll1pb:
    for sig in sigSamples:
        sigXsec[sig] = 1.0
        sigSampleNames[sig] = string.replace(sigSampleNames[sig], str(k)+' x', '1 pb')

for sample in sigSamples:
    sigPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    sigPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    sigPlotters[-1].addCorrectionFactor(str(sigXsec[sample]),'xsec')
    sigPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    sigPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    sigPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    sigPlotters[-1].setFillProperties(0,ROOT.kWhite)
    allPlotters[sample] = sigPlotters[-1]

dataPlotters=[]
dataSamples = [
#'SingleMuon_Run2016B_PromptReco',
#'SingleElectron_Run2016B_PromptReco',
#'SingleMuon_Run2016B_PromptReco_v2',
#'SingleElectron_Run2016B_PromptReco_v2',
#'SingleMuon_Run2016C_PromptReco_v2',
#'SingleElectron_Run2016C_PromptReco_v2',
#'SingleMuon_Run2016D_PromptReco_v2',
#'SingleElectron_Run2016D_PromptReco_v2',
#'SingleEMU_Run2016BCD_PromptReco_killdup', 
#'SingleEMU_Run2016BCD_PromptReco_noRecoil', 
#'SingleEMU_Run2016BCD_PromptReco_newRecoil', 
#'SingleEMU_Run2016BCD_PromptReco_killdup_old', 
'SingleEMU_Run2016BCD_PromptReco', 
]
for sample in dataSamples:
    dataPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))

if DataHLT:
    dataPlotters[0].addCorrectionFactor('(HLT_MUv2||HLT_ELEv2)','HLT')
#    dataPlotters[0].addCorrectionFactor('(HLT_MU)','HLT')
#    dataPlotters[1].addCorrectionFactor('(HLT_ELE&&!HLT_MU)','HLT')
#    dataPlotters[2].addCorrectionFactor('(HLT_MU)','HLT')
#    dataPlotters[3].addCorrectionFactor('(HLT_ELE&&!HLT_MU)','HLT')
#    dataPlotters[4].addCorrectionFactor('(HLT_MU)','HLT')
#    dataPlotters[5].addCorrectionFactor('(HLT_ELE&&!HLT_MU)','HLT')
#    dataPlotters[6].addCorrectionFactor('(HLT_MU)','HLT')
#    dataPlotters[7].addCorrectionFactor('(HLT_ELE&&!HLT_MU)','HLT')


Data = MergedPlotter(dataPlotters)




Stack = StackPlotter(outTag=tag, outDir=outdir)
Stack.setPaveText(paveText)
Stack.addPlotter(Data, "data_obs", "Data", "data")
#Stack.addPlotter(WJets, "WJets","W+Jets", "background")
Stack.addPlotter(WW, "NonReso","WW/WZ/WJets non-reson.", "background")
Stack.addPlotter(TT, "TT","TT", "background")
Stack.addPlotter(VV, "VVZReso","ZZ WZ reson.", "background")
#Stack.addPlotter(ggZZ, "ggZZ","ggZZ", "background")
Stack.addPlotter(ZJets, "ZJets","ZJets", "background")


for i in range(len(sigSamples)):
  sigPlotters[i].setLineProperties(2,ROOT.kRed+i,2)
  Stack.addPlotter(sigPlotters[i],sigSamples[i],sigSampleNames[sigSamples[i]],'signal')  

 
Stack.setLog(LogY)
Stack.doRatio(doRatio)



tag+='_'

if test: 
    #Stack.drawStack('llnunu_l1_mass', cuts, str(lumi*1000), 50, 50, 150, titlex = "M(Z)", units = "GeV",output=tag+'zmass',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_l1_pt', cuts, str(lumi*1000), 200, 0.0, 1000.0, titlex = "P_{T}(l_{1})", units = "GeV",output=tag+'pTlep1',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_l2_pt', cuts, str(lumi*1000), 200, 0.0, 500.0, titlex = "P_{T}(l_{2})", units = "GeV",output=tag+'pTlep2',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_l1_trackerIso', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{1})", units = "",output=tag+'ISOlep1_mu',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_l2_trackerIso', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{2})", units = "",output=tag+'ISOlep2_mu',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('abs(llnunu_l1_pt+llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#parallel}(Z,MET)/P_{T}(Z)", units = "",output=tag+'dPTParaRel',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_high3',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    #Stack.drawStack('llnunu_l2_pt', cuts, str(lumi*1000), 100, 0, 300, titlex = "MET", units = "GeV",output=tag+'met_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
#    Stack.drawStack('llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#parallel}", units = "GeV",output=tag+'met_para',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#perp}", units = "GeV",output=tag+'met_perp',outDir=outdir,separateSignal=sepSig)

elif doSys:
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt',outDir=outdir,separateSignal=sepSig)

    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names): 
        if typeP != "data": plotter.changeCorrectionFactor("trgsf_up*idisotrksf","lepsf")
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_trgup',outDir=outdir,separateSignal=sepSig) 

    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
        if typeP != "data": plotter.changeCorrectionFactor("trgsf_dn*idisotrksf","lepsf") 
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_trgdn',outDir=outdir,separateSignal=sepSig) 

    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
        if typeP != "data": plotter.changeCorrectionFactor("trgsf*idisotrksf_up","lepsf") 
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_idup',outDir=outdir,separateSignal=sepSig) 

    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
        if typeP != "data": plotter.changeCorrectionFactor("trgsf*idisotrksf_dn","lepsf") 
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_iddn',outDir=outdir,separateSignal=sepSig) 

    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
        if typeP != "data" and name=="ZJets" : 
            plotter.changeCorrectionFactor("trgsf*idisotrksf","lepsf") 
            plotter.changeCorrectionFactor("ZPtWeight_up","ZPtWeight") 
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_zptup',outDir=outdir,separateSignal=sepSig) 

    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
        if typeP != "data" and name=="ZJets" :
            plotter.changeCorrectionFactor("trgsf*idisotrksf","lepsf")
            plotter.changeCorrectionFactor("ZPtWeight_dn","ZPtWeight")
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_zptdn',outDir=outdir,separateSignal=sepSig)

else: 
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_high3',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 240, 0.0, 1200.0, titlex = "M_{T}", units = "GeV",output=tag+'mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 120, 0.0, 600.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 60, 0.0, 300.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_l2_pt', cuts, str(lumi*1000), 100, 0, 1000, titlex = "MET", units = "GeV",output=tag+'met',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt', cuts, str(lumi*1000), 50, 0, 500, titlex = "MET", units = "GeV",output=tag+'met_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt', cuts, str(lumi*1000), 100, 0, 300, titlex = "MET", units = "GeV",output=tag+'met_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l1_mass', cuts, str(lumi*1000), 50, 50, 150, titlex = "M(Z)", units = "GeV",output=tag+'zmass',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 75, 0.0, 1500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 100, 0.0, 300.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low2',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('TMath::Tan((TMath::Pi()-TMath::Abs(llnunu_l1_deltaPhi))/2.0)*TMath::Sin(TMath::ACos(TMath::TanH((llnunu_l1_l1_eta-llnunu_l1_l2_eta)/2.0)))', cuts, str(lumi*1000), 100, 0.0, 10, titlex = "#phi_{#eta}*", units = "",output=tag+'PhiStar',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('nVert', cuts, str(lumi*1000), 50, 0.0, 50.0, titlex = "N vertices", units = "",output=tag+'nVert',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('rho', cuts, str(lumi*1000), 50, 0.0, 50.0, titlex = "#rho", units = "",output=tag+'rho',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#parallel}", units = "GeV",output=tag+'met_para',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#perp}", units = "GeV",output=tag+'met_perp',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt/sqrt(llnunu_l2_sumEt)', cuts, str(lumi*1000), 100, 0.0, 20.0, titlex = "MET/#sqrt{sumE_{T}}", units = "",output=tag+'metOvSqSET',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi-llnunu_l1_phi))', cuts, str(lumi*1000), 50, 0, 5, titlex = "#Delta#phi(Z,MET)", units = "",output=tag+'dphiZMet',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('cos(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -1, 1, titlex = "cos(#Delta#phi(Z,MET))", units = "",output=tag+'CosdphiZMet',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l1_pt+llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi))', cuts, str(lumi*1000), 100, 0, 1000, titlex = "#Delta P_{T}^{#parallel}(Z,MET)", units = "GeV",output=tag+'dPTPara',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi))', cuts, str(lumi*1000), 100, 0, 100, titlex = "#Delta P_{T}^{#perp}(Z,MET)", units = "GeV",output=tag+'dPTPerp',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l1_pt+llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#parallel}(Z,MET)/P_{T}(Z)", units = "",output=tag+'dPTParaRel',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#perp}(Z,MET)/P_{T}", units = "",output=tag+'dPTPerpRel',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_eta', cuts, str(lumi*1000), 200, -10.0, 10.0, titlex = "#eta(Z) ", units = "",output=tag+'zeta',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(Z)", units = "",output=tag+'zphi',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_mt', cuts, str(lumi*1000), 50, 0.0, 150.0, titlex = "M_{T}(Z)", units = "GeV",output=tag+'zmt',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_deltaPhi', cuts, str(lumi*1000), 50, 0.0, 5.0, titlex = "#Delta#phi", units = "",output=tag+'ZdeltaPhi',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('cos(llnunu_l1_deltaPhi)', cuts, str(lumi*1000), 50, -1, 1, titlex = "Cos(#Delta#phi)", units = "",output=tag+'CosZdeltaPhi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_deltaR', cuts, str(lumi*1000), 50, 0.0, 5.0, titlex = "#Delta R", units = "",output=tag+'ZdeltaR',outDir=outdir,separateSignal=sepSig)
  
    Stack.drawStack('llnunu_l2_phi', cuts, str(lumi*1000), 100, -3.2, 3.2, titlex = "#phi(MET)", units = "",output=tag+'metPhi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_sumEt', cuts, str(lumi*1000), 80, 0.0, 3000.0, titlex = "sumE_{T}", units = "GeV",output=tag+'metSumEt',outDir=outdir,separateSignal=sepSig)

    if channel!='el' and channel!='mu' :
        Stack.drawStack('llnunu_l1_l1_pt', cuts, str(lumi*1000), 200, 0.0, 1000.0, titlex = "P_{T}(l_{1})", units = "GeV",output=tag+'pTlep1',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_eta', cuts, str(lumi*1000), 60, -3.0, 3.0, titlex = "#eta(l_{1})", units = "",output=tag+'etalep1',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(l_{1})", units = "",output=tag+'philep1',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_pt', cuts, str(lumi*1000), 200, 0.0, 500.0, titlex = "P_{T}(l_{2})", units = "GeV",output=tag+'pTlep2',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_eta', cuts, str(lumi*1000), 60, -3.0, 3.0, titlex = "#eta(l_{2})", units = "",output=tag+'etalep2',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(l_{2})", units = "",output=tag+'philep2',outDir=outdir,separateSignal=sepSig)

    if DrawLeptons:
        if channel!='el' :
            Stack.drawStack('llnunu_l1_l1_pt', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 1000.0, titlex = "P_{T}(#mu_{1})", units = "GeV",output=tag+'pTlep1_mu',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l1_eta', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(#mu_{1})", units = "",output=tag+'etalep1_mu',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l1_phi', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(#mu_{1})", units = "",output=tag+'philep1_mu',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l1_trackerIso', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{1})", units = "",output=tag+'ISOlep1_mu',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l2_pt', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(#mu_{2})", units = "GeV",output=tag+'pTlep2_mu',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l2_eta', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(#mu_{2})", units = "",output=tag+'etalep2_mu',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l2_phi', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(#mu_{2})", units = "",output=tag+'philep2_mu',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l2_trackerIso', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{2})", units = "",output=tag+'ISOlep2_mu',outDir=outdir,separateSignal=sepSig)

        if channel!='mu' : 
            Stack.drawStack('llnunu_l1_l1_pt', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 100, 0.0, 1000.0, titlex = "P_{T}(e_{1})", units = "GeV",output=tag+'pTlep1_el',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l1_eta', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(e_{1})", units = "",output=tag+'etalep1_el',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l1_phi', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(e_{1})", units = "",output=tag+'philep1_el',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l1_electronrelIsoea03', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 100, 0.0, 0.2, titlex = "looseISO_{rel}(e_{1})", units = "",output=tag+'ISOlep1_el',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l2_pt', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(e_{2})", units = "GeV",output=tag+'pTlep2_el',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l2_eta', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(e_{2})", units = "",output=tag+'etalep2_el',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l2_phi', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(e_{2})", units = "",output=tag+'philep2_el',outDir=outdir,separateSignal=sepSig)
            Stack.drawStack('llnunu_l1_l2_electronrelIsoea03', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 100, 0.0, 0.2, titlex = "looseISO_{rel}(e_{2})", units = "",output=tag+'ISOlep2_el',outDir=outdir,separateSignal=sepSig)


Stack.closePSFile()


