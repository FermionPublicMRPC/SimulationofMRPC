
#ifndef MRPCEventAction_h
#define MRPCEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;
class MRPCRecorder;
class MRPCDigitizer;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MRPCEventAction : public G4UserEventAction
{
  public:
    MRPCEventAction(MRPCRecorder*);
   ~MRPCEventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);

  private:
    MRPCRecorder* recorder;
    G4String digitizerName,digitsCollectionName,m_hitsColName;
    MRPCDigitizer * myDM;
    // MRPCSignal* mySigi;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
