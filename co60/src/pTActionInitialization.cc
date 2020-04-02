#include "pTActionInitialization.hh"
#include "pTPrimaryGeneratorAction.hh"
#include "pTRunAction.hh"
#include "pTEventAction.hh"
#include "pTSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pTActionInitialization::pTActionInitialization()
 : G4VUserActionInitialization()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pTActionInitialization::~pTActionInitialization()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pTActionInitialization::BuildForMaster() const
{
  SetUserAction(new pTRunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pTActionInitialization::Build() const
{
  pTPrimaryGeneratorAction* generatorAction = new pTPrimaryGeneratorAction();
  SetUserAction(generatorAction);

  pTRunAction* runAction = new pTRunAction();
  SetUserAction(runAction); 

  pTEventAction* eventAction = new pTEventAction();
  SetUserAction(eventAction);

  pTSteppingAction* steppingAction = new pTSteppingAction(eventAction);
  SetUserAction(steppingAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
