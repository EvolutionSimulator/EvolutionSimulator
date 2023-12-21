#include "simulationdata.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <vector>

#include "collisions.h"
#include "config.h"
#include "food.h"

/*!
 * @brief Adds a creature to the simulation.
 *
 * @param creature The Creature object to be added to the simulation's creature
 * list.
 */

void SimulationData::AddCreature(const Creature& creature) {
  creatures_.push_back(creature);
}

/*!
 * @brief Removes a specified creature from the simulation.
 *
 * @param creature The Creature object to be removed from the simulation's
 * creature list.
 */

void SimulationData::RemoveCreature(const Creature& creature) {
  // Use std::remove_if with a lambda function as the predicate
  creatures_.erase(std::remove_if(creatures_.begin(), creatures_.end(),
                                  [&creature](const Creature& e) {
                                    return &e == &creature;
                                  }),
                   creatures_.end());
}

/*!
 * @brief Retrieves the current environment of the simulation.
 *
 * @return Returns the simulation's current environment.
 */

myEnvironment::Environment SimulationData::GetEnvironment() {
  return environment_;
}

/*!
 * @brief Sets the environment for the simulation.
 *
 * @param environment The new environment to be set for the simulation.
 */

void SimulationData::SetEnvironment(myEnvironment::Environment& environment) {
  environment_ = environment;
}

/*!
 * @brief Retrieves the simulation's grid.
 *
 * @return Returns a 3D vector representing the simulation's spatial grid.
 */

std::vector<std::vector<std::vector<Entity*>>> SimulationData::GetGrid() {
  return grid_;
}

/*!
 * @brief Modifies the positions of all creatures in the simulation.
 *
 * @param delta_x The change in x-coordinate for each creature.
 * @param delta_y The change in y-coordinate for each creature.
 */

void SimulationData::ModifyAllCreatures(double delta_x, double delta_y) {
  std::pair<double, double> coordinates;
  for (Creature& creature : creatures_) {
    coordinates = creature.GetCoordinates();
    coordinates.first = coordinates.first + delta_x;
    coordinates.second = coordinates.second + delta_y;
    creature.SetCoordinates(coordinates.first, coordinates.second,
                            settings::environment::kMapWidth,
                            settings::environment::kMapHeight);
  }
}

/*!
 * @brief Updates the state of all creatures for a given time interval.
 *
 * @param deltaTime The time interval for which the creatures' states are
 * updated.
 */

void SimulationData::UpdateAllCreatures(double deltaTime) {
  for (Creature& creature : creatures_) {
    creature.Update(deltaTime, settings::environment::kMapWidth,
                    settings::environment::kMapHeight, grid_,
                    settings::environment::kGridCellSize);
    if (creature.Fit()) {
      reproduce_.push(creature);
    }
  }
}

/*!
 * @brief Generates additional food entities based on the environment's food
 * density.
 */
void SimulationData::GenerateMoreFood() {
  double size = food_entities_.size();
  double max_number = environment_.GetFoodDensity() *
                      settings::environment::kMapHeight *
                      settings::environment::kMapWidth / 100;
  while (size < max_number) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0); // Define a distribution for double values between 0 and 1
    double randomValue = dis(gen);
    if (randomValue<settings::environment::kPlantProportion){
      Food new_food = Plant();
      new_food.RandomInitialization(settings::environment::kMapWidth,
                                    settings::environment::kMapHeight,
                                    settings::environment::kMaxFoodSize,
                                    settings::environment::kMinCreatureSize);
      food_entities_.emplace_back(new_food);
    }
    else{
      Food new_food = Meat();
      new_food.RandomInitialization(settings::environment::kMapWidth,
                                    settings::environment::kMapHeight,
                                    settings::environment::kMaxFoodSize,
                                    settings::environment::kMinCreatureSize);
      food_entities_.emplace_back(new_food);
    }
    size++;
  }
}

/*!
 * @brief Handles the reproduction process of creatures in the simulation.
 *
 * @details Pairs creatures from the reproduction queue and creates offspring
 * with crossed-over genomes.
 */
