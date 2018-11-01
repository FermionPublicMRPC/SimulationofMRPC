#include "MRPCSignal.hh"
#include "MRPCSignalMessenger.hh"
// #include "MRPCSigi.hh"
#include "G4ThreeVector.hh"
#include "MRPCTrackerHit.hh"
#include "TMath.h"
#include "TF1.h"
#include "TLine.h"
#include "TLatex.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include <TFile.h>
#include <iostream>
#include <fstream>

//order of some parameters 
//alpha 117.6  (5 gas gaps)
//eta 4.836 (5 gas gaps)
 
//alpha 161.2  (15 gas gaps)
//eta 3.667 (15 gas gaps)

//electric field 11 (5 gas gaps)
//electric field 13.1 (32 gas gaps)
// WeightE= 0.71;// /mm 
//velocity 204  (5 gas gaps)
MRPCSignal::MRPCSignal()
{
  //constant
  AverageIonEnergy=23.e-6;//Mev
  pi=3.1415926;
  elec=1.6e-19;
  LightSpeed=3e8;
  //Gas constant
  Alpha=161.2;//144800./1000;// mm^-1
  Eta=3.667;//5013./1000;// mm^-

 //Particle and field constant
  E=13.1;//kV/mm
  RelativePermittivity=8.5;
  Velocity=238./1000.;//mm/ns 204.5
  SpaceChargeThre=1.e6;
  DiffLengthZ=0.005;//mm for total width 0.25mm,diff_z=5um
  DiffSigMaxScale=4.5;//unit: sigma
  
  //electronics
  ThresholdPer=0.1;//fC 
  WaveMaxValue=100;
  NoisePer=0.01;
  CountNbofPointsOverThre=15;
  //Iteration
  Nbofsteps=500;//for 1 gap thickness
  NbofPointAroundThre=33;
  TimeInterval=7.;//ns

  //Bool
  doDiff=false;
  EnergyFlag=true;
  LongPosFlag=true;
  AvalancheFlag=true;
  RandomPeakFlag=true;
  RecordOriginal=0;
  
  m_signalMessenger=new MRPCSignalMessenger(this);

  Tau1=0.5;Tau2=1.5;Amplitude=45;
  DrawPlot=false;
  NbofpointAboveThreshold=0;
  GasMix="Standard";

}

MRPCSignal::~MRPCSignal()
{

  delete m_signalMessenger;
}

void MRPCSignal::Initialize(G4ThreeVector gapgeo,G4int eventID){
  //This function initialize:
  //1. histograms and iterations; 
  //2. gas and field parameters
  //gapgeo.x():Nbofgap, gapgeo.y():Platethickness, gapgeo.z():gapthickness
  Double_t startsig=0.0;//ns 
  gRandom->SetSeed();
  if(eventID==0){//for the first event in this run
    E=RealVoltage*2/gapgeo.x()/gapgeo.z();//kV/mm
    if(ResetGasParameter(E)){
      TownsendK=Eta/Alpha;
      EffAlpha=Alpha-Eta;
    }
    else G4cout<<"Error occures when getting correct gas parameters."<<G4endl;
    
    Gapsize=gapgeo;
    TimeCrossGap=gapgeo.z()/Velocity;
    ZStep=gapgeo.z()/Nbofsteps; 
    TimeStep=TimeCrossGap/Nbofsteps;
    PointStep=TimeInterval/TimeStep;
    if(doDiff) MaxScale=1+DiffSigMaxScale*DiffLengthZ/Velocity/TimeCrossGap;
    else MaxScale=1;
    StartTimeOffset=0;//gRandom->Gaus(4,startsig);//Uniform(0,1);
    NbWithTime=new TH1D("Nbofelectrons_time","Nbof electrons with time",Nbofsteps,0,1);
    IWithTime=new TH1D("Current_time","Current with time",Nbofsteps,0,1);
    ResponseIWithTime=new TH1D("ReponseCurrent_time","Response Current with time",Nbofsteps,0,1);
    WeightE=RelativePermittivity/(Gapsize.x()*Gapsize.z()*RelativePermittivity+(Gapsize.x()+1)*Gapsize.y());//0.17
    
    Nbofpointshist=new TH1D("Nbofpoint","Nbofpoint",10,0,10);
    peakchargehist=new TH1D("peakcharge","peakcharge",100,0,150);
    G4cout<<"Electric Field "<<E<<" kV/mm Alpha "<<Alpha<<" /mm Eta "<<Eta<<" /mm velocity "<<Velocity<<" mm/ns WeightingE "<<WeightE<<" /mm"<<G4endl;
	 G4cout<<"SpaceChargeThre "<<SpaceChargeThre<<" tau1 "<<Tau1<<" tau2 "<<Tau2<<" A "<<Amplitude<<G4endl;
  }
  StartTimeOffset=0;//gRandom->Gaus(4,startsig);//gRandom->Uniform(0,8);
  G4double histrangescale=1.3;
  // if(gapgeo.z()<0.15){
  // 	  histrangescale=1.3;
  // 	  // SpaceChargeThre=1.e6;
  // }
  NbWithTime->SetBins(Nbofsteps*histrangescale,StartTimeOffset,StartTimeOffset+histrangescale*TimeCrossGap);
  IWithTime->SetBins(Nbofsteps*histrangescale,StartTimeOffset,StartTimeOffset+histrangescale*TimeCrossGap);
  ResponseIWithTime->SetBins(Nbofsteps*histrangescale,StartTimeOffset,StartTimeOffset+histrangescale*TimeCrossGap*4);
  ResetSignal();
 
  //plot the length of the leading edge and the peak charge
  if(eventID%1000==0){
	 G4cout<<"eventID="<<eventID<<G4endl;
    // TCanvas *c3=new TCanvas("c3","c3",0,0,1400,700);
    // c3->Divide(2);
    // c3->cd(1);
    // Nbofpointshist->GetXaxis()->SetTitle("Nbofpoints");
    // Nbofpointshist->Draw();
    // c3->cd(2);
    // peakchargehist->GetXaxis()->SetTitle("chargepeak");
    // peakchargehist->Draw();
    // c3->SaveAs("plot.png");
    // Nbofpointshist->SaveAs("Nbofpoint"+TString::Format("%d",eventID)+".root");
    // peakchargehist->SaveAs("peakchargehist"+TString::Format("%d",eventID)+".root");
  }
}

