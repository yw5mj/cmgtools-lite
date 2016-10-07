#!/bin/sh

dir=dt3_truncated
#dir=dt3

#tag="SingleElectron_"
#from="eoscms.cern.ch\/\/eos\/cms"
#to="cms-xrd-global.cern.ch\/"
tag="SingleMuon_"
from="cms-xrd-global.cern.ch"
#to="xrootd-cms.infn.it"
#from="xrootd-cms.infn.it"
to="cmsxrootd.fnal.gov"

cd $dir

for dd in ${tag}*; 
do
  cd $dd
  sed "s/${from}/${to}/g" config.pck > config.pck.new 
  mv config.pck.new config.pck
  cd ..
done

cd ..
