
#include "MRPCRecorder.hh"
#include "MRPCDigi.hh"

//--------------------------------------------
MRPCRecorder::MRPCRecorder()
{
  recordflag=2;
  m_recorderMessenger=new MRPCRecorderMessenger(this);
 
}

MRPCRecorder::~MRPCRecorder() 
{
  delete m_recorderMessenger;
}

void MRPCRecorder::SetupWriteFile(){
  if(recordflag==4) G4cout<<"No Output"<<G4endl;//doNothing
  else{
    outfile = new TFile(OutFilename,"RECREATE");
    if(recordflag==1){
      G4cout<<"Will record DigitsCollection"<<G4endl;
      SetupTreeForDigit();
    }
    else if(recordflag==2){
      G4cout<<"Will record HitsCollection"<<G4endl;
      SetupTreeForHit();
    }
    else{
      G4cout<<"Will record HitsCollection & DigitsCollection"<<G4endl;
      SetupTreeForDigit();
      SetupTreeForHit();
    } 
  }
}

void MRPCRecorder::RecordHitinROOT(MRPCTrackerHitsCollection* hitsCollection){
  if(recordflag==2||recordflag==3){
    G4int nHits = hitsCollection->entries();
    for (G4int itr  = 0 ; itr < nHits ; itr++){
      track.push_back((*hitsCollection)[itr]->GetTrackID());
      strip.push_back((*hitsCollection)[itr]->GetPreStripID());
      px.push_back((*hitsCollection)[itr]->GetPrePosWithRespectToStrip().x());
      py.push_back((*hitsCollection)[itr]->GetPrePosWithRespectToStrip().y());
      pz.push_back((*hitsCollection)[itr]->GetPrePosWithRespectToStrip().z());
      dep.push_back((*hitsCollection)[itr]->GetEdep());
      pdg.push_back((*hitsCollection)[itr]->GetTrackPdgId());
    }
    outfile->cd();
    treeh->Fill();

    track.clear();
    strip.clear();
    px.clear();
    py.clear();
    pz.clear();
    dep.clear();
    pdg.clear();
  }
}

//------------Invoke for every Event-----------------------
void MRPCRecorder::RecordDigitinROOT(MRPCDigitsCollection* digitsCollection){
  if(recordflag==1||recordflag==3){
    G4int nDigits = digitsCollection->entries();
    for (G4int itr  = 0 ; itr < nDigits ; itr++){ 
      truthtime=(*digitsCollection)[itr]->GetTruthTime();
      arrivedtime=(*digitsCollection)[itr]->GetArrivedTime();
      leadtime=(*digitsCollection)[itr]->GetLeadTime();
      peaktime=(*digitsCollection)[itr]->GetPeakTime();
      tot=(*digitsCollection)[itr]->GetToT();
      totalindq=(*digitsCollection)[itr]->GetTotalInducedQ();
      indq=(*digitsCollection)[itr]->GetInducedQAroundThre();
      wavepeak=(*digitsCollection)[itr]->GetWavePeak();
      overthre=(*digitsCollection)[itr]->GetIfOverThreshold();
      nbofpoint=(*digitsCollection)[itr]->GetNbofpointAboveThreshold();
      // G4cout<<"recorder "<<pointtimestep<<" "<<(*digitsCollection)[itr]->GetPointTimeStep()<<G4endl;
      pointtimestep=(*digitsCollection)[itr]->GetPointTimeStep();
		if((*digitsCollection)[itr]->GetParticleName()=="proton")
		  particlename=1;//(*digitsCollection)[itr]->GetParticleName();
		else if((*digitsCollection)[itr]->GetParticleName()=="pi+")
		  particlename=2;
		else
		  particlename=0;
		if((*digitsCollection)[itr]->GetWriteOriginalFlag()==1){
		  originalpointtimestep=(*digitsCollection)[itr]->GetOriginalPointTimeStep();
		  originalpeaktime=(*digitsCollection)[itr]->GetOriginalPeakTime();
		  originalindi=(*digitsCollection)[itr]->GetInducedI();
		}
		
      Walk->Fill(totalindq,leadtime);
    }
    // G4cout<<"nDigits "<<nDigits<<" leadtime "<<leadtime<<" ToT "<<tot<<G4endl;
    // if(nDigits>0){
    outfile->cd();
    treed->Fill();
    // }
    totalindq=999;
    tot=totalindq;
    leadtime=totalindq;
    truthtime=totalindq;
    arrivedtime=totalindq;
    pointtimestep=totalindq;
    nbofpoint=999;
	 particlename=999;
    indq.clear();
  }
}