void MRPCSignal::ResetSignal(){
  NbWithTime->Reset();
  IWithTime->Reset();
  ResponseIWithTime->Reset();

  LeadTime=999;
  TotalInducedQ=999;
  InducedQAroundThre.clear();
  OriginalIAroundThre.clear();
  OverThreshold=false;
}


bool MRPCSignal::CalculateSignal(MRPCTrackerHitsCollection* hitsCollection){
  //main function of this class. Run for every event. Get the hits information can create the waveform
  //No uncertainty
  if(!EnergyFlag&&!LongPosFlag&&!AvalancheFlag){
  	 TruthTime=0;
  	 G4double stepenergy=FixedTotalEnergy/(G4double)FixedNbofstep;
  	 G4double steplength=Gapsize.x()*Gapsize.z()/(G4double)FixedNbofstep;
  	 for(G4int itr=0;itr<FixedNbofstep;itr++){
  		G4int GapID=(itr*steplength)/Gapsize.z();
  		G4double poszwithrespect=itr*steplength-(Double_t)GapID*Gapsize.z();
  		G4double thisstarttime=((Double_t)GapID*(Gapsize.y()+Gapsize.z())+poszwithrespect)/LightSpeed*1e6;//ns
  		AvalancheWithoutUncertainty(-poszwithrespect,thisstarttime,round(stepenergy/AverageIonEnergy));
  	 }
  }
  //EnergyUncert Small/Mid/Large
  if(EnergyFlag&&!LongPosFlag&&!AvalancheFlag){
  	 TruthTime=0;
	 G4double tmptotalenergy=0;
	 G4int nentries=hitsCollection->entries();
	 for(G4int itr=0;itr<nentries;itr++)
		tmptotalenergy+=(*hitsCollection)[itr]->GetEdep();
  	 G4double stepenergy=tmptotalenergy/(G4double)FixedNbofstep;
  	 G4double steplength=Gapsize.x()*Gapsize.z()/(G4double)FixedNbofstep;
  	 for(G4int itr=0;itr<FixedNbofstep;itr++){
  		G4int GapID=(itr*steplength)/Gapsize.z();
  		G4double poszwithrespect=itr*steplength-(Double_t)GapID*Gapsize.z();
  		G4double thisstarttime=((Double_t)GapID*(Gapsize.y()+Gapsize.z())+poszwithrespect)/LightSpeed*1e6;//ns
  		AvalancheWithoutUncertainty(-poszwithrespect,thisstarttime,round(stepenergy/AverageIonEnergy));
  	 }
  }
  //EnergyPosUncert
  if(EnergyFlag&&LongPosFlag&&!AvalancheFlag){
	 G4int nentries=hitsCollection->entries();
	 TruthTime=9999;
	 for(G4int itr=0;itr<nentries;itr++){
		G4double primaryelectrons=round((*hitsCollection)[itr]->GetEdep()/AverageIonEnergy);//get int from double, used as round
		if(primaryelectrons==0) continue;
		if(TruthTime>9990) TruthTime=(*hitsCollection)[itr]->GetLocalDepTime();
		AvalancheWithoutUncertainty((*hitsCollection)[itr]->GetPrePosWithRespectToStrip().z(),(*hitsCollection)[itr]->GetLocalDepTime()-TruthTime,primaryelectrons);
	 }
  }
  //AllUncert
  if(EnergyFlag&&LongPosFlag&&AvalancheFlag){  
	 G4int nentries=hitsCollection->entries();
	 TruthTime=9999;
	 for(G4int itr=0;itr<nentries;itr++){
		G4double primaryelectrons=round((*hitsCollection)[itr]->GetEdep()/AverageIonEnergy);//get int from double, used as round
		if(primaryelectrons==0) continue;
		if(TruthTime>9990) TruthTime=(*hitsCollection)[itr]->GetLocalDepTime();
		// G4cout<<"stepenergy="<<primaryelectrons<<G4endl;
		Avalanche((*hitsCollection)[itr]->GetGapID(),(*hitsCollection)[itr]->GetPrePosWithRespectToStrip(),(*hitsCollection)[itr]->GetLocalDepTime()-TruthTime,primaryelectrons);
	 }
  }
  //PositionUncert
  if(!EnergyFlag&&LongPosFlag&&!AvalancheFlag){
	 G4int thisNbofstep=0;
	 TruthTime=9999;
	 for(G4int itr=0;itr<hitsCollection->entries();itr++){
		G4double primaryelectrons=round((*hitsCollection)[itr]->GetEdep()/AverageIonEnergy);//get int from double, used as round
		if(primaryelectrons>0) thisNbofstep++;
	 }
 	 G4double stepenergy=FixedTotalEnergy/(G4double)thisNbofstep;
	 // G4cout<<"stepenergy="<<round(stepenergy/AverageIonEnergy)<<G4endl;
	 for(G4int itr=0;itr<hitsCollection->entries();itr++){
		G4double primaryelectrons=round((*hitsCollection)[itr]->GetEdep()/AverageIonEnergy);//get int from double, used as round
		if(primaryelectrons==0) continue;
		if(TruthTime>9990) TruthTime=(*hitsCollection)[itr]->GetLocalDepTime();
		AvalancheWithoutUncertainty((*hitsCollection)[itr]->GetPrePosWithRespectToStrip().z(),(*hitsCollection)[itr]->GetLocalDepTime()-TruthTime,round(stepenergy/AverageIonEnergy));
	 }
  }


  CalculateCurrent(); //Get the original current waveform(from)

  ElectronicsResponse();//Add electronics response to the original current.
  //ToT,threshold crosing time=ElectronicsResponse(Iwitht)
  CalculateToT();//Calculate the ToT and judge whether the signal is overthreshold 
  CalculateWaveForm(); //calculate the waveform.
  if(RecordOriginal==1)
	 CalculateOriginalWaveFrom();
  return OverThreshold;
}

