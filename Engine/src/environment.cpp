// Environment.cpp

#include "environment.h"

#include <random>

// Constructor implementation
myEnvironment::Environment::Environment()
    : food_density_func_(
          [](double x, double y) { return settings::environment::kDefaultFoodDensity; }),
    creature_density_(settings::environment::kDefaultCreatureDensity),
    friction_coefficient_(settings::environment::kFrictionalCoefficient) {}

void myEnvironment::Environment::SetFoodDensity(double density)
{
    food_density_func_ = [density](double x, double y) { return density; };
}

void myEnvironment::Environment::SetFoodDensity(std::function<double(double, double)> density)
{
    food_density_func_ = density;
}

double myEnvironment::Environment::GetFoodDensity(double x, double y)
{
    return food_density_func_(x, y);
}
