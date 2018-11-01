#include "MRPCSignalMessenger.hh"
#include "MRPCSignal.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
MRPCSignalMessenger::MRPCSignalMessenger(MRPCSignal *sig)
  : G4UImessenger(),
    fSignal(sig)
{
  fMRPCDirectory = new G4UIdirectory("/MRPC/");
  fMRPCDirectory->SetGuidance("UI commands specific to this example.");

  fSigDirectory = new G4UIdirectory("/MRPC/sig/");
  fSigDirectory->SetGuidance("Signal control");

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


  fFelectronicsParaCmd = new G4UIcmdWith3VectorAndUnit("/MRPC/sig/FElectronicsPara",this);
  fFelectronicsParaCmd->SetGuidance("Define FelectronicsPara:tau1,tau2,A");
  fFelectronicsParaCmd->SetParameterName("tau1","tau2","A",false);
  fFelectronicsParaCmd->SetUnitCategory("Length");
  fFelectronicsParaCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fElectronicsRangeCmd = new G4UIcmdWithADouble("/MRPC/sig/ElectronicsRange",this);
  fElectronicsRangeCmd->SetGuidance("Define ElectronicsRange");
  fElectronicsRangeCmd->SetParameterName("ElectronicsRange",true);
  fElectronicsRangeCmd->SetDefaultValue(12);
  fElectronicsRangeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSpacechargethreCmd = new G4UIcmdWithADouble("/MRPC/sig/Spacechargethre",this);
  fSpacechargethreCmd->SetGuidance("Define Spacechargethre");
  fSpacechargethreCmd->SetParameterName("Spacechargethre",true);
  fSpacechargethreCmd->SetDefaultValue(1e6);
  fSpacechargethreCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // fTau1Cmd = new G4UIcmdWithADouble("/MRPC/sig/Tau1",this);
  // fTau1Cmd->SetGuidance("Define Tau1");
  // fTau1Cmd->SetParameterName("Tau1",true);
  // fTau1Cmd->SetDefaultValue(0.01);
  // fTau1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // fTau2Cmd = new G4UIcmdWithADouble("/MRPC/sig/Tau2",this);
  // fTau2Cmd->SetGuidance("Define Tau2");
  // fTau2Cmd->SetParameterName("Tau2",true);
  // fTau2Cmd->SetDefaultValue(0.01);
  // fTau2Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // fAmplitudeCmd = new G4UIcmdWithADouble("/MRPC/sig/Amplitude",this);
  // fAmplitudeCmd->SetGuidance("Define Amplitude");
  // fAmplitudeCmd->SetParameterName("Amplitude",true);
  // fAmplitudeCmd->SetDefaultValue(0.01);
  // fAmplitudeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fDrawPlotCmd = new G4UIcmdWithAnInteger("/MRPC/sig/DrawPlot",this);
  fDrawPlotCmd->SetParameterName("Define DrawPlot",true);
  fDrawPlotCmd->SetDefaultValue(21);
  fDrawPlotCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fRealVoltageCmd = new G4UIcmdWithADouble("/MRPC/sig/RealVoltage",this);
  fRealVoltageCmd->SetGuidance("Define RealVoltage");
  fRealVoltageCmd->SetParameterName("RealVoltage",false);
  fRealVoltageCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


  fRandomPeakCmd = new G4UIcmdWithABool("/MRPC/sig/FindRandomPeak",this);
  fRandomPeakCmd->SetGuidance("Whether to find the peak with an uncertainty");
  fRandomPeakCmd->SetParameterName("randompeak",true);
  fRandomPeakCmd->SetDefaultValue(true);
  fRandomPeakCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fEnergyCmd = new G4UIcmdWithABool("/MRPC/sig/PrimaryEnergyUncertainty",this);
  fEnergyCmd->SetGuidance("Whether to consider the uncertainty of the primarty energy depo");
  fEnergyCmd->SetParameterName("energyuncertainty",true);
  fEnergyCmd->SetDefaultValue(true);
  fEnergyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fLongitudinalPosCmd = new G4UIcmdWithABool("/MRPC/sig/PrimaryLongitudinalPosUncertainty",this);
  fLongitudinalPosCmd->SetGuidance("Whether to consider the uncertainty of the primarty energy depo");
  fLongitudinalPosCmd->SetParameterName("LongPosuncertainty",true);
  fLongitudinalPosCmd->SetDefaultValue(true);
  fLongitudinalPosCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fAvalancheCmd = new G4UIcmdWithABool("/MRPC/sig/AvalancheUncertainty",this);
  fAvalancheCmd->SetGuidance("Whether to consider the uncertainty of the primarty energy depo");
  fAvalancheCmd->SetParameterName("avaluncertainty",true);
  fAvalancheCmd->SetDefaultValue(true);
  fAvalancheCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fWeightingFieldCmd = new G4UIcmdWithABool("/MRPC/sig/WeightingFieldUncertainty",this);
  fWeightingFieldCmd->SetGuidance("Whether to consider the uncertainty of the primarty energy depo");
  fWeightingFieldCmd->SetParameterName("avaluncertainty",true);
  fWeightingFieldCmd->SetDefaultValue(true);
  fWeightingFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  
  fUncertValueCmd = new G4UIcmdWith3VectorAndUnit("/MRPC/sig/UncertSetting",this);
  fUncertValueCmd->SetGuidance("Define UncertValue");
  fUncertValueCmd->SetParameterName("totaldepenergy","Nbofstep","WhetherRecordOriginal",true);
  fUncertValueCmd->SetDefaultValue(G4ThreeVector(999,999,0)); 
  fUncertValueCmd->SetUnitCategory("Length");
  fUncertValueCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

MRPCSignalMessenger::~MRPCSignalMessenger()
{
  // delete fTau1Cmd;
  // delete fTau2Cmd;
  // delete fAmplitudeCmd;
  delete fFelectronicsParaCmd;
  delete fElectronicsRangeCmd;
  delete fSpacechargethreCmd;
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
  delete fRandomPeakCmd;
  delete fEnergyCmd;
  delete fLongitudinalPosCmd;
  delete fAvalancheCmd;
  delete fWeightingFieldCmd;
  delete fUncertValueCmd;
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
  if(command == fFelectronicsParaCmd) fSignal->SetElectronicsParameter(fFelectronicsParaCmd->GetNew3VectorValue(newValue));
  if(command == fElectronicsRangeCmd) fSignal->SetElectronicsRange(fElectronicsRangeCmd->GetNewDoubleValue(newValue));
  if(command == fSpacechargethreCmd) fSignal->SetSpaceChargeThre(fSpacechargethreCmd->GetNewDoubleValue(newValue));
  // if(command == fTau1Cmd) fSignal->SetTau1(fTau1Cmd->GetNewDoubleValue(newValue));
  // if(command == fTau2Cmd) fSignal->SetTau2(fTau1Cmd->GetNewDoubleValue(newValue)); 
  // if(command == fAmplitudeCmd) fSignal->SetAmplitude(fTau1Cmd->GetNewDoubleValue(newValue));
  if(command == fDrawPlotCmd) fSignal->SetDrawPlot(fDrawPlotCmd->GetNewIntValue(newValue));
  if(command == fRealVoltageCmd) fSignal->SetRealVoltage(fRealVoltageCmd->GetNewDoubleValue(newValue));
  if(command == fRandomPeakCmd) fSignal->SetRandomPeakFlag(fRandomPeakCmd->GetNewBoolValue(newValue));
  if(command == fEnergyCmd) fSignal->SetUncertaintyEnergyFlag(fEnergyCmd->GetNewBoolValue(newValue));
  if(command == fLongitudinalPosCmd) fSignal->SetUncertaintyLongPosFlag(fLongitudinalPosCmd->GetNewBoolValue(newValue));
  if(command == fAvalancheCmd) fSignal->SetUncertaintyAvalancheFlag(fAvalancheCmd->GetNewBoolValue(newValue));
  if(command == fWeightingFieldCmd) fSignal->SetUncertaintyWeightingFieldFlag(fWeightingFieldCmd->GetNewBoolValue(newValue));
  if(command == fUncertValueCmd) fSignal->SetUncertaintyValue(fUncertValueCmd->GetNew3VectorValue(newValue));
  
}
