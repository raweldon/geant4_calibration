//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef pTDetectorConstruction_h
#define pTDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "tls.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VPVParameterisation;
class G4PVReplica;
class G4UserLimits;

//--------------------------------------------------------------
// Detector construction class to define materials and geometry.
//--------------------------------------------------------------

class pTDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
     pTDetectorConstruction();
     virtual ~pTDetectorConstruction();

  public:
     virtual G4VPhysicalVolume* Construct();

     // Set methods
     void SetCheckOverlaps(G4bool );

  private:
     // methods
     void DefineMaterials ();
     G4VPhysicalVolume* DefineVolumes();

     // data members
     G4Material* air;
     G4Material* water;
     G4Material* hydrogen;
     G4Material* carbon;
     G4Material* ej309;
     G4Material* ej204;
     G4Material* ej228;
     G4Material* stilbene;

     G4String name;
     G4int numComponents;
     G4double fractionmass;
     G4bool  fCheckOverlaps;  // option to activate checking of volumes overlaps
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
