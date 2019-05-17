#include "RooStats/HistFactory/MakeModelAndMeasurementsFast.h"
#include "TRandom3.h"
#include "TH1D.h"


TH1D* getUniformBinnedHist(TH1D* h_orig);
void ProfiledUnfolding2(int binRatio = 1)
{
  //Histos were already made by ProfiledUnfolding_makeHists.C. Let's open them up and use them
  TFile* file = new TFile(Form("hists/ProfiledUnfolding2_%i.root",binRatio));

  //Truth distribution
  TH1D* h_mZ_truth = (TH1D*)file->Get("mZ_truth");

  //Reco distribution: This will be used as pseudo-data
  TH1D* h_mZ_reco = (TH1D*)file->Get("mZ_reco");

  //Get the acceptance x migration templates
  int nrBins_truth = 12;
  TH1D** h_mZ_reco_i = new TH1D*[nrBins_truth];
  for (int i=0;i<nrBins_truth;i++) h_mZ_reco_i[i] = (TH1D*)file->Get(Form("mZ_reco_%i",i));
  
  //Get the non-fid Z
  TH1D* h_mZ_reco_nonFid = (TH1D*)file->Get("mZ_reco_nonFid");


  TRandom3 rndm(1);

//--------------------

//create the histfactory model
  RooStats::HistFactory::Measurement meas(Form("ProfiledUnfolding2_%i",binRatio), Form("ProfiledUnfolding2_%i",binRatio)); 
  meas.SetOutputFilePrefix(Form("workspaces/ProfiledUnfolding2_%i/tut",binRatio));
  meas.SetExportOnly(1);

  //This scales the histogram content, which already includes lumi, so set to 1
  meas.SetLumi(1.0);
  meas.SetLumiRelErr(0.02);


//create the SR
  RooStats::HistFactory::Channel chan_ee("ee_mZ");
  chan_ee.SetData(getUniformBinnedHist(h_mZ_reco));

  //Create one cross-section parameter for each truth mZ bin
  for (int i=0;i<nrBins_truth;i++)
  {
    RooStats::HistFactory::NormFactor mZ_XS;
    mZ_XS.SetName(Form("Normalization_mZ_%i",i));
    mZ_XS.SetHigh(h_mZ_truth->GetBinContent(i+1)*5); // maximum value it can take
    mZ_XS.SetLow(0); // minimum value it can take
    mZ_XS.SetVal(h_mZ_truth->GetBinContent(i+1)*rndm.Gaus(1,0.05)); // nominal value (randomize the initial value a bit)
    meas.AddPOI(Form("Normalization_mZ_%i",i));

//add the signal samples
    RooStats::HistFactory::Sample sample(Form("mZ_acceptance_template_%i",i));
    //sample.SetNormalizeByTheory(false)
    sample.SetHisto(getUniformBinnedHist(h_mZ_reco_i[i]));
    sample.AddNormFactor(mZ_XS);
    chan_ee.AddSample(sample);
  }

  RooStats::HistFactory::Sample sample_nonFid("ZnonFid");
  sample_nonFid.SetHisto(getUniformBinnedHist(h_mZ_reco_nonFid));
  chan_ee.AddSample(sample_nonFid);

//add the single region to the measurement
  meas.AddChannel(chan_ee);

//make the workspace
  RooStats::HistFactory::MakeModelAndMeasurementFast(meas);
}


TH1D* getUniformBinnedHist(TH1D* h_orig)
{
  int nrBins = h_orig->GetNbinsX();
  TH1D* h_ret = new TH1D(Form("%s_uniform",h_orig->GetName()),Form("%s, uniform",h_orig->GetTitle()),
			 nrBins,0,nrBins);
  for (int i=0;i<nrBins;i++)
  {
    h_ret->SetBinContent(i+1,h_orig->GetBinContent(i+1));
    h_ret->SetBinError(i+1,h_orig->GetBinError(i+1));
  }
  return h_ret;
}