//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "pTEventAction.hh"


#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4VAnalysisManager.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"

//   C/C++ includes
#include <iostream>
#include <iomanip>
#include <unistd.h>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
pTEventAction::pTEventAction()
: G4UserEventAction(),
  fEdep(0.),
  fgEdep(0.),
  feid(0.)
{
//wrte the file

  cout << "BUILDING primary data" << endl;
  char * currentDir = getcwd(NULL, 0);
  std::string txtfLocation1 = "/gamma_hit_primary";
  std::string txtfEnding1 = ".txt";
  txtFileName1 = currentDir + txtfLocation1 + txtfEnding1;
  txtFile1 = fopen(txtFileName1.c_str(),"w");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pTEventAction::~pTEventAction()
{
 fclose(txtFile1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pTEventAction::BeginOfEventAction(const G4Event* evt)
{
  fEdep = 0.;
  fgEdep = 0.;
  feid = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pTEventAction::EndOfEventAction(const G4Event* evt)
{
  G4int evtNb = evt->GetEventID();
  if (fgEdep < 0.){
  fprintf(txtFile1,"%10d  %12.4E  %12.4E\n",evtNb, fEdep, fgEdep);}
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
