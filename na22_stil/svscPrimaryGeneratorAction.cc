#include "svscPrimaryGeneratorAction.hh"
#include "svscDetectorConstruction.hh"
#include "svscSteppingAction.hh"
#include "svscRunAction.hh"
#include "inputFile.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4Timer.hh"

#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH1F.h"

#include <cmath>
#include <sstream>

#include <iostream>
#include <fstream> 
#include <csignal>

using namespace CLHEP;
//using namespace inputVals;

// Double exponential equation for scintillation light
Double_t doubleExp(Double_t *x, Double_t *par)
   {
      Float_t xx =x[0];
      Double_t f = TMath::Exp(-xx/par[1]) - TMath::Exp(-xx/par[0]);
      return f;
   }

svscPrimaryGeneratorAction::svscPrimaryGeneratorAction()
:
  PI( CLHEP::pi ), numPhotonsPerMeVee(11500), i(0),j(0),k(0)
{

  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  particleName = "opticalphoton"; 
  particleGun->SetParticleDefinition(particleTable->FindParticle(particleName));
  stepAct = (svscSteppingAction*)G4RunManager::GetRunManager()->GetUserSteppingAction();
  
  svscRunAction *runAct = (svscRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
  usePolimi = runAct->usePolimi;
  
  // If option for use polimi File is used, use polmi file foor time position and numPhotons, else use beamOn.dat
  if (usePolimi)
  {
    vecIndex = 0;
    time = runAct->time[vecIndex];
    calcPositionAndPixelNum();
    histEvents = runAct->numPhotons[vecIndex];
  }
  else 
  {
    time = 0;
    posX = (runAct->scintX)*cm;
    posY = (runAct->scintY)*cm;
    posZ = (runAct->scintZ)*cm;
  }
 
  sRise = runAct->scintRise;
  sFall = runAct->scintFall;
  
  // Determine time of photon creation by pulling random time from double exponential
  // with rise and fall time specified in beamOn.dat
  //TF1 * fa1 = new TF1("fa1","exp(-x/4.05)-exp(-x/0.1)",0,20);
  fa1 = new TF1("fa1",doubleExp,0,20,2);
  fa1->SetParameters(sRise,sFall);     // THIS CAUSES SIMULATION TO BE 10x slower!
  
  /*
  std::ifstream inFile("ej204.dat");
  
  double bins[135];
  double intensityArray[134];
  
  double wavelength;
  double intensity;
  for (int i = 0; i < 134; i++){
    inFile >> wavelength >> intensity;
    bins[i] = wavelength;
    intensityArray[i] = intensity;
    
    G4cout << wavelength << " " << intensity << G4endl;
  }
  bins[134] = 500.1;
  scintEmission = new TH1F("emission","emission", 134, bins);
  
  for (int i = 0; i < 134; i++){
    scintEmission->SetBinContent(i+1, intensityArray[i]);
  }
  //scintEmission->Write();
  //TCanvas * c = new TCanvas("name","name");
  //scintEmission->Draw();
  */
  
  inputFile * parameters = new inputFile();
  parameters->PrintInputs();
  std::ifstream inFile;
  
  //std::cerr << "Use Emission Spectrum: " << parameters->UseEmissionSpectrum() << std::endl;
  if (!parameters->UseEmissionSpectrum())
  {
    useEmissionSpectrum = false;
    
    
    G4cout << G4endl;
    G4cout << "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>" << G4endl;
    G4cout << "<> Not using scintillator emission file, using default optical photon wavelength (425 nm)" << G4endl;
    G4cout << "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>" << G4endl;
    G4cout << G4endl;
  }
  else
  {
    useEmissionSpectrum = true;
    std::string scintEmissionFile = parameters->GetScintEmissionName();
    inFile.open(scintEmissionFile.c_str());
    try
    {
      
      if (!inFile.good())
      {
        throw scintEmissionFile.c_str();
      }
      
        double tWavelength, tIntensity;
    
        std::vector<double> wavelength;
        std::vector<double> intensity;
        
        // Read in scintillator wavelength/intensity file
        while(true)
        {
            inFile >> tWavelength >> tIntensity;
            if (inFile.eof()) // check for end of file before writing to vectors
            {
              inFile.close();
              break;
            }
            wavelength.push_back(tWavelength);
            intensity.push_back(tIntensity);
        }
        
        
        int nBins = wavelength.size();
        double xLow = wavelength[0]; 
        double xHigh = wavelength[nBins-1];
  
  
  //cout << "bins:" << nBins << "  xlow: " << xLow << "  xHigh: " << xHigh << endl;
 
  scintEmission = new TH1F("emission","emission", nBins, xLow, xHigh);
  
  for (int i = 0; i < nBins; i++)
  {
    //Root histograms start at 1, bin 0 is the underflow bin.
    scintEmission->SetBinContent(i+1, intensity[i]);
  }
    }
    catch (...)
    { 
      // print to std::cerr as well so we can see it in the SLURM stdout and error outputs
      if(getenv("SLURM_PROCID") && getenv("SLURM_JOBID")) {
        std::cerr << std::endl;
        std::cerr << "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>" << std::endl;
        std::cerr << "<> Scintillator Emission File '" << scintEmissionFile.c_str() << "' does not exist!!" << std::endl;
        std::cerr << "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Exiting Simulation..." << std::endl;
      }
      
      G4cout << G4endl;
      G4cout << "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>" << G4endl;
      G4cout << "<> Scintillator Emission File '" << scintEmissionFile.c_str() << "' does not exist!!" << G4endl;
      G4cout << "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>" << G4endl;
      G4cout << G4endl;
      G4cout << "Exiting Simulation..." << G4endl;
      
      std::raise(SIGTERM);
      
      
    }
  }
  delete parameters;
  
  
  //std::ifstream inFile("ej204.dat");
  
  
  
  
}

svscPrimaryGeneratorAction::~svscPrimaryGeneratorAction()
{
  G4cout << "Deleting Primary Generator Action!!" << G4endl;
  delete particleGun;
  delete fa1;
  delete scintEmission;
}



void svscPrimaryGeneratorAction::GeneratePrimaries(G4Event * anEvent)
{
  int eventNum = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent();
  //if (eventNum % 1000 == 0 && !usePolimi)
  // { 
  //  G4cout << "Event Number: " << eventNum << G4endl;
  //}
  //G4cout << eventNum << G4endl;
  
  // 
  
  
  // move to next interaction in polimi file
  if (usePolimi && eventNum >= histEvents)
  {
    vecIndex++;
    checkLegalPositions();
    
    calcPositionAndPixelNum();
    svscRunAction *runAct = (svscRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
    histEvents = runAct->numPhotons[vecIndex];
    time = runAct->time[vecIndex];

    int numEvents = runAct->numEvents;
    int percent = eventNum*100/numEvents;
    G4cout << "Running...  " << percent << "%" << " complete" << G4endl;

  }
  
  double scintTime;
  // delta function for time, no spread
  if (sRise <= 0.00001 && sFall <= 0.00001)
    scintTime = 0;
  else
    scintTime = fa1->GetRandom(); 
  
  int numPhotonsProduced = 1;
  
  //G4double beamEnergy = 2.92*eV; // (425 nm)
  //G4double beamEnergy = 3.44*eV; // (360 nm)
  //G4double beamEnergy = 3.666*eV; 
  G4double beamEnergy;

  for(int p = 0; p < numPhotonsProduced; p++) 
  {    
    write2File = true;
    
    
    
    
    if (useEmissionSpectrum)
    {
      G4double wavelength  = scintEmission->GetRandom(); //(nm)
      beamEnergy = 1.2398/wavelength * 1000. * eV; // conversion to eV
      
      stepAct->SetWavelength(wavelength);
    }
    else
    {
      beamEnergy = 2.92*eV;  // (425 nm)
      
    }
    
	// ================= For position sampling, Ahmed Moustafa
	double twopi = 2*TMath::Pi();
	G4double x = 0.0;
    G4double y = 0.0;
    G4double z = 0.0;
	G4double radius = 1000; // mm
	G4double cosTheta = 2*G4UniformRand() - 1.;
	G4double phi = twopi*G4UniformRand();
	G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
    x = radius*sinTheta*std::cos(phi);
	y = radius*sinTheta*std::sin(phi);
    z = radius*cosTheta;
    //================= end, Ahmed Moustafa
    
	double twopi = 2*TMath::Pi();
    G4double cosTheta = 2*G4UniformRand() - 1.;
    G4double phi = twopi*G4UniformRand();
    G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
    G4double ux = sinTheta*std::cos(phi);
    G4double uy = sinTheta*std::sin(phi);
    G4double uz = cosTheta;
    
    
    stepAct->setDirections(ux, uy, uz, phi*TMath::RadToDeg(), TMath::ACos(cosTheta)*TMath::RadToDeg());
    
    // write the stats that don't change to a tree once
    if (!eventNum)
    {
      stepAct->WriteStaticStatsTree();
    }
    
    
    //stepAct->setDirections(G4ThreeVector(0,0,1));
    particleGun->SetParticlePosition(G4ThreeVector(posX,posY,posZ));  
    //particleGun->SetParticlePolarization(G4ThreeVector(dirX,dirY,dirZ)); // random polarization
    //particleGun->SetParticlePolarization(G4ThreeVector(dirX,dirY,dirZ)); // random polarization
    //particleGun->SetParticleMomentumDirection(G4ThreeVector(0,1,.2));
    //particleGun->SetParticleMomentumDirection(G4ThreeVector(0.7,0.6,0.5)); //TIR pic, scint position at (0,0,0_
    //particleGun->SetParticleMomentumDirection(G4ThreeVector(0.55,0.9,0.5)); //escape pic, scint position at (0,0,6)
    //articleGun->SetParticleMomentumDirection(G4ThreeVector(0.4,0.9,0.5)); 
    particleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
    //particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0.75, 0.));
    particleGun->SetParticleTime(scintTime+time);
    particleGun->SetParticleEnergy(beamEnergy);
    particleGun->GeneratePrimaryVertex(anEvent);
    
  }
}

//===============================================================================
//===============================================================================
//===============================================================================

/*
// This function calculates which pixel number the event is in given and arbitrary number of scintillator blocks
// in both the x and y direction. The pixel count starts from upper increasing horizontally first then vertically.
// The upper left corner is (0 cm, 0 cm) in (x,y). This function is only used when PolimiFile is used
// ex. for 3 x 3.
// ____________
//\_1_\_2_\_3_\
//\_4_\_5_\_6_\
//\_7_\_8_\_9_\
*/

void svscPrimaryGeneratorAction::calcPositionAndPixelNum()
{
  svscRunAction *runAct = (svscRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
  
  double scintX = runAct->xIn[vecIndex];
  double scintY = runAct->yIn[vecIndex];
  double scintZ = runAct->zIn[vecIndex];
  double blockX = runAct->blockX;
  double blockY = runAct->blockY;
  double blockZ = runAct->blockZ;
  double numBlocksX = runAct->numBlocksX;
 
  int subXbars = scintX/blockX;
  int subYbars = scintY/blockY;
  
  pixel = (subXbars+1)+numBlocksX*subYbars;
  
  double tempX = scintX-subXbars*(blockX);
  double tempY = scintY-subYbars*(blockY);
  
  double cX = blockX/2;
  double cY = blockY/2;
  
  posX = (tempX - cX)*cm;
  posY = (tempY - cY)*cm;  
  posZ = scintZ*cm-(blockZ/2)*cm;
  /*
  G4cout << G4endl << G4endl;
  G4cout << "===================================================" << G4endl;
  G4cout << "ScintX: " << scintX << G4endl;
  G4cout << "ScintY: " << scintY << G4endl;
  G4cout << "ScintZ: " << scintZ << G4endl;
  G4cout << "blockX: " << blockX << G4endl;
  G4cout << "blockY: " << blockY << G4endl;
  G4cout << "NumXbl: " << numBlocksX << G4endl;
  G4cout << "subX  : " << subXbars << G4endl;
  G4cout << "subY  : " << subYbars << G4endl;
  G4cout << "tempX : " << tempX << G4endl;
  G4cout << "tempY : " << tempY << G4endl;
  G4cout << "cX    : " << cX << G4endl;
  G4cout << "cY    : " << cY << G4endl;
  G4cout << "posX  : " << posX << G4endl;
  G4cout << "posY  : " << posY << G4endl;
  G4cout << "posZ  : " << posZ << G4endl;
  G4cout << "===================================================" << G4endl;
  G4cout << G4endl << G4endl;
  */
  
  


}

//===============================================================================
//===============================================================================
//===============================================================================

void svscPrimaryGeneratorAction::checkLegalPositions()
{
  svscRunAction *runAct = (svscRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
  double tX = runAct->xIn[vecIndex];
  double tY = runAct->yIn[vecIndex];
  double tZ = runAct->zIn[vecIndex];
  
  double maxX = runAct->numBlocksX*runAct->blockX;
  double maxY = runAct->numBlocksY*runAct->blockY;
  double maxZ = runAct->blockZ;
  
  
  if (tX > maxX || tY > maxY || tZ > maxZ)
  {
    int lineNum = vecIndex+1;
    if (tX > maxX)
    {
      G4cout << G4endl << G4endl;
      G4cout << "############################################" << G4endl;
      G4cout << "| ERROR!!!" << G4endl;
      G4cout << "| "<< G4endl;
      G4cout << "| X location of " << tX <<" on line " << lineNum << " is outside the scintillator!" << G4endl;
      G4cout << "| Interaction must be within 0 and " << maxX << G4endl;
      G4cout << "| Exiting... " << G4endl;
      G4cout << "############################################" << G4endl;
      G4cout << G4endl << G4endl;
      
      G4bool abort = false;
      G4RunManager::GetRunManager()->AbortRun(abort);
      
    }
    else if (tY > maxY)
    {
      G4cout << G4endl << G4endl;
      G4cout << "############################################" << G4endl;
      G4cout << "| ERROR!!!" << G4endl;
      G4cout << "|       "<< G4endl;
      G4cout << "| Y location of " << tY <<" on line " << lineNum << " is outside the scintillator!" << G4endl;
      G4cout << "| Interaction must be within 0 and " << maxY << G4endl;
      G4cout << "| Exiting... " << G4endl;
      G4cout << "############################################" << G4endl;
      G4cout << G4endl << G4endl;
      
      G4bool abort = false;
      G4RunManager::GetRunManager()->AbortRun(abort);
    }
    else if (tZ > maxZ)
    {
      G4cout << G4endl << G4endl;
      G4cout << "############################################" << G4endl;
      G4cout << "| ERROR!!!" << G4endl;
      G4cout << "| "<< G4endl;
      G4cout << "| Z location of " << tZ <<" on line " << lineNum << " is outside the scintillator!" << G4endl;
      G4cout << "| Interaction must be within 0 and " << maxZ << G4endl;
      G4cout << "| Exiting... " << G4endl;
      G4cout << "############################################" << G4endl;
      G4cout << G4endl << G4endl;
      
      G4bool abort = false;
      G4RunManager::GetRunManager()->AbortRun(abort);
    }
  }
 



}

