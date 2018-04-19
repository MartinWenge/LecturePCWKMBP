#include "rng_linear_congruential.h"

//  ##### Definition der Mitgliederfunktionen ######

//Standardkonstruktor
/* ############ add your ideas ############ */
rng_linear_congruential::rng_linear_congruential():a(1), b(1), m(1), x_0(1){}

//Konstruktor mit Initialisierungsliste
rng_linear_congruential::rng_linear_congruential(int a_, int b_, unsigned int m_, int x_0_):a(a_), b(b_), m(m_), x_0(x_0_){}

// getter m
int rng_linear_congruential::get_m(){
  return m;
}

// setter m
void rng_linear_congruential::set_m(int m_){
  m=m_;
}

// calculate a rng: int
int rng_linear_congruential::get_rng_int(){
  /* ############ add your ideas ############ */
}

// calculate a rng: double in [0,1]
double rng_linear_congruential::get_rng_double(){
  /* ############ add your ideas ############ */
}

int rng_linear_congruential::get_period(){
  /* ############ add your ideas ############ */
}

// write out series
void rng_linear_congruential::print_rng_series_to_file(std::string filename, int N){
  std::vector< std::vector< int > > series;
  
  /* ############ add your ideas ############ */
  
  print_to_file(filename, series);
}

// write out pairs
void rng_linear_congruential::print_rng_pairs_to_file(std::string filename, int N){
  std::vector< std::vector< double > > pairs;
  
  /* ############ add your ideas ############ */
  
  print_to_file(filename, pairs);
}

// generell call of write out function
template<typename TYPE>
void rng_linear_congruential::print_to_file(std::string filename, const std::vector< std::vector< TYPE > > &source){
  std::ofstream outfile(filename.c_str(), std::ios::out | std::ios::trunc);
  for(int i=0; i < source.size(); i++){
    for(int j=0; j< source.at(i).size(); j++){
      outfile << source.at(i).at(j) <<"\t";
    }
    outfile << std::endl;
  }
  outfile.close();
}
