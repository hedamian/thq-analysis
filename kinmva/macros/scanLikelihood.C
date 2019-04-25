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

void scanLikelihood(string version, string parName="mu", double range=2, int nrPoints=30)
{
  //Use a convenient helper struct to make opening the workspace and grabbing the objects easier
  //See macros/ModelInfo.C
  ModelInfo info(Form("_combined_%s_model.root",version.c_str(),version.c_str()),
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
    //double val = muhat - range + i / double(nrPoints-1) * 2*range;
    double xmin=0;
    double val= i*range+xmin;

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
  TCanvas *cs = new TCanvas("cs","cs",10,10,1200,900);
  TGraph* graph = new TGraph(nrPoints+1, x_ary, y_ary);
  graph->Draw("alp*");
  graph->GetXaxis()->SetTitle(parName.c_str());
  graph->GetYaxis()->SetTitle("-2 ln #Lambda");
 graph->SetTitle();
 graph->GetXaxis()->SetLimits(0,50);
 graph->GetYaxis()->SetRangeUser(0,10);
 graph->Draw("lp*");


  TLine l;
  l.SetLineStyle(2);
  l.SetLineWidth(2);
  l.SetLineColor(kRed);
  l.DrawLine(0,1,50,1);
  l.SetLineColor(kBlue);
  l.DrawLine(0,4,50,4);
  l.SetLineColor(kGreen+4);
  l.DrawLine(0,9,50,9);

TLatex lat ;
lat.SetTextSize(0.03);
lat.DrawLatex(45,1,"1#sigma");

TLatex lat2;
lat.SetTextSize(0.03);
lat.DrawLatex(45,4,"2#sigma");
lat.DrawLatex(45,9,"3#sigma");

cs->SaveAs("Likelihood.png");

/////////////////////////////////////////////////////////
TCanvas *tm1 = new TCanvas("tm1","tm1",100,100,1200,900);
TF1 *fa2 = new TF1("fa2","((1/sqrt(2.*pi))*(1/sqrt(x))*exp(-x/2.))",0,10);
//fa2->SetParameters(x_ary);
fa2->SetTitle(" p-value for t_{#mu}  k_{t}=1 ;t_{#mu};P(t_{#mu})");
fa2->SetLineColor(kBlack);

fa2->DrawIntegral();
///////////////////////////////////////////
TLine l2;
l2.SetLineStyle(2);
l2.SetLineWidth(2);
l2.SetLineColor(kRed);
l2.DrawLine(1,0,1,1);
l2.SetLineColor(kBlue);
l2.DrawLine(4,0,4,1);
l2.SetLineColor(kGreen+4);
l2.DrawLine(9,0,9,1);
lat.DrawLatex(1,1,"1#sigma");
lat.DrawLatex(4,1,"2#sigma");
lat.DrawLatex(9,1,"3#sigma");
//////////////////////////////////////////////////////
TArrow *ar1 = new TArrow(0,-0.03,1,-0.03,0.008,"<>");
lat2.SetTextSize(0.03);
lat2.SetTextColor(2);
lat2.DrawLatex(0.32,-0.1,"68.3%");
ar1->SetLineColor(2);
ar1->Draw();
///////////////////////////////////////////////////////
TArrow *ar2 = new TArrow(0,-0.05,4,-0.05,0.01,"<>");
ar2->SetLineColor(4);
ar2->Draw();
lat2.SetTextSize(0.03);
lat2.SetTextColor(4);
lat2.DrawLatex(4.32,-0.1,"95.4%");
////////////////////////////////////////////////////////
TArrow *ar3 = new TArrow(0,-0.07,9,-0.07,0.01,"<>");
ar3->SetLineColor(kGreen+4);
ar3->Draw();
lat2.SetTextSize(0.03);
lat2.SetTextColor(kGreen+4);
lat2.DrawLatex(9.32,-0.1,"99.7%");


tm1->SaveAs("nos.png");

TCanvas *tm2 = new TCanvas("tm2","tm2",100,100,1200,900);
fa2->SetLineColor(kRed);
fa2->SetTitle();
fa2->GetXaxis()->SetTitle("t_{#mu}");
fa2->GetYaxis()->SetTitle("f(t_{#mu})");
fa2->GetXaxis()->SetLimits(0,10);
fa2->Draw();
tm2->SaveAs("tmu.png");

}
