#ifndef ISING_SIMULATOR_H
#define ISING_SIMULATOR_H

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cmath>
#include <random>

//  ##### Definition der Klasse ##### 
class ising_simulator{
public:
  // Konstruktor = Aufruf, der eine Instanz der Klasse erzeugt
  // Standardkonstruktor (leer)
  ising_simulator();
  // Konstruktor mit Initialisungsvariablen a, b, m und x_0
  ising_simulator(int lattice_size_, double interaction_parameter_, double external_field_, std::vector< double > beta_);
  // constructor as above, but instead of a vector of temperatures only one value
  ising_simulator(int lattice_size_, double interaction_parameter_, double external_field_, double beta_);
  
  // Destruktor = Aufruf, der eine erstellte Instanz der Klasse löscht und den Speicher wieder frei gibt
  // Standarddestruktor (leer)
  ~ising_simulator(){}
  
  // getter, setter functions (just to show the idea of this kind of functions)
  int get_B();
  void set_B(int B_);
  
  // calculate stuff
  void perform_simple_sampling(int time, std::string filename);  /* ############ add your ideas ############ */
  void perform_importance_sampling(int time, std::string filename);
   
private:
  // system parameters
  int lattice_size;
  double interaction_parameter;
  double external_field;
  
  std::vector< double > beta;
  
  // system container
  std::vector< int > lattice; // 1 D lattice, we adress it like a 2 D one: lattice(x,y)=lattice.at(x+L*y)
  
  // instance of random number generator
  std::mt19937 rng_mt;
  std::uniform_int_distribution<int> dist_int;
  std::uniform_real_distribution<double> dist_double;
  
  // private support functions
  inline int getLeftNeigbor(int x);  /* ############ add your ideas ############ */
  inline int getRightNeigbor(int x);  /* ############ add your ideas ############ */
  inline int getLowerNeigbor(int y);  /* ############ add your ideas ############ */
  inline int getUpperNeigbor(int y);  /* ############ add your ideas ############ */
  
  void randomize();
  void perform_mcs(int time, double temperature);  /* ############ add your ideas ############ */
  double calc_magnetization();  /* ############ add your ideas ############ */
  double calc_energy();
  double calc_single_spin_energy(int x, int y);
  
  // template class to write out stuff from a 2D array: 
  // TYPE can be everything like int, double, float, uint, string, ...
  // we use this private, as we have special classes for different content
  template<typename TYPE> 
  void print_to_file(std::string filename, const std::vector< std::vector< TYPE > > &source);
};

#endif //ISING_SIMULATOR_H