void SimulationData::ReproduceCreatures() {
  double world_width = settings::environment::kMapWidth;
  double world_height = settings::environment::kMapHeight;
  double max_creature_size = settings::environment::kMaxCreatureSize;
  double min_creature_size = settings::environment::kMinCreatureSize;
  while (reproduce_.size() > 2) {
    Creature creature1 = reproduce_.front();
    reproduce_.pop();
    Creature creature2 = reproduce_.front();
    reproduce_.pop();
    double energy1 = creature1.GetEnergy();
    double energy2 = creature2.GetEnergy();
    if (energy1 > energy2) {
      neat::Genome new_genome =
          neat::Crossover(creature1.GetGenome(), creature2.GetGenome());
      new_genome.Mutate();
      new_genome.Mutate();
      Creature new_creature(new_genome);
      new_creature.RandomInitialization(world_width, world_height,
                                        max_creature_size, min_creature_size);
      new_creature.SetGeneration(creature1.GetGeneration() + 1);
      AddCreature(new_creature);
    } else {
      neat::Genome new_genome =
          neat::Crossover(creature2.GetGenome(), creature1.GetGenome());
      new_genome.Mutate();
      new_genome.Mutate();
      Creature new_creature(new_genome);
      new_creature.RandomInitialization(world_width, world_height,
                                        max_creature_size, min_creature_size);
      new_creature.SetGeneration(creature2.GetGeneration() + 1);
      AddCreature(new_creature);
    }
    creature1.Reproduced();
    creature2.Reproduced();
  }
}

/*!
 * @brief Initializes creatures randomly on the map, mutating their genome
 * multiple times.
 */
void SimulationData::InitializeCreatures() {
  // Retrieve information from the environment
  double world_width = settings::environment::kMapWidth;
  double world_height = settings::environment::kMapHeight;
  double creature_density = settings::environment::kCreatureDensity;
  double max_creature_size = settings::environment::kMaxCreatureSize;
  double min_creature_size = settings::environment::kMinCreatureSize;

  creatures_.clear();

  for (double x = 0; x < world_width; x += 2.0) {
    for (double y = 0; y < world_height; y += 2.0) {
      if (std::rand() / (RAND_MAX + 1.0) < creature_density) {
        neat::Genome genome(settings::environment::kInputNeurons,
                            settings::environment::kOutputNeurons);
        for (int i = 0; i < 40; i++) {
          genome.Mutate();
        }
        Creature new_creature(genome);
        new_creature.RandomInitialization(world_width, world_height,
                                          max_creature_size, min_creature_size);
        creatures_.emplace_back(new_creature);
      }
    }
  }
}

/*!
 * @brief Initializes food entities randomly on the map.
 */
void SimulationData::InitializeFood() {
  double kFoodDensity = environment_.GetFoodDensity();
  food_entities_.clear();

  // Populate the vector with food entities based on the current food density
  for (double x = 0; x < settings::environment::kMapWidth; x += 10.0) {
    for (double y = 0; y < settings::environment::kMapHeight; y += 10.0) {
      if (std::rand() / (RAND_MAX + 1.0) < kFoodDensity) {
        // Get a random double between 0 and 1
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0.0, 1.0); // Define a distribution for double values between 0 and 1
        double randomValue = dis(gen);
        if (randomValue<settings::environment::kPlantProportion){
           food_entities_.emplace_back(Plant(x, y));
        }
        else{
           food_entities_.emplace_back(Meat(x, y));
        }
      }
    }
  }
}

/*!
 * @brief Initializes the simulation grid and places entities within it.
 */
void SimulationData::InitializeGrid() {
  // Number of grid cells
  int num_cells_x = static_cast<int>(std::ceil(
                        static_cast<double>(settings::environment::kMapWidth) /
                        settings::environment::kGridCellSize)) +
                    1;
  int num_cells_y = static_cast<int>(std::ceil(
                        static_cast<double>(settings::environment::kMapHeight) /
                        settings::environment::kGridCellSize)) +
                    1;
  // Resize the grid to the specified dimensions
  grid_.assign(num_cells_x, std::vector<std::vector<Entity*>>(num_cells_y));

  UpdateGrid();
}

/*!
 * @brief Clears the simulation grid of all entities.
 */
void SimulationData::ClearGrid() {
  for (auto& row : grid_) {
    for (auto& cell : row) {
      cell.clear();
    }
  }
}

/*!
 * @brief Template function that erases the dead entities from their
 * corresponding vectors and fills the grid with the remaining entities.
 *
 * @tparam entities Vector of EntityType.
 * @param entityGrid 3D vector of entities.
 * @param cellSize Size of the grid cells.
 */