//Calculate the current with respect to time from the number of electrons
void MRPCSignal::CalculateCurrent(){
  for(Int_t stepi=0;stepi<NbWithTime->GetNbinsX();stepi++)
    IWithTime->SetBinContent(1+stepi,NbWithTime->GetBinContent(1+stepi)*WeightE*Velocity*elec*1e12);//mA
  TotalInducedQ=IWithTime->Integral()*IWithTime->GetBinWidth(1);//pC
  
  if(DrawPlot){   
	 TCanvas *c1=new TCanvas("c1","c1",0,0,1300,600);
	 // TPad *pad1 = new TPad("pad1","",0,0,1,1);
	 // TPad *pad2 = new TPad("pad1","",0,0,1,1);
	 c1->Divide(2);
	 c1->cd(1);
	 NbWithTime->Draw();
	 NbWithTime->GetXaxis()->SetTitle("Time /ns");
	 NbWithTime->GetYaxis()->SetTitle("#electrons");
	 NbWithTime->GetXaxis()->SetTitleSize(0.04);
	 NbWithTime->GetXaxis()->SetLabelSize(0.04);
	 NbWithTime->GetYaxis()->SetTitleSize(0.04);
	 NbWithTime->GetYaxis()->SetLabelSize(0.04);
	 NbWithTime->GetYaxis()->SetTitleOffset(1.2);
	 c1->cd(2);
	 IWithTime->GetXaxis()->SetTitle("Time /ns");
	 IWithTime->GetYaxis()->SetTitle("Current /mA");
	 IWithTime->GetXaxis()->SetTitleSize(0.04);
	 IWithTime->GetXaxis()->SetLabelSize(0.04);
	 IWithTime->GetYaxis()->SetTitleSize(0.04);
	 IWithTime->GetYaxis()->SetLabelSize(0.04);
	 IWithTime->Draw();
	 IWithTime->GetYaxis()->SetTitleOffset(1.2);

	 c1->Update();
	 TString name;
	 TFile *outfile=new TFile("OriginalSignal"+name.Format("%.0f",(TruthTime+gRandom->Uniform()*10)*1000)+".root","recreate");
	 NbWithTime->Write();
	 IWithTime->Write();
	 c1->Write();
	 outfile->Close();
	 c1->SaveAs("OriginalSignal"+name.Format("%.0f",(TruthTime+gRandom->Uniform()*10)*1000)+".png");
  }
}

//Add Electronics Response to the cugrrent signal and get ResponseIWithTime
void MRPCSignal::ElectronicsResponse(){
  G4double range=ElectronicsRange;//ns
  // G4double range=1.4;//ns
  G4int M=IWithTime->GetNbinsX();
  // TH1D *electronichist=new TH1D("electronichist","electronichist",2500,StartTimeOffset,IWithTime->GetXaxis()->GetXmax());
  TH1D *electronichist=new TH1D("electronichist","electronichist",range/IWithTime->GetBinWidth(1),StartTimeOffset,StartTimeOffset+range);
  // electronichist->FillRandom("delta_response",1e8);
  // electronichist->Scale(amplitude/electronichist->Integral());

  G4int N=electronichist->GetNbinsX();

  ResponseIWithTime->SetBins(M+N-1,StartTimeOffset,StartTimeOffset+(M+N-1)*IWithTime->GetBinWidth(1));
  for(G4int k=0;k<M+N-1;k++){
    G4int ilow=TMath::Max(0,k-N+1);
    G4int ihigh=TMath::Min(k,M-1);
    G4double tmp=0;
    for(G4int i=ilow;i<ihigh;i++)
      tmp+=IWithTime->GetBinContent(i)*ElectronicsResponseFunction(electronichist->GetBinCenter(k-i));
    ResponseIWithTime->SetBinContent(k,tmp);
  }
  electronichist->Clear();
  Peak = FindHistPeakValue(ResponseIWithTime);
  // G4cout<<"peak="<<ResponseIWithTime->GetBinCenter(Peak.first)<<" value="<<ResponseIWithTime->GetBinContent(Peak.second)<<G4endl;
}


