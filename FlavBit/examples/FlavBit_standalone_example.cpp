//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  
///  Example of GAMBIT FlavBit standalone
///  main program.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///   
///  \author Martin White
///          (martin.white@adelaide.edu.au)
///  \date Jan 2016
///
///  \author Pat Scott
///          (p.scott@imperial.ac.uk)
///  \date Sep 2016
///
///  *********************************************

// Always required in any standalone module main file
#include "gambit/Utils/standalone_module.hpp"
#include "gambit/FlavBit/FlavBit_rollcall.hpp"

// Just required in this one
#include "gambit/Elements/spectrum_factories.hpp"
#include "gambit/Models/SimpleSpectra/MSSMSimpleSpec.hpp"

using namespace FlavBit::Accessors;     // Helper functions that provide some info about the module
using namespace FlavBit::Functown;      // Functors wrapping the module's actual module functions
using namespace BackendIniBit::Functown;    // Functors wrapping the backend initialisation functions

// Default SLHA file for input, if not given on the command line.
std::string inputfile("FlavBit/data/example.slha");

QUICK_FUNCTION(FlavBit, MSSM_spectrum, NEW_CAPABILITY, createSpectrum, Spectrum, (MSSM30atQ,MSSM30atMGUT))

namespace Gambit
{
  namespace FlavBit
  {

    // Make a GAMBIT spectrum object from an SLHA file
    void createSpectrum(Spectrum& outSpec)
    {
      outSpec = spectrum_from_SLHA<MSSMSimpleSpec>(inputfile);     
    }
    
  }
}

