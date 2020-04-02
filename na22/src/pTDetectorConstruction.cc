//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <fstream>
#include <iostream>

#include "pTDetectorConstruction.hh"


//------------------------------
// Constructing a physical setup and volume in an G4 experiment is based on:
// 1- defining a solid (the below classes are needed).
// 2- defining a logical volume (material etc... ). So, "G4LogicalVolume.hh" is needed.
// 3- defining a physical volume and place it in the 3D space X Y and Z. "G4VPhysicalVolume.hh" & "G4PVPlacement.hh" are needed.
// 4- an option to replicate some volume. So, "G4PVReplica.hh" is needed.
//------------------------------

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4AutoDelete.hh"
#include "G4ThreeVector.hh"

//------------------------------
// Classes needed to define solids
//------------------------------
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"

//------------------------------
// Classes needed to define materials
//------------------------------
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"

/*
//------------------------------
// For a more complexe geometry, some boolean operations on simple volume are needed to create some fancy geometry.
// In G4 we have three boolean operation on solids. Subtruction, Union and Intersection.
// In this project we use only Subtruction, Union as an example. You need G4SubtractionSolid.hh and G4UnionSolid.hh.
// For Intersection the proceedure is similar to Subtruction, Union.
//------------------------------
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
*/

//------------------------------
// Simply needed to define a rotation matrix
//------------------------------
#include "G4RotationMatrix.hh"

//------------------------------
// Needed to set some constraints on a given logical volume
//------------------------------
#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"

#include "G4ios.hh"


#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"


//-------------------------------
pTDetectorConstruction::pTDetectorConstruction()
:G4VUserDetectorConstruction(),
fCheckOverlaps(true)
{

}

pTDetectorConstruction::~pTDetectorConstruction()
{
  /* Make sure that deleting them won't affect scoring
  delete air;
  delete water;
  delete world;
  delete worldLV;
  delete worldPV;
  delete waterphantom;
  delete waterphantomLV;
  delete waterphantomPV;
  delete fStepLimit;
  */
}

//------------------------------
G4VPhysicalVolume* pTDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//-------------------------------
// Materials Definition
//-------------------------------
void pTDetectorConstruction::DefineMaterials()
{
  // Material definition
  G4NistManager* nistManager = G4NistManager::Instance();

  // Air and Water are defined using NIST Manager
  air = nistManager->FindOrBuildMaterial("G4_AIR");
  water = nistManager->FindOrBuildMaterial("G4_WATER");
  hydrogen = nistManager->FindOrBuildMaterial("G4_H");
  carbon = nistManager->FindOrBuildMaterial("G4_C");
  // EJ-309
  ej309 = new G4Material(name="ej309", 0.959*g/cm3, numComponents = 2);
  ej309->AddMaterial(hydrogen, fractionmass=0.094);
  ej309->AddMaterial(carbon, fractionmass=0.906);
 
  G4double density = 0;
  G4int natoms = 0;
  G4int z = 0; // atomic number
  // Hydrogen
  z = 1;
  G4Element *elH = nistManager->FindOrBuildElement(z);
  
  // Carbon
  z = 6;
  G4Element *elC = nistManager->FindOrBuildElement(z);
  // EJ-204 / BC-404 / NE-104
  density = 1.023*g/cm3;
  ej204 = new G4Material("ej204",density,numComponents  = 2);
  ej204->AddElement(elH,natoms = 515);
  ej204->AddElement(elC,natoms = 468);

  // Stilbene
  density = 1.15*g/cm3;
  stilbene = new G4Material("stilbene",density,numComponents  = 2);
  stilbene->AddElement(elH,natoms = 12);
  stilbene->AddElement(elC,natoms = 14);

  // EJ-228
  density = 1.023*g/cm3;
  ej228= new G4Material("ej228",density,numComponents  = 2);
  ej228->AddElement(elH,natoms = 515);
  ej228->AddElement(elC,natoms = 469);

  // Print materials
  // G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//------------------------------------
// Volume definition
//------------------------------------
G4VPhysicalVolume* pTDetectorConstruction::DefineVolumes()
{
  //----------world----------
  G4double worldLength = 1.5*m;
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  G4Box* world
    = new G4Box("world",                          // its name
                25.0*cm/2,25.0*cm/2,25.0*cm/2);         // its size

  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 world,                           // its solid
                 air,                             // its material
                 "worldLV",                       // its name
                 0,0,0);

  // Placing the World Physical volume unrotated at (0,0,0).
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,                               // no rotation
                 G4ThreeVector(),                 // at (0,0,0)
                 worldLV,                         // its logical volume
                 "worldPV",                       // its name
                 0,                               // its mother  volume
                 false,                           // no boolean operations
                 0,                               // copy number
                 fCheckOverlaps);                 // checking overlaps

  //----------Water Phantom-----------
