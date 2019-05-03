#include "ising_simulator.h"

//  ##### Definition der Mitgliederfunktionen ######
/**
 *  Überall wo  ############ add your ideas ############ steht, müsst ihr etwas machen:
 *  eine Möglichkeit für periodische Randbedingungen:
 * inline int ising_simulator::getLeftNeigbor(int x)
 * inline int ising_simulator::getRightNeigbor(int x)
 * inline int ising_simulator::getLowerNeigbor(int y)
 * inline int ising_simulator::getUpperNeigbor(int y)
 * 
 * double ising_simulator::calc_magnetization
 * 
 * void ising_simulator::perform_mcs(int time)
 * 
 * */

//Standardkonstruktor
ising_simulator::ising_simulator():
lattice_size(20), interaction_parameter(1.0), external_field(0.0), beta(1.0), lattice(lattice_size*lattice_size,0), rng_mt(std::random_device()()), dist_int(0,lattice_size-1), dist_double(0.0,1.0) {}

//Konstruktor mit Initialisierungsliste
ising_simulator::ising_simulator(int lattice_size_, double interaction_parameter_, double external_field_, double beta_):
  lattice_size(lattice_size_), interaction_parameter(interaction_parameter_), external_field(external_field_), beta(beta_), 
  lattice(lattice_size*lattice_size,0), rng_mt(std::random_device()()), dist_int(0,lattice_size-1), dist_double(0.0,1.0) {}

// getter B
int ising_simulator::get_beta(){
  return beta;
}
// setter B
void ising_simulator::set_beta(int beta_){
  beta=beta_;
}

// neigbor requests -> nur eine Möglichkeit für periodische randbedingungen
/* ############ add your ideas ############ */
inline int ising_simulator::getLeftNeigbor(int x){

}
/* ############ add your ideas ############ */
inline int ising_simulator::getRightNeigbor(int x){

}
/* ############ add your ideas ############ */
inline int ising_simulator::getLowerNeigbor(int y){

}
/* ############ add your ideas ############ */
inline int ising_simulator::getUpperNeigbor(int y){

}

// calculate observables:
// calculate magnetization of a specific conformation
double ising_simulator::calc_magnetization(){
  double M(0.0);
  
  /* ############ add your ideas ############ 
  M = 1/N * ∑ (i=1 -> N) S_i
  */
  
  return M;
}

// calculate single spins energy (use this function again for task Importance Sampling)
double ising_simulator::calc_single_spin_energy(int x, int y){
  //first calculate magnetic field at spin position
  double b(external_field+(interaction_parameter*0.5)*(
    lattice.at(getLeftNeigbor(x)+lattice_size*y)+
    lattice.at(getRightNeigbor(x)+lattice_size*y)+
    lattice.at(x+getUpperNeigbor(y)*lattice_size)+
    lattice.at(x+getLowerNeigbor(y)*lattice_size)
  ));
  // then calculate energy
  return (b*lattice.at(x+lattice_size*y));
}

// calculate energy of a specific confomration
double ising_simulator::calc_energy(){
  //return value
  double E(0.0);
  //loop over lattice
  for(int x=0;x<lattice_size;x++){
    for(int y=0;y<lattice_size;y++){
      E+=(-calc_single_spin_energy(x,y));
    }
  }
  //return full energy
  return E;
}

//change lattice occupation
// create a new random lattice occupation
void ising_simulator::randomize(){
  for(int i=0;i<(int)lattice.size();i++){
    //get random Number
    double rand(dist_double(rng_mt));
    // Wenn Zufallszahl kleiner 0.5, setzte Spin up, wenn größer(gleich) 0.5 setzte Spin down
    if(rand<0.5)
      lattice.at(i)=1;
    else if(rand>=0.5)
      lattice.at(i)=-1;
  }
}

//perform a MCS
void ising_simulator::perform_mcs(int time){
  int counter(0); // check for moves

  // deine for schleife sollte bis lattice_size*lattice_size gehen, ein -1 ist nicht nötig, wegen dem "i <", das stopt automatisch bei (L*L)-1
  for(int i=0; i < lattice_size*lattice_size - 1; i++){
    int x(dist_int(rng_mt));
    int y(dist_int(rng_mt));
    int k(x + lattice_size*y);

    double E_0(calc_single_spin_energy(x, y));
    double flipprob(exp(-2 * E_0 * beta));
    // das ist ein seltsames Kriterium ;) du willst doch den Boltzmann Faktor exp(\Delta E / k_B T) mit einer zufallszahl zwischen [0,1] vergleichen
    // if ( flipprob < dist_double(rng_mt) ){ ...
    if (k/(lattice_size*lattice_size - 1) < flipprob){
      lattice.at(k) = lattice.at(k)*(-1);
      counter =+ 1;
    } else {
      // dieses break bedeutet, dass du nach einem nicht akzeptierten move aus deiner for Schleife raus springst. Willst du das wirklich? Brauchst du hier ein else?
      break;
    }
  }

  if(counter==0)
    std::cout<<"Keine akzeptierte Bewegung in aktuellem Monte Carlo Schritt!!\n";
}

