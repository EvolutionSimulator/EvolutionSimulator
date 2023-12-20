#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <iostream>
#include <vector>

#include "settings.h"

namespace myEnvironment {
class Environment {
 public:
  // Constructor
  Environment();

  // Getter and setter for food density
  void SetFoodDensity(double density) { food_density_ = density; }

  double GetFoodDensity() const { return food_density_; }

  // Getter and setter for creature density
  void SetCreatureDensity(double density) { creature_density_ = density; }
  double GetCreatureDensity() const { return creature_density_; }

 private:
  double food_density_;  // Variable for creature density
  double creature_density_;
};
}  // namespace myEnvironment

#endif  // ENVIRONMENT_H
