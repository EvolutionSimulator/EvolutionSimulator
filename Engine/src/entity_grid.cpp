#include "../include/entity_grid.h"

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
 * @brief Template function that erases the dead entities from their
 * corresponding vectors and fills the grid with the remaining entities.
 *
 * @tparam entities Vector of EntityType.
 * @param entityGrid 3D vector of entities.
 * @param cellSize Size of the grid cells.
 */
template <typename EntityType>
void UpdateGridTemplate(
    std::vector<EntityType> &entities,
    std::vector<std::vector<std::vector<Entity *>>> &entityGrid,
    double cellSize) {
  entities.erase(std::remove_if(entities.begin(), entities.end(),
                                [](const EntityType &entity) {
                                  return entity.GetState() != Entity::Alive;
                                }),
                 entities.end());

  for (EntityType &entity : entities) {
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
void EntityGrid::RefreshGrid(SimulationData &data, Environment &environment) {
  ClearGrid();
  UpdateGridTemplate<Creature>(data.creatures_, grid_,
                               SETTINGS.environment.grid_cell_size);
  //  std::cout << "Food before: " << data.food_entities_.size() << "\n";
  UpdateGridTemplate<Food>(data.food_entities_, grid_,
                           SETTINGS.environment.grid_cell_size);
  //  std::cout << "Food after: " << data.food_entities_.size() << std::endl;
  UpdateQueue(data.reproduce_);
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

const std::vector<std::vector<std::vector<Entity *>>> &
EntityGrid::GetGrid() const {
  return grid_;
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

  for (int y = y_center - layer_number; y <= y_center + layer_number; y++) {
    for (int x = x_center - layer_number; x <= x_center + layer_number; x++) {
      neighbours.push_back(std::make_pair((x + num_columns_) % num_columns_,
                                          (y + num_rows_) % num_rows_));
    }
  }
  return neighbours;
}
