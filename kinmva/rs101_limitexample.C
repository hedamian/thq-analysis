
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

void rs101_limitexample()
{
   // --------------------------------------
   // An example of setting a limit in a number counting experiment with uncertainty on background and signal

   // --------------------------------------
   // The Model building stage
   // --------------------------------------
   RooWorkspace *wspace = new RooWorkspace();
   wspace->factory("Poisson::countingModel(obs[150,0,300],sum(s[50,0,100]*ratioSigEff[1.,0,3.],b[100]*ratioBkgEff[1.,0.,3.]))"); // counting model
   wspace->factory("Gaussian::sigConstraint(ratioSigEff,1,0.05)");//Systematic: 5% signal efficiency uncertainty
   wspace->factory("Gaussian::bkgConstraint(ratioBkgEff,1,0.1)"); //Systematic: 10% background efficiency uncertainty
   wspace->factory("PROD::modelWithConstraints(countingModel,sigConstraint,bkgConstraint)"); // Model including systematics
   wspace->Print();


   /// define the observed data
   RooRealVar *obs = wspace->var("obs");  // get the observable
   obs->setVal(120.);  // number of observed events
   RooDataSet *data = new RooDataSet("exampleData", "exampleData", RooArgSet(*obs));
   data->add(*obs);

   
   ////  fix the model parameters
   RooAbsPdf *modelWithConstraints = wspace->pdf("modelWithConstraints"); // get the model
   RooRealVar *b = wspace->var("b"); // get the background and set it to a constant.  Uncertainty included in ratioBkgEff
   b->setVal(100);
   b->setConstant();
   RooRealVar *ratioSigEff = wspace->var("ratioSigEff"); // get uncertain parameter to constrain
   //ratioSigEff->setConstant();    //fix the signal systematic
   RooRealVar *ratioBkgEff = wspace->var("ratioBkgEff"); // get uncertain parameter to constrain
   //ratioBkgEff->setConstant();  //fix the background systematic
   RooArgSet constrainedParams(*ratioSigEff,*ratioBkgEff); // need to constrain these in the fit (should change default behavior)

   
   ///  get the parameter of interest
   RooRealVar *s = wspace->var("s"); //parametro de la senal
   RooArgSet paramOfInterest(*s);
   

   /// Configure the Model
   ModelConfig modelConfig(wspace);
   modelConfig.SetName("ModelConfig");
   modelConfig.SetObservables(*obs); // los datos 
   modelConfig.SetPdf(*modelWithConstraints); // el modelo de los datos
   modelConfig.SetParametersOfInterest(paramOfInterest); // parametro de la senal
   modelConfig.SetNuisanceParameters(constrainedParams); // parametros de los sitematicos
   
  
   
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
   float lowerlimit=s->getMax();
   float upperlimit=s->getMin();
   for(int i=0;i<NPTS;i++){
     float val=s->getMin()+i*(s->getMax()-s->getMin())/NPTS;
     s->setVal(val);
     if(lrinterval->IsInInterval(*s)){
       if(val<lowerlimit) lowerlimit=val;
       if(val>upperlimit) upperlimit=val; 
     }
   }
   cout<<endl;
   cout<<"Results:"<<endl;
   cout<<"lower limit= "<<lowerlimit<<endl;
   cout<<"upper limit= "<<upperlimit<<endl;

   

}


