#!/bin/sh


tag="Test1_Photon_"
cutChains="loosecut"
logdir="log_ph_b2h36p1fbinv"

mkdir -p $logdir


for cutChain in $cutChains;
do
   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --LogY  --test &> ${logdir}/${tag}${cutChain}.log &
#  ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --LogY --Blind  --test &> ${logdir}/${tag}${cutChain}.log &
done
