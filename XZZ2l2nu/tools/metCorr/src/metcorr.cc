// Hengne Li, 2016 @ CERN, initial version.

#include "metcorr.h"


int main(int argc, char** argv) {

  if( argc<5 ) {
     std::cout << argv[0] << ":  " << std::endl ;
     std::cout << " Functionality: met kin-fit framework, actually a full analysis framework  ... "  << std::endl;
     std::cout << "                 "  << std::endl;
     std::cout << " usage: " << argv[0] << " config.file inputfile.root outputfile.root Nevts SumWeights " << std::endl ;
     exit(1) ;
  }

  // config file name
  _file_config_name = std::string((const char*)argv[1]);

  // input file name
  _file_in_name = std::string((const char*)argv[2]);

  // output file name
  _file_out_name = std::string((const char*)argv[3]);

  // sum events
  _SumEvents = atof(argv[4]);

  // sum weights
  _SumWeights = atof(argv[5]);


  // input file
  _file_in = TFile::Open(_file_in_name.c_str());

  // output file 
  _file_out = TFile::Open(_file_out_name.c_str(), "recreate");

  // special for DYJets MC samples,
  //  decide if this is the DYJets samples, and LO or NLO, 
  //  based on the output file names. So...note below:
  // ATTENTION: 
  //   Makes sure output NLO DYJets MC output file name has string
  //   "DYJets" but does not have "MGMLM", and LO DYJets one has 
  //   both strings "DYJets" and "MGMLM". 
  _isDyJets = (_file_out_name.find("DYJets")!=std::string::npos);
  _isDyJetsLO = (_file_out_name.find("DYJets")!=std::string::npos && _file_out_name.find("MGMLM")!=std::string::npos);
  
  if (_debug) std::cout << "DEBUG: isDyJets = " << _isDyJets << ", isDyJetsLO = " << _isDyJetsLO << std::endl;


  // read config file
  readConfigFile();

  // prepare the trees
  prepareTrees();

  // prepare inputs for pu weights
  if (_addPUWeights && !_isData) preparePUWeights();

  // GJets skim
  if (_doGJetsSkim) prepareGJetsSkim();

  // prepare inputs for muon re-calib
  if (_doMuonPtRecalib && !_doGJetsSkim) prepareMuonPtRecalib();


  // prepare inputs for addDyZPtWeight
  if (_addDyZPtWeight && !_isData && _isDyJets && !_doGJetsSkim) prepareDyZPtWeight();

  // prepare inputs for simple met recoil tune.
  if (_doRecoil && ((!_isData && _isDyJets) || (_isData && _doGJetsSkim)) ) prepareRecoil();


  // prepare eff scale factors
  if (_addEffScale && (!_isData || _addEffScaleOnData) && !_doGJetsSkim ) prepareEffScale();


  // loop
  int n_pass = 0;

  // get n selected entries to loop 
  Long64_t n_selected_entries = _selected_entries->GetN();

  for (Int_t i_slist = _n_start; i_slist < n_selected_entries; i_slist++) {

    // get _tree_in's entry number from selected list
    Int_t i = _selected_entries->GetEntry(i_slist);

    // get the selected entry from tree_in
    _tree_in->GetEntry(i);

    // beak if runs more events than  you want to test
    n_pass++;
    if (_n_test>0 && n_pass>_n_test) break;

    //
    if (_debug || i%_n_interval == 0) {
      if (_debug) std::cout << "#############################################" << std::endl;
      std::cout << "##  Entry " << i << ", Run " << _run << ", Event " << _evt << std::endl;
      if (_debug) std::cout << "#############################################" << std::endl;
    }

    // _storeOldBranches
    if (_storeOldBranches && !_doGJetsSkim) storeOldBranches();

    // add pu weights
    if (_addPUWeights && !_isData) addPUWeights();

    // GJets skim
    if (_doGJetsSkim) doGJetsSkim();

    // do muon re-calib
    if (_doMuonPtRecalib && !_doGJetsSkim) doMuonPtRecalib(); 

    //  addDyZPtWeight
    if (_addDyZPtWeight && !_isData && _isDyJets && !_doGJetsSkim) addDyZPtWeight();

    // simple met recoil tune.
    if (_doRecoil && ((!_isData && _isDyJets) || (_isData && _doGJetsSkim))) doRecoil();

    // add eff scale factors
    if (_addEffScale && (!_isData || _addEffScaleOnData) && !_doGJetsSkim ) addEffScale();


    // fill output tree
    _tree_out->Fill(); 
  }  


  // store output tree
  _file_out->cd();
  _tree_out->Write();
  _file_out->Close();

  return 0;

}





//======================================================
// ╦═╗╔═╗╔═╗╔╦╗  ╔═╗╔═╗╔╗╔╔═╗╦╔═╗  ╔═╗╦╦  ╔═╗
// ╠╦╝║╣ ╠═╣ ║║  ║  ║ ║║║║╠╣ ║║ ╦  ╠╣ ║║  ║╣ 
// ╩╚═╚═╝╩ ╩═╩╝  ╚═╝╚═╝╝╚╝╚  ╩╚═╝  ╚  ╩╩═╝╚═╝
//======================================================

void readConfigFile() 
{

  // init paramter file
  PParameterReader parm(_file_config_name.c_str());

  // print it
  parm.Print();

  //===============================================
  // NOTE: 
  //  - See notes above in parameter definitions
  //     for the meanings and usage of them.
  //  - Please always use the same variable name
  //     in the c++ codes and in the config files,
  //     with the c++ one has "_" in front, 
  //      e.g.:   
  //       bool _AAA = parm.GetBool("AAA", kFALSE);
  //     This rule can minimize mistakes.
  //==============================================

  //======================
  // general parameters
  //======================
  _debug = parm.GetBool("debug", kFALSE);
  _n_start = parm.GetInt("n_start", 0);
  _n_test = parm.GetInt("n_test", -1);
  _n_interval = parm.GetInt("n_interval", 3000);
  _selection = parm.GetString("selection", "(1)");
  _storeOldBranches = parm.GetBool("storeOldBranches", kFALSE);


  //==========================
  // use slimmed tree or not
  //=========================
  _useLightTree = parm.GetBool("useLightTree", kTRUE);


  //=========================
  // add PU weights
  //=========================
  _addPUWeights = parm.GetBool("addPUWeights", kTRUE);

  if (_addPUWeights) {
    _PUTags = parm.GetVString("PUTags");
    _PUInputDir = parm.GetString("PUInputDir", "data/pileup");
    _PUInputFileNames = parm.GetVString("PUInputFileNames");
    _PUWeightHistName = parm.GetString("PUWeightHistName", "puweight_dtmc");
  }

  //==========================
  // muon pT recalibration
  //==========================
  _doMuonPtRecalib = parm.GetBool("doMuonPtRecalib", kFALSE);

  if (_doMuonPtRecalib) {
    _MuonPtRecalibInputForData = parm.GetString("MuonPtRecalibInputForData", "data/kalman/DATA_80X_13TeV.root");
    _MuonPtRecalibInputForMC = parm.GetString("MuonPtRecalibInputForMC", "data/kalman/MC_80X_13TeV.root");
  }

  //========================
  // Add DYJet gen reweight 
  //========================
  _addDyZPtWeight = parm.GetBool("addDyZPtWeight", kTRUE);
  
  if (_addDyZPtWeight) {
    _addDyZPtWeightUseFunction = parm.GetBool("addDyZPtWeightUseFunction", kTRUE);
    _addDyZPtWeightUseResummationFunction = parm.GetBool("addDyZPtWeightUseResummationFunction", kFALSE);
    _addDyZPtWeightLOUseFunction = parm.GetBool("addDyZPtWeightLOUseFunction", kTRUE);
    _DyZPtWeightInputFileName = parm.GetString("DyZPtWeightInputFileName", "data/zptweight/dyjets_zpt_weight_lo_nlo_sel.root");
    _addDyNewGenWeight = parm.GetBool("addDyNewGenWeight", kTRUE);;
  }

  //==============================================
  // Do simple version of the MET recoil tuning
  //==============================================
  _doRecoil = parm.GetBool("doRecoil", kFALSE);

  if (_doRecoil) {
    _doRecoilUseSmooth = parm.GetBool("doRecoilUseSmooth", kTRUE);
    _doRecoilUseSmoothGraph = parm.GetBool("doRecoilUseSmoothGraph", kTRUE);
    _RecoilInputFileNameData_all = parm.GetString("RecoilInputFileNameData_all", "data/recoil/SingleEMU_Run2016BCD_PromptReco_met_para_study.root"); 
    _RecoilInputFileNameData_mu = parm.GetString("RecoilInputFileNameData_mu", "data/recoil/SingleEMU_Run2016BCD_PromptReco_met_para_study_mu.root"); 
    _RecoilInputFileNameData_el = parm.GetString("RecoilInputFileNameData_el", "data/recoil/SingleEMU_Run2016BCD_PromptReco_met_para_study_el.root"); 
    _RecoilInputFileNameMC_all = parm.GetString("RecoilInputFileNameMC_all", "data/recoil/DYJetsToLL_M50_NoRecoil_met_para_study.root"); 
    _RecoilInputFileNameMC_mu = parm.GetString("RecoilInputFileNameMC_mu", "data/recoil/DYJetsToLL_M50_NoRecoil_met_para_study_mu.root"); 
    _RecoilInputFileNameMC_el = parm.GetString("RecoilInputFileNameMC_el", "data/recoil/DYJetsToLL_M50_NoRecoil_met_para_study_el.root"); 
    _RecoilInputFileNameMCLO_all = parm.GetString("RecoilInputFileNameMCLO_all", "data/recoil/DYJetsToLL_M50_MGMLM_Ext1_NoRecoil_met_para_study.root"); 
    _RecoilInputFileNameMCLO_mu = parm.GetString("RecoilInputFileNameMCLO_mu", "data/recoil/DYJetsToLL_M50_MGMLM_Ext1_NoRecoil_met_para_study_mu.root"); 
    _RecoilInputFileNameMCLO_el = parm.GetString("RecoilInputFileNameMCLO_el", "data/recoil/DYJetsToLL_M50_MGMLM_Ext1_NoRecoil_met_para_study_el.root"); 
    _RecoilInputFileNameGJets_all = parm.GetString("RecoilInputFileNameGJets_all", "data/recoil/SinglePhoton_Run2016BCD_PromptReco_met_para_study_ZSelecLowLPt.root"); 
    _RecoilInputFileNameGJets_mu = parm.GetString("RecoilInputFileNameGJets_mu", "data/recoil/SinglePhoton_Run2016BCD_PromptReco_met_para_study_ZSelecLowLPt_mu.root"); 
    _RecoilInputFileNameGJets_el = parm.GetString("RecoilInputFileNameGJets_el", "data/recoil/SinglePhoton_Run2016BCD_PromptReco_met_para_study_ZSelecLowLPt_el.root"); 
  }

  //==============================================
  // Add efficiency scale factors
  //==============================================  
  _addEffScale = parm.GetBool("addEffScale", kFALSE);
  
  if (_addEffScale){
    _addEffScaleOnData = parm.GetBool("addEffScaleOnData", kFALSE);
    _EffScaleInputFileName_IdIso_El = parm.GetString("EffScaleInputFileName_IdIso_El", "data/eff/egammaEffi.txt_SF2D.root");
    _EffScaleInputFileName_Trk_El = parm.GetString("EffScaleInputFileName_Trk_El", "data/eff/egammatracking.root");
    _EffScaleInputFileName_IdIso_Mu = parm.GetString("EffScaleInputFileName_IdIso_Mu", "data/eff/muon80x12p9.root");
    _EffScaleInputFileName_Trk_Mu = parm.GetString("EffScaleInputFileName_Trk_Mu", "data/eff/muontrackingsf.root");
    _EffScaleInputFileName_Trg_El = parm.GetString("EffScaleInputFileName_Trg_El", "data/eff/trigereff12p9.root");
    _EffScaleInputFileName_Trg_Mu = parm.GetString("EffScaleInputFileName_Trg_Mu", "data/eff/trigeff_mu.root");
  }


  //==============================================
  // Do GJets skimming
  //==============================================  
  _doGJetsSkim = parm.GetBool("doGJetsSkim", kFALSE);
  _doGJetsSkimAddPhiWeight = parm.GetBool("doGJetsSkimAddPhiWeight", kFALSE);

  if (_doGJetsSkim) {
    _GJetsSkimInputFileName = parm.GetString("GJetsSkimInputFileName", "data/gjets/study_gjets.root");
    if (_doGJetsSkimAddPhiWeight) {
      _GJetsSkimPhiWeightInputFileName = parm.GetString("GJetsSkimPhiWeightInputFileName", "data/gjets/gjet_photon_phi_weight.root");
    }
  }

}




