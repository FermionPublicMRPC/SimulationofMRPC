#include "MRPCSignalMessenger.hh"
#include "MRPCSignal.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
MRPCSignalMessenger::MRPCSignalMessenger(MRPCSignal *sig)
  : G4UImessenger(),
    fSignal(sig)
{
  fMRPCDirectory = new G4UIdirectory("/MRPC/");
  fMRPCDirectory->SetGuidance("UI commands specific to this example.");

  fSigDirectory = new G4UIdirectory("/MRPC/sig/");
  fSigDirectory->SetGuidance("Signal control");

  // fAlphaCmd = new G4UIcmdWithADouble("/MRPC/sig/alpha",this);
  // fAlphaCmd->SetGuidance("Define Alpha");
  // fAlphaCmd->SetParameterName("Alpha",false);
  // fAlphaCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // fEtaCmd = new G4UIcmdWithADouble("/MRPC/sig/eta",this);
  // fEtaCmd->SetGuidance("Define Eta");
  // fEtaCmd->SetParameterName("Eta",false);
  // fEtaCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fGastypeCmd = new G4UIcmdWithAString("/MRPC/sig/whichgasmixture",this);
  fGastypeCmd->SetGuidance("Define Gastype");
  fGastypeCmd->SetParameterName("Gastype",true);
  fGastypeCmd->SetDefaultValue("Standard");
  fGastypeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fAverageIonEnergyCmd = new G4UIcmdWithADouble("/MRPC/sig/AverageIonEnergy",this);
  fAverageIonEnergyCmd->SetGuidance("Define AverageIonEnergy");
  fAverageIonEnergyCmd->SetParameterName("AverageIonEnergy",false);
  fAverageIonEnergyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fNbofstepsInAGapCmd = new G4UIcmdWithAnInteger("/MRPC/sig/NbofstepsInAGap",this);
  fNbofstepsInAGapCmd->SetParameterName("Define NbofstepsInAGap",true);
  fNbofstepsInAGapCmd->SetDefaultValue(500);
  fNbofstepsInAGapCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fNbofPointAroundThreCmd = new G4UIcmdWithAnInteger("/MRPC/sig/NbofPointAroundThre",this);
  fNbofPointAroundThreCmd->SetParameterName("Define NbofPointAroundThre",true);
  fNbofPointAroundThreCmd->SetDefaultValue(21);
  fNbofPointAroundThreCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fPointStepCmd = new G4UIcmdWithADouble("/MRPC/sig/TimeIntervalAroundThre",this);
  fPointStepCmd->SetGuidance("Define PointStep");
  fPointStepCmd->SetParameterName("PointStep",false);
  fPointStepCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fThresholdCmd = new G4UIcmdWithADouble("/MRPC/sig/ThresholdPer",this);
  fThresholdCmd->SetGuidance("Define Threshold");
  fThresholdCmd->SetParameterName("ThresholdPer",true);
  fThresholdCmd->SetDefaultValue(0.1);
  fThresholdCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fNoiseCmd = new G4UIcmdWithADouble("/MRPC/sig/NoisePer",this);
  fNoiseCmd->SetGuidance("Define Noise");
  fNoiseCmd->SetParameterName("NoisePer",true);
  fNoiseCmd->SetDefaultValue(0.01);
  fNoiseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fTau1Cmd = new G4UIcmdWithADouble("/MRPC/sig/Tau1",this);
  fTau1Cmd->SetGuidance("Define Tau1");
  fTau1Cmd->SetParameterName("Tau1",true);
  fTau1Cmd->SetDefaultValue(0.01);
  fTau1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fTau2Cmd = new G4UIcmdWithADouble("/MRPC/sig/Tau2",this);
  fTau2Cmd->SetGuidance("Define Tau2");
  fTau2Cmd->SetParameterName("Tau2",true);
  fTau2Cmd->SetDefaultValue(0.01);
  fTau2Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fAmplitudeCmd = new G4UIcmdWithADouble("/MRPC/sig/Amplitude",this);
  fAmplitudeCmd->SetGuidance("Define Amplitude");
  fAmplitudeCmd->SetParameterName("Amplitude",true);
  fAmplitudeCmd->SetDefaultValue(0.01);
  fAmplitudeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fDrawPlotCmd = new G4UIcmdWithAnInteger("/MRPC/sig/DrawPlot",this);
  fDrawPlotCmd->SetParameterName("Define DrawPlot",true);
  fDrawPlotCmd->SetDefaultValue(21);
  fDrawPlotCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fRealVoltageCmd = new G4UIcmdWithADouble("/MRPC/sig/RealVoltage",this);
  fRealVoltageCmd->SetGuidance("Define RealVoltage");
  fRealVoltageCmd->SetParameterName("RealVoltage",false);
  fRealVoltageCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

MRPCSignalMessenger::~MRPCSignalMessenger()
{
  delete fTau1Cmd;
  delete fTau2Cmd;
  delete fAmplitudeCmd;
  delete fDrawPlotCmd;
  delete fMRPCDirectory;
  delete fSigDirectory;
  delete fAverageIonEnergyCmd;
  delete fNbofstepsInAGapCmd;
  delete fNbofPointAroundThreCmd;
  delete fThresholdCmd;
  delete fNoiseCmd;
  delete fPointStepCmd;
  delete fRealVoltageCmd;
  delete fGastypeCmd;
}


void MRPCSignalMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if(command == fGastypeCmd) fSignal->SetGasMixture(newValue);
  if(command == fAverageIonEnergyCmd) fSignal->SetAverageIonEnergy(fAverageIonEnergyCmd->GetNewDoubleValue(newValue));
  if(command == fNbofstepsInAGapCmd) fSignal->SetNbofstepsInAGap(fNbofstepsInAGapCmd->GetNewIntValue(newValue));
  if(command == fNbofPointAroundThreCmd) fSignal->SetNbofPointAroundThre(fNbofPointAroundThreCmd->GetNewIntValue(newValue));
  if(command == fPointStepCmd) fSignal->SetTimeInterval(fPointStepCmd->GetNewDoubleValue(newValue));
  if(command == fThresholdCmd) fSignal->SetThresholdPer(fThresholdCmd->GetNewDoubleValue(newValue));
  if(command == fNoiseCmd) fSignal->SetNoisePer(fNoiseCmd->GetNewDoubleValue(newValue));
  if(command == fTau1Cmd) fSignal->SetTau1(fTau1Cmd->GetNewDoubleValue(newValue));
  if(command == fTau2Cmd) fSignal->SetTau2(fTau1Cmd->GetNewDoubleValue(newValue)); 
  if(command == fAmplitudeCmd) fSignal->SetAmplitude(fTau1Cmd->GetNewDoubleValue(newValue));
  if(command == fDrawPlotCmd) fSignal->SetDrawPlot(fDrawPlotCmd->GetNewIntValue(newValue));
  if(command == fRealVoltageCmd) fSignal->SetRealVoltage(fRealVoltageCmd->GetNewDoubleValue(newValue));
}
