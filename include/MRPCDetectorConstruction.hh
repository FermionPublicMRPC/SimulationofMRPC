
#ifndef MRPCDetectorConstruction_H
#define MRPCDetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"
#include "MRPCDetectorMessenger.hh"
#include "G4UserLimits.hh"

class MRPCDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

  MRPCDetectorConstruction();
  ~MRPCDetectorConstruction();

  G4VPhysicalVolume* Construct();

  void SetGapsize(G4ThreeVector value){Gapsize=value;};
  void SetGlasssize(G4ThreeVector value){Glasssize=value;};
  void SetHoneysize(G4ThreeVector value){Honeysize=value;};
  void SetPCBsize(G4ThreeVector value){PCBsize=value;};
  void SetMylarsize(G4ThreeVector value){Mylarsize=value;};
  void SetNbofGapinAChamber(G4double value){Nbofgapinchamber=value;};
  void SetNbofchamber(G4double value){Nbofchamber=value;};
  void SetMaxStep(G4double value);
  void SetPitchWidth(G4double value){pitchwidth=value;};
  void UpdateGeometry();

  G4ThreeVector GetGlasssize(){return Glasssize;};
  G4ThreeVector GetGapsize(){return Gapsize;};
  G4double GetNbofgap(){return Nbofgap;};
  G4double GetNbofgapinchamber(){return Nbofgapinchamber;};
  G4double GetNbofchamber(){return Nbofchamber;};
  G4double GetPitchWidth(){return pitchwidth;};
  G4double GetChamberThickness(){return chamberThickness;};
  G4double GetTopGapPosition(){return topPCBpos-PCBsize.z()-Mylarsize.z()-Glasssize.z();};
  G4String GetHitCollectionName(){return HitCollectionName;};
  

  private:
  G4ThreeVector Gapsize,Glasssize,Honeysize,PCBsize,Mylarsize;
  G4double Nbofgap,Nbofgapinchamber,pitchwidth,maxStep,Nbofchamber,chamberThickness,topPCBpos;
  MRPCDetectorMessenger *m_detectorMessenger;
  G4UserLimits* fStepLimit;
  G4String HitCollectionName;
};

#endif

