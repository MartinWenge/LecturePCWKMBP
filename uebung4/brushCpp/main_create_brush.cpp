// Definitionen der Ingredients Klasse und der ConfigureSystem Klasse
#include <LeMonADE/core/Ingredients.h>

// Featurepaket mit Feature Box (Box mit Randbedinungen), Feature Bondset (Bindungsvektoren des Bindungs-Fluktuations Modells) und FeatureMolecules (lese und Schreibroutinen für Anzahl Monomere, extra Bindungen und die Koordinaten in Komprimierter Form)
#include <LeMonADE/feature/FeatureMoleculesIO.h>

// Feature, dass an Monomere einen int hinzufügt. Zugriff über Ingredients: ingredients.getMolecules()[index].getAttributeTag() beziehungsweise ingredients.modifyMolecules()[index].setAttributeTag( eine Zahl )
#include <LeMonADE/feature/FeatureAttributes.h>

// Feature, dass excluded volume Wechselwirkungen implementiert. Dafür wird ein Gitter (mit dem entsprechenden Typ) belegt und bei jedem Move überprüft, ob die neue Position frei ist. Alle Funktionalitäten werden von synchronize, check und apply Move übernommen
// das Gitter ist ein Template, es gibt zwei Gitterarten (variable Größe und Zweierpotenzen als Größe) und die Gitter selbst sind Templateklassen, bei denen man entscheiden kann, welcher Typ auf das Gitter geschrieben wird, also ein bool, eine Zahl, ein String oder eine eigene Klasse. Wir benutzten dann einen bool -> frei(0) oder belegt(1)
#include <LeMonADE/feature/FeatureExcludedVolumeSc.h> 

// Feature, dass monere fixierbar macht
#include <LeMonADE/feature/FeatureFixedMonomers.h>

// TaskManager für Ananlyzer und Updater: stellt initialize(), run(nRuns=untilFalse) und cleanup() bereit, neue Objekte mit taskmanager.addAnalyzer(new AnalyzerConstruktor(ingredients,...)); oder taskmanager.addUpdater(new UpdaterConstruktor(ingredients, ...));
#include <LeMonADE/utility/TaskManager.h>

// Moves, die benutzt werden, um ein zufälliges Monomer einen Schritt entlang einer zufäälig gewählten Richtung zu bewegen (MoveLocalSc) oder ein bfm Monomer (Würfel auf dem Gitter) konfliktfrei mit allen Features einzufügen (MoveAddMonomerSc)
#include <LeMonADE/updater/moves/MoveLocalSc.h>
#include <LeMonADE/updater/moves/MoveAddMonomerSc.h>

// vorbereiteter Updater der wie folgt benutzt werden kann:
//taskmanager.addUpdater(new UpdaterAddGraftedChains( ingredients, ANZAHL KETTEN, ANZAHL MONOMERE PRO KETTE ) );
#include "UpdaterAddGraftedChains.h"

// Analyzer zum rausschreiben der Konformationen
// taskmanager.addAnalyzer(new AnalyzerWriteBfmFile( filenname, ingredients, write type (wir möchten ein neues file schreiben, also AnalyzerWriteBfmFile::NEWFILE, es gibt noch APPEND und OVERWRITE
#include <LeMonADE/analyzer/AnalyzerWriteBfmFile.h>

// read in command line options
#include <boost/program_options.hpp>
using namespace boost::program_options;

int main(int argc, char* argv[])
{
  /* +++++++++++++++++++++++++++++  read arguments  ++++++++++++++++++++++++++++ */
  // diese Variablen wollen wir über die Kommandozeile eingeben und den Analyzern/Updatern übergeben
  std::string fileName;
  uint32_t chainLength, numChains;
  uint32_t boxX, boxY, boxZ;
  
  // hier werden die boost funktionen zum Einlesen der Kommadozeilenoptionen benutzt
  options_description desc{"Allowed options"};
  desc.add_options()
    ("help,h", "help message")
    ("filename,f", value<std::string>(&fileName)->default_value("brushConfig.bfm"), "output filename")
    ("chainlength,n", value<uint32_t>(&chainLength)->default_value(32), "number of monomers in chain")
    ("nChains,m", value<uint32_t>(&numChains)->default_value(1), "number of chains in box")
    ("boxX,x", value<uint32_t>(&boxX)->default_value(64), "boxSize in x-direction")
    ("boxY,y", value<uint32_t>(&boxY)->default_value(64), "boxSize in y-direction")
    ("boxZ,z", value<uint32_t>(&boxZ)->default_value(64), "boxSize in z-direction the NONPERIODIC grafting surface");
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
  // hier entscheiden wir, ob wir eine ideale oder eine reale Kette simulieren möchten
  typedef LOKI_TYPELIST_4(FeatureMoleculesIO,FeatureFixedMonomers,FeatureAttributes,FeatureExcludedVolumeSc<FeatureLattice<bool> >) Features;

  
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
  ingredients.setBoxX(boxX);
  ingredients.setBoxY(boxY);
  ingredients.setBoxZ(boxZ);
  ingredients.setPeriodicX(true);
  ingredients.setPeriodicY(true);
  ingredients.setPeriodicZ(false);
  ingredients.modifyBondset().addBFMclassicBondset();
  // synchronize legt bei FeatureBondset die Lookup Table für das Bondset an wobei überprüft wird, ob die existierenden Bindungsvektoren aller verbundenen Monomere teil des Bondsets sind und bei FeatureBox wird überprüft, ob alle Boxgrößen und Periodizitäten gesetzt sind und beim Feature Excluded Volume wird das Gitter initial belegt
  ingredients.synchronize();
  
  /* ++++++++++++++++++++++++++++++  use TaskManager  +++++++++++++++++++++++++ */
  TaskManager taskManager;
  
  //hier benutzen wir den Updater zum Ketten hinzufügen. Wir wollen nur eine Kette (zweiter Parameter=1) der Länge chainLength und es soll nur einmal aufgerufen werden ( 0 hinter dem Konstruktor)
  taskManager.addUpdater(new UpdaterAddGraftedChains<IngredientsType>(ingredients,numChains,chainLength), 0);
  
  // hinzufügen des Schreibtools 
  taskManager.addAnalyzer(new AnalyzerWriteBfmFile<IngredientsType>(fileName,ingredients,AnalyzerWriteBfmFile<IngredientsType>::NEWFILE));
  
  // hier ruft der Taskmanager für alle zugefügten analyzer und Updater die initialize Funktion auf
  taskManager.initialize();
  taskManager.run();
  
  // hier werden alle cleanup-Funktionen ausfgerufen
  taskManager.cleanup();
  
  //fertig
  return 0;
}
    
