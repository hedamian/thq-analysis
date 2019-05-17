#include "RooStats/HistFactory/MakeModelAndMeasurementsFast.h"
#include "TH1D.h"



void SimpleNumberCounting()
{
//define event counts
//in the signal region
  double S = 5.3; // signal
  double B = 9.2; // background
  double O = 20; // observed N


//--------------------


//make and fill histos
  TH1D* h_S = new TH1D("S","S",1,0,1);
  TH1D* h_B = new TH1D("B","B",1,0,1);
  TH1D* h_O = new TH1D("O","O",1,0,1);

  h_S->SetBinContent(1,S);
  h_B->SetBinContent(1,B);
  h_O->SetBinContent(1,O);

//--------------------

//create the histfactory model
  RooStats::HistFactory::Measurement meas("SimpleNumberCounting", "SimpleNumberCounting");
  meas.SetOutputFilePrefix("./workspace/SimpleNumberCounting/tut");
  meas.SetExportOnly(1);
  meas.SetPOI("mu");

  // this scales the histogram content, which already includes lumi, so set to 1
  meas.SetLumi(1.0);
  meas.SetLumiRelErr(0.018);


//create the parameters for the model

  //first the signal normalization
  RooStats::HistFactory::NormFactor normS;
  normS.SetName("mu");
  normS.SetHigh(100); // maximum value it can take
  normS.SetLow(0); // minimum value it can take
  normS.SetVal(1); // nominal value

//create the SR
  RooStats::HistFactory::Channel SR("SR");
  SR.SetData(h_O);

//add the signal and background samples
  RooStats::HistFactory::Sample sample_S("S");
  sample_S.SetHisto(h_S);
  sample_S.AddNormFactor(normS);
  SR.AddSample(sample_S);

  RooStats::HistFactory::Sample sample_B("B");
  sample_B.SetHisto(h_B);
  SR.AddSample(sample_B);

//add the single region to the measurement
  meas.AddChannel(SR);

//make the workspace
  RooStats::HistFactory::MakeModelAndMeasurementFast(meas);
}
