#include "../include/food_manager.h"

FoodManager::FoodManager() {}

/*!
 * @brief Generates additional food entities based on the environment's food
 * density.
 */
void FoodManager::InitializeFood(SimulationData &data, Environment &environment)
{
  double size = data.food_entities_.size();
  double max_number = environment.GetFoodDensity() *
                      SETTINGS.environment.map_height *
                      SETTINGS.environment.map_width / 100;
  while (size < max_number) {
    Plant new_food = Plant();
    new_food.RandomInitialization(SETTINGS.environment.map_width,
                                  SETTINGS.environment.map_height,
                                  SETTINGS.environment.max_food_size,
                                  SETTINGS.environment.min_creature_size);
    data.food_entities_.emplace_back(new_food);
    size++;
  }
}

/*!
 * @brief Generates additional food entities based on the environment's food
 * density.
 */

void FoodManager::GenerateMoreFood(SimulationData &data, Environment &environment)
{
    double size = data.food_entities_.size();
    double max_number = environment.GetFoodDensity() *
                        SETTINGS.environment.map_height *
                        SETTINGS.environment.map_width / 100;

    while (size < max_number) {
      Plant new_food = Plant();
      new_food.RandomInitialization(SETTINGS.environment.map_width,
                                    SETTINGS.environment.map_height,
                                    SETTINGS.environment.max_food_size,
                                    SETTINGS.environment.min_creature_size);

      data.food_entities_.emplace_back(new_food);
      size++;
    }
}
