#ifndef EGG_H
#define EGG_H

#include "entity/creature/creature.h"
#include "entity/creature/mutable.h"
#include "neat/genome.h"
#include "entity/movable_entity.h"

class Egg : virtual public AliveEntity, virtual public MovableEntity {
 public:
  Egg(const GestatingEgg& gestating_egg,
      const std::pair<double, double>& coordinates);

  double GetIncubationTime() const;

  void Break();
  std::shared_ptr<Creature> Hatch();
  void Update(double delta_time);
  double GetNutritionalValue(){ return nutritional_value_; }
  void SetNutritionalValue(double value) { nutritional_value_ = value; }
  bool CompatibleWithCreature(neat::Genome genome, Mutable mutables);

 protected:
  int generation_;
  double incubation_time_;
  double nutritional_value_;
};

#endif  // EGG_H
