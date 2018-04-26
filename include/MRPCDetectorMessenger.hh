#ifndef MRPCDetectorMessenger_h
#define MRPCDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class MRPCDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADouble;

class MRPCDetectorMessenger:public G4UImessenger
{
public:
  MRPCDetectorMessenger(MRPCDetectorConstruction* );
  ~MRPCDetectorMessenger();
  void SetNewValue(G4UIcommand*, G4String);

private:
  MRPCDetectorConstruction*    fDetectorConstruction;
  G4UIdirectory*           fMRPCDirectory;
  G4UIdirectory*           fDetDirectory;

  G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
  G4UIcmdWithADoubleAndUnit* fPitchWidthCmd;
  G4UIcmdWithoutParameter*   fUpdateCmd;
  G4UIcmdWith3VectorAndUnit* fGapsizeCmd,*fGlasssizeCmd,*fHoneysizeCmd,*fPCBsizeCmd,*fMylarsizeCmd;
  G4UIcmdWithADouble* fNbofGapCmd,*fNbofChamberCmd;
};
#endif
