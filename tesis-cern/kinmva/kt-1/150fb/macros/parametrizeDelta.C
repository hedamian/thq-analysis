#include "TFile.h"
#include "RooWorkspace.h"
#include "RooStats/ModelConfig.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooAddition.h"
#include "RooSimultaneous.h"
#include "RooCategory.h"

#include "macros/minimize.C"
#include "macros/ModelInfo.C"

#include <sstream>
#include <string>
#include <iostream>

using namespace std;
using namespace RooFit;
using namespace RooStats;

/*
  This macro will be used to reparametrize POIs in a workspace, in preparation for having
  parameters that represent the relative difference in measurements between two channels:

  Delta_POI_i = (POI_i_chan1 - POI_i_chan2) / POI_i_chan1

  'chan1' will be assumed to be the one that you're reparametrizing now.
  As usual, invert the expression above to right the old parameters in terms of the new:

  => POI_i_chan1 = POI_i_chan2 / (1 - Delta_POI_i)
  
  The logic of this reparametrization is a bit easier and more general than the shape one.
 */

void parametrizeDelta(string version)
{
  //Open the file and grab the objects out
  ModelInfo info(Form("workspaces/%s/tut_combined_%s_model.root",version.c_str(),version.c_str()), 
		 "combined","ModelConfig","obsData");
  
  RooWorkspace* w = info.w;
  ModelConfig* mc = info.mc;
  RooDataSet* data = info.data;
  RooAbsPdf* pdf = mc->GetPdf();
  if (!pdf)
  {
    cout << "ERROR::ModelConfig doesn't have PDF set!" << endl;
    exit(1);
  }
  string pdfName = pdf->GetName();
  
  //Loop over the POIs, for each create new delta parameters, and build the edit string
  //Reminder: The syntax for editing an object in the workspace is the following
  //EDIT::objName(objName,origParam1=newExpression1,origParam2=newExpression2,...,origParamN=newExpressionN)
  //objName is what you want to edit, origParamX is the original parameter being replaced,
  //and newExpressionX is the new expression it will be replaced with. Let's build a string we'll use to call this.

  stringstream editStr;
  editStr << "EDIT::" << pdfName << "(" << pdfName;
  TIterator* pitr = mc->GetParametersOfInterest()->createIterator();
  RooRealVar* var;
  pitr->Reset();
  RooArgSet pois;
  while ((var = (RooRealVar*)pitr->Next())) {
    //Create the new parameters and import into the workspace
    RooRealVar* delta = new RooRealVar(Form("Delta_%s",var->GetName()),Form("Delta_%s",var->GetName()),0,-10,10);

    //Create the RooFormulaVar to replace the old parameter
    //Remember the parametrization:
    //POI_i_chan1 = POI_i_chan2 / (1 - Delta_POI_i)
    //NOTE: The POI in this workspace is used as a proxy for the POI for the channel
    //in the workspace we'll combine it with.
    RooArgList formList;
    string formString = Form("@0/(1-@1)");
    formList.add(*var); // this is the proxy
    formList.add(*delta);

    RooFormulaVar* form = new RooFormulaVar(Form("%s_form",var->GetName()),formString.c_str(),formList);
    w->import(*form);
    pois.add(*w->var(delta->GetName())); // Let's add it to the new POI set while we're in the loop
    
    editStr << "," << var->GetName() << "=" << form->GetName();
  }
  editStr << ")";

  //Print the edit string for verification
  cout << "Printing editStr" << endl;
  cout << "editStr " << editStr.str() << endl;

  //Edit the pdf
  w->factory(editStr.str().c_str());

  //Print the POIs for verification
  pois.Print("v");

  //Grab the edited PDF
  RooSimultaneous* new_simPdf = (RooSimultaneous*)w->pdf(pdfName.c_str());

  //Put everything into a new workspace with a new ModelConfig
  RooWorkspace* w_new = new RooWorkspace("combined");
  ModelConfig* mc_new = new ModelConfig("ModelConfig",w_new);
  mc_new->SetPdf(*new_simPdf);
  mc_new->SetParametersOfInterest(pois);
  mc_new->SetNuisanceParameters(*mc->GetNuisanceParameters());
  mc_new->SetGlobalObservables(*mc->GetGlobalObservables());
  mc_new->SetObservables(*mc->GetObservables());
  w_new->import(*mc_new);
  w_new->import(*data);
  w = w_new;
  mc = mc_new;

  system(Form("mkdir -vp workspaces/%s_Delta",version.c_str()));
  w->writeToFile(Form("workspaces/%s_Delta/tut_combined_%s_Delta_model.root",version.c_str(),version.c_str()),true);
}