// void MRPCRecorder::RecordHitinROOT(MyTrackerHitsCollection* hitsCollection,std::vector<BichselHits> BichselhitsCollection){
//   G4int nHits = hitsCollection->entries();
//   for (G4int itr  = 0 ; itr < nHits ; itr++){
//     track.push_back((*hitsCollection)[itr]->GetTrackID());
//     px.push_back((*hitsCollection)[itr]->GetPosWithRespectToPixel().x());
//     py.push_back((*hitsCollection)[itr]->GetPosWithRespectToPixel().y());
//     pz.push_back((*hitsCollection)[itr]->GetPosWithRespectToPixel().z());
//     dep.push_back((*hitsCollection)[itr]->GetEdep());
//     Nbx.push_back((*hitsCollection)[itr]->GetPixelNbX());
//     Nby.push_back((*hitsCollection)[itr]->GetPixelNbY());
//     particle.push_back((*hitsCollection)[itr]->GetTrackPdgId());
//   }
//   nHits = BichselhitsCollection.size();
//   // std::cout<<nHits<<std::endl;
//   for (G4int itr  = 0 ; itr < nHits ; itr++){
//     Bichseltrack.push_back(BichselhitsCollection[itr].trackid);
//     Bichselpx.push_back(BichselhitsCollection[itr].pos.x());
//     Bichselpy.push_back(BichselhitsCollection[itr].pos.y());
//     Bichselpz.push_back(BichselhitsCollection[itr].pos.z());
//     // std::cout<<BichselhitsCollection[itr].pos.z()<<std::endl;
//     Bichseldep.push_back(BichselhitsCollection[itr].energy);
//     BichselNbx.push_back(BichselhitsCollection[itr].PixelNb.first);
//     BichselNby.push_back(BichselhitsCollection[itr].PixelNb.second);
//   }

//   // std::cout<<"Bichsel "<<BichselNby.size()<<std::endl;
//   outfile->cd();
//   treeh->Fill();

//   track.clear();
//   px.clear();
//   py.clear();
//   pz.clear();
//   dep.clear();
//   Nbx.clear();
//   Nby.clear();
//   particle.clear();
//   Bichseltrack.clear();
//   Bichselpx.clear();
//   Bichselpy.clear();
//   Bichselpz.clear();
//   Bichseldep.clear();
//   BichselNbx.clear();
//   BichselNby.clear();
// }




void MRPCRecorder::SetupTreeForDigit(){
  Walk=new TH2D("Walk","Time VS Induced Charge",100,0,2,100,0,1.2);

  treed=new TTree("DigitTree","Digit tree");
  treed->Branch("truthtime",&truthtime);//ns
//  treed->Branch("arrivedtime",&arrivedtime);//ns
  treed->Branch("peaktime",&peaktime);//ns
  treed->Branch("leadtime",&leadtime);
  treed->Branch("tot",&tot);
  treed->Branch("totalindq",&totalindq);
  treed->Branch("indq",&indq);
  treed->Branch("pointtimestep",&pointtimestep);
//  treed->Branch("wavepeak",&wavepeak);
  treed->Branch("overthre",&overthre);
  treed->Branch("particlename",&particlename); 
//  treed->Branch("nbofpoint",&nbofpoint);
  treed->Branch("originalpeaktime",&originalpeaktime);//ns
  treed->Branch("originalindi",&originalindi);
  treed->Branch("originalpointtimestep",&originalpointtimestep);

}

void MRPCRecorder::SetupTreeForHit(){
  treeh=new TTree("HitTree","Hit tree");
  treeh->Branch("track",&track);
  treeh->Branch("strip",&strip);
  treeh->Branch("px",&px);
  treeh->Branch("py",&py);
  treeh->Branch("pz",&pz);
  treeh->Branch("dep",&dep); 
  treeh->Branch("pdg",&pdg);
}

void MRPCRecorder::WriteCloseROOTFile()
{
  G4cout << "CloseROOTFile:----------------------RUN ENDS----------------------- " << G4endl;
  outfile->cd();
  if(recordflag==1){
    treed->Write();
    // Walk->Write();
  }
  else if(recordflag==2)
    treeh->Write();
  else{
    treed->Write(); 
    Walk->Write();
    treeh->Write();
  } 
  outfile->Close();
}