//Get the over Thre lead time and the total induced charge and the ToT
void MRPCSignal::CalculateToT(){
  //time:ns,q:fC,length:mm
  G4int lead=GetLeadingEdge(ResponseIWithTime,WaveMaxValue*ThresholdPer);//see if the signal is overshold
  //if(Peak.second<0.3*WaveMaxValue) OverThreshold=false;
  
  if(OverThreshold){
    // std::pair<G4int,G4double> trail=GetTrailingEdge(ResponseIWithTime,WaveMaxValue*ThresholdPer);
    G4int trail=GetTrailingEdge(ResponseIWithTime,WaveMaxValue*ThresholdPer);
    LeadTime=ResponseIWithTime->GetBinCenter(lead)+TruthTime;//ns lead.first
    ToT=ResponseIWithTime->GetBinCenter(trail)-ResponseIWithTime->GetBinCenter(lead);//ns .first
//    TotalInducedQ=IWithTime->Integral()*IWithTime->GetBinWidth(1);//pC
    // G4cout<<"CalculateToT Leadtime "<<LeadTime<<" ToTstart "<<ResponseIWithTime->GetBinCenter(lead.first)<<" ToTend "<<ResponseIWithTime->GetBinCenter(trail.first)<<" ToT "<<ToT<<G4endl;
    
    TF1 *drawthre=new TF1("drawthre","[0]",ResponseIWithTime->GetXaxis()->GetXmin(),ResponseIWithTime->GetXaxis()->GetXmax());
    TLine *reach1=new TLine(LeadTime,0,LeadTime,40);
    TLine *reach2=new TLine(ResponseIWithTime->GetBinCenter(trail),0,ResponseIWithTime->GetBinCenter(trail),40);   
    TLine *reach3=new TLine(ResponseIWithTime->GetBinCenter(Peak.first),0,ResponseIWithTime->GetBinCenter(Peak.first),150);
    drawthre->SetParameter(0,WaveMaxValue*ThresholdPer);
    Int_t Nbofpoint=GetLeadingNbofPoints(ResponseIWithTime,0.001,Peak);
    Nbofpointshist->Fill(Nbofpoint);
    peakchargehist->Fill(Peak.second);
    if(DrawPlot){   
      gStyle->SetOptStat("");

      TString name,title;
      TCanvas *c2=new TCanvas("c2","c2",0,0,700,700);
      ResponseIWithTime->Draw();
      ResponseIWithTime->GetXaxis()->SetTitle("Time / ns");
      ResponseIWithTime->GetYaxis()->SetTitle("Arbitrary Unit");
      // ResponseIWithTime->GetYaxis()->SetRangeUser(0,150);
      // ResponseIWithTime->GetXaxis()->CenterTitle();
      ResponseIWithTime->GetYaxis()->SetTitleOffset(1.2);
      ResponseIWithTime->GetXaxis()->SetTitleOffset(1.1);
      drawthre->SetLineColor(2);
      reach1->SetLineColor(3);
      reach2->SetLineColor(4);
      drawthre->Draw("same");
      reach1->Draw("same");
      reach2->Draw("same");
      reach3->SetLineColor(5);
      reach3->Draw("same");
      TLatex *text1=new TLatex();
      text1->SetTextFont(42);
      text1->SetTextSize(0.04);
      text1->SetTextColor(2);
      text1->SetNDC();

      text1->DrawLatex(0.18,0.85,"Threshold = "+name.Format("%.2f",WaveMaxValue*ThresholdPer)+", Nosie = "+name.Format("%.2f",WaveMaxValue*NoisePer));
      TLatex *text2=new TLatex();
      text2->SetTextFont(42);
      text2->SetTextSize(0.04);
      text2->SetTextColor(2);
      text2->SetNDC();
      text2->DrawLatex(0.18,0.8,"Time t1 = "+name.Format("%.2f",LeadTime)+", t2 = "+name.Format("%.2f",ResponseIWithTime->GetBinCenter(trail))+" ns");

      std::vector<std::pair<G4int,G4double> > length=GetLeadingLength(ResponseIWithTime,0.001);
      G4double leadingwavelength=(ResponseIWithTime->GetBinCenter(length[1].first)-ResponseIWithTime->GetBinCenter(length[0].first))*1000;
      TLatex *text3=new TLatex();
      text3->SetTextFont(42);
      text3->SetTextSize(0.04);
      text3->SetTextColor(2);
      text3->SetNDC();
      text3->DrawLatex(0.18,0.75,"leading length = "+name.Format("%.2f",leadingwavelength)+" ps");

      text1->Draw();
      text2->Draw();
      // c2->SaveAs("Resonse"+name.Format("%d",Peak.first)+".png"); 
      TFile *outfile=new TFile("Resonse"+name.Format("%.0f",TruthTime*1000)+".root","recreate");
      ResponseIWithTime->Write();
      reach1->Write();
      reach2->Write();
      reach3->Write();
      text1->Write();
      text2->Write();
      text3->Write();
      c2->Write();
      outfile->Close();
      c2->SaveAs("Resonse"+name.Format("%.0f",(TruthTime+gRandom->Uniform()*10)*1000)+".png"); 
    }
  }
}
void MRPCSignal::CalculateOriginalWaveFrom(){
  // OriginalPeak=FindHistPeakValue(IWithTime);
  // OriginalPeakTime=IWithTime->GetBinCenter(OriginalPeak.first)+TruthTime;
  // for(Int_t bini=0;bini<55;bini++){
  // 	 Double_t tmpy=IWithTime->GetBinContent(OriginalPeak.first-bini);
  // 	 OriginalIAroundThre.push_back(tmpy);
  // }
  // OriginalPeak=FindHistPeakValue(NbWithTime);
  // OriginalPeakTime=NbWithTime->GetBinCenter(OriginalPeak.first)+TruthTime;
  // for(Int_t bini=0;bini<55;bini++){
  // 	 Double_t tmpy=NbWithTime->GetBinContent(OriginalPeak.first-bini);
  // 	 OriginalIAroundThre.push_back(tmpy);
  // }
  // G4int startbin=IWithTime->FindBin(0.25);//ns
  // G4int originalnbofpoint=40;
  // if(IWithTime->GetBinContent(startbin+originalnbofpoint)<IWithTime->GetMaximum()){
  // 	 OriginalPeakTime=IWithTime->GetBinCenter(startbin+originalnbofpoint);
  // 	 for(Int_t bini=0;bini<originalnbofpoint;bini++){
  // 		Double_t tmpy=IWithTime->GetBinContent(startbin+originalnbofpoint-bini);
  // 		OriginalIAroundThre.push_back(tmpy);
  // 	 }
  // }
  // else{
  // 	 G4int maximumbin=IWithTime->GetMaximumBin();
  // 	 OriginalPeakTime=IWithTime->GetBinCenter(maximumbin);
  // 	 for(Int_t bini=0;bini<originalnbofpoint;bini++){
  // 		Double_t tmpy=IWithTime->GetBinContent(maximumbin-bini);
  // 		OriginalIAroundThre.push_back(tmpy);
  // 	 }
  // }
 // G4int startbin=NbWithTime->FindBin(0.25);//ns
 // G4int originalnbofpoint=40;
 //  if(NbWithTime->GetBinContent(startbin+originalnbofpoint)<NbWithTime->GetMaximum()){
 // 	 OriginalPeakTime=NbWithTime->GetBinCenter(startbin+originalnbofpoint);
 // 	 for(Int_t bini=0;bini<originalnbofpoint;bini++){
 // 		Double_t tmpy=NbWithTime->GetBinContent(startbin+originalnbofpoint-bini);
 // 		OriginalIAroundThre.push_back(tmpy);
 // 	 }
 //  }
 //  else{
 // 	 G4int maximumbin=NbWithTime->GetMaximumBin();
 // 	 OriginalPeakTime=NbWithTime->GetBinCenter(maximumbin);
 // 	 for(Int_t bini=0;bini<originalnbofpoint;bini++){
 // 		Double_t tmpy=NbWithTime->GetBinContent(maximumbin-bini);
 // 		OriginalIAroundThre.push_back(tmpy);
 // 	 }
 //  }

  G4int maximumbin=IWithTime->GetMaximumBin();
  G4int originalnbofpoint=80;
  OriginalPeakTime=IWithTime->GetBinCenter(maximumbin);
  for(Int_t bini=0;bini<originalnbofpoint;bini++){
	 Double_t tmpy=IWithTime->GetBinContent(maximumbin-bini);
	 OriginalIAroundThre.push_back(tmpy);
  }
  
}
//Get the waveform
void MRPCSignal::CalculateWaveForm(){
  G4int randomtime;
  if(RandomPeakFlag){
	 gRandom->SetSeed();
	 randomtime=Peak.first+round(gRandom->Uniform(-PointStep/2,PointStep/2));
  }
  else
	 randomtime=Peak.first;
  
  ExperimentsPeakTime=ResponseIWithTime->GetBinCenter(randomtime)+TruthTime;
  // G4cout<<"startrandom time= "<<randomtime<<" Pointstep= "<<PointStep<<G4endl;
  PointTimeStep=ResponseIWithTime->GetBinCenter(randomtime-PointStep)-ResponseIWithTime->GetBinCenter(randomtime-2*PointStep);
  // for(Int_t i=0;i<PointStep*10;i++)
  // 	 G4cout<<Peak.first-i<<" "<<ResponseIWithTime->GetBinContent(Peak.first-i)<<G4endl;;
	 
  //from the peak to the start point
  for(Int_t bini=0;bini<NbofPointAroundThre;bini++){
    // if(ResponseIWithTime->GetBinCenter(randomtime-bini*PointStep)<TruthTime) break;
    Double_t tmpy=ResponseIWithTime->GetBinContent(randomtime-bini*PointStep);
	 // G4cout<<randomtime-bini*PointStep<<" "<<tmpy<<G4endl;
    //if(tmpy<4e-5) break;
    InducedQAroundThre.push_back(tmpy+gRandom->Gaus(0,NoisePer*WaveMaxValue));
  }
}




