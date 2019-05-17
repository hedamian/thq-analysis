using namespace RooFit ;

void ro(){

RooRealVar mes("mes","m_{ES} (GeV)",5.20,5.30);

// --- Parameters ---
RooRealVar sigmean("sigmean","B^{#pm} mass",5.28,5.20,5.30) ;
RooRealVar sigwidth("sigwidth","B^{#pm} width",0.0027,0.001,1.) ;
//
// // --- Build Gaussian PDF ---
RooGaussian signal("signal","signal PDF",mes,sigmean,sigwidth) ;
//
// --- Build Argus background PDF ---
RooRealVar argpar("argpar","argus shape parameter",-20.0,-100.,-1.) ;
RooArgusBG background("background","Argus PDF",mes,RooConst(5.291),argpar) ;
//
// // --- Construct signal+background PDF ---
RooRealVar nsig("nsig","#signal events",200,0.,10000) ;
RooRealVar nbkg("nbkg","#background events",800,0.,10000) ;
RooAddPdf model("model","g+a",RooArgList(signal,background),RooArgList(nsig,nbkg)) ;
//
// --- Generate a toyMC sample from composite PDF ---
RooDataSet *data = model.generate(mes,2000) ;
//
// // --- Perform extended ML fit of composite PDF to toy data ---
model.fitTo(*data) ;
//
// // --- Plot toy data and composite PDF overlaid ---
RooPlot* mesframe = mes.frame() ;
data->plotOn(mesframe) ;
model.plotOn(mesframe);
//model.plotOn(mesframe,Components(argus),LineStyle(kDashed)) ;
/////
// Clone xframe for use below
RooPlot* xframe2 = (RooPlot*) mesframe->Clone("xframe2") ;
TCanvas* c = new TCanvas("rf107_plotstyles","rf107_plotstyles",1200,1900) ;
mesframe->Draw();

//xframe2->Draw();
//mesframe->GetXaxis()->SetLimits(5.2,5.3);

//mesframe->GetYaxis()->SetLimits(0,100);
c->Print("ro2.png");

}
