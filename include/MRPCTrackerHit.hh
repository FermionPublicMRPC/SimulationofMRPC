#ifndef MRPCTrackerHit_h
#define MRPCTrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class MRPCTrackerHit : public G4VHit
{
public:
  MRPCTrackerHit();
  MRPCTrackerHit(const MRPCTrackerHit&);
  ~MRPCTrackerHit();
  // operators
  const MRPCTrackerHit& operator=(const MRPCTrackerHit&);
  G4int operator==(const MRPCTrackerHit&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  // methods from base class
  virtual void Draw();
  virtual void Print();

  // Set methods
  void SetTrackID  (G4int track)      { trackID = track; };
  void SetGapID  (G4int gap)      { gapID = gap; };
  void SetPreStripID(G4int strip)      { prestripID = strip; };
  void SetPostStripID(G4int poststrip) { poststripID = poststrip; };
  void SetEdep     (G4double de)      { edep = de; };
  void SetParticleName(G4String value) { particleName=value;};
  void SetLocalDepTime    (G4double value)      { localdeptime = value; };
  void SetPrePos      (G4ThreeVector xyz){ prepos = xyz; };
  void SetPostPos   (G4ThreeVector xyz){ postpos = xyz; };
  void SetPrePosWithRespectToStrip (G4ThreeVector pxzy) { m_preposWithRespectToStrip = pxzy; };
  // void SetPostPosWithRespectToStrip (G4ThreeVector pxzy) { m_postposWithRespectToStrip = pxzy; };
  void SetProcessName(G4String process) { processName = process; };
  void SetTrackPdgId(G4int pdgId) { pdgIdTrack = pdgId; };
  void SetKinEPrimary(G4double value){ particlePrimaryKinE = value;};
  void SetTrackVolumeName(G4String vn) { trackVolumeName = vn; };
  void SetParentVolumeName(G4String vn) { parentVolumeName = vn; } ;

 // Get methods
  G4int GetTrackID()    { return trackID; };
  G4int GetGapID()    { return gapID; };
  G4int GetPreStripID()  { return prestripID; };
  G4int GetPostStripID()  { return poststripID; };
  G4double GetEdep()    { return edep; };
  G4String GetParticleName() { return particleName; };
  G4double GetLocalDepTime()    { return localdeptime; };
  G4ThreeVector GetPrePos(){ return prepos; };
  G4ThreeVector GetPostPos(){ return postpos; };
  G4ThreeVector GetPrePosWithRespectToStrip() { return m_preposWithRespectToStrip; };
  // G4ThreeVector GetPostPosWithRespectToStrip() { return m_postposWithRespectToStrip; };
  G4String GetProcessName() { return processName; };
  G4int GetTrackPdgId() { return pdgIdTrack; };
  G4double GetKinEPrimary(){return particlePrimaryKinE ;};
  G4String GetTrackVolumeName() {return trackVolumeName;};
  G4String GetParentVolumeName() {return parentVolumeName;};

private:
  G4int trackID,gapID,prestripID,poststripID,pdgIdTrack;
  G4double edep,particlePrimaryKinE,localdeptime;
  G4ThreeVector prepos,postpos,m_preposWithRespectToStrip;//m_postposWithRespectToStrip;
  G4String processName,trackVolumeName,parentVolumeName,particleName;
};

typedef G4THitsCollection<MRPCTrackerHit> MRPCTrackerHitsCollection;
extern G4ThreadLocal G4Allocator<MRPCTrackerHit>* MRPCTrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* MRPCTrackerHit::operator new(size_t)
{
  if(!MRPCTrackerHitAllocator)
      MRPCTrackerHitAllocator = new G4Allocator<MRPCTrackerHit>;
  return (void *) MRPCTrackerHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void MRPCTrackerHit::operator delete(void *hit)
{
  MRPCTrackerHitAllocator->FreeSingle((MRPCTrackerHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
