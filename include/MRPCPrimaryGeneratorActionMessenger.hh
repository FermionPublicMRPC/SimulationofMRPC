#ifndef MRPCPrimaryGeneratorActionMessenger_h
#define MRPCPrimaryGeneratorActionMessenger_h 1


#include "G4UImessenger.hh"
class MRPCPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADouble;
 
class MRPCPrimaryGeneratorActionMessenger: public G4UImessenger
{
public:
  MRPCPrimaryGeneratorActionMessenger(MRPCPrimaryGeneratorAction*);
  ~MRPCPrimaryGeneratorActionMessenger();
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  G4UIdirectory* fMRPCDirectory,*fGeneDirectory;
  G4UIcmdWith3VectorAndUnit* fGunPosCmd,*fGunDirCmd;
  MRPCPrimaryGeneratorAction* fPrimaryGenerator;
};
#endif

