

void gluglu(){

  TFile file("./GluGluHToBB_M125_13TeV_powheg_pythia8.root");

  TTree* t = (TTree*)file.Get("otree");

  TCanvas C;
  C.Clear();
  t->Draw("nAK4PuppijetsPt30dR08jesDown_0");
  C.Print("AK8Puppijet0_pt.pdf");

}
