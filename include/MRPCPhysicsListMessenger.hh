
#ifndef MRPCPhysicsListMessenger_h
#define MRPCPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class MRPCPhysicsList;
class G4UIdirectory;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MRPCPhysicsListMessenger: public G4UImessenger
{
  public:
  
    MRPCPhysicsListMessenger(MRPCPhysicsList* );
   ~MRPCPhysicsListMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    MRPCPhysicsList*           fPhysicsList;
    G4UIdirectory*             fMRPCDirectory;
    G4UIdirectory*             fPhysDir;        
    G4UIcmdWithAString*        fListCmd;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

