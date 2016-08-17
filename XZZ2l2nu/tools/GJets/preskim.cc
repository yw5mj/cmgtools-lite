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
  tree->SetBranchStatus("lep_*",0);
  tree->SetBranchStatus("genLep_*",0);
  tree->SetBranchStatus("jet_*",0);
  tree->SetBranchStatus("vtx_*",0);
  tree->SetBranchStatus("llnunu_LV_*",0);
  tree->SetBranchStatus("llnunu_TuneP_LV_*",0);
  tree->SetBranchStatus("triggers*",0);
  tree->SetBranchStatus("llnunu_l1_l1_leps*",0);
  tree->SetBranchStatus("llnunu_l1_l2_leps*",0);
  
  TTree* tree_out = tree->CloneTree(-1);


  foutput->cd();
  tree_out->Write();
  foutput->Close();
  finput->Close();


  now = time(0);
  dt = ctime(&now);
  std::cout << "End time is: " << dt << std::endl;

  return 0;

}