//Avalanche: Get the total number of electrons with respect to time
void MRPCSignal::Avalanche(G4int gapID, G4ThreeVector PosWithRespectToStrip,G4double StartTime,G4int PrimaryElectrons){
  //Iterarion
  G4double timetoelectrode;
  // if(gapID%2==0) timetoelectrode=(Gapsize.z()+PosWithRespectToStrip.z())/Velocity;//-PosWithRespectToStrip.z()/Velocity;///
  // else timetoelectrode=-PosWithRespectToStrip.z()/Velocity;

  //Particles drift from the top to the bottom of the gap
  //PosWithRespectToStrip.z(): the negative distance between collision to the top of the gap.
  timetoelectrode=(Gapsize.z()+PosWithRespectToStrip.z())/Velocity;//Gapsize.z()+
  // G4cout<<Gapsize.z()+PosWithRespectToStrip.z()<<G4endl;
  // G4cout<<PosWithRespectToStrip.z()<<" "<<PrimaryElectrons<<G4endl;
  G4int totalNbofstep=MaxScale*(timetoelectrode/TimeStep);
 
  if(totalNbofstep>0){
    
    G4double IniTimeBin=NbWithTime->FindBin(StartTime);
   
    //Gain
    G4double naverage=exp(EffAlpha*ZStep);
    G4double scompare=TownsendK*(naverage-1)/(naverage-TownsendK); //Sampling for avalanche
    G4double Nbofele=PrimaryElectrons;

    TH1D *Nbpereventwithtime=new TH1D("Neveryelectrontime","Nbof electron with time",NbWithTime->GetNbinsX(),NbWithTime->GetXaxis()->GetXmin(),NbWithTime->GetXaxis()->GetXmax());

    NbWithTime->SetBinContent(IniTimeBin,NbWithTime->GetBinContent(IniTimeBin)+PrimaryElectrons);

    gRandom->SetSeed();
    // G4cout<<"avalanche total Nbofstep="<<totalNbofstep<<G4endl;
    for(Int_t stepi=1;stepi<totalNbofstep;stepi++){ 
      if(Nbofele<SpaceChargeThre){
		  G4double n=0; 
		  if(Nbofele>1000){//In Cern Thesis, this is 150 at Page64
			 G4double mean=Nbofele*naverage;
			 G4double sigma=sqrt(Nbofele*(1+TownsendK)/(1-TownsendK)*naverage*(naverage-1));
			 Nbofele=gRandom->Gaus(mean,sigma);
			 if(Nbofele>SpaceChargeThre) Nbofele=SpaceChargeThre;
		  }
		  else{
			 for(Int_t ni=0;ni<Nbofele;ni++){
				G4double s=gRandom->Uniform(0,1);
				if(s<scompare) n+=0;
				else n+=1+(G4int)(log((naverage-TownsendK)*(1-s)/naverage/(1-TownsendK))/log(1-(1-TownsendK)/(naverage-TownsendK)));
			 }
			 Nbofele=n;
			 if(Nbofele>SpaceChargeThre) Nbofele=SpaceChargeThre;
		  }
      }//Over Nbofele<SpaceChargeThre
      else Nbofele=SpaceChargeThre;
      NbWithTime->SetBinContent(IniTimeBin+stepi,NbWithTime->GetBinContent(IniTimeBin+stepi)+Nbofele);
      Nbpereventwithtime->SetBinContent(IniTimeBin+stepi,Nbofele);
		// G4cout<<IniTimeBin+stepi<<" Nbofele "<<Nbofele<<" Nwitht "<<NbWithTime->GetBinContent(IniTimeBin+stepi)<<G4endl;
    }//loop over every time step for avalanche
    // NbWithTime->SetBinContent(IniTimeBin+totalNbofstep,NbWithTime->GetBinContent(IniTimeBin+totalNbofstep)+TMath::Max(0.,NbWithTime->GetBinContent(IniTimeBin+totalNbofstep-1)-Nbpereventwithtime->GetBinContent(IniTimeBin+totalNbofstep-1)));
    Nbpereventwithtime->SetBinContent(totalNbofstep,0);
	 // G4cout<<"---------------"<<NbWithTime->GetBinContent(IniTimeBin+totalNbofstep)<<G4endl;

    if(doDiff){
      bool AllArrived=false;
      G4double timemean=timetoelectrode;
      // G4cout<<"Start time "<<StartTime<<" timearrived "<<timemean<<" time cross gap "<<TimeCrossGap<<G4endl;
      G4double timesigma=DiffLengthZ*sqrt(timetoelectrode/TimeCrossGap)/Velocity;
      G4double startbin=timemean/TimeStep;
      // G4cout<<"startbin "<<startbin<<" total "<<totalNbofstep<<G4endl;
      for(Int_t stepi=startbin;stepi<totalNbofstep+1;stepi++)
		  NbWithTime->SetBinContent(IniTimeBin+stepi,NbWithTime->GetBinContent(IniTimeBin+stepi)-Nbpereventwithtime->GetBinContent(IniTimeBin+stepi));//update the Nbofele
      // for(Int_t stepi=1;stepi<totalNbofstep;stepi++){ 
      //   G4double bincontent,arrivedN=0;
      //   if(!AllArrived){
      // 	arrivedN+=Nbofele/sqrt(2*pi)/timesigma*exp(-0.5*pow((stepi*TimeStep-timemean)/timesigma,2))*TimeStep;//Nbofele arrived the anode
      // 	bincontent=Nbpereventwithtime->GetBinContent(IniTimeBin+stepi);//Nbofele at present
      // 	if(bincontent<arrivedN){
      // 	  AllArrived=true;
      // 	  arrivedN=bincontent;
      // 	  // G4cout<<"binsmall "<<NbWithTime->GetBinContent(IniTimeBin+stepi)-arrivedN<<G4endl;
      // 	}
      //   }//Over if electrons haven't arrived
      //   else{ arrivedN=Nbpereventwithtime->GetBinContent(IniTimeBin+stepi);
      // 	G4cout<<"allarrive "<<NbWithTime->GetBinContent(IniTimeBin+stepi)-arrivedN<<G4endl;
      //   }
      //   if(NbWithTime->GetBinContent(IniTimeBin+stepi)-arrivedN<0) G4cout<<"Stranve!"<<G4endl;
      //   NbWithTime->SetBinContent(IniTimeBin+stepi,NbWithTime->GetBinContent(IniTimeBin+stepi)-arrivedN);//update the Nbofele
      // }//loop over every time step for diffuse
    }//Over do diffuse

    // TCanvas *c2=new TCanvas("c2","c2",0,0,1000,600);
    // c2->Divide(2);
    // c2->cd(1);
    // Nbpereventwithtime->Draw();
    // Nbpereventwithtime->GetXaxis()->SetTitle("Time /ns");
    // Nbpereventwithtime->GetXaxis()->CenterTitle();
    // c2->cd(2);
    // NbWithTime->Draw();
    // NbWithTime->GetXaxis()->SetTitle("Time /ns");
    // NbWithTime->GetXaxis()->CenterTitle();
    // TString name1;
    // c2->SaveAs(name1.Format("%.4f",StartTime)+"pre.png");
    Nbpereventwithtime->Delete();
  }//totalNbofstep>0
}

