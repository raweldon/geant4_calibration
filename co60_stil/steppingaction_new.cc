#include "Randomize.hh"

//  Geant4 includes
#include "G4RandomDirection.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"

//  Application includes
#include "pTDetectorConstruction.hh"
#include "pTEventAction.hh"
#include "pTSteppingAction.hh"

//  C/C++ includes
#include <iostream>
#include <fstream>

using namespace std;


//G4int pTSteppingAction::prevStepNo = -1;
//G4int pTSteppingAction::prevTrackID = 0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pTSteppingAction::pTSteppingAction(pTEventAction* event):myEvent(event)
{
  cout << "BUILDING STEPPING ACTION" << endl;
  std::string txtfLocation = "/home/mishra/geant4/examples/pTherapy_build/n_hit_pattern";
  std::string txtfEnding = ".bin";
  txtFileName = txtfLocation + txtfEnding;
//  txtFile = fopen(txtFileName.c_str(),"w");
  txtFile.open(txtFileName.c_str(), ios::binary | ios::out);

//  if(getenv("SLURM_ARRAY_TASK_ID"))
//  {
//    std::string txtfLocation = "/local/AM_geant/pTherapy_n";
//    std::string txtfTaskID = getenv("SLURM_ARRAY_TASK_ID");
//    std::string txtfEnding = ".txt";
//    txtFileName = txtfLocation + txtfTaskID + txtfEnding;
//    txtFile = fopen(txtFileName.c_str(),"a");
//  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pTSteppingAction::~pTSteppingAction()
{
  txtFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pTSteppingAction::UserSteppingAction(const G4Step* aStep)
{

  G4Track* track = aStep->GetTrack();
  G4String particleName = track->GetDefinition()->GetParticleName();

  if(track->GetVolume() != NULL){
    if(aStep->GetPostStepPoint()->GetProcessDefinedStep() != 0){
      if (track->GetVolume()->GetName() == "detectorPV"){
        if(particleName == "neutron"){
          G4String origin = track->GetOriginTouchable()->GetVolume()->GetName();
          G4int parentTrackID = track->GetParentID();
//          if(origin == "waterphantomPV"){fprintf(txtFile,"%2d ", 1);} else {fprintf(txtFile,"%2d ", 0);}
          if(parentTrackID == 0){outfile.write(&parentTrackID, sizeof(G4int));} 

//          if(prevTrackID == trackID && prevStepNo == stepNumber - 1) {
//            track->SetTrackStatus(fKillTrackAndSecondaries);
//            prevStepNo = -1;
//            prevTrackID = 0;
//          } else {
//            prevStepNo = stepNumber;
//            prevTrackID = trackID;
//            if(track->GetNextVolume()->GetName() == "worldPV" || aStep->GetPostStepPoint()->GetKineticEnergy() == 0 || aStep->IsLastStepInVolume() !=0){
//              fprintf(txtFile," 0  0   00  00     00.000   000.000   000.000     0000.000    0.0000E+00    0.0000E+00  NANANANANANANANA  -NANANA");
//              fprintf(txtFile,"     00.000   000.000   000.000    00000.000    0.0000E+00       0.000     0.000     0.000\n");
//              track->SetTrackStatus(fKillTrackAndSecondaries);
//              prevStepNo = -1;
//              prevTrackID = 0;
//            }
//          }
        }
      }
    }
  }
}


//{

//  G4Track* track = aStep->GetTrack();
//  G4String particleName = track->GetDefinition()->GetParticleName();

//  if(track->GetVolume() != NULL){
//    if(aStep->GetPostStepPoint()->GetProcessDefinedStep() != 0){
//      if (track->GetVolume()->GetName() == "detectorPV"){
//        if(particleName == "neutron"){
//          G4String origin = track->GetOriginTouchable()->GetVolume()->GetName();
//          G4int parentTrackID = track->GetParentID();
//          if(origin == "waterphantomPV"){fprintf(txtFile,"%2d ", 1);} else {fprintf(txtFile,"%2d ", 0);}
//          if(parentTrackID == 1){fprintf(txtFile,"%2d ", 1);} else {fprintf(txtFile,"%2d ", 0);}

//          G4int trackID = track->GetTrackID();
//          G4int stepNumber = track->GetCurrentStepNumber();
//          G4String processName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
//          G4ThreeVector stepPosition = aStep->GetPreStepPoint()->GetPosition();
//          G4double trackTime = aStep->GetPreStepPoint()->GetGlobalTime();
          // G4double trackkineticEnergy = track->GetKineticEnergy(); // gets particle energy "after" collision
//          G4double stepkineticEnergy = aStep->GetPreStepPoint()->GetKineticEnergy(); // gets particle energy "before" collision
//          G4double depositedEnergy = aStep->GetDeltaEnergy();

//          fprintf(txtFile,"%4d %3d  %9.3f  %8.3f  %8.3f  ", trackID, stepNumber, stepPosition.x(), stepPosition.y(), stepPosition.z());
//          fprintf(txtFile,"%11.3f  %12.4E  %12.4E  %16s", trackTime, stepkineticEnergy, depositedEnergy, processName.c_str());

//          const std::vector<const G4Track*>* secondaries = aStep->GetSecondaryInCurrentStep();
//          if (secondaries->size()>0) {
//            G4String secParticleName = secondaries->at(0)->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();
//            const G4ThreeVector secTrackPosition = secondaries->at(0)->GetPosition();
            
//            const G4double secTrackGlobalTime = secondaries->at(0)->GetGlobalTime();
//            const G4double secTrackKineticEnergy = secondaries->at(0)->GetKineticEnergy();
//            const G4ThreeVector secTrackMomentumDirection = secondaries->at(0)->GetMomentumDirection();
            
//            fprintf(txtFile,"%9s  %9.3f  %8.3f  %8.3f  ", secParticleName.c_str(), secTrackPosition.x(), secTrackPosition.y(), secTrackPosition.z());
//            fprintf(txtFile,"%11.3f  %12.4E   ", secTrackGlobalTime, secTrackKineticEnergy);
//            fprintf(txtFile," %8.3f  %8.3f  %8.3f\n", secTrackMomentumDirection.x(), secTrackMomentumDirection.y(), secTrackMomentumDirection.z());


//          } else { fprintf(txtFile,"     None     00.000   000.000   000.000    00000.000    0.0000E+00       0.000     0.000     0.000\n"); }
//          if(prevTrackID == trackID && prevStepNo == stepNumber - 1) {
//            track->SetTrackStatus(fKillTrackAndSecondaries);
//            prevStepNo = -1;
//            prevTrackID = 0;
//          } else {
//            prevStepNo = stepNumber;
//            prevTrackID = trackID;
//            if(track->GetNextVolume()->GetName() == "worldPV" || aStep->GetPostStepPoint()->GetKineticEnergy() == 0 || aStep->IsLastStepInVolume() !=0){
//              fprintf(txtFile," 0  0   00  00     00.000   000.000   000.000     0000.000    0.0000E+00    0.0000E+00  NANANANANANANANA  -NANANA");
//              fprintf(txtFile,"     00.000   000.000   000.000    00000.000    0.0000E+00       0.000     0.000     0.000\n");
//              track->SetTrackStatus(fKillTrackAndSecondaries);
//              prevStepNo = -1;
//              prevTrackID = 0;
//            }
//          }
//        }
//      }
//    }
//  }
//}



