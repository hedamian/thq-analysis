#include "RooWorkspace.h"
#include "RooStats/ModelConfig.h"
#include "RooSimultaneous.h"
#include "RooAbsData.h"
#include "RooDataSet.h"
#include "RooSimultaneous.h"
#include "RooCategory.h"

#include "TFile.h"

#include "parseString.C"
#include "minimize.C"

#include <vector>
#include <iostream>
#include <sstream>

using namespace std;
using namespace RooFit;
using namespace RooStats;

//Combine multiple versions into one workspace.
//'versions' syntax is "version1,version2,...,versionN"
//outVersion is the version tag of the combined workspace to be written out
void combine(string versions, string outVersion)
{
  //Parse the version string into a comma-separated list of channels
  vector<string> channels = parseString(versions,",");

  //Define the workspace, MC, and sets to be written out
  RooWorkspace* out_ws = new RooWorkspace("combined");
  ModelConfig* out_mc = new ModelConfig("ModelConfig",out_ws);
  RooArgSet obs,nuis,globs,pois;

  //Loop over the channels and add everything to a new RooSimultaneous pdf
  map<string, RooDataSet*> dataMap;
  RooSimultaneous* baseSim = NULL;
  RooRealVar* weightVar = NULL;
  RooCategory* cat = NULL;
  for (int i=0;i<(int)channels.size();i++)
  {
    //Open the file and grab the objects out
    cout << "Processing channel: " << channels[i] << endl;
    TFile* file = new TFile(Form("./workspace/%s/tut_combined_%s_model.root",channels[i].c_str(),channels[i].c_str()));
    RooWorkspace* combined = (RooWorkspace*)file->Get("combined");
    ModelConfig* mc = (ModelConfig*)combined->obj("ModelConfig");

    //Grab the PDF
    RooSimultaneous* simPdf = (RooSimultaneous*)mc->GetPdf();
    RooCategory* this_cat = (RooCategory*)&simPdf->indexCat();

    //Add the PDFs to the main pdf (or assign the main pdf to the current one if it's not defined)
    if (!baseSim)
    {
      baseSim=simPdf;
      weightVar = combined->var("weightVar");
      cat = (RooCategory*)&simPdf->indexCat();
    }
    else
    {
      RooCatType* tt;
      TIterator* citr = this_cat->typeIterator();
      while ((tt=(RooCatType*)citr->Next()))
      {
	RooAbsPdf* pdf = simPdf->getPdf(tt->GetName());
	baseSim->addPdf(*pdf,tt->GetName());
      }
      delete citr;
    }

    //Add the old sets to the main one
    obs.add(*mc->GetObservables(),true);
    nuis.add(*mc->GetNuisanceParameters(),true);
    globs.add(*mc->GetGlobalObservables(),true);
    pois.add(*mc->GetParametersOfInterest(),true);

    //Add the old data to the new one
    RooDataSet* data = (RooDataSet*)combined->data("obsData");
    TList* datalist = data->split(*this_cat, true);
    TIterator* dataItr = datalist->MakeIterator();
    RooAbsData* ds;
    while ((ds = (RooAbsData*)dataItr->Next()))
    {
      string typeName(ds->GetName());
      cout << "Adding dataset to map: " << ds->GetName() << endl;
      dataMap[string(ds->GetName())] = (RooDataSet*)ds;
    }
  }

  //Make the combined data
  RooDataSet* data = new RooDataSet("obsData","obsData",RooArgSet(obs,*(RooAbsArg*)weightVar),Index(*cat),Import(dataMap),WeightVar(*weightVar));
  data->Print();

  //Import everything to the workspace
  out_ws->import(*baseSim,Silence());
  out_ws->import(*data);

  obs.sort();
  nuis.sort();
  globs.sort();
  pois.sort();

  //Set everything in the MC and add to the workspace
  out_mc->SetPdf(*baseSim);
  out_mc->SetObservables(obs);
  out_mc->SetNuisanceParameters(nuis);
  out_mc->SetGlobalObservables(globs);
  out_mc->SetParametersOfInterest(pois);
  out_ws->import(*out_mc);

  //Write the workspace out
  system(Form("mkdir -vp workspaces/%s",outVersion.c_str()));
  out_ws->writeToFile(Form("workspaces/%s/tut_combined_%s_model.root",outVersion.c_str(),outVersion.c_str()),true);
}