template <typename EntityType>
void UpdateGridTemplate(
    std::vector<EntityType>& entities,
    std::vector<std::vector<std::vector<Entity*>>>& entityGrid,
    double cellSize) {
  entities.erase(std::remove_if(entities.begin(), entities.end(),
                                [](const EntityType& entity) {
                                  return entity.GetState() != Entity::Alive;
                                }),
                 entities.end());

  for (EntityType& entity : entities) {
    std::pair<double, double> coordinates = entity.GetCoordinates();
    int gridX = static_cast<int>(coordinates.first / cellSize);
    int gridY = static_cast<int>(coordinates.second / cellSize);

    entityGrid[gridX][gridY].push_back(&entity);
  }
}

/*!
 * @brief Removes creatures with a state of 'Dead' from a given queue.
 *
 * @param reproduce A queue of Creature objects, potentially containing dead
 * creatures.
 */
void UpdateQueue(std::queue<Creature>& reproduce) {
  std::queue<Creature> tempQueue;

  while (!reproduce.empty()) {
    Creature currentCreature = reproduce.front();
    reproduce.pop();

    // Check if the current creature is not dead
    if (currentCreature.GetState() != Entity::Dead) {
      tempQueue.push(currentCreature);
    }
  }

  // Replace the original queue with the temporary queue
  // containing all living creatures
  reproduce = std::move(tempQueue);
}

/*!
 * @brief Updates the simulation grid, removing dead entities and placing the
 * living ones.
 */
void SimulationData::UpdateGrid() {
  ClearGrid();
  UpdateGridTemplate<Creature>(creatures_, grid_,
                               settings::environment::kGridCellSize);
  UpdateGridTemplate<Food>(food_entities_, grid_,
                           settings::environment::kGridCellSize);
  UpdateQueue(reproduce_);
}

/*!
 * @brief Retrieves neighboring cells in the grid, including the center cell
 * itself.
 *
 * @param num_rows Number of rows in the grid.
 * @param num_cols Number of columns in the grid.
 * @param center The center cell coordinates.
 * @param layer_number The number of layers around the center cell to include.
 *
 * @return A vector of pairs representing the coordinates of neighboring cells.
 */
std::vector<std::pair<int, int>> GetNeighbours(
    const int& num_rows, const int& num_cols, const std::pair<int, int>& center,
    const int& layer_number) {
  std::vector<std::pair<int, int>> neighbours;
  for (int y = center.first - layer_number; y < center.first + layer_number + 1;
       y++) {
    for (int x = center.second - layer_number;
         x < center.second + layer_number + 1; x++) {
      neighbours.push_back(
          std::make_pair((y + num_rows) % num_rows, (x + num_cols) % num_cols));
    }
  }
  return neighbours;
}

/*!
 * @brief Checks for collisions between entities in the simulation.
 *
 * @details Iterates through the grid to detect and handle collisions between
 * different entities.
 */
void SimulationData::CheckCollisions() {
  double tolerance = settings::environment::kTolerance;
  int num_rows = static_cast<int>(std::ceil(
                     static_cast<double>(settings::environment::kMapWidth) /
                     settings::environment::kGridCellSize)) +
                 1;
  int num_cols = static_cast<int>(std::ceil(
                     static_cast<double>(settings::environment::kMapHeight) /
                     settings::environment::kGridCellSize)) +
                 1;
  for (int row = 0; row < num_rows; row++) {
    for (int col = 0; col < num_cols; col++) {
      for (Entity* entity1 : grid_[row][col]) {
        const int layer_number =
            2 *
            ceil((entity1->GetSize() / settings::environment::kGridCellSize));
        std::vector<std::pair<int, int>> neighbours =
            GetNeighbours(num_rows, num_cols, {row, col}, layer_number);
        for (const std::pair<int, int> neighbour : neighbours) {
          for (Entity* entity2 : grid_[neighbour.first][neighbour.second]) {
            if (entity1->CheckCollisionWithEntity(tolerance, *entity2)) {
              if (entity1 != entity2) {
                entity1->OnCollision(*entity2, settings::environment::kMapWidth,
                                     settings::environment::kMapHeight);
              }
            }
          }
        }
      }
    }
  }
}
