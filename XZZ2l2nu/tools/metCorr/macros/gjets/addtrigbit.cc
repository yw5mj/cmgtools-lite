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

bool lightWeight=true;

int main(int argc, char** argv) {

  if( argc<4 ) {
     std::cout << argv[0] << ":  " << std::endl ;
     std::cout << " Functionality: skimming... "  << std::endl;
     std::cout << "                 "  << std::endl;
     std::cout << " usage: " << argv[0] << " inputfile.root trigtreefile.root outputfile.root" << std::endl ;
     exit(1) ;
  }

  time_t now = time(0);
  char* dt = ctime(&now);
  std::cout << "Start time is: " << dt << std::endl;

  // input file name
  std::string inputfile((const char*)argv[1]); 
  // trg tree file name
  std::string trigfile((const char*)argv[2]); 
  // output file name
  std::string outputfile((const char*)argv[3]);
  // initialize
  // root files
  TFile* finput = TFile::Open(inputfile.c_str());
  TFile* ftrig = TFile::Open(trigfile.c_str());
  TFile* foutput = TFile::Open(outputfile.c_str(), "recreate");
  // tree
  TTree* tree = (TTree*)finput->Get("tree");
  TTree* tree_trig = (TTree*)ftrig->Get("tree");

  // vars
  UInt_t run;
  ULong64_t evt;  
  tree->SetBranchAddress("run", &run);
  tree->SetBranchAddress("evt", &evt);
  tree_trig->SetBranchAddress("run", &run);
  tree_trig->SetBranchAddress("evt", &evt);


  // add new branches
  TTree* tree_out = tree->CloneTree(0);

  Bool_t HLT_PHOTONIDISO, HLT_PHOTONIDISO22, HLT_PHOTONIDISO30, HLT_PHOTONIDISO50, HLT_PHOTONIDISO75, HLT_PHOTONIDISO90, HLT_PHOTONIDISO120, HLT_PHOTONIDISO165;
  Bool_t HLT_PHOTONIDISOMETEB, HLT_PHOTONIDISOMETEB22, HLT_PHOTONIDISOMETEB30, HLT_PHOTONIDISOMETEB50, HLT_PHOTONIDISOMETEB75, HLT_PHOTONIDISOMETEB90, HLT_PHOTONIDISOMETEB120;
  Bool_t HLT_PHOTONIDISOVBFEB, HLT_PHOTONIDISOVBFEB22, HLT_PHOTONIDISOVBFEB30, HLT_PHOTONIDISOVBFEB50, HLT_PHOTONIDISOVBFEB75, HLT_PHOTONIDISOVBFEB90, HLT_PHOTONIDISOVBFEB120;

  tree_out->Branch("HLT_PHOTONIDISO", &HLT_PHOTONIDISO, "HLT_PHOTONIDISO/O");
  tree_out->Branch("HLT_PHOTONIDISO22", &HLT_PHOTONIDISO22, "HLT_PHOTONIDISO22/O");
  tree_out->Branch("HLT_PHOTONIDISO30", &HLT_PHOTONIDISO30, "HLT_PHOTONIDISO30/O");
  tree_out->Branch("HLT_PHOTONIDISO50", &HLT_PHOTONIDISO50, "HLT_PHOTONIDISO50/O");
  tree_out->Branch("HLT_PHOTONIDISO75", &HLT_PHOTONIDISO75, "HLT_PHOTONIDISO75/O");
  tree_out->Branch("HLT_PHOTONIDISO90", &HLT_PHOTONIDISO90, "HLT_PHOTONIDISO90/O");
  tree_out->Branch("HLT_PHOTONIDISO120", &HLT_PHOTONIDISO120, "HLT_PHOTONIDISO120/O");
  tree_out->Branch("HLT_PHOTONIDISO165", &HLT_PHOTONIDISO165, "HLT_PHOTONIDISO165/O");
  tree_out->Branch("HLT_PHOTONIDISOMETEB", &HLT_PHOTONIDISOMETEB, "HLT_PHOTONIDISOMETEB/O");
  tree_out->Branch("HLT_PHOTONIDISOMETEB22", &HLT_PHOTONIDISOMETEB22, "HLT_PHOTONIDISOMETEB22/O");
  tree_out->Branch("HLT_PHOTONIDISOMETEB30", &HLT_PHOTONIDISOMETEB30, "HLT_PHOTONIDISOMETEB30/O");
  tree_out->Branch("HLT_PHOTONIDISOMETEB50", &HLT_PHOTONIDISOMETEB50, "HLT_PHOTONIDISOMETEB50/O");
  tree_out->Branch("HLT_PHOTONIDISOMETEB75", &HLT_PHOTONIDISOMETEB75, "HLT_PHOTONIDISOMETEB75/O");
  tree_out->Branch("HLT_PHOTONIDISOMETEB90", &HLT_PHOTONIDISOMETEB90, "HLT_PHOTONIDISOMETEB90/O");
  tree_out->Branch("HLT_PHOTONIDISOMETEB120", &HLT_PHOTONIDISOMETEB120, "HLT_PHOTONIDISOMETEB120/O");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB", &HLT_PHOTONIDISOVBFEB, "HLT_PHOTONIDISOVBFEB/O");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB22", &HLT_PHOTONIDISOVBFEB22, "HLT_PHOTONIDISOVBFEB22/O");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB30", &HLT_PHOTONIDISOVBFEB30, "HLT_PHOTONIDISOVBFEB30/O");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB50", &HLT_PHOTONIDISOVBFEB50, "HLT_PHOTONIDISOVBFEB50/O");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB75", &HLT_PHOTONIDISOVBFEB75, "HLT_PHOTONIDISOVBFEB75/O");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB90", &HLT_PHOTONIDISOVBFEB90, "HLT_PHOTONIDISOVBFEB90/O");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB120", &HLT_PHOTONIDISOVBFEB120, "HLT_PHOTONIDISOVBFEB120/O");


  // make index for trigger tree
  tree_trig->BuildIndex("run", "evt");

  // loop
  for (int i=0; i<(int)tree->GetEntries(); i++){
    if (i%10000==0) std::cout << " Events " << i << std::endl;
    tree->GetEntry(i);
    tree_trig->GetEntryWithIndex(run, evt);
    tree_out->Fill();
  }

  foutput->cd();
  tree_out->Write();
  foutput->Close();
  finput->Close();


  now = time(0);
  dt = ctime(&now);
  std::cout << "End time is: " << dt << std::endl;

  return 0;

}



