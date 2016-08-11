#!/bin/sh

dir=mc2
out=/data2/XZZ2/80X_20160810_Chunks

if [ ! -e "$out" ]; then
  echo "ERROR:: Do not exist output directory $out, exist... "
  exit 0
fi

echo "#### first do a overall sync"

rsync -var $dir $out/

echo "#### go to check each directory "
cd $dir

list=`ls -1 -d */`

for job in $list;
do
  job=${job%%/}
  ss=`heppy_check.py $job | grep "not" | awk {'print $1'}`
  echo "## check $job "
  if [ "$ss" == "$job" ];
  then
    echo "- job is not finished or has problem to be resubmitted .. "
  else
    echo "- job finished successfully copy out and delete .. "
    echo " > rsync -var $job $out/$dir/"
    rsync -var $job $out/$dir/
    echo " > rm -rf $job"
    rm -rf $job
  fi

done

cd -