int main(int argc, char** argv)
{

  cout << "starting" << endl;

  try
  {
      
    cout << "starting" << endl;
    // Get the SLHA filename from the command line, if it has been given.
    if (argc >= 2) inputfile = argv[1];

    cout << "starting" << endl;

    // Make a logging object
    std::map<std::string, std::string> loggerinfo;
    cout << "starting" << endl;
    
    // Define where the logs will end up
    std::string prefix("runs/FlavBit_standalone/logs/");

    // Ensure that the above directory exists
    Utils::ensure_path_exists(prefix);
    
    // Add entries to the loggerinfo map
    loggerinfo["Core, Error"] = prefix+"core_errors.log";
    loggerinfo["Default"]     = prefix+"default.log";
    loggerinfo["Warning"]     = prefix+"warnings.log";
    loggerinfo["FlavBit, Info"] = prefix+"FlavBit_info.log";
    
    // Initialise global LogMaster object
    logger().initialise(loggerinfo);
    
    logger()<<"Running FlavBit standalone example"<<LogTags::info<<EOM;
    
    std::cout << std::endl << "My name is " << name() << std::endl;
    std::cout << " I can calculate: " << endl << iCanDo << std::endl;
    std::cout << " ...but I may need: " << endl << iMayNeed << std::endl << std::endl;

    createSpectrum.notifyOfModel("MSSM30atQ");
    
    // Set up the b2sll_LL likelihood
    // Have to resolve dependencies by hand
    // b2sll_likelihood depends on:
    //    - b2sll_M
    b2sll_likelihood.resolveDependency(&b2sll_measurements);

    //SI_fill depends on:
    //   - BEreq Init_param
    //   - BEreq slha_adjust
    //   - BEopt SuperIso, 3.4
    //   - MSSM_spectrum
    SI_fill.resolveDependency(&createSpectrum);
    SI_fill.resolveBackendReq(&Backends::SuperIso_3_6::Functown::Init_param);
    SI_fill.resolveBackendReq(&Backends::SuperIso_3_6::Functown::slha_adjust);

    // b2sll_measurements depends on:
    //   - BRBKstarmumu_11_25
    //   - BRBKstarmumu_25_40
    //   - BRBKstarmumu_40_60
    //   - BRBKstarmumu_60_80
    //   - BRBKstarmumu_15_17
    //   - BRBKstarmumu_17_19
    b2sll_measurements.resolveDependency(&SI_BRBKstarmumu_11_25);
    b2sll_measurements.resolveDependency(&SI_BRBKstarmumu_25_40);
    b2sll_measurements.resolveDependency(&SI_BRBKstarmumu_40_60);
    b2sll_measurements.resolveDependency(&SI_BRBKstarmumu_60_80);
    b2sll_measurements.resolveDependency(&SI_BRBKstarmumu_15_17);
    b2sll_measurements.resolveDependency(&SI_BRBKstarmumu_17_19);

    // Now resolve dependencies of the BKstar mu mu measurements
    // Each depends on:
    // - SI_fill
    // Each has a BE requirement on:
    // - BRBKstarmumu_CONV (from SuperIso)
    SI_BRBKstarmumu_11_25.resolveDependency(&SI_fill);
    SI_BRBKstarmumu_11_25.resolveBackendReq(&Backends::SuperIso_3_6::Functown::BRBKstarmumu_CONV);
    SI_BRBKstarmumu_25_40.resolveDependency(&SI_fill);
    SI_BRBKstarmumu_25_40.resolveBackendReq(&Backends::SuperIso_3_6::Functown::BRBKstarmumu_CONV);
    SI_BRBKstarmumu_40_60.resolveDependency(&SI_fill);
    SI_BRBKstarmumu_40_60.resolveBackendReq(&Backends::SuperIso_3_6::Functown::BRBKstarmumu_CONV);
    SI_BRBKstarmumu_60_80.resolveDependency(&SI_fill);
    SI_BRBKstarmumu_60_80.resolveBackendReq(&Backends::SuperIso_3_6::Functown::BRBKstarmumu_CONV);
    SI_BRBKstarmumu_15_17.resolveDependency(&SI_fill);
    SI_BRBKstarmumu_15_17.resolveBackendReq(&Backends::SuperIso_3_6::Functown::BRBKstarmumu_CONV);
    SI_BRBKstarmumu_17_19.resolveDependency(&SI_fill);
    SI_BRBKstarmumu_17_19.resolveBackendReq(&Backends::SuperIso_3_6::Functown::BRBKstarmumu_CONV);

    // Now do the b2ll likelihood
    b2ll_likelihood.resolveDependency(&b2ll_measurements);

    // b2ll_measurements depends on:
    // - Bsmumu_untag
    // - Bdmumu
    b2ll_measurements.resolveDependency(&SI_Bsmumu_untag);
    b2ll_measurements.resolveDependency(&SI_Bdmumu);

    // Resolve dependencies of SI_Bsmumu_untag
    // These are:
    //  - SI_fill
    // Plus BE reqs:
    //  - Bsll_untag_CONV
    SI_Bsmumu_untag.resolveDependency(&SI_fill);
    SI_Bsmumu_untag.resolveBackendReq(&Backends::SuperIso_3_6::Functown::Bsll_untag_CONV);
    
    // Resolve dependencies of SI_Bdmumu
    // These are:
    //  - SI_fill
    // Plus BE reqs:
    // - Bdmumu
    // - CW_calculator
    // - C_calculator_base1
    // - CQ_calculator   
    SI_Bdmumu.resolveDependency(&SI_fill);
    SI_Bdmumu.resolveBackendReq(&Backends::SuperIso_3_6::Functown::Bdll_CONV);

    // Now do the semi-leptonic likelihood SL_LL
    // This depends on:
    // - SL_M
    SL_likelihood.resolveDependency(&SL_measurements);

    // Resolve dependencies of SL_measurements
    // which are:
    // - Btaunu
    // - BDtaunu
    // - Kmunu_pimunu
    // - Dstaunu
    // - Dsmunu
    // - Dmunu
    // - BDmunu
    // - BDstartaunu
    // - BDstarmunu
    SL_measurements.resolveDependency(&SI_Btaunu);
    SL_measurements.resolveDependency(&SI_BDtaunu);
    SL_measurements.resolveDependency(&SI_Kmunu_pimunu);
    SL_measurements.resolveDependency(&SI_Dstaunu);
    SL_measurements.resolveDependency(&SI_Dsmunu);
    SL_measurements.resolveDependency(&SI_Dmunu);
    SL_measurements.resolveDependency(&SI_BDmunu);
    SL_measurements.resolveDependency(&SI_BDstartaunu);
    SL_measurements.resolveDependency(&SI_BDstarmunu);

    // Resolve all of the individual dependencies and backend reqs
    // These are:
    // - SI_fill
    // BE Req: BDtaunu, etc
    SI_Btaunu.resolveDependency(&SI_fill);
    SI_Btaunu.resolveBackendReq(&Backends::SuperIso_3_6::Functown::Btaunu);
    SI_BDtaunu.resolveDependency(&SI_fill);
    SI_BDtaunu.resolveBackendReq(&Backends::SuperIso_3_6::Functown::BRBDlnu);
    SI_RD.resolveDependency(&SI_fill);
    SI_RD.resolveBackendReq(&Backends::SuperIso_3_6::Functown::BDtaunu_BDenu);
    SI_Kmunu_pimunu.resolveDependency(&SI_fill);
    SI_Kmunu_pimunu.resolveBackendReq(&Backends::SuperIso_3_6::Functown::Kmunu_pimunu);
    SI_Dstaunu.resolveDependency(&SI_fill);
    SI_Dstaunu.resolveBackendReq(&Backends::SuperIso_3_6::Functown::Dstaunu);
    SI_Dsmunu.resolveDependency(&SI_fill);
    SI_Dsmunu.resolveBackendReq(&Backends::SuperIso_3_6::Functown::Dsmunu);
    SI_Dmunu.resolveDependency(&SI_fill);
    SI_Dmunu.resolveBackendReq(&Backends::SuperIso_3_6::Functown::Dmunu);

    // Now resolve dependencies for the b->sgamma likelihoods 
    b2sgamma_likelihood.resolveDependency(&SI_bsgamma);

    // Resolve dependencies and backend requirements of SI_bsgamma:
    // - SI_fill
    // Plus BE reqs:
    // - bsgamma_CONV
    SI_bsgamma.resolveDependency(&SI_fill);
    SI_bsgamma.resolveBackendReq(&Backends::SuperIso_3_6::Functown::bsgamma_CONV);
    
    // Double-check which backend requirements have been filled with what
    std::cout << std::endl << "My function SI_fill has had its backend requirement on Init_param filled by:" << std::endl;
    std::cout << FlavBit::Pipes::SI_fill::BEreq::Init_param.origin() << "::";
    std::cout << FlavBit::Pipes::SI_fill::BEreq::Init_param.name() << std::endl;
    std::cout << std::endl << "My function SI_fill has had its backend requirement on slha_adjust filled by:" << std::endl;
    std::cout << FlavBit::Pipes::SI_fill::BEreq::slha_adjust.origin() << "::";
    std::cout << FlavBit::Pipes::SI_fill::BEreq::slha_adjust.name() << std::endl;

    // Double-check which backend requirements have been filled with what
    std::cout << std::endl << "My function SI_Bdmumu  has had its backend requirement on Bdll_CONV filled by:" << std::endl;
    std::cout << FlavBit::Pipes::SI_Bdmumu::BEreq::Bdll_CONV.origin() << "::";
    std::cout << FlavBit::Pipes::SI_Bdmumu::BEreq::Bdll_CONV.name() << std::endl;    
    
    // Double-check which dependencies have been filled with whatever (not every combination is shown)
    std::cout << std::endl << "My function SI_fill has had its dependency on MSSM_spectrum filled by:" << endl;
    std::cout << FlavBit::Pipes::SI_fill::Dep::MSSM_spectrum.origin() << "::";
    std::cout << FlavBit::Pipes::SI_fill::Dep::MSSM_spectrum.name() << std::endl;

    std::cout << std::endl << "My function b2sll_likelihood has had its dependency on b2sll_M filled by:" << endl;
    std::cout << FlavBit::Pipes::b2sll_likelihood::Dep::b2sll_M.origin() << "::";
    std::cout << FlavBit::Pipes::b2sll_likelihood::Dep::b2sll_M.name() << std::endl;
    
    std::cout << std::endl << "My function b2sll_measurements has had its dependency on BRBKstarmumu_11_25 filled by:" << endl;
    std::cout << FlavBit::Pipes::b2sll_measurements::Dep::BRBKstarmumu_11_25.origin() << "::";
    std::cout << FlavBit::Pipes::b2sll_measurements::Dep::BRBKstarmumu_11_25.name() << std::endl;
    std::cout << std::endl << "My function b2sll_measurements has had its dependency on BRBKstarmumu_25_40 filled by:" << endl;
    std::cout << FlavBit::Pipes::b2sll_measurements::Dep::BRBKstarmumu_25_40.origin() << "::";
    std::cout << FlavBit::Pipes::b2sll_measurements::Dep::BRBKstarmumu_25_40.name() << std::endl;
    std::cout << std::endl << "My function b2sll_measurements has had its dependency on BRBKstarmumu_40_60 filled by:" << endl;
    std::cout << FlavBit::Pipes::b2sll_measurements::Dep::BRBKstarmumu_40_60.origin() << "::";
    std::cout << FlavBit::Pipes::b2sll_measurements::Dep::BRBKstarmumu_40_60.name() << std::endl;
    std::cout << std::endl << "My function b2sll_measurements has had its dependency on BRBKstarmumu_60_80 filled by:" << endl;
    std::cout << FlavBit::Pipes::b2sll_measurements::Dep::BRBKstarmumu_60_80.origin() << "::";
    std::cout << FlavBit::Pipes::b2sll_measurements::Dep::BRBKstarmumu_60_80.name() << std::endl;
    std::cout << std::endl << "My function b2sll_measurements has had its dependency on BRBKstarmumu_15_17 filled by:" << endl;
    std::cout << FlavBit::Pipes::b2sll_measurements::Dep::BRBKstarmumu_15_17.origin() << "::";
    std::cout << FlavBit::Pipes::b2sll_measurements::Dep::BRBKstarmumu_15_17.name() << std::endl;
    std::cout << std::endl << "My function b2sll_measurements has had its dependency on BRBKstarmumu_17_19 filled by:" << endl;
    std::cout << FlavBit::Pipes::b2sll_measurements::Dep::BRBKstarmumu_17_19.origin() << "::";
    std::cout << FlavBit::Pipes::b2sll_measurements::Dep::BRBKstarmumu_17_19.name() << std::endl;
    
    // Start running here
    {

      double loglike;
      std::cout << std::endl;

      // Call the initialisation functions for all backends that are in use. 
      SuperIso_3_6_init.reset_and_calculate();

      // Now call the module functions in an appropriate order
      createSpectrum.reset_and_calculate();
      SI_fill.reset_and_calculate();

      // Calculate the B -> ll likelihood
      SI_Bsmumu_untag.reset_and_calculate();
      SI_Bdmumu.reset_and_calculate();
      b2ll_measurements.reset_and_calculate();
      b2ll_likelihood.reset_and_calculate();
      loglike = b2ll_likelihood(0);
      std::cout << "Fully leptonic B decays (B->ll) joint log-likelihood: " << loglike << std::endl;

      // Calculate the B -> Xs ll likelihood
      SI_BRBKstarmumu_11_25.reset_and_calculate();
      SI_BRBKstarmumu_25_40.reset_and_calculate();
      SI_BRBKstarmumu_40_60.reset_and_calculate();
      SI_BRBKstarmumu_60_80.reset_and_calculate();
      SI_BRBKstarmumu_15_17.reset_and_calculate();
      SI_BRBKstarmumu_17_19.reset_and_calculate();
      b2sll_measurements.reset_and_calculate();
      b2sll_likelihood.reset_and_calculate();
      loglike = b2sll_likelihood(0);
      std::cout << "Leptonic penguin B decays (B->X_s ll) joint log-likelihood: " << loglike << std::endl;

      // Calculate the semi-leptonic (SL) likelihood
      SI_Btaunu.reset_and_calculate();
      SI_BDtaunu.reset_and_calculate();
      SI_RD.reset_and_calculate();
      SI_Kmunu_pimunu.reset_and_calculate();
      SI_Dstaunu.reset_and_calculate();
      SI_Dsmunu.reset_and_calculate();
      SI_Dmunu.reset_and_calculate();
      SL_measurements.reset_and_calculate();
      SL_likelihood.reset_and_calculate();
      loglike = SL_likelihood(0);
      std::cout << "Semi-leptonic B decays (B->D l nu) joint log-likelihood: " << loglike << std::endl;
      
      // Calculate the B -> X_s gamma likelihood
      SI_bsgamma.reset_and_calculate();
      b2sgamma_likelihood.reset_and_calculate();
      loglike = b2sgamma_likelihood(0);
      std::cout << "B->X_s gamma log-likelihood: " << loglike << std::endl;

      std::cout << endl;

    }
    
  }

  catch (std::exception& e)
  {
    std::cout << "FlavBit_standalone example has exited with fatal exception: " << e.what() << std::endl;
  }

}