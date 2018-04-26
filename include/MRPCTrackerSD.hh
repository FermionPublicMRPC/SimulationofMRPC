#ifndef MRPCTrackerSD_h
#define MRPCTrackerSD_h 1

#include "G4VSensitiveDetector.hh"
#include "MRPCTrackerHit.hh"
#include "G4WrapperProcess.hh"

#include <set>

using namespace std;

class G4Step;
class G4HCofThisEvent;
class G4Event;
class MRPCDetectorConstruction;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MRPCTrackerSD : public G4VSensitiveDetector
{
  public:
  MRPCTrackerSD(const G4String& name, 
	      const G4String& hitsCollectionName, G4ThreeVector, MRPCDetectorConstruction*);
  MRPCTrackerSD(G4String, G4ThreeVector, G4RotationMatrix *);
  ~MRPCTrackerSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);
  
  G4String GetHitsCollectionName(){ return m_thisHitsCollectionName; };

private:
  MRPCTrackerHitsCollection* hitsCollection;
  G4ThreeVector m_absolutePos; // Absolute position
  MRPCDetectorConstruction* m_gD; // Geo description !
  G4String m_thisHitsCollectionName;

  bool firstStrikePrimary;
  G4double _kinEPrimary;
  G4double _totalEdep;
  G4int Nbofgap,Nbofgapinchamber,Nbofchamber;
  G4double topGappos,chamberthickness,gapthickness,glassthickness,pitchwidth,centerofstrip0;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

  
