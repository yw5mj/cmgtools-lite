#!/bin/env python

# import ROOT in batch mode
import ROOT

# load FWLite C++ libraries
ROOT.gSystem.Load("libFWCoreFWLite.so");
ROOT.gSystem.Load("libDataFormatsFWLite.so");
ROOT.FWLiteEnabler.enable()

# load FWlite python libraries
from DataFormats.FWLite import Handle, Events

photons, photonLabel = Handle("std::vector<pat::Photon>"), "slimmedPhotons"

events = Events("afile.root")


for iev,event in enumerate(events):
    event.getByLabel(photonLabel, photons)

    # Photon
    for i,pho in enumerate(photons.product()):
        print "pho.recHits().size(): ",pho.recHits().size()

