#ifndef MRPCPrimaryGeneratorAction_h
#define MRPCPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "MRPCRecorder.hh"
#include "MRPCPrimaryGeneratorActionMessenger.hh"
class G4ParticleGun;
class G4Event;

class MRPCPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    MRPCPrimaryGeneratorAction();
    ~MRPCPrimaryGeneratorAction();
    void SetGunPos(G4ThreeVector value) {GunPos=value;};
    void SetGunDir(G4ThreeVector value) {GunDir=value;};
    void SetParticleType(G4String value) {ParticleType=value;};
    void SetParticleEnergy(G4double value) {Energy=value;};
    void SetParticleMomentum(G4double value) {Momentum=value;};
  
    void GeneratePrimaries(G4Event* anEvent);

  private:
    G4ParticleGun* particleGun;
    G4ThreeVector GunPos,GunDir;
    G4String ParticleType;
    G4double Energy,Momentum;
    MRPCPrimaryGeneratorActionMessenger* m_primarygenMessenger;
};

#endif


