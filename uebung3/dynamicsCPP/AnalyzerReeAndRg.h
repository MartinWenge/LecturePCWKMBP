#ifndef LEMONADE_ANALYZERREEANDRG
#define LEMONADE_ANALYZERREEANDRG

#include <LeMonADE/analyzer/AbstractAnalyzer.h>

template <class IngredientsType> 
class AnalyzerReeAndRg: public AbstractAnalyzer
{
public:
  //! constructor call handling CONST ingredients and the result file name
  AnalyzerReeAndRg(const IngredientsType& ing, const std::string& filename_);
  //!Standard destructor closing the file stream.
  virtual ~AnalyzerReeAndRg(){}
  
  //! public function used by taskManager: initialize
  virtual void initialize();
  //! public functions used by taskManager: run
  virtual bool execute();
  //! public functions used by taskManager: cleanup
  virtual void cleanup();

  
private:
  //! Storage for data that are processed to file (mostly Ingredients).
  const IngredientsType& ingredients;
  //! The filename to store the results in
  std::string filename;
  //! result storage
  std::vector<std::vector<double> > results;

  //! function to calculate end to end vector
  double calculateRee();
  //! function to calculate radius of gyration
  double calculateRg();
  
};

/**
 *  constructor passing ingredients and filename and constructor call for result vector
 */
template <class IngredientsType>
AnalyzerReeAndRg<IngredientsType>::AnalyzerReeAndRg(const IngredientsType& ingredients_, const std::string& filename_):
    ingredients(ingredients_), filename(filename_), results(3,std::vector<double>())
{}

/**
 *  initialize calling execute the very first time
 */
template <class IngredientsType>
void AnalyzerReeAndRg<IngredientsType>::initialize(){
  std::cout <<"initialize AnalyzerReeAndRg"<<std::endl;
  
  execute();
}

/**
 *  do the calculations each time the task manager calls run
 */
template <class IngredientsType>
bool AnalyzerReeAndRg<IngredientsType>::execute(){
  
  results.at(0).push_back(ingredients.getMolecules().getAge());
  results.at(1).push_back(calculateRee());
  results.at(2).push_back(calculateRg());

  return true;
}

/**
 *  write out results of Overlap Analyzer to file
 */
template <class IngredientsType>
void AnalyzerReeAndRg<IngredientsType>::cleanup(){
  //write out results
  ResultFormattingTools::writeResultFile(filename, ingredients, results,"R^2 gyration and R^2 end to end\nframe\tR_ee \tR_G");
}

//! calculate end to end distance
template <class IngredientsType>
double AnalyzerReeAndRg<IngredientsType>::calculateRee(){
  VectorDouble3 Ree(ingredients.getMolecules()[0]-ingredients.getMolecules()[ingredients.getMolecules().size()-1]);
  return Ree*Ree;
}

// calculate squared radius of gyration
template <class IngredientsType>
double AnalyzerReeAndRg<IngredientsType>::calculateRg(){
  VectorDouble3 CoM(0,0,0);
  
  //get center of mass
  for(uint32_t i=0;i<ingredients.getMolecules().size();i++){
    CoM+=ingredients.getMolecules()[i];
  }
  CoM/=ingredients.getMolecules().size();
  
  double x(0),y(0),z(0);
  for(uint32_t i=0;i<ingredients.getMolecules().size();i++){
    x+=(ingredients.getMolecules()[i].getX()-CoM.getX())*(ingredients.getMolecules()[i].getX()-CoM.getX());
    y+=(ingredients.getMolecules()[i].getY()-CoM.getY())*(ingredients.getMolecules()[i].getY()-CoM.getY());
    z+=(ingredients.getMolecules()[i].getZ()-CoM.getZ())*(ingredients.getMolecules()[i].getZ()-CoM.getZ());
  }
  
  return (x+y+z)/ingredients.getMolecules().size();
}

#endif //LEMONADE_ANALYZERREEANDRG
