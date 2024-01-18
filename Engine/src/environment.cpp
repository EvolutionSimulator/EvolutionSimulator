// Environment.cpp

#include "environment.h"
#include <random>
#include "settings.h"

// Constructor implementation
Environment::Environment()
    : food_density_func_(
      [](double x, double y) { return SETTINGS.environment.default_food_density; }),
      creature_density_(SETTINGS.environment.default_creature_density),
      friction_coefficient_(SETTINGS.environment.frictional_coefficient),
      map_width_(SETTINGS.environment.map_width),
      map_height_(SETTINGS.environment.map_height) {}

Environment::Environment(double width, double height)
    : food_density_func_(
          [](double x, double y) { return SETTINGS.environment.default_food_density; }),
      creature_density_(SETTINGS.environment.default_creature_density),
      friction_coefficient_(SETTINGS.environment.frictional_coefficient),
      map_width_(width),
      map_height_(height) {}

void Environment::SetFoodDensity(double density)
{
    food_density_func_ = [density](double x, double y) { return density; };
}

void Environment::SetFoodDensity(std::function<double(double, double)> density)
{
    food_density_func_ = density;
}

double Environment::GetFoodDensity(double x, double y)
{
    return food_density_func_(x, y);
}
