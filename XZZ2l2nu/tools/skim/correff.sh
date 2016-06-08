#!/bin/sh


#file=DYJetsToLL_M50_MGMLM_Ext1
file=DYJetsToLL_M50_ZPt
# compile
g++ correff.cc -o correff.exe `root-config --cflags` `root-config --libs`

#inputs
inputdir=/data/XZZ/80X_Ntuple/80X_20160606_NoHLT_Skim
outputdir=/home/heli/work/XZZ/80X_Ntuple/80X_20160606_NoHLT_Skim
#outputdir=/data/XZZ/80X_Ntuple/80X_20160606_NoHLT_Skim
mkdir -p ${outputdir}
infile=${inputdir}/${file}.root
outfile=${outputdir}/${file}_Effv2.root
  
echo -- Command: ./correff.exe $infile $outfile

./correff.exe $infile $outfile

