// Compilieren mit g++ main_programming.cpp calculator.cpp -o Calculator

// Einfügen unserer selbstgeschrieben Headerklasse
#include "calculator.h"

int main(){
  std::cout << "\nStarte Programm Projektorganisation\n";
  
  //Aufruf des Konstruktors calculator(int a_, int b_)
  calculator Helmut(2,3);
  
  std::cout << "Helmut: Summe von a und b = "<< Helmut.addition()<<std::endl;
  std::cout << "Helmut: Differenz von a und b = "<< Helmut.substraction()<<std::endl;
  std::cout << "Helmut: Produkt von a und b = "<< Helmut.multiplication()<<std::endl;
  //schreibe Funktion raus:
  Helmut.print_linear_function("test_linear_function.dat");
  
  
  //neue Instanz von Calculator mit Standardkonstruktor (a und b haben zufällige Werte)
  calculator Knut;
  //ließ Daten von Helmut ein
  Knut.read_linear_function("test_linear_function.dat");
  std::cout << "Knut: Summe von a und b = "<< Knut.addition()<<std::endl;
  std::cout << "Knut: Differenz von a und b = "<< Knut.substraction()<<std::endl;
  std::cout << "Knut: Produkt von a und b = "<< Knut.multiplication()<<std::endl;
  
  return 0;
}