// prepare the trees
bool  prepareTrees() 
{

  // 1.) prepare in put tree:

  // input tree
  _tree_in = (TTree*)_file_in->Get("tree");

  // selection 
  _tree_in->Draw(">>_selected_entries", _selection.c_str(), "entrylist");
  _selected_entries = (TEntryList*)gDirectory->Get("_selected_entries");

  // isData  
  _tree_in->SetBranchAddress("isData",&_isData);

  // check if tree has events
  if (_tree_in->GetEntries()<=0) {
    std::cout << "prepareTrees():: input tree has no event pass selection. Quit." <<std::endl;
    return false;
  }

  // get isData info
  _tree_in->GetEntry(0);

  // set common branches
  _tree_in->SetBranchAddress("run", &_run);
  _tree_in->SetBranchAddress("lumi", &_lumi);
  _tree_in->SetBranchAddress("evt", &_evt);

  if (_doGJetsSkim) {
    _tree_in->SetBranchAddress("gjet_mt", &_gjet_mt);
    _tree_in->SetBranchAddress("gjet_l1_pt", &_gjet_l1_pt);
    _tree_in->SetBranchAddress("gjet_l1_eta", &_gjet_l1_eta);
    _tree_in->SetBranchAddress("gjet_l1_rapidity", &_gjet_l1_rapidity);
    _tree_in->SetBranchAddress("gjet_l1_phi", &_gjet_l1_phi);
    _tree_in->SetBranchAddress("gjet_l1_idCutBased", &_gjet_l1_idCutBased);
    _tree_in->SetBranchAddress("gjet_l2_pt", &_gjet_l2_pt);
    _tree_in->SetBranchAddress("gjet_l2_phi", &_gjet_l2_phi);
    _tree_in->SetBranchAddress("gjet_l2_sumEt", &_gjet_l2_sumEt);
    _tree_in->SetBranchAddress("gjet_l2_rawPt", &_gjet_l2_rawPt);
    _tree_in->SetBranchAddress("gjet_l2_rawPhi", &_gjet_l2_rawPhi);
    _tree_in->SetBranchAddress("gjet_l2_rawSumEt", &_gjet_l2_rawSumEt);
    if (!_isData) {
      _tree_in->SetBranchAddress("gjet_l2_genPhi", &_gjet_l2_genPhi);
      _tree_in->SetBranchAddress("gjet_l2_genEta", &_gjet_l2_genEta);
    }
  } 
  else {
    _tree_in->SetBranchAddress("llnunu_mt", &_llnunu_mt);
    _tree_in->SetBranchAddress("llnunu_l1_mass",&_llnunu_l1_mass);
    _tree_in->SetBranchAddress("llnunu_l1_mt", &_llnunu_l1_mt);
    _tree_in->SetBranchAddress("llnunu_l1_pt", &_llnunu_l1_pt);
    _tree_in->SetBranchAddress("llnunu_l1_phi", &_llnunu_l1_phi);
    _tree_in->SetBranchAddress("llnunu_l1_eta", &_llnunu_l1_eta);
    _tree_in->SetBranchAddress("llnunu_l1_deltaPhi", &_llnunu_l1_deltaPhi);
    _tree_in->SetBranchAddress("llnunu_l1_deltaR", &_llnunu_l1_deltaR);
    _tree_in->SetBranchAddress("llnunu_l1_rapidity", &_llnunu_l1_rapidity);

    _tree_in->SetBranchAddress("llnunu_l2_pt", &_llnunu_l2_pt);
    _tree_in->SetBranchAddress("llnunu_l2_phi", &_llnunu_l2_phi);

    _tree_in->SetBranchAddress("llnunu_l1_l1_pt", &_llnunu_l1_l1_pt);
    _tree_in->SetBranchAddress("llnunu_l1_l1_eta", &_llnunu_l1_l1_eta);
    _tree_in->SetBranchAddress("llnunu_l1_l1_phi", &_llnunu_l1_l1_phi);
    _tree_in->SetBranchAddress("llnunu_l1_l1_rapidity", &_llnunu_l1_l1_rapidity);
    _tree_in->SetBranchAddress("llnunu_l1_l1_mass", &_llnunu_l1_l1_mass);
    _tree_in->SetBranchAddress("llnunu_l1_l1_pdgId", &_llnunu_l1_l1_pdgId);
    _tree_in->SetBranchAddress("llnunu_l1_l1_charge", &_llnunu_l1_l1_charge);
    _tree_in->SetBranchAddress("llnunu_l1_l1_ptErr", &_llnunu_l1_l1_ptErr);
    _tree_in->SetBranchAddress("llnunu_l1_l1_eSCeta", &_llnunu_l1_l1_eSCeta);

    _tree_in->SetBranchAddress("llnunu_l1_l2_pt", &_llnunu_l1_l2_pt);
    _tree_in->SetBranchAddress("llnunu_l1_l2_eta", &_llnunu_l1_l2_eta);
    _tree_in->SetBranchAddress("llnunu_l1_l2_phi", &_llnunu_l1_l2_phi);
    _tree_in->SetBranchAddress("llnunu_l1_l2_rapidity", &_llnunu_l1_l2_rapidity);
    _tree_in->SetBranchAddress("llnunu_l1_l2_mass", &_llnunu_l1_l2_mass);
    _tree_in->SetBranchAddress("llnunu_l1_l2_pdgId", &_llnunu_l1_l2_pdgId);
    _tree_in->SetBranchAddress("llnunu_l1_l2_charge", &_llnunu_l1_l2_charge);
    _tree_in->SetBranchAddress("llnunu_l1_l2_ptErr", &_llnunu_l1_l2_ptErr);
    _tree_in->SetBranchAddress("llnunu_l1_l2_eSCeta", &_llnunu_l1_l2_eSCeta);

  }

  // other branches for not light weight tree   
  if (!_useLightTree) {
    std::cout << " Warning: for not _useLightTree, to be implemented." << std::endl;
  }

  // 
  // MC only
  if (!_isData) {
    _tree_in->SetBranchAddress("nTrueInt", &_nTrueInt );
    _tree_in->SetBranchAddress("genWeight",&_genWeight);
  }

  if (!_isData&&!_doGJetsSkim) {
    _tree_in->SetBranchAddress("ngenZ", &_ngenZ);
    _tree_in->SetBranchAddress("genZ_pt", _genZ_pt);
  }


  // 2.) Output tree

  // output tree
  _file_out->cd();
  _tree_out = _tree_in->CloneTree(0);

  // sum of events and weights
  _tree_out->Branch("SumEvents", &_SumEvents, "SumEvents/D");
  _tree_out->Branch("SumWeights", &_SumWeights, "SumWeights/D");

  // keep a copy of old branches
  if (_storeOldBranches && !_doGJetsSkim) {
    _tree_out->Branch("llnunu_mt_old", &_llnunu_mt_old, "llnunu_mt_old/F");
    _tree_out->Branch("llnunu_l1_mass_old", &_llnunu_l1_mass_old, "llnunu_l1_mass_old/F");
    _tree_out->Branch("llnunu_l1_pt_old", &_llnunu_l1_pt_old, "llnunu_l1_pt_old/F");
    _tree_out->Branch("llnunu_l1_phi_old", &_llnunu_l1_phi_old, "llnunu_l1_phi_old/F");
    _tree_out->Branch("llnunu_l1_eta_old", &_llnunu_l1_eta_old, "llnunu_l1_eta_old/F");
    _tree_out->Branch("llnunu_l2_pt_old", &_llnunu_l2_pt_old, "llnunu_l2_pt_old/F");
    _tree_out->Branch("llnunu_l2_phi_old", &_llnunu_l2_phi_old, "llnunu_l2_phi_old/F");
    _tree_out->Branch("llnunu_l1_l1_pt_old", &_llnunu_l1_l1_pt_old, "llnunu_l1_l1_pt_old/F");
    _tree_out->Branch("llnunu_l1_l1_eta_old", &_llnunu_l1_l1_eta_old, "llnunu_l1_l1_eta_old/F");
    _tree_out->Branch("llnunu_l1_l1_phi_old", &_llnunu_l1_l1_phi_old, "llnunu_l1_l1_phi_old/F");
    _tree_out->Branch("llnunu_l1_l1_ptErr_old", &_llnunu_l1_l1_ptErr_old, "llnunu_l1_l1_ptErr_old/F");
    _tree_out->Branch("llnunu_l1_l2_pt_old", &_llnunu_l1_l2_pt_old, "llnunu_l1_l2_pt_old/F");
    _tree_out->Branch("llnunu_l1_l2_eta_old", &_llnunu_l1_l2_eta_old, "llnunu_l1_l2_eta_old/F");
    _tree_out->Branch("llnunu_l1_l2_phi_old", &_llnunu_l1_l2_phi_old, "llnunu_l1_l2_phi_old/F");
    _tree_out->Branch("llnunu_l1_l2_ptErr_old", &_llnunu_l1_l2_ptErr_old, "llnunu_l1_l2_ptErr_old/F");
  }

  // GJets Skim
  if (_doGJetsSkim){
    _tree_out->Branch("GJetsPhiWeight", &_GJetsPhiWeight, "GJetsPhiWeight/F");
    _tree_out->Branch("GJetsWeight", &_GJetsWeight, "GJetsWeight/F");
    _tree_out->Branch("GJetsWeightEl", &_GJetsWeightEl, "GJetsWeightEl/F");
    _tree_out->Branch("GJetsWeightMu", &_GJetsWeightMu, "GJetsWeightMu/F");
    _tree_out->Branch("GJetsWeight_up", &_GJetsWeight_up, "GJetsWeight_up/F");
    _tree_out->Branch("GJetsWeightEl_up", &_GJetsWeightEl_up, "GJetsWeightEl_up/F");
    _tree_out->Branch("GJetsWeightMu_up", &_GJetsWeightMu_up, "GJetsWeightMu_up/F");
    _tree_out->Branch("GJetsWeight_dn", &_GJetsWeight_dn, "GJetsWeight_dn/F");
    _tree_out->Branch("GJetsWeightEl_dn", &_GJetsWeightEl_dn, "GJetsWeightEl_dn/F");
    _tree_out->Branch("GJetsWeightMu_dn", &_GJetsWeightMu_dn, "GJetsWeightMu_dn/F");
    _tree_out->Branch("GJetsWeightLowLPt", &_GJetsWeightLowLPt, "GJetsWeightLowLPt/F");
    _tree_out->Branch("GJetsWeightLowLPtEl", &_GJetsWeightLowLPtEl, "GJetsWeightLowLPtEl/F");
    _tree_out->Branch("GJetsWeightLowLPtMu", &_GJetsWeightLowLPtMu, "GJetsWeightLowLPtMu/F");
    _tree_out->Branch("GJetsZPtWeight", &_GJetsZPtWeight, "GJetsZPtWeight/F");
    _tree_out->Branch("GJetsZPtWeightEl", &_GJetsZPtWeightEl, "GJetsZPtWeightEl/F");
    _tree_out->Branch("GJetsZPtWeightMu", &_GJetsZPtWeightMu, "GJetsZPtWeightMu/F");
    _tree_out->Branch("GJetsZPtWeightLowLPt", &_GJetsZPtWeightLowLPt, "GJetsZPtWeightLowLPt/F");
    _tree_out->Branch("GJetsZPtWeightLowLPtEl", &_GJetsZPtWeightLowLPtEl, "GJetsZPtWeightLowLPtEl/F");
    _tree_out->Branch("GJetsZPtWeightLowLPtMu", &_GJetsZPtWeightLowLPtMu, "GJetsZPtWeightLowLPtMu/F");
    _tree_out->Branch("llnunu_mt", &_llnunu_mt, "llnunu_mt/F");
    _tree_out->Branch("llnunu_l1_mass", &_llnunu_l1_mass, "llnunu_l1_mass/F");
    _tree_out->Branch("llnunu_l1_pt", &_llnunu_l1_pt, "llnunu_l1_pt/F");
    _tree_out->Branch("llnunu_l1_phi", &_llnunu_l1_phi, "llnunu_l1_phi/F");
    _tree_out->Branch("llnunu_l1_eta", &_llnunu_l1_eta, "llnunu_l1_eta/F");
    _tree_out->Branch("llnunu_l1_rapidity", &_llnunu_l1_rapidity, "llnunu_l1_rapidity/F");
    _tree_out->Branch("llnunu_l2_pt", &_llnunu_l2_pt, "llnunu_l2_pt/F");
    _tree_out->Branch("llnunu_l2_phi", &_llnunu_l2_phi, "llnunu_l2_phi/F");
    _tree_out->Branch("llnunu_l2_sumEt", &_llnunu_l2_sumEt, "llnunu_l2_sumEt/F");
    _tree_out->Branch("llnunu_l2_rawPt", &_llnunu_l2_rawPt, "llnunu_l2_rawPt/F");
    _tree_out->Branch("llnunu_l2_rawPhi", &_llnunu_l2_rawPhi, "llnunu_l2_rawPhi/F");
    _tree_out->Branch("llnunu_l2_rawSumEt", &_llnunu_l2_rawSumEt, "llnunu_l2_rawSumEt/F");
    _tree_out->Branch("llnunu_l1_l1_pt", &_llnunu_l1_l1_pt, "llnunu_l1_l1_pt/F");
    _tree_out->Branch("llnunu_l1_l1_eta", &_llnunu_l1_l1_eta, "llnunu_l1_l1_eta/F");
    _tree_out->Branch("llnunu_l1_l1_pdgId", &_llnunu_l1_l1_pdgId, "llnunu_l1_l1_pdgId/I");
    _tree_out->Branch("llnunu_l1_l2_pt", &_llnunu_l1_l2_pt, "llnunu_l1_l2_pt/F");
    _tree_out->Branch("llnunu_l1_l2_eta", &_llnunu_l1_l2_eta, "llnunu_l1_l2_eta/F");
    _tree_out->Branch("llnunu_l1_l2_pdgId", &_llnunu_l1_l2_pdgId, "llnunu_l1_l2_pdgId/I");
    _tree_out->Branch("llnunu_l1_l1_highPtID", &_llnunu_l1_l1_highPtID, "llnunu_l1_l1_highPtID/F");
    _tree_out->Branch("llnunu_l1_l2_highPtID", &_llnunu_l1_l2_highPtID, "llnunu_l1_l2_highPtID/F");
    _tree_out->Branch("llnunu_mt_el", &_llnunu_mt_el, "llnunu_mt_el/F");
    _tree_out->Branch("llnunu_mt_mu", &_llnunu_mt_mu, "llnunu_mt_mu/F");
    _tree_out->Branch("llnunu_l1_mass_el", &_llnunu_l1_mass_el, "llnunu_l1_mass_el/F");
    _tree_out->Branch("llnunu_l1_mass_mu", &_llnunu_l1_mass_mu, "llnunu_l1_mass_mu/F");
    _tree_out->Branch("llnunu_l2_pt_el", &_llnunu_l2_pt_el, "llnunu_l2_pt_el/F");
    _tree_out->Branch("llnunu_l2_phi_el", &_llnunu_l2_phi_el, "llnunu_l2_phi_el/F");
    _tree_out->Branch("llnunu_l2_pt_mu", &_llnunu_l2_pt_mu, "llnunu_l2_pt_mu/F");
    _tree_out->Branch("llnunu_l2_phi_mu", &_llnunu_l2_phi_mu, "llnunu_l2_phi_mu/F");
    if (!_isData) {
      _tree_out->Branch("llnunu_l2_genPhi", &_llnunu_l2_genPhi, "llnunu_l2_genPhi/F");
      _tree_out->Branch("llnunu_l2_genEta", &_llnunu_l2_genEta, "llnunu_l2_genEta/F");
    }
    if (!_storeOldBranches) {
      _tree_out->SetBranchStatus("gjet_*", 0);
      _tree_out->SetBranchStatus("gjet_l1_idCutBased", 1);
      _tree_out->SetBranchStatus("gjet_l1_hOverE", 1);
      _tree_out->SetBranchStatus("gjet_l1_r9", 1);
      _tree_out->SetBranchStatus("gjet_l1_sigmaIetaIeta", 1);
    }
  }

  return true;

}

// store Old Branches
void storeOldBranches()
{
  _llnunu_mt_old = _llnunu_mt;
  _llnunu_l1_mass_old = _llnunu_l1_mass;
  _llnunu_l1_pt_old = _llnunu_l1_pt;
  _llnunu_l1_phi_old = _llnunu_l1_phi;
  _llnunu_l1_eta_old = _llnunu_l1_eta;
  _llnunu_l2_pt_old = _llnunu_l2_pt;
  _llnunu_l2_phi_old = _llnunu_l2_phi;
  _llnunu_l1_l1_pt_old = _llnunu_l1_l1_pt;
  _llnunu_l1_l1_eta_old = _llnunu_l1_l1_eta;
  _llnunu_l1_l1_phi_old = _llnunu_l1_l1_phi;
  _llnunu_l1_l1_ptErr_old = _llnunu_l1_l1_ptErr;
  _llnunu_l1_l2_pt_old = _llnunu_l1_l2_pt;
  _llnunu_l1_l2_eta_old = _llnunu_l1_l2_eta;
  _llnunu_l1_l2_phi_old = _llnunu_l1_l2_phi;
  _llnunu_l1_l2_ptErr_old = _llnunu_l1_l2_ptErr;
}

// prepare inputs for pu weights
void preparePUWeights()
{
  // for each puWeight, read input weight hist and create branch
  for (int i=0; i<(int)_PUTags.size(); i++) {
    sprintf(name, "%s/%s", _PUInputDir.c_str(), _PUInputFileNames.at(i).c_str());
    _PUFiles.push_back(new TFile(name));
    _PUHists.push_back((TH1D*)_PUFiles.at(i)->Get(_PUWeightHistName.c_str()));
    _PUWeights.push_back(new Float_t(1.0));
    sprintf(name, "puWeight%s", _PUTags.at(i).c_str());
    sprintf(name1, "puWeight%s/F", _PUTags.at(i).c_str());
    _tree_out->Branch(name, _PUWeights.at(i),name1);
  }

}

// add more pileup weights
void addPUWeights() 
{
  // for each puWeight, get the weight
  for (int i=0; i<(int)_PUTags.size(); i++){
    *(_PUWeights.at(i)) = _PUHists.at(i)->GetBinContent(_PUHists.at(i)->FindBin(_nTrueInt));
  }
}


// prepare inputs for muon re-calib
void prepareMuonPtRecalib()
{
  if (_isData) _muCalib = new KalmanMuonCalibrator(_MuonPtRecalibInputForData.c_str());
  else _muCalib = new KalmanMuonCalibrator(_MuonPtRecalibInputForMC.c_str());
}


// do muon re-calib
void doMuonPtRecalib()
{
  if (abs(_llnunu_l1_l1_pdgId)==13&&abs(_llnunu_l1_l2_pdgId)==13 && _llnunu_l1_l1_pt>2. && _llnunu_l1_l1_pt<200. ) {
    _llnunu_l1_l1_pt = (Float_t)_muCalib->getCorrectedPt(double(_llnunu_l1_l1_pt), double(_llnunu_l1_l1_eta), double(_llnunu_l1_l1_phi), double(_llnunu_l1_l1_charge));
    _llnunu_l1_l2_pt = (Float_t)_muCalib->getCorrectedPt(double(_llnunu_l1_l2_pt), double(_llnunu_l1_l2_eta), double(_llnunu_l1_l2_phi), double(_llnunu_l1_l2_charge));
    if (!_isData) {
      _llnunu_l1_l1_pt = (Float_t)_muCalib->smear(double(_llnunu_l1_l1_pt), double(_llnunu_l1_l1_eta));
      _llnunu_l1_l2_pt = (Float_t)_muCalib->smear(double(_llnunu_l1_l2_pt), double(_llnunu_l1_l2_eta));
    }
    _llnunu_l1_l1_ptErr = _llnunu_l1_l1_pt*(Float_t)_muCalib->getCorrectedError(double(_llnunu_l1_l1_pt), double(_llnunu_l1_l1_eta), double(_llnunu_l1_l1_ptErr/_llnunu_l1_l1_pt));
    _llnunu_l1_l2_ptErr = _llnunu_l1_l2_pt*(Float_t)_muCalib->getCorrectedError(double(_llnunu_l1_l2_pt), double(_llnunu_l1_l2_eta), double(_llnunu_l1_l2_ptErr/_llnunu_l1_l2_pt));
    TLorentzVector l1v, l2v;
    l1v.SetPtEtaPhiM(_llnunu_l1_l1_pt, _llnunu_l1_l1_eta, _llnunu_l1_l1_phi, _llnunu_l1_l1_mass);
    l2v.SetPtEtaPhiM(_llnunu_l1_l2_pt, _llnunu_l1_l2_eta, _llnunu_l1_l2_phi, _llnunu_l1_l2_mass);
    TLorentzVector zv = l1v+l2v;
    _llnunu_l1_l1_rapidity = (Float_t)l1v.Rapidity(); 
    _llnunu_l1_l2_rapidity = (Float_t)l2v.Rapidity();
    _llnunu_l1_pt = (Float_t)zv.Pt();
    _llnunu_l1_eta = (Float_t)zv.Eta();
    _llnunu_l1_phi = (Float_t)zv.Phi();
    _llnunu_l1_rapidity = (Float_t)zv.Rapidity();
    _llnunu_l1_deltaPhi = (Float_t)l1v.DeltaPhi(l2v);
    _llnunu_l1_deltaR = (Float_t)l1v.DeltaR(l2v);
    _llnunu_l1_mt = (Float_t)zv.Mt();
    _llnunu_l1_mass = (Float_t)zv.M();

    TVector2 vec_met(_llnunu_l2_pt*cos(_llnunu_l2_phi), _llnunu_l2_pt*sin(_llnunu_l2_phi));

    Float_t et1 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l1_pt*_llnunu_l1_pt);
    Float_t et2 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l2_pt*_llnunu_l2_pt);
    _llnunu_mt = TMath::Sqrt(2.0*_llnunu_l1_mass*_llnunu_l1_mass+2.0*(et1*et2
               -_llnunu_l1_pt*cos(_llnunu_l1_phi)*_llnunu_l2_pt*cos(_llnunu_l2_phi)
               -_llnunu_l1_pt*sin(_llnunu_l1_phi)*_llnunu_l2_pt*sin(_llnunu_l2_phi)));
  }
}


// prepare inputs for addDyZPtWeight
void prepareDyZPtWeight()
{

  // needed output branches
  _tree_out->Branch("ZPtWeight", &_ZPtWeight, "ZPtWeight/F");
  _tree_out->Branch("ZPtWeight_up", &_ZPtWeight_up, "ZPtWeight_up/F");
  _tree_out->Branch("ZPtWeight_dn", &_ZPtWeight_dn, "ZPtWeight_dn/F");
  if (_addDyNewGenWeight) {
    _tree_out->Branch("ZJetsGenWeight", &_ZJetsGenWeight, "ZJetsGenWeight/F");
  }

  // get input file and building materials
  _fdyzpt = new TFile(_DyZPtWeightInputFileName.c_str());
  _hdyzpt_dtmc_ratio = (TH1D*)_fdyzpt->Get("hdyzpt_dtmc_ratio");
  _fcdyzpt_dtmc_ratio = (TF1*)_fdyzpt->Get("fcdyzpt_dtmc_ratio");
  _fcdyzpt_dtmc_ratio_resbos = (TF1*)_fdyzpt->Get("fcdyzpt_dtmc_ratio_resbos");
  _hdyzpt_mc_nlo_lo_ratio = (TH1D*)_fdyzpt->Get("hdyzpt_mc_nlo_lo_ratio");
  _fcdyzpt_mc_nlo_lo_ratio = (TF1*)_fdyzpt->Get("fcdyzpt_mc_nlo_lo_ratio");
  
}

