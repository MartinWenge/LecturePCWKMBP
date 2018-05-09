#ifndef LEMONADE_ANALYZERBRUSHHIGHT
#define LEMONADE_ANALYZERBRUSHHIGHT

#include <LeMonADE/analyzer/AbstractAnalyzer.h>
#include <cmath>

template <class IngredientsType> 
class AnalyzerBrushHight: public AbstractAnalyzer
{
public:
  //! constructor call handling CONST ingredients and the result file name
  AnalyzerBrushHight(const IngredientsType& ing, const std::string& filename_);
  //!Standard destructor closing the file stream.
  virtual ~AnalyzerBrushHight(){}
  
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

  //! function to calculate brush hight by rg
  double calculateBrushRgZ();
  //! function to calculate brush hight by end monomer 
  double calculateBrushEndHight();
  
};

/**
 *  constructor passing ingredients and filename and constructor call for result vector
 */
template <class IngredientsType>
AnalyzerBrushHight<IngredientsType>::AnalyzerBrushHight(const IngredientsType& ingredients_, const std::string& filename_):
    ingredients(ingredients_), filename(filename_), results(3,std::vector<double>())
{}

/**
 *  initialize calling execute the very first time
 */
template <class IngredientsType>
void AnalyzerBrushHight<IngredientsType>::initialize(){
  std::cout <<"initialize AnalyzerBrushHight"<<std::endl;
  
  execute();
}

/**
 *  do the calculations each time the task manager calls run
 */
template <class IngredientsType>
bool AnalyzerBrushHight<IngredientsType>::execute(){
  
  results.at(0).push_back(ingredients.getMolecules().getAge());
  results.at(1).push_back(calculateBrushEndHight());
  results.at(2).push_back(calculateBrushRgZ());
  return true;
}

/**
 *  write out results of Overlap Analyzer to file
 */
template <class IngredientsType>
void AnalyzerBrushHight<IngredientsType>::cleanup(){
  //write out results
  ResultFormattingTools::writeResultFile(filename, ingredients, results,"Brush hight by Ree and Rg\nframe\tH_ee \tH_Rg");
}

//! calculate end to end distance
template <class IngredientsType>
double AnalyzerBrushHight<IngredientsType>::calculateBrushEndHight(){
  double hight(0.0);
  int32_t nchains(0);
  for(uint32_t i=0;i<ingredients.getMolecules().size();i++){
    if( ( ingredients.getMolecules().getNumLinks(i)==1 ) && ( ingredients.getMolecules()[i].getMovableTag() ) ){
      hight+=ingredients.getMolecules()[i].getZ();
      nchains++;
    }
  }
  return hight/nchains;
}

// calculate squared radius of gyration
template <class IngredientsType>
double AnalyzerBrushHight<IngredientsType>::calculateBrushRgZ(){
  double CoM(0.0);
  
  //get center of mass
  for(uint32_t i=0;i<ingredients.getMolecules().size();i++){
    CoM+=ingredients.getMolecules()[i].getZ();
  }
  CoM/=ingredients.getMolecules().size();
  
  double z(0);
  for(uint32_t i=0;i<ingredients.getMolecules().size();i++){
    z+=(ingredients.getMolecules()[i].getZ()-CoM)*(ingredients.getMolecules()[i].getZ()-CoM);
  }
  
  return std::sqrt(z/(ingredients.getMolecules().size()));
}

#endif //LEMONADE_ANALYZERBRUSHHIGHT
