#ifndef pTActionInitialization_h
#define pTActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class pTDetectorConstruction;

/// Action initialization class.
///

class pTActionInitialization : public G4VUserActionInitialization
{
  public:
    pTActionInitialization();
    virtual ~pTActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif
