#include "RooStats/HistFactory/MakeModelAndMeasurementsFast.h"
#include "TH1D.h"



void OnOff()
{
//define event counts

//in SR
  double S1 = 5.3; // signal in bin 1
  double B1 = 9.2; // background in bin 1
  double O1 = 20; // observed in bin 1

//in CR
  double S2 = 0.5; // signal in bin 2
  double B2 = 53; // background in bin 2
  double O2 = 46; // observed in bin 2

//define relative systematic on S and B in the SR
  double eps_S = 0.2;
  double eps_B = 0.1;


//--------------------


//make and fill histos
  TH1D* h_S1 = new TH1D("S1","S1",1,0,1);
  TH1D* h_B1 = new TH1D("B1","B1",1,0,1);
  TH1D* h_O1 = new TH1D("O1","O1",1,0,1);

  TH1D* h_S2 = new TH1D("S2","S2",1,0,1);
  TH1D* h_B2 = new TH1D("B2","B2",1,0,1);
  TH1D* h_O2 = new TH1D("O2","O2",1,0,1);

  h_S1->SetBinContent(1,S1);
  h_B1->SetBinContent(1,B1);
  h_O1->SetBinContent(1,O1);

  h_S2->SetBinContent(1,S2);
  h_B2->SetBinContent(1,B2);
  h_O2->SetBinContent(1,O2);


//--------------------

//create the histfactory model
  RooStats::HistFactory::Measurement meas("OnOff", "OnOff");
  meas.SetOutputFilePrefix("ex");
  meas.SetExportOnly(1);
  meas.SetPOI("mu");
  meas.SetLumi(1.0);
  meas.SetLumiRelErr(0.02);


//create the parameters

  RooStats::HistFactory::NormFactor normS;
  normS.SetName("mu");
  normS.SetHigh(100);
  normS.SetLow(0);
  normS.SetVal(1);

  RooStats::HistFactory::NormFactor normB;
  normB.SetName("norm_B");
  normB.SetVal(1);

//create the SR
  RooStats::HistFactory::Channel SR("SR");
  SR.SetData(h_O1);

//add the samples
  RooStats::HistFactory::Sample s_S1("S");
  s_S1.SetHisto(h_S1);
  s_S1.AddNormFactor(normS);
  s_S1.AddOverallSys("sys_S",1./(1+eps_S),(1+eps_S));
  SR.AddSample(s_S1);

  RooStats::HistFactory::Sample s_B1("B");
  s_B1.SetHisto(h_B1);
  s_B1.AddNormFactor(normB);
  s_B1.AddOverallSys("sys_B",1./(1+eps_B),(1+eps_B));
  SR.AddSample(s_B1);

  meas.AddChannel(SR);

//create the CR
  RooStats::HistFactory::Channel CR("CR");
  CR.SetData(h_O2);

//add the samples
  RooStats::HistFactory::Sample s_S2("S");
  s_S2.SetHisto(h_S2);
  s_S2.AddNormFactor(normS);
  CR.AddSample(s_S2);

  RooStats::HistFactory::Sample s_B2("B");
  s_B2.SetHisto(h_B2);
  s_B2.AddNormFactor(normB);
  CR.AddSample(s_B2);

  meas.AddChannel(CR);

//--------------------

//make the workspace
  RooStats::HistFactory::MakeModelAndMeasurementFast(meas);
}
