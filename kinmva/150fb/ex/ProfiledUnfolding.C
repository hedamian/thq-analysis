#include "RooStats/HistFactory/MakeModelAndMeasurementsFast.h"
#include "TRandom3.h"
#include "TH1D.h"



void ProfiledUnfolding(int nrBins = 12)
{
  //Histos were already made by ProfiledUnfolding_makeHists.C. Let's open them up and use them
  TFile* file = new TFile("hists/ProfiledUnfolding.root");

  //Truth distribution
  TH1D* h_yZ_truth = (TH1D*)file->Get("yZ_truth");

  //Reco distribution: This will be used as pseudo-data
  TH1D* h_yZ_reco = (TH1D*)file->Get("yZ_reco");
  
  //Get the acceptance x migration templates
  TH1D** h_yZ_reco_i = new TH1D*[nrBins];
  for (int i=0;i<nrBins;i++) h_yZ_reco_i[i] = (TH1D*)file->Get(Form("yZ_reco_%i",i));
  
  //Get the non-fid Z
  TH1D* h_yZ_reco_nonFid = (TH1D*)file->Get("yZ_reco_nonFid");


  TRandom3 rndm(1);

//--------------------

//create the histfactory model
  RooStats::HistFactory::Measurement meas("ProfiledUnfolding", "ProfiledUnfolding"); 
  meas.SetOutputFilePrefix("workspaces/ProfiledUnfolding/tut");
  meas.SetExportOnly(1);

  //This scales the histogram content, which already includes lumi, so set to 1
  meas.SetLumi(1.0);
  meas.SetLumiRelErr(0.02);


//create the SR
  RooStats::HistFactory::Channel chan_ee("ee_yZ");
  chan_ee.SetData(h_yZ_reco);

  //Create one cross-section parameter for each truth yZ bin
  for (int i=0;i<nrBins;i++)
  {
    RooStats::HistFactory::NormFactor yZ_XS;
    yZ_XS.SetName(Form("Normalization_yZ_%i",i));
    yZ_XS.SetHigh(h_yZ_truth->GetBinContent(i+1)*5); // maximum value it can take
    yZ_XS.SetLow(0); // minimum value it can take
    yZ_XS.SetVal(h_yZ_truth->GetBinContent(i+1)*rndm.Gaus(1,0.05)); // nominal value (randomize the initial value a bit)
    meas.AddPOI(Form("Normalization_yZ_%i",i));

//add the signal samples
    RooStats::HistFactory::Sample sample(Form("yZ_acceptance_template_%i",i));
    sample.SetNormalizeByTheory(false);
    sample.SetHisto(h_yZ_reco_i[i]);
    sample.AddNormFactor(yZ_XS);
    chan_ee.AddSample(sample);
  }

  RooStats::HistFactory::Sample sample_nonFid("ZnonFid");
  sample_nonFid.SetHisto(h_yZ_reco_nonFid);
  chan_ee.AddSample(sample_nonFid);

//add the single region to the measurement
  meas.AddChannel(chan_ee);

//make the workspace
  RooStats::HistFactory::MakeModelAndMeasurementFast(meas);
}