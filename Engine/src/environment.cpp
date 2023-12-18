// Environment.cpp

#include "environment.h"

#include <random>

// Constructor implementation
myEnvironment::Environment::Environment()
    : food_density_(settings::environment::kDefaultFoodDensity),
      creature_density_(settings::environment::kDefaultCreatureDensity) {}
