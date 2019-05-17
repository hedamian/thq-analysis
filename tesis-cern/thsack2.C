void thsack2(){

TFile* f = TFile::Open("tHq_2lss_mm_0p5_0p25.input.root");
THStack *hs = new THStack("hs","");
TH1D *h1 =(TH1D*)f->Get("x_ttH_hww_CMS_ttHl_btag_cErr2Down");
TH1D *h2 =(TH1D*)f->Get("x_ttH_htt_CMS_ttHl_btag_HFStats1Up");
TH1D *h3 =(TH1D*)f->Get("x_ttH_htt_CMS_ttHl_btag_LFStats1Down");
TH1D *h4 =(TH1D*)f->Get("x_ttW_CMS_ttHl_btag_HFStats2Up");

hs->Add(h1,"hist");
hs->Add(h2,"hist");
hs->Add(h3,"hist");
hs->Add(h4,"hist");

TCanvas *cs = new TCanvas("cs","cs",10,10,700,900);
 hs->Draw("nostack");


}
