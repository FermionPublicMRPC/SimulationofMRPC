#ifndef MRPCRecorderMessenger_h
#define MRPCRecorderMessenger_h 1

#include "G4UImessenger.hh"
class G4UIcmdWithABool;
class G4UIdirectory;
class MRPCRecorder;
class G4UIcmdWithAnInteger;

class MRPCRecorderMessenger: public G4UImessenger
{
public:
  MRPCRecorderMessenger(MRPCRecorder*);
  ~MRPCRecorderMessenger();
  void SetNewValue(G4UIcommand*, G4String);

private:
  MRPCRecorder*            fRecorder;
  G4UIdirectory*           fMRPCDirectory;
  G4UIdirectory*           fRecDirectory;
  G4UIcmdWithAnInteger*    fWriteCmd;

};
#endif
