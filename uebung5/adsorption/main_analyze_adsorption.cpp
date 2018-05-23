// Definitionen der Ingredients Klasse und der ConfigureSystem Klasse
#include <LeMonADE/core/Ingredients.h>

// Featurepaket mit Feature Box (Box mit Randbedinungen), Feature Bondset (Bindungsvektoren des Bindungs-Fluktuations Modells) und FeatureMolecules (lese und Schreibroutinen für Anzahl Monomere, extra Bindungen und die Koordinaten in Komprimierter Form)
#include <LeMonADE/feature/FeatureMoleculesIO.h>

// Feature für Adsorbierende Box
// #include <LeMonADE/feature/FeatureAdsorption.h> 
#include "FeatureAdsorption.h"

// TaskManager für Ananlyzer und Updater: stellt initialize(), run(nRuns=untilFalse) und cleanup() bereit, neue Objekte mit taskmanager.addAnalyzer(new AnalyzerConstruktor(ingredients,...)); oder taskmanager.addUpdater(new UpdaterConstruktor(ingredients, ...));
#include <LeMonADE/utility/TaskManager.h>

// Updater zum einlesen der Conformationsdateien
// taskmanager.addUpdater(new UpdaterReadBFMFile ( filename, ingredients, read type)); es gibt READ_LAST_CONFIG_SAVE, READ_LAST_CONFIG_FAST, READ_STEPWISE und wir benutzen READ_STEPWISE, weil wir alle conformationen auswerten möchten
#include <LeMonADE/updater/UpdaterReadBfmFile.h>

// unser Updater für Ree und Rg
#include "AnalyzerAdsorption.h"

// read in command line options
#include <boost/program_options.hpp>
using namespace boost::program_options;

int main(int argc, char* argv[])
{
  /* +++++++++++++++++++++++++++++  read arguments  ++++++++++++++++++++++++++++ */
  // diese Variablen wollen wir über die Kommandozeile eingeben und den Analyzern/Updatern übergeben
  std::string inFileName, outFileName;
  
  // hier werden die boost funktionen zum Einlesen der Kommadozeilenoptionen benutzt
  options_description desc{"Allowed options"};
  desc.add_options()
    ("help,h", "help message")
    ("ifile,i", value<std::string>(&inFileName)->default_value("chainConfig.dat"), "input filename")
    ("ofile,o", value<std::string>(&outFileName)->default_value("reeAndrg.dat"), "result filename");
  //neue Einlesevariablen hier vor dem ; hinzufügen über ("langname, kurzform(ein Buchstabe)",value<Typ der Variable>(&zuzuweisende Variable)->default_value(standardwert), "Beschreibung")
    
  variables_map options_map;
  store(parse_command_line(argc, argv, desc), options_map);
  notify(options_map);
  
  // falls -h oder --help benutzt wird, wird die Beschreibung ausgegeben und das Programm hier beendet
  if (options_map.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }
  
  /* +++++++++++++++++++++++++  setup system  +++++++++++++++++++++++++++++++ */
  //zuerst definieren wir die Feature Liste, hier nur zwei, sonst für X Features LOKI_TYPELIST_X(1, 2, ..., X) einfügen. mögliche Features: FeatureExcludedVolume, FeatureBoltzmann, FeatureWall, FeatureNNInteractions
  typedef LOKI_TYPELIST_2(FeatureMoleculesIO,FeatureAdsorption) Features;
  
  //dann definieren wir unseren Molecules Type mit den benutzen Koordinaten, den entsprechenden Monomer Erweiterungen der Features und der maximalen Anzahl Bindungen pro Monomer
  typedef ConfigureSystem<VectorInt3,Features,4> Config;
  
  // jetzt nehmen wir diese Klasse um ingridents zu definieren, das uns alle interfaces bereit stellt wie getMolecules(), modifyMolecules(), getName(), setName(default ist "new_lemonade"), addComment(string ...), synchronize()
  typedef Ingredients<Config> IngredientsType;
  
  //von dieser Klasse legen wir hier eine Instanz an, die wir geistreicher weise ingredients nennen
  IngredientsType ingredients;
  
  /* ++++++++++++++++++++  set up random number generator  +++++++++++++++++++ */
  //es emphielt sich den Zufallszahlengenerator zu seeden, sollte man das nicht machen wird es beim ersten anwenden der jeweiligen Instanz gemacht, allerdings ist es damit dann unmöglich einen bestimmten Seed zu setzten und eine reproduzierbare Simulation zu machen, darum steht das hier im Hauptprogramm
  RandomNumberGenerators rng;
  rng.seedAll();
  
  /* ++++++++++++++++++++++++++++++  use TaskManager  +++++++++++++++++++++++++ */
  TaskManager taskManager;
  
  //hier benutzen wir den Updater zum Ketten hinzufügen. Wir wollen nur eine Kette (zweiter Parameter=1) der Länge chainLength und es soll nur einmal aufgerufen werden ( 0 hinter dem Konstruktor)
  taskManager.addUpdater(new UpdaterReadBfmFile<IngredientsType>(inFileName,ingredients,UpdaterReadBfmFile<IngredientsType>::READ_STEPWISE));
  
  // unser Ree und Rg Analyzer
  taskManager.addAnalyzer(new AnalyzerAdsorption<IngredientsType>(ingredients,outFileName));
  
  // hier ruft der Taskmanager für alle zugefügten analyzer und Updater die initialize Funktion auf
  taskManager.initialize();
  taskManager.run();
  
  // hier werden alle cleanup-Funktionen ausfgerufen
  taskManager.cleanup();
  
  //fertig
  return 0;
}
    
