
#include "MRPCRunAction.hh"
#include "MRPCRecorder.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
MRPCRunAction::MRPCRunAction(MRPCRecorder* rec)
:recorder(rec)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
MRPCRunAction::~MRPCRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void MRPCRunAction::BeginOfRunAction(const G4Run* aRun)
{
  recorder->SetupWriteFile();
  G4cout<<"Begin Running ..." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void MRPCRunAction::EndOfRunAction(const G4Run* aRun)
{
  recorder->WriteCloseROOTFile();
  G4cout<<"End Running ..."<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
