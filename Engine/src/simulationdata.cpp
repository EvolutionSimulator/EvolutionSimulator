#include "simulationdata.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <vector>

#include "collisions.h"
#include "config.h"
#include "egg.h"
#include "food.h"
#include "mathlib.h"

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


void SimulationData::RemoveCreature(const Creature& creature) {
  // Use std::remove_if with a lambda function as the predicate
  creatures_.erase(std::remove_if(creatures_.begin(), creatures_.end(),
                                  [&creature](const Creature& e) {
                                    return &e == &creature;
                                  }),
                   creatures_.end());
}
*/
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
                            environment_.GetMapWidth(),
                            environment_.GetMapHeight());
  }
}

/*!
 * @brief Updates the state of all creatures for a given time interval.
 *
 * @param deltaTime The time interval for which the creatures' states are
 * updated.
 */

void SimulationData::UpdateAllCreatures(double deltaTime) {
  for (Egg& egg : eggs_) {
    egg.Update(deltaTime);
  }

  for (int creature_index = 0; creature_index < creatures_.size();
       creature_index++) {
    Creature& creature = creatures_[creature_index];
    creature.Update(deltaTime, environment_.GetMapWidth(),
                    environment_.GetMapHeight(), grid_,
                    settings::environment::kGridCellSize,
                    environment_.GetFrictionalCoefficient());
    if (creature.GetMatingDesire()) {
      new_reproduce_.push(creature_index);
    }

    if (creature.GetFemaleReproductiveSystem()->CanBirth()) {
      AddEgg(creature.GetFemaleReproductiveSystem()->GiveBirth());
      creature.SetVelocity(
          creature.GetVelocity() *
          settings::physical_constraints::kAfterBirthVelocityFactor);
    }
  }

  world_time_ += deltaTime;

  // Store the creature count when the time increases by a second
  if (static_cast<int>(world_time_) > static_cast<int>(lastRecordedTime_)) {
    lastRecordedTime_ = world_time_;
    creatureCountOverTime_.push_back(creatures_.size());
  }
}

/*!
 * @brief Generates additional food entities based on the environment's food
 * density.
 */
void SimulationData::GenerateMoreFood() {
  double size = food_entities_.size();
  double max_number = environment_.GetFoodDensity() *
                      environment_.GetMapHeight() * environment_.GetMapWidth() /
                      100;
  if (creatures_.size() >
      300) {  // temporary fix so that an abnormal number of creatures can't
              // abuse the food generation system
    return;
  }
  while (size < max_number) {
    Plant new_food = Plant();
    new_food.RandomInitialization(environment_.GetMapWidth(),
                                  environment_.GetMapHeight(),
                                  settings::environment::kMaxFoodSize,
                                  settings::environment::kMinCreatureSize);
    food_entities_.emplace_back(new_food);
    size++;
  }
}

void SimulationData::AddEgg(const Egg& egg) {
  eggs_.push_back(egg);
}

void SimulationData::HatchEggs() {
  std::remove_if(eggs_.begin(), eggs_.end(), [this](Egg& egg) {
    if (egg.GetAge() >= egg.GetIncubationTime()) {
      Creature new_creature = egg.Hatch();
      new_creature.RandomInitialization(environment_.GetMapWidth(),
                                        environment_.GetMapHeight());
      AddCreature(new_creature);
      return true;
    }
    return false;
  });
}

/*!
 * @brief Handles the reproduction process of creatures in the simulation.
 *
 * @details Pairs creatures by compatibility from the reproduction queue and
 * creates offspring with crossed-over genomes.
 */
void SimulationData::ReproduceCreatures() {
  double world_width = environment_.GetMapWidth();
  double world_height = environment_.GetMapHeight();
  double max_creature_size = settings::environment::kMaxCreatureSize;
  double min_creature_size = settings::environment::kMinCreatureSize;

  std::queue<int> not_reproduced;
  std::queue<int> temp_queue;

  while (!reproduce_.empty()) {
    int creature1_index = reproduce_.front();
    Creature& creature1 = creatures_[creature1_index];
    reproduce_.pop();
    bool paired = false;

    // Attempt to pair creature1 with a compatible new creature
    while (!new_reproduce_.empty() && !paired) {
      int creature2_index = new_reproduce_.front();
      Creature& creature2 = creatures_[creature2_index];
      new_reproduce_.pop();
      // If these two creatures are compatible reproduce them otherwise
      // save the creature in a temporary queue for the next pairing round
      if (creature1.Compatible(creature2) and
          creature1.GetMaleReproductiveSystem()->ReadyToProcreate() and
          creature2.GetFemaleReproductiveSystem()->ReadyToProcreate()) {
        ReproduceTwoCreatures(creature1, creature2);
        paired = true;
      } else {
        temp_queue.push(creature2_index);  // Save for next round
      }
    }

    // If the creature wasn't paired add it to not_reproduced
    if (!paired) {
      not_reproduced.push(creature1_index);
    }

    // Refill newCreatures with unpaired creatures for next attempt
    while (!temp_queue.empty()) {
      new_reproduce_.push(temp_queue.front());
      temp_queue.pop();
    }
  }

  // Refill reproduce_ with creatures for the next iteration
  while (!new_reproduce_.empty()) {
    reproduce_.push(new_reproduce_.front());
    new_reproduce_.pop();
  }

  // Refill reproduce_ with the remaining creatures
  while (!not_reproduced.empty()) {
    reproduce_.push(not_reproduced.front());
    not_reproduced.pop();
  }
}

/*!
 *  @brief Reproduces two creatures and adds a descendant to the simulation
 *
 *  @details Takes two creatures and uses the crossover functions for genomes
 * and mutables to create a child creature out of the previous two. We take as
 * the dominant creature for the algorithms that that has the highest energy at
 * the moment of reproduction
 */
