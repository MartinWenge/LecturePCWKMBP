#ifndef ANALYZERSCATTERING_H
#define ANALYZERSCATTERING_H

#include <LeMonADE/analyzer/AbstractAnalyzer.h>
#include <LeMonADE/utility/RandomNumberGenerators.h>
#include <cmath>
#include <complex>

template<class IngredientsType>
class AnalyzerScattering: public AbstractAnalyzer {
public:
  //! constructor
  AnalyzerScattering(const IngredientsType&, std::string filename);
  //! destructor
  ~AnalyzerScattering(){}
  
   //! public function used by taskManager: initialize
  virtual void initialize();
  //! public functions used by taskManager: run
  virtual bool execute();
  //! public functions used by taskManager: cleanup
  virtual void cleanup();

private:
  //! Storage for data like coordinates, bond information, ...
  const IngredientsType& ingredients;
  
  //! Random Number Generator (RNG) to be used in this analyzer
  RandomNumberGenerators rng;
  
  //! number of conformations processed
  int32_t numOfExec;
  
  //! number of different q vectors between lattice constant an 4 times boxsize
  uint32_t num_of_q;
  
  //! output filename
  std::string filename;
  
  //! container for averaged squared absolute value of scattering amplitude
  std::vector<double> averagedSquaredAbsC_q;
  
  //! container for q-vector absolute values
  std::vector<double> q_factor;
  
  // private functions:
  //! calculate the scattering amplitude
  void CalcScatteringAmplitude();
  //! prepare the q-values to have sizes between inverse boxsize and inverse lattice constant
  void initQFactor();
  //! get a random normalized vector which is inside the unit sphere
  VectorDouble3 GetRandQVector();
};

/******************************************************************************/
/**
 * @fn void AnalyzerScattering<IngredientsType>::AnalyzerScattering()
 * @brief Constructor: declaration of intern molecule properties
 */
template<class IngredientsType>
AnalyzerScattering<IngredientsType>::AnalyzerScattering(
  const IngredientsType& ingredients_, std::string filename_):
  ingredients(ingredients_), 
  numOfExec(0),
  num_of_q(100),
  q_factor(),
  filename(filename_),
  averagedSquaredAbsC_q(num_of_q,0.0)
{}
  
/******************************************************************************/
/**
 * @fn void AnalyzerScattering<IngredientsType>::initialize()
 * @brief preparation of q vector array
 */
template<class IngredientsType>
void AnalyzerScattering<IngredientsType>::initialize(){
  std::cout << "\nAnalyzerScattering initialise\n";
  
  initQFactor();

  execute();
}

/******************************************************************************/
/**
 * @fn void AnalyzerScattering<IngredientsType>::execute()
 * @brief Calculates the scattering amplitude of the current frame
 */
template<class IngredientsType>
bool AnalyzerScattering<IngredientsType>::execute(){
  numOfExec++;
  CalcScatteringAmplitude();
  std::cout<<".";
  if( (numOfExec % 50) == 0) std::cout<<std::endl;
  
  return true;
}

/******************************************************************************/
/**
 * @fn void AnalyzerScattering<IngredientsType>::cleanup()
 * @brief Write out results
 */
template<class IngredientsType>
void AnalyzerScattering<IngredientsType>::cleanup(){
  std::cout<<std::endl;
  std::vector<std::vector<double> > results(2,std::vector<double>(num_of_q,0.0));
  
  for (uint32_t i=0;i<q_factor.size();i++){
    results.at(0).at(i)=( q_factor.at(i) / (num_of_q));
    results.at(1).at(i)=( averagedSquaredAbsC_q.at(i) / ( num_of_q * ingredients.getMolecules().size() * numOfExec ));
  }
  
  ResultFormattingTools::writeResultFile(filename, ingredients, results,"structure factor\nq\tS(q)");
  
}

/******************************************************************************/
/**
 * @fn void AnalyzerScattering<IngredientsType>::initQFactor( void )
 * @brief initialise set of q vectors in multiplicative manner
 */
template<class IngredientsType>
void AnalyzerScattering<IngredientsType>::initQFactor(){
  double k(std::exp(std::log((2*ingredients.getBoxX()))/num_of_q));
  double x(1.0);
  std::cout << "multiplicator for abs(q) k = " << k << std::endl;
  for(uint32_t i=0; i<num_of_q; i++){
    x*=k;
    q_factor.push_back(x);
  }
  if((q_factor.at(num_of_q-1) - (double)(2*ingredients.getBoxX())) > 0.00001)
    throw std::runtime_error("wrong q_vector array!");
}

/******************************************************************************/
/**
 * @fn void AnalyzerScattering<IngredientsType>::CalcScatteringAmplitude( void )
 * @brief adding a count per strand length interval 
 */
template<class IngredientsType>
void AnalyzerScattering<IngredientsType>::CalcScatteringAmplitude(){
  // loop over some randomly catched q vectors
  for(uint32_t i=0;i<num_of_q;i++){
    VectorDouble3 q(GetRandQVector());
    
    //loop over some different absolute values of q
    for(uint32_t j=0;j<q_factor.size();j++){
      // multiply q with a certain number to get equidistant points in log-log plot
      VectorDouble3 q_j=(((M_PI)/ingredients.getBoxX())*q_factor.at(j))*q;
      // scattering amplitude of C_q 
      std::complex<double> C_q(0.0,0.0);
      //loop over molecules
      for(uint32_t k=0;k<ingredients.getMolecules().size();k++){
	// "cast" position vector to double vector
	VectorDouble3 r (ingredients.getMolecules()[k]);
	//scalar product of q and monomer position
	double phi( (double)((-1)*(r*q_j)) );
	//"cast" to imaginary complex number 
	std::complex<double> iphi(0.0,phi);
	// sum over the squared abs of the exponential of iphi
	C_q+=std::exp(iphi);
      } /* end loop over molecule */
      // add C_q depending on q as part of the scattering function to the scattering function container
      averagedSquaredAbsC_q.at(j)+=((double)(std::real(C_q*std::conj(C_q))));
      
    }/* end loop over differnt absolute values of q */
  }/* end loop over random q vectors */
}

/******************************************************************************/
/**
 * @fn VectorDouble3 AnalyzerScattering<IngredientsType>::GetRandQVector()
 * @brief get a randomly orientated 3D Vector 
 * @return VectorDouble3 q, restricted to the unit sphere
 */
template<class IngredientsType>
VectorDouble3 AnalyzerScattering<IngredientsType>::GetRandQVector(){
  VectorDouble3 q(1.0,1.0,1.0);
  //get three random numbers in [-1,1] 
  double a((rng.r250_drand()*2)-1);
  double b((rng.r250_drand()*2)-1);
  double c((rng.r250_drand()*2)-1);
  //check them to be inside the unit sphere
  if((a*a+b*b+c*c)<=1){
    // if yes, set them as coordinates for q
    q.setX(a); q.setY(b); q.setZ(c);
  }else if((a*a+b*b+c*c)>1){
    // if not, do an iterative call of GetRandQVector to get another set of random coordinates
    q=GetRandQVector();
  }
  q.normalize();
  return q;
}

#endif /* ANALYZERSCATTERING_H */