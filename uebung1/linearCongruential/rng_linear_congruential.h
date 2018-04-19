#ifndef RNG_LINEAR_CONGRUENTIAL_H
#define RNG_LINEAR_CONGRUENTIAL_H

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cmath>

//  ##### Deklaration der Klasse ##### 
class rng_linear_congruential{
public:
  // Konstruktor = Aufruf, der eine Instanz der Klasse erzeugt
  // Standardkonstruktor (leer)
  rng_linear_congruential();
  // Konstruktor mit Initialisungsvariablen a, b, m und x_0
  rng_linear_congruential(int a_, int b_, unsigned int m_, int x_0_);
  
  // Destruktor = Aufruf, der eine erstellte Instanz der Klasse löscht und den Speicher wieder frei gibt
  // Standarddestruktor (leer)
  ~rng_linear_congruential(){}
  
  // getter, setter functions (just to show the idea of this kind of functions)
  int get_m();
  void set_m(int m_);
  
  // calculate rng
  int get_rng_int();
  double get_rng_double();
  
  //period of rng
  int get_period();
  
  // print some N numbers for further evaluation
  // N has a standart value, so you can call just print...(filename) with N=100
  // or you can call print...(filename, N) for an arbitrary number of values
  void print_rng_series_to_file(std::string filename, int N=100);
  void print_rng_pairs_to_file(std::string filename, int N=100);
  
private:
  int a, b;
  unsigned int m; 
  int x_0;
  
  // template class to write out stuff from a 2D array: 
  // TYPE can be everything like int, double, float, uint, string, ...
  // we use this private, as we have special classes for different content
  template<typename TYPE> 
  void print_to_file(std::string filename, const std::vector< std::vector< TYPE > > &source);
};

#endif //RNG_LINEAR_CONGRUENTIAL_H
