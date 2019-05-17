// Author: Aaron Armbruster
// Date:   2012-05-30
// Email:  armbrusa@umich.edu
// Description: Find the value of sigma evaluated at a specified N_sigma

#ifndef FINDSIGMA
#define FINDSIGMA

#include "TMath.h"
#include "Math/ProbFuncMathCore.h"
#include "Math/QuantFuncMathCore.h"

#include "RooRealVar.h"
#include "Math/MinimizerOptions.h"
#include "RooMinimizerFcn.h"
#include "RooMinimizer.h"

#include "minimize.C"

#include <map>
#include <iostream>

using namespace std;
using namespace RooFit;

double getSigma(RooAbsReal* nll, double nll_min, RooRealVar* par, double val, double val_best, double& tmu);
void setVal(RooRealVar* par, double val);
double getNLL(RooAbsReal* nll);
double getTpar(RooAbsReal* nll, double nll_min, RooRealVar* par, double val);

// ____________________________________________________________________________|__________
// get sigma assuming nll -2logLR is parabolic in par
double findSigma(RooAbsReal* nll, double nll_min, RooRealVar* par, double val_guess, double val_best, double N_sigma, double precision, bool mode = 0) {
  if (mode == 0)
  {
    bool isConst = par->isConstant();
    par->setConstant(true);
    int direction = int(N_sigma/fabs(N_sigma));
    double tmu;
    int nrDamping = 1;
    double damping_factor = 1.0;
    double damping_factor_pre = damping_factor;
    int nrItr = 0;
    double sigma_guess = fabs((val_guess-val_best)/N_sigma);
    double val_pre = val_guess-10*precision*sigma_guess;
    while (fabs(val_pre-val_guess) > precision*sigma_guess) {
        cout << "----------------------" << endl;
        cout << "Starting iteration " << nrItr << " of " << nll->GetName() << " and parameter " << par->GetName() << endl;
        val_pre = val_guess;
        damping_factor_pre = damping_factor;
        sigma_guess = getSigma(nll, nll_min, par, val_guess, val_best, tmu);

        double corr = damping_factor*(val_pre - val_best - N_sigma*sigma_guess);

        // subtract off the difference in the new and damped correction
        val_guess -= corr;

        cout << "NLL:            " << nll->GetName() << " = " << nll->getVal() << endl;
        cout << "delta(NLL):     " << nll->getVal()-nll_min << endl;
        cout << "N_sigma*sigma(pre):   " << fabs(val_pre-val_best) << endl;
        cout << "sigma(guess):   " << sigma_guess << endl;
        cout << "par(guess):     " << val_guess+corr << endl;
        cout << "true val:       " << val_best << endl;
        cout << "tmu:            " << tmu << endl;
        cout << "Precision:      " << sigma_guess*precision << endl;
        cout << "Correction:     " << (-corr<0?" ":"") << -corr << endl;
        cout << "N_sigma*sigma(guess): " << fabs(val_guess-val_best) << endl;
        cout << endl;

        nrItr++;
        if (nrItr > 25) {
            cout << "Infinite loop detected in getSigma(). Please intervene." << endl;
            break;
        }
    }

    cout << "Found sigma for nll " << nll->GetName() << ": " << (val_guess-val_best)/N_sigma << endl;
    cout << "Finished in " << nrItr << " iterations." << endl;
    cout << endl;
    par->setConstant(isConst);

    return (val_guess-val_best)/N_sigma;
  }
  else
  {
    bool isConst = par->isConstant();
    par->setVal(0);
    par->setConstant(1);
    minimize(nll);
    double nll_val = nll->getVal();
    double q0 = 2*(nll_val - nll_min);
    par->setVal(val_best);
    par->setConstant(isConst);

    cout << "q0:       " << q0 << endl;
    cout << "true val: " << val_best << endl;
    cout << "sigma:    " << val_best/sqrt(q0) << endl;


    return val_best/sqrt(q0);
  }
}

// ____________________________________________________________________________|__________
double getSigma(RooAbsReal* nll, double nll_min, RooRealVar* par, double val, double val_best, double& tmu) {
    tmu = getTpar(nll, nll_min, par, val);
    return fabs(val-val_best)/sqrt(tmu);
}

// ____________________________________________________________________________|__________
double getTpar(RooAbsReal* nll, double nll_min, RooRealVar* par, double val) {
    setVal(par, val);
    double nll_val = getNLL(nll);
    return 2*(nll_val-nll_min);
}

// ____________________________________________________________________________|__________
double getNLL(RooAbsReal* nll) {
    minimize(nll);
    double val = nll->getVal();
    return val;
}

// ____________________________________________________________________________|__________
void setVal(RooRealVar* par, double val) {
    if (val > 0 && par->getMax() < val) par->setMax(2*val);
    if (val < 0 && par->getMin() > val) par->setMin(2*val);
    par->setVal(val);
}

#endif
