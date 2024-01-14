#ifndef EGG_H
#define EGG_H

#include "creature.h"
#include "mutable.h"
#include "neat/neat-genome.h"

class Egg : public Food, public Creature {
 public:
  Egg();
  Egg(neat::Genome genome, Mutable mutable_, int generation);
  Egg(neat::Genome genome, Mutable mutable_, double coord_x, double coord_y); // TODO: remove this constructor
  Egg(neat::Genome genome, Mutable mutable_, int generation, double coord_x, double coord_y); // TODO: remove this constructor
  void Break();
  Creature Hatch() const;
  void SimulationUpdate();
  void SetCoordinates(double x_coord, double y_coord);

  double GetAge() const;
  double GetIncubationTime() const;

  double GetPercentageIncubated() const;

 protected:
  double incubation_time_;
  double age_;
  double size_;
  bool fertilized_;
  std::pair<double, double> coordinates_;
};

#endif  // EGG_H
