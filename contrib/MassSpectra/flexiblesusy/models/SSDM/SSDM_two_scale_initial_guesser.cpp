// ====================================================================
// This file is part of FlexibleSUSY.
//
// FlexibleSUSY is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// FlexibleSUSY is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FlexibleSUSY.  If not, see
// <http://www.gnu.org/licenses/>.
// ====================================================================

// File generated at Wed 28 Oct 2015 11:34:33

#include "SSDM_two_scale_initial_guesser.hpp"
#include "SSDM_two_scale_model.hpp"
#include "lowe.h"
#include "error.hpp"
#include "ew_input.hpp"
#include "wrappers.hpp"

#include <Eigen/Core>
#include <cassert>

namespace flexiblesusy {

#define INPUTPARAMETER(p) model->get_input().p
#define MODELPARAMETER(p) model->get_##p()
#define PHASE(p) model->get_##p()
#define LowEnergyConstant(p) Electroweak_constants::p
#define MODEL model

SSDM_initial_guesser<Two_scale>::SSDM_initial_guesser(
   SSDM<Two_scale>* model_,
   const softsusy::QedQcd& oneset_,
   const SSDM_low_scale_constraint<Two_scale>& low_constraint_,
   const SSDM_susy_scale_constraint<Two_scale>& susy_constraint_,
   const SSDM_high_scale_constraint<Two_scale>& high_constraint_
)
   : Initial_guesser<Two_scale>()
   , model(model_)
   , oneset(oneset_)
   , mu_guess(0.)
   , mc_guess(0.)
   , mt_guess(0.)
   , md_guess(0.)
   , ms_guess(0.)
   , mb_guess(0.)
   , me_guess(0.)
   , mm_guess(0.)
   , mtau_guess(0.)
   , running_precision(1.0e-3)
   , low_constraint(low_constraint_)
   , susy_constraint(susy_constraint_)
   , high_constraint(high_constraint_)
{
   assert(model && "SSDM_initial_guesser: Error: pointer to model"
          " SSDM<Two_scale> must not be zero");
}

SSDM_initial_guesser<Two_scale>::~SSDM_initial_guesser()
{
}

/**
 * Guesses the DR-bar model parameters by calling
 * guess_susy_parameters() and guess_soft_parameters() .
 */
void SSDM_initial_guesser<Two_scale>::guess()
{
   guess_susy_parameters();
   guess_soft_parameters();
}

/**
 * Guesses the SUSY parameters (gauge, Yukawa couplings) at
 * \f$m_\text{top}^\text{pole}\f$ from the Standard Model gauge
 * couplings and fermion masses.  Threshold corrections are ignored.
 * The user-defined initial guess at the low-scale
 * (InitialGuessAtLowScale) is applied here:
 *
 * \code{.cpp}
   const auto HiggsIN = INPUTPARAMETER(HiggsIN);
   const auto Lambda2Input = INPUTPARAMETER(Lambda2Input);
   const auto Lambda3Input = INPUTPARAMETER(Lambda3Input);
   const auto mS2Input = INPUTPARAMETER(mS2Input);

   calculate_Yu_DRbar();
   calculate_Yd_DRbar();
   calculate_Ye_DRbar();
   MODEL->set_mu2(Re(HiggsIN));
   MODEL->set_LamSH(Re(Lambda2Input));
   MODEL->set_LamS(Re(Lambda3Input));
   MODEL->set_MS2(Re(mS2Input));

 * \endcode
 */
void SSDM_initial_guesser<Two_scale>::guess_susy_parameters()
{
   using namespace softsusy;

   softsusy::QedQcd leAtMt(oneset);
   const double MZ = Electroweak_constants::MZ;
   const double MW = Electroweak_constants::MW;
   const double sinThetaW2 = 1.0 - Sqr(MW / MZ);
   const double mtpole = leAtMt.displayPoleMt();

   mu_guess = leAtMt.displayMass(mUp);
   mc_guess = leAtMt.displayMass(mCharm);
   mt_guess = leAtMt.displayMass(mTop) - 30.0;
   md_guess = leAtMt.displayMass(mDown);
   ms_guess = leAtMt.displayMass(mStrange);
   mb_guess = leAtMt.displayMass(mBottom);
   me_guess = leAtMt.displayMass(mElectron);
   mm_guess = leAtMt.displayMass(mMuon);
   mtau_guess = leAtMt.displayMass(mTau);

   // guess gauge couplings at mt
   const DoubleVector alpha_sm(leAtMt.getGaugeMu(mtpole, sinThetaW2));

   model->set_g1(sqrt(4.0 * M_PI * alpha_sm(1)));
   model->set_g2(sqrt(4.0 * M_PI * alpha_sm(2)));
   model->set_g3(sqrt(4.0 * M_PI * alpha_sm(3)));
   model->set_scale(mtpole);

   // apply user-defined initial guess at the low scale
   const auto HiggsIN = INPUTPARAMETER(HiggsIN);
   const auto Lambda2Input = INPUTPARAMETER(Lambda2Input);
   const auto Lambda3Input = INPUTPARAMETER(Lambda3Input);
   const auto mS2Input = INPUTPARAMETER(mS2Input);

   calculate_Yu_DRbar();
   calculate_Yd_DRbar();
   calculate_Ye_DRbar();
   MODEL->set_mu2(Re(HiggsIN));
   MODEL->set_LamSH(Re(Lambda2Input));
   MODEL->set_LamS(Re(Lambda3Input));
   MODEL->set_MS2(Re(mS2Input));

}

void SSDM_initial_guesser<Two_scale>::calculate_DRbar_yukawa_couplings()
{
   calculate_Yu_DRbar();
   calculate_Yd_DRbar();
   calculate_Ye_DRbar();
}

/**
 * Calculates the Yukawa couplings Yu of the up-type quarks
 * from the Standard Model up-type quark masses (ignoring threshold
 * corrections).
 */
void SSDM_initial_guesser<Two_scale>::calculate_Yu_DRbar()
{
   Eigen::Matrix<std::complex<double>,3,3> topDRbar(ZEROMATRIXCOMPLEX(3,3));
   topDRbar(0,0) = mu_guess;
   topDRbar(1,1) = mc_guess;
   topDRbar(2,2) = mt_guess;

   const auto v = MODELPARAMETER(v);
   MODEL->set_Yu((-((1.4142135623730951*topDRbar)/v).transpose()).real());

}

/**
 * Calculates the Yukawa couplings Yd of the down-type
 * quarks from the Standard Model down-type quark masses (ignoring
 * threshold corrections).
 */
void SSDM_initial_guesser<Two_scale>::calculate_Yd_DRbar()
{
   Eigen::Matrix<std::complex<double>,3,3> bottomDRbar(ZEROMATRIXCOMPLEX(3,3));
   bottomDRbar(0,0) = md_guess;
   bottomDRbar(1,1) = ms_guess;
   bottomDRbar(2,2) = mb_guess;

   const auto v = MODELPARAMETER(v);
   MODEL->set_Yd((((1.4142135623730951*bottomDRbar)/v).transpose()).real());

}

/**
 * Calculates the Yukawa couplings Ye of the leptons
 * from the Standard Model down-type lepton masses (ignoring threshold
 * corrections).
 */
void SSDM_initial_guesser<Two_scale>::calculate_Ye_DRbar()
{
   Eigen::Matrix<std::complex<double>,3,3> electronDRbar(ZEROMATRIXCOMPLEX(3,3));
   electronDRbar(0,0) = me_guess;
   electronDRbar(1,1) = mm_guess;
   electronDRbar(2,2) = mtau_guess;

   const auto v = MODELPARAMETER(v);
   MODEL->set_Ye((((1.4142135623730951*electronDRbar)/v).transpose()).real());

}

/**
 * Guesses the soft-breaking parameters.  At first it runs to the
 * guess of the high-scale (HighScaleFirstGuess) and imposes the
 * high-scale constraint (HighScaleInput):
 *
 * \code{.cpp}

 * \endcode
 *
 * Afterwards, it runs to the low-scale guess (LowScaleFirstGuess) and
 * solves the EWSB conditions at the tree-level.  Finally the DR-bar
 * mass spectrum is calculated.
 */
void SSDM_initial_guesser<Two_scale>::guess_soft_parameters()
{
   const double low_scale_guess = low_constraint.get_initial_scale_guess();
   const double high_scale_guess = high_constraint.get_initial_scale_guess();

   model->run_to(high_scale_guess, running_precision);

   // apply high-scale constraint
   high_constraint.set_model(model);
   high_constraint.apply();

   // apply user-defined initial guess at the high scale


   model->run_to(low_scale_guess, running_precision);

   // apply EWSB constraint
   model->solve_ewsb_tree_level();

   // calculate tree-level spectrum
   model->calculate_DRbar_masses();
}

} // namespace flexiblesusy