// (C) Copyright CNRS and others 2010
// All Rights Reserved.
// This code is published under the Common Public License.
//
// Authors :
// Pierre Bonami, Université de la Méditérannée
// Hassan Hijazi, Orange Labs
//
// Date : 05/22/2010

#include "BonminConfig.h"
#include "OsiClpSolverInterface.hpp"

#include "BonBonminSetup.hpp"
#include "BonHeuristicInnerApproximation.hpp"
namespace Bonmin
{
  SepaSetup::SepaSetup(const CoinMessageHandler * handler):BonminSetup(handler)
  {}

  SepaSetup::SepaSetup(const SepaSetup &other):BonminSetup(other)
  {}

  SepaSetup::SepaSetup(const SepaSetup &other,
                           OsiTMINLPInterface &nlp):
      BonminSetup(other, nlp)
  {
  }

  SepaSetup::SepaSetup(const SepaSetup &other,
                           OsiTMINLPInterface &nlp,
                           const std::string &prefix):
    BonminSetup(other, nlp, prefix)
  {
   Alorithm algo = getAlgorithm();
    if (algo_ == B_OA)
      initializeSepa();
  }

  void SepaSetup::registerAllOptions(Ipopt::SmartPtr<Bonmin::RegisteredOptions> roptions)
  {
     BonminSetup::registerAllOptions(roptions);
     HeuristicInnerApproximation::registerOptions(roptions);

        roptions->SetRegisteringCategory("Initial Approximations descriptions", RegisteredOptions::UndocumentedCategory);
	roptions->AddStringOption2("initial_outer_description",
		"Do we add all Outer Approximation constraints defining the initial Outer Approximation description of the MINLP. See the number_approximations_initial_outer option for fixing the number of approximation points",
		"no",
		"yes","Generate the description",
		"no","Do not generate the description",
		"");
	roptions->AddUpperBoundedIntegerOption("number_approximations_initial_outer",
		"Number of Outer Approximation points needed for generating the initial Outer Approximation description, maximum value = 500, default value = 50",
		500,
		50,
		"");
  }

  /** Register all the Bonmin options.*/
  void
  SepaSetup::registerOptions()
  {
    registerAllOptions(roptions_);
  }

  /** Initialize, read options and create appropriate bonmin setup using initialized tminlp.*/
  void
  SepaSetup::initialize(Ipopt::SmartPtr<TMINLP> tminlp, bool createContinuousSolver /*= false*/)
  {
    BonminSetup::initialize(tminlp, createContinuousSolver);
    if (algo_ == B_OA)
      initializeSepa();
  }

  /** Initialize, read options and create appropriate bonmin setup using initialized tminlp.*/
  void
  BonminSetup::initialize(const OsiTMINLPInterface &nlpSi, bool createContinuousSolver /*= false*/)
  {
    BonminSetup::initialize(nlpSi, createContinuousSolver);
    if (algo_ == B_OA)
      initializeSepa();
  }

  BonminSetup::initializeSepa(bool createContinuousSolver /*= false*/)
  {

    if(true){
      HeuristicInnerApproximation * inner = new InnerApproximation(this);
      HeuristicMethod h;
      h.heuristic = inner;
      h.id = "InnerApproximation";
      heuristics_.push_back(h);
    }

  }

}/* end namespace Bonmin*/