void MRPCSignal::AvalancheWithoutUncertainty(G4double posz,G4double StartTime,G4int PrimaryElectrons){
  //Particles drift from the top to the bottom of the gap
  //posz: the negative distance between collision to the top of the gap.
  G4double timetoelectrode=(Gapsize.z()+posz)/Velocity;
  // G4double timetoelectrode=(-posz)/Velocity;
  G4double totalNbofstep=timetoelectrode/TimeStep;
  // G4cout<<"Avalanche="<<posz<<G4endl;
  if(totalNbofstep>0){
	 G4double IniTimeBin=NbWithTime->FindBin(StartTime);
	 NbWithTime->SetBinContent(IniTimeBin,NbWithTime->GetBinContent(IniTimeBin)+PrimaryElectrons);
	 G4double naverage=exp(EffAlpha*ZStep);
	 
	 G4double Nbofele=PrimaryElectrons;
	 for(G4int stepi=1;stepi<totalNbofstep;stepi++){
		if(Nbofele<SpaceChargeThre){
		  Nbofele=Nbofele*naverage;
		  if(Nbofele>SpaceChargeThre) Nbofele=SpaceChargeThre;
		}
		else Nbofele=SpaceChargeThre;
		NbWithTime->SetBinContent(IniTimeBin+stepi,NbWithTime->GetBinContent(IniTimeBin+stepi)+Nbofele);
	 }
	 // G4cout<<"Drift length="<<-posz<<" Max Nbofele="<<NbWithTime->GetMaximum()<<G4endl;
  }	
}

 
std::vector<std::pair<G4int,G4double> > MRPCSignal::GetLeadingLength(TH1D* hist,G4double percentofMax){
  
  std::pair<G4int,G4double> leadingstart,leadingend;
  leadingend=FindHistPeakValue(hist);
  // std::cout<<"leading end "<<leadingend.first<<" second "<<leadingend.second<<std::endl;
  for(G4int i=0;i<hist->GetNbinsX();i++){
    if(hist->GetBinContent(i)>percentofMax*leadingend.second){
      leadingstart.first=i;
      leadingstart.second=hist->GetBinContent(i);
      break;
    }
  }
  std::vector<std::pair<G4int,G4double> > leadstartend;
  leadstartend.push_back(leadingstart);
  leadstartend.push_back(leadingend);
  return leadstartend;
}
G4int MRPCSignal::GetLeadingNbofPoints(TH1D* hist,G4double percentofMax,std::pair<G4int,G4double> peak){
  
  std::pair<G4int,G4double> leadingstart,leadingend;
  leadingend=peak;//FindHistPeakValue(hist);
  for(G4int i=0;i<hist->GetNbinsX();i++){
    if(hist->GetBinContent(i)>percentofMax*leadingend.second){
      // G4cout<<"thisy "<<hist->GetBinContent(i)<<G4endl;
      leadingstart.first=i;
      leadingstart.second=hist->GetBinContent(i);
      break;
    }
  }
  return (Int_t)((G4double)(hist->GetBinCenter(leadingend.first)-hist->GetBinCenter(leadingstart.first))/TimeInterval);
 
}

