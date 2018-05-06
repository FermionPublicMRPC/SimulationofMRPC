#include "MRPCPrimaryGeneratorAction.hh"
#include "MRPCPrimaryGeneratorActionMessenger.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "TF2.h"
#include "TRandom.h"
#include "Randomize.hh"

MRPCPrimaryGeneratorAction::MRPCPrimaryGeneratorAction()
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  ParticleType="mu-Cosmic";
  Energy=-10;Momentum=-10;
  m_primarygenMessenger= new MRPCPrimaryGeneratorActionMessenger(this); 
 
}

MRPCPrimaryGeneratorAction::~MRPCPrimaryGeneratorAction()
{
  delete particleGun;
}

void MRPCPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  G4String particleName;
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  if(ParticleType!="mu-Cosmic"){//Not Cosmic ray
	 particleGun->SetParticleDefinition(particleTable->FindParticle(particleName=ParticleType));
	 if(Energy>0)
		particleGun->SetParticleEnergy(Energy*GeV);//kineticEnergy
	 else if(Momentum>0){
		G4double mass=particleGun->GetParticleDefinition()->GetPDGMass();
		particleGun->SetParticleEnergy(sqrt(pow(mass,2)+pow(Momentum,2))-mass);//Momentum*MeV);
	 }
	 else G4cout<<"Please assign particle energy!"<<G4endl;
	 particleGun->SetParticlePosition(GunPos);
	 particleGun->SetParticleMomentumDirection(GunDir);
  }
  else{//Cosmic ray
	 G4double sourceposx,sourceposy,sourceposz,endposx,endposy,endposz,muEnergy,cosTheta;
	 sourceposy=0;
	 sourceposz=30;
	 sourceposx=gRandom->Uniform(-20,20);
	 endposy=0;
	 endposz=-30;
	 endposx=gRandom->Uniform(-20,20);
	 muEnergy=gRandom->Gaus(4,0.5);
	 particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="mu-"));
	 particleGun->SetParticlePosition(G4ThreeVector(sourceposx *mm,sourceposy *mm,sourceposz *mm));
	 particleGun->SetParticleEnergy(muEnergy *GeV);
	 particleGun->SetParticleMomentumDirection(G4ThreeVector(endposx-sourceposx,endposy-sourceposy,endposz-sourceposz));
  }
  particleGun->GeneratePrimaryVertex(anEvent);
}
  // gRandom->SetSeed();
  // G4duoble sourceposx,sourceposy,sourceposz,endposx,endposy,endposz,muEnergy,cosTheta;
  // sourceposy=0;
  // sourceposz=35;
  // sourceposx=gRandom->Uniform(-10,10);
  // endposy=0;
  // endposz=-35;
  // G4double minCosTheta = (sourceposz-endposz)/sqrt(pow(sourceposz-endposz,2)+10*10);

  // TF2 *fMu = new TF2("fMu","0.14*TMath::Power(x,-2.7)*(1/(1+1.1*x*y/115)+0.054/(1+1.1*x*y/850))",0.,20,minCosTheta,1);
  // fMu->GetRandom2(muEnergy,cosTheta);
  // if(gRandom->Uniform(-1,1)>0)
  // 	 endposx=tan(acos(cosTheta))*70+sourceposx;
  // else
  // 	 endposx=-tan(acos(cosTheta))*70+sourceposx;

  // fParticleGun->SetParticlePosition(G4ThreeVector(sourceposx *mm,sourceposy *mm,sourceposz *mm));
  // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(endposx-sourceposx,endposy-sourceposy,endposz-sourceposz));
  // fParticleGun->SetParticleEnergy(muEnergy *GeV);
  // fParticleGun->GeneratePrimaryVertex(anEvent);


