#include "TMINLP.hpp"
#include "IpBlas.hpp"



/** default constructor for Sos constraints */
Ipopt::TMINLP::SosInfo::SosInfo():
        num(0), 
        types(NULL), 
        priorities(NULL), 
        numNz(0), 
        starts(NULL),
        indices(NULL), 
        weights(NULL)
{}

/** Copy constructor.*/
Ipopt::TMINLP::SosInfo::SosInfo(const SosInfo & source):
        num(source.num), 
        types(NULL), 
        priorities(NULL), 
        numNz(source.numNz), 
        starts(NULL),
        indices(NULL),
        weights(NULL)
{

  if(num > 0) {
    assert(source.types!=NULL);
    assert(source.priorities!=NULL);
    assert(source.starts!=NULL);
    assert(source.indices!=NULL);
    assert(source.weights!=NULL);
    types = new char[num];
    priorities = new int[num];
    starts = new int[num + 1];
    indices = new int[numNz];
    weights = new double[numNz];
    for(int i = 0 ; i < num ; i++) {
      source.types[i] = types[i];
      source.priorities[i] = priorities[i];
      source.starts[i] = starts[i];
    }
    for(int i = 0 ; i < numNz ; i++) {
      source.indices[i] = indices[i];
      source.weights[i] = weights[i];
    }
  }
  else {
    assert(source.types==NULL);
    assert(source.priorities==NULL);
    assert(source.starts==NULL);
    assert(source.indices==NULL);
    assert(source.weights==NULL);
  }

}


/** Reset information */
void 
Ipopt::TMINLP::SosInfo::gutsOfDestructor()
{
  num = 0;
  numNz = 0;
  if(types) delete [] types;
  types = NULL;
  if(starts) delete [] starts;
  starts = NULL;
  if(indices) delete [] indices;
  indices = NULL;
  if(priorities) delete [] priorities;
  priorities = NULL;
  if(weights) delete [] weights;
  weights = NULL;
}


Ipopt::TMINLP::TMINLP():
    jCol_(NULL),
    iRow_(NULL),
    elems_(NULL),
    lower_(NULL),
    upper_(NULL),
    nLinearCuts_(0),
    linearCutsNnz_(0),
    linearCutsCapacity_(0),
    linearCutsNnzCapacity_(0) 
    {}


void
Ipopt::TMINLP::addCuts(int numberCuts, const OsiRowCut ** cuts){

 int n,m,nnz_lag,nnz_hess;
 Ipopt::TNLP::IndexStyleEnum fort;
 get_nlp_info(n,m,nnz_lag,nnz_hess,fort);

 //count the number of non-zeroes
 //Cuts have to be added by rows for removeCuts to work
 int nnz = linearCutsNnz_;
 for(int i = 0 ; i < numberCuts ; i++)
 {
   nnz += cuts[i]->row().getNumElements();
 }
 int resizeNnz = nnz > linearCutsNnzCapacity_;
 int resizeCuts = nLinearCuts_ + numberCuts > linearCutsCapacity_;
 resizeLinearCuts(max((1+ resizeCuts) * linearCutsCapacity_, resizeCuts * numberCuts + linearCutsCapacity_), 
                  max((1 + resizeNnz) * linearCutsNnzCapacity_, resizeNnz * nnz + linearCutsNnzCapacity_));

 /* reinit nnz */
 nnz = linearCutsNnz_; 

 for(int i = 0 ; i < numberCuts ; i++)
 {
   const int * ind = cuts[i]->row().getIndices();
   const double * values = cuts[i]->row().getElements();
   int size = cuts[i]->row().getNumElements();
   int iplusn = i + nLinearCuts_;
   for(int j = 0; j < size ; j++)
   {
    DBG_ASSERT(ind[j] < n);
    jCol_[nnz] = ind[j];
    iRow_[nnz] = iplusn;
    elems_[nnz++] = values[j];
   }
   lower_[iplusn] = cuts[i]->lb();
   upper_[iplusn] = cuts[i]->ub();
  }
 nLinearCuts_+=numberCuts;
 linearCutsNnz_=nnz;
}

void
Ipopt::TMINLP::resizeLinearCuts(int newNumberCuts, int newNnz)
{
  if(newNumberCuts > linearCutsCapacity_)
  {
     double * newLower = new double[newNumberCuts];
     double * newUpper = new double[newNumberCuts];
     if(linearCutsCapacity_)
     {
       IpBlasDcopy(nLinearCuts_, lower_, 1, newLower, 1);
       IpBlasDcopy(nLinearCuts_, upper_, 1, newUpper, 1);
       delete [] lower_;
       delete [] upper_;
     }
     lower_ = newLower;
     upper_ = newUpper;
     linearCutsCapacity_ = newNumberCuts;
  }
  if(newNnz > linearCutsNnzCapacity_)
  {
    double * newElems = new double [newNnz];
    int * newiRow = new int [newNnz];
    int * newjCol = new int [newNnz];
    if(linearCutsNnz_)
    {
      IpBlasDcopy(linearCutsNnz_, elems_, 1, newElems, 1);
      for(int i = 0 ; i < linearCutsNnz_ ; i++) 
      {
        newiRow[i] = iRow_[i];
        newjCol[i] = jCol_[i];
      }
      delete [] elems_;
      delete [] iRow_;
      delete [] jCol_;
    }
    elems_ = newElems;
    iRow_ = newiRow;
    jCol_ = newjCol;
    linearCutsNnzCapacity_ = newNnz;
  }
}
void
Ipopt::TMINLP::removeCuts(int number, const int * toRemove){
 if(! CoinIsSorted(toRemove, number))
 {
   int * sorted = new int[number];
   for(int i = 0 ; i < number ; i++) sorted[i] = toRemove[i];
   std::sort(sorted, sorted + number);  
   int iNew = 0;
   int k = 0;
   for(int i = 0 ; i < linearCutsNnz_ ; i++)
   {
     if(toRemove[k] < iRow_[i]) k++;
     if(toRemove[k] < iRow_[i]) throw -1;
     if(toRemove[k] == iRow_[i]) continue;
     iRow_[iNew] = iRow_[i] - k;
     jCol_[iNew] = jCol_[i];
     elems_[iNew++] = jCol_[i];
   }
 linearCutsNnz_ = iNew;
 nLinearCuts_ -= number;
 }
}
void
Ipopt::TMINLP::removeLastCuts(int number){
 number = nLinearCuts_ - number;
 int iNew = 0;
   for(int i = 0 ; i < linearCutsNnz_ ; i++)
   {
     if(iRow_[i] >= number) {
     continue;}
     iRow_[iNew] = iRow_[i];
     jCol_[iNew] = jCol_[i];
     elems_[iNew++] = elems_[i];
   }
 linearCutsNnz_ = iNew;
 nLinearCuts_ = number;
 std::cout<<"Number of cuts remaining "<<nLinearCuts_<<std::endl;
 }
