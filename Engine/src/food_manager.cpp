#include "food_manager.h"

#include "settings.h"
#include "random.h"
#include <omp.h>

FoodManager::FoodManager() {}

/*!
 * @brief Initializes food entities randomly on the map.
 */
void FoodManager::InitializeFood(SimulationData &data,
                                 Environment &environment) {
  data.food_entities_.clear();
  for (int i = 0; i < 500; i++) {
    GenerateMoreFood(data, environment, 3);
  }
}

/*!
 * @brief Generates additional food entities based on the environment's food
 * density.
 */
void FoodManager::GenerateMoreFood(SimulationData &data, Environment &environment, double deltaTime) {
    double spawn_cell_size = 50.0;
    int width = SETTINGS.environment.map_width / spawn_cell_size;
    int height = SETTINGS.environment.map_height / spawn_cell_size;

    // We create a vector of vectors, each sub-vector will be used by a single thread to store new plants
    std::vector<std::vector<std::shared_ptr<Plant>>> local_plants(omp_get_max_threads());
    Random::InitializeThreadLocalEngine();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        // Get the thread ID
        int tid = omp_get_thread_num();

        double x_coord = i * spawn_cell_size;
        double y_coord = j * spawn_cell_size;
        double food_density = environment.GetFoodDensity(x_coord, y_coord);
        double food_spawn_probability =
                food_density * spawn_cell_size * spawn_cell_size *
                SETTINGS.environment.food_spawn_rate * deltaTime;

        double random_number = Random::Double(0, 1);

        if (random_number < food_spawn_probability) {
          double x_pos = x_coord + Random::Double(0, 1) * spawn_cell_size;
          double y_pos = y_coord + Random::Double(0, 1) * spawn_cell_size;
          local_plants[tid].push_back(std::make_shared<Plant>(x_pos, y_pos));
        }
      }
    }

    // Combine all local plants into the main vector
    #pragma omp critical
    {
      for (auto &thread_local_plants : local_plants) {
        data.food_entities_.insert(data.food_entities_.end(), thread_local_plants.begin(), thread_local_plants.end());
      }
    }
  }


/*!
 * @brief Updates all the food entities (meat rots) and plants grow
 *
 * @param deltaTime Time that has passed since last update
 */
void FoodManager::UpdateAllFood(SimulationData &data, double deltaTime){
#pragma omp parallel for
  for (auto food : data.food_entities_){
      food->Update(deltaTime);
  }
}
