#include "minimize.C"
#include "ModelInfo.C"

#include "RooMinimizerFcn.h"
#include "RooRealVar.h"

#include "TGraph.h"
#include "TAxis.h"
#include "TString.h"
#include "TLine.h"

#include <iostream>

using namespace std;

void scanLikelihood(string version, string parName="mu", double range=2, int nrPoints=20)
{
  //Use a convenient helper struct to make opening the workspace and grabbing the objects easier
  //See macros/ModelInfo.C
  ModelInfo info(Form("workspace/%s/tut_combined_%s_model.root",version.c_str(),version.c_str()),
		 "combined",
		 "ModelConfig",
		 "obsData");

  //Grab the parameter you want to scan the NLL over
  RooArgSet params(*info.mc->GetNuisanceParameters(),*info.mc->GetParametersOfInterest());
  RooRealVar* par = info.w->var(parName.c_str());
  if (!par)
  {
    cout << "ERROR::Parameter doesn't exist: " << parName << endl;
    params.Print("v");
    exit(1);
  }

  //Configure MINUIT
  ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
  ROOT::Math::MinimizerOptions::SetDefaultStrategy(0);
  ROOT::Math::MinimizerOptions::SetDefaultPrintLevel(1);

  //Make sure your parameters are floating
  TIterator* itr = params.createIterator();
  RooRealVar* var;
  while ((var=(RooRealVar*)itr->Next()))
  {
    var->setConstant(0);
  }

  //Build the NLL
  RooNLLVar* nll = (RooNLLVar*)info.mc->GetPdf()->createNLL(*info.data, Constrain(params),
							    GlobalObservables(*info.mc->GetGlobalObservables()),
							    RooFit::Offset(1));

  //Do an unconditional fit (all parameters floating)
  minimize(nll);

  //Save the value of the NLL and the best fit value of the parameter at this point
  double nll_best = nll->getVal();
  double muhat = par->getVal();

  //Instantiate the arrays you'll use to save the scanned points
  double* x_ary = new double[nrPoints+1];
  double* y_ary = new double[nrPoints+1];

  //Loop over each point in the scan, do a dit, and save the NLL and the parameter point
  int counter=0;
  bool passedMinimum = false;
  for (int i=0;i<nrPoints;i++)
  {
    //Value of the parameter at this point in the scan
    double val = muhat - range + i / double(nrPoints-1) * 2*range;

    //Save the unconditional point we had earlier at the appropriate point in the array
    if (val > muhat && !passedMinimum)
    {
      passedMinimum = true;
      x_ary[counter] = muhat;
      y_ary[counter] = 0;
      counter++;
    }

    //Fix the parameter to its point in the scan and do the fit there
    par->setVal(val);
    par->setConstant(1);
    minimize(nll);

    //Save the -2*log(L(par) / L(par_hat))
    double dnllx2 = 2*(nll->getVal()-nll_best);
    x_ary[counter] = val;
    y_ary[counter] = dnllx2;
    counter++;
    cout << "On point " << i+1 << " / " << nrPoints << ". " << parName << "=" << val << ", -2lnLambda=" << dnllx2 << " (best fit = " << muhat << ")" << endl;
  }

  //Graph the results
  TGraph* graph = new TGraph(nrPoints+1, x_ary, y_ary);
  graph->SetTitle("Likelihood scan");
  graph->Draw("alp*");
  graph->GetXaxis()->SetTitle(parName.c_str());
  graph->GetYaxis()->SetTitle("-2 ln #Lambda");
  graph->Draw("lp*");

  TLine l;
  l.SetLineStyle(2);
  l.SetLineWidth(2);
  l.SetLineColor(kRed);
  l.DrawLine(muhat-range,1,muhat+range,1);
  l.SetLineColor(kBlue);
  l.DrawLine(muhat-range,4,muhat+range,4);
}
