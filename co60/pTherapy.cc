//....oooOO0OOooo......Version description......oooOO0OOooo......oooOO0OOooo....oooOO0OOooo...//
// 1. Detector & WaterPhantom brought close to the minimum distance and detector is rotated   //
//     30 degrees.                                                                            //
// 2. Using SteppingAction to output to a txt file:                                           //
//     a. Only the first two interactions of neutrons directly from primary protons           //
//           in the detector                                                                  //
//     b. All the needed secondaries informations to characterize the detector response.      //
// 3. Using SLURM to parallelize several independent runs in order to finally get very large  //
//     number of events in timely manner.                                                     //
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......oooOO0OOooo...//

#include "pTDetectorConstruction.hh"
#include "pTActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
//#include "QGSP_BERT.hh"
#include "QGSP_BERT_HP.hh"
#include "G4StepLimiterPhysics.hh"

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Change seed for doing several parallel independent runs Using SLURM on cluster
  G4long seed = time(0);
//  if(getenv("SLURM_ARRAY_TASK_ID"))
//  {
//    seed += atoi(getenv("SLURM_ARRAY_TASK_ID"))*19000000;
//  }

  CLHEP::HepRandom::setTheSeed(seed);

  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(4);
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  //
  runManager->SetUserInitialization(new pTDetectorConstruction());

  G4VModularPhysicsList* physicsList = new QGSP_BERT_HP(1);
  physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  runManager->SetUserInitialization(physicsList);

  // Set user action classes
  runManager->SetUserInitialization(new pTActionInitialization());

  // Initialize G4 kernel
  runManager->Initialize();

  // Initialize visualization
  #ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
  #endif

    // Get the pointer to the User Interface manager
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
          UImanager->ApplyCommand("/control/execute init_vis.mac");
  #else
          UImanager->ApplyCommand("/control/execute init.mac");
  #endif
        if (ui->IsGUI())
           UImanager->ApplyCommand("/control/execute gui.mac");
        ui->SessionStart();
        delete ui;
  #endif
      }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !

  #ifdef G4VIS_USE
    delete visManager;
  #endif
    delete runManager;

    return 0;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
