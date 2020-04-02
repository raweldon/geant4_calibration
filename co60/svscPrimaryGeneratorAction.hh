#ifndef svscPrimaryGeneratorAction_h
#define svscPrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "svscSteppingAction.hh"
#include "G4RunManager.hh"
#include "TF1.h"
#include "TH1F.h"

class G4ParticleGun;
class G4Event;
class svscSteppingAction;

class svscPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  svscPrimaryGeneratorAction();
  ~svscPrimaryGeneratorAction();
  
public:
  void GeneratePrimaries(G4Event* anEvent);
  virtual G4String GetParticleName() {return particleName;}
  virtual G4double GetScintPositionX() {return posX;} 
  virtual G4double GetScintPositionY() {return posY;}
  virtual G4double GetScintPositionZ() {return posZ;}
  bool write2File;
  int  usePolimi;
  int  pixel;
  int  vecIndex;
  void calcPositionAndPixelNum();
  void checkLegalPositions();
  

private:
  const G4double PI;
  
  svscSteppingAction *stepAct;
  G4ParticleGun* particleGun;
  G4String particleName;
  G4double posX;
  G4double posY;
  G4double posZ;
  G4double dirX;
  G4double dirY;
  G4double dirZ;
  G4double time;
  G4bool   useEmissionSpectrum;
  G4int    numPhotonsPerMeVee;
  G4int    i,j,k;  
  TF1      *fa1;
  TH1F      *scintEmission;

  int      histEvents;
  double   sRise;
  double   sFall;

  

}; 
  
#endif
