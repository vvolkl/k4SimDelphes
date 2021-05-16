#ifndef __CINT__ 
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/RecoParticleRefCollection.h"
#include "edm4hep/ReconstructedParticleCollection.h"
#endif

#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

#include "TH1F.h"
#include "TLorentzVector.h"

/*
put this into your .rootlogon.C file
and make sure the (DY)LD_LIBRARY_PATH contains the podio and edm4hep installations
{
 gInterpreter->AddIncludePath("$PODIO/include");
 gSystem->Load("libpodio");
 gSystem->Load("libpodioDict");
 gSystem->Load("libpodioRootIO");
 gSystem->Load("libedm4hep");
 gSystem->Load("libedm4hepDict");

}

 or, at the root interpreter:

 gSystem->Load("libpodioRootIO");
 .L examples/tower_plots.C
 tower_plots("tests/data/test_delphes_edm4hep.root")
*/

/*
to create the sample data:

cd tests/data
DelphesPythia8_EDM4HEP delphes_card_IDEA.tcl edm4hep_output_config.tcl  ~/r/k4simdelphes/examples/data/ee_Z_bbbar_ecm91GeV.cmd  test_delphes_edm4hep.root
DelphesPythia8 delphes_card_IDEA.tcl  ~/r/k4simdelphes/examples/data/ee_Z_bbbar_ecm91GeV.cmd  test_delphes.root


*/


template<class T>
TLorentzVector v4(const T& p){
  return TLorentzVector( p.getMomentum()[0],p.getMomentum()[1], p.getMomentum()[2],p.getEnergy());
}

/** Example script for accessing EFlowPhotons
 */
 
void tower_plots(const char* FILEN) {

 int nEvents = 0  ;
 int maxEvt = 1000 ;  // change as needed

//---- create some histograms ----

 TH1F* eflowphoton_phi = new TH1F("EFlowPhoton_phi","EFlowPhoton - Phi", 100,  -7, 7 ) ;
 TH1F* eflowphoton_eta  = new TH1F("EFlowPhoton_eta","EFlowPhoton - Eta", 100, -7 , 7 ) ;

//---------------------------------
 
 auto reader = podio::ROOTReader();
 reader.openFile( FILEN );

 auto store = podio::EventStore();
 store.setReader(&reader);

  
 //==================== the event loop ============================================================
 const auto nEntries = reader.getEntries();
 for (int entry = 0; entry < nEntries && nEvents++ < maxEvt ; ++entry) {
   
   auto& photons = store.get<edm4hep::RecoParticleRefCollection>("Photon");


   for (auto p: photons) {
     const auto& mom = v4(p.getParticle()) ;

     eflowphoton_phi->Fill( mom.Phi() ) ;
     eflowphoton_eta->Fill( mom.Eta() ) ;
   }

   store.clear();
   reader.endOfEvent();

 }

 //===================================================================================================
 TCanvas* c1 = new TCanvas("EFlowPhoton plots");

 c1->Divide(2,2);
 c1->cd(1) ;
 eflowphoton_phi->Draw() ;
 c1->cd(2) ;
 eflowphoton_eta->Draw();

  c1->Print("recoil_plots.pdf") ;
 //===================================================================================================

  std::cout << std::endl 
	    <<  "  " <<  nEvents 
	    << " events read from file: " 
	    << FILEN << std::endl  ;
  
  
}
