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
using namespace RooFit;
void histofit(){
TFile* f = TFile::Open("tHq_2lss_mm_1_1.input.root");
TH1D *h1 =(TH1D*)f->Get("x_tHq_htt"); //x_tHq_htt
TH1D *h2 =(TH1D*)f->Get("x_tHW_hzz");
TH1D *h3 =(TH1D*)f->Get("x_ttH_hzz");
TH1D *h5 =(TH1D*)f->Get("x_tHW_hzzp5");
TH1D *h4 =(TH1D*)f->Get("x_tHq_http25"); //x_tHq_http25
TH1D *h6 =(TH1D*)f->Get("x_tHq_hzzp5");//t_thq_hzzp5
TH1D *h7 =(TH1D*)f->Get("x_tHW_htt");
TH1D *h8 =(TH1D*)f->Get("x_tHq_hzzp25");//thq_hzz2p5
TH1D *h9 =(TH1D*)f->Get("x_WZ");
TH1D *h10 =(TH1D*)f->Get("x_tHq_http5");//x_tHq_http5
TH1D *h11 =(TH1D*)f->Get("x_tHq_hww"); ///thq_hww
TH1D *h12 =(TH1D*)f->Get("x_tHW_hwwp5");
TH1D *h13 =(TH1D*)f->Get("x_tHW_http25");
TH1D *h14 =(TH1D*)f->Get("x_ttZ");
TH1D *h15 =(TH1D*)f->Get("x_ttW"); //ruido
TH1D *h16 =(TH1D*)f->Get("x_tHq_hzz"); ///thq_hzz
TH1D *h17 =(TH1D*)f->Get("x_tHq_hwwp25");///thq_hwwp25
TH1D *h18 =(TH1D*)f->Get("x_WWss");
TH1D *h19 =(TH1D*)f->Get("x_tHq_hwwp5");////thq_hwwp5
TH1D *h20 =(TH1D*)f->Get("x_ttH_htt");//signal
TH1D *h21 =(TH1D*)f->Get("x_tHW_http5");
TH1D *h22 =(TH1D*)f->Get("x_tHW_hww");
TH1D *h23 =(TH1D*)f->Get("x_ttH_hww");
TH1D *h24 =(TH1D*)f->Get("x_tHW_hwwp25");
TH1D *h25 =(TH1D*)f->Get("x_data_obs");
TH1D *h26 =(TH1D*)f->Get("x_data_fakes");
TH1D *h27=(TH1D*)f->Get("x_Rares");
///////////////////////////////////////////
TH1D *c25=hclone(h25); //data obs
TH1D *c14=hclone(h14); //ttz
TH1D *c9=hclone(h9);///WZ
TH1D *c26=hclone(h26);///data fakes
TH1D *c20=hclone(h20);//tth_htt
TH1D *c23=hclone(h23);////x_ttH_hww
TH1D *c3=hclone(h3);////x_ttH_hzz
TH1D *c15=hclone(h15);///x_ttW
TH1D *c27=hclone(h27);////x_Rares
//////////////////
//tth
///////////////////////
TH1F *ctth =(TH1F*)c3->Clone("ctth"); //////tth background
ctth->Add(c20);
ctth->Add(c23);
/////////////////////////////////
//th
//////////////////////
TH1D *c1=hclone(h1);
TH1D *c4=hclone(h4);
TH1D *c6=hclone(h6);
TH1D *c8=hclone(h8);
TH1D *c10=hclone(h10);
TH1D *c11=hclone(h11);
TH1D *c16=hclone(h16);
TH1D *c17=hclone(h17);
TH1D *c19=hclone(h19);

TH1F *cthq=(TH1F*)c1->Clone("cthq"); //th
cthq->Add(c4);
cthq->Add(c6);
cthq->Add(c8);
cthq->Add(c10);
cthq->Add(c11);
cthq->Add(c16);
cthq->Add(c17);
cthq->Add(c19);

//////////////////////////////////////////
cthq->SetFillColor(kOrange+7);
ctth->SetFillColor(kRed+1);
c14->SetFillColor(kSpring+2);
c15->SetFillColor(kSpring+3);
c9->SetFillColor(kMagenta-3);
c27->SetFillColor(kCyan);
c26->SetFillStyle(3005);
c26->SetFillColor(kBlue-3);
c25->SetMarkerStyle(8);
/////////////////////////////////////



//constraint
float(ifake)=c26->Integral();
float(iwz)=c9->Integral();
float(ittz)=c14->Integral();
float(ittw)=c15->Integral();
float(itz)=c27->Integral();
float(idata)=c25->Integral();
float(ins)=cthq->Integral();
float(itth)=ctth->Integral();
////////////////////////////////
//RooFit
//////////////////////////////////////
RooRealVar x("x","x",0.5,10.0);
RooDataHist s("s","s",RooArgList(x),&*cthq); ///signal
RooHistPdf thq_s("thq_s","signal thq",RooArgSet(x),s);
RooDataHist data_obs("data_obs","data_obs",RooArgList(x),&*c25); //histogram to fit
RooRealVar nsig("nsig","#signal events",ins,0,10000) ;


//tth
RooDataHist btth("bttw","bttw",RooArgList(x),&*ctth); //background tth
RooHistPdf tth_b("tth_b","background ttw",RooArgSet(x),btth);
RooRealVar nbkg_tth("nbkg_ttw","#background events",itth,0.,10000) ;

////////////////////////////////////////////////////////
//ttw
RooDataHist bttw("bttw","bttw",RooArgList(x),&*c15); //background ttw
RooHistPdf ttw_b("ttw_b","background ttw",RooArgSet(x),bttw);

RooRealVar nbkg_ttw("nbkg_ttw","#background events",ittw,0.,10000) ;

////////////////////////////////
//////////
//ttz
//////
RooDataHist bttz("bttz","bttz",RooArgList(x),&*c14); //background tth
RooHistPdf ttz_b("ttz_b","background ttz",RooArgSet(x),bttz);

RooRealVar nbkg_ttz("nbkg_ttz","#background events",ittz,0.,10000) ;
//////////////////////////////////////
//wz
RooDataHist bwz("bwz","bttz",RooArgList(x),&*c9); //background wz
RooHistPdf wz_b("wz_b","background wz",RooArgSet(x),bwz);
RooRealVar nbkg_wz("nbkg_wz","#background events",iwz,0.,10000) ;
///////////////////////
///tz,ww,tttt,vvv
RooDataHist btz("btz","btz",RooArgList(x),&*c27); //background tz
RooHistPdf tz_b("tz_b","background tz",RooArgSet(x),btz);
RooRealVar nbkg_tz("nbkg_tz","#background events",itz,0.,10000) ;
////////////////////
//fake
RooDataHist bf("bf","bf",RooArgList(x),&*c26); //background fake
RooHistPdf f_b("f_b","background fake",RooArgSet(x),bf);
RooRealVar nbkg_f("nbkg_f","#background events",ifake,0,10000) ;

////////////////////////////////////
RooGaussian const_f("const_f","fconstraint",nbkg_f,RooConst(ifake),RooConst(ifake*0.2)) ;
//RooGaussian const_ng("const_ng","fconstraint",nsig,RooConst(ins),RooConst(ins*0.2)) ;
RooGaussian const_ttz("const_ttz","fconstraint",nbkg_ttz,RooConst(ittz),RooConst(ittz*0.2)) ;
RooGaussian const_ttw("const_ttw","fconstraint",nbkg_ttw,RooConst(ittw),RooConst(ittw*0.2)) ;
RooGaussian const_wz("const_wz","fconstraint",nbkg_wz,RooConst(iwz),RooConst(iwz*0.2)) ;
RooGaussian const_tz("const_tz","fconstraint",nbkg_tz,RooConst(itz),RooConst(itz*0.2)) ;
RooGaussian const_tth("const_tth","fconstraint",nbkg_tth,RooConst(itth),RooConst(itth*0.2));
/////////////////////////////////
RooAddPdf model("model","thq",RooArgList(thq_s,tth_b,ttw_b,ttz_b,wz_b,tz_b,f_b),
RooArgList(nsig,nbkg_tth,nbkg_ttw,nbkg_ttz, nbkg_wz,nbkg_tz,nbkg_f));
//model.fitTo(data_obs) ;

//////////////////////
///model with constraint

RooProdPdf modelc("modelc","model with constraint",RooArgSet(model,const_tth,const_ttz,const_ttw,const_wz,const_tz,const_f));
modelc.fitTo(data_obs,Constrain(RooArgSet(nbkg_tth,nbkg_ttz,nbkg_ttw,nbkg_wz,nbkg_tz,nbkg_f)));
//////////////////////////


// Draw the ratio plot
TH1D *hratio=(TH1D*)modelc.createHistogram("hratio",x);
hratio->Divide(c25);


TCanvas* cs = new TCanvas("cs","fit bdt const",100,100,700,900) ;
TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
pad1->SetBottomMargin(0.01); // Upper and lower plot are joined
//pad1->SetGridx();         // Vertical grid
pad1->Draw();             // Draw the upper pad: pad1
pad1->cd();               // pad1 becomes the current pad
///////////////////////////////
RooPlot* pc=x.frame(Title("CMs preliminary                35.9f/b (13TeV)"));
data_obs.plotOn(pc,Name("Data Obs"));
modelc.plotOn(pc,Name("hfit"),LineColor(kBlack));
modelc.plotOn(pc,Components(RooArgSet(thq_s,tth_b,ttz_b,ttw_b,wz_b,tz_b,f_b)),FillColor(kOrange+7),DrawOption("F"),Name("th"));//thq
modelc.plotOn(pc,Components(RooArgSet(tth_b,ttz_b,ttw_b,wz_b,tz_b,f_b)),FillColor(kRed+1),DrawOption("F"),Name("tth")); //ttw
modelc.plotOn(pc,Components(RooArgSet(ttz_b,ttw_b,wz_b,tz_b,f_b)),FillColor(kSpring+3),DrawOption("F"),Name("ttw")); //ttw
modelc.plotOn(pc,Components(RooArgSet(ttz_b,wz_b,tz_b,f_b)),FillColor(kSpring+2),DrawOption("F"),Name("ttz")); //ttz
modelc.plotOn(pc,Components(RooArgSet(wz_b,tz_b,f_b)),FillColor(kMagenta-3),DrawOption("F"),Name("Wz")); //wz
modelc.plotOn(pc,Components(RooArgSet(tz_b,f_b)),FillColor(kCyan),DrawOption("F"),Name("tz,WW,vvv")); //tz
modelc.plotOn(pc,Components(RooArgSet(f_b)),FillColor(0),FillStyle(1001),DrawOption("F"),Name("Fakes")); //fake
modelc.plotOn(pc,Components(RooArgSet(f_b)),FillColor(kBlue-3),LineColor(1),FillStyle(3005),DrawOption("F"),Name("Fakes")); //fake
data_obs.plotOn(pc)->GetYaxis()->SetTitle("Events/bins");
pc->Draw();

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
legend->AddEntry("hfit","fit","l");
legend->AddEntry(cthq,"th","f");
legend->AddEntry(ctth,"tth","f");
legend->AddEntry(c15,"ttw","f");
legend->AddEntry(c14,"ttz","f");
legend->AddEntry(c9,"WZ","f");
legend->AddEntry(c27,"tZ, WW, VVV,tttt","f");
legend->AddEntry(c26,"Fakes","f");
legend->SetBorderSize(0);
legend->Draw();
pad1->RedrawAxis();

cs->cd();          // Go back to the main canvas before defining pad2
/////////////////////////////////////////
TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
pad2->SetTopMargin(0.1);
pad2->SetBottomMargin(0.2);
pad2->SetLeftMargin(0.1);
pad2->Draw();
pad2->cd();       // pad2 becomes the current pad
hratio->Draw();       // Draw the ratio plot
hratio->SetTitle(" ");
///////////////////////////////////
hratio->GetXaxis()->SetTitle("BDT bin");
hratio->GetXaxis()->SetTitleOffset(1.0);
hratio->GetXaxis()->SetTitleSize(0.1);
hratio->GetXaxis()->SetLabelSize(0.1);
/////////////////////////////////////
hratio->GetYaxis()->SetTitle("Pred/Data");
hratio->GetYaxis()->SetTitleOffset(0.4);
hratio->GetYaxis()->SetTitleSize(0.1);
hratio->GetYaxis()->SetLabelSize(0.1);
hratio->SetAxisRange(0, 3,"Y");
hratio->SetStats(0);


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

cs->Print("histofit-constrain.png");

}
