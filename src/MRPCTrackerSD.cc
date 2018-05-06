#include "MRPCTrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4DecayTable.hh"
#include "G4LogicalVolume.hh"
#include "MRPCDetectorConstruction.hh"
#include "MRPCTrackerHit.hh"
#include "G4SystemOfUnits.hh"
#include "TMath.h"
#include "TString.h"


MRPCTrackerSD::MRPCTrackerSD(const G4String& name,const G4String& hitsCollectionName,G4ThreeVector absPos,MRPCDetectorConstruction *gD)
  :G4VSensitiveDetector(name),
   hitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName); 

  m_absolutePos = absPos;
  m_gD = gD; // Geo description
  Nbofgap=gD->GetNbofgap();
  Nbofgapinchamber=gD->GetNbofgapinchamber();
  Nbofchamber=gD->GetNbofchamber();
  topGappos=gD->GetTopGapPosition();
  chamberthickness=gD->GetChamberThickness();
  gapthickness=gD->GetGapsize().z();
  glassthickness=gD->GetGlasssize().z();


  pitchwidth=gD->GetPitchWidth();
  centerofstrip0=-gD->GetGapsize().y()/2.+pitchwidth/2.;
  firstStrikePrimary = false;
  _totalEdep = 0;
  _kinEPrimary=0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MRPCTrackerSD::~MRPCTrackerSD(){ 

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MRPCTrackerSD::Initialize(G4HCofThisEvent* HCE)
{
  // Create the hit collection
  hitsCollection =new MRPCTrackerHitsCollection(SensitiveDetectorName, collectionName[0]);
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection( HCID, hitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MRPCTrackerSD::ProcessHits(G4Step * aStep, G4TouchableHistory *)
{
  G4Track * aTrack = aStep->GetTrack();
  // particle
  G4ParticleDefinition * aParticle = aTrack->GetDefinition();
  // create a hit instance
  MRPCTrackerHit * newHit = new MRPCTrackerHit();

  if ( aTrack->GetTrackID() == 1 && ! firstStrikePrimary ) {
    _kinEPrimary = aTrack->GetKineticEnergy()/keV;
    _kinEPrimary -= (aStep->GetDeltaEnergy()/keV);
    // G4cout<<aTrack->GetKineticEnergy()<<" momentum "<<aTrack->GetMomentum()<<endl;
    firstStrikePrimary = true;
	 // G4cout<<"particle "<<aTrack->GetDefinition()->GetParticleName()<<" momentum "<<aTrack->GetMomentum()<<G4endl;
  }

  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;
  G4StepPoint * preStepPoint = aStep->GetPreStepPoint();
  G4StepPoint * postStepPoint = aStep->GetPostStepPoint();
  const G4TouchableHandle touchablepre = preStepPoint->GetTouchableHandle();
  const G4TouchableHandle touchablepost = postStepPoint->GetTouchableHandle();

  // This positions are global, bring them to strip-center frame
  G4ThreeVector prePos = preStepPoint->GetPosition();
  G4ThreeVector correctedPos=prePos-m_absolutePos;
  G4int GapID=touchablepre->GetCopyNumber(1);

  G4int chamberID=GapID/Nbofgapinchamber;
  G4int GapinchamberID=GapID%Nbofgapinchamber;

  G4int prestripID=touchablepre->GetCopyNumber();

  G4ThreeVector centerOfStrip(0, centerofstrip0+prestripID*pitchwidth,topGappos-GapinchamberID*(gapthickness+glassthickness)-chamberID*chamberthickness); // the middle of the pitch
  // G4cout<<"particle pos "<<correctedPos.z()<<" gappos "<<centerOfStrip.z()<<" gap "<<GapID<<" chamber "<<chamberID<<" gapinchamber "<<GapinchamberID<<G4endl;
  correctedPos-=centerOfStrip;


  // G4cout<<"prepos "<<prePos<<" Nb "<<prestripID<<G4endl;
  // Look at the touchablepost only if in the same volume, i.e. in the sensitive Si Box
  // If the hit is in a different pixel, it is still the same phys volume
  // The problem is that if I the postStep is in a different volume, GetCopyNumber(1)
  //  doesn't make sense anymore.
  G4ThreeVector postPos(0,0,0);
  G4int poststripID;
  if(preStepPoint->GetPhysicalVolume() == postStepPoint->GetPhysicalVolume()){
    postPos = postStepPoint->GetPosition();
    poststripID = touchablepost->GetCopyNumber();
  }

  // process
  const G4VProcess * aProcessPointer = aStep->GetPostStepPoint()->GetProcessDefinedStep();

  //set function
  newHit->SetParticleName(aTrack->GetDefinition()->GetParticleName());
  newHit->SetTrackID(aTrack->GetTrackID());
  newHit->SetGapID(GapID);
  newHit->SetPreStripID(prestripID);
  newHit->SetPostStripID(poststripID);
  newHit->SetEdep(edep);
  newHit->SetLocalDepTime(aTrack->GetLocalTime());
  _totalEdep += edep;
  newHit->SetPrePos(prePos);
  newHit->SetPostPos(postPos);
  newHit->SetPrePosWithRespectToStrip(correctedPos);
  newHit->SetProcessName(aProcessPointer->GetProcessName());
  newHit->SetTrackPdgId(aParticle->GetPDGEncoding());
  newHit->SetKinEPrimary( _kinEPrimary );
  newHit->SetTrackVolumeName(aTrack->GetVolume()->GetName());
  newHit->SetParentVolumeName(aTrack->GetLogicalVolumeAtVertex()->GetName());
 
  hitsCollection->insert(newHit);
  if ( _totalEdep > _kinEPrimary ) {
    cout << "[WARNING] totalEdep = " << _totalEdep << ", kinEPrimary = " << _kinEPrimary << endl;
  }
  return true;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MRPCTrackerSD::EndOfEvent(G4HCofThisEvent* HCE )
{
  // G4int NbHits = hitsCollection->entries();
  // if(NbHits > 0)
  // 	G4cout << "--------> Hits Collection : " << collectionName[0] << " has " << NbHits << " hits " << G4endl;

  firstStrikePrimary = false;
  _totalEdep = 0.;
  _kinEPrimary = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

