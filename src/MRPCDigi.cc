#include "MRPCDigi.hh"


G4ThreadLocal G4Allocator<MRPCDigi> *MRPCDigiAllocator = 0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MRPCDigi::MRPCDigi()
{
	StripID = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MRPCDigi::~MRPCDigi()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MRPCDigi::MRPCDigi(const MRPCDigi& right)
{
	StripID = right.StripID;
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const MRPCDigi& MRPCDigi::operator=(const MRPCDigi& right)
{

	StripID = right.StripID;
	return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4int MRPCDigi::operator==(const MRPCDigi& right) const
{
	return (StripID==right.StripID);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MRPCDigi::Draw()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MRPCDigi::Print()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
