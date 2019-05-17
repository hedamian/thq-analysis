void tree5() {
TH2F *h_pxpy = new TH2F("h_pxpy","py vs px",100,-4.0,4.0,100,-4.0,4.0);
TH1F *h_pt= new TH1F("h_pt","pt",100,0.,8.0);

TFile *f=new TFile("ejemplo1.root");
TTree *t1 = (TTree*)f->Get("T");

Float_t px[10],py[10],pt[10];
Int_t np;

t1->SetBranchAddress ("np",&np);
t1->SetBranchAddress ("px",px);
t1->SetBranchAddress ("py",py);
t1->SetBranchAddress ("pt",pt);

Int_t nentries = (Int_t)t1->GetEntries();

for (Int_t i=0; i < nentries; i++) {
  t1->GetEntry(i);
  //Find the object with highest pt and fill its distributions
  Float_t hPt=0;
  Int_t h_index =-1;
  if(np>0){
    for (Int_t j=0;j<np;j++){
      if(pt[j]>hPt){
        hPt=pt[j];
        h_index=j;
      }
    }
    h_pxpy->Fill(px[h_index],py[h_index]);
    h_pt->Fill(pt[h_index]);
  }
}

TCanvas *c1= new TCanvas();
h_pxpy->Draw("colz");
c1->SaveAs("tree5.png");

c1->Update();
h_pt->Draw();
c1->SaveAs("tree6.png");
f->Close();
}
