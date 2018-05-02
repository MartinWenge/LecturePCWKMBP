#ifndef LEMONADE_UPDATER_RANDOMWALK_H
#define LEMONADE_UPDATER_RANDOMWALK_H

#include <LeMonADE/updater/UpdaterAbstractCreate.h>

template<class IngredientsType>
class UpdaterRandomWalk:public UpdaterAbstractCreate<IngredientsType>
{
public:
  //! shortcut for Base class, is needed in member declaration and constructor
  typedef UpdaterAbstractCreate<IngredientsType> BaseClass;
  
  //! constructor
  UpdaterRandomWalk(IngredientsType& ingredients_, uint32_t nSteps_);
  //! destructor
  virtual ~UpdaterRandomWalk(){}
  
  //! public function used by taskManager: initialize
  virtual void initialize();
  //! public functions used by taskManager: run
  virtual bool execute();
  //! public functions used by taskManager: cleanup
  virtual void cleanup(){}
  
private:
  //! use the ingredients reference of base class
  using BaseClass::ingredients;
  //! use the predefined functions to add monomers to each other 
  using BaseClass::addMonomerToParent;
  //! use predefined function to add monomer alone in the box.
  using BaseClass::addSingleMonomer;
  //! length of random walk
  uint32_t nSteps;
};

/** 
 *  Member implementations for template classes 
 * are implemented also in the header file:
 * first there is the constructor calling base class constructor
 */
template<class IngredientsType>
UpdaterRandomWalk<IngredientsType>::UpdaterRandomWalk(IngredientsType& ingredients_, uint32_t nSteps_):
BaseClass(ingredients_), nSteps(nSteps_)
{}

/**
 *  next initialise, there we always call execute once 
 * to make shure we have a fully working system after 
 * taskManager.initialise()
 */
template < class IngredientsType >
void UpdaterRandomWalk<IngredientsType>::initialize(){
  std::cout << "initialize UpdaterRandomWalk with "<<nSteps<<" steps." << std::endl;
  
  execute();
}

/**
 *  then the most important part: the execute function
 * here the "walk should be made
 */
template < class IngredientsType >
bool UpdaterRandomWalk<IngredientsType>::execute(){
  //clear old molecules
  uint64_t systemAge(ingredients.getMolecules().getAge());
  ingredients.modifyMolecules().clear();
  
  // add monomers: use addMonomerToParent(parent Index); and addSingleMonomer();
  /* ############ add your ideas ############ */

  
  
  
  
  //check system before return ok
  if(ingredients.getMolecules().size()==nSteps){
    ingredients.modifyMolecules().setAge( systemAge +1);
    return true;
  }else
    throw std::runtime_error("UpdaterRandomWalk: incorrect system size");;
}

#endif /* LEMONADE_UPDATER_RANDOMWALK_H */
