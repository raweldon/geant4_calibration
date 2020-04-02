//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef pTRunAction_h
#define pTRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class pTEventAction;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

/// Run action class

class pTRunAction : public G4UserRunAction
{
  public:
    pTRunAction();
    virtual ~pTRunAction();
    // Where is GenerateRun() ?!!
    virtual void BeginOfRunAction(const G4Run* run);
    virtual void   EndOfRunAction(const G4Run* run);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
