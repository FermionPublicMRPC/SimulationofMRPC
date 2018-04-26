#include "MRPCDigitizerMessenger.hh"
#include "MRPCDigitizer.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
MRPCDigitizerMessenger::MRPCDigitizerMessenger(MRPCDigitizer *dig)
  :G4UImessenger(),
   fdig(dig)
{

  fMRPCDirectory = new G4UIdirectory("/MRPC/");
  fMRPCDirectory->SetGuidance("UI commands specific to this example.");

  fDigDirectory = new G4UIdirectory("/MRPC/dig/");
  fDigDirectory->SetGuidance("Digitizer control");

  fGapsizeCmd = new G4UIcmdWith3VectorAndUnit("/MRPC/dig/Geosize",this);
  fGapsizeCmd->SetGuidance("Define Gapsize");
  fGapsizeCmd->SetParameterName("Nbofgap","Platez","Gapz",false);
  fGapsizeCmd->SetUnitCategory("Length");
  fGapsizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

MRPCDigitizerMessenger::~MRPCDigitizerMessenger(){
  delete fMRPCDirectory;
  delete fDigDirectory;
  delete fGapsizeCmd;
}

void MRPCDigitizerMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fGapsizeCmd ) fdig->SetGapsize(fGapsizeCmd->GetNew3VectorValue(newValue));
}
