TH1D *hclone(TH1D *tclone)
{
TH1D *ht =(TH1D*)tclone->Clone("ht");

Double_t g1 = ht->GetBinContent(1);
Double_t g2= ht->GetBinContent(2);
Double_t g3 = ht->GetBinContent(3);
Double_t g4 = ht->GetBinContent(4);
Double_t g5 = ht->GetBinContent(5);
Double_t g6 = ht->GetBinContent(6);
Double_t g7 = ht->GetBinContent(7);
Double_t g8 = ht->GetBinContent(8);
Double_t g9 = ht->GetBinContent(9);
Double_t g10 = ht->GetBinContent(10);

ht->SetBinContent(2,g3);
ht->SetBinContent(3,g5);
ht->SetBinContent(4,g2);
ht->SetBinContent(5,g6);
ht->SetBinContent(8,g4);
ht->SetBinContent(6,g9);
ht->SetBinContent(7,g7);
ht->SetBinContent(9,g10);
ht->SetBinContent(10,g8);

return ht;
}
/////////////////////////////////////////////////////////////////////////
void thclone(){
gROOT->cd();
TFile* f = TFile::Open("tHq_2lss_mm_1_1.input.root");
THStack *hs = new THStack("hs","");
TH1D *h1 =(TH1D*)f->Get("x_tHq_htt");
TH1D *h2 =(TH1D*)f->Get("x_tHW_hzz");
TH1D *h3 =(TH1D*)f->Get("x_ttH_hzz");
TH1D *h5 =(TH1D*)f->Get("x_tHW_hzzp5");
TH1D *h4 =(TH1D*)f->Get("x_tHq_http25");
TH1D *h6 =(TH1D*)f->Get("x_tHq_hzzp5");
TH1D *h7 =(TH1D*)f->Get("x_tHW_htt");
TH1D *h8 =(TH1D*)f->Get("x_tHq_hzzp25");
TH1D *h9 =(TH1D*)f->Get("x_WZ");
TH1D *h10 =(TH1D*)f->Get("x_tHq_http5");
TH1D *h11 =(TH1D*)f->Get("x_tHq_hww");
TH1D *h12 =(TH1D*)f->Get("x_tHW_hwwp5");
TH1D *h13 =(TH1D*)f->Get("x_tHW_http25");
TH1D *h14 =(TH1D*)f->Get("x_ttZ");
TH1D *h15 =(TH1D*)f->Get("x_ttW");
TH1D *h16 =(TH1D*)f->Get("x_tHq_hzz");
TH1D *h17 =(TH1D*)f->Get("x_tHq_hwwp25");
TH1D *h18 =(TH1D*)f->Get("x_WWss");
TH1D *h19 =(TH1D*)f->Get("x_tHq_hwwp5");
TH1D *h20 =(TH1D*)f->Get("x_ttH_htt");
TH1D *h21 =(TH1D*)f->Get("x_tHW_http5");
TH1D *h22 =(TH1D*)f->Get("x_tHW_hww");
TH1D *h23 =(TH1D*)f->Get("x_ttH_hww");
TH1D *h24 =(TH1D*)f->Get("x_tHW_hwwp25");
TH1D *h25 =(TH1D*)f->Get("x_data_obs");
TH1D *h26 =(TH1D*)f->Get("x_data_fakes");
TH1D *h27=(TH1D*)f->Get("x_Rares");
///////////////////////////////////////////
TH1D *c25=hclone(h25);
TH1D *c14=hclone(h14);
TH1D *c9=hclone(h9);
TH1D *c26=hclone(h26);
TH1D *c20=hclone(h20);
TH1D *c23=hclone(h23);
TH1D *c3=hclone(h3);
TH1D *c15=hclone(h15);
TH1D *c27=hclone(h27);

TH1F *ctth =(TH1F*)c3->Clone("ctth");
ctth->Add(c20);
ctth->Add(c23);

TH1D *c1=hclone(h1);
TH1D *c4=hclone(h4);
TH1D *c6=hclone(h6);
TH1D *c8=hclone(h8);
TH1D *c10=hclone(h10);
TH1D *c11=hclone(h11);
TH1D *c16=hclone(h16);
TH1D *c17=hclone(h17);
TH1D *c19=hclone(h19);

TH1F *cthq=(TH1F*)c1->Clone("cthq");
cthq->Add(c4);
cthq->Add(c6);
cthq->Add(c8);
cthq->Add(c10);
cthq->Add(c11);
cthq->Add(c16);
cthq->Add(c17);
cthq->Add(c19);
cthq->Add(ctth);

//////////////////////////////////////////
cthq->SetFillColor(kOrange+7);
c14->SetFillColor(kSpring+2);
c15->SetFillColor(kSpring+3);
c9->SetFillColor(kMagenta-3);
c27->SetFillColor(kCyan-7);
c26->SetFillStyle(3005);
c26->SetFillColor(kBlue-3);
c25->SetMarkerStyle(8);
/////////////////////////////////////
hs->Add(c26,"hist");
hs->Add(c27,"hist");
hs->Add(c9,"hist");
hs->Add(c14,"hist");
hs->Add(c15,"hist");
hs->Add(cthq,"hist");

////////////////////////////////
TCanvas *cs = new TCanvas("cs","cs",100,100,700,900);

TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
pad1->SetBottomMargin(0.01); // Upper and lower plot are joined
//pad1->SetGridx();         // Vertical grid
pad1->Draw();             // Draw the upper pad: pad1
pad1->cd();               // pad1 becomes the current pad

c25->SetStats(0);
c25->Draw();
hs->Draw("same");
c25->Draw("same");
//////////////////
c25->GetXaxis()->SetLabelSize(0);
TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
axis->SetLabelSize(15);
axis->Draw();

TGaxis *axis6 = new TGaxis(0.5,110,10.5,110,100,220,510,"-");
//axis6->SetName("axis6");
axis6->SetLabelFont(0); // Absolute font size in pixel (precision 3)
axis6->SetLabelSize(15);
axis6->Draw();

TGaxis *axis5 = new TGaxis(10.5,0,10.5,110,100,220,510,"+L");
axis5->SetLabelFont(0); // Absolute font size in pixel (precision 3)
axis5->SetLabelSize(15);
axis5->Draw();

///////////////////////////
c25->SetStats(0);
c25->GetYaxis()->SetLimits(0,100);
c25->SetTitle("CMs preliminary Test               35.9f/b (13TeV)");

c25->GetYaxis()->SetTitle("Events/bin");
//h25->GetYaxis()->SetTitleOffset(1.0); h25->GetYaxis()->CenterTitle(true);
//h25->GetXaxis()->SetTitle("BDT bin");
c25->GetXaxis()->CenterTitle(true);
//////////////////////////////////////////////////
auto legend = new TLegend(0.6,0.6,0.89,0.89);
//legend->SetHeader("Test","C"); // option "C" allows to center the header
legend->AddEntry(c25,"data_obs","lep");
legend->AddEntry(cthq,"tth","f");
legend->AddEntry(c15,"ttw","f");
legend->AddEntry(c14,"ttz","f");
legend->AddEntry(c9,"WZ","f");
legend->AddEntry(c27,"tZ, WW, VVV,tttt","f");
legend->AddEntry(c26,"Fakes","f");
legend->SetBorderSize(0);
legend->Draw();
pad1->RedrawAxis();

// Draw the ratio plot
TH1F *data2 = (TH1F*)c25->Clone("data2");
TH1F *sum1 =(TH1F*)ctth->Clone("sum1");
sum1->Add(c26);
sum1->Add(c9);
sum1->Add(c14);
sum1->Add(c15);
sum1->Add(c27);
sum1->Divide(data2);
sum1->SetLineColor(kBlack);
cs->cd();          // Go back to the main canvas before defining pad2
/////////////////////////////////////////
TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
pad2->SetTopMargin(0.1);
pad2->SetBottomMargin(0.2);
pad2->SetLeftMargin(0.1);
pad2->Draw();
pad2->cd();       // pad2 becomes the current pad
sum1->Draw();       // Draw the ratio plot
sum1->SetTitle(" ");
///////////////////////////////////
sum1->GetXaxis()->SetTitle("BDT bin");
sum1->GetXaxis()->SetTitleOffset(1.0);
sum1->GetXaxis()->SetTitleSize(0.1);
sum1->GetXaxis()->SetLabelSize(0.1);
/////////////////////////////////////
sum1->GetYaxis()->SetTitle("Pred/Data");
sum1->GetYaxis()->SetTitleOffset(0.4);
sum1->GetYaxis()->SetTitleSize(0.1);
sum1->GetYaxis()->SetLabelSize(0.1);
sum1->SetAxisRange(0, 3,"Y");
sum1->SetStats(0);


TGaxis *axis3 = new TGaxis(0.5,3,10.5,3,100,220,510,"-");
axis3->SetLabelSize(0);
axis3->Draw();

TGaxis *axis4 = new TGaxis(10.5,0,10.5,3,100,220,510,"+L");
axis4->SetLabelSize(0);
axis4->Draw();
////////////////////////////
TLine *line = new TLine(0.5,1.0,10.5,1.0);
line->SetLineColor(kRed);
line->Draw("same");
/////////////////////////////////

cs->SaveAs("hist-function.png");


float(i18)=h27->Integral();
float(i26)=h26->Integral();
float(ih)=cthq->Integral();
cout<<"tz "<<i18<<endl;
cout<<i26<<endl;
cout<<"thq"<<ih<<endl;
}
