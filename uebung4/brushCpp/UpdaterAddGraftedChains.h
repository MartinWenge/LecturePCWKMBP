/*--------------------------------------------------------------------------------
    ooo      L   attice-based  |
  o\.|./o    e   xtensible     | LeMonADE: An Open Source Implementation of the
 o\.\|/./o   Mon te-Carlo      |           Bond-Fluctuation-Model for Polymers
oo---0---oo  A   lgorithm and  |
 o/./|\.\o   D   evelopment    | Copyright (C) 2013-2015 by 
  o/.|.\o    E   nvironment    | LeMonADE Principal Developers
    ooo                        | 
----------------------------------------------------------------------------------

This file is part of LeMonADE.

LeMonADE is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LeMonADE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LeMonADE.  If not, see <http://www.gnu.org/licenses/>.

--------------------------------------------------------------------------------*/

#ifndef LEMONADE_UPDATER_SETUP_LINEAR_CHAINS
#define LEMONADE_UPDATER_SETUP_LINEAR_CHAINS
/**
 * @file
 *
 * @class UpdaterAddGraftedChains
 *
 * @brief Updater to add grafted chains
 * */

#include <LeMonADE/updater/UpdaterAbstractCreate.h>
#include <LeMonADE/utility/Vector3D.h>
#include <cmath>
#include <LeMonADE/utility/RandomNumberGenerators.h>

template<class IngredientsType>
class UpdaterAddGraftedChains: public UpdaterAbstractCreate<IngredientsType>
{
  typedef UpdaterAbstractCreate<IngredientsType> BaseClass;
  
public:
  UpdaterAddGraftedChains(IngredientsType& ingredients_, uint32_t NChain_, uint32_t NMonoPerChain_);
  
  virtual void initialize();
  virtual bool execute();
  virtual void cleanup();

  //! getter function for number of monomers in chains
  const int32_t getNMonomerPerChain() const {return NMonoPerChain;}
  
  //! getter function for number of chains
  const int32_t getNChain() const {return NChain;}
  
  //! getter function for calculated density
  const double getDensity() const {return density;}
  
private:
  // provide access to functions of UpdaterAbstractCreate used in this updater
  using BaseClass::ingredients;
  using BaseClass::addMonomerToParent;
  using BaseClass::linearizeSystem;
  using BaseClass::addMonomerAtPosition;
  
  //! number of monomers in a chain
  uint32_t NMonoPerChain;
  
  //! number of linear chains in the box
  uint32_t NChain;
  
  //! lattice occupation density
  double density;
  
  //! bool for execution
  bool wasExecuted;
  
  //! Random number generator
  RandomNumberGenerators rng;
  
};

/** 
* @brief Constructor handling the new systems paramters
*
* @param ingredients_ a reference to the IngredientsType - mainly the system
* @param NChain_ number of chains to be added in the system instead of solvent
* @param NMonoPerChain_ number of monomers in one chain
*/
template < class IngredientsType >
UpdaterAddGraftedChains<IngredientsType>::UpdaterAddGraftedChains(IngredientsType& ingredients_, uint32_t NChain_, uint32_t NMonoPerChain_):
BaseClass(ingredients_), NChain(NChain_), NMonoPerChain(NMonoPerChain_), density(0.0), wasExecuted(false)
{}

/**
* @brief initialise function, calculate the target density to compare with at the end.
*
* @tparam IngredientsType Features used in the system. See Ingredients.
*/
template < class IngredientsType >
void UpdaterAddGraftedChains<IngredientsType>::initialize(){
  std::cout << "initialize UpdaterAddGraftedChains" << std::endl;
  
  // get the target density from the sum of existing monomers and the new added chains
  density=(double)( ingredients.getMolecules().size() + NMonoPerChain*NChain ) * 8  /(double)( ingredients.getBoxX()*ingredients.getBoxY()*ingredients.getBoxZ() );
  
  std::cout << "add "<<NChain*NMonoPerChain<<" monomers to the box"<<std::endl;
  
  execute();
}

/**
* @brief Execution of the system creation
*
* @tparam IngredientsType Features used in the system. See Ingredients.
*/
template < class IngredientsType >
bool UpdaterAddGraftedChains<IngredientsType>::execute(){
  if(wasExecuted)
    return true;
  
  std::cout << "execute UpdaterAddGraftedChains" << std::endl;
  
  //loop over chains and chain monomers and build it up
  for(uint32_t i=0;i<(NChain);i++){
    for(uint32_t j=0;j<(NMonoPerChain);j++){
      if(j==0){
	bool check(false);
	// add the first Monomer to the floor and fix it
	while(!check){
	check=addMonomerAtPosition(VectorInt3(rng.r250_rand32()%ingredients.getBoxX(),rng.r250_rand32()%ingredients.getBoxY(),0));
	ingredients.modifyMolecules()[ingredients.getMolecules().size()-1].setMovableTag(false);
	}
      }else{
	// add the other monomers
	addMonomerToParent(ingredients.getMolecules().size()-1);
      }
    }
  }
  
  ingredients.synchronize();
  double lattice_volume(ingredients.getBoxX()*ingredients.getBoxY()*ingredients.getBoxZ());
  if(std::abs(density - ( (double)(ingredients.getMolecules().size()*8) / lattice_volume )) > 0.0000000001 ){
    std::cout << density << " " <<( (ingredients.getMolecules().size()*8) / lattice_volume)<<std::endl;
    throw std::runtime_error("UpdaterAddGraftedChains: number of monomers in molecules does not match the calculated number of monomers!");
  }else{
    std::cout << "real lattice occupation density =" << (8*ingredients.getMolecules().size()) / lattice_volume<<std::endl;
    wasExecuted=true;

    linearizeSystem();
    return true;
  }
}

/**
* @brief Standard clean up.
*
* @tparam IngredientsType Features used in the system. See Ingredients.
*/
template < class IngredientsType >
void UpdaterAddGraftedChains<IngredientsType>::cleanup(){
  
}


#endif /* LEMONADE_UPDATER_SETUP_LINEAR_CHAINS */