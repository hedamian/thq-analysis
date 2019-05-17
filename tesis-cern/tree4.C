void tree4() {

TFile *f=new TFile("ejemplo1.root","RECREATE");
TTree *T=new TTree("T","tree with fixed array");
TRandom r;
Float_t px[10],py[10],pt[10];
Int_t np;
T->Branch("np",&np,"np/I");
T->Branch("px",px,"px[np]/F");
T->Branch("py",py,"py[np]/F");
T->Branch("pt",pt,"pt[np]/F");

for (Int_t i = 0; i < 1000; i++) {
  Float_t x,y;
np=(Int_t)(r.Rndm()*10);
for(Int_t j=0;j<np;j++){
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
