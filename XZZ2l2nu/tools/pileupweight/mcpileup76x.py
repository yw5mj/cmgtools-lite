#!/usr/bin/env python

from ROOT import *

#pileup pdf from:  https://github.com/cms-sw/cmssw/blob/CMSSW_7_6_3/SimGeneral/MixingModule/python/mix_2015_25ns_FallMC_matchData_PoissonOOTPU_cfi.py
pdf = [0.000108643,
                0.000388957,
                0.000332882,
                0.00038397,
                0.000549167,
                0.00105412,
                0.00459007,
                0.0210314,
                0.0573688,
                0.103986,
                0.142369,
                0.157729,
                0.147685,
                0.121027,
                0.08855,
                0.0582866,
                0.0348526,
                0.019457,
                0.0107907,
                0.00654313,
                0.00463195,
                0.00370927,
                0.0031137,
                0.00261141,
                0.00215499,
                0.00174491,
                0.00138268,
                0.00106731,
                0.000798828,
                0.00057785,
                0.00040336,
                0.00027161,
                0.000176535,
                0.00011092,
                6.75502e-05,
                4.00323e-05,
                2.32123e-05,
                1.32585e-05,
                7.51611e-06,
                4.25902e-06,
                2.42513e-06,
                1.39077e-06,
                8.02452e-07,
                4.64159e-07,
                2.67845e-07,
                1.5344e-07,
                8.68966e-08,
                4.84931e-08,
                2.6606e-08,
                1.433e-08, 
                0.0, 0.0, 0.0]

fdt=TFile('../../data/pileup_DATA.root')
pileup_dt = fdt.Get('pileup')
scl=pileup_dt.Integral()
fmc=TFile('pileup_MC.root','recreate')
pileup_mc=TH1F('pileup','pileup',52,0,52)
pileup_mc.Sumw2()
for i in range(52):    pileup_mc.SetBinContent(i+1,pdf[i])
pileup_mc.Scale(scl/pileup_mc.Integral())
gStyle.SetStatStyle(0)
pileup_dt.SetStats(0)
pileup_mc.SetStats(0)

c1 = TCanvas("c1","c1")
pileup_dt.SetLineColor(kRed)
pileup_dt.SetMarkerColor(kRed)
pileup_dt.SetMarkerStyle(20)
pileup_mc.SetLineColor(kBlue)
pileup_mc.SetMarkerColor(kBlue)
pileup_mc.SetMarkerStyle(20)
lg = TLegend(0.6,0.7,0.85,0.85)
lg.AddEntry(pileup_dt, "Data", "pl")
lg.AddEntry(pileup_mc, "MC", "pl")
pileup_dt.Draw()
pileup_mc.Draw("same")
lg.Draw("same")
c1.SaveAs("pileup76x.eps")
c1.SaveAs("pileup76x.pdf")

pileup_mc.Write()
fmc.Close()
