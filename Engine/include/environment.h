#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <iostream>
#include <vector>
#include <functional>

#include "config.h"

namespace myEnvironment {
class Environment {
 public:
  // Constructor
  Environment();

  // Getter and setter for food density
  void SetFoodDensity(double density);
  void SetFoodDensity(std::function<double(double, double)> density_func);

  double GetFoodDensity(double x, double y);

  // Getter and setter for creature density
  void SetCreatureDensity(double density) { creature_density_ = density; }
  double GetCreatureDensity() const { return creature_density_; }

  void SetFrictionalCoefficient(double friction_coefficient) {friction_coefficient_ = friction_coefficient; }
  double GetFrictionalCoefficient() const { return friction_coefficient_; }

 private:
  // lambda for food density
  std::function <double(double, double)> food_density_func_;
  double creature_density_;
  double friction_coefficient_;
};
}  // namespace myEnvironment

#endif  // ENVIRONMENT_H
