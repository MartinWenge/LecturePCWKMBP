// Compilieren mit g++ main_rng.cpp rng_linear_congruential.cpp -o RandomNumbers
// oder cmake

// Einfügen unserer selbstgeschrieben Headerklasse
#include "rng_linear_congruential.h"

int main(){
  std::cout << "\nStarte Programm RandomNumbers\n"<<std::endl;
  
  //Aufruf des Konstruktors rng_linear_congruential(int a_, int b_, int m_, int x_0_)
  int a(47), b(7), x_0(42);
  unsigned int m(83);
  
  rng_linear_congruential myRngHelmut(a,b,m,x_0);
  
  std::cout << "myRngHelmut: get parameter m = "<< myRngHelmut.get_m()<<std::endl;
  std::cout << "myRngHelmut: get a \"random\" int = "<< myRngHelmut.get_rng_int()<<std::endl;
  std::cout << "myRngHelmut: get a \"random\" double = "<< myRngHelmut.get_rng_double()<<std::endl;
  // get period of rng:
  std::cout << "myRngHelmut: get rng period = "<< myRngHelmut.get_period()<<", expected 82"<<std::endl;
  
  myRngHelmut.print_rng_series_to_file("myRngHelmut_series.dat", 1000);
  myRngHelmut.print_rng_pairs_to_file("myRngHelmut_pairs.dat", 1000);
  std::cout << "Files for myRngHelmut written out."<<std::endl;
  
  
  //neue Instanz von Calculator mit Standardkonstruktor
  rng_linear_congruential myRngKnut;
  
  // do the same for the "best" int 32 parameters
  std::cout << "myRngKnut: get parameter m = "<< myRngKnut.get_m()<<std::endl;
  std::cout << "myRngKnut: get a \"random\" int = "<< myRngKnut.get_rng_int()<<std::endl;
  std::cout << "myRngKnut: get a \"random\" double = "<< myRngKnut.get_rng_double()<<std::endl;
  // get period of rng:
  // std::cout << "myRngKnut: get rng period = "<< myRngKnut.get_period()<<std::endl;
  
  myRngKnut.print_rng_series_to_file("myRngKnut_series.dat");
  myRngKnut.print_rng_pairs_to_file("myRngKnut_pairs.dat",10000);
  std::cout << "Files for myRngKnut written out."<<std::endl;
  
  std::cout << "\nBeende Programm RandomNumbers\n"<<std::endl;
  
  return 0;
}
