#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1D.h"
#include "TMath.h"
#include "TF1.h"
#include "TVector2.h"
#include "TGraphErrors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

int main(int argc, char** argv) {

  if( argc<3 ) {
     std::cout << argv[0] << ":  " << std::endl ;
     std::cout << " Functionality: apply corrections ... "  << std::endl;
     std::cout << "                 "  << std::endl;
     std::cout << " usage: " << argv[0] << " inputfile.root outputfile.root " << std::endl ;
     exit(1) ;
  }

  // input file name
  std::string inputfile((const char*)argv[1]); 
  // output file name
  std::string outputfile((const char*)argv[2]);


  // initialize
  // root files
  TFile* finput = new TFile(inputfile.c_str());
  TFile* foutput = new TFile(outputfile.c_str(), "recreate");


  // tree
  TTree* tree = (TTree*)finput->Get("tree");

  // out_tree
  TTree* tree_out = tree->CloneTree(0);

  // trigger eff
  //TFile* ftrig = TFile::Open("data/dataefficiency.root");
  //TF1* ftrg_z_e = (TF1*)ftrig->Get("dataz_e_func");
  //TF1* ftrg_z_m = (TF1*)ftrig->Get("dataz_m_func");
  TFile* ftrig = new TFile("data/trigEff.root");
  TH1D* htrg_z_e = (TH1D*)ftrig->Get("heff_el_zpt");
  TH1D* htrg_z_m = (TH1D*)ftrig->Get("heff_mu_zpt");

  Float_t triggersf;
  Float_t llnunu_l1_pt, llnunu_l1_eta, llnunu_l1_l1_pt, llnunu_l1_l2_pt, llnunu_l1_l1_eta,llnunu_l1_l2_eta;
  Int_t llnunu_l1_l1_pdgId, llnunu_l1_l2_pdgId;
  
  tree->SetBranchAddress("triggersf",&triggersf);
  tree->SetBranchAddress("llnunu_l1_pt",&llnunu_l1_pt);
  tree->SetBranchAddress("llnunu_l1_eta",&llnunu_l1_eta);
  tree->SetBranchAddress("llnunu_l1_l1_pt",&llnunu_l1_l1_pt);
  tree->SetBranchAddress("llnunu_l1_l1_eta",&llnunu_l1_l1_eta);
  tree->SetBranchAddress("llnunu_l1_l2_pt",&llnunu_l1_l2_pt);
  tree->SetBranchAddress("llnunu_l1_l2_eta",&llnunu_l1_l2_eta);



  for (int i=0; i<(int)tree->GetEntries(); i++){
  //for (int i=0; i<10000; i++){
    tree->GetEntry(i);

    if (fabs(llnunu_l1_l1_pdgId)==11&&fabs(llnunu_l1_l2_pdgId)==11) {
      //triggersf = ftrg_z_e->Eval(llnunu_l1_pt);
      if (llnunu_l1_pt<htrg_z_e->GetXaxis()->GetXmin()) {
        triggersf=0.0;
      }
      else if (llnunu_l1_pt>htrg_z_e->GetXaxis()->GetXmin() && llnunu_l1_pt<htrg_z_e->GetXaxis()->GetXmax()){
        triggersf = htrg_z_e->GetBinContent(htrg_z_e->GetXaxis()->FindBin(llnunu_l1_pt));
      }
      else {
        triggersf = htrg_z_e->GetBinContent(htrg_z_e->GetXaxis()->GetLast());
      }
    }
    else if (fabs(llnunu_l1_l1_pdgId)==13&&fabs(llnunu_l1_l2_pdgId)==13) {
      //triggersf = ftrg_z_m->Eval(llnunu_l1_pt);
      if (llnunu_l1_pt<htrg_z_m->GetXaxis()->GetXmin()) {
        triggersf=0.0;
      }
      else if (llnunu_l1_pt>htrg_z_m->GetXaxis()->GetXmin() && llnunu_l1_pt<htrg_z_m->GetXaxis()->GetXmax()){
        triggersf = htrg_z_m->GetBinContent(htrg_z_m->GetXaxis()->FindBin(llnunu_l1_pt));
      }
      else {
        triggersf = htrg_z_m->GetBinContent(htrg_z_m->GetXaxis()->GetLast());
      }
    }

    tree_out->Fill();

  }

  foutput->cd();
  tree_out->Write();
  foutput->Close();
  finput->Close();

  return 0;

}



