#ifndef EGG_H
#define EGG_H

#include "creature.h"
#include "mutable.h"
#include "neat/neat-genome.h"

class Egg : public Food, public Creature {
 public:
  Egg(neat::Genome genome, Mutable mutable_, double x_coord, double y_coord);
  void Break();
  Creature Hatch() const;
  void SimulationUpdate();

  double GetAge() const;
  double GetIncubationTime() const;

 protected:
  double incubation_time_;
  double age_;
  double size_;
  bool fertilized_;
  std::pair<double, double> coordinates_;
};

#endif  // EGG_H