//  G4VSolid* waterphantom
//    = new G4Tubs("waterphantom",                      // its name
//                0.,50.*mm,350.*mm/2,0.*deg,360.*deg); // its size

//  G4LogicalVolume* waterphantomLV
//    = new G4LogicalVolume(
//                 waterphantom,                    // its solid
//                 water,                           // its material
//                 "waterphantomLV");               // its name

  // Placing the Water Phantom physical volume at (0,0,0).
//  G4VPhysicalVolume* waterphantomPV
//    = new G4PVPlacement(
//                 0,                               // no rotation
//                 G4ThreeVector(),                 // at (0,0,0)
//                 waterphantomLV,                  // its logical volume
//                 "waterphantomPV",                // its name
//                 worldLV,                         // its mother volume
//                 false,                           // no boolean operations
//                 0,                               // copy number
//                 fCheckOverlaps);                 // checking overlaps

  //----------Detector-----------
  G4Box* detector
    = new G4Box("detector",                          // its name
                1.0*cm/2,1.0*cm/2,1.0*cm/2);   // its size

  G4LogicalVolume* detectorLV
    = new G4LogicalVolume(
                 detector,                        // its solid
                 ej228,                           // its material
                 "detectorLV",                    // its name
                 0,0,0);

  // Placing the detector Physical volume rotated y 30*deg at (0,160.*mm,275.*mm).
//  G4RotationMatrix* rotate = new G4RotationMatrix(0,60.*deg,0);
  G4VPhysicalVolume* detectorPV
    = new G4PVPlacement(
                 0,                          //  rotation
                 G4ThreeVector(),  // at (0,0,0)
                 detectorLV,                      // its logical volume
                 "detectorPV",                    // its name
                 worldLV,                         // its mother  volume
                 false,                           // no boolean operations
                 0,                               // copy number
                 fCheckOverlaps);                 // checking overlaps



  //----------Visualization attributes-----------
  G4VisAttributes * White = new G4VisAttributes( G4Colour(1.0, 1.0, 1.0) );
  G4VisAttributes * Grey  = new G4VisAttributes( G4Colour(0.5, 0.5, 0.5) );

  worldLV         ->SetVisAttributes(White);
//  waterphantomLV  ->SetVisAttributes(Grey);
  detectorLV  ->SetVisAttributes(White);


  /*
  // Sets a max step length in the tracker region, with G4StepLimiter
  G4double maxStep = 150.*mm;
  fStepLimit = new G4UserLimits(maxStep);
  trackerLV->SetUserLimits(fStepLimit);
  */


  // Always return the physical world
  return worldPV;
}

/*
//-------------------------------
// Set sensitive detector
//-------------------------------

void pTDetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors

  G4String trackerChamberSDname = "pTherapy/TrackerChamberSD";
  pTTrackerSD* aTrackerSD = new pTTrackerSD(trackerChamberSDname,
                                            "TrackerHitsCollection");
  // Setting aTrackerSD to all logical volumes with the same name
  // of "Chamber_LV".
  SetSensitiveDetector("detectorLV", aTrackerSD, true);
}
*/

//-------------------------------
// Check for overlaps
//-------------------------------
void pTDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  fCheckOverlaps = checkOverlaps;
}