// do the complex things
void ising_simulator::perform_simple_sampling(int time, std::string filename){
  std::vector<std::vector<double> > magnetization_data;
  std::vector<std::vector<double> > energy_data;
  
  double ave_magnetization(0.0);
  double ave_energy(0.0);
  double normalization(0.0);
  for(int j=0; j < time; j++){
    randomize();
    double E(calc_energy());
    double bolzmannFactor(exp(-E*beta));
    normalization+=bolzmannFactor;
    ave_magnetization+=(calc_magnetization()*bolzmannFactor);
    ave_energy+=(E*bolzmannFactor);
  }
  //write magnetization data
  std::vector<double> dummy(2,0);
  dummy.at(0)=beta;
  dummy.at(1)=(ave_magnetization/normalization);
  magnetization_data.push_back(dummy);
  
  //write energy data
  dummy.at(1)=(ave_energy/normalization);
  energy_data.push_back(dummy);

  std::cout << "write magnetization data to file "<< filename+"_magnetization.dat" << std::endl;
  print_to_file(filename+"_magnetization.dat", magnetization_data);
  std::cout << "write energy data to file "<< filename+"_energy.dat" << std::endl;
  print_to_file(filename+"_energy.dat", energy_data);
}

void ising_simulator::perform_importance_sampling(int time, std::string filename){
  std::cout << "Importance sampling"<<std::endl;
  std::vector<std::vector<double> > magnetization_data;
  std::vector<std::vector<double> > energy_data;
  std::vector<std::vector<double> > heat_capacity_data;
  
  double ave_magnetization(0.0);
  double ave_energy(0.0);
  double mean_squared_energy(0.0);
  
  //equlibration
  randomize();
  std::vector<std::vector<double> > equilibrate;
  for(int j=0; j < time; j++){
    perform_mcs(10);
    std::vector<double> timestep(2,0);
    timestep.at(0)=j*10;
    timestep.at(1)=calc_magnetization();
    equilibrate.push_back(timestep);
  }
  std::stringstream ss;
  ss << beta;
  print_to_file(filename+"_equilibrate_"+ss.str()+".dat",equilibrate);
  
  // calculate averages
  for(int j=0; j < time; j++){
    perform_mcs(100);
    double E(calc_energy());
    ave_energy+=E;
    mean_squared_energy+=(E*E);
    ave_magnetization+=calc_magnetization();
  }
  
  //collect data
  std::vector <double> dummy(2,0);
  dummy.at(0)=beta;
  
  dummy.at(1)=ave_energy/time;
  energy_data.push_back(dummy);
  
  dummy.at(1)=ave_magnetization/time;
  magnetization_data.push_back(dummy);
  
  dummy.at(1)=(beta*beta*((mean_squared_energy/time)-(ave_energy*ave_energy/(time*time))))/(lattice.size());
  heat_capacity_data.push_back(dummy);
  
  std::cout << "write magnetization data to file "<< filename+"_magnetization.dat" << std::endl;
  print_to_file(filename+"_magnetization.dat", magnetization_data);
  std::cout << "write energy data to file "<< filename+"_energy.dat" << std::endl;
  print_to_file(filename+"_energy.dat", energy_data);
  std::cout << "write heat capacity data to file "<< filename+"_heat_cap.dat" << std::endl;
  print_to_file(filename+"_heat_cap.dat", heat_capacity_data);
}


// generell call of write out function
template<typename TYPE>
void ising_simulator::print_to_file(std::string filename, const std::vector< std::vector< TYPE > > &source){
  std::ofstream outfile(filename.c_str(), std::ios::out | std::ios::trunc);
  for(int i=0; i < source.size(); i++){
    for(int j=0; j< source.at(i).size(); j++){
      outfile << source.at(i).at(j) <<"\t";
    }
    outfile << std::endl;
  }
  outfile.close();
}