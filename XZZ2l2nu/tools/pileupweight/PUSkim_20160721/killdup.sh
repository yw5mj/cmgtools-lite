#!/bin/sh


g++ killdup.cc -o killdup.exe `root-config --cflags` `root-config --libs`

#inputs
inputdir=80X_20160721_LinksForSkim_v2
outputdir=/data/XZZ/80X_20160721_Skim
mkdir -p ${outputdir}

infile=/data/XZZ/80X_20160721_Skim/SingleEMU_Run2016BCD_PromptReco.root
outfile=/data/XZZ/80X_20160721_Skim/SingleEMU_Run2016BCD_PromptReco_killdup.root

#infile=/data2/XZZ/80X_20160721/SingleEMU_Run2016BCD_PromptReco/vvTreeProducer/tree.root
#outfile=/data2/XZZ/80X_20160721/SingleEMU_Run2016BCD_PromptReco/vvTreeProducer/tree_killdup.root


echo -- Command: ./killdup.exe $infile $outfile
./killdup.exe $infile $outfile

