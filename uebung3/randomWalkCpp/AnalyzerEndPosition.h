#ifndef LEMONADE_ANALYZERENDPOSITON
#define LEMONADE_ANALYZERENDPOSITON

#include <LeMonADE/analyzer/AbstractAnalyzer.h>
#include <math.h> // für die sqrt funktion

template <class IngredientsType> 
class AnalyzerEndPosition: public AbstractAnalyzer
{
public:
  //! constructor call handling CONST ingredients and the result file name
  AnalyzerEndPosition(const IngredientsType& ing, const std::string& filename_);
  //!Standard destructor closing the file stream.
  virtual ~AnalyzerEndPosition(){}
  
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
  std::vector<std::vector<int32_t> > results;
  //! make the histogram
  void getHistrogram();

};

/**
 *  constructor passing ingredients and filename and constructor call for result vector
 */
template <class IngredientsType>
AnalyzerEndPosition<IngredientsType>::AnalyzerEndPosition(const IngredientsType& ingredients_, const std::string& filename_):
    ingredients(ingredients_), filename(filename_), results(4,std::vector<int32_t>())
{}

/**
 *  initialize calling execute the very first time
 */
template <class IngredientsType>
void AnalyzerEndPosition<IngredientsType>::initialize(){
  std::cout <<"initialize AnalyzerEndPosition"<<std::endl;
  
  execute();
}

/**
 *  do the calculations each time the task manager calls run
 */
template <class IngredientsType>
bool AnalyzerEndPosition<IngredientsType>::execute(){
  
  results.at(0).push_back(ingredients.getMolecules().getAge());
  int32_t lastMonoIndex(ingredients.getMolecules().size()-1);
  results.at(1).push_back(ingredients.getMolecules()[lastMonoIndex].getX()-ingredients.getMolecules()[0].getX());
  results.at(2).push_back(ingredients.getMolecules()[lastMonoIndex].getY()-ingredients.getMolecules()[0].getY());
  results.at(3).push_back(ingredients.getMolecules()[lastMonoIndex].getZ()-ingredients.getMolecules()[0].getZ());

  return true;
}

/**
 *  write out results of Overlap Analyzer to file
 */
template <class IngredientsType>
void AnalyzerEndPosition<IngredientsType>::cleanup(){
  
  /* ############ add your ideas ############ */
  //getHistrogram(); // or just uncomment me
  
  
  //write out results
  ResultFormattingTools::writeResultFile(filename, ingredients, results,"End monomer positions\nframe\tx \ty \tz");
}

template <class IngredientsType>
void AnalyzerEndPosition<IngredientsType>::getHistrogram(){
  double binSize(2.0);
  /* one dimensional distribution */
  std::map<int32_t,double> Histo1D_x,Histo1D_y,Histo1D_z;
   std::vector< std::vector <double> > histogram_1dx(2, std::vector<double>());
   std::vector< std::vector <double> > histogram_1dy(2, std::vector<double>());
   std::vector< std::vector <double> > histogram_1dz(2, std::vector<double>());
  //fill Histogram
  for(uint32_t i=0;i<results.at(1).size();i++){
    Histo1D_x[(int)(results.at(1).at(i)/binSize)]++;
    Histo1D_y[(int)(results.at(2).at(i)/binSize)]++;
    Histo1D_z[(int)(results.at(3).at(i)/binSize)]++;
  }
  //normalize
  for(std::map<int32_t,double>::iterator it=Histo1D_x.begin();it!=Histo1D_x.end();++it){
    histogram_1dx.at(0).push_back(it->first*binSize);
    histogram_1dx.at(1).push_back(it->second/results.at(0).size());
  }
  for(std::map<int32_t,double>::iterator it=Histo1D_y.begin();it!=Histo1D_y.end();++it){
    histogram_1dy.at(0).push_back(it->first*binSize);
    histogram_1dy.at(1).push_back(it->second/results.at(0).size());
  }
  for(std::map<int32_t,double>::iterator it=Histo1D_z.begin();it!=Histo1D_z.end();++it){
    histogram_1dz.at(0).push_back(it->first*binSize);
    histogram_1dz.at(1).push_back(it->second/results.at(0).size());
  }
  
  std::size_t found = filename.find_last_of(".");
  ResultFormattingTools::writeResultFile((filename.substr(0,found)+"_histo1Dx.dat"), ingredients, histogram_1dx,"End position histogram 1D\nrx\tP(rx)");
  ResultFormattingTools::writeResultFile((filename.substr(0,found)+"_histo1Dy.dat"), ingredients, histogram_1dy,"End position histogram 1D\nrx\tP(rx)");
  ResultFormattingTools::writeResultFile((filename.substr(0,found)+"_histo1Dz.dat"), ingredients, histogram_1dz,"End position histogram 1D\nrx\tP(rx)");
 
  
  /* radial distribution in spherical coordiantes  */
  std::map<int32_t,double> HistoRadial;
  std::vector< std::vector <double> > histogram_radial(3, std::vector<double>());
  int32_t counter(0);
  for(uint32_t i=0;i<results.at(0).size();i++){
    double ree(
      results.at(1).at(i)*results.at(1).at(i)
      +results.at(2).at(i)*results.at(2).at(i)
      +results.at(3).at(i)*results.at(3).at(i)
	   );
    HistoRadial[(int)(sqrt(ree)/binSize)]++;
    
    counter++;
  }
  if(counter!=results.at(0).size())
    throw std::runtime_error("histogram error!");
  
  //normalize
  for(std::map<int32_t,double>::iterator it=HistoRadial.begin();it!=HistoRadial.end();++it){
    histogram_radial.at(0).push_back(it->first*binSize+(binSize/2.0));
    //normalize to number of frames
    it->second/=results.at(0).size();
    histogram_radial.at(1).push_back(it->second);
    //normalize to spherical coordinates
    it->second/=(4*3.1415926535*((it->first*binSize)*(it->first*binSize)*binSize));
    histogram_radial.at(2).push_back(it->second);
  }
  
  ResultFormattingTools::writeResultFile((filename.substr(0,found)+"_histoRadial.dat"), ingredients, histogram_radial,"End position histogram radial\nR\tP(R)\tP(R)4Pi/R^2dR");
  
}


#endif //LEMONADE_ANALYZERENDPOSITON
