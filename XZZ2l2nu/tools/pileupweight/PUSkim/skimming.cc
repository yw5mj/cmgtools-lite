#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1D.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

int main(int argc, char** argv) {

  if( argc<6 ) {
     std::cout << argv[0] << ":  " << std::endl ;
     std::cout << " Functionality: skimming... "  << std::endl;
     std::cout << "                 "  << std::endl;
     std::cout << " usage: " << argv[0] << " inputfile.root outputfile.root Nevts SumWeights cut_string " << std::endl ;
     exit(1) ;
  }

  // input file name
  std::string inputfile((const char*)argv[1]); 
  // output file name
  std::string outputfile((const char*)argv[2]);


  // nevts 
  double SumEvents = atof(argv[3]);  
  // sum weights 
  double SumWeights = atof(argv[4]);  
  // cuts
  std::string Cuts((const char*)argv[5]);

  // initialize
  // root files
  TFile* finput = new TFile(inputfile.c_str());
  TFile* foutput = new TFile(outputfile.c_str(), "recreate");


  // tree
  TTree* tree = (TTree*)finput->Get("tree");

  // out_tree
  TTree* tree_out = tree->CopyTree(Cuts.c_str());

  TBranch *b_SumEvents=tree_out->Branch("SumEvents",&SumEvents,"SumEvents/D");
  TBranch *b_SumWeights=tree_out->Branch("SumWeights",&SumWeights,"SumWeights/D");


  // pileup
  TFile* fpileup691 = new TFile("pileup_MC_76x_691.root");
  TFile* fpileup692 = new TFile("pileup_MC_76x_692.root");
  TFile* fpileup693 = new TFile("pileup_MC_76x_693.root");
  TFile* fpileup694 = new TFile("pileup_MC_76x_694.root");
  TFile* fpileup696 = new TFile("pileup_MC_76x_696.root");
  TFile* fpileup698 = new TFile("pileup_MC_76x_698.root");
  TFile* fpileup700 = new TFile("pileup_MC_76x_700.root");
  TFile* fpileup702 = new TFile("pileup_MC_76x_702.root");
  TFile* fpileup800 = new TFile("pileup_MC_76x_800.root");
  TH1D* hpileup691 = (TH1D*)fpileup691->Get("puweight");
  TH1D* hpileup692 = (TH1D*)fpileup692->Get("puweight");
  TH1D* hpileup693 = (TH1D*)fpileup693->Get("puweight");
  TH1D* hpileup694 = (TH1D*)fpileup694->Get("puweight");
  TH1D* hpileup696 = (TH1D*)fpileup696->Get("puweight");
  TH1D* hpileup698 = (TH1D*)fpileup698->Get("puweight");
  TH1D* hpileup700 = (TH1D*)fpileup700->Get("puweight");
  TH1D* hpileup702 = (TH1D*)fpileup702->Get("puweight");
  TH1D* hpileup800 = (TH1D*)fpileup800->Get("puweight");

  Double_t puWeight691;
  Double_t puWeight692;
  Double_t puWeight693;
  Double_t puWeight694;
  Double_t puWeight696;
  Double_t puWeight698;
  Double_t puWeight700;
  Double_t puWeight702;
  Double_t puWeight800;
  TBranch *b_puWeight691 = tree_out->Branch("puWeight691",&puWeight691,"puWeight691/D");
  TBranch *b_puWeight692 = tree_out->Branch("puWeight692",&puWeight692,"puWeight692/D");
  TBranch *b_puWeight693 = tree_out->Branch("puWeight693",&puWeight693,"puWeight693/D");
  TBranch *b_puWeight694 = tree_out->Branch("puWeight694",&puWeight694,"puWeight694/D");
  TBranch *b_puWeight696 = tree_out->Branch("puWeight696",&puWeight696,"puWeight696/D");
  TBranch *b_puWeight698 = tree_out->Branch("puWeight698",&puWeight698,"puWeight698/D");
  TBranch *b_puWeight700 = tree_out->Branch("puWeight700",&puWeight700,"puWeight700/D");
  TBranch *b_puWeight702 = tree_out->Branch("puWeight702",&puWeight702,"puWeight702/D");
  TBranch *b_puWeight800 = tree_out->Branch("puWeight800",&puWeight800,"puWeight800/D");

  //
  Int_t nTrueInt; 
  tree_out->SetBranchAddress( "nTrueInt", &nTrueInt );

  for (int i=0; i<(int)tree_out->GetEntries(); i++){
    tree_out->GetEntry(i);
    puWeight691 = hpileup691->GetBinContent(hpileup691->FindBin(nTrueInt)); 
    puWeight692 = hpileup692->GetBinContent(hpileup692->FindBin(nTrueInt)); 
    puWeight693 = hpileup693->GetBinContent(hpileup693->FindBin(nTrueInt)); 
    puWeight694 = hpileup694->GetBinContent(hpileup694->FindBin(nTrueInt)); 
    puWeight696 = hpileup696->GetBinContent(hpileup696->FindBin(nTrueInt)); 
    puWeight698 = hpileup698->GetBinContent(hpileup698->FindBin(nTrueInt)); 
    puWeight700 = hpileup700->GetBinContent(hpileup700->FindBin(nTrueInt)); 
    puWeight702 = hpileup702->GetBinContent(hpileup702->FindBin(nTrueInt)); 
    puWeight800 = hpileup800->GetBinContent(hpileup800->FindBin(nTrueInt)); 
    b_SumEvents->Fill();
    b_SumWeights->Fill();
    b_puWeight691->Fill();
    b_puWeight692->Fill();
    b_puWeight693->Fill();
    b_puWeight694->Fill();
    b_puWeight696->Fill();
    b_puWeight698->Fill();
    b_puWeight700->Fill();
    b_puWeight702->Fill();
    b_puWeight800->Fill();
  }

  foutput->cd();
  tree_out->Write();
  foutput->Close();
  finput->Close();

  return 0;

}



