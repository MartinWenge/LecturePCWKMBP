#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include<iostream>

//  ##### Definition der Klasse ##### 
class calculator{
public:
  // Konstruktor = Aufruf, der eine Instanz der Klasse erzeugt
  // Standardkonstruktor (leer)
  calculator();
  // Konstruktor mit Initialisungsvariablen a und b
  calculator(int a_, int b_);
  
  // Destruktor = Aufruf, der eine erstellte Instanz der Klasse löscht und den Speicher wieder frei gibt
  // Standarddestruktor (leer)
  ~calculator(){}
  
  int addition();
  int substraction();
  int multiplication();
  void print_linear_function(std::string filename);
  void read_linear_function(std::string filename);
  
private:
  int a;
  int b;
};

#endif //CALCULATOR_H
