#include "TFile.h"
#include "TTree.h"

#include "TH1D.h"
#include "TH2D.h"

#include <iostream>
#include <string>

using namespace std;
/*
  (See ProfiledUnfolding_makeHists.C for a description first)

  In this example, we'll do a profiled unfolding in mZ as per the HMDY analysis, 
  but using more bins in reco than in truth to improve sensitivity.
 */


void ProfiledUnfolding2_makeHists(int binRatio = 1)
{
  //Open the file, grab the tree, and load the branches
  TFile f("tree.root");
  TTree* tree = (TTree*)f.Get("tiny");

  double w_reco, pt_reco, m_reco, y_reco;
  double w_truth, pt_truth, m_truth, y_truth;
  int isEE_reco;
  tree->SetBranchAddress("w_reco",&w_reco); // event weight for reco
  tree->SetBranchAddress("pt_reco",&pt_reco);
  tree->SetBranchAddress("m_reco",&m_reco);
  tree->SetBranchAddress("y_reco",&y_reco);
  tree->SetBranchAddress("w_truth",&w_truth); // event weight for truth
  tree->SetBranchAddress("pt_truth",&pt_truth);
  tree->SetBranchAddress("m_truth",&m_truth);
  tree->SetBranchAddress("y_truth",&y_truth);
  tree->SetBranchAddress("isEE_reco",&isEE_reco); // has two reco leptons

  int nrEntries = tree->GetEntries();

  //First step! Loop and get the truth |mZ| distribution. Only truth info is needed here.
  TFile* outFile = new TFile(Form("hists/ProfiledUnfolding2_%i.root",binRatio),"recreate");

  //Let's use the binning from the 8 TeV HMDY paper 
  int nrBins_truth = 12;
  double* bins_truth = new double[nrBins_truth+1];
  bins_truth[0] = 116;
  bins_truth[1] = 130;
  bins_truth[2] = 150;
  bins_truth[3] = 175;
  bins_truth[4] = 200;
  bins_truth[5] = 230;
  bins_truth[6] = 260;
  bins_truth[7] = 300;
  bins_truth[8] = 380;
  bins_truth[9] = 500;
  bins_truth[10] = 700;
  bins_truth[11] = 1000;
  bins_truth[12] = 1500;

  // int nrBins_truth = 24;
  // double* bins_truth = new double[nrBins_truth+1];
  // bins_truth[0] = 116;
  // bins_truth[1] = 123;
  // bins_truth[2] = 130;
  // bins_truth[3] = 140;
  // bins_truth[4] = 150;
  // bins_truth[5] = 162.5;
  // bins_truth[6] = 175;
  // bins_truth[7] = 187.5;
  // bins_truth[8] = 200;
  // bins_truth[9] = 215;
  // bins_truth[10] = 230;
  // bins_truth[11] = 245;
  // bins_truth[12] = 260;
  // bins_truth[13] = 280;
  // bins_truth[14] = 300;
  // bins_truth[15] = 340;
  // bins_truth[16] = 380;
  // bins_truth[17] = 440;
  // bins_truth[18] = 500;
  // bins_truth[19] = 600;
  // bins_truth[20] = 700;
  // bins_truth[21] = 850;
  // bins_truth[22] = 1000;
  // bins_truth[23] = 1250;
  // bins_truth[24] = 1500;


  int nrBins_reco = nrBins_truth*binRatio;
  double* bins_reco = new double[nrBins_reco+1];
  int counter = 0;
  for (int i=0;i<nrBins_truth;i++)
  {
    for (int j=0;j<binRatio;j++) bins_reco[counter++] = bins_truth[i] + (bins_truth[i+1]-bins_truth[i])*j/binRatio;
  }
  bins_reco[nrBins_reco] = bins_truth[nrBins_truth];

  TH1D* h_mZ_truth = new TH1D("mZ_truth","Truth Z-boson mass",nrBins_truth,bins_truth);

  cout << "On truth loop" << endl;
  for (int ientry=0;ientry<nrEntries;ientry++)
  {
    if ((ientry % 1000000) == 0) cout << "On entry: " << ientry << " / " << nrEntries << endl;
    tree->GetEntry(ientry);
    if (m_truth > bins_truth[nrBins_truth] || m_truth < bins_truth[0]) continue; // apply the mass window
    
    //just fill the histogram with the appropriate weight
    h_mZ_truth->Fill(m_truth, w_truth);
  }





  //Second step! Make histograms categorized in the |mZ| truth bins,
  //where it has the same binning in reco as truth.
  //Note, events falling into this histogram must also fall into the 
  //truth AND reco mZ window! 
  TH1D** h_mZ_reco_i = new TH1D*[nrBins_truth];
  for (int i=0;i<nrBins_truth;i++) h_mZ_reco_i[i] = new TH1D(Form("mZ_reco_%i",i),
							     Form("Reco Z-boson mass, truth bin %i",i),
							     nrBins_reco,bins_reco);

  //Also make the full reco distribution itself, 
  //which we will use as pseudo-data to fit to.
  TH1D* h_mZ_reco = new TH1D("mZ_reco","Reco Z-boson mass",nrBins_reco,bins_reco);

  //Finally, make a template for the events that fall into the reco window,
  //but fail truth. These events must still be considered in the unfolding 
  //process, since they pass the reco selections, but are considered as a 
  //background. This is the so-called "non-fiducial Z"
  TH1D* h_mZ_reco_nonFid = new TH1D("mZ_reco_nonFid","Reco mass of non-fiducial Z",nrBins_reco,bins_reco);

  //To visualize the migrations, plot reco vs truth
  TH2D* h_mZ_reco_truth = new TH2D("mZ_reco_truth","Reco vs truth Z-boson mass",
				   nrBins_reco,bins_reco,nrBins_truth,bins_truth);

  cout << "On reco loop" << endl;
  for (int ientry=0;ientry<nrEntries;ientry++)
  {
    if ((ientry % 1000000) == 0) cout << "On entry: " << ientry << " / " << nrEntries << endl;
    tree->GetEntry(ientry);
    
    if (m_reco > bins_reco[nrBins_reco] || m_reco < bins_reco[0]) continue; // apply the mass window
    if (!isEE_reco) continue; // require two good electrons

    //just fill the histogram with the appropriate weight
    h_mZ_reco->Fill(m_reco, w_reco);

    //Select events that are inside the Z-fiducial volume
    if (m_truth < bins_truth[nrBins_truth] && m_truth > bins_truth[0])
    {
      //Find the mZ-truth bin for this event
      int mZ_truth_bin=nrBins_truth-1;
      for (int i=0;i<nrBins_truth;i++)
      {
	if (m_truth < bins_truth[i+1])
	{
	  mZ_truth_bin = i;
	  break;
	}
      }

      //Fill the histo with the reco mZ, 
      //and reco weight divided by the true XS
      double w = w_reco / h_mZ_truth->GetBinContent(mZ_truth_bin+1);
      h_mZ_reco_i[mZ_truth_bin]->Fill(m_reco, w);

      h_mZ_reco_truth->Fill(m_reco, m_truth, w_reco);
    }
    else //Select events that are outside the Z-fiducial volume
    {
      h_mZ_reco_nonFid->Fill(m_reco, w_reco);
    }
  }
  outFile->Write();
  outFile->Close();  
}