#!/bin/sh

#mbs="670 630 600 570 530 500"
mbs="665 660 655 650 645 640 635 630 625 620"
for mb in $mbs;
do
  pileupCalc.py -i Cert_271036-273730_13TeV_PromptReco_Collisions16_JSON.txt --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/pileup_latest.txt --calcMode true --minBiasXsec ${mb}00 --maxPileupBin 50 --numPileupBins 50  pileup_DATA_80x_${mb}.root

  ./mcpileup80x.py -m ${mb}
done
