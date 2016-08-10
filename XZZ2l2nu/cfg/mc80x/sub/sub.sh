#!/bin/sh

#heppy_batch.py -o mc2 run_xzz2l2nu_80x_cfg_loose_mc.py -b 'bsub -q 2nd < ./batchScript.sh'
heppy_batch.py -o mc2 run_xzz2l2nu_80x_cfg_photon_mc.py -b 'bsub -q ssssss < ./batchScript.sh'

