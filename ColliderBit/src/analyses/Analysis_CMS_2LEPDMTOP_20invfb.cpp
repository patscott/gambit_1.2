#include <vector>
#include <cmath>
#include <memory>
#include <iomanip>

#include "gambit/ColliderBit/analyses/BaseAnalysis.hpp"
#include "gambit/ColliderBit/ATLASEfficiencies.hpp"

/// @todo Remove the ROOT classes...

using namespace std;

// The CMS 2 lepton DM + top pair analysis (20fb^-1)

// based on: https://twiki.cern.ch/twiki/bin/view/CMSPublic/PhysicsResultsB2G13004

//    Code by Martin White, Guy Pitman
//    Known issues:
//    a) Impossible to test results against CMS due to the impossibility of reproducing their model information (even after contacting CMS). Note that the variables used have been debugged in other contexts however.
//    b) Overlap removal is not applied (CMS do not use it, but we don't exactly use their particle flow technique either)
//    c) Jets here need kT radius of 0.5 not 0.4
//    d) Jets go out to eta of 5: need to make sure that sim does not cut these away

namespace Gambit {
  namespace ColliderBit {


    class Analysis_CMS_2LEPDMTOP_20invfb : public HEPUtilsAnalysis {
    private:

      // Numbers passing cuts
      int _numSR;

      vector<int> cutFlowVector;
      vector<string> cutFlowVector_str;
      int NCUTS; //=24;

      // Debug histos

    public:

      Analysis_CMS_2LEPDMTOP_20invfb()
        : _numSR(0),
          NCUTS(6)
      {
        set_luminosity(19.7);
        for (int i=0; i<NCUTS; i++) {
          cutFlowVector.push_back(0);
          cutFlowVector_str.push_back("");
        }
      }

      double SmallestdPhi(std::vector<HEPUtils::Jet *> jets,double phi_met)
      {
	if (jets.size()<2) return(999);
	double dphi1 = std::acos(std::cos(jets.at(0)->phi()-phi_met));
	double dphi2 = std::acos(std::cos(jets.at(1)->phi()-phi_met));
	// double dphi3 = 999;
	//if (jets.size() > 2 && jets[2]->pT() > 40.)
	//  dphi3 = std::acos(std::cos(jets[2]->phi() - phi_met));
	double min1 = std::min(dphi1, dphi2);

	return min1;

      }

