#include "MRPCEventAction.hh"
#include "MRPCRecorder.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "MRPCDigitizer.hh"
#include "G4DigiManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
MRPCEventAction::MRPCEventAction(MRPCRecorder* rec)
  : recorder(rec)
{
  digitizerName="MyDigitizer";
  digitsCollectionName="DigitCollection";
  m_hitsColName="TrackerHitsCollection";
  myDM = new MRPCDigitizer(digitizerName,digitsCollectionName,m_hitsColName);
  G4DigiManager::GetDMpointer()->AddNewModule(myDM);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
MRPCEventAction::~MRPCEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void MRPCEventAction::BeginOfEventAction(const G4Event* evt)
{
  G4int eventID=evt->GetEventID();
  if(eventID%10000==0)
    G4cout<<"event="<<evt->GetEventID()<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void MRPCEventAction::EndOfEventAction(const G4Event* evt)
{
  myDM->Digitize();
  MRPCDigitsCollection* digitsCollection=myDM->GetDigitsCollection();

  G4DigiManager * digiMan = G4DigiManager::GetDMpointer();
  G4int hcID = digiMan->GetHitsCollectionID(m_hitsColName);
  MRPCTrackerHitsCollection* hitsCollection = (MRPCTrackerHitsCollection*)(digiMan->GetHitsCollection(hcID));
  recorder->RecordHitinROOT(hitsCollection);
  recorder->RecordDigitinROOT(digitsCollection);
  // MRPCSigi* mySigi=CalculateSignal(hitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
