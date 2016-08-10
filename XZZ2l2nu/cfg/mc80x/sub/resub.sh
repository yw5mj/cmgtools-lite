#!/bin/sh

dir="mc2"
queue="2nd"

cd $dir
list=`heppy_check.py * | grep "not" | awk {'print $1'}`

listjobs=`bjobs -o "name" | grep -v JOB_NAME`

for dd in $list;
do
  echo "### check job $dd"
  ss=`echo $listjobs | grep "$dd "`"_has"
  #echo " # ss=$ss "
  #echo " # dd=$dd "

  if [ "$ss" == "_has" ]
  then
    echo "  submit $dd"
    cd $dd
    echo "  bsub -q $queue -J $dd  < batchScript.sh"
    bsub -q $queue -J $dd  < batchScript.sh
    cd ../
  else
    echo "  job $dd is still running"
  fi
done 

cd ../

