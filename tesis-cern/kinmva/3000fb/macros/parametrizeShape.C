#include "TFile.h"
#include "RooWorkspace.h"
#include "RooStats/ModelConfig.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooAddition.h"
#include "RooSimultaneous.h"
#include "RooCategory.h"

#include "minimize.C"
#include "ModelInfo.C"

#include <sstream>
#include <string>
#include <iostream>

using namespace std;
using namespace RooFit;
using namespace RooStats;

/*
  This macro will be used to reparametrize the likelihood from a absolute differential
  measurement to a shape measurement, ie go from dsigma/dy to 1/sigma * dsigma/dy

  (Note: The notation below is for yZ, but the macro lets you also do this for the mZ workspace)

  In the workspace there are parameters Normalization_y_X, with X = {0..N-1}.
  We want new parameters Shape_y_X = Normalisation_y_X / Sigma_tot,
  where Sigma_tot = sum_X Normalization_y_X is the integrated cross-section

  To reparametrize likelihoods in RooFit, you generally need to invert the equations above
  to write the old parameters in terms of the new. In this case, we'll write

  Normalization_y_X = Shape_y_X * Sigma_tot, for X = {1..N-1}, and
  Normalization_y_0 = Sigma_tot - sum_{X!=0} Normalization_y_X
                    = Sigma_tot * (1 - sum_{X!=0} Shape_y_X)

  This means that there's actually no parameter Shape_y_0 ! This is necessary to preserve
  the number of free parameters.



  A small trick can be implemented to get around this,
  in case you want to measure directly Shape_y_0. Instead introduce an auxiliary constant 'a',
  and change the parametrization to:

  Normalization_y_0 = b * Shape_y_0 * Sigma_tot
                      + (1-b) * Sigma_tot (1 - sum_{X!=0} Shape_y_X)
  Normalization_y_1 = (1-b) * Shape_y_1 * Sigma_tot
                      + b * Sigma_tot * (1 - sum_{X!=1} Shape_y_X)
		    = Sigma_tot * [ (1-b) * Shape_y_1 + b * (1 - sum_{X!=1} Shape_y_X) ]
  Normalization_y_X = Shape_y_X * Sigma_tot, for X = {2..N-1}, and

  When b=0, you recover the original parametrization, but when b=1,
  Normalization_y_1 becomes the parameter to 'eat' the Sigma_tot DoF.
  You can set this dynamically in your fitting macros to switch between the two.
  Otherwise, you would have to create a second reparametrized workspace for when you want
  access to Shape_y_0.

 */

