void tree_example2() {
TH2F *h_pxpy = new TH2F("h_pxpy", "py vs px",100,-2.0,2.0,100,-2.0,2.0);
TH1F *h_pt = new TH1F("h_pt","pt",100,0.,5.0);

TFile *f = new TFile("ejemplo1.root");
TTree *t1 = (TTree*)f->Get("T");

Float_t px,py,pt;
Double_t random;

t1->SetBranchAddress("px",&px);
t1->SetBranchAddress("py",&py);
t1->SetBranchAddress("pt",&pt);
t1->SetBranchAddress("random",&random);

Int_t nentries = (Int_t)t1->GetEntries();

for (Int_t i = 0; i<nentries; i++) {
  t1->GetEntry(i);

  h_pxpy->Fill(px, py);
  h_pt->Fill(pt);

}

TCanvas *cl = new TCanvas();
h_pxpy->Draw("colz");
cl->SaveAs("nou.png");

cl->Update();
h_pt->Draw();
cl->SaveAs("pt.png");

f->Close();
}