std::pair<G4int,G4double> MRPCSignal::FindHistPeakValue(TH1D* hist){
  std::pair<G4int,G4int> maxtime=std::make_pair(0,hist->GetNbinsX()*3.5/4.);
  Int_t nbins=hist->GetNbinsX();
  Int_t stopdis=TMath::Max(4.,0.0003*nbins);
  // stopdis=2;
  while((maxtime.second-maxtime.first)>stopdis){
    G4int step=(G4int)TMath::Max(1.,0.0001*(maxtime.second-maxtime.first));
    maxtime=FindWavePeak(maxtime,step,hist);
  }
  // G4cout<<"FindPeak="<<hist->GetBinCenter(maxtime.first)<<G4endl;
  return std::make_pair(round(maxtime.first+maxtime.second)/2.,hist->GetBinContent(round(maxtime.first+maxtime.second)/2.));
  // return std::make_pair(maxtime.first,hist->GetBinContent(maxtime.first));
}



std::pair<G4int,G4int> MRPCSignal::FindWavePeak(std::pair<G4int,G4int> range,G4int step, TH1D* hist){
  G4int Nbofiter=(range.second-range.first)/step;
  for(G4int i=2;i<Nbofiter-2;i++){
    G4double content=hist->GetBinContent(i*step+range.first);
	 // G4cout<<"i="<<i<<" "<<content<<" "<<hist->GetBinContent((i-2)*step+range.first)<<" "<<hist->GetBinContent((i-1)*step+range.first)<<" "<<hist->GetBinContent((i+1)*step+range.first)<<" "<<hist->GetBinContent((i+2)*step+range.first)<<G4endl;
    if(content>hist->GetBinContent((i-2)*step+range.first)&&content>hist->GetBinContent((i-1)*step+range.first)&&content>hist->GetBinContent((i+1)*step+range.first)&&content>hist->GetBinContent((i+2)*step+range.first))
      return std::make_pair((i-1)*step+range.first,(i+1)*step+range.first);
  }
  return std::make_pair(0,0);
}


