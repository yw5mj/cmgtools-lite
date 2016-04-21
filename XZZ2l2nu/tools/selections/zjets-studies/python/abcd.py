#!/usr/bin/env python

import ROOT
import os,copy
from python.TreePlotter import TreePlotter
from python.MergedPlotter import MergedPlotter
from python.myStackPlotter import StackPlotter
import python.SetCuts
from python.mylib import *

class abcdAnalyzer:
    def __init__(self, indir="../AnalysisRegion", outdir='plots',
                 lumi = 2.169126704526,  sepSig=True,
                 LogY=True,   doRatio=True):
        if not os.path.exists(outdir): os.system('mkdir '+outdir)

        self.Channel=raw_input("Please choose a channel (el or mu): \n")
        self.outdir = outdir
        self.lumi = lumi
        self.sepSig = sepSig
        self.tex_dic = python.SetCuts.Tex_dic
        self.whichregion=raw_input("Please choose a benchmarck Region (SR or VR): \n")
        self.cuts = python.SetCuts.Cuts(self.Channel, self.whichregion)
        self.preCuts = python.SetCuts.Cuts(self.Channel, self.whichregion, True)
        
        #######----------- Prepare samples to plot:
        wwPlotters=[]
        wwSamples = ['WWTo2L2Nu','WWToLNuQQ','WZTo1L1Nu2Q']
        
        for sample in wwSamples:
            wwPlotters.append(TreePlotter(indir+'/'+sample+'.root','tree'))
            wwPlotters[-1].addCorrectionFactor('1./SumWeights','tree')
            wwPlotters[-1].addCorrectionFactor('xsec','tree')
            wwPlotters[-1].addCorrectionFactor('genWeight','tree')
            wwPlotters[-1].addCorrectionFactor('puWeight','tree')
            
        WW = MergedPlotter(wwPlotters)
        WW.setFillProperties(1001,ROOT.kOrange)
            
            
        vvPlotters=[]
        vvSamples = ['WZTo2L2Q','WZTo3LNu',
                     'ZZTo2L2Nu',
                     'ZZTo2L2Q','ZZTo4L']
            
        for sample in vvSamples:
            vvPlotters.append(TreePlotter(indir+'/'+sample+'.root','tree'))
            vvPlotters[-1].addCorrectionFactor('1./SumWeights','tree')
            vvPlotters[-1].addCorrectionFactor('xsec','tree')
            vvPlotters[-1].addCorrectionFactor('genWeight','tree')
            vvPlotters[-1].addCorrectionFactor('puWeight','tree')
            
        VV = MergedPlotter(vvPlotters)
        VV.setFillProperties(1001,ROOT.kMagenta)
            
        wjetsPlotters=[]
        wjetsSamples = ['WJetsToLNu']
            
        for sample in wjetsSamples:
            wjetsPlotters.append(TreePlotter(indir+'/'+sample+'.root','tree'))
            wjetsPlotters[-1].addCorrectionFactor('1./SumWeights','tree')
            wjetsPlotters[-1].addCorrectionFactor('xsec','tree')
            wjetsPlotters[-1].addCorrectionFactor('genWeight','tree')
            wjetsPlotters[-1].addCorrectionFactor('puWeight','tree')

        WJets = MergedPlotter(wjetsPlotters)
        WJets.setFillProperties(1001,ROOT.kBlue-6)

        zjetsPlotters=[]
        #zjetsSamples = ['DYJetsToLL_M50_HT100to200','DYJetsToLL_M50_HT200to400','DYJetsToLL_M50_HT400to600','DYJetsToLL_M50_HT600toInf']
        #zjetsSamples = ['DYJetsToLL_M50','DYJetsToLL_M50_Ext']
        zjetsSamples = ['DYJetsToLL_M50_BIG'] # M50_BIG = M50 + M50_Ext

        for sample in zjetsSamples:
            zjetsPlotters.append(TreePlotter(indir+'/'+sample+'.root','tree'))
            zjetsPlotters[-1].addCorrectionFactor('1./SumWeights','tree')
            zjetsPlotters[-1].addCorrectionFactor('xsec','tree')
            zjetsPlotters[-1].addCorrectionFactor('genWeight','tree')
            zjetsPlotters[-1].addCorrectionFactor('puWeight','tree')

        self.ZJets = MergedPlotter(zjetsPlotters)
        self.ZJets.setFillProperties(1001,ROOT.kGreen+2)

        ttPlotters=[]
        ttSamples = ['TTTo2L2Nu']#,'TTZToLLNuNu','TTWJetsToLNu']

        for sample in ttSamples:
            ttPlotters.append(TreePlotter(indir+'/'+sample+'.root','tree'))
            ttPlotters[-1].addCorrectionFactor('1./SumWeights','tree')
            ttPlotters[-1].addCorrectionFactor('xsec','tree')
            ttPlotters[-1].addCorrectionFactor('genWeight','tree')
            ttPlotters[-1].addCorrectionFactor('puWeight','tree')
            
        TT = MergedPlotter(ttPlotters)
        TT.setFillProperties(1001,ROOT.kAzure-9)

        sigPlotters=[]
        sigSamples = [
            'BulkGravToZZToZlepZinv_narrow_800', 
            'BulkGravToZZToZlepZinv_narrow_1000', 
            'BulkGravToZZToZlepZinv_narrow_1200', 
        ]
        k=1000
        sigSampleNames = [
            str(k)+' x BulkG-800',
            str(k)+' x BulkG-1000',
            str(k)+' x BulkG-1200',
        ]
        sigXsec = {
            'BulkGravToZZToZlepZinv_narrow_800'  : 4.42472e-04*k,
            'BulkGravToZZToZlepZinv_narrow_1000' : 1.33926e-04*k,
            'BulkGravToZZToZlepZinv_narrow_1200' : 4.76544e-05*k,
        }

        for sample in sigSamples:
            sigPlotters.append(TreePlotter(indir+'/'+sample+'.root','tree'))
            sigPlotters[-1].addCorrectionFactor('1./SumWeights','tree')
            sigPlotters[-1].addCorrectionFactor(str(sigXsec[sample]),'tree')
            sigPlotters[-1].addCorrectionFactor('genWeight','tree')
            sigPlotters[-1].addCorrectionFactor('puWeight','tree')
            sigPlotters[-1].setFillProperties(0,ROOT.kWhite)


        dataPlotters=[]
        dataSamples = ['SingleElectron_Run2015C_25ns_16Dec',
                       'SingleElectron_Run2015D_16Dec',
                       'SingleMuon_Run2015C_25ns_16Dec',
                       'SingleMuon_Run2015D_16Dec']
        for sample in dataSamples:
            dataPlotters.append(TreePlotter(indir+'/'+sample+'.root','tree'))
            
        Data = MergedPlotter(dataPlotters)

        self.Stack = StackPlotter()
        self.Stack.addPlotter(Data, "data_obs", "Data", "data")
        #Stack.addPlotter(WJets, "WJets","W+Jets", "background")
        self.Stack.addPlotter(WW, "WW","WW, WZ non-reson.", "background")
        self.Stack.addPlotter(TT, "TT","TT", "background")
        self.Stack.addPlotter(VV, "ZZ","ZZ, WZ reson.", "background")
        self.Stack.addPlotter(self.ZJets, "ZJets","Z+Jets", "background")
        
        for i in range(len(sigSamples)):
            sigPlotters[i].setLineProperties(2,ROOT.kRed+i,2)
            self.Stack.addPlotter(sigPlotters[i],sigSamples[i],sigSampleNames[i],'signal')  

 
        self.Stack.setLog(LogY)
        self.Stack.doRatio(doRatio)
        ROOT.gROOT.ProcessLine('.x tdrstyle.C') 

