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
  // void SetTau1(G4double value){Tau1=value;};
  // void SetTau2(G4double value){Tau2=value;};
  // void SetAmplitude(G4double value){Amplitude=value;};
  void SetElectronicsParameter(G4ThreeVector value){Tau1=value.x();Tau2=value.y();Amplitude=value.z();};
  void SetElectronicsRange(G4double value){ElectronicsRange=value;};
  void SetSpaceChargeThre(G4double value){SpaceChargeThre=value;};
  void SetDrawPlot(G4int value){if(value==0) DrawPlot=false;else DrawPlot=true;};
  void SetRealVoltage(G4double value){RealVoltage=value;};
  void SetGasMixture(G4String value){GasMix=value;};

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
  G4int GetOriginalFlag(){return RecordOriginal;};
  G4double GetOriginalPeakTime(){return OriginalPeakTime;};
  std::vector<G4double > GetOriginalIAroundThre(){return OriginalIAroundThre;};
  G4double GetOriginalPointTimeStep(){return TimeStep;};

  
  bool CalculateSignal(MRPCTrackerHitsCollection* hitsCollection);

  void SetRandomPeakFlag(bool value){RandomPeakFlag=value;};
  void SetUncertaintyEnergyFlag(bool value){EnergyFlag=value;};
  void SetUncertaintyLongPosFlag(bool value){LongPosFlag=value;};
  void SetUncertaintyAvalancheFlag(bool value){AvalancheFlag=value;};
  void SetUncertaintyWeightingFieldFlag(bool value){WeightingFieldFlag=value;};
  void SetUncertaintyValue(G4ThreeVector value){FixedTotalEnergy=value.x();FixedNbofstep=value.y();RecordOriginal=value.z();};



  
private:
  void Avalanche(G4int gapID,G4ThreeVector PosWithRespectToStrip,G4double StartTime,G4int PrimaryElectrons);
  void AvalancheWithoutUncertainty(G4double posz,G4double StartTime,G4int PrimaryElectrons);
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
  void CalculateOriginalWaveFrom();

  
  TH1D* DriftDistance[5];
  TH1D* NbWithTime,*IWithTime,*ResponseIWithTime;
  MRPCSignalMessenger* m_signalMessenger;
  G4ThreeVector Gapsize;
  G4double TimeCrossGap,MaxScale,ZStep,TimeStep;
  G4double AverageIonEnergy,pi,elec,LightSpeed;
  G4double Alpha,Eta,EffAlpha,TownsendK;
  G4double RealVoltage,E,WeightE,Velocity,SpaceChargeThre,DiffLengthZ,DiffSigMaxScale,RelativePermittivity;
  G4double ThresholdPer,NoisePer,StartTimeOffset;
  G4double LeadTime,TotalInducedQ,TruthTime,ToT,ExperimentsPeakTime,OriginalPeakTime;
  G4double WaveMaxValue,FixedTotalEnergy,FixedNbofstep;
  G4int Nbofsteps,NbofPointAroundThre,RecordOriginal;
  bool doDiff,OverThreshold,DrawPlot,RandomPeakFlag,EnergyFlag,LongPosFlag,AvalancheFlag,WeightingFieldFlag;;
  bool dogap[5];
  std::vector<G4double > InducedQAroundThre,OriginalIAroundThre;
  G4int PointStep,CountNbofPointsOverThre,NbofpointAboveThreshold;
  G4double TimeInterval;
  std::pair<G4int,G4int> Peak,OriginalPeak;
  TH1D* Nbofpointshist,*peakchargehist;
  G4double Tau1,Tau2,Amplitude,PointTimeStep,ElectronicsRange;
  G4String GasMix;
};
#endif
