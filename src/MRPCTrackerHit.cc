#include "MRPCTrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<MRPCTrackerHit>* MRPCTrackerHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MRPCTrackerHit::MRPCTrackerHit()
  : G4VHit()
    // fTrackID(-1),
    // fChamberNb(-1),
    // fEdep(0.),
    // fPos(G4ThreeVector())
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MRPCTrackerHit::~MRPCTrackerHit() {}
MRPCTrackerHit::MRPCTrackerHit(const MRPCTrackerHit& right)
  : G4VHit()
{
  trackID   = right.trackID;
  // detID = right.detID;
  // parentID = right.parentID;
  prestripID = right.prestripID;
  poststripID = right.poststripID;
  // postPixelNbX = right.postPixelNbX;
  // postPixelNbY = right.postPixelNbY;
  edep      = right.edep;
  prepos       = right.prepos;
  postpos       = right.postpos;
  m_preposWithRespectToStrip = right.m_preposWithRespectToStrip;
  // m_postposWithRespectToStrip = right.m_postposWithRespectToPixel;
  processName = right.processName;
  pdgIdTrack = right.pdgIdTrack;
  trackVolumeName = right.trackVolumeName;
  parentVolumeName = right.parentVolumeName;
}

const MRPCTrackerHit& MRPCTrackerHit::operator=(const MRPCTrackerHit& right)
{
  trackID   = right.trackID;
  // detID = right.detID;
  // parentID = right.parentID;
  prestripID = right.prestripID;
  poststripID = right.poststripID;
  // postPixelNbX = right.postPixelNbX;
  // postPixelNbY = right.postPixelNbY;
  edep      = right.edep;
  prepos       = right.prepos;
  postpos       = right.postpos;
  m_preposWithRespectToStrip = right.m_preposWithRespectToStrip;
  // m_postposWithRespectToPixel = right.m_postposWithRespectToPixel;
  processName = right.processName;
  pdgIdTrack = right.pdgIdTrack;
  trackVolumeName = right.trackVolumeName;
  parentVolumeName = right.parentVolumeName;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int MRPCTrackerHit::operator==(const MRPCTrackerHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MRPCTrackerHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {
      G4Circle circle(postpos);
      circle.SetScreenSize(4.);
      circle.SetFillStyle(G4Circle::filled);
      G4Colour colour(1.,0.,0.);
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MRPCTrackerHit::Print()
{
  G4cout << "  trackID: " << trackID << "  prestripNb: " << prestripID
	 << "  poststripNb: " << poststripID
	 << "  energy deposit: " << G4BestUnit(edep,"Energy")
	 << "  position: " << G4BestUnit(postpos,"Length") << G4endl;
}