#######----------- Start Plotting:
    def draw_preselection(self):
        
        xMax, xMin, nbins = 50, 0, 25
        #preCuts=python.SetCuts.PreSelections(self.Channel)

        tag='PreSelection_'+self.Channel+'_'
        #print self.Stack.log
        
        if self.Stack.log: tag=tag+'log_'
        
        self.Stack.drawStack('met_pt', self.preCuts, str(self.lumi*1000), nbins, xMin, xMax, titlex = "E_{T}^{miss}", units = "GeV",
                        output=tag+'met_low',outDir=self.outdir,separateSignal=self.sepSig,
                        drawtex=self.whichregion+" pre-selection", channel=self.Channel)
        self.Stack.drawStack('llnunu_l1_deltaR', self.preCuts, str(self.lumi*1000), 30,0,3, titlex = "#Delta R(l,l)", units = "",
                        output=tag+'dR_ll',outDir=self.outdir,separateSignal=self.sepSig,
                        drawtex=self.whichregion+" pre-selection", channel=self.Channel)
        self.Stack.drawStack('llnunu_deltaPhi', self.preCuts, str(self.lumi*1000), 35,0,3.5, titlex = "#Delta#Phi(Z,E_{T}^{miss})", units = "",
                        output=tag+'dPhi_llvv',outDir=self.outdir,separateSignal=self.sepSig,
                        drawtex=self.whichregion+" pre-selection", channel=self.Channel)
        self.Stack.drawStack('llnunu_l1_deltaPhi', self.preCuts, str(self.lumi*1000), 30,0,3, titlex = "#Delta#Phi(l,l)", units = "",
                        output=tag+'dPhi_ll',outDir=self.outdir,separateSignal=self.sepSig,
                        drawtex=self.whichregion+" pre-selection", channel=self.Channel)
        self.Stack.drawStack('llnunu_l1_mass', self.preCuts, str(self.lumi*1000), 50, 50, 150, titlex = "M_{ll}", units = "GeV",
                        output=tag+'zmass',outDir=self.outdir,separateSignal=self.sepSig,
                        drawtex=self.whichregion+" pre-selection", channel=self.Channel)
        self.Stack.drawStack('llnunu_mt', self.preCuts, str(self.lumi*1000), 30, 0.0, 600.0, titlex = "M_{T}^{ll#nu#nu}", units = "GeV",
                        output=tag+'mt_low',outDir=self.outdir,separateSignal=self.sepSig,
                        drawtex=self.whichregion+" pre-selection", channel=self.Channel)
        #self.Stack.drawStack('llnunu_mt', self.preCuts, str(self.lumi*1000), 60, 0.0, 1200.0, titlex = "M_{T}^{ll#nu#nu}", units = "GeV", output=tag+'mt',outDir=self.outdir,separateSignal=self.sepSig,drawtex=self.whichregion+" pre-selection", channel=self.Channel)
        #self.Stack.drawStack('llnunu_mt', self.preCuts, str(self.lumi*1000), 150, 0.0, 3000.0, titlex = "M_{T}^{ll#nu#nu}", units = "GeV", output=tag+'mt_high',outDir=self.outdir,separateSignal=self.sepSig,drawtex=self.whichregion+" pre-selection", channel=self.Channel)
    
        # merge all output plots into one pdf file
        os.system('gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile='+self.outdir+'/'+tag+'all.pdf '+self.outdir+'/'+tag+'*.eps')
        print 'All plots merged in single pdf file '+tag+'.pdf .'
        # merge root file
        os.system('hadd -f '+self.outdir+'/'+tag+'all.root '+self.outdir+'/'+tag+'*.root')
        return

    def draw_BCD(self):
        #cuts=python.SetCuts.Cuts(self.Channel)

        xMax, xMin, nbins = 50, 0, 25
        for key in self.tex_dic:
            if ROOT.TString(key).Contains("SR"):
                continue
            else:
                tag = key+'_'+self.Channel+'_'
                if self.Stack.log: tag=tag+'log_'
                else: pass
                self.Stack.drawStack('llnunu_mt', self.cuts[key], str(self.lumi*1000), 60, 0.0, 1200.0, titlex = "M_{T}", units = "GeV",
                                     output=tag+'mt',outDir=self.outdir,separateSignal=self.sepSig,
                                     drawtex=self.tex_dic[key],channel=self.Channel)
                self.Stack.drawStack('llnunu_mt', self.cuts[key], str(self.lumi*1000), 150, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",
                                     output=tag+'mt_high',outDir=self.outdir,separateSignal=self.sepSig,
                                     drawtex=self.tex_dic[key],channel=self.Channel)
                self.Stack.drawStack('llnunu_l1_pt', self.cuts[key], str(self.lumi*1000), 100, 0, 1000, titlex = "p_{T}^{ll}", units = "GeV",
                                     output=tag+'pt',outDir=self.outdir,separateSignal=self.sepSig,
                                     drawtex=self.tex_dic[key],channel=self.Channel)
                self.Stack.drawStack('met_pt', self.cuts[key], str(self.lumi*1000), nbins, xMin, xMax, titlex = "E_{T}^{miss}", units = "GeV",
                                     output=tag+'met_low',outDir=self.outdir,separateSignal=self.sepSig,
                                     drawtex=self.tex_dic[key],channel=self.Channel)
                # merge all output plots into one pdf file
                os.system('gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile='+self.outdir+'/'+tag+'all.pdf '+self.outdir+'/'+tag+'*.eps')
                print 'All plots merged in single pdf file '+tag+'.pdf .'
                # merge root file
                os.system('hadd -f '+self.outdir+'/'+tag+'all.root '+self.outdir+'/'+tag+'*.root')
        return

    def draw_A(self, sf_bd = 0.0):
        if sf_bd == 0.0: sf_bd=ROOT.Double(raw_input("Please give the scale factor derived from B,D regions: \n"))
        else: pass
        xMax, xMin, nbins = 50, 0, 25
        tag ='regionA_'+self.Channel+'_'

        self.Stack.rmPlotter(self.ZJets, "ZJets","Z+Jets", "background")
        self.Stack.drawStack('met_pt', self.cuts['SR'], str(self.lumi*1000), nbins, xMin, xMax,
                              titlex = "E_{T}^{miss}", units = "GeV",output=tag+'met_low',outDir=self.outdir,separateSignal=self.sepSig,
                              drawtex=self.tex_dic['SR'],channel=self.Channel)
        
        file=ROOT.TFile(self.outdir+'/'+tag+'met_low.root')
        h_met_zjets=self.ZJets.drawTH1('met_pt',self.cuts['CRc'],str(self.lumi*1000),nbins,xMin,xMax,titlex='E_{T}^{miss}',units='GeV',drawStyle="HIST")
        h_met_zjets.Scale(sf_bd)
        
        hframe=file.Get(tag+'met_low_frame')
        hs=file.Get(tag+'met_low_stack')
        hs.Add(h_met_zjets)
        hdata=file.Get(tag+'met_low_data')
        hratio=GetRatio_TH1(hdata,hs,True)
    
        legend=file.Get(tag+'met_low_legend')
        myentry=ROOT.TLegendEntry(h_met_zjets,"Z+jets(data-driven)","f")
        legend.GetListOfPrimitives().AddFirst(myentry)

        # Let's remove the signal entries in the legend 
        for i in legend.GetListOfPrimitives():
            if ROOT.TString(i.GetLabel()).Contains("Bulk"):
                legend.GetListOfPrimitives().Remove(i)
                
                
        drawStack_simple(hframe, hs, hdata,hratio,legend,
                         hstack_opt="A, HIST",
                         outDir=self.outdir, output=self.whichregion+'_regionA_'+self.Channel+'_met_low',channel=ROOT.TString(self.Channel),
                         xmin=xMin, xmax=xMax, xtitle="E_{T}^{miss}" ,units="GeV",
                         lumi=self.lumi, notes="Region A ("+self.whichregion+")")


        self.Stack.addPlotter(self.ZJets, "ZJets","Z+Jets", "background")
        return
        
