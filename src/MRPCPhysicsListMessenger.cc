#include "MRPCPhysicsListMessenger.hh"
#include "MRPCPhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MRPCPhysicsListMessenger::MRPCPhysicsListMessenger(MRPCPhysicsList* phys)
 : G4UImessenger(),fPhysicsList(phys)
{
  fMRPCDirectory = new G4UIdirectory("/MRPC/");
  fMRPCDirectory->SetGuidance("UI commands specific to this example.");

  fPhysDir = new G4UIdirectory("/MRPC/Phy/");
  fPhysDir->SetGuidance("physics list commands");
   
  fListCmd = new G4UIcmdWithAString("/MRPC/Phy/Physics",this);  
  // fListCmd->SetGuidance("Add modula physics list.");
  fListCmd->SetParameterName("PList",true);
  fListCmd->SetDefaultValue("emstandard_opt0");
  fListCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MRPCPhysicsListMessenger::~MRPCPhysicsListMessenger()
{
  delete fListCmd;
  delete fPhysDir;   
  delete fMRPCDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MRPCPhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{       
  if( command == fListCmd )
    fPhysicsList->AddPhysicsList(newValue);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
