// (C) Copyright Carnegie Mellon University 2005
// All Rights Reserved.
// This code is published under the Common Public License.
//
// Authors :
// P. Bonami, Carnegie Mellon University
//
// Date :  05/26/2005

#include "IpCbcOACutGenerator.hpp"
#include "OsiAuxInfo.hpp"
/// Default constructor
IpCbcOACutGenerator::IpCbcOACutGenerator(IpoptInterface * si,
    int maxDepth)
    :
    CglCutGenerator(),
    nlp_(si),
    maxDepth_(maxDepth),
    nSolve_(0)
{
  handler_ = new CoinMessageHandler();
  handler_ -> setLogLevel(1);
  messages_ = OaMessages();
}
/// Assign an IpoptInterface
void
IpCbcOACutGenerator::assignInterface(IpoptInterface * si)

{
  nlp_ = si;
}
static int nCalls = 0;
/// cut generation method
void
IpCbcOACutGenerator::generateCuts( const OsiSolverInterface & si, OsiCuts & cs,
    const CglTreeInfo info) const
{
  if(nlp_ == NULL) {
    std::cerr<<"Error in cut generator for outer approximation no ipopt NLP assigned"<<std::endl;
    throw -1;
  }

  int numcols = nlp_->getNumCols();

  //Get the continuous solution
  //const double *colsol = si.getColSolution();
  //Check for integer feasibility
  nCalls++;
  if(info.level > maxDepth_ || info.pass > 0 || !info.inTree  )
    return;
  //Fix the variable which have to be fixed, after having saved the bounds
  double * saveColLb = new double[numcols];
  double * saveColUb = new double[numcols];
  CoinCopyN(nlp_->getColLower(), numcols , saveColLb);
  CoinCopyN(nlp_->getColUpper(), numcols , saveColUb);
  for(int i = 0; i < numcols ; i++) {
    if(nlp_->isInteger(i)) {
      nlp_->setColBounds(i,si.getColLower()[i],si.getColUpper()[i]);
    }
  }

  //Now solve the NLP get the cuts, reset bounds and get out

  //  nlp_->turnOnIpoptOutput();
  nSolve_++;
  nlp_->resolve();
  nlp_->getOuterApproximation(cs);
  if(nlp_->isProvenOptimal()) {
    handler_->message(LP_ERROR,messages_)
    <<nlp_->getObjValue()-si.getObjValue()<<CoinMessageEol;
    bool feasible = 1;
    const double * colsol2 = nlp_->getColSolution();
    for(int i = 0 ; i < numcols && feasible; i++) {
      if(nlp_->isInteger(i)) {
        if(fabs(colsol2[i] - floor(colsol2[i] + 0.5) ) > 1e-07)
          feasible = 0;
      }
    }
    if(feasible ) {
#if 1
      // Also store into solver
      OsiAuxInfo * auxInfo = si.getAuxiliaryInfo();
      OsiBabSolver * auxiliaryInfo = dynamic_cast<OsiBabSolver *> (auxInfo);
      if (auxiliaryInfo) {
        double * lpSolution = new double[numcols + 1];
        CoinCopyN(colsol2, numcols, lpSolution);
        lpSolution[numcols] = nlp_->getObjValue();
        auxiliaryInfo->setSolution(lpSolution, numcols + 1, lpSolution[numcols]);
        delete [] lpSolution;
      }
      else
        printf("No auxiliary info in nlp solve!\n");
#endif

    }
  }
  else if(nlp_->isAbandoned() || nlp_->isIterationLimitReached()) {
    std::cerr<<"Unsolved NLP ... exit"<<std::endl;
    nlp_->turnOnIpoptOutput();
    nlp_->resolve();
    throw -1;
  }
  else {
    //   std::cout<<"Infeasible NLP => Infeasible node"<<std::endl;
    //       //Add an infeasibility local constraint
    //       CoinPackedVector v;
    //       double rhs = 1.;
    //       for(int i = 0; i < numcols ; i++)
    // 	{
    // 	  if(nlp_->isInteger(i) && (si.getColUpper()[i] - si.getColLower()[i] < 0.9))
    // 	    {
    // 	      double value = floor(colsol[i] + 0.5);
    // 	      assert(fabs(colsol[i]-value)<1e-8 && value >= -1e-08 && value <= 1+ 1e08);
    // 	      v.insert(i, -(2*value - 1));
    // 	      rhs -= value;
    // 	    }
    // 	}
    //       OsiRowCut cut;
    //       cut.setRow(v);
    //       cut.setLb(rhs);
    //       cut.setUb(1e300);
    //       cut.setGloballyValid();
    //       cs.insert(cut);
  }
  for(int i = 0; i < numcols ; i++) {
    if(nlp_->isInteger(i)) {
      nlp_->setColBounds(i,saveColLb[i],saveColUb[i]);
    }
  }
  delete [] saveColLb;
  delete [] saveColUb;
}