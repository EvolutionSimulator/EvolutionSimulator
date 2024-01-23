#include "collision_manager.h"

#include "settings.h"

#include <omp.h>

CollisionManager::CollisionManager() {}

/*!
 * @brief Checks for collisions between entities in the simulation.
 *
 * @details Iterates through the grid to detect and handle collisions between
 * different entities.
 */
void CollisionManager::CheckCollisions(EntityGrid& entity_grid) {
  auto grid = entity_grid.GetGrid();

  double tolerance = SETTINGS.environment.tolerance;

  auto entity_grid_size = entity_grid.GetGridSize();
  int num_rows = entity_grid_size.first;
  int num_cols = entity_grid_size.second;

  #pragma omp parallel for collapse(2)
  for (int row = 0; row < num_rows; row++) {
    for (int col = 0; col < num_cols; col++) {
      for (auto entity1 : grid[col][row]) {
        const int layer_number =
            2 *
            ceil((entity1->GetSize() / SETTINGS.environment.grid_cell_size));

        std::vector<std::pair<int, int>> neighbours =
            entity_grid.GetNeighbours({col, row}, layer_number);
        for (const std::pair<int, int> neighbour : neighbours) {
          for (auto entity2 : grid[neighbour.first][neighbour.second]) {
            if (entity1->CheckCollisionWithEntity(tolerance, *entity2)) {
              if (entity1 != entity2) {
                #pragma omp critical
                {
                  entity1->OnCollision(*entity2, SETTINGS.environment.map_width,
                                       SETTINGS.environment.map_height);
                }
              }
            }
          }
        }
      }
    }
  }
}
