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


  //Save to file
  system(Form("mkdir -vp root-files/%s",version.c_str()));
  result->SetName("result");
  result->SaveAs(Form("root-files/%s/fit.root",version.c_str()));
}