void parametrizeShape(string version, string obsName = "yZ")
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

  //We will want to first compute some initial value for the reparametrization
  double sigma_tot_val = 0;
  vector<double> shape_val;

  //Loop over the POIs and get these initial values
  TIterator* pitr = mc->GetParametersOfInterest()->createIterator();
  RooRealVar* var;
  pitr->Reset();
  // loop over the Norm parameters and get total sigma and establish if it is CC, MM or combined
  while ((var = (RooRealVar*)pitr->Next())) {
    if (string(var->GetName()).find("Normalization") == string::npos) continue;
    cout << var->GetName() << " " << var->getVal() << " " << var->getError() << endl;
    sigma_tot_val += var->getVal(); // sum all the normalization parameters to get sigma_tot

    shape_val.push_back(var->getVal());
  }
  int nrBins = shape_val.size();
  for (int i=0;i<nrBins;i++) shape_val[i] /= sigma_tot_val;


  //Create the total cross section parameter, and the auxiliary variable 'b'
  RooRealVar* Sigma_tot = new RooRealVar("Sigma_tot","Sigma_tot",sigma_tot_val,0,10*sigma_tot_val);
  Sigma_tot->setConstant(0);
  w->import(*Sigma_tot);
  RooRealVar* b = new RooRealVar("b","b",0,0,1);
  b->setConstant(1);
  w->import(*b);

  //Create Shape_* parameters with the values of Norm_*/Sigma_tot - import them to the workspace
  for(int i=0;i<nrBins;i++)
  {
    string normName(Form("Normalization_%s_%i",obsName.c_str(),i));
    string shapeName(Form("Shape_%s_%i",obsName.c_str(),i));

    RooRealVar* Shape = new RooRealVar(shapeName.c_str(),shapeName.c_str(),
				       shape_val[i],0,1);
    Shape->setConstant(0);
    w->import(*Shape);
  }

  //The syntax for editing an object in the workspace is the following
  //EDIT::objName(objName,origParam1=newExpression1,origParam2=newExpression2,...,origParamN=newExpressionN)
  //objName is what you want to edit, origParamX is the original parameter being replaced,
  //and newExpressionX is the new expression it will be replaced with. Let's build a string we'll use to call this.

  //Remember our formula for reparametrization:
  // Normalization_yZ_0 = Sigma_tot * [   b   * Shape_yZ_0 + (1-b) * (1 - sum_{X!=0} Shape_yZ_X) ]
  // Normalization_yZ_1 = Sigma_tot * [ (1-b) * Shape_yZ_1 +   b   * (1 - sum_{X!=1} Shape_yZ_X) ]
  // Normalization_yZ_X = Sigma_tot * Shape_yZ_X, for X = {2..N-1}

  stringstream editStr;
  editStr << "EDIT::" << pdf->GetName() << "(" << pdf->GetName();
  for(int i=0;i<nrBins;i++){

    //We'll use a RooFormulaVar to replace the original parameters. This takes a formula written as a string.
    //It then takes the parameters for that formula in a (ordered) RooArgList
    stringstream formStr;
    RooArgList formSet;
    if(i == 0 ){
      formStr << "@0*(@1 * @2 + (1-@1) * (1 - (";
      formSet.add(*Sigma_tot);
      formSet.add(*b);
      formSet.add(*w->var(Form("Shape_%s_0",obsName.c_str())));

      // sum_{X!=0} Shape_yZ_X
      for (int j=0;j<nrBins;j++){
	if (j == 0) continue;
	formStr << "@" << j+2;
	if (j != nrBins-1) formStr << "+";
	formSet.add(*w->var(Form("Shape_%s_%i",obsName.c_str(),j)));
      }
      formStr << ")))";
    }

    else if (i == 1){
      formStr << "@0*((1-@1) * @2 + @1 * (1 - (";
      formSet.add(*Sigma_tot);
      formSet.add(*b);
      formSet.add(*w->var(Form("Shape_%s_1",obsName.c_str())));

      // sum_{X!=1} Shape_yZ_X
      for (int j=0;j<nrBins;j++){
	if (j == 1) continue;
	formStr << "@" << j+2;
	if (j != nrBins-1) formStr << "+";
	if (j == 0) formSet.add(*w->var(Form("Shape_%s_0",obsName.c_str())));
	else formSet.add(*w->var(Form("Shape_%s_%i",obsName.c_str(),j)));
      }
      formStr << ")))";
    }
    else{
      formStr << "@0*@1";
      formSet.add(*Sigma_tot);
      formSet.add(*w->var(Form("Shape_%s_%i",obsName.c_str(),i)));
    }

    //Make a RooFormulaVar for each Norm parameter expressed as a function of Sigma_tot, Shape_* and b
    RooFormulaVar* form = new RooFormulaVar(Form("Normalization_%s_%i_form",obsName.c_str(),i),formStr.str().c_str(),formSet);
    w->import(*form);

    //Now put this into the edit string
    editStr << ",Normalization_" << obsName << "_" << i << "=Normalization_" << obsName << "_" << i << "_form";

    cout << "Formula for " << form->GetName() << ": " << formStr.str() << endl;
  }
  editStr << ")";

  //Print the edit string for verification
  cout << "Printing editStr" << endl;
  cout << "editStr " << editStr.str() << endl;

  //Edit the pdf
  string pdfName = pdf->GetName();
  w->factory(editStr.str().c_str());

  //Now we need to remake the POI set for the ModelConfig to include the new parameters
  RooArgSet pois;
  pois.add(*w->var("Sigma_tot"));
  for (int i=1;i<nrBins;i++) pois.add(*w->var(Form("Shape_%s_%i",obsName.c_str(),i)));

  //Print for verification
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

  //Make sure Shape_yZ_0 is constant at first
  RooRealVar* shape_0 = w->var(Form("Shape_%s_0",obsName.c_str()));
  shape_0->setConstant(1);

  system(Form("mkdir -vp workspaces/%s_Shape",version.c_str()));
  w->writeToFile(Form("workspaces/%s_Shape/tut_combined_%s_Shape_model.root",version.c_str(),version.c_str()),true);
}
