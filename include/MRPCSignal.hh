#ifndef MRPCSignal_h
#define MRPCSignal_h 1

#include <TH1D.h>
#include "G4ThreeVector.hh"
#include "MRPCTrackerHit.hh"
#include "MRPCSignalMessenger.hh"
class MRPCSignal{

public:
  MRPCSignal();
  ~MRPCSignal();
  void Initialize(G4ThreeVector gapgeo,G4int eventID);
  void ResetSignal();
  void SetGapsize(G4ThreeVector value){Gapsize=value;};
  void SetAverageIonEnergy(G4double value){AverageIonEnergy=value;};
  void SetNbofstepsInAGap(G4int value){Nbofsteps=value;};
  void SetNbofPointAroundThre(G4int value){NbofPointAroundThre=value;};
  void SetThresholdPer(G4double value){ThresholdPer=value;};
  void SetNoisePer(G4double value){NoisePer=value;};
  void SetTimeInterval(G4double value){TimeInterval=value/1000;};//value:ps,TimeInterval:ns
  void SetTau1(G4double value){Tau1=value;};
  void SetTau2(G4double value){Tau2=value;};
  void SetAmplitude(G4double value){Amplitude=value;};
  void SetDrawPlot(G4int value){if(value==0) DrawPlot=false;else DrawPlot=true;};
  void SetRealVoltage(G4double value){RealVoltage=value;};


  bool IfSignalOverThreshold(){return OverThreshold;}
  TH1D* GetNbWithTime(){return NbWithTime;};
  TH1D* GetIWithTime(){return IWithTime;};
  TH1D* GetResponseIWithTime(){return ResponseIWithTime;};
  G4double GetLeadTime(){return LeadTime;};
  G4double GetTruthTime(){return TruthTime;};
  G4double GetPeakTime(){return ExperimentsPeakTime;};
  G4double GetArrivedTime(){return StartTimeOffset;};
  G4double GetTotalInducedQ(){return TotalInducedQ;};
  G4double GetToT(){return ToT;};
  G4double GetPointTimeStep(){return PointTimeStep;};
  G4double GetWavePeak(){return ResponseIWithTime->GetBinContent(Peak.first);};
  G4int GetNbofpointAboveThreshold(){return NbofpointAboveThreshold;};
  std::vector<G4double > GetInducedQAroundThre(){return InducedQAroundThre;};
  bool CalculateSignal(MRPCTrackerHitsCollection* hitsCollection);


private:
  void Avalanche(G4int gapID,G4ThreeVector PosWithRespectToStrip,G4double StartTime,G4int PrimaryElectrons); 
  void CalculateCurrent();
  void ElectronicsResponse();
  void CalculateToT();
  void CalculateWaveForm();
  G4double ElectronicsResponseFunction(G4double x);
  std::pair<G4int,G4int> FindWavePeak(std::pair<G4int,G4int> range,G4int step, TH1D* hist);
  std::vector<std::pair<G4int,G4double> > GetLeadingLength(TH1D* hist,G4double percentofMax);
  G4int GetLeadingNbofPoints(TH1D* hist,G4double percentofMax,std::pair<G4int,G4double> peak);
  // std::pair<G4int,G4double> GetTrailingEdge(TH1D* hist,G4double threshold);
  // std::pair<G4int,G4double> GetLeadingEdge(TH1D* hist,G4double threshold);
  G4int GetTrailingEdge(TH1D* hist,G4double threshold);
  G4int GetLeadingEdge(TH1D* hist,G4double threshold);
  std::pair<G4int,G4double> FindHistPeakValue(TH1D* hist);
  bool ResetGasParameter(G4double electricField);

  TH1D* DriftDistance[5];
  TH1D* NbWithTime,*IWithTime,*ResponseIWithTime;
  MRPCSignalMessenger* m_signalMessenger;
  G4ThreeVector Gapsize;
  G4double TimeCrossGap,MaxScale,ZStep,TimeStep;
  G4double AverageIonEnergy,pi,elec;
  G4double Alpha,Eta,EffAlpha,TownsendK;
  G4double RealVoltage,E,WeightE,Velocity,SpaceChargeThre,DiffLengthZ,DiffSigMaxScale,RelativePermittivity;
  G4double ThresholdPer,NoisePer,StartTimeOffset;
  G4double LeadTime,TotalInducedQ,TruthTime,ToT,ExperimentsPeakTime;
  G4double WaveMaxValue;
  G4int Nbofsteps,NbofPointAroundThre;
  bool doDiff,OverThreshold,DrawPlot;
  bool dogap[5];
  std::vector<G4double > InducedQAroundThre;
  G4int PointStep,CountNbofPointsOverThre,NbofpointAboveThreshold;
  G4double TimeInterval;
  std::pair<G4int,G4int> Peak;
  TH1D* Nbofpointshist,*peakchargehist;
  G4double Tau1,Tau2,Amplitude,PointTimeStep;
};
#endif
