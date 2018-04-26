
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "MRPCDetectorConstruction.hh"
#include "QGSP_BIC_HP.hh"
#include "MRPCPrimaryGeneratorAction.hh"
#include "MRPCRecorder.hh"
#include "MRPCRunAction.hh"
#include "MRPCEventAction.hh"
#include "G4StepLimiterPhysics.hh"

#include "MRPCPhysicsList.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc,char** argv)
{
  // Construct the default run manager
  //
  G4RunManager* runManager = new G4RunManager;

  // set mandatory initialization classes
  //
  G4VUserDetectorConstruction* detector = new MRPCDetectorConstruction;
  runManager->SetUserInitialization(detector);
  //G4VModularPhysicsList
//   G4VUserPhysicsList* physics = new QGSP_BIC_HP;
  //  runManager->SetUserInitialization(physics);
   G4VUserPhysicsList* physics = new MRPCPhysicsList();//PhysicsList();
  // physics->RegisterPhysics(new G4StepLimiterPhysics());
  runManager->SetUserInitialization(physics);

  // set mandatory user action class
  //
  
  MRPCRecorder* recorder = new MRPCRecorder();
  
  G4VUserPrimaryGeneratorAction* gen_action = new MRPCPrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);
  MRPCRunAction* run_action = new MRPCRunAction(recorder);
  runManager->SetUserAction(run_action);
  //
  MRPCEventAction* event_action = new MRPCEventAction(recorder);
  runManager->SetUserAction(event_action);
  

  // Initialize G4 kernel
  //
  runManager->Initialize();

#ifdef G4VIS_USE
    // Initialize visualization
    //
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // Get the pointer to the UI manager and set verbosities
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer();



  if (argc!=1)   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }
  else
  
    {  // interactive mode : define UI session
  
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");
#endif
  
      ui->SessionStart();
      delete ui;
#endif
    }
  


  // Job termination
  //
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
  //

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  // delete recorder;


  return 0;
}


