#include "MRPCDetectorConstruction.hh"
#include "MRPCDetectorMessenger.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVDivision.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4UserLimits.hh"
#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "MRPCTrackerSD.hh"
#include "G4UserEventAction.hh"
#include "MRPCEventAction.hh"
#include "MRPCDigitizer.hh"
#include "G4DigiManager.hh"
MRPCDetectorConstruction::MRPCDetectorConstruction()
{
  fStepLimit = new G4UserLimits();
 
  m_detectorMessenger = new MRPCDetectorMessenger(this);

  HitCollectionName="TrackerHitsCollection";

  Gapsize=G4ThreeVector(30.*mm,30.*mm,0.25*mm);
  Glasssize=G4ThreeVector(30.*mm,30.*mm,0.7*mm);
  Honeysize=G4ThreeVector(30.*mm,30.*mm,7.5*mm);
  PCBsize=G4ThreeVector(30.*mm,30.*mm,1.2*mm);
  Mylarsize=G4ThreeVector(30.*mm,30.*mm,0.25*mm);
  Nbofgap=5;//This is Nbof
  Nbofgapinchamber=5;
  Nbofchamber=1;
  pitchwidth=20.*mm;
  // maxStep = 0.06*mm;
}

MRPCDetectorConstruction::~MRPCDetectorConstruction()
{
  delete m_detectorMessenger;
}

