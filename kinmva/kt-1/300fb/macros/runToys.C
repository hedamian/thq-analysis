#include "TFile.h"
#include "TTree.h"

#include "RooNLLVar.h"
#include "RooMinimizer.h"
#include "RooStats/ToyMCSampler.h"
#include "RooRandom.h"
#include "RooStats/RooStatsUtils.h"

#include "macros/minimize.C"
#include "macros/ModelInfo.C"

#include <sstream>
#include <stdlib.h>
#include <vector>
#include <set>
#include <list>
#include <map>

using namespace std;
using namespace RooFit;
using namespace RooStats;

void runToys(string version="OnOff", string parName="mu", 
	     double testVal=2.3042, double genVal=3.3042,
	     int nrToys=1000, int seed=1)
{
  //Open the file and grab the objects out
  ModelInfo info(Form("workspaces/%s/tut_combined_%s_model.root",version.c_str(),version.c_str()), 
		 "combined","ModelConfig","obsData");
  
  RooWorkspace* w = info.w;
  ModelConfig* mc = info.mc;
  RooDataSet* data = info.data;
  RooAbsPdf* pdf = mc->GetPdf();
  RooRealVar* par = info.w->var(parName.c_str());
  if (!par)
  {
    cout << "ERROR::Parameter " << parName << " doesn't exist!" << endl;
    exit(1);
  }

  //Configure MINUIT
  ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
  ROOT::Math::MinimizerOptions::SetDefaultStrategy(0);
  ROOT::Math::MinimizerOptions::SetDefaultPrintLevel(-1); // suppress this a bit so that it doesn't print at each toy
  RooRandom::randomGenerator()->SetSeed(seed);

  //Define the sets we'll feed to the toy sampler
  RooArgSet pois(*mc->GetParametersOfInterest());
  RooArgSet nuis(*mc->GetNuisanceParameters());
  RooArgSet globs(*mc->GetGlobalObservables());
  RooArgSet obs(*mc->GetObservables());
  RooArgSet allParams(*mc->GetPdf()->getParameters(*data));
  RooStats::RemoveConstantParameters(&allParams);
  RooArgSet all(allParams,globs);
  

  RooNLLVar* nll = (RooNLLVar*)pdf->createNLL(*data, Constrain(allParams), GlobalObservables(globs), RooFit::Offset(1));

  //Do a conditional fit to the data to condition the NPs
  par->setRange(-10, 10);
  par->setVal(genVal);
  par->setConstant(1);
  minimize(nll);

  //Save snapshots of the parameters and global observables so that we can load them before each toy
  w->saveSnapshot("genParams",allParams);
  w->saveSnapshot("nominalGlobs",globs);

  //Setup the ToyMCSampler
  ToyMCSampler* sampler = new ToyMCSampler();
  sampler->SetNuisanceParameters(nuis);
  sampler->SetObservables(obs);
  if (globs.getSize() > 0) sampler->SetGlobalObservables(globs);
  sampler->SetPdf(*pdf);


//setup TTree to save results
  system(Form("mkdir -vp root-files/%s/toys",version.c_str()));
  string outFileName(Form("root-files/%s/toys/toys.root",version.c_str()));
  TFile* file = new TFile(outFileName.c_str(),"recreate");
  TTree* tree = new TTree("params","Distribution of fit parameters");
  map<string, double> map_params;
  TIterator* itr = allParams.createIterator();
  RooRealVar* var;
  while ((var=(RooRealVar*)itr->Next()))
  {
    string varName(var->GetName());
    map_params[varName] = 0;
    tree->Branch(varName.c_str(),&map_params[varName]);
    
    string varNameGen = varName+"_gen";
    map_params[varNameGen] = w->var(varName.c_str())->getVal();
    tree->Branch(varNameGen.c_str(),&map_params[varNameGen]);
  }
  double qmu=0;
  tree->Branch("qmu",&qmu);


  //Loop and throw N toys
  for (int i=0;i<nrToys;i++)
  {
    w->loadSnapshot("nominalGlobs");
    w->loadSnapshot("genParams");

    if ((i % 100) == 0) cout << "Generating toy: " << i << " / " << nrToys << endl;
    RooAbsData* toyData = sampler->GenerateToyData(all); // this also randomizes the global observables
    nll->setData(*toyData, kFALSE);
    //minimize(nll); // actually i don't know why this is needed, but the nll offset changes from the first to second fit without it

    //Evaluate qmu = -2*log(L(par_test) / L(par_hat))

    //first -log L(par_hat)
    par->setConstant(0);
    minimize(nll);
    double nll_hat = nll->getVal();

    //and let's just save the parameter values here
    // itr->Reset();
    // while ((var=(RooRealVar*)itr->Next()))
    // {
    //   string varName(var->GetName());
    //   map_params[varName] = var->getVal();
    // }
    // tree->Fill();

    //then -log L(par_test)
    par->setConstant(1);
    par->setVal(testVal);
    minimize(nll);
    double nll_test = nll->getVal();

    qmu = 2*(nll_test - nll_hat);
    cout << "Qmu = " << qmu << endl;


    delete toyData;
  }

  file->Write();
  file->Close();
}