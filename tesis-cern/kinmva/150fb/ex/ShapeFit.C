#include "RooStats/HistFactory/MakeModelAndMeasurementsFast.h"
#include "TH1D.h"



void ShapeFit()
{
  //Histos were already made by ShapeFit_makeHists.C. Let's open them up and use them
  TFile* file = new TFile("hists/ShapeFit.root");
  //signal
  TH1D* h_sig = (TH1D*)file->Get("Signal");
  TH1D* h_sig_scale_down = (TH1D*)file->Get("Signal_scale_down");
  TH1D* h_sig_scale_up = (TH1D*)file->Get("Signal_scale_up");
  //background
  TH1D* h_bkg = (TH1D*)file->Get("Background");
  TH1D* h_bkg_slope_down = (TH1D*)file->Get("Background_slope_down");
  TH1D* h_bkg_slope_up = (TH1D*)file->Get("Background_slope_up");
  //data
  TH1D* h_obs = (TH1D*)file->Get("Data");

//--------------------

//create the histfactory model
  RooStats::HistFactory::Measurement meas("ShapeFit", "ShapeFit"); 
  meas.SetOutputFilePrefix("workspaces/ShapeFit/tut");
  meas.SetExportOnly(1);
  meas.SetPOI("mu");

  // this scales the histogram content, which already includes lumi, so set to 1
  meas.SetLumi(1.0);
  meas.SetLumiRelErr(0.02);


//create the parameters for the model

  //first the signal normalization
  RooStats::HistFactory::NormFactor normS;
  normS.SetName("mu");
  normS.SetHigh(100); // maximum value it can take
  normS.SetLow(0); // minimum value it can take
  normS.SetVal(1); // nominal value

//create the SR
  RooStats::HistFactory::Channel SR("SR_shape");
  SR.SetData(h_obs);

//add the signal and background samples
  RooStats::HistFactory::Sample sample_S("S_shape");
  sample_S.SetHisto(h_sig);
  sample_S.AddNormFactor(normS);
//add the shape uncertainty for the signal
  RooStats::HistFactory::HistoSys shapeSys_S("Scale");
  shapeSys_S.SetHistoHigh(h_sig_scale_up);
  shapeSys_S.SetHistoLow(h_sig_scale_down);
  sample_S.AddHistoSys(shapeSys_S);
//let's also add a normalization systematic
  sample_S.AddOverallSys("sys_S",1./1.05,1.05);

  SR.AddSample(sample_S);

  RooStats::HistFactory::Sample sample_B("B_shape");
  sample_B.SetHisto(h_bkg);
//add the shape uncertainty for the signal
  RooStats::HistFactory::HistoSys shapeSys_B("Slope");
  shapeSys_B.SetHistoHigh(h_bkg_slope_up);
  shapeSys_B.SetHistoLow(h_bkg_slope_down);
  sample_B.AddHistoSys(shapeSys_B);
//also add a normalization uncertainty
  sample_B.AddOverallSys("sys_B",1./1.15,1.15);
  SR.AddSample(sample_B);

//add the single region to the measurement
  meas.AddChannel(SR);

//make the workspace
  RooStats::HistFactory::MakeModelAndMeasurementFast(meas);
}