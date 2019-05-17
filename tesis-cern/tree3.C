void tree3() {
TFile *f=new TFile("ejemplo1.root", "RECREATE");
TTree *T=new TTree("T","tree with fixed arrays");
TRandom r;
Float_t px[5],py[5],pt[5];

T->Branch("px",px,"px[5]/F");
T->Branch("py",py,"py[5]/F");
T->Branch("pt",pt,"pt[5]/F");

for (Int_t i=0; i<10000; i++) {
Float_t x,y;
for (Int_t j = 0; j < 5; j++) {
r.Rannor(x,y);
px[j]=x;
py[j]=y;
pt[j]=std::sqrt(x*x+y*y);
}
T->Fill();
}
f->Write();
f->Close();
}
