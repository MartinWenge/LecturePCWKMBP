#ifndef LEMONADE_ANALYZERREEANDRG
#define LEMONADE_ANALYZERREEANDRG

#include <LeMonADE/analyzer/AbstractAnalyzer.h>

// Analyzer required FeatureAdsorption

template <class IngredientsType> 
class AnalyzerAdsorption: public AbstractAnalyzer
{
public:
  //! constructor call handling CONST ingredients and the result file name
  AnalyzerAdsorption(const IngredientsType& ing, const std::string& filename_);
  //!Standard destructor closing the file stream.
  virtual ~AnalyzerAdsorption(){}
  
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
  VectorInt3 calculateAdsorbedMonomers();
};

/**
 *  constructor passing ingredients and filename and constructor call for result vector
 */
template <class IngredientsType>
AnalyzerAdsorption<IngredientsType>::AnalyzerAdsorption(const IngredientsType& ingredients_, const std::string& filename_):
    ingredients(ingredients_), filename(filename_), results(7,std::vector<double>())
{}

/**
 *  initialize calling execute the very first time
 */
template <class IngredientsType>
void AnalyzerAdsorption<IngredientsType>::initialize(){
  std::cout <<"initialize AnalyzerAdsorption"<<std::endl;
  
  execute();
}

/**
 *  do the calculations each time the task manager calls run
 */
template <class IngredientsType>
bool AnalyzerAdsorption<IngredientsType>::execute(){
  
  results.at(0).push_back(ingredients.getMolecules().getAge());
  
  VectorInt3 dummy(calculateAdsorbedMonomers());
  results.at(1).push_back(dummy.getX());
  results.at(2).push_back(dummy.getX()*ingredients.getAdsorptionEnergyX());
  
  results.at(3).push_back(dummy.getY());
  results.at(4).push_back(dummy.getY()*ingredients.getAdsorptionEnergyY());
  
  results.at(5).push_back(dummy.getZ());
  results.at(6).push_back(dummy.getZ()*ingredients.getAdsorptionEnergyZ());
  
  return true;
}

/**
 *  write out results of Overlap Analyzer to file
 */
template <class IngredientsType>
void AnalyzerAdsorption<IngredientsType>::cleanup(){
  //write out results
  ResultFormattingTools::writeResultFile(filename, ingredients, results,"Adsorption energy\nframe\tNx\tEx\tNy\tEy\tNz\tEz");
}

//! calculate end to end distance
template <class IngredientsType>
VectorInt3 AnalyzerAdsorption<IngredientsType>::calculateAdsorbedMonomers(){
  VectorInt3 counter(0,0,0);
  
  for(uint32_t i=0;i<ingredients.getMolecules().size();i++){
    VectorInt3 pos(ingredients.getMolecules()[i]);
    if( (pos.getX()==0) || (pos.getX()==ingredients.getBoxX()-2) )
      counter[0]++;
    if( (pos.getY()==0) || (pos.getY()==ingredients.getBoxY()-2) )
      counter[1]++;
    if( (pos.getZ()==0) || (pos.getZ()==ingredients.getBoxZ()-2) )
      counter[2]++;
  }
  
  return counter;
}

#endif //LEMONADE_ANALYZERREEANDRG
