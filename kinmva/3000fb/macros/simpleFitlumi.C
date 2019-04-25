#include "RooWorkspace.h"
#include "RooStats/ModelConfig.h"
#include "RooRealVar.h"
#include "RooMinimizerFcn.h"
#include "RooNLLVar.h"
#include "TFile.h"
#include "minimize.C"
#include <iostream>
#include "eps.h"

using namespace std;
using namespace RooFit;
using namespace RooStats;

void qw(TH1* ht)
{
for (int i=1;i<=16;i++){
ht->SetBinError(i,sqrt(ht->GetBinContent(i)));
}
}

void simpleFitlumi(string version, int strategy=0)
{
  //Open the file, grab the workspace
  TFile* f = new TFile(Form("_combined_kinMVA_model.root",version.c_str(),version.c_str()));

  RooWorkspace* w = (RooWorkspace*)f->Get("combined");
  if (!w)
  {
    cout << "ERROR::Workspace doesn't exist! Check file name" << endl;
    exit(1);
  }
  //Grab the ModelConfig and the data
  ModelConfig* mc = (ModelConfig*)w->obj("ModelConfig");
  RooDataSet* data = (RooDataSet*)w->data("obsData");


  //Configure MINUIT
  ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
  ROOT::Math::MinimizerOptions::SetDefaultStrategy(strategy);
  ROOT::Math::MinimizerOptions::SetDefaultPrintLevel(1);

  //Put the NPs and POIs into one set to send to the NLL
RooArgSet params(*mc->GetNuisanceParameters(),*mc->GetParametersOfInterest());


  //Build the NLL
  RooNLLVar* nll = (RooNLLVar*)mc->GetPdf()->createNLL(*data, Constrain(params),
						       GlobalObservables(*mc->GetGlobalObservables()),
						       RooFit::Offset(1));
////////////////////////////////////////////////////////////////////////////////
RooAbsArg* a1 =params.find("alpha_sample_ttz_sys");
RooAbsArg* a2 =params.find("alpha_sample_fakes_sys");
RooAbsArg* a3 =params.find("alpha_sample_tth_sys");
RooAbsArg* a4 =params.find("alpha_sample_ttw_sys");
RooAbsArg* a5 =params.find("alpha_sample_wz_sys");
RooAbsArg* a6 =params.find("alpha_sample_tz_sys");
RooAbsArg* a7 =params.find("mu");

cout<<a7->ClassName()<<endl;

RooRealVar* alpha_ttz=(RooRealVar*)a1; //ttz
RooRealVar* alpha_fakes=(RooRealVar*)a2; //non prompt /fakes
RooRealVar* alpha_tth=(RooRealVar*)a3; //tth
RooRealVar* alpha_ttw=(RooRealVar*)a4;  //ttw
RooRealVar* alpha_wz=(RooRealVar*)a5; //wz
RooRealVar* alpha_tz=(RooRealVar*)a6;  //tz
RooRealVar* alpha_mu=(RooRealVar*)a7; //signal
//////////////////////////////////////////////////////////////

  //Do the minimization and save the RooFitResult
/// get RooArgSet containing the nuisance parameters (return NULL if not existing)
// const RooArgSet * GetNuisanceParameters() const { return (GetWS()) ? GetWS()->set(fNuisParamsName.c_str()) : 0; }

//Definition in macros/minimize.C:
RooFitResult* minimize(RooAbsReal* fcn, bool save=0, int retry_mode=3, int* ret_status=NULL);

RooFitResult* result = minimize(nll, true, 0);
//////////////////////////////////
////RooPlot



RooAbsPdf* mf= mc->GetPdf();
RooArgSet spdf(*mf->getComponents());
RooHistFunc* th_SR_nominal=(RooHistFunc*)spdf.find("th_SR_nominal");
RooHistFunc* tth_SR_nominal=(RooHistFunc*)spdf.find("tth_SR_nominal");
RooHistFunc* ttz_SR_nominal=(RooHistFunc*)spdf.find("ttz_SR_nominal");
RooHistFunc* ttw_SR_nominal=(RooHistFunc*)spdf.find("ttw_SR_nominal");
RooHistFunc* wz_SR_nominal=(RooHistFunc*)spdf.find("wz_SR_nominal");
RooHistFunc* tz_SR_nominal=(RooHistFunc*)spdf.find("tz_SR_nominal");
RooHistFunc* fakes_SR_nominal=(RooHistFunc*)spdf.find("fakes_SR_nominal");


RooArgSet * parSet = (RooArgSet*)mc->GetObservables();
RooRealVar * x_obs = (RooRealVar*)parSet->find("obs_x_SR"); //variable

RooDataHist sth=th_SR_nominal->dataHist(); //signal thq
RooDataHist stth=tth_SR_nominal->dataHist(); //tth
RooDataHist sttz=ttz_SR_nominal->dataHist(); //ttz
RooDataHist sttw=ttw_SR_nominal->dataHist();//ttw
RooDataHist sfakes=fakes_SR_nominal->dataHist();//fakes
RooDataHist swz=wz_SR_nominal->dataHist();//wz
RooDataHist stz=tz_SR_nominal->dataHist();//tz
/////////////////////////////////////////////////////////////


TH1* data1=data->createHistogram("obs_x_SR",16);
TH1* th=sth.createHistogram("obs_x_SR",16); //th
TH1* tth=stth.createHistogram("obs_x_SR",16);  //tth
TH1* ttz=sttz.createHistogram("obs_x_SR",16);  //ttz
TH1* ttw=sttw.createHistogram("obs_x_SR",16);  ///ttw
TH1* fakes=sfakes.createHistogram("obs_x_SR",16);  ///non prompt/fakes
TH1* wz=swz.createHistogram("obs_x_SR",16); ///wz
TH1* tz=stz.createHistogram("obs_x_SR",16); /// tz


////////////////////////////////////////
tth->SetFillColor(kOrange+7);
th->SetFillColor(kRed+1);
ttz->SetFillColor(kSpring+2);
ttw->SetFillColor(kSpring+3);
wz->SetFillColor(kMagenta-3);
tz->SetFillColor(kCyan);
fakes->SetFillStyle(3005);
fakes->SetFillColor(kBlue-3);
data1->SetMarkerStyle(8);
data1->SetLineColor(kBlack);
///////////////////////////////////////////////
//number of events post fit//

float(i1)=(fakes->Integral())*(1.0+eps_fakes*alpha_fakes->getVal()); //fakes eps=0.4
float(i2)=(tz->Integral())*(1.0+ eps_tz*alpha_tz->getVal());  //tz    eps=0.5
float(i3)=(wz->Integral())*(1.0+ eps_wz*alpha_wz->getVal()); //wz   eps=0.5
float(i4)=(ttz->Integral())*(1.0+ eps_ttz*alpha_ttz->getVal()); //ttz  double  eps_ttz = sqrt(pow(0.04,2)+pow(0.1,2));
float(i5)=(ttw->Integral())*(1.0+ eps_ttw*alpha_ttw->getVal()); //ttw   double eps_ttw= sqrt(pow(0.04,2)+pow(0.12,2));
float(i6)=(th->Integral())*(alpha_mu->getVal());  //signal (kt--1 mu=6.12861e-01)
float(i7)=(tth->Integral())*(1.0+ eps_tth*alpha_tth->getVal()); //tth    eps=0.05
float(id)=data1->Integral(); //data

//cout<<params.InheritsFrom(RooAbsArg::Class())<<endl;
//cout<< result->ClassName()<<endl;

float(efakes)=(fakes->Integral())*(eps_fakes*alpha_fakes->getError()); //fakes eps=0.4
float(etz)=(tz->Integral())*(eps_tz*alpha_tz->getError());  //tz    eps=0.5
float(ewz)=(wz->Integral())*(eps_wz*alpha_wz->getError()); //wz   eps=0.5
float(ettz)=(ttz->Integral())*(eps_ttz*alpha_ttz->getError()); //ttz  double  eps_ttz = sqrt(pow(0.04,2)+pow(0.1,2));
float(ettw)=(ttw->Integral())*(eps_ttw*alpha_ttw->getError()); //ttw   double eps_ttw= sqrt(pow(0.04,2)+pow(0.12,2));
float(eth)=(th->Integral())*(alpha_mu->getError());  //signal (kt--1 mu=6.12861e-01)
float(etth)=(tth->Integral())*(eps_tth*alpha_tth->getVal()); //tth    eps=0.05

/////////////////////////////////////////////////////////////
THStack *hs = new THStack("hs","");
hs->Add(fakes,"hist");
hs->Add(tz,"hist");
hs->Add(wz,"hist");
hs->Add(ttz,"hist");
hs->Add(ttw,"hist");
hs->Add(tth,"hist");
hs->Add(th,"hist");

cout<<"######################################"<<endl;
cout<<"event  "<<"& "<<"N prefit  "<<"  & "<<"N Postfit "<<"\\"<<endl;
cout<<"tH "<<"& "<<th->Integral()<<"  & "<<i6 <<"$\pm$" <<eth<< "\\"<<endl;
cout<<"ttH  "<<"& "<<tth->Integral()<<" & "<<i7<<"$\pm$"<<etth<<"\\"<<endl;
cout<<"ttW  "<<"& "<<ttw->Integral()<<" & " <<i5<<"$\pm$"<<ettw<<"\\"<<endl;
cout<<"ttZ  "<<"& "<<ttz->Integral()<<" & "<<i4<<"$\pm$"<<ettz<<"\\"<<endl;
cout<<"tZ " <<"& "<<tz->Integral()<<" & " <<i2<<"$\pm$"<<etz<<"\\"<<endl;
cout<<"WZ " <<"& "<<wz->Integral()<<" & " <<i3<<"$\pm$"<<ewz<<"\\"<<endl;
cout<<"fakes  " <<"& "<<fakes->Integral()<<"  & "<<i1<<"$\pm$" << efakes<<"\\"<<endl;
cout<<"data "<<"&" <<id<<"&"<<endl;

cout<<"######################################"<<endl;
result->Print();
cout<<"######################################"<<endl;
params.Print();
cout<<"######################################"<<endl;

///////////////////////////////////////////
TCanvas *cs = new TCanvas("cs","cs",100,100,1200,900);
TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
pad1->SetBottomMargin(0.01); // Upper and lower plot are joined
//pad1->SetGridx();         // Vertical grid
pad1->Draw();             // Draw the upper pad: pad1
pad1->cd();               // pad1 becomes the current pad]

data1->Draw();
hs->Draw("same");
data1->Draw("same");
data1->SetStats(0);
data1->SetAxisRange(0,70,"Y");
data1->SetTitle("CMs preliminary Test               35.9f/b (13TeV)");
data1->GetYaxis()->SetTitle("Events/bin");

TLegend* legend1 = new TLegend(0.6,0.6,0.89,0.89);
//legend->SetHeader("Test","C"); // option "C" allows to center the header
legend1->AddEntry(data1,"Data Obs","lep");
legend1->AddEntry(th,"thq (k_{t})","f");
legend1->AddEntry(tth,"tth","f");
legend1->AddEntry(ttw,"ttw","f");
legend1->AddEntry(ttz,"ttz","f");
legend1->AddEntry(wz,"Wz","f");
legend1->AddEntry(tz,"tZ,tttt,VVV,WW","f");
legend1->AddEntry(fakes,"fakes","f");
legend1->SetBorderSize(0);
legend1->Draw();
pad1->RedrawAxis();


// Draw the ratio plot
TH1F *data_obs = (TH1F*)data1->Clone("data_obs");
TH1F *sum =(TH1F*)fakes->Clone("sum");
sum->Add(th);
sum->Add(tth);
sum->Add(ttw);
sum->Add(ttz);
sum->Add(wz);
sum->Add(tz);
data_obs->Divide(sum);
cout<<sum->Integral()<<endl;
data_obs->SetLineColor(kBlack);
data_obs->SetMarkerStyle(8);
cs->cd();          // Go back to the main canvas before defining pad2
/////////////////////////////////////////
TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
pad2->SetTopMargin(0.1);
pad2->SetBottomMargin(0.2);
pad2->SetLeftMargin(0.1);
pad2->Draw();
pad2->cd();       // pad2 becomes the current pad
data_obs->Draw();       // Draw the ratio plot
data_obs->SetTitle(" ");
///////////////////////////////////
data_obs->GetXaxis()->SetTitle("KinMVA tHq vs ttV");
data_obs->GetXaxis()->SetTitleOffset(1.0);
data_obs->GetXaxis()->SetTitleSize(0.1);
data_obs->GetXaxis()->SetLabelSize(0.1);
/////////////////////////////////////
data_obs->GetYaxis()->SetTitle("Data/Pred");
data_obs->GetYaxis()->SetTitleOffset(0.4);
data_obs->GetYaxis()->SetTitleSize(0.1);
data_obs->GetYaxis()->SetLabelSize(0.1);
data_obs->SetAxisRange(0, 4,"Y");
data_obs->SetStats(0);
data_obs->GetYaxis()->SetNdivisions(5);

////////////////////////////
TLine *line = new TLine(-1.0,1,1,1.0);
line->SetLineColor(kRed);
line->Draw("same");

cs->SaveAs("simple.png");
  //Save to file
  system(Form("mkdir -vp root-files/%s",version.c_str()));
  result->SetName("result");
  result->SaveAs(Form("root-files/%s/fit.root",version.c_str()));
}
