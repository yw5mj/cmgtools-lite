{
  TFile* fout = new TFile("dyjets_zpt_weight.root", "recreate");

  TFile* fdyzpt = new TFile("UnfoldingOutputZPt.root");
  TH1D* hdyzptdt = (TH1D*)fdyzpt->Get("hUnfold");
  TH1D* hdyzptmc = (TH1D*)fdyzpt->Get("hTruth");
  hdyzptdt->SetName("hdyzptdt");
  hdyzptmc->SetName("hdyzptmc");
  TH1D* hdyzpt_dtmc_ratio = (TH1D*)hdyzptdt->Clone("hdyzpt_dtmc_ratio");
  hdyzpt_dtmc_ratio->Divide(hdyzptmc);
  TGraphErrors* gdyzpt_dtmc_ratio = new TGraphErrors(hdyzpt_dtmc_ratio);
  gdyzpt_dtmc_ratio->SetName("gdyzpt_dtmc_ratio");

  TF1* fczpt1 = new TF1("fczpt1", "([0]-[1]*TMath::Erf((x-[2])/[3])+[4]*TMath::Erf((x-[5])/[6]))",0,3000);

  fczpt1->SetParameters(1.02852,0.0949640,19.0422,10.4487,0.0758834,56.1146,41.1653);
 
//(1.02852 - 0.0949640*TMath::Erf((gen_ptll-19.0422)/10.4487) + 0.0758834*Tmath::Erf((gen_ptll- 56.1146)/41.1653)) 

  TH1D* hdyzpt_dtmc_ratio_smooth= (TH1D*)hdyzpt_dtmc_ratio->Clone("hdyzpt_dtmc_ratio_smooth");
  hdyzpt_dtmc_ratio_smooth->Smooth();
  hdyzpt_dtmc_ratio_smooth->SetLineColor(4);
  hdyzpt_dtmc_ratio_smooth->SetMarkerColor(4);

  TF1* fczpt2 = (TF1*)fczpt1->Clone("fczpt2");

  hdyzpt_dtmc_ratio->Fit(fczpt2);
  hdyzpt_dtmc_ratio->Fit(fczpt2);
  hdyzpt_dtmc_ratio->Fit(fczpt2);
  hdyzpt_dtmc_ratio->Fit(fczpt2);
  hdyzpt_dtmc_ratio->Fit(fczpt2);
  hdyzpt_dtmc_ratio->Fit(fczpt2);

  
  //hdyzpt_dtmc_ratio->Draw();
  //fc1->Draw("same");

  fout->cd();
  hdyzptdt->Write();
  hdyzptmc->Write();
  hdyzpt_dtmc_ratio->Write(); 
  hdyzpt_dtmc_ratio_smooth->Write(); 
  gdyzpt_dtmc_ratio->Write(); 
  fczpt1->Write(); 
  fczpt2->Write(); 
}
