// Environment.cpp

#include "environment.h"

#include <random>

// Constructor implementation
Environment::Environment()
    : food_density_(SETTINGS.environment.default_food_density),
      creature_density_(SETTINGS.environment.default_creature_density) {}
