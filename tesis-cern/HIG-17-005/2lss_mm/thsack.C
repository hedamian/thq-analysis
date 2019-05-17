  ///////////////////
//created by hiram ernesto damian
///////////////////////////
void thsack(){

gROOT->cd();
TFile* f = TFile::Open("tHq_2lss_mm_1_1.input.root");

THStack *hs = new THStack("hs","");
TH1D *h1 =(TH1D*)f->Get("x_tHq_htt");
TH1D *h2 =(TH1D*)f->Get("x_tHW_hzz");
TH1D *h3 =(TH1D*)f->Get("x_ttH_hzz");
TH1D *h4 =(TH1D*)f->Get("x_tHq_http25");
TH1D *h5 =(TH1D*)f->Get("x_tHW_hzzp5");
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
TH1F *tth =(TH1F*)h3->Clone("tth");
tth->Add(h20);
tth->Add(h23);
//////////////////////////////////////////
tth->SetFillColor(kOrange+7);
h14->SetFillColor(kSpring+2);
h15->SetFillColor(kSpring+3);
h9->SetFillColor(kMagenta-3);
h27->SetFillColor(kCyan-7);
h26->SetFillStyle(3005);
h26->SetFillColor(kBlue-3);
h26->SetMarkerStyle(3);
/////////////////////////////////////
hs->Add(h26,"hist");
hs->Add(h9,"hist");
hs->Add(h27,"hist");
hs->Add(h14,"hist");
hs->Add(h15,"hist");
hs->Add(tth,"hist");

/////////////////////////////////////
float(i1)=h1->Integral();
float(i2)=h2->Integral();
float(i3)=h3->Integral();
float(i4)=h4->Integral();
float(i5)=h5->Integral();
float(i6)=h6->Integral();
float(i7)=h7->Integral();
float(i8)=h8->Integral();
float(i9)=h9->Integral();
float(i10)=h10->Integral();
float(i11)=h11->Integral();
float(i12)=h12->Integral();
float(i13)=h13->Integral();
float(i14)=h14->Integral();
float(i15)=h15->Integral();
float(i16)=h16->Integral();
float(i17)=h17->Integral();
float(i18)=h18->Integral();
float(i19)=h19->Integral();
float(i20)=h20->Integral();
float(i21)=h21->Integral();
float(i22)=h22->Integral();
float(i23)=h23->Integral();
float(i24)=h24->Integral();
float(i25)=h25->Integral();
/////////////////////////////////////////////////
cout << "Value of tHq_htt is " << i1 << endl;
cout << "Value of tHW_hzz is " << i2 << endl;
cout << "Value of ttH_hzz is " << i3 << endl;
cout << "Value of tHq_http25 is " << i4 << endl;
cout << "Value of tHW_hzzp5 is " << i5 << endl;
cout << "Value of tHq_hzzp5 is " << i6 << endl;
cout << "Value of tHW_htt is " << i7 << endl;
cout << "Value of tHq_hzzp25 is " << i8 << endl;
cout << "Value of WZ is " << i9 << endl;
cout << "Value of tHq_http5 is " << i10 << endl;
cout << "Value of tHq_hww is " << i11 << endl;
cout << "Value of tHW_hwwp5 is " << i12 << endl;
cout << "Value of tHW_http25 is " << i13 << endl;
cout << "Value of ttZ is " << i14 << endl;
cout << "Value of ttW is " << i15 << endl;
cout << "Value of tHq_hzz is " << i16 << endl;
cout << "Value of tHq_hwwp25 is " << i17 << endl;
cout << "Value of WWss is " << i18 << endl;
cout << "Value of tHq_hwwp5  is " << i19 << endl;
cout << "Value of ttH_htt is " << i20 << endl;
cout << "Value of  tHW_http5 is " << i21 << endl;
cout << "Value of tHW_hww is is " << i22 << endl;
cout << "Value of ttH_hww is " << i23<< endl;
cout << "Value of tHW_hwwp25 " << i24 << endl;
cout << "Value of data_obs  is " << i25<< endl;

cout << "tth " << i3+i20+i23 <<endl;
cout << "ttz "<< i14<<endl;

cout << "WZ "<< i9 <<endl;
cout<<"ttw "<< i15<<endl;
cout<<"thq "<<i1+i11+i16<<endl;
cout<<"thw "<<i2+i5+i7+i22<<endl;
Double_t q=h25->GetBinContent(1);
cout<<q<<endl;
////////////////////////////////
TCanvas *cs = new TCanvas("cs","cs",100,100,700,900);

TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
pad1->SetBottomMargin(0.01); // Upper and lower plot are joined
//pad1->SetGridx();         // Vertical grid
pad1->Draw();             // Draw the upper pad: pad1
pad1->cd();               // pad1 becomes the current pad

h25->SetStats(0);
h25->Draw();
hs->Draw("same");

//////////////////
h25->GetXaxis()->SetLabelSize(0);
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
h25->SetStats(0);
h25->GetYaxis()->SetLimits(0,100);
h25->SetTitle("CMs preliminary Test               35.9f/b (13TeV)");

h25->GetYaxis()->SetTitle("Events/bin");
//h25->GetYaxis()->SetTitleOffset(1.0); h25->GetYaxis()->CenterTitle(true);
//h25->GetXaxis()->SetTitle("BDT bin");
//////////////////////////////////////////////////
h25->GetXaxis()->CenterTitle(true);
auto legend = new TLegend(0.6,0.6,0.89,0.89);
//legend->SetHeader("Test","C"); // option "C" allows to center the header
legend->AddEntry(h25,"data_obs","lep");
legend->AddEntry(tth,"tth","f");
legend->AddEntry(h15,"ttw","f");
legend->AddEntry(h14,"ttz","f");
legend->AddEntry(h9,"WZ","f");
legend->AddEntry(h27,"tz","f");
legend->AddEntry(h26,"Fakes","f");
legend->SetBorderSize(0);
legend->Draw();
pad1->RedrawAxis();

// Draw the ratio plot
TH1F *data2 = (TH1F*)h25->Clone("data2");
TH1F *sum1 =(TH1F*)tth->Clone("sum1");
sum1->Add(h26);
sum1->Add(h9);
sum1->Add(h14);
sum1->Add(h15);
sum1->Add(h18);
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
sum1->GetYaxis()->SetTitle("Data/Pred");
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
//bin array
//data data_obs
TH1F *bin1=(TH1F*)h25->Clone("bin1");
Double_t w1 = bin1->GetBinContent(1);
Double_t w2= bin1->GetBinContent(2);
Double_t w3 = bin1->GetBinContent(3);
Double_t w4 = bin1->GetBinContent(4);
Double_t w5 = bin1->GetBinContent(5);
Double_t w6 = bin1->GetBinContent(6);
Double_t w7 = bin1->GetBinContent(7);
Double_t w8 = bin1->GetBinContent(8);
Double_t w9 = bin1->GetBinContent(9);
Double_t w10 = bin1->GetBinContent(10);


cout<<"bin1 "<<w1<<endl;
cout<<"bin2 "<<w2<<endl;
cout<<"bin3 "<<w3<<endl;
cout<<"bin4 "<<w4<<endl;
cout<<"bin5 "<<w5<<endl;
cout<<"bin6 "<<w6<<endl;
cout<<"bin7 "<<w7<<endl;
cout<<"bin8 "<<w8<<endl;
cout<<"bin9 "<<w9<<endl;
cout<<"bin10 "<<w10<<endl;

bin1->SetBinContent(2,w3);
bin1->SetBinContent(3,w5);
bin1->SetBinContent(4,w2);
bin1->SetBinContent(5,w6);
bin1->SetBinContent(8,w4);
bin1->SetBinContent(6,w9);
bin1->SetBinContent(7,w7);
bin1->SetBinContent(9,w10);
bin1->SetBinContent(10,w8);
//////////////////////////////////////
TH1F *c18 =(TH1F*)h18->Clone("c18");
double_t p1 = c18->GetBinContent(1);
Double_t p2= c18->GetBinContent(2);
Double_t p3 = c18->GetBinContent(3);
Double_t p4 = c18->GetBinContent(4);
Double_t p5 = c18->GetBinContent(5);
Double_t p6 = c18->GetBinContent(6);
Double_t p7 = c18->GetBinContent(7);
Double_t p8 = c18->GetBinContent(8);
Double_t p9 = c18->GetBinContent(9);
Double_t p10 = c18->GetBinContent(10);

c18->SetBinContent(2,p3);
c18->SetBinContent(3,p5);
c18->SetBinContent(4,p2);
c18->SetBinContent(5,p6);
c18->SetBinContent(8,p4);
c18->SetBinContent(6,p9);
c18->SetBinContent(7,p7);
c18->SetBinContent(9,p10);
c18->SetBinContent(10,p8);

//////////////////hs
TH1F *c20 =(TH1F*)h20->Clone("c20");
double_t a1 = c20->GetBinContent(1);
Double_t a2= c20->GetBinContent(2);
Double_t a3 = c20->GetBinContent(3);
Double_t a4 = c20->GetBinContent(4);
Double_t a5 = c20->GetBinContent(5);
Double_t a6 = c20->GetBinContent(6);
Double_t a7 = c20->GetBinContent(7);
Double_t a8 = c20->GetBinContent(8);
Double_t a9 = c20->GetBinContent(9);
Double_t a10 = c20->GetBinContent(10);

c20->SetBinContent(2,a3);
c20->SetBinContent(3,a5);
c20->SetBinContent(4,a2);
c20->SetBinContent(5,a6);
c20->SetBinContent(8,a4);
c20->SetBinContent(6,a9);
c20->SetBinContent(7,a7);
c20->SetBinContent(9,a10);
c20->SetBinContent(10,a8);
TH1F *c23 =(TH1F*)h23->Clone("c23");
Double_t b1 = c23->GetBinContent(1);
Double_t b2= c23->GetBinContent(2);
Double_t b3 = c23->GetBinContent(3);
Double_t b4 = c23->GetBinContent(4);
Double_t b5 = c23->GetBinContent(5);
Double_t b6 = c23->GetBinContent(6);
Double_t b7 = c23->GetBinContent(7);
Double_t b8 = c23->GetBinContent(8);
Double_t b9 = c23->GetBinContent(9);
Double_t b10 = c23->GetBinContent(10);

c23->SetBinContent(2,b3);
c23->SetBinContent(3,b5);
c23->SetBinContent(4,b2);
c23->SetBinContent(5,b6);
c23->SetBinContent(8,b4);
c23->SetBinContent(6,b9);
c23->SetBinContent(7,b7);
c23->SetBinContent(9,b10);
c23->SetBinContent(10,b8);

///////////////////////////////////////

TH1F *tth1 =(TH1F*)h3->Clone("tth1");
Double_t t1 = tth1->GetBinContent(1);
Double_t t2= tth1->GetBinContent(2);
Double_t t3 = tth1->GetBinContent(3);
Double_t t4 = tth1->GetBinContent(4);
Double_t t5 = tth1->GetBinContent(5);
Double_t t6 = tth1->GetBinContent(6);
Double_t t7 =tth1->GetBinContent(7);
Double_t t8 = tth1->GetBinContent(8);
Double_t t9 = tth1->GetBinContent(9);
Double_t t10 = tth1->GetBinContent(10);


tth1->SetBinContent(2,t3);
tth1->SetBinContent(3,t5);
tth1->SetBinContent(4,t2);
tth1->SetBinContent(5,t6);
tth1->SetBinContent(8,t4);
tth1->SetBinContent(6,t9);
tth1->SetBinContent(7,t7);
tth1->SetBinContent(9,t10);
tth1->SetBinContent(10,t8);

tth1->Add(c20);
tth1->Add(c23);
/////////////////////////////////////////////////
TH1F *c26 =(TH1F*)h26->Clone("c26");
Double_t f1 = c26->GetBinContent(1);
Double_t f2= c26->GetBinContent(2);
Double_t f3 = c26->GetBinContent(3);
Double_t f4 = c26->GetBinContent(4);
Double_t f5 = c26->GetBinContent(5);
Double_t f6 = c26->GetBinContent(6);
Double_t f7 = c26->GetBinContent(7);
Double_t f8 = c26->GetBinContent(8);
Double_t f9 = c26->GetBinContent(9);
Double_t f10 = c26->GetBinContent(10);

c26->SetBinContent(2,f3);
c26->SetBinContent(3,f5);
c26->SetBinContent(4,f2);
c26->SetBinContent(5,f6);
c26->SetBinContent(8,f4);
c26->SetBinContent(6,f9);
c26->SetBinContent(7,f7);
c26->SetBinContent(9,f10);
c26->SetBinContent(10,f8);

c26->SetFillColor(kBlue+3);
TH1F *c9 =(TH1F*)h9->Clone("c9");
Double_t d1 = c9->GetBinContent(1);
Double_t d2= c9->GetBinContent(2);
Double_t d3 = c9->GetBinContent(3);
Double_t d4 = c9->GetBinContent(4);
Double_t d5 = c9->GetBinContent(5);
Double_t d6 = c9->GetBinContent(6);
Double_t d7 = c9->GetBinContent(7);
Double_t d8 = c9->GetBinContent(8);
Double_t d9 = c9->GetBinContent(9);
Double_t d10 = c9->GetBinContent(10);

c9->SetBinContent(2,d3);
c9->SetBinContent(3,d5);
c9->SetBinContent(4,d2);
c9->SetBinContent(5,d6);
c9->SetBinContent(8,d4);
c9->SetBinContent(6,d9);
c9->SetBinContent(7,d7);
c9->SetBinContent(9,d10);
c9->SetBinContent(10,d8);

TH1F *c14 =(TH1F*)h14->Clone("c14");

Double_t e1 = c14->GetBinContent(1);
Double_t e2= c14->GetBinContent(2);
Double_t e3 = c14->GetBinContent(3);
Double_t e4 = c14->GetBinContent(4);
Double_t e5 = c14->GetBinContent(5);
Double_t e6 = c14->GetBinContent(6);
Double_t e7 = c14->GetBinContent(7);
Double_t e8 = c14->GetBinContent(8);
Double_t e9 = c14->GetBinContent(9);
Double_t e10 = c14->GetBinContent(10);

c14->SetBinContent(2,e3);
c14->SetBinContent(3,e5);
c14->SetBinContent(4,e2);
c14->SetBinContent(5,e6);
c14->SetBinContent(8,e4);
c14->SetBinContent(6,e9);
c14->SetBinContent(7,e7);
c14->SetBinContent(9,e10);
c14->SetBinContent(10,e8);

TH1F *c15 =(TH1F*)h15->Clone("c15");
Double_t g1 = c15->GetBinContent(1);
Double_t g2= c15->GetBinContent(2);
Double_t g3 = c15->GetBinContent(3);
Double_t g4 = c15->GetBinContent(4);
Double_t g5 = c15->GetBinContent(5);
Double_t g6 = c15->GetBinContent(6);
Double_t g7 = c15->GetBinContent(7);
Double_t g8 = c15->GetBinContent(8);
Double_t g9 = c15->GetBinContent(9);
Double_t g10 = c15->GetBinContent(10);

c15->SetBinContent(2,g3);
c15->SetBinContent(3,g5);
c15->SetBinContent(4,g2);
c15->SetBinContent(5,g6);
c15->SetBinContent(8,g4);
c15->SetBinContent(6,g9);
c15->SetBinContent(7,g7);
c15->SetBinContent(9,g10);
c15->SetBinContent(10,g8);

/////////////////////////////////////////
THStack *hs1 = new THStack("hs1","");
hs1->Add(c26,"hist");
hs1->Add(c9,"hist");
hs1->Add(c18,"hist");
hs1->Add(c14,"hist");
hs1->Add(c15,"hist");
hs1->Add(tth1,"hist");
///////////////////////////////////////////////////////////
tth1->SetFillColor(kOrange+7);
//h25->SetFillColor(kBlue+1);
c14->SetFillColor(kSpring+2);
c15->SetFillColor(kSpring+3);
c9->SetFillColor(kMagenta-3);
c26->SetFillColor(kBlue-3);
c26->SetFillStyle(3005);

TCanvas *cs1 = new TCanvas("cs1","cs1",100,100,700,900);
TPad *padc1 = new TPad("padc1", "padc1", 0, 0.3, 1, 1.0);
padc1->SetBottomMargin(0.01); // Upper and lower plot are joined
//pad1->SetGridx();         // Vertical grid
padc1->Draw();             // Draw the upper pad: pad1
padc1->cd();               // pad1 becomes the current pad

bin1->SetStats(0);
bin1->Draw();
hs1->Draw("same");

//////////////////
bin1->GetXaxis()->SetLabelSize(0);
TGaxis *axisc = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
axisc->SetLabelFont(43); // Absolute font size in pixel (precision 3)
axisc->SetLabelSize(15);
axisc->Draw();

TGaxis *axisc6 = new TGaxis(0.5,110,10.5,110,100,220,510,"-");
//axis6->SetName("axis6");
axisc6->SetLabelFont(0); // Absolute font size in pixel (precision 3)
axisc6->SetLabelSize(15);
axisc6->Draw();

TGaxis *axisc5 = new TGaxis(10.5,0,10.5,110,100,220,510,"+L");
axisc5->SetLabelFont(0); // Absolute font size in pixel (precision 3)
axisc5->SetLabelSize(15);
axisc5->Draw();

///////////////////////////
bin1->GetYaxis()->SetLimits(0,100);
bin1->SetTitle("CMs preliminary Test               35.9f/b (13TeV)");

bin1->GetYaxis()->SetTitle("Events/bin");
//h25->GetYaxis()->SetTitleOffset(1.0); h25->GetYaxis()->CenterTitle(true);
//h25->GetXaxis()->SetTitle("BDT bin");
bin1->SetStats(0);
bin1->GetXaxis()->CenterTitle(true);
//////////////////////////////////////////////////
auto legendc = new TLegend(0.6,0.6,0.89,0.89);
//legend->SetHeader("Test","C"); // option "C" allows to center the header
legendc->AddEntry(bin1,"data_obs","lep");
legendc->AddEntry(tth1,"tth","f");
legendc->AddEntry(c15,"ttw","f");
legendc->AddEntry(c14,"ttz","f");
legendc->AddEntry(c9,"WZ","f");
legendc->AddEntry(c18,"tz","f");
legendc->AddEntry(c26,"Fakes","f");
legendc->SetBorderSize(0);
legendc->Draw();
padc1->RedrawAxis();
cs1->cd();
///////////////////////////////
// Draw the ratio plot
TH1F *data3 = (TH1F*)bin1->Clone("data3");
TH1F *sum2 =(TH1F*)tth1->Clone("sum2");
sum2->Add(c26);
sum2->Add(c9);
sum2->Add(c14);
sum2->Add(c15);
sum2->Divide(data3);
sum2->SetLineColor(kBlack);



TPad *padc2 = new TPad("padc2", "padc2", 0, 0.05, 1, 0.3);
padc2->SetTopMargin(0.1);
padc2->SetBottomMargin(0.2);
padc2->SetLeftMargin(0.1);
padc2->Draw();
padc2->cd();       // pad2 becomes the current pad
sum2->Draw();       // Draw the ratio plot
sum2->SetTitle(" ");
///////////////////////////////////
sum2->GetXaxis()->SetTitle("BDT bin");
sum2->GetXaxis()->SetTitleOffset(1.0);
sum2->GetXaxis()->SetTitleSize(0.1);
sum2->GetXaxis()->SetLabelSize(0.1);
/////////////////////////////////////
sum2->GetYaxis()->SetTitle("Data/Pred");
sum2->GetYaxis()->SetTitleOffset(0.4);
sum2->GetYaxis()->SetTitleSize(0.1);
sum2->GetYaxis()->SetLabelSize(0.1);
sum2->SetAxisRange(0, 3,"Y");
sum2->SetStats(0);

TGaxis *axis3c = new TGaxis(0.5,3,10.5,3,100,220,510,"-");
axis3c->SetLabelSize(0);
axis3c->Draw();

TGaxis *axis4c = new TGaxis(10.5,0,10.5,3,100,220,510,"+L");
axis4c->SetLabelSize(0);
axis4c->Draw();
////////////////////////////
TLine *linec = new TLine(0.5,1.0,10.5,1.0);
linec->SetLineColor(kRed);
linec->Draw("same");

cs->SaveAs("hist1.png");
cs1->SaveAs("hist2.png");
//gROOT->ProcessLine(".q");
}
