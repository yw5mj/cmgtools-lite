#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TMath.h"
#include "TVector2.h"
#include "TGraphErrors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <ctime>


int main(int argc, char** argv) {

  if( argc<3 ) {
     std::cout << argv[0] << ":  " << std::endl ;
     std::cout << " Functionality: skimming... "  << std::endl;
     std::cout << "                 "  << std::endl;
     std::cout << " usage: " << argv[0] << " inputfile.root outputfile.root" << std::endl ;
     exit(1) ;
  }

  time_t now = time(0);
  char* dt = ctime(&now);
  std::cout << "Start time is: " << dt << std::endl;

  // input file name
  std::string inputfile((const char*)argv[1]); 
  // output file name
  std::string outputfile((const char*)argv[2]);
  // initialize
  // root files
  TFile* finput = TFile::Open(inputfile.c_str());
  TFile* foutput = TFile::Open(outputfile.c_str(), "recreate");

  // tree
  TTree* tree = (TTree*)finput->Get("tree");

  Int_t isData;
  tree->SetBranchAddress("isData",&isData);
  // check if tree has events
  if (tree->GetEntries()<=0) return 0;
  // get isData info
  tree->GetEntry(0);

  // select branches
//  tree->SetBranchStatus("photon_*",0);
  tree->SetBranchStatus("jet_*",0);
/*
  tree->SetBranchStatus("gjet_l*_px",0);
  tree->SetBranchStatus("gjet_l*_py",0);
  tree->SetBranchStatus("gjet_l*_pz",0);
  tree->SetBranchStatus("gjet_l2_eta",0);
  tree->SetBranchStatus("gjet_l2_rapidity",0);
  tree->SetBranchStatus("gjet_l2_mass",0);
  tree->SetBranchStatus("gjet_dPT*",0); 
  tree->SetBranchStatus("gjet_deltaPhi",0); 
  tree->SetBranchStatus("gjet_CosDeltaPhi",0);
  tree->SetBranchStatus("gjet_metOvSqSET",0);
  tree->SetBranchStatus("gjet_l1_mass",0);
  tree->SetBranchStatus("gjet_l1_chHadIso",0);
  tree->SetBranchStatus("gjet_l1_phIso",0);
  tree->SetBranchStatus("gjet_l1_neuHadIso",0);
  tree->SetBranchStatus("gjet_l2_metSig",0);
*/
  if (!isData) {
    tree->SetBranchStatus("HLT_*",0); 
    tree->SetBranchStatus("gjet_l1_mcMatchId",0);
    tree->SetBranchStatus("gjet_l1_mcPt",0);
    tree->SetBranchStatus("gjet_l1_mcEta",0);
    tree->SetBranchStatus("gjet_l1_mcPhi",0);
    tree->SetBranchStatus("gjet_l2_genPt",0);
  }

  tree->SetAlias("absDeltaPhi", "fabs(TVector2::Phi_mpi_pi(gjet_l2_phi-gjet_l1_phi))");
  tree->SetAlias("metPara", "gjet_l2_pt*cos(gjet_l2_phi-gjet_l1_phi)");
  tree->SetAlias("metPerp", "gjet_l2_pt*sin(gjet_l2_phi-gjet_l1_phi)");
  tree->SetAlias("uPara", "-gjet_l2_pt*cos(gjet_l2_phi-gjet_l1_phi)-gjet_l1_pt");
  tree->SetAlias("uPerp", "-gjet_l2_pt*sin(gjet_l2_phi-gjet_l1_phi)");
  tree->SetAlias("ut", "sqrt(uPara*uPara+uPerp*uPerp)");
  tree->SetAlias("eta", "gjet_l1_eta");
  tree->SetAlias("phi", "gjet_l1_phi");
  tree->SetAlias("pt", "gjet_l1_pt");
  tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter&&Flag_CSCTightHalo2015Filter)");

  tree->SetAlias("ieta", "gjet_l1_ieta");
  tree->SetAlias("iphi", "gjet_l1_iphi");

  tree->SetAlias("flag1", "!((ieta==5&&iphi==41)||(ieta==-51&&iphi==196)||(ieta==56&&iphi==67)||(ieta==-45&&iphi==340)||(ieta==58&&iphi==74)||(ieta==79&&iphi==67)||(ieta==72&&iphi==67)||(ieta==4&&iphi==70)||(ieta==17&&iphi==290)||(ieta==-44&&iphi==133)||(ieta==13&&iphi==67)||(ieta==-24&&iphi==119)||(ieta==-84&&iphi==168)||(ieta==73&&iphi==299)||(ieta==49&&iphi==6)||(ieta==-21&&iphi==308)||(ieta==59&&iphi==180)||(ieta==2&&iphi==81)||(ieta==22&&iphi==138))");

  tree->SetAlias("filter1", "(gjet_l1_sigmaIetaIeta>0.001&&gjet_l1_sigmaIphiIphi>0.001&&gjet_l1_SwissCross<0.95&&gjet_l1_mipTotE<4.9&&gjet_l1_time>-2.08&&gjet_l1_time<0.92)");

  //TTree* tree_out = tree->CloneTree(-1);

  
  
  TTree* tree_out = tree->CopyTree("HLT_PHOTONIDISO&&metfilter&&fabs(eta)<1.47&&ngjet==1&&flag1&&filter1");


  foutput->cd();
  tree_out->Write();
  foutput->Close();
  finput->Close();


  now = time(0);
  dt = ctime(&now);
  std::cout << "End time is: " << dt << std::endl;

  return 0;

}



