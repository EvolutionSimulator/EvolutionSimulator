#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <iostream>
#include <vector>

#include "settings.h"

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

  void SetFrictionalCoefficient(double friction_coefficient) {friction_coefficient_ = friction_coefficient; }
  double GetFrictionalCoefficient() const { return friction_coefficient_; }

 private:
  double food_density_;  // Variable for creature density
  double creature_density_;
  double friction_coefficient_;
};

#endif  // ENVIRONMENT_H
