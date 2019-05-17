#include "RooWorkspace.h"
#include "RooStats/ModelConfig.h"
#include "RooRealVar.h"
#include "RooMinimizerFcn.h"
#include "RooNLLVar.h"
#include "TFile.h"
#include "minimize.C"
#include <iostream>

using namespace std;
using namespace RooFit;
using namespace RooStats;

void qw(TH1* ht)
{
for (int i=1;i<=10;i++){
ht->SetBinError(i,sqrt(ht->GetBinContent(i)));
}

}

void simpleFit(string version, int strategy=0)
{
  //Open the file, grab the workspace
  TFile* f = new TFile(Form("workspace/%s/tut_combined_%s_model.root",version.c_str(),version.c_str()));

  RooWorkspace* w = (RooWorkspace*)f->Get("combined");
  if (!w)
  {
    cout << "ERROR::Workspace doesn't exist! Check file name" << endl;
    exit(1);
  }
  //Grab the ModelConfig and the data
  ModelConfig* mc = (ModelConfig*)w->obj("ModelConfig");
  RooDataSet* data = (RooDataSet*)w->data("obsData");

  //RooArgSet apdfs=w->allPdfs();
  //RooRealSumPdf* P=(RooRealSumPdf*)apdfs.find("SR_model");
  //P->getComponents()->Print();
  //return;
  //RooArgSet spdf=P->getComponents();
  ///RooAbsPdf* S_SR_nominal=(RooAbsPdf*)spdf.find("S_SR_nominal");
  //RooAbsPdf* B_SR_nominal=(RooAbsPdf*)spdf.find("B_SR_nominal");


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

  //Do the minimization and save the RooFitResult

  //Definition in macros/minimize.C:
  //RooFitResult* minimize(RooAbsReal* fcn, bool save=0, int retry_mode=3, int* ret_status=NULL)
  RooFitResult* result = minimize(nll, true, 0);
//////////////////////////////////
////RooPlot
RooAbsPdf* mf= mc->GetPdf();
RooArgSet spdf(*mf->getComponents());
RooHistFunc* S_SR_nominal=(RooHistFunc*)spdf.find("S_SR_nominal");
RooHistFunc* H_SR_nominal=(RooHistFunc*)spdf.find("H_SR_nominal");
RooHistFunc* B_SR_nominal=(RooHistFunc*)spdf.find("B_SR_nominal");
RooHistFunc* T_SR_nominal=(RooHistFunc*)spdf.find("T_SR_nominal");
RooHistFunc* F_SR_nominal=(RooHistFunc*)spdf.find("F_SR_nominal");
RooHistFunc* W_SR_nominal=(RooHistFunc*)spdf.find("W_SR_nominal");
RooHistFunc* Z_SR_nominal=(RooHistFunc*)spdf.find("Z_SR_nominal");



RooArgSet * parSet = (RooArgSet*)mc->GetObservables();
RooRealVar * x_obs = (RooRealVar*)parSet->find("obs_x_SR"); //variable

RooDataHist sr=S_SR_nominal->dataHist(); //signal thq
RooDataHist sh=H_SR_nominal->dataHist(); //tth
RooDataHist sb=B_SR_nominal->dataHist(); //ttz
RooDataHist st=T_SR_nominal->dataHist();//ttw
RooDataHist sf=F_SR_nominal->dataHist();//fakes
RooDataHist sw=W_SR_nominal->dataHist();//wz
RooDataHist sz=Z_SR_nominal->dataHist();//tz



TH1* data1=data->createHistogram("obs_x_SR",10);
TH1* shh=sh.createHistogram("obs_x_SR",10); //tth
TH1* srh=sr.createHistogram("obs_x_SR",10);
TH1* sbh=sb.createHistogram("obs_x_SR",10);
TH1* sth=st.createHistogram("obs_x_SR",10);
TH1* sfh=sf.createHistogram("obs_x_SR",10);
TH1* swh=sw.createHistogram("obs_x_SR",10);
TH1* szh=sz.createHistogram("obs_x_SR",10);


qw(data1);


srh->SetFillColor(kOrange+7);
shh->SetFillColor(kRed+1);
sbh->SetFillColor(kSpring+2);
sth->SetFillColor(kSpring+3);
swh->SetFillColor(kMagenta-3);
szh->SetFillColor(kCyan);
sfh->SetFillStyle(3005);
sfh->SetFillColor(kBlue-3);
data1->SetMarkerStyle(8);


THStack *hs = new THStack("hs","");
hs->Add(sfh,"hist");
hs->Add(szh,"hist");
hs->Add(swh,"hist");
hs->Add(sbh,"hist");
hs->Add(sth,"hist");
hs->Add(shh,"hist");

hs->Add(srh,"hist");

float(i1)=sfh->Integral();
float(i2)=szh->Integral();
float(i3)=swh->Integral();
float(i4)=sbh->Integral();
float(i5)=sth->Integral();
float(i6)=srh->Integral();
float(i7)=shh->Integral();
float(id)=data1->Integral();


cout<<"fakes"<< i1<<endl;
cout<<"tz" << i2<<endl;
cout<<"wz" << i3<<endl;
cout<<"ttz"  <<i4<<endl;
cout<<"ttw" << i5<<endl;
cout<<"tth"<<i7<<endl;
cout<<"signal"<<  i6<<endl;
cout<<"data   "<<id<<endl;

TCanvas *cs = new TCanvas("cs","cs",100,100,700,900);
data1->Draw();
hs->Draw("same");
data1->Draw("same");
data1->SetStats(0);

TLegend* legend1 = new TLegend(0.6,0.6,0.89,0.89);
//legend->SetHeader("Test","C"); // option "C" allows to center the header
legend1->AddEntry(data1,"Data Obs","lep");
legend1->AddEntry(srh,"th","f");
legend1->AddEntry(shh,"tth","f");
legend1->AddEntry(sth,"ttw","f");
legend1->AddEntry(sbh,"ttz","f");
legend1->AddEntry(swh,"Wz","f");
legend1->AddEntry(szh,"tz","f");
legend1->AddEntry(sfh,"fakes","f");
legend1->SetBorderSize(0);
legend1->Draw();

cs->SaveAs("simple.png");
  //Save to file
  system(Form("mkdir -vp root-files/%s",version.c_str()));
  result->SetName("result");
  result->SaveAs(Form("root-files/%s/fit.root",version.c_str()));
}
