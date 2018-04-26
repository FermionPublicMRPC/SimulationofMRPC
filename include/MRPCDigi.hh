#ifndef MRPCDigit_h
#define MRPCDigit_h 1

#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include "G4VDigi.hh"
/**
 *  Digit MRPC class.
 */
class MRPCDigi : public G4VDigi {

public:
  MRPCDigi();
  ~MRPCDigi();

  MRPCDigi(const MRPCDigi&);
  const MRPCDigi& operator=(const MRPCDigi&);
  int operator==(const MRPCDigi&) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  void Draw();
  void Print();

  inline void SetStripID(G4int value)   {StripID = value;};
  inline void SetLeadTime(G4double value) {LeadTime=value;};
  inline void SetTruthTime(G4double value) {TruthTime=value;};
  inline void SetPeakTime(G4double value) {PeakTime=value;};
  inline void SetArrivedTime(G4double value) {ArrivedTime=value;};
  inline void SetTrailTime(G4double value) {TrailTime=value;};
  inline void SetToT(G4double value)   {ToT = value;};
  inline void SetWavePeak(G4double value)   {WavePeak = value;};
  inline void SetEdep(G4double value)   {Edep = value;};
  inline void SetInducedI(std::vector<G4double> value) {InducedI=value;};
  inline void SetInducedQ(std::vector<G4double> value) {InducedQ=value;};
  inline void SetTotalInducedQ(G4double value) {TotalInducedQ=value;};
  inline void SetInducedQAroundThre(std::vector<G4double> value) {InducedQAroundThre=value;};
  inline void SetPointTimeStep(G4double value){PointTimeStep=value;};
  inline void SetIfOverThreshold(bool value) {IfOverThreshold=value;};
  inline void SetNbofpointAboveThreshold(G4int value) {NbofpointAboveThreshold=value;};
  inline void SetParticleName(G4String value) {particleName=value;};
  inline G4int GetStripID() {return StripID;};
  inline G4double GetLeadTime() {return LeadTime;};
  inline G4double GetTruthTime() {return TruthTime;};
  inline G4double GetPeakTime() {return PeakTime;};
  inline G4double GetArrivedTime() {return ArrivedTime;};
  inline G4double GetTrailTime() {return TrailTime;};
  inline G4double GetToT() {return ToT;};
  inline G4double GetWavePeak() {return WavePeak;};
  inline G4double GetEdep() {return Edep;};
  inline std::vector<G4double> GetInducedI() {return InducedI;};
  inline std::vector<G4double> GetInducedQ() {return InducedQ;};
  inline G4double GetTotalInducedQ() {return TotalInducedQ;}
  inline G4int GetNbofpointAboveThreshold() {return NbofpointAboveThreshold;};
  inline G4double GetPointTimeStep() {return PointTimeStep;};
  inline std::vector<G4double> GetInducedQAroundThre(){return InducedQAroundThre;};
  inline G4String GetParticleName() {return particleName;};
  inline bool GetIfOverThreshold(){return IfOverThreshold;};
private:
  G4int StripID,NbofpointAboveThreshold;
  G4double LeadTime,TrailTime,Edep,TotalInducedQ,TruthTime,ToT,WavePeak,ArrivedTime,PeakTime,PointTimeStep;
  bool IfOverThreshold;
  G4String particleName;
  std::vector<G4double> InducedI,InducedQ;
  std::vector<G4double > InducedQAroundThre;
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

typedef G4TDigiCollection<MRPCDigi> MRPCDigitsCollection;

extern G4ThreadLocal G4Allocator<MRPCDigi> *MRPCDigiAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void* MRPCDigi::operator new(size_t)
{
  // void* aDigi;
  // aDigi = (void*) MRPCDigitAllocator.MallocSingle();
  // return aDigi;
  if (!MRPCDigiAllocator)
    MRPCDigiAllocator = new G4Allocator<MRPCDigi>;
  return (void*) MRPCDigiAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void MRPCDigi::operator delete(void* aDigi)
{
  MRPCDigiAllocator->FreeSingle((MRPCDigi*) aDigi);
}

#endif

