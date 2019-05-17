#include "RooStats/HistFactory/MakeModelAndMeasurementsFast.h"
#include "TH1D.h"
#include <cmath>

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

void SimpleNumberCounting()
{
//define event counts

TFile* f = TFile::Open("tHq_2lss_mm_1_1.input.root");
//--------------------
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
//tthrams(*mc->GetNuisancePa
///////////////////////
TH1F *ctth =(TH1F*)c3->Clone("ctth"); //tth
ctth->Add(c20);
ctth->Add(c23);
/////////////////////////////////
//thq
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

TH1F *cthq=(TH1F*)c1->Clone("cthq");
cthq->Add(c4);
cthq->Add(c6);
cthq->Add(c8);
cthq->Add(c10);
cthq->Add(c11);
cthq->Add(c16);
cthq->Add(c17);
cthq->Add(c19);
//cthq->Add(ctth);


//--------------------

//create the histfactory model
RooStats::HistFactory::Measurement meas("SimpleNumberCounting", "SimpleNumberCounting");
meas.SetOutputFilePrefix("./workspace/SimpleNumberCounting/tut");
meas.SetExportOnly(1);
meas.SetPOI("mu");

  // this scales the histogram content, which already includes lumi, so set to 1
  meas.SetLumi(1.0);
  meas.SetLumiRelErr(0.0001);

//create the parameters for the model

  //first the signal normalization
RooStats::HistFactory::NormFactor normS;
normS.SetName("mu");
normS.SetHigh(100); // maximum value it can take
normS.SetLow(0); // minimum value it can take
normS.SetVal(1); // nominal value

//create the SR
RooStats::HistFactory::Channel SR("SR");
SR.SetData(c25);

//add the signal and background samples
RooStats::HistFactory::Sample sample_S("S"); //signal thq
sample_S.SetHisto(cthq);
sample_S.AddNormFactor(normS);
SR.AddSample(sample_S);


double eps_H = 0.05;
RooStats::HistFactory::Sample sample_H("H"); //background tth
sample_H.SetHisto(ctth);
sample_H.AddOverallSys("sample_B_sys",1./(1+eps_H),(1+eps_H));
SR.AddSample(sample_H);
cout<<eps_H<<endl;


double eps_B = sqrt(pow(0.04,2)+pow(0.1,2));
RooStats::HistFactory::Sample sample_B("B"); //background ttz
sample_B.SetHisto(c14);
sample_B.AddOverallSys("sample_B_sys",1./(1+eps_B),(1+eps_B));
SR.AddSample(sample_B);
cout<<eps_B<<endl;

double eps_T = sqrt(pow(0.04,2)+pow(0.12,2));
RooStats::HistFactory::Sample sample_ttw("T");
sample_ttw.SetHisto(c15); //background ttw
sample_ttw.AddOverallSys("sample_T_sys",1./(1+eps_T),(1+eps_T));
SR.AddSample(sample_ttw);
cout<<eps_T<<endl;

RooStats::HistFactory::Sample sample_wz("W");
sample_wz.SetHisto(c9); //background wz
sample_wz.AddOverallSys("sample_W_sys",1./(0.5+1),(1+0.5));
SR.AddSample(sample_wz);


RooStats::HistFactory::Sample sample_z("Z");
sample_z.SetHisto(c27);//background tz
sample_z.AddOverallSys("sample_Z_sys",1./(0.5+1),(1+0.5));
SR.AddSample(sample_z);

RooStats::HistFactory::Sample sample_fake("F");
sample_fake.SetHisto(c26);//background fake
sample_fake.AddOverallSys("sample_F_sys",1./(1+0.4),(1+0.4));
SR.AddSample(sample_fake);

//add the single region to the measurement
meas.AddChannel(SR);

//make the workspace
RooStats::HistFactory::MakeModelAndMeasurementFast(meas);

}
