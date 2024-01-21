#ifndef EGG_H
#define EGG_H

#include "creature.h"
#include "mutable.h"
#include "neat/neat-genome.h"

class Egg : public Food, public Creature {
 public:
  Egg(const GestatingEgg& gestating_egg);

  double GetIncubationTime() const;

  void Break();
  Creature Hatch();
  void Update(double delta_time);

 protected:
  int generation_;
  double incubation_time_;
};

#endif  // EGG_H
