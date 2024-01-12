#include "egg.h"

/*!
 * @class Egg
 * @brief Represents the eggs produced from reproduction between creatures,
 * that stores the traits of the offspring to hatch.
 *
 * @details This class stores the attributes of the offspring that are necessary
 * for Creature initialization. Furthermore stores attributes that dictate behaviour
 * of the egg prior to hatching, such as size_ as they can be consumed.
 */
Egg::Egg(neat::Genome genome, Mutable mutables, const double x_coord, const double y_coord)
  : Food(x_coord, y_coord, mutables.GetBabySize(), settings::environment::kEggNutritionalValue),
    Creature(genome, mutables) {
  type_ = egg;
  age_ = 0;
  incubation_time_ = mutables.GetMaturityAge();
  fertilized_ = true; //FOR NOW WE SAY THAT THE EGG IS FERTILIZED BY DEFAULT, CHANGES IF NOT FERTILIZED UPON BEING LAYED
}

void Egg::Break(){
  Creature::SetState(Dead);
}

Creature Egg::Hatch(){
    return Creature(genome_,mutable_);
}
