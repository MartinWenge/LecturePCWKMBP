
// Definitionen der Ingredients Klasse und der ConfigureSystem Klasse
#include <LeMonADE/core/Ingredients.h>

// Featurepaket mit Feature Box (Box mit Randbedinungen), Feature Bondset (Bindungsvektoren des Bindungs-Fluktuations Modells) und FeatureMolecules (lese und Schreibroutinen für Anzahl Monomere, extra Bindungen und die Koordinaten in Komprimierter Form)
#include <LeMonADE/feature/FeatureMoleculesIO.h>

// Feature, dass an Monomere einen int hinzufügt. Zugriff über Ingredients: ingredients.getMolecules()[index].getAttributeTag() beziehungsweise ingredients.modifyMolecules()[index].setAttributeTag( eine Zahl )
#include <LeMonADE/feature/FeatureAttributes.h>

// TaskManager für Ananlyzer und Updater: stellt initialize(), run(nRuns=untilFalse) und cleanup() bereit, neue Objekte mit taskmanager.addAnalyzer(new AnalyzerConstruktor(ingredients,...)); oder taskmanager.addUpdater(new UpdaterConstruktor(ingredients, ...));
#include <LeMonADE/utility/TaskManager.h>

// Moves, die benutzt werden, um ein zufälliges Monomer einen Schritt entlang einer zufäälig gewählten Richtung zu bewegen (MoveLocalSc) oder ein bfm Monomer (Würfel auf dem Gitter) konfliktfrei mit allen Features einzufügen (MoveAddMonomerSc)
#include <LeMonADE/updater/moves/MoveLocalSc.h>
#include <LeMonADE/updater/moves/MoveAddMonomerSc.h>

// eure selbst geschriebenen Analyzer/Updater, die nicht im Hauptrepository liegen
#include "UpdaterRandomWalk.h"
#include "AnalyzerReeAndRg.h"
#include "AnalyzerEndPosition.h"

// read in command line options
#include <boost/program_options.hpp>
using namespace boost::program_options;

int main(int argc, char* argv[])
{
  /* +++++++++++++++++++++++++++++  read arguments  ++++++++++++++++++++++++++++ */
  // diese drei Variablen wollen wir über die Kommandozeile eingeben und den Analyzern/Updatern übergeben
  std::string filenameRadius, filenameDistribution;
  uint32_t numberOfSteps;
  
  // hier werden die boost funktionen zum Einlesen der Kommadozeilenoptionen benutzt
  options_description desc{"Allowed options"};
  desc.add_options()
    ("help,h", "help message")
    ("fileRad,r", value<std::string>(&filenameRadius)->default_value("reeAndrg.dat"), "radius filename")
    ("fileDist,d", value<std::string>(&filenameDistribution)->default_value("endDist.dat"), "distribution filename")
    ("nstep,n", value<uint32_t>(&numberOfSteps)->default_value(64), "number of steps");
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
  typedef LOKI_TYPELIST_2(FeatureMoleculesIO,FeatureAttributes) Features;
  
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
  
  /* +++++++++++++++++++++++++++  prepare ingredients  +++++++++++++++++++++++ */
  //Wenn man ein völlig leeres System benutzt und nicht schon fertige Konformationen einließt, müssen für manche Features (hier FeatureBox) Werte gesetzt werden, kann aber auch der Updater übernehmen, der das System erstellt. Ich habe es hier rein gemacht, damit man sieht, was man macht und wie die interfaces aussehen:
  ingredients.setBoxX(64);
  ingredients.setBoxY(64);
  ingredients.setBoxZ(64);
  ingredients.setPeriodicX(true);
  ingredients.setPeriodicY(true);
  ingredients.setPeriodicZ(true);
  ingredients.modifyBondset().addBFMclassicBondset();
  // synchronize legt bei FeatureBondset die Lookup Table für das Bondset an wobei überprüft wird, ob die existierenden Bindungsvektoren aller verbundenen Monomere teil des Bondsets sind und bei FeatureBox wird überprüft, ob alle Boxgrößen und Periodizitäten gesetzt sind
  ingredients.synchronize();
  
  /* ++++++++++++++++++++++++++++++  use TaskManager  +++++++++++++++++++++++++ */
  TaskManager taskManager;
  
  //Hier fügen wir den Random Walk Updater ein, der zuerst das "alte" molecules löscht und einen neuen random walk anlegt, bei dem hintereinander weg monomere an das letzte Monomer eingefügt werden
  taskManager.addUpdater(new UpdaterRandomWalk<IngredientsType>(ingredients,numberOfSteps));
  
  // dieser Updater berechnet die Verteilung der EndzuEnd Abstände in 1D und radial und gibt diese Informationen in jeweils einzelne Dateien aus, die alle mit dem filenameDistribution (ohne Dateiendung) anfangen
  taskManager.addAnalyzer(new AnalyzerEndPosition<IngredientsType>(ingredients,filenameDistribution));
  
  // dieser Analyzer berechnet für jede Konformation den Gyrationsradius und den End zu end Abstand und schreibt am Ende die Zeitreihen in eine Datei
  taskManager.addAnalyzer(new AnalyzerReeAndRg<IngredientsType>(ingredients,filenameRadius));
  
  // hier ruft der Taskmanager für alle zugefügten analyzer und Updater die initialize Funktion auf
  taskManager.initialize();
  
  // hier ruft der TaskManager hintereinader weg 200000 mal zuerst für alle Updater, danach für alle Analyzer die execute funktion auf. Hat man bei Hinzufügen der Analyzer/Updater eine add..(new Constuctor(...),0); null hinzugefügt, wird dieser Objekt nur einmal ausgeführt, was bei Simulatioenen mit einer neu erstellten Startkonformation nützlich ist
  taskManager.run(200000);
  
  // hier werden alle cleanup-Funktionen ausfgerufen
  taskManager.cleanup();
  
  //fertig
  return 0;
}
    
