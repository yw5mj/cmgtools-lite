{

std::string  name_file1 = "recoil_out/DYJetsToLL_M50_NoRecoil_met_para_study_effSf_mu.root";
std::string  name_file2 = "recoil_out/DYJetsToLL_M50_NoRecoil_met_para_study_mu.root";



TFile* _file_dt_sigma[10];
TFile* _file_mc_sigma[10];
TH1D* _h_dt_met_para_shift[10];
TH1D* _h_mc_met_para_shift[10];
TH1D* _h_met_para_shift_dtmc[10];
TH1D* _h_dt_met_para_sigma[10];
TH1D* _h_dt_met_perp_sigma[10];
TH1D* _h_mc_met_para_sigma[10];
TH1D* _h_mc_met_perp_sigma[10];
TH1D* _h_ratio_met_para_sigma_dtmc[10];
TH1D* _h_ratio_met_perp_sigma_dtmc[10];
TGraphErrors* _gr_dt_met_para_shift[10];
TGraphErrors* _gr_mc_met_para_shift[10];
TGraphErrors* _gr_met_para_shift_dtmc[10];
TGraphErrors* _gr_ratio_met_para_sigma_dtmc[10];
TGraphErrors* _gr_ratio_met_perp_sigma_dtmc[10];


    _file_dt_sigma[0] = new TFile(name_file1.c_str());
    _file_mc_sigma[0] = new TFile(name_file2.c_str());
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

    // smooth functions
    _h_dt_met_para_shift[0]->SetName("h_dt_met_para_shift_all");
    _h_mc_met_para_shift[0]->SetName("h_mc_met_para_shift_all");
    _h_dt_met_para_shift[3] = (TH1D*)_h_dt_met_para_shift[0]->Clone("h_dt_met_para_shift_all_smooth");
    _h_mc_met_para_shift[3] = (TH1D*)_h_mc_met_para_shift[0]->Clone("h_mc_met_para_shift_all_smooth");
    _h_dt_met_para_shift[3]->Smooth();
    _h_mc_met_para_shift[3]->Smooth();

    _gr_dt_met_para_shift[3] = new TGraphErrors(_h_dt_met_para_shift[3]);
    _gr_mc_met_para_shift[3] = new TGraphErrors(_h_mc_met_para_shift[3]);

    _h_met_para_shift_dtmc[3] = (TH1D*)_h_met_para_shift_dtmc[0]->Clone("h_met_para_shift_dtmc_all_smooth");
    _h_met_para_shift_dtmc[3]->Smooth();

    _gr_met_para_shift_dtmc[3] = new TGraphErrors(_h_met_para_shift_dtmc[3]);

    _h_ratio_met_para_sigma_dtmc[3] = (TH1D*)_h_ratio_met_para_sigma_dtmc[0]->Clone("h_ratio_met_para_sigma_dtmc_all_smooth");
    _h_ratio_met_para_sigma_dtmc[3]->Smooth();

    _h_ratio_met_perp_sigma_dtmc[3] = (TH1D*)_h_ratio_met_perp_sigma_dtmc[0]->Clone("h_ratio_met_perp_sigma_dtmc_all_smooth");
    _h_ratio_met_perp_sigma_dtmc[3]->Smooth();

    _gr_ratio_met_para_sigma_dtmc[3] = new TGraphErrors(_h_ratio_met_para_sigma_dtmc[3]);
    _gr_ratio_met_perp_sigma_dtmc[3] = new TGraphErrors(_h_ratio_met_perp_sigma_dtmc[3]);



}

