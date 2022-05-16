#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TEfficiency.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "Math/Vector4D.h"
#include "TRotation.h"
#include "THStack.h"

using namespace ROOT::Math;

void macro_analysis(){
	// declaration of file paths
	TChain* c_sig = new TChain("c_sig");
  c_sig->Add("ntuple_mumuHZZ4mu.root/T");
  
  //variables for MonteCarlo particles
  
  Int_t n_mcp=0;    // number of MonteCarlo particles per event
  Float_t *mc_mox;  // momentum x
  Float_t *mc_moy;  // momentum y 
  Float_t *mc_moz;  // momentum z
  Float_t *mc_ene;  // rnergy
  Int_t *mc_pdg;    // pdg code
  Int_t *mc_pa0;    // parent index
  Int_t *mc_gst;    // simulation status (gst=1 --> not decayed)
  
  int num=200000;
  mc_mox = (float *) malloc(sizeof(float) * num);
  mc_moy = (float *) malloc(sizeof(float) * num);
  mc_moz = (float *) malloc(sizeof(float) * num);
  mc_ene = (float *) malloc(sizeof(float) * num);
  mc_pdg = (int *) malloc(sizeof(int) * num);
  mc_pa0 = (int *) malloc(sizeof(int) * num);
  mc_gst = (int *) malloc(sizeof(int) * num);
  
  
  // variables for reco particles
  
  Int_t n_rec;    // numbero of reco particles per event
  Int_t *rc_typ;  //pdg code
  Float_t *rc_mox;
  Float_t *rc_moy;
  Float_t *rc_moz;
  Float_t *rc_ene;
  
  int num2=3000;
  rc_mox = (float *) malloc(sizeof(float) * num2);
  rc_moy = (float *) malloc(sizeof(float) * num2);
  rc_moz = (float *) malloc(sizeof(float) * num2);
  rc_ene = (float *) malloc(sizeof(float) * num2);
  rc_typ = (int *) malloc(sizeof(int) * num2);  
  
  
   ////  branches for MonteCarlo particles  /////////////////////////////////
  c_sig->SetBranchAddress("nmcp", &n_mcp);
  c_sig->SetBranchAddress("mcmox", mc_mox);
  c_sig->SetBranchAddress("mcmoy", mc_moy);
  c_sig->SetBranchAddress("mcmoz", mc_moz);
  c_sig->SetBranchAddress("mcpdg", mc_pdg);
  c_sig->SetBranchAddress("mcpa0", mc_pa0);
  c_sig->SetBranchAddress("mcgst", mc_gst);
  c_sig->SetBranchAddress("mcene", mc_ene);
  
  
  ////  branches for reconstructed particles   //////////////////////////
  c_sig->SetBranchAddress("nrec", &n_rec);
  c_sig->SetBranchAddress("rctyp", rc_typ);
  c_sig->SetBranchAddress("rcmox", rc_mox);
  c_sig->SetBranchAddress("rcmoy", rc_moy);
  c_sig->SetBranchAddress("rcmoz", rc_moz);
  c_sig->SetBranchAddress("rcene", rc_ene);
  
  
  //// Histograms   ///////////////////////////////////////////
  
  TH1F *histo_pT_mu1reco = new TH1F ("histo pT mu1 reco","histo pT mu1 reco",100,0,500);
  TH1F *histo_pT_mu2reco = new TH1F ("histo pT mu2 reco","histo pT mu2 reco",100,0,500);
  TH1F *histo_pT_mu3reco = new TH1F ("histo pT mu3 reco","histo pT mu3 reco",100,0,500);
  TH1F *histo_pT_mu4reco = new TH1F ("histo pT mu4 reco","histo pT mu4 reco",100,0,500);
  TH1F *histo_pT_mu1MC = new TH1F ("histo pT mu1 MC","histo pT mu1 MC",100,0,500);
  TH1F *histo_pT_mu2MC = new TH1F ("histo pT mu2 MC","histo pT mu2 MC",100,0,500);
  TH1F *histo_pT_mu3MC = new TH1F ("histo pT mu3 MC","histo pT mu3 MC",100,0,500);
  TH1F *histo_pT_mu4MC = new TH1F ("histo pT mu4 MC","histo pT mu4 MC",100,0,500);
  
  TH1F *histo_Hmass_MC = new TH1F ("histo Higgs mass MC","histo Higgs mass MC",300,0,300);
  TH1F *histo_Hmass_RC = new TH1F ("histo Higgs mass RC","histo Higgs mass RC",300,0,300);
  
  TH1F *histo_px= new TH1F ("histo px","histo px",100,0,100);
  
  // declaring varoables ////////////////
  int muMC_counter=0, muRC_counter=0; 
  
  TLorentzVector muMC_p4[6];
  TLorentzVector muRC_p4[6];
  TLorentzVector HiggsMC_p4, HiggsRC_p4;
  
  
  // loop on events
  for(unsigned int ientry=0; ientry<c_sig->GetEntries(); ++ientry){
  	c_sig->GetEntry(ientry);
  	cout<<"#############################################################"<<endl;
  	
  	muMC_counter=0; 
  	muRC_counter=0;
  	
  	//loop on MonteCarlo particles
  	for(unsigned int imc=0; imc<n_mcp; ++imc){ //select muons in the final state
  		if(abs(mc_pdg[imc])==13 && mc_gst[imc]==1){
  			histo_px->Fill(mc_mox[imc]);
  			cout<<"Mc muon: "<<muMC_counter<<endl;
  			cout<<"px: "<<mc_mox[imc]<<"   py: "<<mc_moy[imc]<<"   pz: "<<mc_moz[imc]<<"   ene:"<<mc_ene[imc]<<endl;
  			muMC_p4[muMC_counter].SetPxPyPzE(mc_mox[imc],mc_moy[imc],mc_moz[imc],mc_ene[imc]);
  			cout<<"mass: "<<muMC_p4[muMC_counter].M()<<endl;
  			muMC_counter++;
  		}
  	}
  	
  	if(muMC_counter>=4){
  		HiggsMC_p4 = muMC_p4[0] + muMC_p4[1] + muMC_p4[2] + muMC_p4[3];
  		histo_Hmass_MC->Fill(HiggsMC_p4.M());
  		
  		//Fill Histograms 
  		histo_pT_mu1MC->Fill(muMC_p4[0].Pt());
  		histo_pT_mu2MC->Fill(muMC_p4[1].Pt());
  		histo_pT_mu3MC->Fill(muMC_p4[2].Pt());
  		histo_pT_mu4MC->Fill(muMC_p4[3].Pt());
  		
  	}
  	
  	//loop on reconstructed particles
  	for(unsigned int irc=0; irc<n_rec; ++irc){
  		if(abs(rc_typ[irc])==13){
  			muRC_p4[muRC_counter].SetPxPyPzE(rc_mox[irc],rc_moy[irc],rc_moz[irc],rc_ene[irc]);
  			muRC_counter++;
  			
  		}
  	}
  	
  	if(muRC_counter>=4){
  		HiggsRC_p4 = muRC_p4[0] + muRC_p4[1] + muRC_p4[2] + muRC_p4[3];
  		
  		histo_pT_mu1reco->Fill(muRC_p4[0].Pt());
  		histo_pT_mu2reco->Fill(muRC_p4[1].Pt());
  		histo_pT_mu3reco->Fill(muRC_p4[2].Pt());
  		histo_pT_mu4reco->Fill(muRC_p4[3].Pt());
  		histo_Hmass_RC->Fill(HiggsRC_p4.M());
  	}
  	
  	//cout<<"number of MC muons: "<<muMC_counter<<endl;
  	//cout<<"number of RECO muons: "<<muRC_counter<<endl;
  	
   	
  	
  }
  
  //histo_Hmass_MC->Draw();
  //histo_Hmass_RC->Draw();
  //histo_pT_mu1reco->Draw();
  //histo_px->Draw();
  
  //free(mc_mox); free(mc_moy); free(mc_moz); free(mc_ene); free(mc_pdg); free(rc_typ); free(rc_mox); free(rc_moy); free(mc_moz); free(mc_ene);
  
}
