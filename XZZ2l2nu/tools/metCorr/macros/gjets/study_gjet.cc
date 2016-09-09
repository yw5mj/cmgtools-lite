#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include "TVector2.h"
#include "TEntryList.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <utility>
#include "TLegend.h"
#include "TROOT.h"

// Hengne Li @ CERN, 2016

int main(int argc, char** argv) {


  TFile* file1 = TFile::Open("/home/heli/XZZ/80X_20160825_light_Skim/DYJetsToLL_M50_BIG.root");
  //TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20160810_light/GJet_Pt_20toInf_DoubleEMEnriched/vvTreeProducer/tree.root");
  TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20160810_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree.root");

  //std::string outtag="study_gjets_tight";
  //std::string outtag="study_gjets";
  std::string outtag="study_gjets_data";

  char name[1000];

  TCanvas* plots = new TCanvas("plots", "plots");

  sprintf(name, "%s.pdf[", outtag.c_str());
  plots->Print(name);

  TTree* tree1 = (TTree*)file1->Get("tree");
  TTree* tree2 = (TTree*)file2->Get("tree");


  // define cuts
  std::string metfilter="(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_CSCTightHalo2015Filter&&Flag_eeBadScFilter)";
  std::string cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID==1||llnunu_l1_l2_highPtID==1))";
  cuts_lepaccept+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5))";
  std::string cuts_lepaccept_lowlpt="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>20&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID==1||llnunu_l1_l2_highPtID==1))";
  cuts_lepaccept_lowlpt+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>20&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.5))";
  std::string cuts_zmass="(llnunu_l1_mass>70&&llnunu_l1_mass<110)";
  std::string cuts_loose_z="("+metfilter+"&&"+cuts_lepaccept+"&&"+cuts_zmass+")";
  std::string cuts_loose_z_lowlpt="("+metfilter+"&&"+cuts_lepaccept_lowlpt+"&&"+cuts_zmass+")";


  std::string base_selec =  cuts_loose_z;
  std::string base_selec_lowlpt =  cuts_loose_z_lowlpt;

  std::string base_selec_el = "(" + cuts_loose_z + "&&(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11))";
  std::string base_selec_mu = "(" + cuts_loose_z + "&&(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13))";
  std::string base_selec_lowlpt_el = "(" + cuts_loose_z_lowlpt + "&&(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11))";
  std::string base_selec_lowlpt_mu = "(" + cuts_loose_z_lowlpt + "&&(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13))";

  // add weight
  std::string weight_selec = std::string("*(ZJetsGenWeight*ZPtWeight*puWeight68075*1921.8*3)");
  //std::string weight_selec = std::string("*(ZJetsGenWeight*ZPtWeight*1921.8*3*12900.0)");
  // rho weight
  std::string rhoweight_selec = std::string("*(0.602*exp(-0.5*pow((rho-8.890)/6.187,2))+0.829*exp(-0.5*pow((rho-21.404)/10.866,2)))");
  // scale factors
  //std::string effsf_selec = std::string("*(trgsf*isosf*idsf*trksf)");
  std::string effsf_selec = std::string("*(trgsf*isosf*idsf*trksf)");

  // selec, cuts + weights
  std::string zjet_selec = base_selec + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_el = base_selec_el + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_mu = base_selec_mu + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt = base_selec_lowlpt + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt_el = base_selec_lowlpt_el + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt_mu = base_selec_lowlpt_mu + weight_selec + rhoweight_selec + effsf_selec;

  //std::string gjet_selec = metfilter + rhoweight_selec;
  std::string gjet_selec = metfilter;

  //Double_t ZPtBins[] = {0,1.25,2.5,3.75,5,6.25,7.5,8.75,10,11.25,12.5,15,17.5,20,25,30,35,40,45,50,60,70,80,90,100,110,130,150,170,190,220,250,400,1000};
  Double_t ZPtBins[] = {20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,65,70,75,80,85,90,95,100,105,110,120,130,140,150,160,170,180,190,200,220,240,260,300,400,500,1000};
  Int_t NZPtBins = sizeof(ZPtBins)/sizeof(ZPtBins[0]) - 1;
  Double_t ZMassBins[] = {50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180};
  Int_t NZMassBins = sizeof(ZMassBins)/sizeof(ZMassBins[0]) - 1;
  //Double_t ZRapBins[] = {-3.0,-2.9,-2.8,-2.7,-2.6,-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0};
  Double_t ZRapBins[] = {-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5};
  //Double_t ZRapBins[] = {-2.5,-2.3,-2.1,-1.9,-1.7,-1.5,-1.3,-1.1,-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9,1.1,1.3,1.5,1.7,1.9,2.1,2.3,2.5};
  Int_t NZRapBins = sizeof(ZRapBins)/sizeof(ZRapBins[0]) - 1;

  // all
  TH1D* hzptbb1 = new TH1D("hzptbb1", "hzptbb1", NZPtBins, ZPtBins);
  TH1D* hzptbb2 = new TH1D("hzptbb2", "hzptbb2", NZPtBins, ZPtBins);

  hzptbb1->Sumw2();
  hzptbb2->Sumw2();


  tree1->Draw("llnunu_l1_pt>>hzptbb1", zjet_selec.c_str());
  tree2->Draw("gjet_l1_pt>>hzptbb2",gjet_selec.c_str());

  hzptbb1->SetMarkerColor(2);
  hzptbb2->SetMarkerColor(4);
  hzptbb1->SetLineColor(2);
  hzptbb2->SetLineColor(4);

  TLegend* lgzptbb = new TLegend(0.5,0.6,0.8,0.8);
  lgzptbb->SetName("lgzptbb");
  lgzptbb->AddEntry(hzptbb1, "ZJets", "pl");
  lgzptbb->AddEntry(hzptbb2, "GJets", "pl");

  plots->Clear();

  hzptbb1->Draw();
  hzptbb2->Draw("same");
  lgzptbb->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12 = (TH1D*)hzptbb1->Clone("hzptbbr12");
  hzptbbr12->Divide(hzptbb2);

  plots->Clear();
  hzptbbr12->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  // el
  TH1D* hzptbb1_el = new TH1D("hzptbb1_el", "hzptbb1_el", NZPtBins, ZPtBins);

  hzptbb1_el->Sumw2();


  tree1->Draw("llnunu_l1_pt>>hzptbb1_el", zjet_selec_el.c_str());

  hzptbb1_el->SetMarkerColor(2);
  hzptbb1_el->SetLineColor(2);


  TLegend* lgzptbb_el = new TLegend(0.5,0.6,0.8,0.8);
  lgzptbb_el->SetName("lgzptbb_el");
  lgzptbb_el->AddEntry(hzptbb1_el, "ZJets", "pl");
  lgzptbb_el->AddEntry(hzptbb2, "GJets", "pl");

  plots->Clear();

  hzptbb1_el->Draw();
  hzptbb2->Draw("same");
  lgzptbb_el->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12_el = (TH1D*)hzptbb1_el->Clone("hzptbbr12_el");
  hzptbbr12_el->Divide(hzptbb2);

  plots->Clear();
  hzptbbr12_el->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // mu
  TH1D* hzptbb1_mu = new TH1D("hzptbb1_mu", "hzptbb1_mu", NZPtBins, ZPtBins);

  hzptbb1_mu->Sumw2();


  tree1->Draw("llnunu_l1_pt>>hzptbb1_mu", zjet_selec_mu.c_str());

  hzptbb1_mu->SetMarkerColor(2);
  hzptbb1_mu->SetLineColor(2);


  TLegend* lgzptbb_mu = new TLegend(0.5,0.6,0.8,0.8);
  lgzptbb_mu->SetName("lgzptbb_mu");
  lgzptbb_mu->AddEntry(hzptbb1_mu, "ZJets", "pl");
  lgzptbb_mu->AddEntry(hzptbb2, "GJets", "pl");

  plots->Clear();

  hzptbb1_mu->Draw();
  hzptbb2->Draw("same");
  lgzptbb_mu->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12_mu = (TH1D*)hzptbb1_mu->Clone("hzptbbr12_mu");
  hzptbbr12_mu->Divide(hzptbb2);

  plots->Clear();
  hzptbbr12_mu->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();



  // 3D ZMass

  TH3D* hzmass_zpt_zrap = new TH3D("hzmass_zpt_zrap", "hzmass_zpt_zrap", NZMassBins, ZMassBins, NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzmass_zpt_zrap->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_zrap", zjet_selec.c_str());


  // 2D zpt zrap
  TH2D* hzpt_zrap1 = new TH2D("hzpt_zrap1", "hzpt_zrap1", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  TH2D* hzpt_zrap2 = new TH2D("hzpt_zrap2", "hzpt_zrap2", NZPtBins,ZPtBins,NZRapBins,ZRapBins);

  hzpt_zrap1->Sumw2();
  hzpt_zrap2->Sumw2();


  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1", zjet_selec.c_str());
  tree2->Draw("gjet_l1_rapidity:gjet_l1_pt>>hzpt_zrap2", gjet_selec.c_str());

  //hzpt_zrap1->Scale(1.0/hzpt_zrap1->Integral(),"width");
  //hzpt_zrap2->Scale(1.0/hzpt_zrap2->Integral(),"width");


  TH2D* hzpt_zrap_r12 = (TH2D*)hzpt_zrap1->Clone("hzpt_zrap_r12");
  hzpt_zrap_r12->Divide(hzpt_zrap2);


  plots->Clear();
  hzpt_zrap1->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap2->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_r12->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();



  // 2D zpt zrap el
  TH2D* hzpt_zrap1_el = new TH2D("hzpt_zrap1_el", "hzpt_zrap1_el", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_el->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_el", zjet_selec_el.c_str());
  TH2D* hzpt_zrap_r12_el = (TH2D*)hzpt_zrap1_el->Clone("hzpt_zrap_r12_el");
  hzpt_zrap_r12_el->Divide(hzpt_zrap2);

  plots->Clear();
  hzpt_zrap1_el->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_r12_el->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // 2D zpt zrap mu
  TH2D* hzpt_zrap1_mu = new TH2D("hzpt_zrap1_mu", "hzpt_zrap1_mu", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_mu->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_mu", zjet_selec_mu.c_str());
  TH2D* hzpt_zrap_r12_mu = (TH2D*)hzpt_zrap1_mu->Clone("hzpt_zrap_r12_mu");
  hzpt_zrap_r12_mu->Divide(hzpt_zrap2);

  plots->Clear();
  hzpt_zrap1_mu->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_r12_mu->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // 2D zpt zrap _lowlpt
  TH2D* hzpt_zrap1_lowlpt = new TH2D("hzpt_zrap1_lowlpt", "hzpt_zrap1_lowlpt", NZPtBins,ZPtBins,NZRapBins,ZRapBins);

  hzpt_zrap1_lowlpt->Sumw2();

  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_lowlpt", zjet_selec_lowlpt.c_str());

  TH2D* hzpt_zrap_lowlpt_r12 = (TH2D*)hzpt_zrap1_lowlpt->Clone("hzpt_zrap_lowlpt_r12");
  hzpt_zrap_lowlpt_r12->Divide(hzpt_zrap2);


  plots->Clear();
  hzpt_zrap1_lowlpt->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_lowlpt_r12->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();



  // 2D zpt zrap el
  TH2D* hzpt_zrap1_lowlpt_el = new TH2D("hzpt_zrap1_lowlpt_el", "hzpt_zrap1_lowlpt_el", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_lowlpt_el->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_lowlpt_el", zjet_selec_lowlpt_el.c_str());
  TH2D* hzpt_zrap_lowlpt_r12_el = (TH2D*)hzpt_zrap1_lowlpt_el->Clone("hzpt_zrap_lowlpt_r12_el");
  hzpt_zrap_lowlpt_r12_el->Divide(hzpt_zrap2);

  plots->Clear();
  hzpt_zrap1_lowlpt_el->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_lowlpt_r12_el->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // 2D zpt zrap mu
  TH2D* hzpt_zrap1_lowlpt_mu = new TH2D("hzpt_zrap1_lowlpt_mu", "hzpt_zrap1_lowlpt_mu", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_lowlpt_mu->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_lowlpt_mu", zjet_selec_lowlpt_mu.c_str());
  TH2D* hzpt_zrap_lowlpt_r12_mu = (TH2D*)hzpt_zrap1_lowlpt_mu->Clone("hzpt_zrap_lowlpt_r12_mu");
  hzpt_zrap_lowlpt_r12_mu->Divide(hzpt_zrap2);

  plots->Clear();
  hzpt_zrap1_lowlpt_mu->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_lowlpt_r12_mu->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  //
  sprintf(name, "%s.root", outtag.c_str());
  TFile* fout = new TFile(name, "recreate");

  hzptbb1->Write("h_zpt_1");
  hzptbb2->Write("h_zpt_2");
  hzptbbr12->Write("h_zpt_ratio");

  hzptbb1_el->Write("h_zpt_1_el");
  hzptbbr12_el->Write("h_zpt_ratio_el");

  hzptbb1_mu->Write("h_zpt_1_mu");
  hzptbbr12_mu->Write("h_zpt_ratio_mu");

  hzmass_zpt_zrap->Write("h_zmass_zpt_zrap");

  hzpt_zrap1->Write("h_zpt_zrap_1");
  hzpt_zrap2->Write("h_zpt_zrap_2");
  hzpt_zrap_r12->Write("h_zpt_zrap_ratio");

  hzpt_zrap1_el->Write("h_zpt_zrap_1_el");
  hzpt_zrap_r12_el->Write("h_zpt_zrap_ratio_el");

  hzpt_zrap1_mu->Write("h_zpt_zrap_1_mu");
  hzpt_zrap_r12_mu->Write("h_zpt_zrap_ratio_mu");


  hzpt_zrap1_lowlpt->Write("h_zpt_zrap_lowlpt_1");
  hzpt_zrap_lowlpt_r12->Write("h_zpt_zrap_lowlpt_ratio");

  hzpt_zrap1_lowlpt_el->Write("h_zpt_zrap_lowlpt_1_el");
  hzpt_zrap_lowlpt_r12_el->Write("h_zpt_zrap_lowlpt_ratio_el");

  hzpt_zrap1_lowlpt_mu->Write("h_zpt_zrap_lowlpt_1_mu");
  hzpt_zrap_lowlpt_r12_mu->Write("h_zpt_zrap_lowlpt_ratio_mu");

  fout->Close();

  sprintf(name, "%s.pdf]", outtag.c_str());
  plots->Print(name);


  return 0; 
}
