#include "RooWorkspace.h"
#include "RooStats/ModelConfig.h"
#include "RooRealVar.h"
#include "RooMinimizerFcn.h"
#include "RooSimultaneous.h"
#include "RooCategory.h"

#include "TFile.h"
#include "TH1D.h"

#include "findsigma.C"
#include "minimize.C"
#include "ModelInfo.C"

#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;
using namespace RooFit;
using namespace RooStats;


double subtract(double err1, double err2);
double add(double err1, double err2, double err3);
double add(vector<double> errors);

map<string, double> MLE_values;
void loadMLEs(RooArgSet& params);
void getErrorWithBreakdown(string version, string parName)
{
  //Open the file and grab the objects out
ModelInfo info(Form("./workspace/%s/tut_combined_%s_model.root", version.c_str(),version.c_str())
  ,"combined","ModelConfig","obsData");

  RooWorkspace* w = info.w;
  ModelConfig* mc = info.mc;
  RooDataSet* data = info.data;
  RooAbsPdf* pdf = mc->GetPdf();
  if (!pdf)
  {
    cout << "ERROR::ModelConfig doesn't have PDF set!" << endl;
    exit(1);
  }

  //Configure MINUIT
  ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
  ROOT::Math::MinimizerOptions::SetDefaultStrategy(0);
  ROOT::Math::MinimizerOptions::SetDefaultPrintLevel(1);
  double prec = 0.002; // this will be the precision of the Minos root finder

  //Grab the parameter
  RooArgSet params(*mc->GetNuisanceParameters(),*mc->GetParametersOfInterest());
  RooRealVar* poi = w->var(parName.c_str());
  if (!poi)
  {
    cout << "POI " << parName << " doesn't exist" << endl;
    params.Print("v");
    exit(1);
  }
  poi->setConstant(0);

  //Create the NLL
  RooNLLVar* nll = (RooNLLVar*)mc->GetPdf()->createNLL(*data, Constrain(params), GlobalObservables(*mc->GetGlobalObservables()),RooFit::Offset(1));


  //Do the unconditional fit
  minimize(nll);

  //Save the nll minimum and the best fit parameter value
  double nll_min = nll->getVal();
  double poi_best = poi->getVal();

  //Get the estimate of the error from the fit result
  double err_guess = poi->getError();

  //Save a snapshot of the best fit values, and cache their values
  info.w->saveSnapshot("BestFit",params);

  TIterator* parItr = params.createIterator();
  parItr->Reset();
  RooRealVar* var;
  while ((var=(RooRealVar*)parItr->Next()))
  {
    MLE_values[var->GetName()] = var->getVal();
  }



  //Get the uncertainty breakdown in a loop over parameter categories:
  //1) get the total uncertainty
  //2) get the uncertainty with NPs from each category fixed
  //3) subtract in quadrature from the uncertainty in the previous iteration
  map<string, pair<double, double> > breakdown;
  vector<pair<string, string> > categories; // will match parameter names to these
  categories.push_back(make_pair("alpha_","Sys")); // first item is the string to match, second is category name
  categories.push_back(make_pair("Lumi","Lumi"));
  categories.push_back(make_pair("norm_","Bkg norm"));


  cout << endl << "%%%% On total uncertainty" << endl << endl;

  //Get the total uncertainty (find separate HI and LOW), using the initial guess
  double tot_hi = findSigma(nll, nll_min, poi, poi_best+err_guess, poi_best, +1, prec);

  //Find the lower uncertainty (updating the guess from the previous result)
  double tot_lo = findSigma(nll, nll_min, poi, poi_best-tot_hi, poi_best, -1, prec);

  breakdown["TOT"] = make_pair(tot_hi, tot_lo);
  cout << "=== TOTAL = " << tot_hi << " / " << tot_lo << endl;

  double previous_hi = tot_hi;
  double previous_lo = tot_lo;
  int nrCat = categories.size();
  for (int icat=0;icat<nrCat;icat++)
  {
    cout << "\n%%%% On uncertainty for category : " << categories[icat].second << endl << endl;
    double sigma_hi = previous_hi, sigma_lo = previous_lo, comp_hi = 0, comp_lo = 0;

    //Iterate over the NPs and fix the ones matching the string to constant
    parItr->Reset();
    bool oneChanged=false;
    while ((var=(RooRealVar*)parItr->Next()))
    {
      if (string(var->GetName()).find(categories[icat].first) != string::npos)
      {
	cout << "Setting parameter to constant: " << var->GetName() << endl;
	var->setConstant(1);
	oneChanged=true;
      }
    }

    //Set the parameters back to their best fit value.
    //Use a custom function rather than snapshots to preserve previous parameters set to constant
    loadMLEs(params);

    //If there's a match and >= 1 parameter was fixed, get the uncertainty again
    if (oneChanged)
    {
      sigma_hi = findSigma(nll, nll_min, poi, poi_best+previous_hi, poi_best,  1, prec);
      sigma_lo = findSigma(nll, nll_min, poi, poi_best-previous_lo, poi_best, -1, prec);
      comp_hi = subtract(previous_hi, sigma_hi);
      comp_lo = subtract(previous_lo, sigma_lo);

      previous_hi = sigma_hi;
      previous_lo = sigma_lo;
    }
    else
    {
      sigma_hi = previous_hi;
      sigma_lo = previous_lo;
      comp_hi = 0;
      comp_lo = 0;
    }
    breakdown[categories[icat].second] = make_pair(comp_hi, comp_lo);
    cout << "=== " << categories[icat].second << " = " << comp_hi << " / " << comp_lo << endl;
  }

  //Now all parameters should be set to constant, so get the uncertainty purely from the parameter
  double stat_hi = (breakdown["STAT"].first = findSigma(nll, nll_min, poi, poi_best+previous_hi, poi_best,  +1, prec));
  double stat_lo = (breakdown["STAT"].second = findSigma(nll, nll_min, poi, poi_best-previous_hi, poi_best, -1, prec));
  cout << "=== STAT = " << stat_hi << " / " << stat_lo << endl;




  //Now just print the full set of results
  cout << setprecision(5) << "\n\n\n\n";
  cout << "Parameter name: " << poi->GetName() << endl;
  cout << "Best fit      : " << poi_best << endl;
  cout << setprecision(3);
  cout << "Hessian guess   : " << err_guess << endl;
  cout << "---------------------------" << endl;
  cout << "Total           : " << breakdown["TOT"].first << " / " << breakdown["TOT"].second << endl;
  cout << "Stat            : " << breakdown["STAT"].first << " / " << breakdown["STAT"].second << endl;
  for (int icat=0;icat<nrCat;icat++){
    cout << categories[icat].second << " : " << breakdown[categories[icat].second].first << " / " << breakdown[categories[icat].second].first << endl;
  }
}


double subtract(double err1, double err2)
{
  if (err2 >= err1) return 0.;
  return sqrt(err1*err1-err2*err2);
}

double add(double err1, double err2, double err3)
{
  return sqrt(err1*err1+err2*err2+err3*err3);
}
double add(vector<double> errors)
{
  double quadsum = 0;
  for(unsigned int i=0; i < errors.size(); i++){
    quadsum += errors[i]*errors[i];
    //cout << "adding " << errors[i] << endl;
  }
  return sqrt(quadsum);
}

void loadMLEs(RooArgSet& params)
{
  for (map<string, double>::iterator itr=MLE_values.begin();itr!=MLE_values.end();itr++)
  {
    ((RooRealVar*)params.find(itr->first.c_str()))->setVal(itr->second);
  }
}
