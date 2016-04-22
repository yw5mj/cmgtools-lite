#!/bin/sh


# selection
selection="((abs(llnunu_l1_l1_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.1&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4)||(abs(llnunu_l1_l1_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5)&&(llnunu_l1_pt>100.0&&abs(llnunu_l1_mass-91.1876)<20.0&&met_pt>0.0))"


# compile
g++ skimming.cc -o skimming.exe `root-config --cflags` `root-config --libs`

#inputs
inputdir=/afs/cern.ch/work/m/mewu/public/76X_new
outputdir=AnalysisRegion
mkdir -p ${outputdir}

for infile in `ls $inputdir/*/vvTreeProducer/tree.root` ; 
do
  echo "+++ skimming $infile +++"
  outfile="${outputdir}/${infile/$inputdir\//}"
  outfile="${outfile/\/vvTreeProducer\/tree/}"

  inSkimFile=${infile/vvTreeProducer\/tree.root/skimAnalyzerCount\/SkimReport.txt}

  #echo $inSkimFile
  AllEvents=`grep "All Events" ${inSkimFile} | awk {'print $3'}`
  SumWeights=`grep "Sum Weights" ${inSkimFile} | awk {'print $3'}`

  ./skimming.exe $infile $outfile $AllEvents $SumWeights $selection

done

