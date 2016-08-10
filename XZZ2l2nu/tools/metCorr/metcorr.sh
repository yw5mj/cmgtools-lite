#!/bin/sh

file=DYJetsToLL_M50
#file=SingleEMU_Run2016BCD_PromptReco_
#file=BulkGravToZZToZlepZinv_narrow_1000
config=config/parameters.conf
# compile
inputs
inputdir=/data2/XZZ2/80X_20160721_EffSkim_v2
outputdir=/data2/XZZ2/80X_20160721_EffSkim_v2_MetFit
mkdir -p ${outputdir}
infile=${inputdir}/${file}.root
outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepHardOnly.root
 

echo -- Command: ./bin/metcorr.exe $infile $outfile $config

./bin/metcorr.exe $infile $outfile $config
