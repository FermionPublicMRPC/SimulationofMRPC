#include "MRPCPrimaryGeneratorActionMessenger.hh"
#include "MRPCPrimaryGeneratorAction.hh"

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIdirectory.hh"

MRPCPrimaryGeneratorActionMessenger::MRPCPrimaryGeneratorActionMessenger(MRPCPrimaryGeneratorAction *gene)
  : G4UImessenger(),
    fPrimaryGenerator(gene)
{
  fMRPCDirectory = new G4UIdirectory("/MRPC/");
  fMRPCDirectory->SetGuidance("UI commands specific to this example.");

  fGeneDirectory = new G4UIdirectory("/MRPC/gene/");
  fGeneDirectory->SetGuidance("Primary Generator control");

  fBeamtypeCmd = new G4UIcmdWithAString("/MRPC/gene/BeamType",this);
  fBeamtypeCmd->SetGuidance("Define Beamtype");
  fBeamtypeCmd->SetParameterName("Beamtype",true);
  fBeamtypeCmd->SetDefaultValue("mu-Cosmic");
  fBeamtypeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fBeamenergyCmd = new G4UIcmdWithADouble("/MRPC/gene/BeamEnergy",this);
  fBeamenergyCmd->SetGuidance("Define Beamenergy");
  fBeamenergyCmd->SetParameterName("Beamenergy",true);
  fBeamenergyCmd->SetDefaultValue(-10);//GeV
  fBeamenergyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fBeammomentumCmd = new G4UIcmdWithADouble("/MRPC/gene/BeamMomentum",this);
  fBeammomentumCmd->SetGuidance("Define Beammomentum");
  fBeammomentumCmd->SetParameterName("Beammomentum",true);
  fBeammomentumCmd->SetDefaultValue(-10);//MeV
  fBeammomentumCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fGunPosCmd = new G4UIcmdWith3VectorAndUnit("/MRPC/gene/GunPos",this);
  fGunPosCmd->SetGuidance("Define GunPos");
  fGunPosCmd->SetParameterName("GunPosx","GunPosy","GunPosz",false);
  fGunPosCmd->SetUnitCategory("Length");
  fGunPosCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fGunDirCmd = new G4UIcmdWith3VectorAndUnit("/MRPC/gene/GunDir",this);
  fGunDirCmd->SetGuidance("Define GunDir");
  fGunDirCmd->SetParameterName("GunDirx","GunDiry","GunDirz",false);
  fGunDirCmd->SetUnitCategory("Length");
  fGunDirCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

MRPCPrimaryGeneratorActionMessenger::~MRPCPrimaryGeneratorActionMessenger()
{
  delete fMRPCDirectory;
  delete fGeneDirectory;
  delete fGunPosCmd;
  delete fGunDirCmd;
  delete fBeamtypeCmd;
  delete fBeamenergyCmd;
  delete fBeammomentumCmd;
}

void MRPCPrimaryGeneratorActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if(command == fBeamtypeCmd) fPrimaryGenerator->SetParticleType(newValue);
  if(command == fBeamenergyCmd) fPrimaryGenerator->SetParticleEnergy(fBeamenergyCmd->GetNewDoubleValue(newValue));
  if(command == fBeammomentumCmd) fPrimaryGenerator->SetParticleMomentum(fBeammomentumCmd->GetNewDoubleValue(newValue));
  if(command ==fGunPosCmd ) fPrimaryGenerator->SetGunPos(fGunPosCmd->GetNew3VectorValue(newValue));
  if(command ==fGunDirCmd ) fPrimaryGenerator->SetGunDir(fGunDirCmd->GetNew3VectorValue(newValue));
 }
