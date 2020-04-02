//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4GeneralParticleSource.hh"
#include "pTPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"


//#include "TF1.h"
//#include "TMath.h"
//#include "TCanvas.h"
//#include "TH1F.h"

#include <cmath>
#include <sstream>

#include <iostream>
#include <fstream> 
#include <csignal>

//  C/C++ includes
#include <iostream>
#include <fstream>

using namespace std;


using namespace CLHEP;
//using namespace inputVals;


// Cf-252 spectrum
/*Double_t E_specCf(Double_t *x, Double_t *par)
   {
      Float_t xx =x[0];
      Double_t f = TMath::Exp(-xx*par[0]) * TMath::SinH(TMath::Sqrt(par[1]*xx));
      return f;
   }*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pTPrimaryGeneratorAction::pTPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0)//added with gps
{
//  G4int nofParticles = 1;
//  fParticleGun = new G4ParticleGun(nofParticles);
  fParticleGun = new G4GeneralParticleSource();
//gps
  G4GeneralParticleSource *gps = new G4GeneralParticleSource();

  // default particle kinematic
//  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
//  gps->GetCurrentSource()->GetEneDist()->SetMonoEnergy(2.0*GeV);
//  gps->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0.0*cm, 0.0*cm, 0.0*cm));
//  gps->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  

/*
//write the binary file
  cout << "BUILDING STEPPING ACTION" << endl;
  std::string txtfLocation1 = "/home/mishra/geant4/examples/pTherapy_build/n_hit_primary";
  std::string txtfEnding1 = ".bin";
  txtFileName1 = txtfLocation1 + txtfEnding1;
//  txtFile = fopen(txtFileName.c_str(),"w");
  txtFile1.open(txtFileName1.c_str(), ios::binary | ios::out);
*/
/*
//wrte the file
  cout << "BUILDING primary data" << endl;
  std::string txtfLocation1 = "/home/mishra/geant4/examples/pTherapy_build/n_hit_primary";
  std::string txtfEnding1 = ".txt";
  txtFileName1 = txtfLocation1 + txtfEnding1;
  txtFile1 = fopen(txtFileName1.c_str(),"w");
*/

//  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
//  fParticleGun->SetParticleEnergy(200.0*MeV);
//  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -30.*cm));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pTPrimaryGeneratorAction::~pTPrimaryGeneratorAction()
{
  delete fParticleGun;

//  txtFile1.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pTPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore.
/*
  G4double kineticEnergy = 662.0*keV;  
  fParticleGun->SetParticleEnergy(kineticEnergy);
  fParticleGun->SetParticleDefinition(G4Gamma::GammaDefinition());
  // the detector has z coordinate going from 0 to 4 cm (0 to boxLength), 
  // therefore we place it at (0.,0.,5.0*cm)
  G4double zOfSource = 5.0*cm; 
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,zOfSource));
  G4double cosTheta = -1.0 + 2.0*G4UniformRand();
  G4double phi = twopi*G4UniformRand();
  G4double sinTheta = sqrt(1 - cosTheta*cosTheta);
  // these are the cosines for an isotropic direction
  fParticleGun -> SetParticleMomentumDirection(G4ThreeVector(sinTheta*cos(phi),
							    sinTheta*sin(phi),
							    cosTheta));
*/


//  fprintf(txtFile1,"%12.4E  %8.3f  %8.3f  %8.3f\n",beamEnergy, x, y, z);
/*  txtFile1.write((char*)&beamEnergy, sizeof(beamEnergy));
  txtFile1.write((char*)&x, sizeof(x));
  txtFile1.write((char*)&y, sizeof(y));
  txtFile1.write((char*)&z, sizeof(z));*/


  G4double worldZHalfLength = 0;
  G4LogicalVolume* worldLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("worldLV");
  G4Box* worldBox = NULL;
  if ( worldLV ) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  if ( worldBox ) worldZHalfLength = worldBox->GetZHalfLength();
  else  {
    G4cerr << "World volume of box not found." << G4endl;
    G4cerr << "Perhaps you have changed geometry." << G4endl;
    G4cerr << "The gun will be place in the center." << G4endl;
  }
  


  fParticleGun->GeneratePrimaryVertex(anEvent);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
