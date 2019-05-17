#include "RooMultiVarGaussian.h"
#include "RooStats/ModelConfig.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TMatrixDSym.h"

using namespace std;
using namespace RooFit;
using namespace RooStats;

void MultiVarGaussian(string version)
{
//Open the covariance matrix of the specified version
  TFile f(Form("root-files/%s/cov.root",version.c_str()));
  TH2D* cov = (TH2D*)f.Get("cov");
  TH1D* cen = (TH1D*)f.Get("cen");

  int nrPars = cen->GetNbinsX();

  //Convert it into a TMatrixDSym
  TMatrixDSym covMat(nrPars);  
  for (int i=0;i<nrPars;i++)
  {
    for (int j=0;j<nrPars;j++)
    {
      covMat[i][j] = cov->GetBinContent(i+1,j+1);
    }
  }

  //Make a RooArgList of the parameters, and also one of their central value
  RooArgList xvec, muvec;
  for (int i=0;i<nrPars;i++)
  {
    double bestFit = cen->GetBinContent(i+1);
    RooRealVar* x = new RooRealVar(cen->GetXaxis()->GetBinLabel(i+1),
				   cen->GetXaxis()->GetBinLabel(i+1),
				   bestFit,0,bestFit*10);

    RooRealVar* mu = new RooRealVar(Form("mu_%s",cen->GetXaxis()->GetBinLabel(i+1)),
				    Form("mu_%s",cen->GetXaxis()->GetBinLabel(i+1)),
				    bestFit,0,bestFit*10);
    xvec.add(*x);
    muvec.add(*mu);
  }

  //Make the MVG
  RooMultiVarGaussian* mvg = new RooMultiVarGaussian(Form("pdf_%s",version.c_str()),Form("pdf_%s",version.c_str()),
						     xvec, muvec, covMat);


  //Create the "data", which are just the best fit values
  RooDataSet* data = new RooDataSet("obsData","obsData",muvec);
  data->add(muvec);

  //Create a workspace that we'll import this to
  RooWorkspace* w_out = new RooWorkspace("combined");

  //Create the ModelConfig
  RooArgSet nuis, globs; // these are empty, but we should set them to define the sets in ModelConfig
  ModelConfig mc("ModelConfig",w_out);
  mc.SetPdf(*mvg);
  mc.SetParametersOfInterest(xvec);
  mc.SetObservables(muvec);
  mc.SetNuisanceParameters(nuis);
  mc.SetGlobalObservables(globs);

  //Import the MC and the data into the workspace
  w_out->import(mc);
  w_out->import(*data);

  //Write the workspace to file
  system(Form("mkdir -vp workspaces/%s_MVG/",version.c_str()));
  w_out->writeToFile(Form("workspaces/%s_MVG/tut_combined_%s_MVG_model.root",version.c_str(),version.c_str()));
}