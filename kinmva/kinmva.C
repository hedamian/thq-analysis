#include "RooStats/HistFactory/MakeModelAndMeasurementsFast.h"
#include "TH1D.h"
#include "eps.h"
void kinmva(){

TFile* f = TFile::Open("plots-thq-2lss-kinMVA.root");
TH1D *tzq =(TH1D*)f->Get("thqMVA_ttv_2lss_40_tZq");
TH1D *ttz =(TH1D*)f->Get("thqMVA_ttv_2lss_40_ttZ");
TH1D *tzw =(TH1D*)f->Get("thqMVA_ttv_2lss_40_tZW");
TH1D *vvv =(TH1D*)f->Get("thqMVA_ttv_2lss_40_VVV");
TH1D *sg =(TH1D*)f->Get("thqMVA_ttv_2lss_40_signal"); ///////**
TH1D *ttw =(TH1D*)f->Get("thqMVA_ttv_2lss_40_ttW");
TH1D *fakes =(TH1D*)f->Get("thqMVA_ttv_2lss_40_data_fakes");
TH1D *gstar =(TH1D*)f->Get("thqMVA_ttv_2lss_40_Gstar"); ///****
TH1D *tth =(TH1D*)f->Get("thqMVA_ttv_2lss_40_ttH");
TH1D *thw =(TH1D*)f->Get("thqMVA_ttv_2lss_40_tHW_hww");
TH1D *zz =(TH1D*)f->Get("thqMVA_ttv_2lss_40_ZZ");
TH1D *ww =(TH1D*)f->Get("thqMVA_ttv_2lss_40_WWss");
TH1D *tttt =(TH1D*)f->Get("thqMVA_ttv_2lss_40_tttt");
TH1D *data =(TH1D*)f->Get("thqMVA_ttv_2lss_40_data");
TH1D *wz =(TH1D*)f->Get("thqMVA_ttv_2lss_40_WZ");
TH1D *thq =(TH1D*)f->Get("thqMVA_ttv_2lss_40_tHq_hww");


tzq->Scale(5.77/tzq->Integral()); //tzq
vvv->Scale(2.99/vvv->Integral()); //vvv
ww->Scale(3.96/ww->Integral()); //wwss
tttt->Scale(2.32/tttt->Integral()); // tttt
fakes->Scale(80.94/fakes->Integral()); //non prompt
wz->Scale(15.07/wz->Integral()); //WZ
ttz->Scale(25.89/ttz->Integral());//ttZ
ttw->Scale(68.03/ttw->Integral());//ttW
tth->Scale(24.18/tth->Integral());//tth
thq->Scale(1.42/thq->Integral());//thq
thw->Scale(0.71/thw->Integral());//thw
tzw->Scale(2.13/tzw->Integral());//tzw
zz->Scale(1.16/zz->Integral()); //zz


TH1F *th=(TH1F*)thq->Clone("th");
th->Add(thw);

TH1F *tz=(TH1F*)tzq->Clone("tz");
tz->Add(vvv);
tz->Add(tttt);
tz->Add(ww);
tz->Add(tzw);

wz->Add(zz);
////////////////////////////////////////////
//create the histfactory model
RooStats::HistFactory::Measurement meas("kinMVA", "kinMVA");
//meas.SetOutputFilePrefix("./workspace/SimpleNumberCounting/tut");
meas.SetExportOnly(1);
meas.SetPOI("mu");

  // this scales the histogram content, which already includes lumi, so set to 1
  meas.SetLumi(1.0);
  meas.SetLumiRelErr(0.026);
//create the parameters for the model

  //first the signal normalization
RooStats::HistFactory::NormFactor normS;
normS.SetName("mu");
normS.SetHigh(100); // maximum value it can take
normS.SetLow(-33); // minimum value it can take
normS.SetVal(1); // nominal value

//create the SR
RooStats::HistFactory::Channel SR("SR");
SR.SetData(data);

//add the signal and background samples
RooStats::HistFactory::Sample sample_th("th"); //signal thq
sample_th.SetHisto(th);
sample_th.AddNormFactor(normS);
SR.AddSample(sample_th);


RooStats::HistFactory::Sample sample_tth("tth"); //background tth
sample_tth.SetHisto(tth);
sample_tth.AddOverallSys("sample_tth_sys",1./(1+eps_tth),(1+eps_tth));
SR.AddSample(sample_tth);

RooStats::HistFactory::Sample sample_ttz("ttz"); //background ttz
sample_ttz.SetHisto(ttz);
sample_ttz.AddOverallSys("sample_ttz_sys",1./(1+eps_ttz),(1+eps_ttz));
SR.AddSample(sample_ttz);


RooStats::HistFactory::Sample sample_ttw("ttw");
sample_ttw.SetHisto(ttw); //background ttw
sample_ttw.AddOverallSys("sample_ttw_sys",1./(1+eps_ttw),(1+eps_ttw));
SR.AddSample(sample_ttw);

RooStats::HistFactory::Sample sample_tz("tz");
sample_tz.SetHisto(tz);//background tz
sample_tz.AddOverallSys("sample_tz_sys",1./(eps_tz+1),(1+eps_tz));
SR.AddSample(sample_tz);


RooStats::HistFactory::Sample sample_wz("wz");
sample_wz.SetHisto(wz); //background wz
sample_wz.AddOverallSys("sample_wz_sys",1./(eps_wz+1),(1+eps_wz));
SR.AddSample(sample_wz);


RooStats::HistFactory::Sample sample_fakes("fakes");
sample_fakes.SetHisto(fakes);//background fake
sample_fakes.AddOverallSys("sample_fakes_sys",1./(1+eps_fakes),(1+eps_fakes));
SR.AddSample(sample_fakes);

//add the single region to the measurement
meas.AddChannel(SR);

//make the workspace
RooStats::HistFactory::MakeModelAndMeasurementFast(meas);


cout<<"error tth  "<<tth->Integral()*eps_tth<<endl;
cout<<"error ttw  "<<ttw->Integral()*eps_ttw<<endl;
cout<<"error ttz  "<<ttz->Integral()*eps_ttz<<endl;
cout<<"error wz "<<wz->Integral()*eps_wz<<endl;
cout<<"error tz "<<tz->Integral()*eps_tz<<endl;
cout<<"error fakes  "<<fakes->Integral()*eps_fakes<<endl;

///////////////////////////////////////////
THStack *hs = new THStack("hs","");
hs->Add(fakes,"hist"); //fakes
hs->Add(tz,"hist");// tz
hs->Add(wz,"hist");//wz
hs->Add(ttz,"hist"); // ttz
hs->Add(ttw,"hist");//ttw
hs->Add(tth,"hist");//tth`
hs->Add(th,"hist");//signal th

tth->SetFillColor(kOrange+7);
tz->SetFillColor(kCyan);
ttw->SetFillColor(kSpring+3);
ttz->SetFillColor(kSpring+2);
wz->SetFillColor(kMagenta-3);
fakes->SetFillStyle(3005);
fakes->SetFillColor(kBlue-3);
fakes->SetMarkerStyle(8);
th->SetFillColor(kRed);

TCanvas *cs = new TCanvas("cs","cs",100,100,700,900);
cs->SetCanvasSize(900,900);
TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
pad1->SetBottomMargin(0.01); // Upper and lower plot are joined
//pad1->SetGridx();         // Vertical grid
pad1->Draw();             // Draw the upper pad: pad1
pad1->cd();               // pad1 becomes the current pad]

data->Draw();
hs->Draw("same");
data->Draw("same");

data->SetStats(0);
data->GetYaxis()->SetRangeUser(0,70);
data->SetTitle("CMs preliminary Test               35.9f/b (13TeV)");
data->GetYaxis()->SetTitle("Events/bin");

data->GetXaxis()->SetLabelSize(0);
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

auto legend = new TLegend(0.6,0.6,0.89,0.89);
//legend->SetHeader("Test","C"); // option "C" allows to center the header
legend->AddEntry(data,"data","lep");
legend->AddEntry(th,"th","f");
legend->AddEntry(tth,"tth","f");
legend->AddEntry(ttw,"ttw","f");
legend->AddEntry(ttz,"ttZ","f");
legend->AddEntry(tz,"Rares SM","f");
legend->AddEntry(wz,"WZ","f");
legend->AddEntry(fakes,"Fakes","f");
legend->SetBorderSize(0);
legend->Draw();
pad1->RedrawAxis();
/////////////////////////////////////////////////////////////////
// Draw the ratio plot
TH1F *data1 = (TH1F*)data->Clone("data");
TH1F *sum =(TH1F*)th->Clone("sum");
sum->Add(ttw);
sum->Add(ttz);
sum->Add(tth);
sum->Add(wz);
sum->Add(tz);
sum->Add(fakes);
data1->Divide(sum);
data1->SetLineColor(kBlack);
cs->cd();          // Go back to the main canvas before defining pad2
/////////////////////////////////////////
TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
pad2->SetTopMargin(0.1);
pad2->SetBottomMargin(0.2);
pad2->SetLeftMargin(0.1);
pad2->Draw();
pad2->cd();       // pad2 becomes the current pad
data1->Draw();       // Draw the ratio plot
data1->SetTitle(" ");
///////////////////////////////////
data1->GetXaxis()->SetTitle("KinMVA tHq vs ttV");
data1->GetXaxis()->SetTitleOffset(1.0);
data1->GetXaxis()->SetTitleSize(0.1);
data1->GetXaxis()->SetLabelSize(0.1);
/////////////////////////////////////
data1->GetYaxis()->SetTitle("Data/Pred");
data1->GetYaxis()->SetTitleOffset(0.4);
data1->GetYaxis()->SetTitleSize(0.1);
data1->GetYaxis()->SetLabelSize(0.1);
data1->SetAxisRange(0, 2,"Y");
data1->SetStats(0);
data1->GetYaxis()->SetNdivisions(5);


////////////////////////////
TLine *line = new TLine(-1.0,1,1,1.0);
line->SetLineColor(kBlue);
/////////////////////////////////
line->Draw("same");
cs->SaveAs("kin.png");


cout<<eps_tz <<endl;
cout<<eps_tth <<endl;
cout<<eps_ttw <<endl;
cout<<eps_ttz <<endl;
cout<<eps_wz <<endl;
cout<<eps_fakes <<endl;
cout<<sqrt(pow(0.06,2)+pow(0.05,2)+pow(0.06,2)+2*pow(0.01,2))*th->Integral() <<"th uncertainty"<<endl;
}
