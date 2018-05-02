#include <LeMonADE/core/Ingredients.h>
#include <LeMonADE/feature/FeatureMoleculesIO.h>
#include <LeMonADE/feature/FeatureAttributes.h>
#include <LeMonADE/utility/TaskManager.h>
#include <LeMonADE/updater/moves/MoveLocalSc.h>
#include <LeMonADE/updater/moves/MoveAddMonomerSc.h>

// personal Updater/Analyzer
#include "UpdaterRandomWalk.h"
#include "AnalyzerReeAndRg.h"
#include "AnalyzerEndPosition.h"

// read in command line options
#include <boost/program_options.hpp>
using namespace boost::program_options;

int main(int argc, char* argv[])
{
  /* +++++++++++++++++++++++++++++  read arguments  ++++++++++++++++++++++++++++ */
  std::string filenameRadius, filenameDistribution;
  uint32_t numberOfSteps;
  
  options_description desc{"Allowed options"};
  desc.add_options()
    ("help,h", "help message")
    ("fileRad,r", value<std::string>(&filenameRadius)->default_value("reeAndrg.dat"), "radius filename")
    ("fileDist,d", value<std::string>(&filenameDistribution)->default_value("endDist.dat"), "distribution filename")
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
  typedef LOKI_TYPELIST_2(FeatureMoleculesIO,FeatureAttributes) Features;
  //define ingredients type (3D Vektor, oben genannte features, maximale anzahl Bindungspartner)
  typedef ConfigureSystem<VectorInt3,Features,4> Config;
  //define the ingredients type with the configuration above
  typedef Ingredients<Config> IngredientsType;
  
  IngredientsType ingredients;
  
  /* ++++++++++++++++++++  set up random number generator  +++++++++++++++++++ */
  RandomNumberGenerators rng;
  rng.seedAll();
  
  /* +++++++++++++++++++++++++++  prepare ingredients  +++++++++++++++++++++++ */
  ingredients.setBoxX(64);
  ingredients.setBoxY(64);
  ingredients.setBoxZ(64);
  ingredients.setPeriodicX(true);
  ingredients.setPeriodicY(true);
  ingredients.setPeriodicZ(true);
  ingredients.modifyBondset().addBFMclassicBondset();
  ingredients.synchronize();
  
  /* ++++++++++++++++++++++++++++++  use TaskManager  +++++++++++++++++++++++++ */
  TaskManager taskManager;
  taskManager.addUpdater(new UpdaterRandomWalk<IngredientsType>(ingredients,numberOfSteps));
  taskManager.addAnalyzer(new AnalyzerEndPosition<IngredientsType>(ingredients,filenameDistribution));
  taskManager.addAnalyzer(new AnalyzerReeAndRg<IngredientsType>(ingredients,filenameRadius));
  
  taskManager.initialize();
  taskManager.run(200000);
  taskManager.cleanup();
  
  return 0;
}
    
