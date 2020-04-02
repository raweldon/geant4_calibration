//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef pTEventAction_h
#define pTEventAction_h 1

#include "G4UserEventAction.hh"

#include "globals.hh"

#include <iostream>
#include <fstream>

using namespace std;

/// Event action class

class pTEventAction : public G4UserEventAction
{
  public:
    pTEventAction();
    virtual ~pTEventAction();

    virtual void  BeginOfEventAction(const G4Event* );
    virtual void    EndOfEventAction(const G4Event* );
    void AddEdep(G4double edep) { fEdep += edep; }
    void AddgEdep(G4double edep1) { fgEdep += edep1; }
    void getEID(G4double eid) { feid = eid; }

  private:
//    pTRunAction* fRunAction;
    G4double     fEdep;
    G4double     fgEdep;
    G4double     feid;
    std::string           txtFileName1;
    FILE*                     txtFile1;
  };

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
