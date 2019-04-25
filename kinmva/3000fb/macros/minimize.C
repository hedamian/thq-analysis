// Author: Aaron Armbruster
// Date:   2012-05-30
// Email:  armbrusa@umich.edu
// Description: minimize a function with a smart retry strategy, eventually compute Minos error

#ifndef MINIMIZE
#define MINIMIZE

#include "Math/Minimizer.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooMinimizer.h"
#include "RooNLLVar.h"
#include "TStopwatch.h"
#include "RooFitResult.h"

#include <iostream>
#include <string>

using namespace std;
using namespace RooFit;

//retry_mode:
//0: retry by upping the strategy each time until a strategy 2 fit is performed. if strat 2 fit fails, give up
//N!=0: retry by performing default strategy fits 3 times
RooFitResult* minimize(RooAbsReal* fcn, bool save=0, int retry_mode=3, int* ret_status=NULL) 
{
  //Grab the default minimizer options
  int printLevel = ROOT::Math::MinimizerOptions::DefaultPrintLevel();
  RooFit::MsgLevel msglevel = RooMsgService::instance().globalKillBelow();
  if (printLevel < 0) RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);
  
  int strat = ROOT::Math::MinimizerOptions::DefaultStrategy();
  int save_strat = strat;

  //Configure the minimizer
  RooMinimizer minim(*fcn);
  minim.optimizeConst(2);
  minim.setStrategy(strat);
  minim.setPrintLevel(printLevel);
  //minim.setProfile(1);
  minim.setMinimizerType(ROOT::Math::MinimizerOptions::DefaultMinimizerType().c_str());

  //Do the minimization here!
  int status = minim.minimize(ROOT::Math::MinimizerOptions::DefaultMinimizerType().c_str(), ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo().c_str());
  
  //In case the fit failed, impose a semi-smart retry strategy

  //Here, increase the MINUIT strategy from 0 through 2 until the fit succeeds (or give up after 2)
  if (retry_mode == 0)
  {
    // up the strategy
    if (status != 0 && status != 1 && strat < 2) {
      strat++;
      cout << "Fit failed with status " << status << ". Retrying with strategy " << strat << endl;
      minim.setStrategy(strat);
      status = minim.minimize(ROOT::Math::MinimizerOptions::DefaultMinimizerType().c_str(), ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo().c_str());
    }
    
    // up the strategy
    if (status != 0 && status != 1 && strat < 2) {
      strat++;
      cout << "Fit failed with status " << status << ". Retrying with strategy " << strat << endl;
      minim.setStrategy(strat);
      status = minim.minimize(ROOT::Math::MinimizerOptions::DefaultMinimizerType().c_str(), ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo().c_str());
    }
  }

  //Here, just retry N times with the default strategy
  else
  {
    for (int i=0;i<retry_mode;i++)
    {
      if (status == 0 || status == 1) break;
      cout << "Fit failed with status " << status << ". Retrying with strategy " << strat << endl;
      minim.setStrategy(strat);
      status = minim.minimize(ROOT::Math::MinimizerOptions::DefaultMinimizerType().c_str(), ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo().c_str());
    }
  }
  
  
  //Reset the global configuration to the previous one
  if (printLevel < 0) RooMsgService::instance().setGlobalKillBelow(msglevel);
  ROOT::Math::MinimizerOptions::SetDefaultStrategy(save_strat);
  

  //Save the RooFitResult if asked to
  RooFitResult* fitresult = NULL;
  if (save) fitresult = minim.save(Form("fitresult_%s",fcn->GetName()), Form("fitresult_%s",fcn->GetName()));
  

  //Save the fit status if the pointer is valid
  if (ret_status) *ret_status = status;

  //Print the status of the fit
  if (status != 0 && status != 1) {
    cout << "Fit failed with status " << status << endl;
  }
  //else cout << "Fit succeeded with status " << status << endl;      

  //Return
  return fitresult;
}

#endif