      void analyze(const HEPUtils::Event* event) {
        HEPUtilsAnalysis::analyze(event);

        // Missing energy
        // HEPUtils::P4 ptot = event->missingmom();
        double met = event->met();

        // Now define vectors of baseline objects
	vector<HEPUtils::Particle*> baselineLeptons;

        vector<HEPUtils::Particle*> baselineElectrons;
        for (HEPUtils::Particle* electron : event->electrons()) {
          if (electron->pT() > 20. && fabs(electron->eta()) < 2.5) {
	    baselineElectrons.push_back(electron);
	    baselineLeptons.push_back(electron);
	  }
        }
        vector<HEPUtils::Particle*> baselineMuons;
        for (HEPUtils::Particle* muon : event->muons()) {
          if (muon->pT() > 20. && fabs(muon->eta()) < 2.4) {
	    baselineMuons.push_back(muon);
	    baselineLeptons.push_back(muon);
	  }
        }

        vector<HEPUtils::Jet*> baselineJets;
	vector<HEPUtils::P4> jets;
        vector<HEPUtils::Jet*> bJets;
	vector<bool> btag;

	const std::vector<double>  a = {0,10.};
        const std::vector<double>  b = {0,10000.};
        const std::vector<double> c = {0.60};
        HEPUtils::BinnedFn2D<double> _eff2d(a,b,c);

        for (HEPUtils::Jet* jet : event->jets()) {
          if (jet->pT() > 30. && fabs(jet->eta()) < 5.0) {
	    baselineJets.push_back(jet);
	    //LorentzVector j1 (jet->mom().px(),jet->mom().py(),jet->mom().pz(),jet->mom().E()) ;
	    jets.push_back(jet->mom());
	    bool hasTag=has_tag(_eff2d, jet->eta(), jet->pT());
	    bool isB=false;

	    if(jet->btag() && hasTag && fabs(jet->eta()) < 2.4 && jet->pT() > 30.) {
	      isB=true;
	      bJets.push_back(jet);
	    }
	    btag.push_back(isB);
	  }
        }

        // Calculate common variables and cuts first
        //applyTightIDElectronSelection(signalElectrons);

        //int nElectrons = signalElectrons.size();
        //int nMuons = signalMuons.size();
        int nJets = baselineJets.size();
        int nLeptons = baselineLeptons.size();
	// int nBJets = bJets.size();

	//Preselection cuts

	bool passMll=true;
	if(baselineElectrons.size()==2 && baselineMuons.size()==0){
	  double mll=(baselineElectrons[0]->mom()+baselineElectrons[1]->mom()).m();
	  if(mll<=20.)passMll=false;
	  if(fabs(mll-91.)<=15.)passMll=false;
	}

	if(baselineMuons.size()==2 && baselineElectrons.size()==0){
	  double mll=(baselineMuons[0]->mom()+baselineMuons[1]->mom()).m();
	  if(mll<=20.)passMll=false;
	  if(fabs(mll-91.)<=15.)passMll=false;
	}

	bool passPresel=false;
	if(nLeptons==2 &&
	   nJets>=2 &&
	   passMll)passPresel=true;

	//Cuts

	// met > 320 GeV
	// Scalar sum of leading 2 jet pTs < 400 GeV
	// Scalar sum of pT of leptons > 120 GeV
	// dPhi_ll < 2.

	double jetPtSum=0;
	if(nJets>=2)jetPtSum=baselineJets[0]->pT()+baselineJets[1]->pT();
	double lepPtSum=0;
	if(baselineLeptons.size()==2)lepPtSum=baselineLeptons[0]->pT()+baselineLeptons[1]->pT();

	double dPhiLL=99.;
	if(baselineLeptons.size()==2)dPhiLL=acos(cos((baselineLeptons[0]->phi() - baselineLeptons[1]->phi())));

	cutFlowVector_str[0] = "No cuts ";
        cutFlowVector_str[1] = "Presel ";
        cutFlowVector_str[2] = "MET > 320 GeV ";
        cutFlowVector_str[3] = "pT_j1 + pT_j2 < 400 GeV ";
        cutFlowVector_str[4] = "pT_l1 + pT_l2 > 120 GeV ";
        cutFlowVector_str[5] = "dPhi_ll < 2. ";

        for(int j=0;j<NCUTS;j++){
          if(
             (j==0) ||

             (j==1 && passPresel) ||

             (j==2 && passPresel && met > 320.) ||

             (j==3 && passPresel && met > 320. && jetPtSum < 400.) ||

             (j==4 && passPresel && met > 320. && jetPtSum < 400. && lepPtSum > 120.) ||

             (j==5 && passPresel && met > 320. && jetPtSum < 400. && lepPtSum > 120. && dPhiLL < 2.))

            cutFlowVector[j]++;
        }

        //We're now ready to apply the cuts for each signal region
        //_numSR1, _numSR2, _numSR3;

        if(passPresel && met > 320. && jetPtSum < 400. && lepPtSum > 120. && dPhiLL < 2.)_numSR++;

        return;
      }


      void add(BaseAnalysis* other) {
        // The base class add function handles the signal region vector and total # events.
        HEPUtilsAnalysis::add(other);

        Analysis_CMS_2LEPDMTOP_20invfb* specificOther
                = dynamic_cast<Analysis_CMS_2LEPDMTOP_20invfb*>(other);

        // Here we will add the subclass member variables:
        if (NCUTS != specificOther->NCUTS) NCUTS = specificOther->NCUTS;
        for (int j=0; j<NCUTS; j++) {
          cutFlowVector[j] += specificOther->cutFlowVector[j];
          cutFlowVector_str[j] = specificOther->cutFlowVector_str[j];
        }
        _numSR += specificOther->_numSR;
      }


      double loglikelihood() {
        /// @todo Implement!
        return 0;
      }

      void collect_results() {
        SignalRegionData results_SR;
        results_SR.set_observation(1.);
        results_SR.set_background(1.89);
        results_SR.set_backgroundsys(0.66);
        results_SR.set_signalsys(0.);
        results_SR.set_signal(_numSR);

        add_result(results_SR);

        return;
      }

    };

    DEFINE_ANALYSIS_FACTORY(CMS_2LEPDMTOP_20invfb)


  }
}
