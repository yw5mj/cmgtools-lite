#!/bin/sh


tag="Test1_DataB2H33fbinv_RecoilZJets_NoRhoWt_VtxWt_"
#tag="Test1_DataB2H33fbinv_NoRecoilZJets_NoRhoWt_VtxWt_"
#tag="Test1_DataB2H33fbinv_RecoilZJets_NoRhoWt_"
#tag="DataB2H33fbinv_RecoilZJets_NoRhoWt_"
#tag="DataB2H33fbinv_NoRecoilZJets_NoRhoWt_"
#tag="Test1_NoRecoilZJets_NoRhoWt_DataB2H33fbinv_"
#tag="Test1_NoRecoilZJets_DataB2H33fbinv_"
#tag="Test1_Old29fbinvZJets_DataB2H33fbinv_"
#tag="Test1_Old29fbinvZJets_NoRhoWt_DataB2H33fbinv_"
#tag="Test1_DataB2H33fbinv_"
#tag="ForRhoWeight_DataB2H33fbinv_"
#channels="mu"
channels="all mu el"
#cutChains="tight tightzpt100 tightzpt100met50"
#cutChains="tightzpt100 tightzpt100met50"
cutChains="tight"
#cutChains="tightzptlt200"
#blind="--Blind"
#logy="--LogY"
logdir="log_b2h33fbinv"

#./stack_dtmc_skim.py -l -b -q  --tag="Test_DataB2G_ICHEPcfg_" --cutChain="tight" --channel="mu" --Blind --LogY --test


for cutChain in $cutChains;
do
   for channel in $channels;
   do
      #./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind --LogY &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &
      #./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind &> ${logdir}/${tag}${cutChain}_bld_${channel}.log &
      #./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
      #./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" &> ${logdir}/${tag}${cutChain}_${channel}.log &
      #./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --test &> ${logdir}/${tag}${cutChain}_${channel}.log &
      ./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY  --test &> ${logdir}/${tag}${cutChain}_${channel}.log &
      ./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY --Blind  --test &> ${logdir}/${tag}${cutChain}_${channel}.log &

   done
done