// addDyZPtWeight
void addDyZPtWeight()
{
  if (_addDyZPtWeight && !_isData && _isDyJets) {
    Int_t zptBin=0;
    if (_ngenZ>0) {
      zptBin = _hdyzpt_dtmc_ratio->FindBin(_genZ_pt[0]);
      if (_genZ_pt[0]>1000) zptBin = _hdyzpt_dtmc_ratio->FindBin(999);
    }
    else {
      zptBin = _hdyzpt_dtmc_ratio->FindBin(_llnunu_l1_pt);
      if (_llnunu_l1_pt>1000) zptBin = _hdyzpt_dtmc_ratio->FindBin(999);
    }
    if (_addDyZPtWeightUseFunction && !_addDyZPtWeightUseResummationFunction) {
      if (_ngenZ>0) _ZPtWeight = _fcdyzpt_dtmc_ratio->Eval(_genZ_pt[0]);
      else _ZPtWeight = _fcdyzpt_dtmc_ratio->Eval(_llnunu_l1_pt);
    }
    else if (_addDyZPtWeightUseFunction && _addDyZPtWeightUseResummationFunction) {
      if (_ngenZ>0) _ZPtWeight = _fcdyzpt_dtmc_ratio_resbos->Eval(_genZ_pt[0]);
      else _ZPtWeight = _fcdyzpt_dtmc_ratio_resbos->Eval(_llnunu_l1_pt);
    }
    else {
      _ZPtWeight = _hdyzpt_dtmc_ratio->GetBinContent(zptBin);
    }
    _ZPtWeight_up = _ZPtWeight+0.5*_hdyzpt_dtmc_ratio->GetBinError(zptBin);
    _ZPtWeight_dn = _ZPtWeight-0.5*_hdyzpt_dtmc_ratio->GetBinError(zptBin);

    if (_isDyJetsLO) {
      // for LO ZJets samples
      zptBin=0;
      if (_ngenZ>0) {
        zptBin = _hdyzpt_mc_nlo_lo_ratio->FindBin(_genZ_pt[0]);
        if (_genZ_pt[0]>1000) zptBin = _hdyzpt_mc_nlo_lo_ratio->FindBin(999);
      }
      else {
        zptBin = _hdyzpt_mc_nlo_lo_ratio->FindBin(_llnunu_l1_pt);
        if (_llnunu_l1_pt>1000) zptBin = _hdyzpt_mc_nlo_lo_ratio->FindBin(999);
      }
      if (_addDyZPtWeightLOUseFunction) {
        if (_ngenZ>0) _ZPtWeight *= _fcdyzpt_mc_nlo_lo_ratio->Eval(_genZ_pt[0]);
        else _ZPtWeight *= _fcdyzpt_mc_nlo_lo_ratio->Eval(_llnunu_l1_pt);
      }
      else {
        _ZPtWeight *= _hdyzpt_mc_nlo_lo_ratio->GetBinContent(zptBin);
      }

      _ZPtWeight_up = _ZPtWeight+0.5*_hdyzpt_dtmc_ratio->GetBinError(zptBin);
      _ZPtWeight_dn = _ZPtWeight-0.5*_hdyzpt_dtmc_ratio->GetBinError(zptBin);

    }
  }

  if (_addDyNewGenWeight && !_isData && _isDyJets) {
    Float_t ZJetsLOSumWeights(49877138);
    Float_t ZJetsNLOSumWeights(450670522117);
    Float_t ZJetsLOSumEvents(49877138);
    Float_t ZJetsNLOSumEvents(28696958);
    if (!_isDyJetsLO) {
      _ZJetsGenWeight = _genWeight/ZJetsNLOSumWeights*ZJetsNLOSumEvents/(ZJetsNLOSumEvents+ZJetsLOSumEvents);
    }
    else {
      _ZJetsGenWeight = _genWeight/ZJetsLOSumWeights*ZJetsLOSumEvents/(ZJetsNLOSumEvents+ZJetsLOSumEvents);
    }
  }

}

// prepare inputs for simple met recoil tune.
void prepareRecoil()
{
  if (_doRecoil && (_isDyJets||_doGJetsSkim)) {
    // met shift  sigma
    _file_dt_sigma[0] = new TFile(_RecoilInputFileNameData_all.c_str());
    _file_dt_sigma[1] = new TFile(_RecoilInputFileNameData_mu.c_str());
    _file_dt_sigma[2] = new TFile(_RecoilInputFileNameData_el.c_str());
    if (_doGJetsSkim){
      _file_mc_sigma[0] = new TFile(_RecoilInputFileNameGJets_all.c_str());
      _file_mc_sigma[1] = new TFile(_RecoilInputFileNameGJets_mu.c_str());
      _file_mc_sigma[2] = new TFile(_RecoilInputFileNameGJets_el.c_str());
    }
    else if (!_isDyJetsLO) {
      _file_mc_sigma[0] = new TFile(_RecoilInputFileNameMC_all.c_str());
      _file_mc_sigma[1] = new TFile(_RecoilInputFileNameMC_mu.c_str());
      _file_mc_sigma[2] = new TFile(_RecoilInputFileNameMC_el.c_str());
    }
    else {
      _file_mc_sigma[0] = new TFile(_RecoilInputFileNameMCLO_all.c_str());
      _file_mc_sigma[1] = new TFile(_RecoilInputFileNameMCLO_mu.c_str());
      _file_mc_sigma[2] = new TFile(_RecoilInputFileNameMCLO_el.c_str());
    }
    _h_dt_met_para_shift[0] = (TH1D*)_file_dt_sigma[0]->Get("h_met_para_vs_zpt_mean");
    _h_mc_met_para_shift[0] = (TH1D*)_file_mc_sigma[0]->Get("h_met_para_vs_zpt_mean");
    _h_met_para_shift_dtmc[0] = (TH1D*)_h_dt_met_para_shift[0]->Clone("h_met_para_shift_dtmc_all");
    _h_met_para_shift_dtmc[0]->Add(_h_mc_met_para_shift[0], -1);

    _h_dt_met_para_sigma[0] = (TH1D*)_file_dt_sigma[0]->Get("h_met_para_vs_zpt_sigma");
    _h_dt_met_perp_sigma[0] = (TH1D*)_file_dt_sigma[0]->Get("h_met_perp_vs_zpt_sigma");
    _h_mc_met_para_sigma[0] = (TH1D*)_file_mc_sigma[0]->Get("h_met_para_vs_zpt_sigma");
    _h_mc_met_perp_sigma[0] = (TH1D*)_file_mc_sigma[0]->Get("h_met_perp_vs_zpt_sigma");

    _h_ratio_met_para_sigma_dtmc[0] = (TH1D*)_h_dt_met_para_sigma[0]->Clone("h_ratio_met_para_sigma_dtmc_all");
    _h_ratio_met_perp_sigma_dtmc[0] = (TH1D*)_h_dt_met_perp_sigma[0]->Clone("h_ratio_met_perp_sigma_dtmc_all");
    _h_ratio_met_para_sigma_dtmc[0]->Divide(_h_mc_met_para_sigma[0]);
    _h_ratio_met_perp_sigma_dtmc[0]->Divide(_h_mc_met_perp_sigma[0]);

    _h_dt_met_para_shift[1] = (TH1D*)_file_dt_sigma[1]->Get("h_met_para_vs_zpt_mean");
    _h_mc_met_para_shift[1] = (TH1D*)_file_mc_sigma[1]->Get("h_met_para_vs_zpt_mean");
    _h_met_para_shift_dtmc[1] = (TH1D*)_h_dt_met_para_shift[1]->Clone("h_met_para_shift_dtmc_mu");
    _h_met_para_shift_dtmc[1]->Add(_h_mc_met_para_shift[1], -1);

    _h_dt_met_para_sigma[1] = (TH1D*)_file_dt_sigma[1]->Get("h_met_para_vs_zpt_sigma");
    _h_dt_met_perp_sigma[1] = (TH1D*)_file_dt_sigma[1]->Get("h_met_perp_vs_zpt_sigma");
    _h_mc_met_para_sigma[1] = (TH1D*)_file_mc_sigma[1]->Get("h_met_para_vs_zpt_sigma");
    _h_mc_met_perp_sigma[1] = (TH1D*)_file_mc_sigma[1]->Get("h_met_perp_vs_zpt_sigma");

    _h_ratio_met_para_sigma_dtmc[1] = (TH1D*)_h_dt_met_para_sigma[1]->Clone("h_ratio_met_para_sigma_dtmc_mu");
    _h_ratio_met_perp_sigma_dtmc[1] = (TH1D*)_h_dt_met_perp_sigma[1]->Clone("h_ratio_met_perp_sigma_dtmc_mu");
    _h_ratio_met_para_sigma_dtmc[1]->Divide(_h_mc_met_para_sigma[1]);
    _h_ratio_met_perp_sigma_dtmc[1]->Divide(_h_mc_met_perp_sigma[1]);

    _h_dt_met_para_shift[2] = (TH1D*)_file_dt_sigma[2]->Get("h_met_para_vs_zpt_mean");
    _h_mc_met_para_shift[2] = (TH1D*)_file_mc_sigma[2]->Get("h_met_para_vs_zpt_mean");
    _h_met_para_shift_dtmc[2] = (TH1D*)_h_dt_met_para_shift[2]->Clone("h_met_para_shift_dtmc_el");
    _h_met_para_shift_dtmc[2]->Add(_h_mc_met_para_shift[2], -1);

    _h_dt_met_para_sigma[2] = (TH1D*)_file_dt_sigma[2]->Get("h_met_para_vs_zpt_sigma");
    _h_dt_met_perp_sigma[2] = (TH1D*)_file_dt_sigma[2]->Get("h_met_perp_vs_zpt_sigma");
    _h_mc_met_para_sigma[2] = (TH1D*)_file_mc_sigma[2]->Get("h_met_para_vs_zpt_sigma");
    _h_mc_met_perp_sigma[2] = (TH1D*)_file_mc_sigma[2]->Get("h_met_perp_vs_zpt_sigma");

    _h_ratio_met_para_sigma_dtmc[2] = (TH1D*)_h_dt_met_para_sigma[2]->Clone("h_ratio_met_para_sigma_dtmc_el");
    _h_ratio_met_perp_sigma_dtmc[2] = (TH1D*)_h_dt_met_perp_sigma[2]->Clone("h_ratio_met_perp_sigma_dtmc_el");
    _h_ratio_met_para_sigma_dtmc[2]->Divide(_h_mc_met_para_sigma[2]);
    _h_ratio_met_perp_sigma_dtmc[2]->Divide(_h_mc_met_perp_sigma[2]);

    // smooth functions
    _h_dt_met_para_shift[0]->SetName("h_dt_met_para_shift_all");
    _h_mc_met_para_shift[0]->SetName("h_mc_met_para_shift_all");
    _h_dt_met_para_shift[1]->SetName("h_dt_met_para_shift_mu");
    _h_mc_met_para_shift[1]->SetName("h_mc_met_para_shift_mu");
    _h_dt_met_para_shift[2]->SetName("h_dt_met_para_shift_el");
    _h_mc_met_para_shift[2]->SetName("h_mc_met_para_shift_el");
    _h_dt_met_para_shift[3] = (TH1D*)_h_dt_met_para_shift[0]->Clone("h_dt_met_para_shift_all_smooth");
    _h_mc_met_para_shift[3] = (TH1D*)_h_mc_met_para_shift[0]->Clone("h_mc_met_para_shift_all_smooth");
    _h_dt_met_para_shift[4] = (TH1D*)_h_dt_met_para_shift[1]->Clone("h_dt_met_para_shift_mu_smooth");
    _h_mc_met_para_shift[4] = (TH1D*)_h_mc_met_para_shift[1]->Clone("h_mc_met_para_shift_mu_smooth");
    _h_dt_met_para_shift[5] = (TH1D*)_h_dt_met_para_shift[2]->Clone("h_dt_met_para_shift_el_smooth");
    _h_mc_met_para_shift[5] = (TH1D*)_h_mc_met_para_shift[2]->Clone("h_mc_met_para_shift_el_smooth");
    _h_dt_met_para_shift[3]->Smooth();
    _h_mc_met_para_shift[3]->Smooth();
    _h_dt_met_para_shift[4]->Smooth();
    _h_mc_met_para_shift[4]->Smooth();
    _h_dt_met_para_shift[5]->Smooth();
    _h_mc_met_para_shift[5]->Smooth();

    _gr_dt_met_para_shift[3] = new TGraphErrors(_h_dt_met_para_shift[3]);
    _gr_mc_met_para_shift[3] = new TGraphErrors(_h_mc_met_para_shift[3]);
    _gr_dt_met_para_shift[4] = new TGraphErrors(_h_dt_met_para_shift[4]);
    _gr_mc_met_para_shift[4] = new TGraphErrors(_h_mc_met_para_shift[4]);
    _gr_dt_met_para_shift[5] = new TGraphErrors(_h_dt_met_para_shift[5]);
    _gr_mc_met_para_shift[5] = new TGraphErrors(_h_mc_met_para_shift[5]);

    _h_met_para_shift_dtmc[3] = (TH1D*)_h_met_para_shift_dtmc[0]->Clone("h_met_para_shift_dtmc_all_smooth");
    _h_met_para_shift_dtmc[4] = (TH1D*)_h_met_para_shift_dtmc[1]->Clone("h_met_para_shift_dtmc_mu_smooth");
    _h_met_para_shift_dtmc[5] = (TH1D*)_h_met_para_shift_dtmc[2]->Clone("h_met_para_shift_dtmc_el_smooth");
    _h_met_para_shift_dtmc[3]->Smooth();
    _h_met_para_shift_dtmc[4]->Smooth();
    _h_met_para_shift_dtmc[5]->Smooth();

    _gr_met_para_shift_dtmc[3] = new TGraphErrors(_h_met_para_shift_dtmc[3]);
    _gr_met_para_shift_dtmc[4] = new TGraphErrors(_h_met_para_shift_dtmc[4]);
    _gr_met_para_shift_dtmc[5] = new TGraphErrors(_h_met_para_shift_dtmc[5]);

    _h_ratio_met_para_sigma_dtmc[3] = (TH1D*)_h_ratio_met_para_sigma_dtmc[0]->Clone("h_ratio_met_para_sigma_dtmc_all_smooth");
    _h_ratio_met_para_sigma_dtmc[4] = (TH1D*)_h_ratio_met_para_sigma_dtmc[1]->Clone("h_ratio_met_para_sigma_dtmc_mu_smooth");
    _h_ratio_met_para_sigma_dtmc[5] = (TH1D*)_h_ratio_met_para_sigma_dtmc[2]->Clone("h_ratio_met_para_sigma_dtmc_el_smooth");
    _h_ratio_met_para_sigma_dtmc[3]->Smooth();
    _h_ratio_met_para_sigma_dtmc[4]->Smooth();
    _h_ratio_met_para_sigma_dtmc[5]->Smooth();

    _h_ratio_met_perp_sigma_dtmc[3] = (TH1D*)_h_ratio_met_perp_sigma_dtmc[0]->Clone("h_ratio_met_perp_sigma_dtmc_all_smooth");
    _h_ratio_met_perp_sigma_dtmc[4] = (TH1D*)_h_ratio_met_perp_sigma_dtmc[1]->Clone("h_ratio_met_perp_sigma_dtmc_mu_smooth");
    _h_ratio_met_perp_sigma_dtmc[5] = (TH1D*)_h_ratio_met_perp_sigma_dtmc[2]->Clone("h_ratio_met_perp_sigma_dtmc_el_smooth");
    _h_ratio_met_perp_sigma_dtmc[3]->Smooth();
    _h_ratio_met_perp_sigma_dtmc[4]->Smooth();
    _h_ratio_met_perp_sigma_dtmc[5]->Smooth();

    _gr_ratio_met_para_sigma_dtmc[3] = new TGraphErrors(_h_ratio_met_para_sigma_dtmc[3]);
    _gr_ratio_met_para_sigma_dtmc[4] = new TGraphErrors(_h_ratio_met_para_sigma_dtmc[4]);
    _gr_ratio_met_para_sigma_dtmc[5] = new TGraphErrors(_h_ratio_met_para_sigma_dtmc[5]);
    _gr_ratio_met_perp_sigma_dtmc[3] = new TGraphErrors(_h_ratio_met_perp_sigma_dtmc[3]);
    _gr_ratio_met_perp_sigma_dtmc[4] = new TGraphErrors(_h_ratio_met_perp_sigma_dtmc[4]);
    _gr_ratio_met_perp_sigma_dtmc[5] = new TGraphErrors(_h_ratio_met_perp_sigma_dtmc[5]);
  }
  
  

}

