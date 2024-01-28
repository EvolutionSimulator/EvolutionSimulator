// Environment.cpp

#include "environment.h"
#include "settings.h"

// Constructor implementation
Environment::Environment()
    : creature_density_(SETTINGS.environment.default_creature_density),
      friction_coefficient_(SETTINGS.environment.frictional_coefficient) {

    food_density_func_ = [](double x, double y) {
        // Mean values (center of the map)
        double mean_x = SETTINGS.environment.map_width / 2.0;
        double mean_y = SETTINGS.environment.map_height / 2.0;

        // Standard deviations
        double sigma_x = SETTINGS.environment.map_width / 6.0;
        double sigma_y = SETTINGS.environment.map_height / 6.0;

        // Calculate the 2D Gaussian function
        double exponent = -((std::pow((x - mean_x), 2) / (2 * std::pow(sigma_x, 2)))
                                + (std::pow((y - mean_y), 2) / (2 * std::pow(sigma_y, 2))));
        return SETTINGS.environment.default_food_density * std::exp(exponent);
    };
}

Environment::Environment(double width, double height)
    : creature_density_(SETTINGS.environment.default_creature_density),
      friction_coefficient_(SETTINGS.environment.frictional_coefficient) {
    food_density_func_ = [](double x, double y) {
        // Mean values (center of the map)
        double mean_x = SETTINGS.environment.map_width / 2.0;
        double mean_y = SETTINGS.environment.map_height / 2.0;

        // Standard deviations
        double sigma_x = SETTINGS.environment.map_width / 6.0;
        double sigma_y = SETTINGS.environment.map_height / 6.0;

        // Calculate the 2D Gaussian function
        double exponent = -((std::pow((x - mean_x), 2) / (2 * std::pow(sigma_x, 2)))
                                + (std::pow((y - mean_y), 2) / (2 * std::pow(sigma_y, 2))));
        return SETTINGS.environment.default_food_density * std::exp(exponent);
    };
}

void Environment::SetFoodDensity(double density)
{
    food_density_func_ = [density](double x, double y) {
        // Mean values (center of the map)
        double mean_x = SETTINGS.environment.map_width / 2.0;
        double mean_y = SETTINGS.environment.map_height / 2.0;

        // Standard deviations
        double sigma_x = SETTINGS.environment.map_width / 3.0;
        double sigma_y = SETTINGS.environment.map_height / 3.0;

        // Calculate the 2D Gaussian function
        double exponent = -((std::pow((x - mean_x), 2) / (2 * std::pow(sigma_x, 2)))
                                + (std::pow((y - mean_y), 2) / (2 * std::pow(sigma_y, 2))));
        return density * std::exp(exponent); };
}

void Environment::SetFoodDensity(std::function<double(double, double)> density)
{
    food_density_func_ = density;
}

double Environment::GetFoodDensity(double x, double y)
{
    return food_density_func_(x, y);
}
