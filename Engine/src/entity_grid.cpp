#include "../include/entity_grid.h"

#include "settings.h"
#include <cmath>
#include <vector>

EntityGrid::EntityGrid() {
  num_columns_ = static_cast<int>(std::ceil(
                     static_cast<double>(SETTINGS.environment.map_width) /
                     SETTINGS.environment.grid_cell_size)) +
                 1;
  num_rows_ = static_cast<int>(std::ceil(
                  static_cast<double>(SETTINGS.environment.map_height) /
                  SETTINGS.environment.grid_cell_size)) +
              1;

  grid_.assign(num_columns_, std::vector<std::vector<Entity *>>(num_rows_));
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

void UpdateGridFood(std::vector<Food> &foods,
                    std::vector<std::vector<std::vector<Entity *>>> &entityGrid,
                    double cellSize) {
  foods.erase(std::remove_if(foods.begin(), foods.end(),
                             [](const Food &food) {
                               return food.GetState() != Entity::Alive;
                             }),
              foods.end());


  for (Food &entity : foods) {
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
    std::vector<Creature> &creatures,
    std::vector<std::vector<std::vector<Entity *>>> &entityGrid,
    double cellSize, std::vector<Food> &food) {
  creatures.erase(std::remove_if(creatures.begin(), creatures.end(),
                                 [&food](const Creature &entity) {
                                   if (entity.GetState() == Entity::Dead) {
                                     food.emplace_back(
                                         Meat(entity.GetCoordinates().first,
                                              entity.GetCoordinates().second,
                                              entity.GetSize()));
                                   }
                                   return entity.GetState() != Entity::Alive;
                                 }),
                  creatures.end());

  for (Creature &creature : creatures) {
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
void UpdateQueue(std::queue<Creature> &reproduce) {
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
void EntityGrid::UpdateGrid(SimulationData &data, Environment &environment) {
  ClearGrid();
  UpdateGridCreature(data.creatures_, grid_, SETTINGS.environment.grid_cell_size, data.food_entities_);
  UpdateGridFood(data.food_entities_, grid_, SETTINGS.environment.grid_cell_size);
  UpdateQueue(data.reproduce_);
}

const std::vector<Entity *> &EntityGrid::GetEntitiesAt(const int row,
                                                       const int col) const {
  return grid_[row][col];
}

const std::vector<Entity *> &
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
std::vector<std::pair<int, int>>
EntityGrid::GetNeighbours(const std::pair<int, int> &center,
                          const int &layer_number) {
  std::vector<std::pair<int, int>> neighbours;
  int x_center = center.first;
  int y_center = center.second;

  for (int y = y_center - layer_number; y <= y_center + layer_number;
       y++) {
    for (int x = x_center - layer_number;
         x <= x_center + layer_number; x++) {
      neighbours.push_back(std::make_pair((x + num_columns_) % num_columns_,
                                          (y + num_rows_) % num_rows_));
    }
  }
  return neighbours;
}

std::vector<std::vector<std::vector<Entity *>>> EntityGrid::GetGrid() const {
  return grid_;
}
