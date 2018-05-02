#ifndef LEMONADE_ANALYZERAUTOCORRELATION_H
#define LEMONADE_ANALYZERAUTOCORRELATION_H

#include <LeMonADE/analyzer/AbstractAnalyzer.h>
#include <math.h> // für die ceil funktion (aufrunden)

template <class IngredientsType> 
class AnalyzerAutoCorrelation: public AbstractAnalyzer
{
public:
  //! constructor call handling CONST ingredients and the result file name
  AnalyzerAutoCorrelation(const IngredientsType& ing, const std::string& filename_, const uint32_t timeStep_);
  //!Standard destructor closing the file stream.
  virtual ~AnalyzerAutoCorrelation(){}
  
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
  //! time step delta_t
  const uint32_t timeStep;
  //! Ree timeline storage
  std::vector<VectorDouble3> reeTimeline;
  //! make the histogram
  void getCorrelations();

};

/**
 *  constructor passing ingredients and filename and constructor call for result vector
 */
template <class IngredientsType>
AnalyzerAutoCorrelation<IngredientsType>::AnalyzerAutoCorrelation(const IngredientsType& ingredients_, const std::string& filename_, const uint32_t timeStep_):
    ingredients(ingredients_), filename(filename_), timeStep(timeStep_), reeTimeline(1,VectorDouble3())
{}

/**
 *  initialize calling execute the very first time
 */
template <class IngredientsType>
void AnalyzerAutoCorrelation<IngredientsType>::initialize(){
  std::cout <<"initialize AnalyzerAutoCorrelation"<<std::endl;
  
  execute();
}

/**
 *  do the calculations each time the task manager calls run
 */
template <class IngredientsType>
bool AnalyzerAutoCorrelation<IngredientsType>::execute(){

  reeTimeline.push_back(ingredients.getMolecules()[0]-ingredients.getMolecules()[ingredients.getMolecules().size()-1]);

  return true;
}

/**
 *  write out results of Overlap Analyzer to file
 */
template <class IngredientsType>
void AnalyzerAutoCorrelation<IngredientsType>::cleanup(){
  //calculate the correlations and write them out
  getCorrelations();
}

template <class IngredientsType>
void AnalyzerAutoCorrelation<IngredientsType>::getCorrelations(){
   std::vector< std::vector <double> > correlations(2, std::vector<double>());
   
   // do the correltation
   for(uint32_t delta_t=0;delta_t<(reeTimeline.size()-1);delta_t++){
     double numerator(0.0);
     double denominator(0.0);
     for(uint32_t t=0;t<(reeTimeline.size()-delta_t);t++){
	numerator+=(reeTimeline.at(t+delta_t)*reeTimeline.at(t));
	denominator+=(reeTimeline.at(t)*reeTimeline.at(t));
     }
     correlations.at(0).push_back(delta_t*timeStep);
     correlations.at(1).push_back(numerator/denominator);
   }
   
  
  //std::size_t found = filename.find_last_of(".");
  ResultFormattingTools::writeResultFile(filename, ingredients, correlations,"End position histogram radial\nR\tP(R)\tP(R)4Pi/R^2dR");
  
}


#endif //LEMONADE_ANALYZERAUTOCORRELATION_H
