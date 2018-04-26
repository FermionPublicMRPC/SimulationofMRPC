
#ifndef MRPCDigitizer_h
#define MRPCDigitizer_h 1

#include "G4VDigitizerModule.hh"
#include "MRPCDigi.hh"
#include "G4PrimaryVertex.hh"
#include <map>
#include <vector>
#include "G4PrimaryVertex.hh"
#include "MRPCDigitizerMessenger.hh"
#include "MRPCDetectorConstruction.hh"
#include "MRPCSignal.hh"
class MRPCDigitizer : public G4VDigitizerModule {

public:

  MRPCDigitizer(const G4String& modName,const G4String& digsCollectionName,const G4String& hitsColName);
  virtual ~MRPCDigitizer();
  void SetGapsize(G4ThreeVector value){Gapsize=value;};
  void Digitize ();
  MRPCDigitsCollection* GetDigitsCollection(){return digitsCollection;};

private:
  MRPCDigitizerMessenger* m_digitizerMessenger;
  G4ThreeVector Gapsize;
  MRPCSignal* Signal;
  G4String m_hitsColName,digitizerName;
  MRPCDigitsCollection* digitsCollection;
};
#endif
