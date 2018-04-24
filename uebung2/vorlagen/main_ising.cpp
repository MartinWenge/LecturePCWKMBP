/* 
 * die Main funktion ist soweit fertig, Änderungen müssen nur in ising_simulator.cpp gemacht werden
 *
 * use the simulation class. compile with g++ main_ising.cpp ising_simulator.cpp -o Ising
 * ore use the cmake scipt if cmake is installed:
 * mkdir build
 * cd build
 * cmake ../
 * make
 * */

#include "ising_simulator.h"
#include <cstdlib>


int main(int argc, char* argv[]){
  std::cout << "\nStarte Programm Ising\n"<<std::endl;
  
  // simulation paramters we need:
  int simulation_time(1000);
  int lattice_size(20);
  double extern_field_B(0.0);
  double temp(0.0);
  double temperatue(0.1);
  
  // read in arguments from command line: 
  //this is dine by argc = number of arguments, e.g. "./Ising -n 40 -time 100000" ->argc=5
  // argv = array of characters holding exactly the command line input
  if(argc==1){	//set filenames to "standard" value
    std::cout << "using standard values\n";
  }else if((argc%2==1) && (argc>1)){ //even number of extra arguments -> key + value
    // iterate over input
    for(int i=1;i<argc;i=i+2){
      // convert array of characters to sting -> easy handling
      std::string option(argv[i]);
      std::string value(argv[i+1]);
      // for instance easy to print:
      std::cout << option <<" "<< value<< std::endl;
      
      // ...and easy to compare
      if(option.compare("-time")==0){ // set simulation time
	      // ...and easy to convert to int
        simulation_time = atoi(argv[i+1]);
      }else if(option.compare("-size")==0){ // set lattice size
        lattice_size = atoi(argv[i+1]);
      }else if(option.compare("-field")==0){ // set field
	      // ...and easy to convert to double
        extern_field_B = atol(argv[i+1]);
      }else if(option.compare("-temperature")==0){ // set temperatue
        temp = atol(argv[i+1]);
      }else{
	      std::cout << "unknown paramter, use: -time, -size, -field, -temperature"<<std::endl;
      }
    }
    
  }else{
    throw std::runtime_error("check input: -time ..., -size ..., -field ..., -temperature ...");
  }
  //show values:
  std::cout << "time= "<<simulation_time<<", extern field= "<<extern_field_B<<", lattice size= "<<lattice_size<<", invTemperature= "<<temp<<std::endl;
  
  // ############ hier we start with the simulations ################## //
  
  // we use the simulation class
  std::cout << "init Knut" <<std::endl;
  //ising_simulator(int lattice_size_, double interaction_parameter_, double external_field_, std::vector< double > beta_);
  ising_simulator Knut(lattice_size,1.0,extern_field_B,temperatue);
  
  // do the simple sampling
  std::cout << "do simple sampling" <<std::endl;
  Knut.perform_simple_sampling(simulation_time*100,"simple_sampling");
  
  // do the importance sampling
  //  std::cout << "do importance sampling" <<std::endl;
  //  Knut.perform_importance_sampling(simulation_time,"importance_sampling");
  
  std::cout << "\n Beende Programm Ising\n"<<std::endl;
  
  return 0;
}
