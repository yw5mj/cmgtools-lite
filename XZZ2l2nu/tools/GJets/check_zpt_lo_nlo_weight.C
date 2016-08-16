{


TFile* file1 = TFile::Open("DYJetsToLL_M50_SkimV3.root");
TFile* file2 = TFile::Open("DYJetsToLL_M50_MGMLM_SkimV3.root");


TTree* tree1 = (TTree*)file1->Get("tree");
TTree* tree2 = (TTree*)file2->Get("tree");

Double_t ZPtBins[] = {0,1.25,2.5,3.75,5,6.25,7.5,8.75,10,11.25,12.5,15,17.5,20,25,30,35,40,45,50,60,70,80,90,100,110,130,150,170,190,220,250,300,500,1000};
Int_t NZPtBins = sizeof(ZPtBins)/sizeof(ZPtBins[0]) - 1;

TH1D* hzpt1 = new TH1D("hzpt1", "hzpg1", NZPtBins, ZPtBins);
TH1D* hzpt2 = new TH1D("hzpt2", "hzpg2", NZPtBins, ZPtBins);

//TH1D* hzpt1 = new TH1D("hzpt1", "hzpg1", 100, 0, 1500);
//TH1D* hzpt2 = new TH1D("hzpt2", "hzpg2", 100, 0, 1500);

hzpt1->Sumw2();
hzpt2->Sumw2();


tree1->Draw("llnunu_l1_pt>>hzpt1", "(1)*(genWeight*ZPtWeight)");
tree2->Draw("llnunu_l1_pt>>hzpt2", "(1)*(genWeight*ZPtWeight)");

hzpt1->SetLineColor(2);
hzpt2->SetLineColor(4);

hzpt1->Scale(1./hzpt1->Integral());
hzpt2->Scale(1./hzpt2->Integral());

hzpt1->Draw();
hzpt2->Draw("same");


TH1D* hzptr12 = (TH1D*)hzpt1->Clone("hzptr12");
hzptr12->Divide(hzpt2);

TH1D* hzeta1 = new TH1D("hzeta1", "hzeta1", 100, -10, 10);
TH1D* hzeta2 = new TH1D("hzeta2", "hzeta2", 100, -10, 10);

hzeta1->Sumw2();
hzeta2->Sumw2();


tree1->Draw("llnunu_l1_eta>>hzeta1", "(1)*(genWeight*ZPtWeight)");
tree2->Draw("llnunu_l1_eta>>hzeta2", "(1)*(genWeight*ZPtWeight)");

hzeta1->SetLineColor(2);
hzeta2->SetLineColor(4);


hzeta1->Scale(1./hzeta1->Integral());
hzeta2->Scale(1./hzeta2->Integral());

hzeta1->Draw();
hzeta2->Draw("same");


TH1D* hzmass1 = new TH1D("hzmass1", "hzmass1", 100, -10, 10);
TH1D* hzmass2 = new TH1D("hzmass2", "hzmass2", 100, -10, 10);

hzmass1->Sumw2();
hzmass2->Sumw2();


tree1->Draw("llnunu_l1_mass>>hzmass1", "(1)*(genWeight*ZPtWeight)");
tree2->Draw("llnunu_l1_mass>>hzmass2", "(1)*(genWeight*ZPtWeight)");

hzmass1->SetLineColor(2);
hzmass2->SetLineColor(4);


hzmass1->Scale(1./hzmass1->Integral());
hzmass2->Scale(1./hzmass2->Integral());

hzmass1->Draw();
hzmass2->Draw("same");



TH1D* hzeta1_wt = new TH1D("hzeta1_wt", "hzeta1_wt", 100, -10, 10);
TH1D* hzeta2_wt = new TH1D("hzeta2_wt", "hzeta2_wt", 100, -10, 10);

hzeta1_wt->Sumw2();
hzeta2_wt->Sumw2();


TFile* fout = new TFile("check_zpt_lo_nlo_weight.root", "recreate");

hzpt1->Write();
hzpt2->Write();
hzptr12->Write();

hzeta1->Write();
hzeta2->Write();
hzmass1->Write();
hzmass2->Write();

fout->Close();


}
