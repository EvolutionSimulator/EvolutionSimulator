// Environment.cpp

#include "environment.h"

#include <random>

Environment::Environment()
    : food_density_(SETTINGS.environment.default_food_density),
      creature_density_(SETTINGS.environment.default_creature_density),
      friction_coefficient_(SETTINGS.environment.frictional_coefficient) {}
