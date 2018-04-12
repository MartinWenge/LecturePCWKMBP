#include "calculator.h"

//  ##### Definition der Mitgliederfunktionen ######

//Standardkonstruktor
calculator::calculator():a(0), b(0){}

//Konstruktor mit Initialisierungsliste
calculator::calculator(int a_, int b_):a(a_), b(b_){}

//Addition
int calculator::addition(){
  return a+b;
}

//Subtraktion
int calculator::substraction(){
  return a-b;
}

//Multiplikation
int calculator::multiplication(){
  return a*b;
}

//Ausgabe der linearen Funktion
void calculator::print_linear_function(std::string filename){
  //Initialisierung des ofstream zum Rausschreiben der Funktion
  std::ofstream outfile;
  //Datei öffnen, "Flaggen" setzen: out zum rausschreiben, trunc zum Überschreiben falls Datei bereits existierte
  outfile.open(filename.c_str(), std::ios::out | std::ios::trunc);
  
  //Funktion berechnen:
  //eindimensionales Feld = std::vector aus Kontainer für Funktionswerte, auch eine Klasse
  std::vector<int> linear_function;
  //Schleife über die natürlichen Zahlen bis 20
  for( int i=0;i<21;i++ ){
    int f=a*i+b;
    linear_function.push_back(f);
  }
  
  //Werte in die Datei Schreiben
  for(int i=0;i<21;i++ ){
    //rausschreiben der Funktion in der Form: x [Tabulator] f(x)
    outfile << i << "\t" << linear_function.at(i) << std::endl;
  }
  outfile.close();
}

/* *das geht natürlich auch kürzer:
void calculator::print_linear_function(std::string filename){
  std::ofstream outfile(filename.c_str());
  for( int i=0;i<21;i++ )
    outfile << i << "\t" << a*i+b << std::endl;
} // */

//Einlesen der linearen Funktion
void calculator::read_linear_function(std::string filename){
  // Initialisierung des ifstreams zum Einlesen der Funktion
  std::ifstream infile(filename.c_str());
  //2D kontainer für Funktion der Form x, f(x)
  std::vector<std::vector<int> > Fx;
  //Werte einlesen (mit getline und stringstream)
  if(infile.good()){
    std::string line;
    // getline liest aktuelle Zeile bis zum Ende der Datei ein
    while(getline(infile,line)){
      std::stringstream dummy_ss(line);
      std::vector<int> dummy_vec(2,0);
      //Strinstream wird "Stückweise" ausgelesen
      dummy_ss >> dummy_vec.at(0) >> dummy_vec.at(1);
      //dummyvector wird dem 2D Kontainer hinzugefügt
      Fx.push_back(dummy_vec);
    }
  }
  
  // Berechne lineare Funktionsparameter
  for(int i=1;i<(int)(Fx.size());i++){
    //berechne a aus Anstieg
    int a_new=(Fx.at(i).at(1) - Fx.at(i-1).at(1)) / (Fx.at(i).at(0) - Fx.at(i-1).at(0));
    //berechne b aus Funktionsgleichung
    int b_new=Fx.at(i-1).at(1)-a*Fx.at(i-1).at(0);
    // Überprüfe ob immer das gleiche rauskommt
    //std::cout << a_new <<" "<<b_new<<std::endl;
    if(i==1){
      a=a_new;
      b=b_new;
    }else{
      if((a != a_new) || (b != b_new) )
	throw std::runtime_error("read_linear_function: keine lineare Funktion");
    }
  }
  std::cout << "Parameter a= "<<a<<" und b= "<<b<<" wurden aus "<< filename <<" berechnet"<<std::endl;
}
