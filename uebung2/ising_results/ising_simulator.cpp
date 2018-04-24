#include "ising_simulator.h"

//  ##### Definition der Mitgliederfunktionen ######

//Standardkonstruktor
ising_simulator::ising_simulator():
lattice_size(20), interaction_parameter(1.0), external_field(0.0), beta(1,1.0), lattice(lattice_size*lattice_size,0), rng_mt(std::random_device()()), dist_int(0,lattice_size-1), dist_double(0.0,1.0) {}

//Konstruktor mit Initialisierungsliste
ising_simulator::ising_simulator(int lattice_size_, double interaction_parameter_, double external_field_, std::vector< double > beta_):
lattice_size(lattice_size_), interaction_parameter(interaction_parameter_), external_field(external_field_), beta(beta_), lattice(lattice_size*lattice_size,0), rng_mt(std::random_device()()), dist_int(0,lattice_size-1), dist_double(0.0,1.0) {}

//Konstruktor mit Initialisierungsliste 2
ising_simulator::ising_simulator(int lattice_size_, double interaction_parameter_, double external_field_, double beta_):
lattice_size(lattice_size_), interaction_parameter(interaction_parameter_), external_field(external_field_), beta(1,beta_), lattice(lattice_size*lattice_size,0), rng_mt(std::random_device()()), dist_int(0,lattice_size-1), dist_double(0.0,1.0) {}

// getter B
int ising_simulator::get_B(){
  return external_field;
}
// setter B
void ising_simulator::set_B(int B_){
  external_field=B_;
}

// neigbor requests
inline int ising_simulator::getLeftNeigbor(int x){
  if((x-1)<0) // prüfe Nachbar in Box liegt
    return ((x-1)+lattice_size); // wenn nein gehe auf die andere Seite
  else
    return (x-1); // sonst gehe direkt zum Nachbarn
}
inline int ising_simulator::getRightNeigbor(int x){
  if((x+1)>(lattice_size-1))
    return ((x+1)-lattice_size);
  else
    return (x+1);
}
inline int ising_simulator::getLowerNeigbor(int y){
  if((y-1)<0)
    return ((y-1)+lattice_size);
  else
    return (y-1);
}
inline int ising_simulator::getUpperNeigbor(int y){
  if((y+1)>(lattice_size-1))
    return ((y+1)-lattice_size);
  else
    return (y+1);
}

// calculate observables:
// calculate magnetization of a specific conformation
double ising_simulator::calc_magnetization(){
  double M(0.0);
  // sum up lattice spins
  for(int i=0;i<(int)lattice.size();i++){
    M=M+lattice.at(i);
  }
  // normalize Magnetization
  return (M/((double)lattice.size()));
}

// calculate single spins energy (use this function again for task Importance Sampling!)
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
void ising_simulator::perform_mcs(int time, double temperature){
  int counter(0); // check for moves
  for(int i=0;i<time;i++){
    for(int j=0;j<(int)lattice.size();j++){
      int x(dist_int(rng_mt));
      int y(dist_int(rng_mt));
      double E_old(calc_single_spin_energy(x,y));
      double E_new(-E_old);
      double w(exp((E_new-E_old)*temperature));
      if(dist_double(rng_mt) < w){
	lattice.at(x+lattice_size*y)=-lattice.at(x+lattice_size*y);
	counter++;
      }
    }
  }
  if(counter==0) 
    std::cout<<"Keine akzeptierte Bewegung in aktuellem Monte Carlo Schritt!!\n"; 
}

// do the complex things
void ising_simulator::perform_simple_sampling(int time, std::string filename){
  std::vector<std::vector<double> > magnetization_data;
  std::vector<std::vector<double> > energy_data;
  
  for(int i=0;i<(int)beta.size();i++){
    double ave_magnetization(0.0);
    double ave_energy(0.0);
    double normalization(0.0);
    for(int j=0; j < time; j++){
      randomize();
      double E(calc_energy());
      double bolzmannFactor(exp(-E*beta.at(i)));
      normalization+=bolzmannFactor;
      ave_magnetization+=(calc_magnetization()*bolzmannFactor);
      ave_energy+=(E*bolzmannFactor);
    }
    //write magnetization data
    std::vector<double> dummy(2,0);
    dummy.at(0)=beta.at(i);
    dummy.at(1)=(ave_magnetization/normalization);
    magnetization_data.push_back(dummy);
    
    //write energy data
    dummy.at(1)=(ave_energy/normalization);
    energy_data.push_back(dummy);
  }
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
  
  for(int i=0;i<(int)beta.size();i++){
    std::cout << "Inverse temperature set to beta= "<<beta.at(i)<<std::endl;
    double ave_magnetization(0.0);
    double ave_energy(0.0);
    double mean_squared_energy(0.0);
    
    //equlibration
    randomize();
    std::vector<std::vector<double> > equilibrate;
    for(int j=0; j < time; j++){
      perform_mcs(10, beta.at(i));
      std::vector<double> timestep(2,0);
      timestep.at(0)=j*10;
      timestep.at(1)=calc_magnetization();
      equilibrate.push_back(timestep);
    }
    std::stringstream ss;
    ss << beta.at(i);
    print_to_file(filename+"_equilibrate_"+ss.str()+".dat",equilibrate);
    
    // calculate averages
    for(int j=0; j < time; j++){
      perform_mcs(100, beta.at(i));
      double E(calc_energy());
      ave_energy+=E;
      mean_squared_energy+=(E*E);
      ave_magnetization+=calc_magnetization();
    }
    
    //collect data
    std::vector <double> dummy(2,0);
    dummy.at(0)=beta.at(i);
    
    dummy.at(1)=ave_energy/time;
    energy_data.push_back(dummy);
    
    dummy.at(1)=ave_magnetization/time;
    magnetization_data.push_back(dummy);
    
    dummy.at(1)=(beta.at(i)*beta.at(i)*((mean_squared_energy/time)-(ave_energy*ave_energy/(time*time))))/(lattice.size());
    heat_capacity_data.push_back(dummy);
  }
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