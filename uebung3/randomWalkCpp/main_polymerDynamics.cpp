#include <LeMonADE/core/Ingredients.h>
#include <LeMonADE/feature/FeatureMoleculesIO.h>
#include <LeMonADE/feature/FeatureAttributes.h>
#include <LeMonADE/utility/TaskManager.h>
#include <LeMonADE/updater/moves/MoveLocalSc.h>
#include <LeMonADE/updater/moves/MoveAddMonomerSc.h>
#include <LeMonADE/updater/UpdaterSimpleSimulator.h>
#include <LeMonADE/analyzer/AnalyzerWriteBfmFile.h>

// personal Updater/Analyzer
#include "UpdaterRandomWalk.h"
#include "AnalyzerReeAndRg.h"
#include "AnalyzerAutoCorrelation.h"

// read in command line options
#include <boost/program_options.hpp>
using namespace boost::program_options;

int main(int argc, char* argv[])
{
  /* +++++++++++++++++++++++++++++  read arguments  ++++++++++++++++++++++++++++ */
  std::string fileconfig, fileReeRg, fileCorr;
  uint32_t numberOfSteps;
  uint32_t timeStep;
  
  options_description desc{"Allowed options"};
  desc.add_options()
    ("help,h", "help message")
    ("filename,f", value<std::string>(&fileconfig)->default_value("config.bfm"), "conformation file")
    ("filenRad,r", value<std::string>(&fileReeRg)->default_value("reeAndRg.dat"), "radius file")
    ("filenCor,c", value<std::string>(&fileCorr)->default_value("correlat.dat"), "radius file")
    ("nstep,n", value<uint32_t>(&numberOfSteps)->default_value(512), "number of walk steps")
    ("timeSteps,t", value<uint32_t>(&timeStep)->default_value(100), "number of time steps");
  
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
  
  /* ++++++++++++++++++++  prepare ingredients  +++++++++++++++++++ */
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
  taskManager.addUpdater(new UpdaterRandomWalk<IngredientsType>(ingredients,numberOfSteps),0);
  taskManager.addUpdater(new UpdaterSimpleSimulator<IngredientsType,MoveLocalSc>(ingredients,timeStep));
  taskManager.addAnalyzer(new AnalyzerReeAndRg<IngredientsType>(ingredients,fileReeRg));
  taskManager.addAnalyzer(new AnalyzerAutoCorrelation<IngredientsType>(ingredients,fileCorr,timeStep));
  taskManager.addAnalyzer(new AnalyzerWriteBfmFile<IngredientsType>(fileconfig,ingredients));
  
  taskManager.initialize();
  taskManager.run(2000);
  taskManager.cleanup();
  
  return 0;
}
    
