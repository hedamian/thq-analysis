#ifndef MODELINFO_C
#define MODELINFO_C

#include "TFile.h"

#include "RooWorkspace.h"
#include "RooStats/ModelConfig.h"
#include "RooDataSet.h"

#include <iostream>

using namespace std;
using namespace RooFit;
using namespace RooStats;

struct ModelInfo
{
  ModelInfo(string fileName, 
	    string wsName,
	    string mcName,
	    string dataName,
	    string asimovDataName="")
  {
    file = new TFile(fileName.c_str());
    w=(RooWorkspace*)file->Get(wsName.c_str());
    if (!w)
    {
      cout << "Workspace '" << wsName << "' doesn't exist... Trying 'combined'" << endl;
      w = (RooWorkspace*)file->Get("combined");
    }
    if (!w)
    {
      cout << "Workspace 'combined' doesn't exist... Trying 'combWS'" << endl;
      w = (RooWorkspace*)file->Get("combWS");
    }
    if (!w)
    {
      cout << "ERROR::Couldn't open workspace" << endl;
      exit(1);
    }

    data = (RooDataSet*)w->data(dataName.c_str());
    if (!data && dataName == "obsData")
    {
      cout << "Couldn't find data '" << dataName << "'... Trying 'combData'" << endl;
      data = (RooDataSet*)w->data("combData");
    }
    if (!data && dataName == "combData")
    {
      cout << "Couldn't find data '" << dataName << "'... Trying 'obsData'" << endl;
      data = (RooDataSet*)w->data("obsData");
    }

    if (!data && dataName != "")
    {
      cout << "ERROR::Couldn't find data" << endl;
      //exit(1);
    }

    mc = (ModelConfig*)w->obj(mcName.c_str());
    if (!mc && dataName != "")
    {
      cout << "WARNING::Couldn't find ModelConfig" << endl;
      //exit(1);
    }

    if (asimovDataName != "")
    {
      asimovData=(RooDataSet*)w->data(asimovDataName.c_str());
      if (!asimovData) // try this first
      {
	asimovData=(RooDataSet*)w->data("asimovDataFullModel");
      }
      if (!asimovData) 
      {
	cout << "WARNING::Couldn't find asimov data: " << asimovDataName << endl;
      }
    }
    else asimovData=NULL;
    
    if (mc) poi = (RooRealVar*)mc->GetParametersOfInterest()->first();
  }

  ModelInfo()
  {
    file=NULL;
    w=NULL;
    mc=NULL;
    data=NULL;
    poi=NULL;
    asimovData=NULL;
  }

  // ModelInfo(const ModelInfo& other)
  // {
  //   file=other.file;
  //   w=other.w;
  //   mc=other.mc;
  //   data=other.data;
  //   poi=other.poi;
  //   asimovData=other.asimovData;
  // }

  TFile* file;
  RooWorkspace* w;
  ModelConfig* mc;
  RooDataSet* data;
  RooRealVar* poi;
  RooDataSet* asimovData;
  map<string, RooDataSet*> dataMap;
};



#endif