#include "MRPCDetectorMessenger.hh"
#include "MRPCDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"


MRPCDetectorMessenger::MRPCDetectorMessenger(MRPCDetectorConstruction *det) 
  : G4UImessenger(),
    fDetectorConstruction(det)
{

  fMRPCDirectory = new G4UIdirectory("/MRPC/");
  fMRPCDirectory->SetGuidance("UI commands specific to this example.");

  fDetDirectory = new G4UIdirectory("/MRPC/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fGapsizeCmd = new G4UIcmdWith3VectorAndUnit("/MRPC/det/Gapsize",this);
  fGapsizeCmd->SetGuidance("Define Gapsize");
  fGapsizeCmd->SetParameterName("Gapx","Gapy","Gapz",false);
  fGapsizeCmd->SetUnitCategory("Length");
  fGapsizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fGlasssizeCmd = new G4UIcmdWith3VectorAndUnit("/MRPC/det/Glasssize",this);
  fGlasssizeCmd->SetGuidance("Define Glasssize");
  fGlasssizeCmd->SetParameterName("Glassx","Glassy","Glassz",false);
  fGlasssizeCmd->SetUnitCategory("Length");
  fGlasssizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fHoneysizeCmd = new G4UIcmdWith3VectorAndUnit("/MRPC/det/Honeysize",this);
  fHoneysizeCmd->SetGuidance("Define Honeysize");
  fHoneysizeCmd->SetParameterName("Honeyx","Honeyy","Honeyz",false);
  fHoneysizeCmd->SetUnitCategory("Length");
  fHoneysizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fPCBsizeCmd = new G4UIcmdWith3VectorAndUnit("/MRPC/det/PCBsize",this);
  fPCBsizeCmd->SetGuidance("Define PCBsize");
  fPCBsizeCmd->SetParameterName("PCBx","PCBy","PCBz",false);
  fPCBsizeCmd->SetUnitCategory("Length");
  fPCBsizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fMylarsizeCmd = new G4UIcmdWith3VectorAndUnit("/MRPC/det/Mylarsize",this);
  fMylarsizeCmd->SetGuidance("Define Mylarsize");
  fMylarsizeCmd->SetParameterName("Mylarx","Mylary","Mylarz",false);
  fMylarsizeCmd->SetUnitCategory("Length");
  fMylarsizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fNbofGapCmd = new G4UIcmdWithADouble("/MRPC/det/NumberOfGapinAChamber",this);
  fNbofGapCmd->SetGuidance("Define NbofGap");
  fNbofGapCmd->SetParameterName("NbofGap",false);
  fNbofGapCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fNbofChamberCmd = new G4UIcmdWithADouble("/MRPC/det/NumberOfChamber",this);
  fNbofChamberCmd->SetGuidance("Define NbofChamber");
  fNbofChamberCmd->SetParameterName("NbofChamber",false);
  fNbofChamberCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/MRPC/det/StepMax",this);
  fStepMaxCmd->SetGuidance("Define a step max");
  fStepMaxCmd->SetParameterName("StepMax",false);
  fStepMaxCmd->SetUnitCategory("Length");
  fStepMaxCmd->AvailableForStates(G4State_Idle);

  fPitchWidthCmd = new G4UIcmdWithADoubleAndUnit("/MRPC/det/PitchWidth",this);
  fPitchWidthCmd->SetGuidance("Define pitch width, this include the strip and gap");
  fPitchWidthCmd->SetParameterName("PitchWidth",false);
  fPitchWidthCmd->SetUnitCategory("Length");
  fPitchWidthCmd->AvailableForStates(G4State_Idle);

  fUpdateCmd = new G4UIcmdWithoutParameter("/MRPC/det/update",this);
  fUpdateCmd->SetGuidance("Update detector geometry.");
  fUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  fUpdateCmd->SetGuidance("if you changed geometrical value(s).");
  fUpdateCmd->AvailableForStates(G4State_Idle);
}

MRPCDetectorMessenger::~MRPCDetectorMessenger()
{
  delete fMRPCDirectory;
  delete fDetDirectory;
  delete fGapsizeCmd;
  delete fGlasssizeCmd;
  delete fHoneysizeCmd;
  delete fPCBsizeCmd;
  delete fMylarsizeCmd;
  delete fStepMaxCmd;
  delete fNbofGapCmd;
  delete fNbofChamberCmd;
  delete fPitchWidthCmd;
  delete fUpdateCmd;
}

void MRPCDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fGapsizeCmd ) fDetectorConstruction->SetGapsize(fGapsizeCmd->GetNew3VectorValue(newValue));
  if(command == fGlasssizeCmd) fDetectorConstruction->SetGlasssize(fGlasssizeCmd->GetNew3VectorValue(newValue));
  if(command == fHoneysizeCmd) fDetectorConstruction->SetHoneysize(fHoneysizeCmd->GetNew3VectorValue(newValue));
  if(command == fPCBsizeCmd) fDetectorConstruction->SetPCBsize(fPCBsizeCmd->GetNew3VectorValue(newValue));
  if(command == fMylarsizeCmd) fDetectorConstruction->SetMylarsize(fMylarsizeCmd->GetNew3VectorValue(newValue));
  if(command == fNbofChamberCmd) fDetectorConstruction->SetNbofchamber(fNbofChamberCmd->GetNewDoubleValue(newValue));
  if(command == fNbofGapCmd) fDetectorConstruction->SetNbofGapinAChamber(fNbofGapCmd->GetNewDoubleValue(newValue));
  if( command == fStepMaxCmd ) fDetectorConstruction->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
  if( command == fPitchWidthCmd ) fDetectorConstruction->SetPitchWidth(fPitchWidthCmd->GetNewDoubleValue(newValue));
  if(command == fUpdateCmd) fDetectorConstruction->UpdateGeometry(); 
}
    
