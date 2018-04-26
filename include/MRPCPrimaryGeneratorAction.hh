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
  
    void GeneratePrimaries(G4Event* anEvent);

  private:
    G4ParticleGun* particleGun;
    G4ThreeVector GunPos,GunDir;
    MRPCPrimaryGeneratorActionMessenger* m_primarygenMessenger;
};

#endif


