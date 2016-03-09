from ROOT import *
gROOT.SetBatch()
def deltR(i,j):
    dp=t.Lep_phi[i]-t.Lep_phi[j]
    while dp<-3.1415926:dp+=6.2831852
    while dp>3.1415926:dp-=6.2831852
    de=t.Lep_eta[i]-t.Lep_eta[j]
    return (de**2+dp**2)**.5

def geteff(a,b,n=4):
    if a.GetSize()!=b.GetSize():
        print 'ERROR: Different bin number!'
        return TH1F()
    e=TH1F('','',a.GetSize()-2,a.GetXaxis().GetXmin(),a.GetXaxis().GetXmax())
    for i in range(1,a.GetSize()-1): 
        a0=a.GetBinContent(i)
        b0=b.GetBinContent(i)
        if b0:
            e.SetBinContent(i,100.*a0/b0)
            a1=b0-a0
            e.SetBinError(i,((a1**3+a0**3)/b0**4)**.5*100)
    e.SetMarkerColor(n)
    e.SetLineColor(n)
    return e

if __name__=='__main__':
    gStyle.SetOptStat(000000)
    gStyle.SetLegendBorderSize(0)
    c=TCanvas('c','c',800,800)
    flnm='/afs/cern.ch/work/y/yanchu/graviton/CMSSW_7_6_3_patch2/src/CMGTools/XZZ2l2nu/cfg/genlepeff/all.root'
#    flnm='/afs/cern.ch/work/y/yanchu/graviton/CMSSW_7_6_3_patch2/src/CMGTools/XZZ2l2nu/cfg/test/BulkGravToZZToZlepZhad_narrow_1600/lepTreeProducer/tree.root'
    lst=[TH1F('','',20,0,.4) for i in range(5)]
    f=TFile(flnm)
    t=f.Get('tree')
    for i in range(t.GetEntries()):
        t.GetEntry(i)
        fx=[x for x in range(t.nLep) if t.Lep_isfromX[x]==1 and abs(t.Lep_pdgId[x])==11]
        if len(fx)!=2: continue
        dr=deltR(fx[0],fx[1])
        lst[0].Fill(dr)
#        if t.Lep_looseelectronnoiso[fx[0]]==1 and abs(t.Lep_eta[fx[0]])<2.5 and abs(t.Lep_looseisoelectron[fx[0]]==1 and t.Lep_looseelectron[fx[0]]==0): print i
        if t.Lep_looseelectronnoiso[fx[0]]==1 and t.Lep_looseelectronnoiso[fx[1]]==1:lst[1].Fill(dr)
        if t.Lep_looseelectron[fx[0]]==1 and t.Lep_looseelectron[fx[1]]==1:lst[2].Fill(dr)
        if t.Lep_heepV60_noISO[fx[0]]==1 and t.Lep_heepV60_noISO[fx[1]]==1:
            lst[3].Fill(dr)
            if t.Lep_miniRelIso[fx[0]]<0.1 and t.Lep_miniRelIso[fx[1]]<0.1:lst[4].Fill(dr)
    f.Close()
    lsnoiso=geteff(lst[1],lst[0],1)
    lsiso=geteff(lst[2],lst[0],2)
    heepnoiso=geteff(lst[3],lst[0],3)
    heepiso=geteff(lst[4],lst[0],4)
    lsnoiso.SetTitle("electron efficiency;deltaR;efficiency [%]")
    leg=TLegend(.91,.8,1,.9)
    leg.AddEntry(lsnoiso,"loose no iso")
    leg.AddEntry(lsiso,"loose with iso")
    leg.AddEntry(heepnoiso,"heep no iso")
    leg.AddEntry(heepiso,"heep with iso")
    lsnoiso.Draw('e')
    lsiso.Draw('esame')
    heepnoiso.Draw('esame')
    heepiso.Draw('esame')
    leg.Draw('same')
    c.Print('deltR.png')

