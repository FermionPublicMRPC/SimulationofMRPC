#ifndef MRPCDigitizerMessenger_h
#define MRPCDigitizerMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class MRPCDigitizer;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;

class MRPCDigitizerMessenger:public G4UImessenger
{
public:
  MRPCDigitizerMessenger(MRPCDigitizer *);
  ~MRPCDigitizerMessenger();
  void SetNewValue(G4UIcommand*, G4String);

private:
  MRPCDigitizer * fdig;
  G4UIdirectory*           fMRPCDirectory;
  G4UIdirectory*           fDigDirectory;
  G4UIcmdWith3VectorAndUnit* fGapsizeCmd;
};
#endif
