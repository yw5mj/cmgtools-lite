#!/bin/sh



# compile
g++ preskim_gjets.cc -o preskim_gjets.exe `root-config --cflags` `root-config --libs`

#inputs
indir=/home/heli/XZZ/80X_20160927
outdir=/home/heli/XZZ/80X_20160927_light

mkdir -p $outdir

#for dd in ${indir}/*/vvTreeProducer;
for dd in ${indir}/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer;
do 
  infile="${dd}/tree.root";
  oo="${dd/$indir/$outdir}";
  outfile="${oo}/tree_light.root";
  echo mkdir -p $oo;
  mkdir -p $oo ;
  echo "./preskim_gjets.exe $infile $outfile &> ${outfile}.log & "
  ./preskim_gjets.exe $infile $outfile &> ${outfile}.log &
done



# wait and replace old one

wait

#for dd in  ${indir}/* ;
for dd in  ${indir}/SinglePhoton_Run2016BCD_PromptReco ;
do
  echo $dd;
  ddo=${dd/$indir/$outdir}
  echo "mkdir -p ${ddo} " 
  mkdir -p ${ddo}

  for dd2 in ${dd}/* ;
  do
    if [[ ${dd2} != *"vvTree"* ]]; then
      #echo $dd2;
      ddo2="${dd2/$indir/$outdir} "
      echo "cp -rp $dd2 $ddo2 " 
      cp -rp $dd2 $ddo2
    fi
  done;

  ttin="${ddo/_light/}/vvTreeProducer"

  echo "cp -rp $ttin $ddo/"
  cp -rp $ttin $ddo/
  echo "mv $ddo/vvTreeProducer/tree_light.root $ddo/vvTreeProducer/tree.root"
  mv $ddo/vvTreeProducer/tree_light.root $ddo/vvTreeProducer/tree.root
done
