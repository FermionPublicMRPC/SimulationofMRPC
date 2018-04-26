#include "MRPCRecorderMessenger.hh"
#include "MRPCRecorder.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"

MRPCRecorderMessenger::MRPCRecorderMessenger(MRPCRecorder *rec)
 :G4UImessenger(),
   fRecorder(rec)
{
  fMRPCDirectory = new G4UIdirectory("/MRPC/");
  fMRPCDirectory->SetGuidance("UI commands specific to this example.");

  fRecDirectory = new G4UIdirectory("/MRPC/Rec/");
  fRecDirectory->SetGuidance("Recorder control");

  fWriteCmd = new G4UIcmdWithAnInteger("/MRPC/Rec/doWrite",this);
  fWriteCmd->SetParameterName("DoWrite",true);
  fWriteCmd->SetDefaultValue(1);
  fWriteCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

MRPCRecorderMessenger::~MRPCRecorderMessenger(){
  delete fMRPCDirectory;
  delete fRecDirectory;
  delete fWriteCmd;
}

void MRPCRecorderMessenger::SetNewValue(G4UIcommand* command,G4String newValue){
 if( command ==  fWriteCmd )
   fRecorder->SetRecordFlag(fWriteCmd->GetNewIntValue(newValue));
}
