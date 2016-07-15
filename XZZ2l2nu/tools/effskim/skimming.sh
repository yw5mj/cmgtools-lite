#!/bin/sh


# selection
selection="(1)"
#selection="((abs(llnunu_l1_l1_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.1&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4)||(abs(llnunu_l1_l1_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5)&&(llnunu_l1_pt>100.0&&abs(llnunu_l1_mass-91.1876)<20.0&&met_pt>0.0))"
#selection="((abs(llnunu_l1_l1_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.1&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4)||(abs(llnunu_l1_l1_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5)&&(llnunu_l1_pt>0.0&&abs(llnunu_l1_mass-91.1876)<20.0&&met_pt>0.0))"


# compile
g++ skimming.cc -o skimming.exe `root-config --cflags` `root-config --libs`

#inputs
inputdir=/dataf/heli/XZZ/80X_20160705_L6p26_Skim
outputdir=/datad/heli/XZZ/80X_20160705_L6p26_Skim_ZpTLepTSkim
#inputdir=/data/XZZ/76X_Ntuple/76X_20160514
#inputdir=/data/XZZ/80X_Ntuple/80X_20160705_Skim
#inputdir=/data/mewu/76X_new
#inputdir=/afs/cern.ch/work/m/mewu/public/76X_new
#outputdir=AnalysisRegion
#outputdir=/data2/yanchu/80X_Ntuple/80X_20160705_ZPTSkim
mkdir -p ${outputdir}

njob="0"

#for infile in $(ls $inputdir/*.root|grep -v 2016B|grep -v DYJetsToLL|grep -v BulkGrav);
for infile in  $inputdir/*.root ;
do
#  echo "+++ skimming $infile +++"
  outfile="${outputdir}/${infile/$inputdir\//}"
  outfile="${outfile/\/vvTreeProducer\/tree/}"

  echo -- Command: ./skimming.exe $infile $outfile 

  ./skimming.exe $infile $outfile &> ${outfile}.log &

  njob=$(( njob + 1 ))
  if [ "$njob" -eq "100" ]; then
    wait
    njob="0"
  fi
done

