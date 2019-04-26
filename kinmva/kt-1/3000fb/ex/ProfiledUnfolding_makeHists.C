#include "TFile.h"
#include "TTree.h"

#include "TH1D.h"
#include "TH2D.h"

#include <iostream>
#include <string>

using namespace std;
/*
  INTRO:

  This macro will make the input histograms for HistFactory.
  The output likelihood will be used to perform a profiled unfolding.
  The unfolding will be performed in the full phase space of the truth
  Born leptons, in the region 80 < mZ < 100 GeV, integrated over pTZ,
  and differential in yZ from 0 < |yZ| < 2.4.

  The sample used is from the 8 TeV Powheg+Pythia8 MC sample. The sample 
  was divided into lepton-filtered slices, and the event weights already 
  contain the proper weighting for this. The reco event weights have
  all scale factors applied already, so for both truth and reco the events
  simply need to be weighted by the corresponding weight.

  The sample is a random selection of 500k events from the full 80M to save
  time and space.


  PROCESS:

  The process proceeds in two steps. The first step is to generate the 
  histograms for truth phase space distribution we wish to unfold to, ie
  80 < mZ < 100, binned in |yZ|: dsigma / d|y^Z_{truth}|

  The second step is to produce histograms of the reco |yZ| distribution, 
  which are categorized in truth |yZ|, ie (dsigma / d|y^Z_{reco}|)_i, 
  where i is the bin in |y^Z_{truth}|. The ratio 

  (dsigma / d|y^Z_{reco}|)_i / (dsigma / d|y^Z_{truth}|)_i 

  is the acceptance folded with reco migrations, and is the identical to the
  reco row 'j' of the response matrix R_{ij}.
 */


void ProfiledUnfolding_makeHists(int nrBins = 12)
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

  //First step! Loop and get the truth |yZ| distribution in the 
  //mass window 80-100. Only truth info is needed here.
  //Let's use 12 bins between 0-2.4, ie bins of 0.2 in |yZ|.
  TFile* outFile = new TFile("hists/ProfiledUnfolding.root","recreate");

  TH1D* h_yZ_truth = new TH1D("yZ_truth","Truth Z-boson rapidity",nrBins,0,2.4);


  cout << "On truth loop" << endl;
  for (int ientry=0;ientry<nrEntries;ientry++)
  {
    if ((ientry % 1000000) == 0) cout << "On entry: " << ientry << " / " << nrEntries << endl;
    tree->GetEntry(ientry);
    if (m_truth > 100 || m_truth < 80) continue; // apply the mass window
    
    //just fill the histogram with the appropriate weight
    h_yZ_truth->Fill(y_truth, w_truth);
  }

  //Second step! Make histograms categorized in the |yZ| truth bins,
  //where it has the same binning in reco as truth.
  //Note, events falling into this histogram must also fall into the 
  //truth AND reco mZ window! 
  TH1D** h_yZ_reco_i = new TH1D*[nrBins];
  for (int i=0;i<nrBins;i++) h_yZ_reco_i[i] = new TH1D(Form("yZ_reco_%i",i),Form("Reco Z-boson rapidity, truth bin %i",i),nrBins,0,2.4);

  //Also make the full reco distribution itself, 
  //which we will use as pseudo-data to fit to.
  TH1D* h_yZ_reco = new TH1D("yZ_reco","Reco Z-boson rapidity",nrBins,0,2.4);

  //Finally, make a template for the events that fall into the reco window,
  //but fail truth. These events must still be considered in the unfolding 
  //process, since they pass the reco selections, but are considered as a 
  //background. This is the so-called "non-fiducial Z"
  TH1D* h_yZ_reco_nonFid = new TH1D("yZ_reco_nonFid","Reco rapidity of non-fiducial Z",nrBins,0,2.4);

  //To visualize the migrations, plot reco vs truth
  TH2D* h_yZ_reco_truth = new TH2D("yZ_reco_truth","Reco vs truth Z-boson rapidity",nrBins,0,2.4,nrBins,0,2.4);

  cout << "On reco loop" << endl;
  for (int ientry=0;ientry<nrEntries;ientry++)
  {
    if ((ientry % 1000000) == 0) cout << "On entry: " << ientry << " / " << nrEntries << endl;
    tree->GetEntry(ientry);
    //cout << y_reco << ", " << y_truth << endl;
    
    if (m_reco > 100 || m_reco < 80) continue; // apply the mass window
    if (!isEE_reco) continue; // require two good electrons

    //just fill the histogram with the appropriate weight
    h_yZ_reco->Fill(y_reco, w_reco);

    //Select events that are inside the Z-fiducial volume
    if (m_truth < 100 && m_truth > 80)
    {
      //Find the yZ-truth bin for this event
      int yZ_truth_bin=nrBins-1;
      for (int i=0;i<nrBins;i++)
      {
	if (y_truth < double(i+1)/nrBins*2.4)
	{
	  yZ_truth_bin = i;
	  break;
	}
      }

      //Fill the histo with the reco yZ, 
      //and reco weight divided by the true XS
      double w = w_reco / h_yZ_truth->GetBinContent(yZ_truth_bin+1);
      h_yZ_reco_i[yZ_truth_bin]->Fill(y_reco, w);

      h_yZ_reco_truth->Fill(y_reco, y_truth, w_reco);
    }
    else //Select events that are outside the Z-fiducial volume
    {
      h_yZ_reco_nonFid->Fill(y_reco, w_reco);
    }
  }
  outFile->Write();
  outFile->Close();  
}