G4VPhysicalVolume* MRPCDetectorConstruction::Construct()
{
  G4cout<<"Nbofchamber "<<Nbofchamber<<" Nbofgap "<<Nbofgapinchamber<<G4endl;
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
 


//------------------------------------------------------ elements
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4String name,symbol;

  G4Element* H=new G4Element(name="Hydrogen",symbol="H",z=1.,a=1.01*g/mole);
  G4Element* C=new G4Element(name="Carbon",symbol="C",z=6.,a=12.01*g/mole);
  G4Element* S= new G4Element(name="Sulful" ,symbol="S" , z=16., a=32.06*g/mole);
  G4Element* F= new G4Element(name="fluorine" ,symbol="F" , z=9., a=18.01*g/mole);
  G4Element* O = new G4Element(name="Oxygen",symbol="O",z=8., a=16.00*g/mole);
  G4Element* Si = new G4Element(name="Silicon",symbol="Si",z=14.,a=28.09*g/mole);
  G4Element* N = new G4Element(name="Nitrogen",symbol="N",z=7., a=14.01*g/mole); 
//------------------------------------------------------ materials
  G4double density;
  G4int ncomponents,natoms;
  G4double fractionmass;

  G4Material* sf6 = new G4Material("SF",density= 6.14*mg/cm3, ncomponents=2 );
  sf6->AddElement(S, natoms=1);
  sf6->AddElement(F, natoms=6);
 
  G4Material* freon = new G4Material("C2H2F4",density= 4.241*mg/cm3, ncomponents=3,kStateGas,273.15*kelvin, 1*atmosphere);
  freon->AddElement(C, natoms=2);
  freon->AddElement(H, natoms=2);
  freon->AddElement(F, natoms=4);

  G4Material* butane = new G4Material("Isobutane",density= 2.487*mg/cm3, ncomponents=2);
  butane->AddElement(C, natoms=4);
  butane->AddElement(H, natoms=10);

  //gas for chamber
  G4Material* gas=new G4Material(name="MRPCgas",density=4.17*mg/cm3,ncomponents=3);
  gas->AddMaterial(freon, fractionmass=90*perCent);
  gas->AddMaterial(butane ,  fractionmass=5*perCent);
  gas->AddMaterial(sf6 ,  fractionmass=5*perCent);
  // G4NistManager * nistman = G4NistManager::Instance();
  // G4Material * gas = nistman->FindOrBuildMaterial("G4_Si");
  //MRPC material
  //The stop block in the gas container
  G4Material* Nylon = new G4Material("Nylon", density=1.15*g/cm3, ncomponents=4);
  Nylon->AddElement(C, natoms=2);
  Nylon->AddElement(H, natoms=3);
  Nylon->AddElement(O, natoms=1);
  Nylon->AddElement(N, natoms=1);
    
  G4Material* honeycombCore = new G4Material("honeycombCore", density=0.024*g/cm3, ncomponents=3);
  honeycombCore->AddElement(C,natoms=6);
  honeycombCore->AddElement(H,natoms=10);
  honeycombCore->AddElement(O,natoms=5);

  //The honeyComb surface and the PCB board are thought to be composed of SiO2(60%) and epoxy resins(40%)
  G4Material* epoxy = new G4Material("epoxy", density=1.2*g/cm3, ncomponents=3);
  epoxy->AddElement(C,natoms=11);
  epoxy->AddElement(H,natoms=12);
  epoxy->AddElement(O,natoms=3);
  
  //glass
  G4Material* SiO2 = new G4Material(name="SiO2", density=2.200*g/cm3, ncomponents=2);
  SiO2->AddElement(Si, natoms=1);
  SiO2->AddElement(O, natoms=2);

  //PCB
  G4Material* insulationBoard = new G4Material("insulationBoard", density=1.85*g/cm3, 2);
  insulationBoard->AddMaterial(G4Material::GetMaterial("SiO2"), 0.6);
  insulationBoard->AddMaterial(epoxy, 0.4);

  //Mylar
  G4Material* PET = new G4Material("PET", density= 1.39*g/cm3, ncomponents=3);
  PET->AddElement(C,natoms=10);
  PET->AddElement(H,natoms=8);
  PET->AddElement(O,natoms=4);
  //Vaccum for world
  G4Material* vacuum=new G4Material("Galactic",z=1.,a=1.01*g/mole,density=universe_mean_density,kStateGas,2.73*kelvin,3.e-18*pascal);




//------------------------------------------------------ volumes
//experimental hall (world volume)


  chamberThickness=PCBsize.z()+2*Mylarsize.z()+Nbofgapinchamber*Gapsize.z()+(Nbofgapinchamber+1.)*Glasssize.z();
  G4double DetectorThickness=Nbofchamber*chamberThickness+2*Honeysize.z()+PCBsize.z();
  G4Box* world=new G4Box("world",Glasssize.x()/2.*1.5,Glasssize.y()/2.*1.5,DetectorThickness*1.5/2.*mm);
  G4LogicalVolume* world_log=new G4LogicalVolume(world,vacuum,"world_log");
  G4VPhysicalVolume* world_phys=new G4PVPlacement(0,G4ThreeVector(),world_log,"world_phys",0,false,0);
  world_log->SetVisAttributes(G4VisAttributes::Invisible);

  //honeycomb
  G4Box* honeycomb=new G4Box("honeycomb",Honeysize.x()/2.,Honeysize.y()/2.,Honeysize.z()/2.);
  G4LogicalVolume* honeycomb_log=new G4LogicalVolume(honeycomb,honeycombCore,"honeycomb");
  G4double zposi=DetectorThickness/2.-Honeysize.z()/2.;
  new G4PVPlacement(0,G4ThreeVector(0,0,zposi*mm),honeycomb_log,"honeycomb",world_log,false,0,true);
  new G4PVPlacement(0,G4ThreeVector(0,0,-zposi*mm),honeycomb_log,"honeycomb",world_log,false,1,true);
  G4cout<<"Honeycomb1 "<<zposi+Honeysize.z()/2<<" "<<zposi-Honeysize.z()/2<<endl;
  G4cout<<"Honeycomb2 "<<-zposi+Honeysize.z()/2<<" "<<-zposi-Honeysize.z()/2<<endl;
  // G4cout<<"honeycomb1 "<<zposi-Honeysize.z()/2.<<" "<<zposi+Honeysize.z()/2.<<" honeycomb2 "<<-zposi-Honeysize.z()/2.<<" "<<-zposi+Honeysize.z()/2.<<G4endl;
  G4VisAttributes* honeycomb_visAttr=new G4VisAttributes(G4Color(1.0,0.9,0.0));//yellow
  honeycomb_visAttr->SetForceSolid(true);
  honeycomb_log->SetVisAttributes(honeycomb_visAttr);

   //PCB
    G4Box* pcb =new G4Box("pcb",PCBsize.x()/2.0,PCBsize.y()/2.0,PCBsize.z()/2.0);
    G4LogicalVolume* pcb_log=new G4LogicalVolume(pcb,insulationBoard,"pcb");
    G4VisAttributes* pcb_visAttr=new G4VisAttributes(G4Color(0.0,1.0,0.5));//green
    pcb_visAttr->SetForceSolid(true);
    pcb_log->SetVisAttributes(pcb_visAttr);
//Mylar
  G4Box* mylar =new G4Box("mylar",Mylarsize.x()/2.0,Mylarsize.y()/2.0,Mylarsize.z()/2.0);
  G4LogicalVolume* mylar_log=new G4LogicalVolume(mylar,PET,"mylar");
  G4VisAttributes* mylar_visAttr=new G4VisAttributes(G4Color(1.0,1.0,1.0));//white
  mylar_visAttr->SetForceSolid(true);
  mylar_log->SetVisAttributes(mylar_visAttr);
  //glass
  G4Box* glass=new G4Box("glass",Glasssize.x()/2.0,Glasssize.y()/2.0,Glasssize.z()/2.0);
  G4LogicalVolume* glass_log=new G4LogicalVolume(glass,SiO2,"glass");
 G4VisAttributes* glass_visAttr=new G4VisAttributes(G4Color(0.4,0.4,0.6,1));
  glass_visAttr->SetForceSolid(true);
  glass_log->SetVisAttributes(glass_visAttr);
  //gas
  G4Box* gasgap=new G4Box("gasgap",Gapsize.x()/2.0,Gapsize.y()/2.0,Gapsize.z()/2.0);
  G4LogicalVolume* gap_log=new G4LogicalVolume(gasgap,gas,"gasgap");
 G4VisAttributes* gasgap_visAttr=new G4VisAttributes(G4Color(0.0,1.0,1.0));//cyan
  gasgap_visAttr->SetForceSolid(true);
  gap_log->SetVisAttributes(gasgap_visAttr);
  //strip
  G4Box* strip_box=new G4Box("strip_box",Gapsize.x()/2.0,pitchwidth/2.,Gapsize.z()/2.);
  G4LogicalVolume* strip_log=new G4LogicalVolume(strip_box,gas,"strip_log");

  G4int Nbofstrips=floor(Gapsize.y()/pitchwidth);

  new G4PVDivision(//divide Box into slices
		   "strip_log",
		   strip_log,
		   gap_log,
		   kYAxis,
		   Nbofstrips,
		   0); // offset

  if(maxStep<900) strip_log->SetUserLimits(fStepLimit);

  topPCBpos=DetectorThickness/2.-Honeysize.z();
  for(G4int chamberi=0;chamberi<Nbofchamber;chamberi++){
    zposi =topPCBpos-chamberi*chamberThickness-PCBsize.z()/2.;
    new G4PVPlacement(0,G4ThreeVector(0,0,zposi*mm),pcb_log,"pcb",world_log,false,chamberi,false);
    // G4cout<<"PCB1 "<<zposi-PCBsize.z()/2.<<" "<<zposi+PCBsize.z()/2.<<" PCB2 "<<-zposi-PCBsize.z()/2.<<" "<<-zposi+PCBsize.z()/2.<<G4endl;
    G4cout<<"chamber "<<chamberi<<" PCB"<<chamberi<<" "<<zposi+PCBsize.z()/2<<" "<<zposi-PCBsize.z()/2<<endl;
    zposi=zposi-PCBsize.z()/2.-Mylarsize.z()/2.;
    new G4PVPlacement(0,G4ThreeVector(0,0,zposi*mm),mylar_log,"mylar",world_log,false,chamberi*2,false);
    G4cout<<"mylar"<<chamberi*2<<" "<<zposi+Mylarsize.z()/2.<<" "<<zposi-Mylarsize.z()/2.<<endl;
    zposi=zposi-Mylarsize.z()/2.-Glasssize.z()/2.;
    for(G4int i=0;i<Nbofgapinchamber;i++){
      new G4PVPlacement(0,G4ThreeVector(0,0,zposi*mm),glass_log,"glass",world_log,false,chamberi*(Nbofgapinchamber+1)+i,false);
      G4cout<<"glass"<<chamberi*(Nbofgapinchamber+1)+i<<" "<<zposi+Glasssize.z()/2.<<" "<<zposi-Glasssize.z()/2.<<endl;
      zposi=zposi-Glasssize.z()/2.-Gapsize.z()/2;
      new G4PVPlacement(0,G4ThreeVector(0,0,zposi*mm),gap_log,"gasgap",world_log,false,chamberi*Nbofgapinchamber+i,false);
      G4cout<<"gas"<<chamberi*(Nbofgapinchamber+0)+i<<" "<<zposi+Gapsize.z()/2<<" "<<zposi-Gapsize.z()/2.<<endl;
      zposi=zposi-Glasssize.z()/2.-Gapsize.z()/2;
    }
    new G4PVPlacement(0,G4ThreeVector(0,0,zposi*mm),glass_log,"glass",world_log,false,chamberi*(Nbofgapinchamber+1)+Nbofgapinchamber,false);
    G4cout<<"glass"<<chamberi*(Nbofgapinchamber+1)+Nbofgapinchamber<<" "<<zposi+Glasssize.z()/2.<<" "<<zposi-Glasssize.z()/2<<endl;
    zposi=zposi-Glasssize.z()/2.-Mylarsize.z()/2.;
    new G4PVPlacement(0,G4ThreeVector(0,0,-zposi*mm),mylar_log,"mylar",world_log,false,chamberi*2+1,false);
    G4cout<<"mylar"<<chamberi*2+1<<" "<<zposi+Mylarsize.z()/2.<<" "<<zposi-Mylarsize.z()/2.<<endl;
  }
  zposi =-(topPCBpos-PCBsize.z()/2.);
  new G4PVPlacement(0,G4ThreeVector(0,0,zposi*mm),pcb_log,"pcb",world_log,false,Nbofchamber,false);
  G4cout<<"PCB"<<Nbofchamber<<" "<<zposi+PCBsize.z()/2<<" "<<zposi-PCBsize.z()/2<<endl;
  //Sensitive Detector
  MRPCTrackerSD* aTrackerSD = new MRPCTrackerSD("TrackerChamberSD",HitCollectionName,
                                            G4ThreeVector(0,0,0),this);
  SetSensitiveDetector(strip_log ,  aTrackerSD );
  return world_phys;
}

void MRPCDetectorConstruction::SetMaxStep(G4double value)
{
  maxStep=value;
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
  G4cout<<"MaxStep="<<maxStep<<G4endl;
}

void MRPCDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}
