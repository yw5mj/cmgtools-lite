#!/bin/sh


tag="Test1_GJets_ResBos_BkgSub_Rc36p46wHLT_"
#tag="Test1_GJets_ResBos_NoBkgSub_Rc36p46wHLT_"

channels="mu el"
#channels="all mu el"
#cutChains="tightzpt50 tightzpt100 tightzpt100met50"
#cutChains="tightzpt100 tightzpt100met50"
cutChains="tightzpt50"
#cutChains="tightzpt100"
#cutChains="tightzptgt50lt200"
logdir="log_gjets_36p46"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind --LogY &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &
      #./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind &> ${logdir}/${tag}${cutChain}_bld_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
      #./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" &> ${logdir}/${tag}${cutChain}_${channel}.log &
      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --test &> ${logdir}/${tag}${cutChain}_${channel}.log &
      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY  --test &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind --test &> ${logdir}/${tag}${cutChain}_bld_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind --LogY  --test &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &

   done
done
