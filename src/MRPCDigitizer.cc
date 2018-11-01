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
 if(hitsCollection->entries()>0){
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
	 digit->SetOriginalPointTimeStep(Signal->GetOriginalPointTimeStep());
	 digit->SetWriteOriginalFlag(Signal->GetOriginalFlag());
	 if(Signal->GetOriginalFlag()==1){
		digit->SetOriginalPeakTime(Signal->GetOriginalPeakTime());
		digit->SetInducedI(Signal->GetOriginalIAroundThre());
		digit->SetOriginalPointTimeStep(Signal->GetOriginalPointTimeStep());
	 }
	 
    digitsCollection->insert(digit);  
    StoreDigiCollection(digitsCollection);
  }
 
}
