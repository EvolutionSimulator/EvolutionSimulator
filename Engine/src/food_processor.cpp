#include "../include/food_processor.h"

FoodProcessor::FoodProcessor() {}

/*!
 * @brief Generates additional food entities based on the environment's food
 * density.
 */
void FoodProcessor::InitializeFood(SimulationData &data, Environment &environment)
{
  double size = data.food_entities_.size();
  double max_number = environment.GetFoodDensity() *
                      SETTINGS.environment.map_height *
                      SETTINGS.environment.map_width / 100;
  while (size < max_number) {
    Food new_food = Food();
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

void FoodProcessor::GenerateMoreFood(SimulationData &data, Environment &environment)
{
    double size = data.food_entities_.size();
    double max_number = environment.GetFoodDensity() *
                        SETTINGS.environment.map_height *
                        SETTINGS.environment.map_width / 100;

    while (size < max_number) {
      Food new_food = Food();
      new_food.RandomInitialization(SETTINGS.environment.map_width,
                                    SETTINGS.environment.map_height,
                                    SETTINGS.environment.max_food_size,
                                    SETTINGS.environment.min_creature_size);

      data.food_entities_.emplace_back(new_food);
      size++;
    }
}
