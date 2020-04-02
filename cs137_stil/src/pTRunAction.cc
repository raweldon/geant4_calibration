//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "pTRunAction.hh"
#include "pTEventAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pTRunAction::pTRunAction()
 : G4UserRunAction()
{
  // set printing event number per each "printfreq" events
  G4int printfreq = 19000000;
  G4RunManager::GetRunManager()->SetPrintProgress(printfreq);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pTRunAction::~pTRunAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pTRunAction::BeginOfRunAction(const G4Run*)
{
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pTRunAction::EndOfRunAction(const G4Run* )
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
