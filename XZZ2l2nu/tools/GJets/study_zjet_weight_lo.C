{
  TFile* fout = new TFile("dyjets_zpt_weight_lo_nlo.root", "recreate");

  
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

  TF1* fczpt2 = new TF1("fczpt2", "[0]-[1]*TMath::Erf((x-[2])/[3])+[4]*TMath::Erf((x-[5])/[6])-[7]*TMath::Erf((x-[8])/[9])",0,3000);;
  fczpt2->SetParameters(1.02852,0.0949640,19.0422,10.4487,0.0758834,56.1146,41.1653, 0.1, 100, 100);
  //fczpt2->SetParameters(-2.978752, 0.131293, 14.500684, 10.332572, 4.097619, -252.264980, 196.546957, 0.258976, 142.987571, 394.031326);

//Par   0                   p0 =   -2.978752 
//Par   1                   p1 =    0.131293 
//Par   2                   p2 =   14.500684 
//Par   3                   p3 =   10.332572 
//Par   4                   p4 =    4.097619 
//Par   5                   p5 =  -252.264980 
//Par   6                   p6 =  196.546957 
//Par   7                   p7 =    0.258976 
//Par   8                   p8 =  142.987571 
//Par   9                   p9 =  394.031326 

  Int_t nfit=100;
  for (int i=0; i<nfit; i++){
    hdyzpt_dtmc_ratio->Fit(fczpt2);
  }

  // print function
  std::cout << "  fczpt2->SetParameters(" ;
  for (int i=0; i<fczpt2->GetNpar(); i++){
    if (i==0) std::cout << fczpt2->GetParameter(i);
    else std::cout << "," << fczpt2->GetParameter(i);
  }
  std::cout << ");" << std::endl;

  char name[3000];
  sprintf(name, "  double weight = %f-%f*TMath::Erf((x-%f)/%f)+%f*TMath::Erf((x-%f)/%f)-%f*TMath::Erf((x-%f)/%f)",
     fczpt2->GetParameter(0),
     fczpt2->GetParameter(1),
     fczpt2->GetParameter(2),
     fczpt2->GetParameter(3),
     fczpt2->GetParameter(4),
     fczpt2->GetParameter(5),
     fczpt2->GetParameter(6),
     fczpt2->GetParameter(7),
     fczpt2->GetParameter(8),
     fczpt2->GetParameter(9)
  );
  std::cout << name << std::endl;
  
  TFile* flotonlo = TFile::Open("study_zpt_new2.root");
  TH1D* hzptnlo = (TH1D*)flotonlo->Get("hzpt1");
  TH1D* hzptlo = (TH1D*)flotonlo->Get("hzpt2");
  TH1D* hzptrnlolo = (TH1D*)flotonlo->Get("hzptr12");
  hzptnlo->SetName("hzptnlo");
  hzptlo->SetName("hzptlo");
  hzptrnlolo->SetName("hzptrnlolo");


  TH1D* hdyzpt_dtmc_lo_ratio = (TH1D*)hdyzpt_dtmc_ratio->Clone("hdyzpt_dtmc_lo_ratio");
  hdyzpt_dtmc_lo_ratio->Multiply(hzptrnlolo);
  hdyzpt_dtmc_lo_ratio->Draw();
 
  TF1* fczpt3 = new TF1("fczpt3", "[0]-[1]*TMath::Erf((x-[2])/[3])+[4]*TMath::Erf((x-[5])/[6])-[7]*TMath::Erf((x-[8])/[9])",0,3000);
  fczpt3->SetParameters(-0.388696,0.128165,14.5515,10.1973,2.0109,-181.141,184.501,0.783707,-275.078,652.422); 
  //hdyzpt_dtmc_lo_ratio->Fit(fczpt3);
  
  //Int_t nfit=2;
  nfit=100;
  for (int i=0; i<nfit; i++){
    hdyzpt_dtmc_lo_ratio->Fit(fczpt3);
  }

  // print function
  std::cout << "  fczpt3->SetParameters(" ;
  for (int i=0; i<fczpt3->GetNpar(); i++){
    if (i==0) std::cout << fczpt3->GetParameter(i);
    else std::cout << "," << fczpt3->GetParameter(i);
  }
  std::cout << ");" << std::endl;

  //char name[3000];
  sprintf(name, "  double weight = %f-%f*TMath::Erf((x-%f)/%f)+%f*TMath::Erf((x-%f)/%f)-%f*TMath::Erf((x-%f)/%f)",
     fczpt3->GetParameter(0),
     fczpt3->GetParameter(1),
     fczpt3->GetParameter(2),
     fczpt3->GetParameter(3),
     fczpt3->GetParameter(4),
     fczpt3->GetParameter(5),
     fczpt3->GetParameter(6),
     fczpt3->GetParameter(7),
     fczpt3->GetParameter(8),
     fczpt3->GetParameter(9)
  );
  std::cout << name << std::endl;


  TH1D* hdyzpt_dtmc_lo_ratio_smooth = (TH1D*)hdyzpt_dtmc_lo_ratio->Clone("hdyzpt_dtmc_lo_ratio_smooth");
  hdyzpt_dtmc_lo_ratio_smooth->Smooth();

  fout->cd();
  hdyzptdt->Write();
  hdyzptmc->Write();
  hdyzpt_dtmc_ratio->Write(); 
  hdyzpt_dtmc_ratio_smooth->Write(); 
  gdyzpt_dtmc_ratio->Write(); 
  fczpt1->Write(); 
  fczpt2->Write("fcdyzpt_dtmc_ratio"); 
 
  hzptnlo->Write();
  hzptlo->Write();
  hzptrnlolo->Write();
  hdyzpt_dtmc_lo_ratio->Write();
  hdyzpt_dtmc_lo_ratio_smooth->Write();
  fczpt3->Write("fcdyzpt_dtmc_lo_ratio"); 
}
