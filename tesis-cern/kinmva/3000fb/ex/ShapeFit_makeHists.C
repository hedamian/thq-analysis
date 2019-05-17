#include "TH1D.h"
#include "TRandom.h"
#include "THStack.h"

#include <iostream>

using namespace std;

void ShapeFit_makeHists()
{
  TRandom3 rndm(1);
  TFile* file = new TFile("hists_ShapeFit.root","recreate");

  //Generate a Gaussian signal histogram
  double expected_S = 102.3;

  TH1D* h_sig = new TH1D("Signal","Signal",10,0,10);
  //Also make some shape systematics for the signal, say, with a varied scale
  TH1D* h_sig_scale_down = new TH1D("Signal_scale_down","Signal_scale_down",10,0,10);
  TH1D* h_sig_scale_up = new TH1D("Signal_scale_up","Signal_scale_up",10,0,10);

  int nrEntries_S = 10000;
  for (int i=0;i<nrEntries_S;i++)
  {
    double x_val = rndm.Gaus(5,2);
    double weight = expected_S / nrEntries_S;
    h_sig->Fill(x_val, weight);
    h_sig_scale_down->Fill(x_val - 0.3, weight);
    h_sig_scale_up->Fill(x_val + 0.3, weight);
  }

  //Generate a uniform background
  double expected_B = 53.1;
  TH1D* h_bkg = new TH1D("Background","Background",10,0,10);

  //Also make some shape systematics for the background, say, with a different slope
  TH1D* h_bkg_slope_down = new TH1D("Background_slope_down","Background_slope_down",10,0,10);
  TH1D* h_bkg_slope_up = new TH1D("Background_slope_up","Background_slope_up",10,0,10);
  int nrEntries_B = 1000;
  for (int i=0;i<nrEntries_B;i++)
  {
    double x_val = rndm.Uniform(10);
    double weight = expected_B / nrEntries_B;
    h_bkg->Fill(x_val, weight);

    double delta = x_val*0.03;
    h_bkg_slope_down->Fill(x_val, weight*(1-delta));
    h_bkg_slope_up->Fill(x_val, weight*(1+delta));
  }

  //Generate the observed by Poisson fluctuations
  TH1D* h_obs = new TH1D("Data","Data",10,0,10);
  for (int i=0;i<10;i++)
  {
    double exp = h_sig->GetBinContent(i+1) + h_bkg->GetBinContent(i+1);
    double obs = rndm.Poisson(exp);
    h_obs->SetBinContent(i+1,obs);
    h_obs->SetBinError(i+1,sqrt(obs));
  }

  file->Write();

  TCanvas* c1 = new TCanvas("c1","c1",1600,600);
  c1->Divide(3,1);
  c1->cd(1);
  
  THStack* stack = new THStack("stack","stack");
  h_bkg->SetLineColor(kRed);
  //h_bkg->SetFillColor(kRed);
  stack->Add(h_bkg,"hist");
  h_sig->SetLineColor(kBlack);
  //h_sig->SetFillColor(kWhite);
  stack->Add(h_sig,"hist");
  h_obs->SetLineColor(kBlack);
  h_obs->SetMinimum(0);
  h_obs->SetStats(0);
  h_obs->SetTitle("Data vs expected");
  h_obs->GetYaxis()->SetTitle("Expected events");
  h_obs->GetXaxis()->SetTitle("Observable");
  h_obs->Draw("e");
  stack->Draw("same");
  h_obs->Draw("esame");

  c1->cd(2);
  h_sig->SetMinimum(0);
  h_sig->SetStats(0);
  h_sig->GetYaxis()->SetTitle("Expected events");
  h_sig->GetXaxis()->SetTitle("Observable");
  h_sig->Draw("hist");
  h_sig_scale_down->SetLineColor(kBlue);
  h_sig_scale_up->SetLineColor(kRed);
  h_sig_scale_down->SetLineStyle(2);
  h_sig_scale_up->SetLineStyle(2);
  h_sig_scale_down->Draw("histsame");
  h_sig_scale_up->Draw("histsame");

  c1->cd(3);
  //h_bkg->SetFillColor(0);
  h_bkg->SetMinimum(0);
  h_bkg->SetMaximum(8);
  h_bkg->SetStats(0);
  h_bkg->GetYaxis()->SetTitle("Expected events");
  h_bkg->GetXaxis()->SetTitle("Observable");
  h_bkg->Draw("hist");
  h_bkg_slope_down->SetLineColor(kBlue);
  h_bkg_slope_up->SetLineColor(kRed);
  h_bkg_slope_down->SetLineStyle(2);
  h_bkg_slope_up->SetLineStyle(2);
  h_bkg_slope_down->Draw("histsame");
  h_bkg_slope_up->Draw("histsame");
}