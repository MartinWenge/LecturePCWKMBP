#include <LeMonADE/core/Ingredients.h>
#include <LeMonADE/feature/FeatureMoleculesIO.h>
#include <LeMonADE/feature/FeatureAttributes.h>
#include <LeMonADE/feature/FeatureExcludedVolumeSc.h>
#include <LeMonADE/utility/TaskManager.h>

#include <LeMonADE/analyzer/AnalyzerWriteBfmFile.h>

// personal Updater/Analyzer
#include "UpdaterSAW.h"
#include "AnalyzerReeAndRg.h"

// read in command line options
#include <boost/program_options.hpp>
using namespace boost::program_options;

int main(int argc, char* argv[])
{
  /* +++++++++++++++++++++++++++++  read arguments  ++++++++++++++++++++++++++++ */
  std::string filenameRadius, ofilename;
  uint32_t numberOfSteps;
  
  options_description desc{"Allowed options"};
  desc.add_options()
    ("help,h", "help message")
    ("fileConfig,o", value<std::string>(&ofilename)->default_value("config.dat"), "config output filename")
    ("fileRad,r", value<std::string>(&filenameRadius)->default_value("reeAndrg.dat"), "radius filename")
    ("nstep,n", value<uint32_t>(&numberOfSteps)->default_value(64), "number of steps");
  
  variables_map options_map;
  store(parse_command_line(argc, argv, desc), options_map);
  notify(options_map);
  
  // help option
  if (options_map.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }
  
  /* +++++++++++++++++++++++++  setup system  +++++++++++++++++++++++++++++++ */
  //define feature list
  typedef LOKI_TYPELIST_3(FeatureMoleculesIO,FeatureExcludedVolumeSc<>,FeatureAttributes<>) Features;
  //define ingredients type (3D Vektor, oben genannte features, maximale anzahl Bindungspartner)
  typedef ConfigureSystem<VectorInt3,Features,4> Config;
  //define the ingredients type with the configuration above
  typedef Ingredients<Config> IngredientsType;
  
  IngredientsType ingredients;
  
  /* ++++++++++++++++++++  set up random number generator  +++++++++++++++++++ */
  RandomNumberGenerators rng;
  rng.seedAll();
  
  /* +++++++++++++++++++++++++++  prepare ingredients  +++++++++++++++++++++++ */
  ingredients.setBoxX(128);
  ingredients.setBoxY(128);
  ingredients.setBoxZ(128);
  ingredients.setPeriodicX(true);
  ingredients.setPeriodicY(true);
  ingredients.setPeriodicZ(true);
  ingredients.modifyBondset().addBFMclassicBondset();
  ingredients.synchronize();
  
  /* ++++++++++++++++++++++++++++++  use TaskManager  +++++++++++++++++++++++++ */
  TaskManager taskManager;
  taskManager.addUpdater(new UpdaterSelfAvoidingWalk<IngredientsType>(ingredients,numberOfSteps));
  taskManager.addAnalyzer(new AnalyzerReeAndRg<IngredientsType>(ingredients,filenameRadius));
  taskManager.addAnalyzer(new AnalyzerWriteBfmFile<IngredientsType>(ofilename,ingredients,AnalyzerWriteBfmFile<IngredientsType>::APPEND));
  
  taskManager.initialize();
  taskManager.run(100);
  taskManager.cleanup();
  
  return 0;
}
    
