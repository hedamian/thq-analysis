void tejemplo() {
TFile *f = new TFile("ejemplo1.root","RECREATE");
TTree *T = new TTree("T","simple tree");
TRandom r;
Float_t px,py,pt;
Double_t random;
UShort_t i;

T->Branch("px",&px, "px/F");
T->Branch("py",&py, "py/F");
T->Branch("pt",&pt, "pt/F");
T->Branch("random",&random, "TRandom/F");

for (i=0;i<10000;i++) {
  r.Rannor(px,py);
  pt=std::sqrt(px*px+py*py);
  random=r.Rndm();
  T->Fill();
}

f->Write();
f->Close();
}
