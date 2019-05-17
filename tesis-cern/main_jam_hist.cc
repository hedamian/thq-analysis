// main01.cc is a part of the PYTHIA event generator.
// Copyright (C) 2018 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program. It fits on one slide in a talk.
// It studies the charged multiplicity distribution at the LHC.
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include "TROOT.h"
#include "TTree.h"
#include "TH2.h"
#include "TH1.h"
#include "TString.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TAxis.h"
#include "TLatex.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TMinuit.h"
#include "TFile.h"            
#include "TCanvas.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TSlider.h"
#include <iomanip>
#include <math.h>
#include <cmath>
#include "TGaxis.h"
#include "TMatrixDSym.h"
#include "TFitResult.h"
#include "TPaveLabel.h"


#include "Pythia8/Pythia.h"
using namespace Pythia8;

int main(){             // Begin main program.

    // Set up generation. Incoming pp beams is default.
    Pythia pythia;         // Declare Pythia object
    pythia.readString("Beams:eCM = 13000."); // 8 TeV CM energy.
    pythia.readString("HiggsSM:gg2H = on"); // Switch on g g -> H.
    pythia.init();         // Initialize.
    // Generate event(s).
    //pythia.next();         // Generate an(other) event. Fill event record.
    
    // * numero de eventos *
    int numero_evt = 1000;
    
    // * Archivo de root donde se guardara la informacion*
    TFile *test_file = TFile::Open("ROOT_files/gg2H_test_file.root","RECREATE");  
    
    // create a subdirectory "higgs" in this file
    TDirectory *Dhiggs = test_file->mkdir("higgs");
    
    // Histogramas de Root con distintas variables 
    TH1F *hist_mass = new TH1F("hist_mass", "hist_mass", 100, 124.9, 125.1);
    TH1F *higgs_eta = new TH1F("higgs_eta", "higgs_eta", 60, -5.0, 5.0);
    TH1F *higgs_phi = new TH1F("higgs_phi", "higgs_phi", 60, 0.0, 3.1416);
    TH1F *higgs_pt = new TH1F("higgs_pt", "higgs_pt", 100, 0.0, 50.0);
    
    // Hist Mass("Higgs Mass", 100, 80., 180.);

    // Hacer loop sobre todos los eventos
    for (int iEvent = 0; iEvent < numero_evt; ++iEvent){
        
        // generar un evento nuevo 
        pythia.next();
        
        //pythia.readString("Next:numberShowEvent = 5");
        
        // cout << "|||||||||||||||||||||||||||||||||||||||||| " << endl;
        // cout << "|||||||||||||||||||||||||||||||||||||||||| " << endl;
        cout << "|||||||||||||||||||||||||||||||||||||||||| " << endl;
        
        cout << "Event number " << iEvent << endl;
        
        // identificador de higgs para este evento
        int iHiggs = 0;
        
        // * hacer un loop sobre todas las particulas del evento: pythia.event.size() *
        for(int i = 0; i < pythia.event.size(); ++i){
            
            // Muestra el id, status, masa de todas las particulas en el evento
            // cout << "i = " << i << ", id = " << pythia.event[i].id() << endl;
            // cout << "i = " << i << ", status = " << pythia.event[i].status() << endl;
            
            // if(pythia.event[i].isFinal())
            //     cout << "Final particle = " << i << ", id = " << pythia.event[i].id() << endl;
            
            if(pythia.event[i].id() == 25) iHiggs = i;

        }
        
        // LLenando propiedades en histograma correspondientes
        // Mass.fill(pythia.event[iHiggs].m());
        hist_mass->Fill(pythia.event[iHiggs].m());
        higgs_eta->Fill(pythia.event[iHiggs].eta());
        higgs_phi->Fill(pythia.event[iHiggs].phi());
        higgs_pt->Fill(pythia.event[iHiggs].pT());
        
        // cout << " Event Higgs Mass " <<  pythia.event[iHiggs].m() << endl;

        
        /* // Selected Higgs properties
        
        cout << " Higgs status "      << pythia.event[iHiggs].status()     << endl; 
        
        // cout << " Higgs status code " << pythia.event[iHiggs].statusCode() << endl;
        cout << " Mother 1 id "         << pythia.event[iHiggs].mother1()    << endl;
        cout << " Mother 2 id "         << pythia.event[iHiggs].mother2()    << endl;
        cout << " Daughter 1 id "       << pythia.event[iHiggs].daughter1()  << endl;
        cout << " Daughter 2 id "       << pythia.event[iHiggs].daughter2()  << endl;
        
        cout << " Is Higgs final? " << pythia.event[iHiggs].isFinal() << endl;*/
    }
    
    // cout << Mass;
    
    pythia.stat();
    
    test_file->cd();
    Dhiggs->cd();    
     
    hist_mass->Write();
    higgs_eta->Write();
    higgs_phi->Write();
    higgs_pt->Write();
    
    test_file->Close();
    
    return 0;
}                        // End main program with error-free return               