#include <LeMonADE/core/Ingredients.h>
#include <LeMonADE/feature/FeatureMoleculesIO.h>
#include <LeMonADE/feature/FeatureAttributes.h>
#include <LeMonADE/feature/FeatureExcludedVolumeSc.h>
#include <LeMonADE/feature/FeatureFixedMonomers.h>
#include <LeMonADE/utility/TaskManager.h>
#include <LeMonADE/updater/UpdaterReadBfmFile.h>
#include <LeMonADE/updater/UpdaterSimpleSimulator.h>
#include <LeMonADE/analyzer/AnalyzerWriteBfmFile.h>

#include <boost/program_options.hpp>
using namespace boost::program_options;

int main(int argc, char* argv[])
{
  /* +++++++++++++++++++++++++++++  read arguments  ++++++++++++++++++++++++++++ */
  std::string inFileName, outFileName;
  uint32_t nMCS, nSave;
  
  options_description desc{"Allowed options"};
  desc.add_options()
    ("help,h", "help message")
    ("ifile,i", value<std::string>(&inFileName)->default_value("brushConfig.bfm"), "input filename")
    ("ofile,o", value<std::string>(&outFileName)->default_value("brushConfig_sim.dat"), "output filename")
    ("nmcs,n", value<uint32_t>(&nMCS)->default_value(64), "total number of mcs")
    ("nsave,s", value<uint32_t>(&nSave)->default_value(64), "save interval in mcs");
    
  variables_map options_map;
  store(parse_command_line(argc, argv, desc), options_map);
  notify(options_map);
  
  if (options_map.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }
  
  /* +++++++++++++++++++++++++  setup system  +++++++++++++++++++++++++++++++ */
  typedef LOKI_TYPELIST_4(FeatureMoleculesIO,FeatureAttributes,FeatureFixedMonomers,FeatureExcludedVolumeSc<FeatureLattice<bool> >) Features;
  typedef ConfigureSystem<VectorInt3,Features,4> Config;
  typedef Ingredients<Config> IngredientsType;
  
  IngredientsType ingredients;
  
    /* ++++++++++++++++++++++++++++++  use TaskManager  +++++++++++++++++++++++++ */
  TaskManager taskManager;
  
  taskManager.addUpdater(new UpdaterReadBfmFile<IngredientsType>(inFileName,ingredients,UpdaterReadBfmFile<IngredientsType>::READ_LAST_CONFIG_SAVE),0);
  taskManager.addUpdater(new UpdaterSimpleSimulator<IngredientsType,MoveLocalSc>(ingredients,nSave));
  taskManager.addAnalyzer(new AnalyzerWriteBfmFile<IngredientsType>(outFileName,ingredients,AnalyzerWriteBfmFile<IngredientsType>::APPEND));
  
  taskManager.initialize();
  taskManager.run(nMCS/nSave);
  taskManager.cleanup();
  
  return 0;
}