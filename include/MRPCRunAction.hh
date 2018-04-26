
#ifndef MRPCRunAction_h
#define MRPCRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class MRPCRecorder;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MRPCRunAction : public G4UserRunAction
{
  public:
   MRPCRunAction(MRPCRecorder*);
   ~MRPCRunAction();

  public:
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);

  private:
    MRPCRecorder* recorder;     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
