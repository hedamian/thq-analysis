///roofit example using histograms
//made by hiram
using namespace RooFit;
void hor(){


TH1F H("H","",10,0,1);
RooRealVar x("x","x",0,1);
RooDataHist DH("DH","DH",RooArgList(x),&H);
RooHistPdf HP("HP","HP",RooArgSet(x),DH);

TCanvas* c = new TCanvas("c","hor",1200,1900) ;
RooPlot* p=x.frame();
DH.plotOn(p);
p->Draw();
c->Print("o.pdf");
}