// do simple met recoil fine tuning
void doRecoil()
{
  if ( _doRecoil && ((!_isData && _isDyJets)||(_doGJetsSkim)) ) {
    Float_t met_para = _llnunu_l2_pt*cos(_llnunu_l2_phi-_llnunu_l1_phi);
    Float_t met_perp = _llnunu_l2_pt*sin(_llnunu_l2_phi-_llnunu_l1_phi);
    Float_t met_para_mu = met_para;
    Float_t met_perp_mu = met_perp;
    Float_t met_para_el = met_para;
    Float_t met_perp_el = met_perp;
    if (_doGJetsSkim) {//GJets
      Int_t idd0=0;
      if (_doRecoilUseSmooth) idd0=3;
      for (int idd=0; idd<3; idd++){
        _h_dt_met_para_shift[6+idd] = _h_dt_met_para_shift[idd+idd0];
        _h_mc_met_para_shift[6+idd] = _h_mc_met_para_shift[idd+idd0];
        _h_met_para_shift_dtmc[6+idd] = _h_met_para_shift_dtmc[idd+idd0];
        _h_ratio_met_para_sigma_dtmc[6+idd] = _h_ratio_met_para_sigma_dtmc[idd+idd0];
        _h_ratio_met_perp_sigma_dtmc[6+idd] = _h_ratio_met_perp_sigma_dtmc[idd+idd0];
        _gr_dt_met_para_shift[6+idd] = _gr_dt_met_para_shift[3+idd];
        _gr_mc_met_para_shift[6+idd] = _gr_mc_met_para_shift[3+idd];
        _gr_met_para_shift_dtmc[6+idd] = _gr_met_para_shift_dtmc[3+idd];
        _gr_ratio_met_para_sigma_dtmc[6+idd] = _gr_ratio_met_para_sigma_dtmc[3+idd];
        _gr_ratio_met_perp_sigma_dtmc[6+idd] = _gr_ratio_met_perp_sigma_dtmc[3+idd];
      }
    }
    else if (!_doGJetsSkim&&abs(_llnunu_l1_l1_pdgId)==13&&abs(_llnunu_l1_l2_pdgId)==13) {
      Int_t idd=1;
      if (_doRecoilUseSmooth) idd=4;
      _h_dt_met_para_shift[6] = _h_dt_met_para_shift[idd];
      _h_mc_met_para_shift[6] = _h_mc_met_para_shift[idd];
      _h_met_para_shift_dtmc[6] = _h_met_para_shift_dtmc[idd];
      _h_ratio_met_para_sigma_dtmc[6] = _h_ratio_met_para_sigma_dtmc[idd];
      _h_ratio_met_perp_sigma_dtmc[6] = _h_ratio_met_perp_sigma_dtmc[idd];
      _gr_dt_met_para_shift[6] = _gr_dt_met_para_shift[4];
      _gr_mc_met_para_shift[6] = _gr_mc_met_para_shift[4];
      _gr_met_para_shift_dtmc[6] = _gr_met_para_shift_dtmc[4];
      _gr_ratio_met_para_sigma_dtmc[6] = _gr_ratio_met_para_sigma_dtmc[4];
      _gr_ratio_met_perp_sigma_dtmc[6] = _gr_ratio_met_perp_sigma_dtmc[4];
    }
    else if (!_doGJetsSkim&&abs(_llnunu_l1_l1_pdgId)==11&&abs(_llnunu_l1_l2_pdgId)==11) {
      Int_t idd=2;
      if (_doRecoilUseSmooth) idd=5;
      _h_dt_met_para_shift[6] = _h_dt_met_para_shift[idd];
      _h_mc_met_para_shift[6] = _h_mc_met_para_shift[idd];
      _h_met_para_shift_dtmc[6] = _h_met_para_shift_dtmc[idd];
      _h_ratio_met_para_sigma_dtmc[6] = _h_ratio_met_para_sigma_dtmc[idd];
      _h_ratio_met_perp_sigma_dtmc[6] = _h_ratio_met_perp_sigma_dtmc[idd];
      _gr_dt_met_para_shift[6] = _gr_dt_met_para_shift[5];
      _gr_mc_met_para_shift[6] = _gr_mc_met_para_shift[5];
      _gr_met_para_shift_dtmc[6] = _gr_met_para_shift_dtmc[5];
      _gr_ratio_met_para_sigma_dtmc[6] = _gr_ratio_met_para_sigma_dtmc[5];
      _gr_ratio_met_perp_sigma_dtmc[6] = _gr_ratio_met_perp_sigma_dtmc[5];
    }
    else {
      Int_t idd=0;
      if (_doRecoilUseSmooth) idd=3;
      _h_dt_met_para_shift[6] = _h_dt_met_para_shift[idd];
      _h_mc_met_para_shift[6] = _h_mc_met_para_shift[idd];
      _h_met_para_shift_dtmc[6] = _h_met_para_shift_dtmc[idd];
      _h_ratio_met_para_sigma_dtmc[6] = _h_ratio_met_para_sigma_dtmc[idd];
      _h_ratio_met_perp_sigma_dtmc[6] = _h_ratio_met_perp_sigma_dtmc[idd];
      _gr_dt_met_para_shift[6] = _gr_dt_met_para_shift[3];
      _gr_mc_met_para_shift[6] = _gr_mc_met_para_shift[3];
      _gr_met_para_shift_dtmc[6] = _gr_met_para_shift_dtmc[3];
      _gr_ratio_met_para_sigma_dtmc[6] = _gr_ratio_met_para_sigma_dtmc[3];
      _gr_ratio_met_perp_sigma_dtmc[6] = _gr_ratio_met_perp_sigma_dtmc[3];
    }

    // peak shift
    if (_doRecoilUseSmoothGraph) {
      met_para += _gr_met_para_shift_dtmc[6]->Eval(_llnunu_l1_pt);
      if (_doGJetsSkim) {
        met_para_mu += _gr_met_para_shift_dtmc[7]->Eval(_llnunu_l1_pt);
        met_para_el += _gr_met_para_shift_dtmc[8]->Eval(_llnunu_l1_pt);
      }
    }
    else {
      met_para += _h_met_para_shift_dtmc[6]->GetBinContent(_h_met_para_shift_dtmc[6]->FindBin(_llnunu_l1_pt));
      if (_doGJetsSkim) {
        met_para_mu += _h_met_para_shift_dtmc[7]->GetBinContent(_h_met_para_shift_dtmc[7]->FindBin(_llnunu_l1_pt));
        met_para_el += _h_met_para_shift_dtmc[8]->GetBinContent(_h_met_para_shift_dtmc[8]->FindBin(_llnunu_l1_pt));
      }
    }

    // smearing
    if (_doRecoilUseSmoothGraph) {
      met_para = (met_para-_gr_dt_met_para_shift[6]->Eval(_llnunu_l1_pt))*_gr_ratio_met_para_sigma_dtmc[6]->Eval(_llnunu_l1_pt) + _gr_dt_met_para_shift[6]->Eval(_llnunu_l1_pt);
      met_perp *= _gr_ratio_met_perp_sigma_dtmc[6]->Eval(_llnunu_l1_pt);
      if (_doGJetsSkim) {
        met_para_mu = (met_para_mu-_gr_dt_met_para_shift[7]->Eval(_llnunu_l1_pt))*_gr_ratio_met_para_sigma_dtmc[7]->Eval(_llnunu_l1_pt) + _gr_dt_met_para_shift[7]->Eval(_llnunu_l1_pt);
        met_perp_mu *= _gr_ratio_met_perp_sigma_dtmc[7]->Eval(_llnunu_l1_pt);
        met_para_el = (met_para_el-_gr_dt_met_para_shift[8]->Eval(_llnunu_l1_pt))*_gr_ratio_met_para_sigma_dtmc[8]->Eval(_llnunu_l1_pt) + _gr_dt_met_para_shift[8]->Eval(_llnunu_l1_pt);
        met_perp_el *= _gr_ratio_met_perp_sigma_dtmc[8]->Eval(_llnunu_l1_pt);

      }
    } 
    else {
      met_para = (met_para-_h_dt_met_para_shift[6]->GetBinContent(_h_dt_met_para_shift[6]->FindBin(_llnunu_l1_pt)))
               * _h_ratio_met_para_sigma_dtmc[6]->GetBinContent(_h_ratio_met_para_sigma_dtmc[6]->FindBin(_llnunu_l1_pt)) 
               + _h_dt_met_para_shift[6]->GetBinContent(_h_dt_met_para_shift[6]->FindBin(_llnunu_l1_pt));
      met_perp *= _h_ratio_met_perp_sigma_dtmc[6]->GetBinContent(_h_ratio_met_perp_sigma_dtmc[6]->FindBin(_llnunu_l1_pt));
      if (_doGJetsSkim) {
        met_para_mu = (met_para_mu-_h_dt_met_para_shift[7]->GetBinContent(_h_dt_met_para_shift[7]->FindBin(_llnunu_l1_pt)))
                    * _h_ratio_met_para_sigma_dtmc[7]->GetBinContent(_h_ratio_met_para_sigma_dtmc[7]->FindBin(_llnunu_l1_pt))
                    + _h_dt_met_para_shift[7]->GetBinContent(_h_dt_met_para_shift[7]->FindBin(_llnunu_l1_pt));
        met_perp_mu *= _h_ratio_met_perp_sigma_dtmc[7]->GetBinContent(_h_ratio_met_perp_sigma_dtmc[7]->FindBin(_llnunu_l1_pt));
        met_para_el = (met_para_el-_h_dt_met_para_shift[8]->GetBinContent(_h_dt_met_para_shift[8]->FindBin(_llnunu_l1_pt)))
                    * _h_ratio_met_para_sigma_dtmc[8]->GetBinContent(_h_ratio_met_para_sigma_dtmc[8]->FindBin(_llnunu_l1_pt))
                    + _h_dt_met_para_shift[8]->GetBinContent(_h_dt_met_para_shift[8]->FindBin(_llnunu_l1_pt));
        met_perp_el *= _h_ratio_met_perp_sigma_dtmc[8]->GetBinContent(_h_ratio_met_perp_sigma_dtmc[8]->FindBin(_llnunu_l1_pt));
      }
    }

    // recalculate vars
    Float_t met_px = met_para*cos(_llnunu_l1_phi)-met_perp*sin(_llnunu_l1_phi);
    Float_t met_py = met_para*sin(_llnunu_l1_phi)+met_perp*cos(_llnunu_l1_phi);
    TVector2 vec_met;
    vec_met.Set(met_px, met_py);
    _llnunu_l2_pt = vec_met.Mod();
    _llnunu_l2_phi = TVector2::Phi_mpi_pi(vec_met.Phi());
    
    Float_t et1 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l1_pt*_llnunu_l1_pt);
    Float_t et2 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l2_pt*_llnunu_l2_pt);
    _llnunu_mt = TMath::Sqrt(2.0*_llnunu_l1_mass*_llnunu_l1_mass + 2.0* (et1*et2 - _llnunu_l1_pt*cos(_llnunu_l1_phi)*met_px - _llnunu_l1_pt*sin(_llnunu_l1_phi)*met_py));
    if (_doGJetsSkim){
      //mu
      met_px = met_para_mu*cos(_llnunu_l1_phi)-met_perp_mu*sin(_llnunu_l1_phi);
      met_py = met_para_mu*sin(_llnunu_l1_phi)+met_perp_mu*cos(_llnunu_l1_phi);
      vec_met.Set(met_px, met_py);
      _llnunu_l2_pt_mu = vec_met.Mod();
      _llnunu_l2_phi_mu = TVector2::Phi_mpi_pi(vec_met.Phi());
      et1 = TMath::Sqrt(_llnunu_l1_mass_mu*_llnunu_l1_mass_mu + _llnunu_l1_pt*_llnunu_l1_pt);
      et2 = TMath::Sqrt(_llnunu_l1_mass_mu*_llnunu_l1_mass_mu + _llnunu_l2_pt_mu*_llnunu_l2_pt_mu);
      _llnunu_mt_mu = TMath::Sqrt(2.0*_llnunu_l1_mass_mu*_llnunu_l1_mass_mu + 2.0* (et1*et2 - _llnunu_l1_pt*cos(_llnunu_l1_phi)*met_px - _llnunu_l1_pt*sin(_llnunu_l1_phi)*met_py));
      //el
      met_px = met_para_el*cos(_llnunu_l1_phi)-met_perp_el*sin(_llnunu_l1_phi);
      met_py = met_para_el*sin(_llnunu_l1_phi)+met_perp_el*cos(_llnunu_l1_phi);
      vec_met.Set(met_px, met_py);
      _llnunu_l2_pt_el = vec_met.Mod();
      _llnunu_l2_phi_el = TVector2::Phi_mpi_pi(vec_met.Phi());
      et1 = TMath::Sqrt(_llnunu_l1_mass_el*_llnunu_l1_mass_el + _llnunu_l1_pt*_llnunu_l1_pt);
      et2 = TMath::Sqrt(_llnunu_l1_mass_el*_llnunu_l1_mass_el + _llnunu_l2_pt_el*_llnunu_l2_pt_el);
      _llnunu_mt_el = TMath::Sqrt(2.0*_llnunu_l1_mass_el*_llnunu_l1_mass_el + 2.0* (et1*et2 - _llnunu_l1_pt*cos(_llnunu_l1_phi)*met_px - _llnunu_l1_pt*sin(_llnunu_l1_phi)*met_py));
    }
  }

}



// prepareEffScale
void prepareEffScale()
{

  // needed branches 
  _tree_out->Branch("trgsf", &_trgsf, "trgsf/F");
  _tree_out->Branch("isosf", &_isosf, "isosf/F");
  _tree_out->Branch("idsf", &_idsf, "idsf/F");
  _tree_out->Branch("trksf", &_trksf, "trksf/F");
  _tree_out->Branch("trgsf_err", &_trgsf_err, "trgsf_err/F");
  _tree_out->Branch("isosf_err", &_isosf_err, "isosf_err/F");
  _tree_out->Branch("idsf_err", &_idsf_err, "idsf_err/F");
  _tree_out->Branch("trksf_err", &_trksf_err, "trksf_err/F");
  _tree_out->Branch("trgsf_up", &_trgsf_up, "trgsf_up/F");
  _tree_out->Branch("trgsf_dn", &_trgsf_dn, "trgsf_dn/F");
  _tree_out->Branch("idisotrksf", &_idisotrksf, "idisotrksf/F");
  _tree_out->Branch("idisotrksf_up", &_idisotrksf_up, "idisotrksf_up/F");
  _tree_out->Branch("idisotrksf_dn", &_idisotrksf_dn, "idisotrksf_dn/F");

  // Electron ID ISO scale factors 
  _file_idiso_el = TFile::Open(_EffScaleInputFileName_IdIso_El.c_str());
  _h_sf_idiso_el = (TH2F*)_file_idiso_el->Get("EGamma_SF2D");

  // Electron tracking scale factors
  _file_trksf_el = TFile::Open(_EffScaleInputFileName_Trk_El.c_str());
  _h_sf_trk_el = (TH2F*)_file_trksf_el->Get("EGamma_SF2D");

  // muon tracking scale factors
  _file_trksf_mu = TFile::Open(_EffScaleInputFileName_Trk_Mu.c_str());
  _h_sf_trk_mu = (TH1F*)_file_trksf_mu->Get("hist_ratio_eta");

  // muon id iso scale factors
  _file_idiso_mu = TFile::Open(_EffScaleInputFileName_IdIso_Mu.c_str());
  _h_eff_trkhpt_mu_dt = (TH2F*)_file_idiso_mu->Get("eff_trackHighPt_80Xdata_pteta");
  _h_eff_trkhpt_mu_mc = (TH2F*)_file_idiso_mu->Get("eff_trackHighPt_80Xmc_pteta");
  _h_eff_hpt_mu_dt = (TH2F*)_file_idiso_mu->Get("eff_HighPt_80Xdata_pteta");
  _h_eff_hpt_mu_mc = (TH2F*)_file_idiso_mu->Get("eff_HighPt_80Xmc_pteta");
  _h_sf_iso_mu = (TH2F*)_file_idiso_mu->Get("sf_trackerIso_80X_pteta");

  // electron trigger scale factors
  _file_trg_el = TFile::Open(_EffScaleInputFileName_Trg_El.c_str());
  _h_sf_trg_el_l1=(TH2D*)_file_trg_el->Get("ell1pteta"); 

  // muon trigger scale factors
  _file_trg_mu = TFile::Open(_EffScaleInputFileName_Trg_Mu.c_str());
  _h_eff_trg_mu_l1_tot = (TH2D*)_file_trg_mu->Get("htrg_l1_tot");
  _h_eff_trg_mu_l2_tot = (TH2D*)_file_trg_mu->Get("htrg_l2_tot");
  _h_eff_trg_mu_l1_l1p = (TH2D*)_file_trg_mu->Get("htrg_l1_l1p");
  _h_eff_trg_mu_l2_l1p = (TH2D*)_file_trg_mu->Get("htrg_l2_l1p");
  _h_eff_trg_mu_l1_l1f = (TH2D*)_file_trg_mu->Get("htrg_l1_l1f");
  _h_eff_trg_mu_l2_l1f = (TH2D*)_file_trg_mu->Get("htrg_l2_l1f");
  _h_eff_trg_mu_l1_l1pl2f = (TH2D*)_file_trg_mu->Get("htrg_l1_l1pl2f");
  _h_eff_trg_mu_l1_l1pl2p = (TH2D*)_file_trg_mu->Get("htrg_l1_l1pl2p");
  _h_eff_trg_mu_l1_l1fl2p = (TH2D*)_file_trg_mu->Get("htrg_l1_l1fl2p");
  _h_eff_trg_mu_l2_l1pl2f = (TH2D*)_file_trg_mu->Get("htrg_l2_l1pl2f");
  _h_eff_trg_mu_l2_l1pl2p = (TH2D*)_file_trg_mu->Get("htrg_l2_l1pl2p");
  _h_eff_trg_mu_l2_l1fl2p = (TH2D*)_file_trg_mu->Get("htrg_l2_l1fl2p");

  _NPtBins_eff_trg_mu = _h_eff_trg_mu_l2_tot->GetNbinsX();
  _NEtaBins_eff_trg_mu = _h_eff_trg_mu_l2_tot->GetNbinsY();
  _N_eff_trg_mu_tot = _h_eff_trg_mu_l2_tot->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_tot_err);
  _N_eff_trg_mu_l1p = _h_eff_trg_mu_l2_l1p->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1p_err);
  _N_eff_trg_mu_l1f = _h_eff_trg_mu_l2_l1f->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1f_err);
  _N_eff_trg_mu_l1pl2f = _h_eff_trg_mu_l2_l1pl2f->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1pl2f_err);
  _N_eff_trg_mu_l1pl2p = _h_eff_trg_mu_l2_l1pl2p->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1pl2p_err);
  _N_eff_trg_mu_l1fl2p = _h_eff_trg_mu_l2_l1fl2p->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1fl2p_err);

  _h_eff_trg_mu_l1_tot_norm = (TH2D*)_h_eff_trg_mu_l1_tot->Clone("htrg_l1_tot_norm");
  _h_eff_trg_mu_l2_tot_norm = (TH2D*)_h_eff_trg_mu_l2_tot->Clone("htrg_l2_tot_norm");
  _h_eff_trg_mu_l1_l1p_norm = (TH2D*)_h_eff_trg_mu_l1_l1p->Clone("htrg_l1_l1p_norm");
  _h_eff_trg_mu_l1_l1f_norm = (TH2D*)_h_eff_trg_mu_l1_l1f->Clone("htrg_l1_l1f_norm");
  _h_eff_trg_mu_l2_l1p_norm = (TH2D*)_h_eff_trg_mu_l2_l1p->Clone("htrg_l2_l1p_norm");
  _h_eff_trg_mu_l2_l1f_norm = (TH2D*)_h_eff_trg_mu_l2_l1f->Clone("htrg_l2_l1f_norm");
  _h_eff_trg_mu_l1_l1pl2f_norm = (TH2D*)_h_eff_trg_mu_l1_l1pl2f->Clone("htrg_l1_l1pl2f_norm");
  _h_eff_trg_mu_l1_l1pl2p_norm = (TH2D*)_h_eff_trg_mu_l1_l1pl2p->Clone("htrg_l1_l1pl2p_norm");
  _h_eff_trg_mu_l1_l1fl2p_norm = (TH2D*)_h_eff_trg_mu_l1_l1fl2p->Clone("htrg_l1_l1fl2p_norm");
  _h_eff_trg_mu_l2_l1pl2f_norm = (TH2D*)_h_eff_trg_mu_l2_l1pl2f->Clone("htrg_l2_l1pl2f_norm");
  _h_eff_trg_mu_l2_l1pl2p_norm = (TH2D*)_h_eff_trg_mu_l2_l1pl2p->Clone("htrg_l2_l1pl2p_norm");
  _h_eff_trg_mu_l2_l1fl2p_norm = (TH2D*)_h_eff_trg_mu_l2_l1fl2p->Clone("htrg_l2_l1fl2p_norm");


  _h_eff_trg_mu_l1_tot_norm->Scale(1./_N_eff_trg_mu_tot);
  _h_eff_trg_mu_l2_tot_norm->Scale(1./_N_eff_trg_mu_tot);
  _h_eff_trg_mu_l1_l1p_norm->Scale(1./_N_eff_trg_mu_l1p);
  _h_eff_trg_mu_l1_l1f_norm->Scale(1./_N_eff_trg_mu_l1f);
  _h_eff_trg_mu_l2_l1p_norm->Scale(1./_N_eff_trg_mu_l1p);
  _h_eff_trg_mu_l2_l1f_norm->Scale(1./_N_eff_trg_mu_l1f);
  _h_eff_trg_mu_l1_l1pl2f_norm->Scale(1./_N_eff_trg_mu_l1pl2f);
  _h_eff_trg_mu_l1_l1pl2p_norm->Scale(1./_N_eff_trg_mu_l1pl2p);
  _h_eff_trg_mu_l1_l1fl2p_norm->Scale(1./_N_eff_trg_mu_l1fl2p);
  _h_eff_trg_mu_l2_l1pl2f_norm->Scale(1./_N_eff_trg_mu_l1pl2f);
  _h_eff_trg_mu_l2_l1pl2p_norm->Scale(1./_N_eff_trg_mu_l1pl2p);
  _h_eff_trg_mu_l2_l1fl2p_norm->Scale(1./_N_eff_trg_mu_l1fl2p);

  _h_eff_trg_mu_l1_l1p_norm_vs_tot    = (TH2D*)_h_eff_trg_mu_l1_l1p_norm->Clone("htrg_l1_l1p_norm_vs_tot");
  _h_eff_trg_mu_l1_l1f_norm_vs_tot    = (TH2D*)_h_eff_trg_mu_l1_l1f_norm->Clone("htrg_l1_l1f_norm_vs_tot");
  _h_eff_trg_mu_l2_l1p_norm_vs_tot    = (TH2D*)_h_eff_trg_mu_l2_l1p_norm->Clone("htrg_l2_l1p_norm_vs_tot");
  _h_eff_trg_mu_l2_l1f_norm_vs_tot    = (TH2D*)_h_eff_trg_mu_l2_l1f_norm->Clone("htrg_l2_l1f_norm_vs_tot");
  _h_eff_trg_mu_l1_l1pl2f_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l1_l1pl2f_norm->Clone("htrg_l1_l1pl2f_norm_vs_tot");
  _h_eff_trg_mu_l1_l1pl2p_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l1_l1pl2p_norm->Clone("htrg_l1_l1pl2p_norm_vs_tot");
  _h_eff_trg_mu_l1_l1fl2p_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l1_l1fl2p_norm->Clone("htrg_l1_l1fl2p_norm_vs_tot");
  _h_eff_trg_mu_l2_l1pl2f_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l2_l1pl2f_norm->Clone("htrg_l2_l1pl2f_norm_vs_tot");
  _h_eff_trg_mu_l2_l1pl2p_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l2_l1pl2p_norm->Clone("htrg_l2_l1pl2p_norm_vs_tot");
  _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l2_l1fl2p_norm->Clone("htrg_l2_l1fl2p_norm_vs_tot");
  _h_eff_trg_mu_l1_l1pl2f_norm_vs_l1p = (TH2D*)_h_eff_trg_mu_l1_l1pl2f_norm->Clone("htrg_l1_l1pl2f_norm_vs_l1p");
  _h_eff_trg_mu_l1_l1pl2p_norm_vs_l1p = (TH2D*)_h_eff_trg_mu_l1_l1pl2p_norm->Clone("htrg_l1_l1pl2p_norm_vs_l1p");
  _h_eff_trg_mu_l1_l1fl2p_norm_vs_l1f = (TH2D*)_h_eff_trg_mu_l1_l1fl2p_norm->Clone("htrg_l1_l1fl2p_norm_vs_l1f");
  _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p = (TH2D*)_h_eff_trg_mu_l2_l1pl2f_norm->Clone("htrg_l2_l1pl2f_norm_vs_l1p");
  _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p = (TH2D*)_h_eff_trg_mu_l2_l1pl2p_norm->Clone("htrg_l2_l1pl2p_norm_vs_l1p");
  _h_eff_trg_mu_l2_l1fl2p_norm_vs_l1f = (TH2D*)_h_eff_trg_mu_l2_l1fl2p_norm->Clone("htrg_l2_l1fl2p_norm_vs_l1f");

  _h_eff_trg_mu_l1_l1p_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
  _h_eff_trg_mu_l1_l1f_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
  _h_eff_trg_mu_l2_l1p_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
  _h_eff_trg_mu_l2_l1f_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
  _h_eff_trg_mu_l1_l1pl2f_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
  _h_eff_trg_mu_l1_l1pl2p_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
  _h_eff_trg_mu_l1_l1fl2p_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
  _h_eff_trg_mu_l2_l1pl2f_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
  _h_eff_trg_mu_l2_l1pl2p_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
  _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
  _h_eff_trg_mu_l1_l1pl2f_norm_vs_l1p->Divide(_h_eff_trg_mu_l1_l1p_norm);
  _h_eff_trg_mu_l1_l1pl2p_norm_vs_l1p->Divide(_h_eff_trg_mu_l1_l1p_norm);
  _h_eff_trg_mu_l1_l1fl2p_norm_vs_l1f->Divide(_h_eff_trg_mu_l1_l1f_norm);
  _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p->Divide(_h_eff_trg_mu_l2_l1p_norm);
  _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p->Divide(_h_eff_trg_mu_l2_l1p_norm);
  _h_eff_trg_mu_l2_l1fl2p_norm_vs_l1f->Divide(_h_eff_trg_mu_l2_l1f_norm);

 

}


