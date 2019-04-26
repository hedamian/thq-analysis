
/// Limits: number counting experiment with uncertainty on both the background rate and signal efficiency.
/// The usage of a Confidence Interval Calculator to set a limit on the signal is illustrated
/// \author Kyle Cranmer

#include "RooProfileLL.h"
#include "RooAbsPdf.h"
#include "RooStats/HypoTestResult.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooTreeDataStore.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TStopwatch.h"

#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/UniformProposal.h"
#include "RooStats/FeldmanCousins.h"
#include "RooStats/NumberCountingPdfFactory.h"
#include "RooStats/ConfInterval.h"
#include "RooStats/PointSetInterval.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "RooStats/RooStatsUtils.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/MCMCInterval.h"
#include "RooStats/MCMCIntervalPlot.h"
#include "RooStats/ProposalFunction.h"
#include "RooStats/ProposalHelper.h"
#include "RooFitResult.h"
#include "TGraph2D.h"

#include <cassert>

// use this order for safety on library loading
using namespace RooFit;
using namespace RooStats;

void limite()
{
   // --------------------------------------
   // An example of setting a limit in a number counting experiment with uncertainty on background and signal

   // --------------------------------------
   // The Model building stage
   // --------------------------------------
   //Open the file, grab the workspace
   TFile* f = new TFile(Form("_combined_kinMVA_model.root"));
   RooWorkspace* w = (RooWorkspace*)f->Get("combined");

   //Grab the ModelConfig and the data

   ////  fix the model parameters
  RooAbsPdf* modelWithConstraints = w->pdf("ModelConfig"); // get the model

   /// define the observed data
   RooRealVar* obs = w->var("obs_x_SR");  // get the observable
   //RooDataSet *data = new RooDataSet("datos", "datos", RooArgSet(*obs));
   //data->add(*obs);
  RooDataSet* data = (RooDataSet*)w->data("obsData");

  // RooRealVar *b = wspace->var("b"); // get the background and set it to a constant.  Uncertainty included in ratioBkgEff
  // b->setVal(100);
  // b->setConstant();
  // RooRealVar *ratioSigEff = wspace->var("ratioSigEff"); // get uncertain parameter to constrain
   //ratioSigEff->setConstant();    //fix the signal systematic
  // RooRealVar *ratioBkgEff = wspace->var("ratioBkgEff"); // get uncertain parameter to constrain
   //ratioBkgEff->setConstant();  //fix the background systematic
  // RooArgSet constrainedParams(*ratioSigEff,*ratioBkgEff); // need to constrain these in the fit (should change default behavior)


   ///  get the parameter of interest
  RooRealVar *mu = w->var("mu"); //parametro de la senal
  RooArgSet paramOfInterest(*mu);


   //Grab the ModelConfig and the data


   /// Configure the Model
   ModelConfig modelConfig(w);
   modelConfig.SetName("ModelConfig");
   modelConfig.SetObservables(*obs); // los datos
   modelConfig.SetPdf(*modelWithConstraints); // el modelo de los datos
   modelConfig.GetParametersOfInterest(); // parametro de la senal
   modelConfig.GetNuisanceParameters(); // parametros de los sitematicos



   //  use test based on the Profile Likelihood Ratio
   ProfileLikelihoodCalculator plc(*data, modelConfig);
   plc.SetTestSize(.05);  //confidence level, it means with  95%  the true value of the signal is within the range determined by this test
   ConfInterval *lrinterval = plc.GetInterval();



   // Let's make a plot
   TCanvas *dataCanvas = new TCanvas("dataCanvas");
   LikelihoodIntervalPlot plotInt((LikelihoodInterval *)lrinterval);
   plotInt.SetTitle("Profile Likelihood Ratio and Posterior for S");
   plotInt.SetNPoints(300);
   plotInt.SetRange(0,60);
   plotInt.SetMaximum(4);
   plotInt.Draw();


   //Scan the parameter of interest to determine numerical values for the limits:
   int NPTS=1000;
   float lowerlimit=mu->getMax();
   float upperlimit=mu->getMin();
   for(int i=0;i<NPTS;i++){
     float val=mu->getMin()+i*(mu->getMax()-mu->getMin())/NPTS;
     mu->setVal(val);
     if(lrinterval->IsInInterval(*mu)){
       if(val<lowerlimit) lowerlimit=val;
       if(val>upperlimit) upperlimit=val;
     }
   }
   cout<<endl;
   cout<<"Results:"<<endl;
   cout<<"lower limit= "<<lowerlimit<<endl;
   cout<<"upper limit= "<<upperlimit<<endl;



}
