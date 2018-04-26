#ifndef MRPCRecorder_h
#define MRPCRecorder_h 1
#include "MRPCDigi.hh"
#include "MRPCDigitizer.hh"
#include "MRPCTrackerHit.hh"
#include <TFile.h>
#include <TTree.h>
#include "MRPCRecorderMessenger.hh"
#include "TH2D.h"
class G4Run;
class G4Event;
class G4Step;


class MRPCRecorder
{
public:
  MRPCRecorder();
  ~MRPCRecorder();

  void SetupWriteFile();
  void RecordHitinROOT(MRPCTrackerHitsCollection* hitsCollection);
  void RecordDigitinROOT(MRPCDigitsCollection* digitsCollection);
  void SetRecordFlag(G4int value){recordflag=value;};
  void WriteCloseROOTFile();

private:
  void SetupTreeForDigit();
  void SetupTreeForHit();

private:

  TFile* outfile;
  TTree* treed,*treeh;
  std::vector<G4int> track,strip,pdg;
  std::vector<G4double> px,py,pz,dep;
  std::vector<G4double> indq;
  G4double leadtime,totalindq,truthtime,tot,wavepeak,arrivedtime,peaktime,pointtimestep;
  G4int recordflag,nbofpoint,particlename;
  bool overthre;
  MRPCRecorderMessenger* m_recorderMessenger;
  TH2D *Walk;
};

#endif