// add efficiency scale factors
void addEffScale()
{
  // muon 
  if (abs(_llnunu_l1_l1_pdgId)==13 && abs(_llnunu_l1_l2_pdgId)==13) {

    // id 
    double effdt1  = _h_eff_trkhpt_mu_dt->GetBinContent(_h_eff_trkhpt_mu_dt->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
    double effmc1  = _h_eff_trkhpt_mu_mc->GetBinContent(_h_eff_trkhpt_mu_mc->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
    double errdt1  = _h_eff_trkhpt_mu_dt->GetBinError(_h_eff_trkhpt_mu_dt->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
    double errmc1  = _h_eff_trkhpt_mu_mc->GetBinError(_h_eff_trkhpt_mu_mc->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
    double effdt1a = _h_eff_hpt_mu_dt->GetBinContent(_h_eff_hpt_mu_dt->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
    double effmc1a = _h_eff_hpt_mu_mc->GetBinContent(_h_eff_hpt_mu_mc->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
    double errdt1a = _h_eff_hpt_mu_dt->GetBinError(_h_eff_hpt_mu_dt->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
    double errmc1a = _h_eff_hpt_mu_mc->GetBinError(_h_eff_hpt_mu_mc->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
    double effdt2  = _h_eff_trkhpt_mu_dt->GetBinContent(_h_eff_trkhpt_mu_dt->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
    double effmc2  = _h_eff_trkhpt_mu_mc->GetBinContent(_h_eff_trkhpt_mu_mc->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
    double errdt2  = _h_eff_trkhpt_mu_dt->GetBinError(_h_eff_trkhpt_mu_dt->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
    double errmc2  = _h_eff_trkhpt_mu_mc->GetBinError(_h_eff_trkhpt_mu_mc->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
    double effdt2a = _h_eff_hpt_mu_dt->GetBinContent(_h_eff_hpt_mu_dt->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
    double effmc2a = _h_eff_hpt_mu_mc->GetBinContent(_h_eff_hpt_mu_mc->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
    double errdt2a = _h_eff_hpt_mu_dt->GetBinError(_h_eff_hpt_mu_dt->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
    double errmc2a = _h_eff_hpt_mu_mc->GetBinError(_h_eff_hpt_mu_mc->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
    double effdt = effdt1*effdt2a+effdt1a*effdt2-effdt1a*effdt2a;
    double effmc = effmc1*effmc2a+effmc1a*effmc2-effmc1a*effmc2a;
    if(effdt&&effmc){
      _idsf = effdt/effmc;
      _idsf_err = (  TMath::Power((effdt2-effdt2a)*errdt1a,2)
                  + TMath::Power((effdt1-effdt1a)*errdt2a,2)
                  + TMath::Power(effdt1a*errdt2,2)
                  + TMath::Power(effdt2a*errdt1,2) 
                 ) / TMath::Power(effdt,2) 
               + (  TMath::Power((effmc2-effmc2a)*errmc1a,2)
                  + TMath::Power((effmc1-effmc1a)*errmc2a,2)
                  + TMath::Power(effmc1a*errmc2,2)
                  + TMath::Power(effmc2a*errmc1,2)
                 ) / TMath::Power(effmc,2);

      _idsf_err = TMath::Power(_idsf_err,.5)*_idsf;
    }
    else {
      _idsf = 1;
      _idsf_err = 1;
    }


    // iso
    effdt1 = _h_sf_iso_mu->GetBinContent(_h_sf_iso_mu->FindBin(_llnunu_l1_l1_eta,_llnunu_l1_l1_pt));
    effdt2 = _h_sf_iso_mu->GetBinContent(_h_sf_iso_mu->FindBin(_llnunu_l1_l2_eta,_llnunu_l1_l2_pt));
    errdt1 = _h_sf_iso_mu->GetBinError(_h_sf_iso_mu->FindBin(_llnunu_l1_l1_eta,_llnunu_l1_l1_pt));
    errdt2 = _h_sf_iso_mu->GetBinError(_h_sf_iso_mu->FindBin(_llnunu_l1_l2_eta,_llnunu_l1_l2_pt));
    _isosf = effdt1*effdt2;
    _isosf_err = TMath::Power((TMath::Power(effdt1*errdt2,2) + TMath::Power(errdt1*effdt2,2)), .5);

    // tracking
    effdt1 = _h_sf_trk_mu->GetBinContent(_h_sf_trk_mu->FindBin(_llnunu_l1_l1_eta));
    effdt2 = _h_sf_trk_mu->GetBinContent(_h_sf_trk_mu->FindBin(_llnunu_l1_l2_eta));
    errdt1 = _h_sf_trk_mu->GetBinError(_h_sf_trk_mu->FindBin(_llnunu_l1_l1_eta));
    errdt2 = _h_sf_trk_mu->GetBinError(_h_sf_trk_mu->FindBin(_llnunu_l1_l2_eta));
    _trksf = effdt1*effdt2;
    _trksf_err = TMath::Power((TMath::Power(effdt1*errdt2,2) + TMath::Power(errdt1*effdt2,2)), .5);

    // id, iso, tracking combined uncertainty up/down
    double idisotrksf_err = sqrt(_idsf_err*_idsf_err+_isosf_err*_isosf_err+_trksf_err*_trksf_err);
    _idisotrksf = _idsf*_isosf*_trksf;
    _idisotrksf_up = _idisotrksf+0.5*idisotrksf_err;
    _idisotrksf_dn = _idisotrksf-0.5*idisotrksf_err;

    // trigger
    int trg_bin_l1 = _h_eff_trg_mu_l1_l1p_norm_vs_tot->FindBin(_llnunu_l1_l1_pt,fabs(_llnunu_l1_l1_eta));
    int trg_bin_l2 = _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p->FindBin(_llnunu_l1_l2_pt,fabs(_llnunu_l1_l2_eta));
    double trg_sc_l1_l1p_vs_tot = _h_eff_trg_mu_l1_l1p_norm_vs_tot->GetBinContent(trg_bin_l1);
    double trg_sc_l2_l1pl2f_vs_l1p = _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p->GetBinContent(trg_bin_l2);
    double trg_sc_l2_l1pl2p_vs_l1p = _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p->GetBinContent(trg_bin_l2);
    double trg_sc_l2_l1fl2p_vs_tot = _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot->GetBinContent(trg_bin_l2);
    double trg_sc_l1_l1p_vs_tot_err = _h_eff_trg_mu_l1_l1p_norm_vs_tot->GetBinError(trg_bin_l1);
    double trg_sc_l2_l1pl2f_vs_l1p_err = _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p->GetBinError(trg_bin_l2);
    double trg_sc_l2_l1pl2p_vs_l1p_err = _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p->GetBinError(trg_bin_l2);
    double trg_sc_l2_l1fl2p_vs_tot_err = _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot->GetBinError(trg_bin_l2);

    double trg_npass = _N_eff_trg_mu_l1pl2f*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2f_vs_l1p
                     + _N_eff_trg_mu_l1pl2p*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2p_vs_l1p
                     + _N_eff_trg_mu_l1fl2p*trg_sc_l2_l1fl2p_vs_tot
                     ;
    double trg_npass_err = pow(_N_eff_trg_mu_l1pl2f_err*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2f_vs_l1p,2)
                         + pow(_N_eff_trg_mu_l1pl2f*trg_sc_l1_l1p_vs_tot_err*trg_sc_l2_l1pl2f_vs_l1p,2)
                         + pow(_N_eff_trg_mu_l1pl2f*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2f_vs_l1p_err,2)
                         + pow(_N_eff_trg_mu_l1pl2p_err*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2p_vs_l1p,2)
                         + pow(_N_eff_trg_mu_l1pl2p*trg_sc_l1_l1p_vs_tot_err*trg_sc_l2_l1pl2p_vs_l1p,2)
                         + pow(_N_eff_trg_mu_l1pl2p*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2p_vs_l1p_err,2)
                         + pow(_N_eff_trg_mu_l1fl2p_err*trg_sc_l2_l1fl2p_vs_tot,2)
                         + pow(_N_eff_trg_mu_l1fl2p*trg_sc_l2_l1fl2p_vs_tot_err,2)
                         ;
    trg_npass_err = sqrt(trg_npass_err);

    double trg_nfail = _N_eff_trg_mu_tot-trg_npass;
    double trg_nfail_err = sqrt(_N_eff_trg_mu_tot_err*_N_eff_trg_mu_tot_err
                                 - _N_eff_trg_mu_l1pl2f_err*_N_eff_trg_mu_l1pl2f_err
                                 - _N_eff_trg_mu_l1pl2p_err*_N_eff_trg_mu_l1pl2p_err
                                 - _N_eff_trg_mu_l1fl2p_err*_N_eff_trg_mu_l1fl2p_err);

    double trg_eff = trg_npass/(trg_npass+trg_nfail);
    double trg_eff_err = (pow(trg_nfail*trg_npass_err,2)+pow(trg_npass*trg_nfail_err,2))/pow(trg_npass+trg_nfail,4);
    trg_eff_err = sqrt(trg_eff_err);

    double trg_eff_up = trg_eff+0.5*trg_eff_err;
    double trg_eff_dn = trg_eff-0.5*trg_eff_err;

    if (trg_eff>=1) trg_eff=1;
    if (trg_eff<=0) trg_eff=0;
    if (trg_eff_up>=1) trg_eff_up=1;
    if (trg_eff_dn>=1) trg_eff_dn=1;
    if (trg_eff_up<=0) trg_eff_up=0;
    if (trg_eff_dn<=0) trg_eff_dn=0;
    trg_eff_err = fabs(trg_eff_up-trg_eff_dn);

    _trgsf = trg_eff;
    _trgsf_err = trg_eff_err;
    _trgsf_up = trg_eff_up;
    _trgsf_dn = trg_eff_dn;

  }
  // electron
  else if (abs(_llnunu_l1_l1_pdgId)==11 && abs(_llnunu_l1_l2_pdgId)==11) {

    // id
    double effdt1 = 1.0;
    if(_llnunu_l1_l1_pt<=200) {
      effdt1 = _h_sf_idiso_el->GetBinContent(_h_sf_idiso_el->FindBin(_llnunu_l1_l1_eSCeta,_llnunu_l1_l1_pt));
    }
    double effdt2 = 1.0;
    if(_llnunu_l1_l2_pt<=200) {
      effdt2 = _h_sf_idiso_el->GetBinContent(_h_sf_idiso_el->FindBin(_llnunu_l1_l2_eSCeta,_llnunu_l1_l2_pt));
    }
    double errdt1 = _h_sf_idiso_el->GetBinError(_h_sf_idiso_el->FindBin(_llnunu_l1_l1_eSCeta,_llnunu_l1_l1_pt));
    double errdt2 = _h_sf_idiso_el->GetBinError(_h_sf_idiso_el->FindBin(_llnunu_l1_l2_eSCeta,_llnunu_l1_l2_pt));
    _idsf = effdt1*effdt2;
    _idsf_err = TMath::Power((TMath::Power(effdt1*errdt2,2)+TMath::Power(errdt1*effdt2,2)),.5);

    // iso (iso included in id sf)
    _isosf=1;
    _isosf_err=0;

    // track
    effdt1 = _h_sf_trk_el->GetBinContent(_h_sf_trk_el->FindBin(_llnunu_l1_l1_eSCeta,100));
    effdt2 = _h_sf_trk_el->GetBinContent(_h_sf_trk_el->FindBin(_llnunu_l1_l2_eSCeta,100));
    errdt1 = _h_sf_trk_el->GetBinError(_h_sf_trk_el->FindBin(_llnunu_l1_l1_eSCeta,100));
    errdt2 = _h_sf_trk_el->GetBinError(_h_sf_trk_el->FindBin(_llnunu_l1_l2_eSCeta,100));
    _trksf = effdt1*effdt2;
    _trksf_err = TMath::Power((TMath::Power(effdt1*errdt2,2)+TMath::Power(errdt1*effdt2,2)),.5);


    // id, iso, tracking combined uncertainty up/down
    double idisotrksf_err = sqrt(_idsf_err*_idsf_err+_isosf_err*_isosf_err+_trksf_err*_trksf_err);
    _idisotrksf = _idsf*_isosf*_trksf;
    _idisotrksf_up = _idisotrksf+0.5*idisotrksf_err;
    _idisotrksf_dn = _idisotrksf-0.5*idisotrksf_err;

    // trigger
    _trgsf = _h_sf_trg_el_l1->GetBinContent(_h_sf_trg_el_l1->FindBin(_llnunu_l1_l1_pt,fabs(_llnunu_l1_l1_eta)))/100;
    _trgsf_err = _h_sf_trg_el_l1->GetBinError(_h_sf_trg_el_l1->FindBin(_llnunu_l1_l1_pt,fabs(_llnunu_l1_l1_eta)))/100;

    _trgsf_up = _trgsf+0.5*_trgsf_err;
    _trgsf_dn = _trgsf-0.5*_trgsf_err;
    if (_trgsf>=1) _trgsf = 1;
    if (_trgsf<=0) _trgsf = 0;
    if (_trgsf_up>=1) _trgsf_up=1;
    if (_trgsf_up<=0) _trgsf_up=0;
    if (_trgsf_dn>=1) _trgsf_dn=1;
    if (_trgsf_dn<=0) _trgsf_dn=0;

    _trgsf_err = fabs(_trgsf_up-_trgsf_dn);



  }

}



// prepare gjets skimming
void prepareGJetsSkim() 
{
  if (_doGJetsSkim) {
    _gjets_input_file = TFile::Open(_GJetsSkimInputFileName.c_str());
    _gjets_h_zmass_zpt_zrap = (TH3D*)_gjets_input_file->Get("h_zmass_zpt_zrap_lowlpt");
    _gjets_h_zmass_zpt_zrap_el = (TH3D*)_gjets_input_file->Get("h_zmass_zpt_zrap_lowlpt_el");
    _gjets_h_zmass_zpt_zrap_mu = (TH3D*)_gjets_input_file->Get("h_zmass_zpt_zrap_lowlpt_mu");
    _gjets_h_zmass_zpt = (TH2D*)_gjets_input_file->Get("h_zmass_zpt_lowlpt");
    _gjets_h_zmass_zpt_el = (TH2D*)_gjets_input_file->Get("h_zmass_zpt_lowlpt_el");
    _gjets_h_zmass_zpt_mu = (TH2D*)_gjets_input_file->Get("h_zmass_zpt_lowlpt_mu");
    _gjets_h_zpt_ratio = (TH1D*)_gjets_input_file->Get("h_zpt_ratio");
    _gjets_h_zpt_ratio_el = (TH1D*)_gjets_input_file->Get("h_zpt_ratio_el");
    _gjets_h_zpt_ratio_mu = (TH1D*)_gjets_input_file->Get("h_zpt_ratio_mu");
    _gjets_h_zpt_lowlpt_ratio = (TH1D*)_gjets_input_file->Get("h_zpt_lowlpt_ratio");
    _gjets_h_zpt_lowlpt_ratio_el = (TH1D*)_gjets_input_file->Get("h_zpt_lowlpt_ratio_el");
    _gjets_h_zpt_lowlpt_ratio_mu = (TH1D*)_gjets_input_file->Get("h_zpt_lowlpt_ratio_mu");
    _gjets_h_zpt_zrap_ratio = (TH2D*)_gjets_input_file->Get("h_zpt_zrap_ratio");
    _gjets_h_zpt_zrap_ratio_el = (TH2D*)_gjets_input_file->Get("h_zpt_zrap_ratio_el");
    _gjets_h_zpt_zrap_ratio_mu = (TH2D*)_gjets_input_file->Get("h_zpt_zrap_ratio_mu");
    _gjets_h_zpt_zrap_ratio_up = (TH2D*)_gjets_input_file->Get("h_zpt_zrap_ratio_up");
    _gjets_h_zpt_zrap_ratio_el_up = (TH2D*)_gjets_input_file->Get("h_zpt_zrap_ratio_el_up");
    _gjets_h_zpt_zrap_ratio_mu_up = (TH2D*)_gjets_input_file->Get("h_zpt_zrap_ratio_mu_up");
    _gjets_h_zpt_zrap_ratio_dn = (TH2D*)_gjets_input_file->Get("h_zpt_zrap_ratio_dn");
    _gjets_h_zpt_zrap_ratio_el_dn = (TH2D*)_gjets_input_file->Get("h_zpt_zrap_ratio_el_dn");
    _gjets_h_zpt_zrap_ratio_mu_dn = (TH2D*)_gjets_input_file->Get("h_zpt_zrap_ratio_mu_dn");
    _gjets_h_zpt_zrap_lowlpt_ratio = (TH2D*)_gjets_input_file->Get("h_zpt_zrap_lowlpt_ratio");
    _gjets_h_zpt_zrap_lowlpt_ratio_el = (TH2D*)_gjets_input_file->Get("h_zpt_zrap_lowlpt_ratio_el");
    _gjets_h_zpt_zrap_lowlpt_ratio_mu = (TH2D*)_gjets_input_file->Get("h_zpt_zrap_lowlpt_ratio_mu");


    for (int ix=0; ix<(int)_gjets_h_zmass_zpt->GetNbinsX(); ix++) {
      for (int iy=0; iy<(int)_gjets_h_zmass_zpt->GetNbinsY(); iy++) {
        if (_gjets_h_zmass_zpt->GetBinContent(ix+1, iy+1)<0) {
          _gjets_h_zmass_zpt->SetBinContent(ix+1, iy+1, 0.0);
        }
      }
    }

    for (int ix=0; ix<(int)_gjets_h_zmass_zpt_el->GetNbinsX(); ix++) {
      for (int iy=0; iy<(int)_gjets_h_zmass_zpt_el->GetNbinsY(); iy++) {
        if (_gjets_h_zmass_zpt_el->GetBinContent(ix+1, iy+1)<0) {
          _gjets_h_zmass_zpt_el->SetBinContent(ix+1, iy+1, 0.0);
        }
      }
    }

    for (int ix=0; ix<(int)_gjets_h_zmass_zpt_mu->GetNbinsX(); ix++) {
      for (int iy=0; iy<(int)_gjets_h_zmass_zpt_mu->GetNbinsY(); iy++) {
        if (_gjets_h_zmass_zpt_mu->GetBinContent(ix+1, iy+1)<0) {
          _gjets_h_zmass_zpt_mu->SetBinContent(ix+1, iy+1, 0.0);
        }
      }
    }


    for (int ix=0; ix<(int)_gjets_h_zmass_zpt_zrap->GetNbinsX(); ix++) {
      for (int iy=0; iy<(int)_gjets_h_zmass_zpt_zrap->GetNbinsY(); iy++) {
        for (int iz=0; iz<(int)_gjets_h_zmass_zpt_zrap->GetNbinsZ(); iz++) {
          if (_gjets_h_zmass_zpt_zrap->GetBinContent(ix+1, iy+1, iz+1)<0) {
            _gjets_h_zmass_zpt_zrap->SetBinContent(ix+1, iy+1, iz+1, 0.0);
          }    
        }
      }
    }

    for (int ix=0; ix<(int)_gjets_h_zmass_zpt_zrap_el->GetNbinsX(); ix++) {
      for (int iy=0; iy<(int)_gjets_h_zmass_zpt_zrap_el->GetNbinsY(); iy++) {
        for (int iz=0; iz<(int)_gjets_h_zmass_zpt_zrap_el->GetNbinsZ(); iz++) {
          if (_gjets_h_zmass_zpt_zrap_el->GetBinContent(ix+1, iy+1, iz+1)<0) {
            _gjets_h_zmass_zpt_zrap_el->SetBinContent(ix+1, iy+1, iz+1, 0.0);
          }
        }
      }
    }

    for (int ix=0; ix<(int)_gjets_h_zmass_zpt_zrap_mu->GetNbinsX(); ix++) {
      for (int iy=0; iy<(int)_gjets_h_zmass_zpt_zrap_mu->GetNbinsY(); iy++) {
        for (int iz=0; iz<(int)_gjets_h_zmass_zpt_zrap_mu->GetNbinsZ(); iz++) {
          if (_gjets_h_zmass_zpt_zrap_mu->GetBinContent(ix+1, iy+1, iz+1)<0) {
            _gjets_h_zmass_zpt_zrap_mu->SetBinContent(ix+1, iy+1, iz+1, 0.0);
          }
        }
      }
    }

    for (int iy=0; iy<(int)_gjets_h_zmass_zpt->GetNbinsY(); iy++){
      sprintf(name, "h_zmass_zpt_%i", iy+1);
      TH1D* htmp = (TH1D*)_gjets_h_zmass_zpt->ProjectionX(name, iy+1, iy+1, "e");
      _gjets_h_zmass_zpt_1d_vec.push_back(htmp);
    }

    for (int iy=0; iy<(int)_gjets_h_zmass_zpt_el->GetNbinsY(); iy++){
      sprintf(name, "h_zmass_zpt_el_%i", iy+1);
      TH1D* htmp = (TH1D*)_gjets_h_zmass_zpt_el->ProjectionX(name, iy+1, iy+1, "e");
      _gjets_h_zmass_zpt_el_1d_vec.push_back(htmp);
    }

    for (int iy=0; iy<(int)_gjets_h_zmass_zpt_mu->GetNbinsY(); iy++){
      sprintf(name, "h_zmass_zpt_mu_%i", iy+1);
      TH1D* htmp = (TH1D*)_gjets_h_zmass_zpt_mu->ProjectionX(name, iy+1, iy+1, "e");
      _gjets_h_zmass_zpt_mu_1d_vec.push_back(htmp);
    }

    for (int iy=0; iy<(int)_gjets_h_zmass_zpt_zrap->GetNbinsY(); iy++){
      std::vector<TH1D*> h_zmass_zpt_temp;
      for (int iz=0; iz<(int)_gjets_h_zmass_zpt_zrap->GetNbinsZ(); iz++){
        sprintf(name, "h_zmass_zpt%i_zrap%i", iy+1, iz+1);
        TH1D* htmp = (TH1D*)_gjets_h_zmass_zpt_zrap->ProjectionX(name, iy+1, iy+1, iz+1, iz+1, "e");
        h_zmass_zpt_temp.push_back(htmp); 
      }
      _gjets_h_zmass_zpt_zrap_1d_vec.push_back(h_zmass_zpt_temp);
    }
 
    for (int iy=0; iy<(int)_gjets_h_zmass_zpt_zrap_el->GetNbinsY(); iy++){
      std::vector<TH1D*> h_zmass_zpt_el_temp;
      for (int iz=0; iz<(int)_gjets_h_zmass_zpt_zrap_el->GetNbinsZ(); iz++){
        sprintf(name, "h_zmass_zpt%i_zrap_el%i", iy+1, iz+1);
        TH1D* htmp = (TH1D*)_gjets_h_zmass_zpt_zrap_el->ProjectionX(name, iy+1, iy+1, iz+1, iz+1, "e");
        h_zmass_zpt_el_temp.push_back(htmp);
      }
      _gjets_h_zmass_zpt_zrap_el_1d_vec.push_back(h_zmass_zpt_el_temp);
    }

    for (int iy=0; iy<(int)_gjets_h_zmass_zpt_zrap_mu->GetNbinsY(); iy++){
      std::vector<TH1D*> h_zmass_zpt_mu_temp;
      for (int iz=0; iz<(int)_gjets_h_zmass_zpt_zrap_mu->GetNbinsZ(); iz++){
        sprintf(name, "h_zmass_zpt%i_zrap_mu%i", iy+1, iz+1);
        TH1D* htmp = (TH1D*)_gjets_h_zmass_zpt_zrap_mu->ProjectionX(name, iy+1, iy+1, iz+1, iz+1, "e");
        h_zmass_zpt_mu_temp.push_back(htmp);
      }
      _gjets_h_zmass_zpt_zrap_mu_1d_vec.push_back(h_zmass_zpt_mu_temp);
    }

    // photon phi weight
    if (_doGJetsSkimAddPhiWeight) {
      _gjets_phi_weight_input_file = TFile::Open(_GJetsSkimPhiWeightInputFileName.c_str());
      _gjets_h_photon_phi_weight = (TH1D*)_gjets_phi_weight_input_file->Get("h_gjet_phi_weight");
    }    
  }

}


// do gjets skim
void doGJetsSkim()
{

  // copy branches
  _llnunu_mt = _gjet_mt;
  _llnunu_l1_pt = _gjet_l1_pt;
  _llnunu_l1_eta = _gjet_l1_eta;
  _llnunu_l1_rapidity = _gjet_l1_rapidity;
  _llnunu_l1_phi = _gjet_l1_phi;
  _llnunu_l2_pt = _gjet_l2_pt;
  _llnunu_l2_phi = _gjet_l2_phi;
  _llnunu_l2_sumEt = _gjet_l2_sumEt;
  _llnunu_l2_rawPt = _gjet_l2_rawPt;
  _llnunu_l2_rawPhi = _gjet_l2_rawPhi;
  _llnunu_l2_rawSumEt = _gjet_l2_rawSumEt;
  _llnunu_l1_l1_pt = 19801117;
  _llnunu_l1_l1_eta = 0;
  _llnunu_l1_l1_pdgId = 19801117;
  _llnunu_l1_l2_pt = 19801117;
  _llnunu_l1_l2_eta = 0;
  _llnunu_l1_l2_pdgId = 19801117;
  _llnunu_l1_l1_highPtID = 1.0;
  _llnunu_l1_l2_highPtID = 1.0;

  _llnunu_l2_pt_el = _llnunu_l2_pt;
  _llnunu_l2_phi_el = _llnunu_l2_phi;
  _llnunu_l2_pt_mu = _llnunu_l2_pt;
  _llnunu_l2_phi_mu = _llnunu_l2_phi;

  if (!_isData){
    _llnunu_l2_genPhi = _gjet_l2_genPhi;
    _llnunu_l2_genEta = _gjet_l2_genEta;  
  }

  // generate z mass
  // default all, known to be wrong
  int ipt = _gjets_h_zmass_zpt_zrap->GetYaxis()->FindBin(_llnunu_l1_pt) - 1; 
  int irap = _gjets_h_zmass_zpt_zrap->GetZaxis()->FindBin(_llnunu_l1_rapidity) - 1;
  if (ipt<=0) ipt=0;
  if (ipt>=_gjets_h_zmass_zpt_zrap->GetNbinsY()) ipt=_gjets_h_zmass_zpt_zrap->GetNbinsY()-1;
  if (irap<=0) irap=0;
  if (irap>=_gjets_h_zmass_zpt_zrap->GetNbinsZ()) irap=_gjets_h_zmass_zpt_zrap->GetNbinsZ()-1; 
  if (_debug) std::cout << "doGJetsSkim:: begin all zmass random " << std::endl;
  //_llnunu_l1_mass = _gjets_h_zmass_zpt_zrap_1d_vec.at(ipt).at(irap)->GetRandom();
  _llnunu_l1_mass = _gjets_h_zmass_zpt_1d_vec.at(ipt)->GetRandom();
  if (_debug) std::cout << "doGJetsSkim:: end all zmass random " << std::endl;

  // calculate mt
  Float_t et1 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l1_pt*_llnunu_l1_pt);
  Float_t et2 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l2_pt*_llnunu_l2_pt);
  _llnunu_mt = TMath::Sqrt(2.0*_llnunu_l1_mass*_llnunu_l1_mass+2.0*(et1*et2
             -_llnunu_l1_pt*cos(_llnunu_l1_phi)*_llnunu_l2_pt*cos(_llnunu_l2_phi)
             -_llnunu_l1_pt*sin(_llnunu_l1_phi)*_llnunu_l2_pt*sin(_llnunu_l2_phi)));

  //  el
  ipt = _gjets_h_zmass_zpt_zrap_el->GetYaxis()->FindBin(_llnunu_l1_pt) - 1;
  irap = _gjets_h_zmass_zpt_zrap_el->GetZaxis()->FindBin(_llnunu_l1_rapidity) - 1;
  if (ipt<=0) ipt=0;
  if (ipt>=_gjets_h_zmass_zpt_zrap_el->GetNbinsY()) ipt=_gjets_h_zmass_zpt_zrap_el->GetNbinsY()-1;
  if (irap<=0) irap=0;
  if (irap>=_gjets_h_zmass_zpt_zrap_el->GetNbinsZ()) irap=_gjets_h_zmass_zpt_zrap_el->GetNbinsZ()-1;
  if (_debug) std::cout << "doGJetsSkim:: begin el zmass random " << std::endl;
  //_llnunu_l1_mass_el = _gjets_h_zmass_zpt_zrap_el_1d_vec.at(ipt).at(irap)->GetRandom();
  _llnunu_l1_mass_el = _gjets_h_zmass_zpt_el_1d_vec.at(ipt)->GetRandom();
  if (_debug) std::cout << "doGJetsSkim:: end el zmass random " << std::endl;
  
  // calculate mt
  et1 = TMath::Sqrt(_llnunu_l1_mass_el*_llnunu_l1_mass_el + _llnunu_l1_pt*_llnunu_l1_pt);
  et2 = TMath::Sqrt(_llnunu_l1_mass_el*_llnunu_l1_mass_el + _llnunu_l2_pt_el*_llnunu_l2_pt_el);
  _llnunu_mt_el = TMath::Sqrt(2.0*_llnunu_l1_mass_el*_llnunu_l1_mass_el+2.0*(et1*et2
                 -_llnunu_l1_pt*cos(_llnunu_l1_phi)*_llnunu_l2_pt_el*cos(_llnunu_l2_phi_el)
                 -_llnunu_l1_pt*sin(_llnunu_l1_phi)*_llnunu_l2_pt_el*sin(_llnunu_l2_phi_el)));

  //  mu
  ipt = _gjets_h_zmass_zpt_zrap_mu->GetYaxis()->FindBin(_llnunu_l1_pt) - 1;
  irap = _gjets_h_zmass_zpt_zrap_mu->GetZaxis()->FindBin(_llnunu_l1_rapidity) - 1;
  if (ipt<=0) ipt=0;
  if (ipt>=_gjets_h_zmass_zpt_zrap_mu->GetNbinsY()) ipt=_gjets_h_zmass_zpt_zrap_mu->GetNbinsY()-1;
  if (irap<=0) irap=0;
  if (irap>=_gjets_h_zmass_zpt_zrap_mu->GetNbinsZ()) irap=_gjets_h_zmass_zpt_zrap_mu->GetNbinsZ()-1;
  if (_debug) std::cout << "doGJetsSkim:: begin mu zmass random " << std::endl;
  //_llnunu_l1_mass_mu = _gjets_h_zmass_zpt_zrap_mu_1d_vec.at(ipt).at(irap)->GetRandom();
  _llnunu_l1_mass_mu = _gjets_h_zmass_zpt_mu_1d_vec.at(ipt)->GetRandom();
  if (_debug) std::cout << "doGJetsSkim:: end mu zmass random " << std::endl;

  // calculate mt
  et1 = TMath::Sqrt(_llnunu_l1_mass_mu*_llnunu_l1_mass_mu + _llnunu_l1_pt*_llnunu_l1_pt);
  et2 = TMath::Sqrt(_llnunu_l1_mass_mu*_llnunu_l1_mass_mu + _llnunu_l2_pt_mu*_llnunu_l2_pt_mu);
  _llnunu_mt_mu = TMath::Sqrt(2.0*_llnunu_l1_mass_mu*_llnunu_l1_mass_mu+2.0*(et1*et2
                 -_llnunu_l1_pt*cos(_llnunu_l1_phi)*_llnunu_l2_pt_mu*cos(_llnunu_l2_phi_mu)
                 -_llnunu_l1_pt*sin(_llnunu_l1_phi)*_llnunu_l2_pt_mu*sin(_llnunu_l2_phi_mu)));

  // get zpt zrap weight
  ipt = _gjets_h_zpt_zrap_ratio->GetXaxis()->FindBin(_llnunu_l1_pt) ;
  irap = _gjets_h_zpt_zrap_ratio->GetYaxis()->FindBin(_llnunu_l1_rapidity) ;
  _GJetsWeight = _gjets_h_zpt_zrap_ratio->GetBinContent(ipt, irap);
  _GJetsWeightEl = _gjets_h_zpt_zrap_ratio_el->GetBinContent(ipt, irap);
  _GJetsWeightMu = _gjets_h_zpt_zrap_ratio_mu->GetBinContent(ipt, irap);
  _GJetsWeight_up = _gjets_h_zpt_zrap_ratio_up->GetBinContent(ipt, irap);
  _GJetsWeightEl_up = _gjets_h_zpt_zrap_ratio_el_up->GetBinContent(ipt, irap);
  _GJetsWeightMu_up = _gjets_h_zpt_zrap_ratio_mu_up->GetBinContent(ipt, irap);
  _GJetsWeight_dn = _gjets_h_zpt_zrap_ratio_dn->GetBinContent(ipt, irap);
  _GJetsWeightEl_dn = _gjets_h_zpt_zrap_ratio_el_dn->GetBinContent(ipt, irap);
  _GJetsWeightMu_dn = _gjets_h_zpt_zrap_ratio_mu_dn->GetBinContent(ipt, irap);
  _GJetsWeightLowLPt = _gjets_h_zpt_zrap_lowlpt_ratio->GetBinContent(ipt, irap);
  _GJetsWeightLowLPtEl = _gjets_h_zpt_zrap_lowlpt_ratio_el->GetBinContent(ipt, irap);
  _GJetsWeightLowLPtMu = _gjets_h_zpt_zrap_lowlpt_ratio_mu->GetBinContent(ipt, irap);
  _GJetsZPtWeight = _gjets_h_zpt_ratio->GetBinContent(ipt);
  _GJetsZPtWeightEl = _gjets_h_zpt_ratio_el->GetBinContent(ipt);
  _GJetsZPtWeightMu = _gjets_h_zpt_ratio_mu->GetBinContent(ipt);
  _GJetsZPtWeightLowLPt = _gjets_h_zpt_lowlpt_ratio->GetBinContent(ipt);
  _GJetsZPtWeightLowLPtEl = _gjets_h_zpt_lowlpt_ratio_el->GetBinContent(ipt);
  _GJetsZPtWeightLowLPtMu = _gjets_h_zpt_lowlpt_ratio_mu->GetBinContent(ipt);

  // get photon phi weight
  if (_doGJetsSkimAddPhiWeight) {
    _GJetsPhiWeight = _gjets_h_photon_phi_weight->GetBinContent(_gjets_h_photon_phi_weight->FindBin(_llnunu_l1_phi));
  }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ,.  .:;::;@;,;;:,::...@;,@,                                                                                                                 
//  ..,,.,,.,,:.,':,:::.`.;::':                                                                                                                 
//   ,``. .`,;'.,;::::'`     +@                                                                                                                 
//  :.```.``..:;:'::.::`,@@@+;,:                                                                                                                
//  ,`. :````.,.:..,`';,,    .@++                                                                                                               
//  ```.,``.`..`.```    .:@@@@#,``                                                                                                              
//    `,```` :..,.;@@@             +                                                                                                            
//   ,,,`...,:,,:    .+@@'                                                                                                                      
//  `,,,`,``.:::;@        `:;',,,,:',                                                                                                           
//  ,,.```.`:::.':@@@@@@@@@;,  `@'::.,                                                                                                          
//  ,`,````.::,,;:,.`@+#@@@@';`` ;@.,,                                                                                                          
//  ,`...`..,,.,;,:, @ .,,@@@@@@++. ::,:                                                  `                                                     
//  ``..``.`;:`,;::. @ `..':@@@@++''+@',`;                                  `     .        `                                                    
//  .``,....;,:,:,;,`@ `.`'.;+@   @@+';':;.,                            `         `.,  ``.,.     :,                                             
//  `...```.;:,`:,:. @ ```+`;.@     @@@@@@',`,.                    ,.,,.,.    `   `,;.':':.```      ``                                          
//  :,,`.` ,,:..:`. `@ ` .'`:.@      @@#+'''+#```.               `.::::;;'.,,;` ..',,::,.```                                                    
//  :,,.,`.`,;, ;````@ ``:'.:,'       :@++';::,..``,             ``: ``;;''';.;++#,.`  ,.`  `.:'..,`` ``                                        
//  ,.,,,``  ,: '` ` ' ,,:;:,:'         @+++';:,,.``:@                  `,;;;';;::` `      .  `,  `.`:,`..                                      
//  .```  `` ```..`,:,,::;::,,;          @+#+''::,.. ``.        `          .:.    ,    `     ` `. ``..`,.``                                     
//  `   `...``` ..` .,,,,,::.,,           @+##+';:,,..```,       ``     ``..,.`     `       ` `  .` `..`,.`                                     
//  ` .``````   `,`.` ;`,.::..,`           ;@#@@@'#@:,,.```.   .@@.`    `..,,:::#@'`.`,    `  ````..``,`.,.                                     
//  ` ``` ,  :@@ .; ,.;`,`,,`,,`             @'@@@+@@@@,,.```. ,; .;@:``....,:;:;'#@;`  .  .. `.`.```..,`..                                     
//  ,    `,, :,:# ',`,;., ,`,:,`              `@###@@'@@':';,,`  :'#@@@@@,`,,:;'+@':;,` +.`.,.`.....``..` ``                                    
//  ,    `` ;`@'.:' ;`:.,  :,,,.                ,@##@@@#@@+@` `,...`,@#`:;:+,::'+'+#:.` :,,.,,.``,.``.`.` `.                                    
//  .  `,  `.``,.,;.`.`,::@+,,,,                   @@@@@#+;;#@,.:':`` '@@@@#';;''@@,`. ,;;:,`:,.```` ``````.`                                   
//   @.`,  :.``` :,,::``@` :.;,;                      @@##':.,@@@@;.`.....`,:;;++@:.;``,:::,;;.        `. `.                                    
//  `..`, ,,```` , .`:.`@``+:`,@                          @@`@. `..,,,:::::;;+++@@:::`.,,`, . ,,.,;,``      `                                   
//    , .`;`.``. .`, ,,,@. ,,`:'                            @`,@:.,::;;'''++++#@@@,;,`. .  .  ``   .:.;.``                                      
//  ```,:,. ```` .```,,,@.,:,.:@                            :@@@:'';;'''+'+++#@@#@,,,`;        `, `  ` ;``..`                                   
//  .``.,..     ```, .`:@,,,:.:@                            @::;;;'''''''''+++#@@@@,  @;`   `.` ..     `,.`````                                 
//  .` `,.` ` ```.```` `@,,':,;+                              @@@@@@@@@+''++@@@@@@@,  @@ .` .:.`.,`.` ``````` ` `                               
//  .```,`` : ,.``.`.`` @`.:.','                                       @@@@@@@@@@@#'  @'` , `.,`...```   ```                                    
//  .```.`` . ,``.`....`@.`:...;                                       @@@@@@@@@@;@  .:,  ,.`,:`....``` `.                                      
//  ``` .`` ` ,``.````` @:`;.,.@                                      ,,  ````..,:,  `, ,``,.,.,.`...``````                                     
//:#,`` . . ` ,     .`.`@:`.`.,@                                   ..`        `.,,:` , `:. ,.,...`.,,. ```                                      
//':, ``, ,```, `` , `` ';`.,.,#                           .``         ``````..,,:::   ;., ,,;:.``..,; ````                                     
//,,, ` ``.   .` ` ,`,`.,+,,,,:,@@@,;;@:::`             ..`    `` ````````.,.:;';'': ` :, . ';,   :.,:.,`````                                   
//,.,`` `.,.```  ``. :`,,@.:'::,::,..:,:.,,,` `  .,,#@..``       .`    ``...,..,;;``',  ,   @,    :.,.....``                                    
//@#:`.``.,```.``````,`,,@,;,,:,::. :: `.: .:+,`....::,..`        ``````...,,::;;; @@,,`    ;      ',..``,. `                                   
//@@@````,, .``.`.`.`,`,,@::.,,,::..``'@, ',,@,``,:;;::,.`         `..`..,,::::;: @'+`, ,   .     .  .,,`,                                      
//+++``.`.,``,`..`..`;`:,@:..:,::@+'''';::@@,#@+:::;+;;:,.`         ``..,,,:;::,';.# @ # :   .   : `. ..:                                       
// ,@`.``.,` .```` ` ,`',#:.,;:::@@@@@@@@@@@@@@@@#+';,:;,..`        ``..;'::;;:;;''  @@.:.:``     .. .```                                       
//   ,.,`.:. ,``..```:.;:':,`:;:;@@@@@@@@@@@@@@@@@@@@. ;::..`        ``.,@;;;;;;;;++ @@ ,`,.`. .`   ,`.      ,;;@@`                             
//   ,.,.`,,`. `..`  , :,;::,:::':@@@@@@@@@@@@@@@@   `.:;;:,.``      ```,:@;;;;;;;'@. @:@ `:,... ;;` `` ,`  .  ::  :,,..,:::,.                  
//   :.`.,;,,. ``.```:`:.+;,:,::@                `   .,:;;;:,.```     ``.,''';;:;;'#@@ @.:``;,.`;.,;,.   `.` ,   ``:,``  `.:+.`.,,:;'++@,,`     
//   ,...,:,:. `.`` `:,,.@.:..::@            :`` ` ``.;;#:;:,..``      ``.,@'';;;''+##@@ .,,.:.',.;..`.. , .::;@,+,,;'':;, ``  ```...  `',``...,
//   .``..,:,, `,.   :,:.@.,:::;@        `,,.````` `..;#@@::,,.```      ``,;@'''''+++@@:,.:,,`,,:::,. ; , `,@@,';;+#@@@@@+:,,:,,:'';;:;';`.:.   
//   .``.,.,,, .,`. ```..@`,,:::@        ;:,....`````..,:#;:,,.```      ``.,@+''+'+'@, ``:#@ :,.;:;,:@ ,`'@,@@@@@@@@@@@@@@@@@':;';::'@@@@: ;; ``
//   .`` :,:,.``. `` `, `@`:,;:;@         ;:,,...````...,:',,,..``       ``.,@++++#: @@:.'@ ;.,,,#,.@    :'@@@@@@@@@@@@+@@@@@@@@@@@@@@@@@@@@@;+@
//   , .`,.,`` `.`` ``.` @`::::;@         @;::,,.......,,:;',,..`         ``.;@@@+;+@@@,,,;;,,,:@';;.                 `;@@@@@@@@@@@@@@@@@@@@@#@@
//`,,` . .,.````,``  `.``@,,,::;@          @;;:::,,,,,,,,::';,..``         `..@@+#@`@@;:.,'.`,.;+''@                                .@@@@@@@@@@@
//+'@`````::`  `.`.```..`@:.,:::+           @++';;;;;::,,,,:',,.``         ``.,@@@##@@@,.,,`;`@`:+;:                                  `         
//@@@````.:,,```..`. ..``+;`.:::',,;'  `:    @@@@@@@'::,,,,,:'..``          ``.'@''+@@'.;+:;`'';#'#                                             
//     `.::,,   `` .``,``+; .`,:;,,.``..,. `,` @@@@@':,,,,,,:;:,.``          ``.;''@@#`;;'''';;';#                                              
//     `,:::, `  .`...,``:;`.`.:,+@@@@@@@@@';.   ,@@;:,,,,,:::',..````       ``.;+@@';;;;;;;;;;;'`                                              
//    `,..,::`.````...,``.@,,.,.,, `:@@@@@@@@@@@@@@@,:::::,:,:;+:,.``        ``.:@@;::::::::;;;'                                                
//    `,, ,,, ` .`.`...``:',,::,,:                `@@;:,,,,,:::;+:,``         ``:@'::::::::;;';,,,,...:,                                        
//    `....,,  `.```..`.;,+````:.:                   @:::,,,,::;+@;,.`        `..;;::::::::;;;:@@@@@';;:,:;,````  `,,',.`                       
//    `..,:,, ``.```.`.,:,:..,,`:'                    +;::,,,,::;'@'.`       ``..:;::::::::::;@@@@@+;..  `...::.  .`.```  ..  ,.`.',..`         
//    `` ,.,:```````..:;...,,.,``@                    :;;,,,,,::;@;         ```.,,::::::;:;+` `:@@@@@@@@@@@@@@@@@#''@@@@'.:.::,,`,,,..  ``.`` `,
//     ` ``,,.,.    `+,: ..;,:` @;                     '::,,,:'@.          ``...,,:,,:;;:;@                    `,'@@@@@@@@@@@@@@@@@@+@@+'';::,,,
//     ```.:,.,`....`..,`..',.,.@;                    .::,,,:;         `....,,,,,,:,,,,::@                                       `'@@@@@@@@@@@@@
//     ,`.;:,..     .``.``.+, :;,:                    ;::::.       ``...,,,::::;;+:,,,,,,                                                       
//    `. :,;   `,++`.;..``.@.:'.:,                    ;,;`      ``....,,::;;;'+@;::,.,,,,`                                                      
//    @`:` ``.` .` ,;`:,...#`,::`:                   @:       ``....,::;'''+@;;;;::,,...,;                                                      
//    `;,`  , `  , `'`,,+,.;..,:.,                  `       ``....::;;''+@':;;;;;::,,...,,'                                                     
//@@@@`..``,.;;+:.         ;.,;;.;.                      ``....,:;''++@';;;;;;:::::,,,,,...;                                     `,,:'@@@@@;;:;'
//    `.` `'`.,``@@,,,,.#'..   ;,'.  `.,``;@,         ``...,,:;'+++@+:;;;;;;;::::,,,,.........                       .`,;';:;:,,:::,;::;,::,,,::
//    ..`  :` . .@.@@@@@@        `@@@@@@;        `````..,:;''++#@+::;;;;;;;;::::,,,....```....,`             .:++''::@;,:,,,,,,,:.,:::;,:;';;;''
//+:;:```                         `            ````.,,:;'++#@@:,:;;;;;;';;;;:::,,...````````````, `.,;@+;,:,:;,,   ::@+'''::::::::::;;::;;'++@@@
//;@@'`` .,,,,,,,,,,:;',.                  `  ``..:;'+#@@;..::::;;'''''';;;:::.``````` ` ````````` @#@';;'';;:.:  ``:@@'@@@@@@@@@@@@@@@@@@.     
//@@@@.`.@@@@;:.                    `````````.,:'+@@@,`.::::;;;''+'''';::,..````````````     `````` ,@'';###@@@@@@`.:.:+@@@@                    
//    . `,,.,:::,..,,:';.:,````````..`.``.,:;++;::::,::::;;'+++++';:,..``````````````` `       `````` @@@@@@;.  ,.``..;+;@@@@                   
//    :,.                  ....,,.....:@@@@@'@@@@::::;;;'+++#':,.....```````````````.```        ``````.         ,..`@;. `;#@                    
//    ,`;@@@'':;':;;'@@@@:` `...,,.'@     :'+@@@;,:;;'+++':,,.....```       ` ``````.,````      ```````.:        @.@@@#'@;;@                    
//    .`,: :,,          .;+'#+#@@@@      @;:;@:.,:;'+#':,,,,....````          ` ```..,.```       ` ```...,       ;,:,,;@@@ @                    
//    .`., ,:@@@@@@@@@@@##+##@#@++@       @;@@@@,:+#'::,,,....`````           ````...::..``      ` ```....       .,:` `.;'@@                    
//    .`., `,:::``;..`,.`,.,.::';'@      `@;@;@;;'+'::,,,....``````         ` ```...,:#,..`   `` ````....,,       :.`,'`:';@                    
//    .`.:,`::,:``:.``,, :...;,':;@       @';;@;;';;::,,....````````   ``````````..,,:@:..``````````....,,,`      :,..`.,;:+                    
//    ```;` ;:,,`,::....`.,,.;,';;@        ;::';'';::,,,.....`````````````````....,,:;@:...```````.....,,,,:      ,,.,`.,;:'                    
//    ```,,.;::,,:',..,,.`,.,,.':;@.       ;::#:';;::,,,.......``````````````...,,,::@@:,............,,,,,,:      ,...`..:;;                    
//    ```,` ':;,:::,;.;.,,:,:..;:;',       ,::+:'';:,,,..........`````````....,,,:::;@':,,,.........,,,,,,,:.     .,...`,::;                    
//     ` ;` '::,,'::;,,.:::,:..::;:;       +,:';';::,,,,,...........``.......,,:::;;@@';::,,,,.,,.,,,,,,,,,::     ..,,`.,:,;`                   
//     . .` :,`,,':,'::.,::,;.`::,:#       +,:';';::,,,,,,.................,,,::;;'@@@';;:::,,,,,,,,,,,,::::;     ....`..,;:`                   
//     .``. ;  :,;::'::.,;::;.`::,.@       ,,:''';:::,,,,,.,...........,,,,:::;''+@@@@@+'';;:::,,,::::::::::;     ..`@ ..,:::                   
//     .`.. :. ,,;:,'::`:;:::;.:,,,@       ::::;';;:::,,,,,,,,,,,,,,,,,::::;''+@@@@@@@@@@@+'';;;;;:::;:;;;;:;     .,....,.:,;                   
//     ,`.` ,``.,,,:+::,;::,;:.,,``+       ,:,:;';;;;:::,,,,,,,,,,,,:::;'+#@@@@@:@@@@@@@@@@@@@@@++''';'';;;:;     `,..`,,,:,;                   
//     ,``  ; ` .,::'::::,,:'  `;';#       ,:,,;';;;;:::,,,,,,,,,,,,::;;;;;;;;@ @@@@@@':,:::''+#@@+'';;;;;;;'      ,,,.,,.:,+                   
//     .``` '`` ::.:':,:;,::.`,::;;@       ::,:.@'';;;::,,,,,,,,,,,,,:::::::;;'+@@@@@;:,,,,,....,,:::::;;;;;;      :..,,.,,,+                   
//     ,.`  ;.` `:,:'::;;::;.`.:;;:@       ::,,`@'';;::::,,,,,,,,,,,,,::::;:;;;#@@@@;::,,,......,,,,::::;;;;:      ,.` `..,:;                   
//     ,...`+.`  `.:;,,:::::'.`:;;:@       ::,,,:+';;;::,,,,,,,,,,,,,,,:::::::;+@@@+;:,,,.........,,::::;;;;+.,,,,,,, @ ,,.::,:,.` :            
//     ,,..`',...` .:,:;:,::':`,;:,+       .,..:`@+'';:::,,,,,,,,,,,,,,,::::;;;#@@@';::,..........,,:::;;;;+       ,,:`,,..,:`     :            
//``   :,.,`+..` ``,,,:;:.;,;;`.::,'`      ,.+;:.#''';;::,,,,,,..,.,,,,,:::;;;;@@@@';::,.........,,,::::;;;;@;;@@@@,.,.,,,.:,;@+;.`+`:,:,,,``.``
//`,   ,...`'`,``. ..,,;..;,;:,..;;+`      .,;....#++';::,,,,,.......,,,::::;;#.@@@';::,,........,,,,:::::;;:,,,,,...,,,,,,,,,  ` `      ` `   `
// :  .,... '.. `` `.``:`.',;;,..+;';;';;,;:,,,..`@+'';;:,,,,.........,,,:::;;+`@@@+;::,,.........,,,,::::;,,..````.,.,,.,,,:,.,...``.:.,.,,,`,`
//  ,` :``. @``  ` ````;..:,;:,..';;;,... ..,,,.``:#+'';;:,,,.........,,:::;;;@.@@@';::,,.........,,,,::::;````.`.`:.,.,,,,,,:.`... `,..,`.,,...
//...`.,.., @`` ` ``. `:`.,,;;:`,+;;::...``.:,:,..:'++'';::,,,........,,::::;;@ ;@@';::,,.........,,:::::::,,,,:,.,.,.,.,` ,:,.:,,,:,.,::,...``.
// `,`.,`.. @``   ` .` , `..;:;;`,',:.`..`,.:;....:.@+'';::,,,,....,,,,,:::;;':,#@@+;;:,,........,,,:::::;,  `.,,,,,,.,...@`,:';@;;,;;:,::;::,:'
//.`..`,:`` @ `     .``, `.`,`:;:...:`.,.,..,:,@...,:#+'';::,,,,...,,,,,:::;;@..:@@+;::,,.......,,,::::::;.,.`..`.`,,,.,,;..,:.`,,,..`,,..`.,,..
//..,`.,:.. @ `:;.:;;. .,;,.,`.:;,..:``..,...::,.`.: @+''';::,,,..,.,,,,::;;'@` :@@+'::,,,,.....,,,,:::::'. ..,....,.:.:,,`,,;:,;,::,:;,.,,,...,
//..`.,,,;;:@,:`,;.:;:;,;:::+,.,:;.:,..,`.`.,:::.`,;.,@+'';::,,,,,,,,,,:::;;'@``:@@+'::,,,......,,,::::;:'.,,,,`..,,,..:,,.:,;'';:;:'';,;';,:,::
//.,,,,`````@````.` ,.;::,:,''`..;'+'`.,`,.,,;:::.,'.`'#+';;:,,,,,,,,,,,:::;'@`,,@@';::,,.......,,,::::;;:,,,....,,,,,:.:,.,,;,.,,::.:`,,,..,:..
//,.,,...```;,.``.``..;::```;,'.`.';',`,.,..,:::,..+`. @+'';;:,,,,,,,,,,,::;;''.`@@';::,........,,,:::;:;,.,.,`,`,::,,,,,:.,,:..`..@,@.,.`.';,;,
//;,:.::,.:,:;`` `  .`;:...:;..';`'.,::.,::,,:,,,..:.;,,@'';;:,,,,,,,,,,,:::;;@':@#':,,,....`....,,:::::'`...,.`..`,,,,:,,;,:;:`,``,,`,;;'::,:',
//`,.'`,.:,::',,,,,;  :..:,,,,;.',,,,..``. ..:,...,'.,..,@';;;:::,,,,,,,,,:::;#;:@@';,,......`...,,,::::,           :,,::,,,:;,   .. .`` `. ...`
//...``, ....+,.,,::,`.`,,`:;;.,:,':#::@,   .,:,.:.;     `@';;:,,,,,,.,,,,,:::;, @@+::,...````...,,,:::'            :,,,:,,::,,                 
//.,,...: ``:#` .``.;,,.,;;.,`.;,..:,' `,;',:.:.;:,:      ,+;;::,,,,....,,,::::@ ,@#;:,....```...,,::,,+            ;.,,,,.,:::                 
//...`;,; `:.+;:;::..:,:.,.,+;+..`:.'@:.    ..,,``.:       @;;::,,......,,,::::@  @@';,,..`````..,,,,,:@            ;,,,,,,,:,:                 
//::.,.::`.; @.`  .'.,..:`;:+,::' ,#,#         ...:'       @;;:::,.......,,,,,:+  ;@+;:,...```...,,,,,,+            :.,.,,.:,,: `    ``         
//      .,,``#,`..,:' ``, '.,,;,`:;,;':;.::,               ;;;::,...```...,,,,:'`  @@';:,..```....,,,..:            ;,,....:,,:           ````  
//,,.   ,`: `@,``.;,`` '`. `.;';`,:'.; ;.`.`. .`;  `. ..   `';;:,..``  `..,,,,:'    @#;:,.....`..........          ,,.....,,;:,         ````    
//````  ;, ` @:;`: ;`   . ;..;'`'...#,    `  ``    ``       @;;:,.`    `.,,,,::+. `.@@+;:,,........,..``,              ``.,:':'  `              
//      ;:,.`@.:..`; @@@@.`;`@,.'.';@:     `          . @   ';;:,..` ``.,,,,,:;@, ` @@#';:,,,,.....,.```.:     ,::,`. `                         
//      ;`.;.@:@, : :+@+:.@`.@,::,':',  `  .             ` ,::;:,..`    `...,,;;   @ .@#';::,,,..,..``   `.   :.`:`;@@`..,.,,,,.,;.,..,. .'`,,;.
//  ```.::.+:+.@ `;'':.:.@;@,, @.,#,:,.,,,:..` .``.    .    ;:::,,..`   ``...,'`   `.`.@++;::,.....       .. `, `` ` `, ': .  .,`;,  `,`;   ```,
//      :,:,:;.@ ..`.` `...`.##:;,+:;,`           ,:    `` .+,:,,..``   ``...,;, .``...`@+';:,...```      `.`@`   .   .      ::.@`.           ` 
//      ,.: ;'.@:.` ,: ,,.++`..,'.@,,.,.```.`,.`.`:.        ;,,....````  `...,:.`. . .`,.@';:,,.```       `.,   , ,`              `   ` :      `
//@@@@# :`;.`;;@:::::`,,,;;,:,:;,,+:,,,`..``....`:..```.   ::,,..`````  ```...,# ``.,.:;.;@';:,.``        `..,` @ `              .   `; ;`      
//````  :..:..:@,:;,``:,`'::;:;@`''.,,+`` ````,`` .`..@@@@@::,,..`````` ```..,,:@@@,      :+;:,.``        ``..,., , `` `,        `     ` `:     
//`   ` :,`:;`:@ ````,::., :,;;:.'@.,.@````   `.,;,,:,;'''';:,,..`````` ````..,:;`,...  ```@;:,.``        ```...,`#@`.:;@ ```@  .@ .    ``      
//      .,`,;,:@:,:,,:,:.:.,`...:':,:.@`..``. ```.`.#@@@@@@;:,,...``````````...,' `  ````  ';:,..```   ``````..:                 ``.`` .```';   
//,,,',`.:....:@,:,,::,:.: .`,,,+::.:'@            ``,     .;:,,...`....``.`...,:,...`...``:;:,..`` ` `  ```.,,,,.`           `                 
//     `.:``.`,@,,,,:,::`.`,.:,:;.,`';@    .```.```.:.....``@;:,,.........`....,,'`......``,;:,..```` ` ````..,,:.::::::.,.....``.,`..`.``````  
//,`.;`..;`..`,;:...:.,:`.,:,:;;,.:.@;@..   `   `      .``.,@';:,,,,,,,,......,,,;:,.......,':,,.````` ````..,::: `  ```  ` ` .`` `````` . ``.` 
//..    `'``.`,':```+ ,`  '::+;::,,,+;+`      `  .``       .`@';::::,,,,.,....,,::,`,`..``..+:,,...````````.,::::`` `  `            ` .`..`.:`  
// .`  ``:;`.`.;:``.,`, .`+;:;:::.:,';' `..` ` ``..,`.` ```...';::,,,,,,,,,.,,,,:;;...``.   @;,,.....``..`.,::::,`.``. ` `.,:,`..`,,;'@@;:,@':,`
//       .;;: ,';`.`. . .,;::;;:.`.,':'`  ``;@@@@:,,.,.``.....@'::,,....,,,,,,,,:;,    `  ` :':,,.........,::::;`` `.,;;:.,`                    
//`     ``;';:`;:`.`,`: ::::;;:;.,.,';;:;@@@,.` ,:,,`,...`` `` @;:,,.....,,,:::::':, `,@@@@@@';:,,,.....,::::::#                                
//`      ,:,,:.::` `. ;,,;,,:;:;..,:';;.        ``:.`.:.`.,`.. :+::,......,,,,:;;+`          @'';::,,,,,:::,,::@                         `   `  
//     `.,:;,,',;`..`.',:,``.::,...;'';' @@@@@@':+'.  `         @';,,......,,,:::'`           +;;;:;:::::,:,,::' :`#.,, ` ` `:.                 
//@@@'@::..,``;,'.::;:':,,`@@;::,,,;:;:;.:``                     @;:,,......,,,,:+  .   ` `:;,@;::::::,,,,,,,:'                                 
//,,,.. `.`..`;.::::::',.,.`.,:,,.:;::;'     ,@+@          `     :+;:,,.....,,,,:@  ` ` `     :;:,,,....,,,,,:#  `                              
//.:::,:;.`.``,.;,:;,:':..,...,...:',:;' ,    ;: .,@@@@@          @';:,,...,.,,:'.       ``    @::,,....,,,,:;:            .``'@@':@`       .::#
//       , .` ,.::,:,:;:.`..`.,..,:;,.+:   ` @`  .',         `.    @';:,,.,.,,,:@ .;  , .``  . ,;:,,.....,,,:;@@@@';,+@@@;:':@@'.:;;++;@@@:,@   
//:   .@ ` .` ,+:.::,:;,,`,.`,,.`,:;``;:        .@         .       :+'::,,,.,,,;@     ,+. ;:@#+:+;:,,....,,:;;':+:@@:@@,.,@'@@@;                
//` @   `.``` `'.:,.,:':` ,` ,,`.::'``:,    :    ',`:@       ``     @;;:,,,,.,,;#   `@.#@+:#:'+@;;:,,....,::;@@+                `.` `     .``@+`
//:@@,:  `..  `# :,:::;`` ,``,.`,:::``:.  ,  `@ +,@::#@@,       @:@@,';:,,,,.,,;:+;@;@@'@;@@@@@@'':,,,,..,::'      ``   ,     `.  ` `` .  .   ` 
// @:@ ,````  `+ .`,,:'`` :``,``,:::..',@  @@@@@#@@.@'@.@;@@,@@+,@,,'@;:,,,,,.,::`  `           +;::,....,:;: ``          `` . `    `` ;, `     
//@'@@@@@.``  .' .....,```,``,..:;;;.:+,;@@` @@@@@@@@@,.              @;,,....,::'      `` ..,..';;:,,...,:'.` ,.`          `.  `. :  .,  `     
//`@@#@@@,``` .+ .```., ` ,``:`..;::,:;,:            . ,       ` ````..':,,..`.,:+`.`,`` .,`  ``,';::,,..,,' `. ,`      `` ,   #```  `   ``  `  
//;`@@.@@:    .# `````.`.`,``;`.`;:;,,',:    ``.`;.,`,``.,`.:,,``   .  @;:,.```,:'...,.;```.:`  .';;:,,.`,,# ``,` ,..   ,`         ``   `@@@@@@@
//    `  . ```.' .`.``.:.`:`.:`..;;;.::,'    `    .` .     . .: .`,::..,'::,.`..,'` ,. `';.. `;@:;;::,..`,:;``: ``    ,@@@@':@.````.`..;;,`     
// ,@  @ . ````' :....,..`,.,,`.:::;:';:;    `` ` ``  ':@@'@+::;@@;;    +;:,,,` .:;  `  `   .`;+;;;;::,.``:.   ` , ` `    . `,. . ` `    ``  `  
//      @:`,```',,,,...,,.,`,,`.;;:;;;+,'    :.    :    .;#  .;'.,;'@# ,@:;:.,,  :'@@:'`@@@@@+#@;;;;::,.``;' .`.`:@'., `,:,@@@'` ```  `.   . :``
//@++@@@@@`.` .:.``...`,,`,.,.,;,';::::,,@ . `,', :.;`  .`    , ,,  ';@@+,:'..    ,   `:@@'@:;+@:;::;;,.``':..'+@@,:;;,.                 `  `  `
//';@;@@''`.  `:. ....`..`:`,,`,,;;;;;::: . .````  .'..,.@@';@'@@@@@@@@+@:,,:;      @@@+@#@@@@'+,::,:;:.  ,, ``. `  .`  `` ` `         `` ` ` , 
//+#'''#;;`.`  ;` .``.....,.,...,';;':;',,`   ``.`, `: ``:.`:@@@+@;@@+`:,;,,,         ,`.  `,  ,:,`.,::.`  '      `   .` ` .``.,..   ```  `    .
//':,@@@@@ ,`  ;` .`,````.` ,...,;:::;''`.`     ` .@;.` .  `;            @:,``           `  `.,.',`.,;@     `.:``..,`  ``  ,   ` ``` . ``     ` 
//    '@;'`.`  '.`.......:`..`..,,:;;;+#.:       `.`,`:::@@@. `      ``` `',``          `````` :@:,.;        `   ..` `., ,`, `.`   `. ., ` , :,.
//        `.`  ', `.`...`;...`..,.;:':+@`,.  `. ` ` . `,`.,    ,'@@@@.,   `..``   `   `,  ```  @;:,, `        `.`` .   `  .            ```.```:`
//@@    :@ ``  ;,``..:.``;``..`...,:::;;';@.,.` `.,`,``, . ,``` `    ```:@@,.```        ,`,`.`.:;;`` `         .`` ` .     ` .     ,  ` .`.  .  
//.`.. `. .`` `;:`.:,:.``;``.`...`,.:,.,:.  `..,,,,``.`..,..```. ``,.`.,:@@:.....      `,.`   ':;```.          ..        ` `        `  `.  ``,  
//. ````. :  `.:;,.,.,```;``.`,.,`,`,;;.`,`....;.:;`,....;...,...,``.. ` ` `,,.`.`     `,..,;;+#``..`     ``, ,.  `` `,.``` . .`` . .`,`` ` ` .,
//  ` `.:.; ``,:;,,..;```; ```.@''+',..':; :., ;`: ..,. .,.,`` .,`.:`,.:``,,,,`@@`.    @ ,`` :;,`....`'. ` ```:... `` ,  `.` ```..`.. ,`....`   
//   ,.`.`;`.`.,',:,`:`. . ``;:`` ,.;;..`:.,`.,.. `.:.,`.,`...., , ;,,````, :, @@@:`   @..,``+.,,.,,+.,`.`  `,` ` ``..,. ,`.` .  .````  ` `     
//        :`,.:,',,.,',';#,:';.`.:;. +..`:` ,.:  .,,...`. `:;,.,  ,`..:`.`, :. @@@@`.  @ `,. ',;`.,.@@`...` . `.```.,...``,,. . ```  `. ,`,  .  
//   ``   ;. `` '.`````.,',:;: ::..;: ,;`@. .` `: :+ ;,,,:,.,`,,, .`. ,: ,  .,.;@@@@   @ `.,;::,,:;@@@@..`  ` `,`, .`.````; `.`` `. .    ``     
//```,` ..;.`,.`::`.``.``,,;:, ;``@. ,.`;; .`..'::` ;.,`.:````  ```. `.` .`,`,,,@@@@:` @ ,`.,:;,,,`@@@@@ `  `.,`.`..``.` ``  ... `   ` ``.``   `
//`  . . `.' ```,;,,::,.` ..,`., ::``., ;' ,`': ,. `  `  ` ` `  `` `    `  ` `, @#@@@   .  . ;:::,,@@@@@ . .```.``  ` ```` ````` ``` `..`   ``  
//. `  ` `.;`.  ,@.',.,,,.  ` ,, ;:``.,,,;.` ..``.  `````. `...``,``..`.  `. .. @@+@@@  @ ` .,,.,`@@@@@@   ..`.. ``,. ``,`;`   `` .` , `` ` `  `
// ``  ..` :`:` `@.':',,,:,: `., ;;,`,`.``,......``. ```,``  `  `,``...`  ` `,. @##@@@` @@  `:',`,@@@@@@.  ,````.`.,,..` `..   ` ``  ``  ` `  ,`
// . ```, ` ,,` ,@,,`.,   .:,::`,:,;. ,..:::..`.`..` ``  . ```    .``` ,,: ``,, @''+#@@  @`  :;,`@@@@@@@,  ,...,.,`    ` ` ` ..  . ` `` .` `  ``
//  ;: `. `::.,.+ ;,.. ` `` :  ,;.`:+,,,.,.,.,::,:,.`  ```   ;` . `` .., ,...`, @'#+#+@  @@  :+.@@@@@@@@;  :..`.`` `  .``  .::   , ``  ,.```,`.`
//`.`...,.,#,,, ,@,,   ',`.`@`@,`.  .``.,`,,,....,,.``.,,,`  .    .``...`.`:`., @#@@@#@@@@@@ :;@@@@@@@@@@  ,,,.. ;. ``    `,, :.`. .  :  `  ` . 
//....` ...`,`.`,. ;#: ,  ,.```,`   `.``.`:@;,...``..`.:  `.,   `     .```, `:,:@'@@@@@@@@@@@@:;.  .. .,,  .. ,.``  .`. `,  `` ``  ` .  . .  ,.,
//   ::,::'`` ..':. ,,    ,,,   .`` ,,,@#;       ` ` `..,,;#,.:.;    .', ..`,`:,+@@@@@@@@@@@#:,':..,`,. @@@  .      `` ``    ` .     .`` `` `   
//  ` `   ,+::.,;@+'#,..` :,...,,                 ``.........  `        `,+,. .  .   ` ,,.,`,   .  ,;.  ` .  .   ,   .   .@.  @ .:  .`. .   ,`. 
//
