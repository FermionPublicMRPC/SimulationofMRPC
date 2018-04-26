#include "MRPCDigitizer.hh"
#include "MRPCDigitizerMessenger.hh"
#include "MRPCTrackerHit.hh"
#include "G4DigiManager.hh"
#include "G4VDigitizerModule.hh"
#include "MRPCSignal.hh"

#include "TRandom.h"
#include <vector>
#include "G4RunManager.hh"
MRPCDigitizer::MRPCDigitizer(const G4String& modName,const G4String& digsCollectionName,const G4String& hitsColName) 
  : G4VDigitizerModule (modName) {

  collectionName.push_back(digsCollectionName);
  digitizerName=modName;
  m_hitsColName=hitsColName;
  // doBichsel=false;
  Signal=new MRPCSignal();
  m_digitizerMessenger = new MRPCDigitizerMessenger(this);
}

MRPCDigitizer::~MRPCDigitizer(){
  delete m_digitizerMessenger;
}


void MRPCDigitizer::Digitize(){
 
  G4int eventID=G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  digitsCollection = new MRPCDigitsCollection(digitizerName, collectionName[0] );
 
  G4DigiManager * digiMan = G4DigiManager::GetDMpointer();
  G4int hcID = digiMan->GetHitsCollectionID(m_hitsColName);

  MRPCTrackerHitsCollection* hitsCollection = (MRPCTrackerHitsCollection*)(digiMan->GetHitsCollection(hcID));

  Signal->Initialize(Gapsize,eventID);
  bool ifOverThreshold=Signal->CalculateSignal(hitsCollection);
  // if(ifOverThreshold){
    MRPCDigi * digit = new MRPCDigi();
	 digit->SetParticleName((*hitsCollection)[0]->GetParticleName());
    digit->SetTruthTime(Signal->GetTruthTime());
    digit->SetArrivedTime(Signal->GetArrivedTime());
    digit->SetLeadTime(Signal->GetLeadTime());
    digit->SetPeakTime(Signal->GetPeakTime());
    digit->SetToT(Signal->GetToT());
    digit->SetTotalInducedQ(Signal->GetTotalInducedQ());
    digit->SetWavePeak(Signal->GetWavePeak());
    digit->SetInducedQAroundThre(Signal->GetInducedQAroundThre());
    digit->SetIfOverThreshold(ifOverThreshold);
    digit->SetNbofpointAboveThreshold(Signal->GetNbofpointAboveThreshold());
    digit->SetPointTimeStep(Signal->GetPointTimeStep());
    // G4cout<<"Digitizer "<<Signal->GetPointTimeStep()<<G4endl;
    digitsCollection->insert(digit);  
    StoreDigiCollection(digitsCollection);
  // }
  // G4int nEntries = hitsCollection->entries();
  // // G4cout<<"Hits entries "<<nEntries<<G4endl;
  // for(G4int itr=0;itr<nEntries;itr++){
  //   G4cout<<"hits "<<(*hitsCollection)[itr]->GetTrackID()<<" "<<(*hitsCollection)[itr]->GetGapID()<<" "<<(*hitsCollection)[itr]->GetPreStripID()<<" "<<(*hitsCollection)[itr]->GetPostStripID()<<" "<<(*hitsCollection)[itr]->GetEdep()<<(*hitsCollection)[itr]->GetPrePos()<<" "<<(*hitsCollection)[itr]->GetPrePosWithRespectToStrip()<<" "<<(*hitsCollection)[itr]->GetPostPos()<<G4endl;

  // }
}
