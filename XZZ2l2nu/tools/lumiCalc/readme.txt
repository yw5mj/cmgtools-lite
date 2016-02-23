#!/bin/sh

#http://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html

export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:$PATH

pip install --install-option="--prefix=$HOME/.local" brilws

pip show brilws

brilcalc -h

briltag -h

brilcalc lumi -h

brilcalc beam -h

# calculate int lumi for 2015D
brilcalc lumi --normtag /afs/cern.ch/user/c/cmsbril/public/normtag_json/OfflineNormtagV1.json -i Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2_2015D.txt

# the output is in lumi_2015D.txt, L=2153 pb-1 


# calculate int lumi for re-reco 2015C 25ns + 2015D
# rereco golden json: /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Reprocessing/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON.txt
brilcalc lumi --normtag /afs/cern.ch/user/c/cmsbril/public/normtag_json/OfflineNormtagV1.json -i Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON.txt
# the output is in lumi_rereoco_2015CD25ns.txt, L=2169 pb-1

OfflineNormtagV2.json 
