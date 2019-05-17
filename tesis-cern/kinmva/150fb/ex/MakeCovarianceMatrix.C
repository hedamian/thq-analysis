#include "RooFitResult.h"

#include <iostream>

using namespace std;
using namespace RooFit;

void MakeCovarianceMatrix(string version)
{
  //Open the RooFitResult of the specified version
  TFile f(Form("root-files/%s/fit.root",version.c_str()));
  RooFitResult* rfr = (RooFitResult*)f.Get("result");
  
  //Grab the covariance matrix
  TMatrixDSym covMat = rfr->covarianceMatrix();
  RooArgList pars = rfr->floatParsFinal();

  //Fill the histogram. That's all!
  TFile* file = new TFile(Form("root-files/%s/cov.root",version.c_str()),"recreate");
  TH2D* covHist = new TH2D("cov","cov",pars.getSize(),0,pars.getSize(),
			   pars.getSize(),0,pars.getSize());

  //Also save a histogram of the central values
  TH1D* cen = new TH1D("cen","cen",pars.getSize(),0,pars.getSize());
  for (int i=0;i<pars.getSize();i++)
  {
    covHist->GetXaxis()->SetBinLabel(i+1,pars.at(i)->GetName());
    covHist->GetYaxis()->SetBinLabel(i+1,pars.at(i)->GetName());
    for (int j=0;j<pars.getSize();j++)
    {
      covHist->SetBinContent(i+1,j+1,covMat[i][j]);
    }
    cen->GetXaxis()->SetBinLabel(i+1,pars.at(i)->GetName());
    cen->SetBinContent(i+1,((RooRealVar*)pars.at(i))->getVal());
  }

  
  //Write to file
  file->Write();

  covHist->SetStats(0);
  covHist->Draw("colz");
}