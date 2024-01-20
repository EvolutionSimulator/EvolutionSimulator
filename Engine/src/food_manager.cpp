#include "../include/food_manager.h"

#include "settings.h"

FoodManager::FoodManager() {}

/*!
 * @brief Initializes food entities randomly on the map.
 */
void FoodManager::InitializeFood(SimulationData &data,
                                 Environment &environment) {
  data.food_entities_.clear();
  for (int i = 0; i < 100; i++) {
    GenerateMoreFood(data, environment, 3);
  }
}

/*!
 * @brief Generates additional food entities based on the environment's food
 * density.
 */
void FoodManager::GenerateMoreFood(SimulationData &data,
                                   Environment &environment, double deltaTime) {
  double spawn_cell_size = 50.0;
  for (double x = 0; x < SETTINGS.environment.map_width; x += spawn_cell_size) {
    for (double y = 0; y < SETTINGS.environment.map_height; y += spawn_cell_size) {
      double food_density = environment.GetFoodDensity(x, y);
      double food_spawn_probability =
          food_density * spawn_cell_size * spawn_cell_size *
          SETTINGS.environment.food_spawn_rate * deltaTime;

      double random_number = static_cast<double>(rand()) / RAND_MAX;
      if (random_number < food_spawn_probability) {
        double x_pos =
            x + static_cast<double>(rand()) / RAND_MAX * spawn_cell_size;
        double y_pos =
            y + static_cast<double>(rand()) / RAND_MAX * spawn_cell_size;
        // Use std::make_shared to create a shared pointer to a new Plant
        auto plant = std::make_shared<Plant>(x_pos, y_pos);
        data.food_entities_.push_back(plant);
      }
    }
  }
}
