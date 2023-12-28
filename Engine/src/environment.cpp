// Environment.cpp

#include "environment.h"

#include <random>

// Constructor implementation
myEnvironment::Environment::Environment()
    : food_density_(settings::environment::kDefaultFoodDensity),
      creature_density_(settings::environment::kDefaultCreatureDensity),
      friction_coefficient_(settings::environment::kFrictionalCoefficient),
      map_width_(settings::environment::kDMapWidth),
      map_height_(settings::environment::kDMapHeight) {}

myEnvironment::Environment::Environment(double width, double height)
    : food_density_(settings::environment::kDefaultFoodDensity),
      creature_density_(settings::environment::kDefaultCreatureDensity),
      friction_coefficient_(settings::environment::kFrictionalCoefficient),
      map_width_(width),
      map_height_(height) {}
