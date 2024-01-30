// Environment.cpp

#include "environment.h"
#include "settings.h"

// Constructor implementation
Environment::Environment()
    : creature_density_(SETTINGS.environment.default_creature_density),
      friction_coefficient_(SETTINGS.environment.frictional_coefficient) {

    food_density_func_ = [](double x, double y) {
          // Mean values for the first peak (center of the map)
          double mean_x1 = SETTINGS.environment.map_width / 2.0;
          double mean_y1 = SETTINGS.environment.map_height / 2.0;

          // Mean values for the second peak (opposite side of the map)
          double mean_x2 = (mean_x1 + SETTINGS.environment.map_width / 2.0);
          double mean_y2 = (mean_y1 + SETTINGS.environment.map_height / 2.0);

          // Standard deviations (smaller values will make the peaks less spread out)
          double sigma_x = SETTINGS.environment.map_width / 10.0;
          double sigma_y = SETTINGS.environment.map_height / 10.0;

          // Adjust mean_x2 and mean_y2 for toroidal wrapping
          if (mean_x2 >= SETTINGS.environment.map_width) mean_x2 -= SETTINGS.environment.map_width;
          if (mean_y2 >= SETTINGS.environment.map_height) mean_y2 -= SETTINGS.environment.map_height;

          // Function to calculate Gaussian based on toroidal distance
          auto gaussian = [sigma_x, sigma_y](double mx, double my, double x, double y, double width, double height) {
            // Toroidal distance calculations
            double dx = std::min(std::abs(x - mx), width - std::abs(x - mx));
            double dy = std::min(std::abs(y - my), height - std::abs(y - my));

            // Gaussian function calculation
            double exponent = -((dx * dx) / (2 * sigma_x * sigma_x) + (dy * dy) / (2 * sigma_y * sigma_y));
            return std::exp(exponent);
          };

          // Sum the densities of the two peaks
          double density1 = gaussian(mean_x1, mean_y1, x, y, SETTINGS.environment.map_width, SETTINGS.environment.map_height);
          double density2 = gaussian(mean_x2, mean_y2, x, y, SETTINGS.environment.map_width, SETTINGS.environment.map_height);

          return SETTINGS.environment.default_food_density * (density1 + density2);
        };
}

Environment::Environment(double width, double height)
    : creature_density_(SETTINGS.environment.default_creature_density),
      friction_coefficient_(SETTINGS.environment.frictional_coefficient) {
    food_density_func_ = [](double x, double y) {
          // Mean values for the first peak (center of the map)
          double mean_x1 = SETTINGS.environment.map_width / 2.0;
          double mean_y1 = SETTINGS.environment.map_height / 2.0;

          // Mean values for the second peak (opposite side of the map)
          double mean_x2 = (mean_x1 + SETTINGS.environment.map_width / 2.0);
          double mean_y2 = (mean_y1 + SETTINGS.environment.map_height / 2.0);

          // Standard deviations (smaller values will make the peaks less spread out)
          double sigma_x = SETTINGS.environment.map_width / 10.0;
          double sigma_y = SETTINGS.environment.map_height / 10.0;

          // Adjust mean_x2 and mean_y2 for toroidal wrapping
          if (mean_x2 >= SETTINGS.environment.map_width) mean_x2 -= SETTINGS.environment.map_width;
          if (mean_y2 >= SETTINGS.environment.map_height) mean_y2 -= SETTINGS.environment.map_height;

          // Function to calculate Gaussian based on toroidal distance
          auto gaussian = [sigma_x, sigma_y](double mx, double my, double x, double y, double width, double height) {
            // Toroidal distance calculations
            double dx = std::min(std::abs(x - mx), width - std::abs(x - mx));
            double dy = std::min(std::abs(y - my), height - std::abs(y - my));

            // Gaussian function calculation
            double exponent = -((dx * dx) / (2 * sigma_x * sigma_x) + (dy * dy) / (2 * sigma_y * sigma_y));
            return std::exp(exponent);
          };

          // Sum the densities of the two peaks
          double density1 = gaussian(mean_x1, mean_y1, x, y, SETTINGS.environment.map_width, SETTINGS.environment.map_height);
          double density2 = gaussian(mean_x2, mean_y2, x, y, SETTINGS.environment.map_width, SETTINGS.environment.map_height);

          return SETTINGS.environment.default_food_density * (density1 + density2);
    };
}

void Environment::SetFoodDensity(double density)
{
    food_density_func_ = [density](double x, double y) {
          // Mean values for the first peak (center of the map)
          double mean_x1 = SETTINGS.environment.map_width / 2.0;
          double mean_y1 = SETTINGS.environment.map_height / 2.0;

          // Mean values for the second peak (opposite side of the map)
          double mean_x2 = (mean_x1 + SETTINGS.environment.map_width / 2.0);
          double mean_y2 = (mean_y1 + SETTINGS.environment.map_height / 2.0);

          // Standard deviations (smaller values will make the peaks less spread out)
          double sigma_x = SETTINGS.environment.map_width / 10.0;
          double sigma_y = SETTINGS.environment.map_height / 10.0;

          // Adjust mean_x2 and mean_y2 for toroidal wrapping
          if (mean_x2 >= SETTINGS.environment.map_width) mean_x2 -= SETTINGS.environment.map_width;
          if (mean_y2 >= SETTINGS.environment.map_height) mean_y2 -= SETTINGS.environment.map_height;

          // Function to calculate Gaussian based on toroidal distance
          auto gaussian = [sigma_x, sigma_y](double mx, double my, double x, double y, double width, double height) {
            // Toroidal distance calculations
            double dx = std::min(std::abs(x - mx), width - std::abs(x - mx));
            double dy = std::min(std::abs(y - my), height - std::abs(y - my));

            // Gaussian function calculation
            double exponent = -((dx * dx) / (2 * sigma_x * sigma_x) + (dy * dy) / (2 * sigma_y * sigma_y));
            return std::exp(exponent);
          };

          // Sum the densities of the two peaks
          double density1 = gaussian(mean_x1, mean_y1, x, y, SETTINGS.environment.map_width, SETTINGS.environment.map_height);
          double density2 = gaussian(mean_x2, mean_y2, x, y, SETTINGS.environment.map_width, SETTINGS.environment.map_height);

          return density * (density1 + density2);
        };
}

void Environment::SetFoodDensity(std::function<double(double, double)> density)
{
    food_density_func_ = density;
}

double Environment::GetFoodDensity(double x, double y)
{
    return food_density_func_(x, y);
}
