#ifndef pTSteppingAction_h
#define pTSteppingAction_h 1

//  Geant4 includes
#include "G4DynamicParticle.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4ParticleGun.hh"
#include "G4UserSteppingAction.hh"
#include "pTEventAction.hh"

//  C/C++ includes
#include <vector>
#include <fstream>

class pTDetectorConstruction;
class pTEventAction;

class pTSteppingAction : public G4UserSteppingAction
{
public:
  pTSteppingAction(pTEventAction* );
  ~pTSteppingAction();

  void UserSteppingAction(const G4Step*);

//  static G4int prevStepNo;
//  static G4int prevTrackID;

private:
  pTEventAction*            myEvent;
  std::string           txtFileName;
//  FILE*                     txtFile;
  ofstream                  txtFile;
};

#endif
