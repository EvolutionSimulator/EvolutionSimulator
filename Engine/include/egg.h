#ifndef EGG_H
#define EGG_H

#include "creature.h"
#include "mutable.h"
#include "neat/neat-genome.h"

class Egg : virtual public Food, virtual public AliveEntity {
 public:
  Egg(const GestatingEgg& gestating_egg,
      const std::pair<double, double>& coordinates);

  double GetIncubationTime() const;

  void Break();
  Creature Hatch();
  void Update(double delta_time);

 protected:
  int generation_;
  double incubation_time_;
};

#endif  // EGG_H
