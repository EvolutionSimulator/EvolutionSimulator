#include "../include/entity_grid.h"

#include <cmath>
#include <vector>

#include "settings.h"

EntityGrid::EntityGrid() {
  num_columns_ = static_cast<int>(std::ceil(
                     static_cast<double>(SETTINGS.environment.map_width) /
                     SETTINGS.environment.grid_cell_size)) +
                 1;
  num_rows_ = static_cast<int>(std::ceil(
                  static_cast<double>(SETTINGS.environment.map_height) /
                  SETTINGS.environment.grid_cell_size)) +
              1;

  grid_.assign(num_columns_, std::vector<std::vector<std::shared_ptr<Entity>>>(num_rows_));
}

/*!
 * @brief Clears the simulation grid of all entities.
 */
void EntityGrid::ClearGrid() {
  for (auto &row : grid_) {
    for (auto &cell : row) {
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

void UpdateGridFood(std::vector<std::shared_ptr<Food>> &foods,
                    std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &entityGrid,
                    double cellSize) {
    foods.erase(std::remove_if(foods.begin(), foods.end(),
                               [](const std::shared_ptr<Food> food) {
                                   return food->GetState() != Entity::Alive;
                               }),
                foods.end());

    for (const auto food : foods) {
        std::pair<double, double> coordinates = food->GetCoordinates();
        int gridX = static_cast<int>(coordinates.first / cellSize);
        int gridY = static_cast<int>(coordinates.second / cellSize);

        entityGrid[gridX][gridY].push_back(food);
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
        std::vector<std::shared_ptr<Creature>> &creatures,
        std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &entityGrid,
        double cellSize, std::vector<std::shared_ptr<Food>> &food) {
    for (auto &creature : creatures) {
        if (creature->GetState() == Entity::Dead) {
            // Convert dead creatures to meat and add to the food vector
            food.push_back(std::make_shared<Meat>(creature->GetCoordinates().first,
                                                  creature->GetCoordinates().second,
                                                  creature->GetSize()));
            // Creature will be removed in the next erase-remove call
        }
    }

    creatures.erase(std::remove_if(creatures.begin(), creatures.end(),
                                    [](const std::shared_ptr<Creature> creature) {
                                        return creature->GetState() == Entity::Dead;
                                    }),
                    creatures.end());

    for (const auto creature : creatures) {
        std::pair<double, double> coordinates = creature->GetCoordinates();
        int gridX = static_cast<int>(coordinates.first / cellSize);
        int gridY = static_cast<int>(coordinates.second / cellSize);

        entityGrid[gridX][gridY].push_back(creature);
    }
}

/*!
 * @brief Removes creatures with a state of 'Dead' from a given queue.
 *
 * @param reproduce A queue of Creature objects, potentially containing dead
 * creatures.
 */
void UpdateQueue(std::queue<std::shared_ptr<Creature>> &reproduce) {
    std::queue<std::shared_ptr<Creature>> tempQueue;

    while (!reproduce.empty()) {
        auto currentCreature = reproduce.front();
        reproduce.pop();

        if (currentCreature->GetState() != Entity::Dead) {
            tempQueue.push(currentCreature);
        }
        // Dead creatures are not re-added to the queue and will be destroyed
    }

    reproduce = std::move(tempQueue);
}

void UpdateGridPheromones(
        std::vector<std::shared_ptr<Pheromone>> &pheromones,
        std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>>& entityGrid,
        double cellSize,
        double deltaTime){

    for (std::shared_ptr<Pheromone> pheromone : pheromones){
        pheromone->SetSize(pheromone->GetSize() - deltaTime/5);
        if (pheromone->GetSize() < 0.5){
            pheromone->SetState(Entity::Dead);
        }
    }

    pheromones.erase(std::remove_if(pheromones.begin(), pheromones.end(),
                               [](const std::shared_ptr<Pheromone> pheromone) {
                                   return pheromone->GetState() == Entity::Dead;
                               }),
                pheromones.end());
    for (std::shared_ptr<Pheromone> pheromone: pheromones) {
        std::pair<double, double> coordinates = pheromone->GetCoordinates();
        int gridX = static_cast<int>(coordinates.first / cellSize);
        int gridY = static_cast<int>(coordinates.second / cellSize);

        entityGrid[gridX][gridY].push_back(pheromone);
    }
}

/*!
 * @brief Function that erases the eaten eggs from their
 * corresponding vectors and fills the grid with the remaining eggs.
 *
 * @param food Vector of type Food.
 * @param entityGrid 3D vector of entities.
 * @param cellSize Size of the grid cells.
 */

void UpdateGridEgg(std::vector<std::shared_ptr<Egg>> &eggs,
                    std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &entityGrid,
                    double cellSize) {
    eggs.erase(std::remove_if(eggs.begin(), eggs.end(),
                               [](const std::shared_ptr<Egg> egg) {
                                 return egg->GetState() != Entity::Alive;
                               }),
                eggs.end());

    for (const auto egg : eggs) {
        std::pair<double, double> coordinates = egg->GetCoordinates();
        int gridX = static_cast<int>(coordinates.first / cellSize);
        int gridY = static_cast<int>(coordinates.second / cellSize);

        entityGrid[gridX][gridY].push_back(egg);
    }
}

/*!
 * @brief Updates the simulation grid, removing dead entities and placing the
 * living ones.
 */
void EntityGrid::UpdateGrid(SimulationData &data, Environment &environment, double deltaTime) {
    ClearGrid();
    UpdateGridCreature(data.creatures_, grid_, SETTINGS.environment.grid_cell_size, data.food_entities_);
    UpdateGridFood(data.food_entities_, grid_, SETTINGS.environment.grid_cell_size);
    UpdateGridEgg(data.eggs_, grid_, SETTINGS.environment.grid_cell_size);
    UpdateQueue(data.reproduce_);
    UpdateGridPheromones(data.pheromones_, grid_, SETTINGS.environment.grid_cell_size, deltaTime);
}

const std::vector<std::shared_ptr<Entity>> &EntityGrid::GetEntitiesAt(const int row,
                                                       const int col) const {
  return grid_[row][col];
}

const std::vector<std::shared_ptr<Entity>> &
EntityGrid::GetEntitiesAt(const std::pair<int, int> &coords) const {
  return grid_[coords.first][coords.second];
}

/*!
 * @brief Retrieves the size of the grid.
 *
 * @return A pair of integers representing the number of rows and columns in the
 * grid respectively.
 */
const std::pair<int, int> EntityGrid::GetGridSize() const {
  return std::make_pair(num_rows_, num_columns_);
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
std::vector<std::pair<int, int>> EntityGrid::GetNeighbours(
    const std::pair<int, int> &center, const int &layer_number) {
  std::vector<std::pair<int, int>> neighbours;
  int x_center = center.first;
  int y_center = center.second;

  for (int y = y_center - layer_number; y <= y_center + layer_number; y++) {
    for (int x = x_center - layer_number; x <= x_center + layer_number; x++) {
      neighbours.push_back(std::make_pair((x + num_columns_) % num_columns_,
                                          (y + num_rows_) % num_rows_));
    }
  }
  return neighbours;
}

std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> EntityGrid::GetGrid() const {
  return grid_;
}
