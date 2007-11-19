// (C) Copyright International Business Machines Corporation 2007
// All Rights Reserved.
// This code is published under the Common Public License.
//
// Authors :
// Pierre Bonami, International Business Machines Corporation
//
// Date : 04/18/2007
#ifndef BonCouenneSetup_H
#define BonCouenneSetup_H
#include "BonBabSetupBase.hpp"
#include "CbcFeasibilityBase.hpp"

struct ASL;



namespace Bonmin{
  
  class SmartAsl : public Ipopt::ReferencedObject{
public:
    ASL * asl;
    SmartAsl():
      Ipopt::ReferencedObject(),
      asl(NULL)
    {}
    virtual ~SmartAsl();
  };
  
  class CouenneSetup : public BabSetupBase{
public:
    /** Default constructor*/
    CouenneSetup():
    BabSetupBase(),
    aslfg_(NULL){}
    
    /** Copy constructor.*/
    CouenneSetup(const CouenneSetup& other):
      BabSetupBase(other),
      aslfg_(NULL){}
    
    /** virtual copy constructor.*/
    virtual BabSetupBase * clone() const{
      return new CouenneSetup(*this);
    }
    
    virtual ~CouenneSetup();
    /** Initialize from command line arguments.*/
    void InitializeCouenne(char **& argv);
    /** register the options */
    virtual void registerOptions();
    /** Register all Couenne options.*/
    static void registerAllOptions(Ipopt::SmartPtr<Bonmin::RegisteredOptions> roptions);
    
    /** Get the basic options if don't already have them.*/
    virtual void readOptionsFile(){
      if(readOptions_) return;
      BabSetupBase::readOptionsFile("couenne.opt");}

private:
      SmartPtr<SmartAsl> aslfg_;
  };
  
}

#endif