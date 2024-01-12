// Environment.cpp

#include "environment.h"

#include <random>
#include "settings.h"

// Constructor implementation
myEnvironment::Environment::Environment()
    : food_density_(SETTINGS.environment.default_food_density),
      creature_density_(SETTINGS.environment.default_creature_density),
      friction_coefficient_(SETTINGS.environment.frictional_coefficient),
      map_width_(SETTINGS.environment.d_map_width),
      map_height_(SETTINGS.environment.d_map_height) {}

myEnvironment::Environment::Environment(double width, double height)
    : food_density_(SETTINGS.environment.default_food_density),
      creature_density_(SETTINGS.environment.default_creature_density),
      friction_coefficient_(SETTINGS.environment.frictional_coefficient),
      map_width_(width),
      map_height_(height) {}