void SimulationData::ReproduceTwoCreatures(Creature& father, Creature& mother) {
  father.GetMaleReproductiveSystem()->MateWithFemale();
  mother.GetFemaleReproductiveSystem()->MateWithMale(&father, &mother);
  father.AfterMate();
  mother.AfterMate();
}

/*!
 * @brief Initializes creatures randomly on the map, mutating their genome
 * multiple times.
 */
void SimulationData::InitializeCreatures() {
  // Retrieve information from the environment
  double world_width = environment_.GetMapWidth();
  double world_height = environment_.GetMapHeight();
  double creature_density = environment_.GetCreatureDensity();
  double max_creature_size = settings::environment::kMaxCreatureSize;
  double min_creature_size = settings::environment::kMinCreatureSize;

  creatures_.clear();
  neat::Genome genome = neat::minimallyViableGenome();
  for (double x = 0; x < world_width; x += 2.0) {
    for (double y = 0; y < world_height; y += 2.0) {
      if (std::rand() / (RAND_MAX + 1.0) < creature_density) {
        // neat::Genome genome(settings::environment::kInputNeurons,
        //                     settings::environment::kOutputNeurons);
        Mutable mutables;
        for (int i = 0; i < 40; i++) {
          mutables.Mutate();
        }
        Creature new_creature(genome, mutables);
        new_creature.RandomInitialization(world_width, world_height);
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
  for (double x = 0; x < environment_.GetMapWidth(); x += 10.0) {
    for (double y = 0; y < environment_.GetMapHeight(); y += 10.0) {
      if (std::rand() / (RAND_MAX + 1.0) < kFoodDensity) {
        food_entities_.emplace_back(Plant(x, y));
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
                        static_cast<double>(environment_.GetMapWidth()) /
                        settings::environment::kGridCellSize)) +
                    1;
  int num_cells_y = static_cast<int>(std::ceil(
                        static_cast<double>(environment_.GetMapHeight()) /
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
 * @brief Function that erases the eaten food from their
 * corresponding vectors and fills the grid with the remaining food.
 *
 * @param food Vector of type Food.
 * @param entityGrid 3D vector of entities.
 * @param cellSize Size of the grid cells.
 */

void UpdateGridFood(std::vector<Food>& foods,
                    std::vector<std::vector<std::vector<Entity*>>>& entityGrid,
                    double cellSize) {
  foods.erase(std::remove_if(foods.begin(), foods.end(),
                             [](const Food& food) {
                               return food.GetState() != Entity::Alive;
                             }),
              foods.end());

  for (Food& entity : foods) {
    std::pair<double, double> coordinates = entity.GetCoordinates();
    int gridX = static_cast<int>(coordinates.first / cellSize);
    int gridY = static_cast<int>(coordinates.second / cellSize);

    entityGrid[gridX][gridY].push_back(&entity);
  }
}

/*!
 * @brief Function that turns the dead creatures to meat from their
 * corresponding vectors and fills the grid with the remaining entities.
 *
 * @param creatures Vector of type Creature.
 * @param entityGrid 3D vector of entities.
 * @param cellSize Size of the grid cells.
 * @param food Vector of type Food.
 */

void UpdateGridCreature(
    std::vector<Creature>& creatures,
    std::vector<std::vector<std::vector<Entity*>>>& entityGrid, double cellSize,
    std::vector<Food>& food) {
  creatures.erase(
      std::remove_if(creatures.begin(), creatures.end(),
                     [&food](const Creature& entity) {
                       if (entity.GetState() == Entity::Dead) {
                         food.emplace_back(Meat(entity.GetCoordinates().first,
                                                entity.GetCoordinates().second,
                                                entity.GetSize()));
                       }
                       return entity.GetState() != Entity::Alive;
                     }),
      creatures.end());
  for (Creature& creature : creatures) {
    std::pair<double, double> coordinates = creature.GetCoordinates();
    int gridX = static_cast<int>(coordinates.first / cellSize);
    int gridY = static_cast<int>(coordinates.second / cellSize);

    entityGrid[gridX][gridY].push_back(&creature);
  }
}

/*!
 * @brief Removes creatures with a state of 'Dead' from a given queue.
 *
 * @param reproduce A queue of Creature objects, potentially containing dead
 * creatures.
 */
void UpdateQueue(std::queue<int>& reproduce, const std::vector<Creature>& creatures) {
  std::queue<int> tempQueue;

  while (!reproduce.empty()) {
    int current_creature_index = reproduce.front();
    reproduce.pop();

    // Check if the current creature is not dead
    if (creatures[current_creature_index].GetState() != Entity::Dead) {
      tempQueue.push(current_creature_index);
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
  UpdateGridCreature(creatures_, grid_, settings::environment::kGridCellSize,
                     food_entities_);
  UpdateGridFood(food_entities_, grid_, settings::environment::kGridCellSize);
  UpdateQueue(reproduce_, creatures_);
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
 * @return A vector of pairs representing the coordinates of neighboring
 * cells.
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
  int num_rows = static_cast<int>(
                     std::ceil(static_cast<double>(environment_.GetMapWidth()) /
                               settings::environment::kGridCellSize)) +
                 1;
  int num_cols = static_cast<int>(std::ceil(
                     static_cast<double>(environment_.GetMapHeight()) /
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
                entity1->OnCollision(*entity2, environment_.GetMapWidth(),
                                     environment_.GetMapHeight());
              }
            }
          }
        }
      }
    }
  }
}

std::vector<int> SimulationData::GetCreatureCountOverTime() const {
  return creatureCountOverTime_;
}
