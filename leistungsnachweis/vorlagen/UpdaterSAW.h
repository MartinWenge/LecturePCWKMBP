#ifndef LEMONADE_UPDATER_SAW_H
#define LEMONADE_UPDATER_SAW_H

#include <LeMonADE/updater/UpdaterAbstractCreate.h>

template<class IngredientsType>
class UpdaterSelfAvoidingWalk:public UpdaterAbstractCreate<IngredientsType>
{
public:
  //! shortcut for Base class, is needed in member declaration and constructor
  typedef UpdaterAbstractCreate<IngredientsType> BaseClass;
  
  //! constructor
  UpdaterSelfAvoidingWalk(IngredientsType& ingredients_, uint32_t nSteps_);
  //! destructor
  virtual ~UpdaterSelfAvoidingWalk(){}
  
  //! public function used by taskManager: initialize
  virtual void initialize();
  //! public functions used by taskManager: run
  virtual bool execute();
  //! public functions used by taskManager: cleanup
  virtual void cleanup(){}
  
private:
  //! use the ingredients reference of base class
  using BaseClass::ingredients;
  //! use the predefined functions to add monomers at certain position
  using BaseClass::addMonomerAtPosition;
  //! use predefined function to add monomer alone in the box.
  using BaseClass::addSingleMonomer;
  //! instance of static Random Number Generator (RNG)
  RandomNumberGenerators rng;
  //! length of random walk
  uint32_t nSteps;
};

/** 
 *  Member implementations for template classes 
 * are implemented also in the header file:
 * first there is the constructor calling base class constructor
 */
template<class IngredientsType>
UpdaterSelfAvoidingWalk<IngredientsType>::UpdaterSelfAvoidingWalk(IngredientsType& ingredients_, uint32_t nSteps_):
BaseClass(ingredients_), nSteps(nSteps_)
{}

/**
 *  next initialise, there we always call execute once 
 * to make shure we have a fully working system after 
 * taskManager.initialise()
 */
template < class IngredientsType >
void UpdaterSelfAvoidingWalk<IngredientsType>::initialize(){
  std::cout << "initialize UpdaterSelfAvoidingWalk with "<<nSteps<<" steps." << std::endl;
  
  execute();
}

/**
 *  then the most important part: the execute function
 * here the "walk" should be made
 */
template < class IngredientsType >
bool UpdaterSelfAvoidingWalk<IngredientsType>::execute(){
  //clear old molecules
  uint64_t systemAge(ingredients.getMolecules().getAge());
  ingredients.modifyMolecules().clear();
  
  // a saw is not always successfull, so we try it serval times
  bool successfull(false);
  int32_t counter(0);
  while( !successfull && counter<10000 ){
    //std::cout << counter << ": "<<successfull<< std::endl;

    for(int32_t i=0;i<nSteps;i++){
      // if there is at lest one monomer, add a new one
      if(i>0){
        int32_t monoID(i-1);
        VectorInt3 position( ingredients.getMolecules()[monoID] );
        int32_t moveCounter(0);
        while(moveCounter<20){
          int32_t dir( rng.r250_rand32()%3 );
          int32_t sign( ((rng.r250_rand32()%2)*2)-1);
          // bool ? true : false; == if(bool){trueAction;}else{falseAction;}
          VectorInt3 direction(dir==0 ? 2*sign : 0, dir==1 ? 2*sign : 0,dir==2 ? 2*sign : 0);

          //std::cout << "dir = "<<dir<<", sign = "<< sign<<", direction = "<<direction<< std::endl;
          addMonomerAtPosition(position+direction,1) ? moveCounter=100 : moveCounter++;
        }
        if(moveCounter==100){
          // in this case the monomer could be placed, lets connect it
          ingredients.modifyMolecules().connect( monoID, ingredients.getMolecules().size()-1 );
        }else{
          // in this case the monomer could not be placed, we have to restart the loop
          counter++;
          break;
        }
      }else{
        addSingleMonomer(1);
      }
      // check if system was created completely
      if(ingredients.getMolecules().size()==nSteps){
        successfull=true;
      }
    }
    if( !successfull ){
      // if system was not fully created, remove the old approach
      ingredients.modifyMolecules().clear();
    }
  }
  
  //check system before return ok
  if(ingredients.getMolecules().size()==nSteps){
    ingredients.modifyMolecules().setAge( systemAge +1);
    return true;
  }else
    throw std::runtime_error("UpdaterSelfAvoidingWalk: incorrect system size");;
}

#endif /* LEMONADE_UPDATER_SAW_H */
