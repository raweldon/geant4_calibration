//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef pTPrimaryGeneratorAction_h
#define pTPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"
//#include "TF1.h"
//#include "TH1F.h"
//  C/C++ includes
#include <vector>
#include <fstream>
using namespace std;

//class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;

// The primary generator action class with particle gun.
// Defines a single particle that hits the Tracker perpendicular to the input face.
// The type of the particle can be changed via the G4 build-in commands of G4ParticleGun
// class in macro files.

class pTPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    pTPrimaryGeneratorAction();
    virtual ~pTPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* );

//    G4ParticleGun* GetParticleGun() {return fParticleGun;}

  private:
//    G4ParticleGun*          fParticleGun;      // G4particle gun
    G4GeneralParticleSource*          fParticleGun;      // G4particle gun
//    TF1      *fa1;
//    std::string           txtFileName1;
//    FILE*                     txtFile1;
//    ofstream                  txtFile1;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
