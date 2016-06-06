#define selections_cxx
#include <TROOT.h>
#include <TTree.h>
#include "selections.h"

#include <iostream>
//#include <iomanip>
#include <fstream>
#include <sstream>
//#include <algorithm>
//#include <vector>
#include <cstdio> // for printf

#include "TLorentzVector.h"
#include "TClonesArray.h"

using namespace std;

void Selections::skimming()
{
  TTree* tIn = (TTree*)infile->Get("tree");
  
  readCut();
  //  cout<<"double check: "<<cutChain<<endl;
  TTree* tOut = tIn->CopyTree(cutChain);

  TBranch *b_SumWeights=tOut->Branch("SumWeights",&SumWeights,"SumWeights/D");
  TBranch *b_SumEvents=tOut->Branch("SumEvents",&SumEvents,"SumEvents/D");

  //-- read branches from in Tree:
  Float_t         llnunu_l1_pt;   //[nllnunu]
  Float_t         metNoJet_pt;

  TBranch        *b_metNoJet_pt;   //!
  TBranch        *b_llnunu_l1_pt;   //!

  tIn->SetBranchAddress("metNoJet_pt", &metNoJet_pt, &b_metNoJet_pt);
  tIn->SetBranchAddress("llnunu_l1_pt", &llnunu_l1_pt, &b_llnunu_l1_pt);
  
  //-- derive new variables: 
  Float_t PtDiff = 0.;
  TBranch *b_PtDiff=tOut->Branch("PtDiff",&PtDiff,"PtDiff/D");
  
  for (int ii=0; ii<(int)tOut->GetEntries(); ii++){
    b_SumWeights->Fill();
    b_SumEvents->Fill();
    PtDiff=abs(metNoJet_pt-llnunu_l1_pt)/llnunu_l1_pt;
    b_PtDiff->Fill();
  }
  
  tOut->Write();
  outfile->ls();
  outfile->Close();
  infile->Close();
  return;
  
}


void Selections::readCut(string config)
{
  printf("Using customized cuts...\n");
  std::ifstream ifs( config );
  std::string line;
  TString channel;
  
  while (std::getline(ifs, line)){
    if ( !line.empty() ){
      while ( line[0] == ' ' ) line.erase(0,1);
    }
    if ( !line.empty() && line[0] != '#' ){
      stringstream is(line);
      TString cut;
      is >> cut;
      if (cutChain.IsNull())  cutChain=cut;
      else{
	//if (cut.Contains("pdgId")) cutChain.Append("||"+cut);
	//else 
	cutChain.Append("&&"+cut);
      }
    }
  }
  cout<<"^_^ You are using the following cuts: \n"<<cutChain<<endl;
  return;
}