G4int MRPCSignal::GetLeadingEdge(TH1D* hist,G4double threshold){
  gRandom->SetSeed();
  G4int leadingend;
  for(G4int i=1;i<hist->GetNbinsX();i++){
    G4double value=hist->GetBinContent(i);//+gRandom->Gaus(0,NoisePer*WaveMaxValue);
    if(value>threshold){
      OverThreshold=true;
      G4double slope1=hist->GetBinContent(i+1)-hist->GetBinContent(i-1);
      G4double slope2=TMath::Abs(hist->GetBinContent(i+1)-hist->GetBinContent(i-1))/2.;
      G4double slope=TMath::Abs(hist->GetBinContent(i+1)-hist->GetBinContent(i-1))/2./hist->GetBinWidth(1);
      leadingend=hist->FindBin(gRandom->Gaus(hist->GetBinCenter(i),NoisePer*WaveMaxValue/slope));
      break;
    }
  }
  return leadingend;
}


G4int MRPCSignal::GetTrailingEdge(TH1D* hist,G4double threshold){
  gRandom->SetSeed();
  G4int trailingend;
  for(G4int i=Peak.first+200;i<hist->GetNbinsX();i++){
    G4double value=hist->GetBinContent(i);//+
    if(value<threshold){
      G4double slope=(hist->GetBinContent(i+1)-hist->GetBinContent(i-1))/2./hist->GetBinWidth(1);
      trailingend=hist->FindBin(gRandom->Gaus(hist->GetBinCenter(i),NoisePer*WaveMaxValue/slope));
      // G4cout<<hist->FindBin(hist->GetBinCenter(i))<<" "<<hist->GetBinCenter(i)<<" "<<trailingend<<" "<<gRandom->Gaus(hist->GetBinCenter(i),NoisePer*WaveMaxValue/slope)<<G4endl;
      break;
    }
  }
  return trailingend;
}




G4double MRPCSignal::ElectronicsResponseFunction(G4double x){
  G4double tau1=0.5,tau2=1.5,amplitude=10;//ns
  tau1=Tau1;tau2=Tau2;amplitude=Amplitude;
  //5 gaps tau1=0.2,tau2=1.3,amplitude=2.5
  //15 gaps tau1=0.5,tau2=1.58,amplitude=250
  //33 gaps tau1=0.5,tau2=1.5,amplitude=100;
  return -amplitude*(exp(-(x-StartTimeOffset)/tau1)-exp(-(x-StartTimeOffset)/tau2));
}

bool MRPCSignal::ResetGasParameter(G4double electricField){
  electricField=10.*electricField;
  G4int totalNboftxtline=48;
  TString title,infilename;
  G4double tmpfield,tmpalpha,tmpeta,tmpvelocity,readtmp;
  std::ifstream in;
  if(GasMix=="Standard") infilename="GasData.txt";
  else infilename="GasData955.txt";
  in.open(infilename);
  in>>title>>title>>title>>title;
  for(G4int linei=0;linei<totalNboftxtline;linei++){
    in>>readtmp;  
    if(readtmp>=electricField){
      if(linei==0){
	in.close();
	return false;
      }
      G4double k=(electricField-tmpfield)/(readtmp-tmpfield);
      in>>readtmp; 
      Alpha=(k*(readtmp-tmpalpha)+tmpalpha)/10.;
      in>>readtmp; 
      Eta=(k*(readtmp-tmpeta)+tmpeta)/10;
      in>>readtmp; 
      Velocity=(k*(readtmp-tmpvelocity)+tmpvelocity)*10.;
      in.close();
      return true;
    }
    tmpfield=readtmp;
    in>>readtmp;  tmpalpha=readtmp;
    in>>readtmp;  tmpeta=readtmp;
    in>>readtmp;  tmpvelocity=readtmp;
  }
  in.close();
  return false;

}
