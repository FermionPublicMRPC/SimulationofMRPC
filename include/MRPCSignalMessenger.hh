#ifndef MRPCSignalMessenger_h
#define MRPCSignalMessenger_h 1

#include "G4UImessenger.hh"

class MRPCSignal;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;

class MRPCSignalMessenger:public G4UImessenger
{
public:
  MRPCSignalMessenger(MRPCSignal *);
  ~MRPCSignalMessenger();
  void SetNewValue(G4UIcommand*, G4String);

private:
  MRPCSignal*           fSignal;
  G4UIdirectory*           fMRPCDirectory;
  G4UIdirectory*           fSigDirectory;
  G4UIcmdWithADouble* fAverageIonEnergyCmd,*fThresholdCmd,*fNoiseCmd,*fPointStepCmd,*fTau1Cmd,*fTau2Cmd,*fAmplitudeCmd,*fRealVoltageCmd;
  G4UIcmdWithAnInteger *fNbofstepsInAGapCmd,*fNbofPointAroundThreCmd,*fDrawPlotCmd;
};
#endif
