#include "vision_system.h"
#include "settings.h"
#include <queue>
#include <set>

VisionSystem::VisionSystem(neat::Genome genome, Mutable mutables)
    : AliveEntity(genome, mutables),
      vision_radius_(mutables.GetVisionFactor()),
      vision_angle_(SETTINGS.physical_constraints.vision_ar_ratio
                    / mutables.GetVisionFactor())
{

}

/*!
 * @brief Sets the vision parameters for the VisionSystem.
 *
 * @details This function sets the VisionSystem's vision radius and angle.
 * The vision radius determines how far the VisionSystem can see,
 * while the vision angle specifies the total angle at which it sees,
 * which will be centered at its orientation (so at most angle/2 to the
 * left and angle/2 to the right of its orientation).
 *
 * @param radius The radius of the vision, representing how far the VisionSystem can see.
 * @param angle The angle of the vision, representing the full angle of the visual field.
 */
void VisionSystem::SetVision(double radius, double angle) {
  vision_radius_ = radius;
  vision_angle_ = angle;
}

double VisionSystem::GetVisionRadius() const { return vision_radius_; }

double VisionSystem::GetVisionAngle() const { return vision_angle_; }



Food* VisionSystem::GetClosestMeatInSight(
    std::vector<std::vector<std::vector<Entity *>>> &grid,
    double grid_cell_size) const {
    return (GetClosestFoodInSight(grid, grid_cell_size, Food::type::meat));
}

// Function to get the closest plant in sight
Food* VisionSystem::GetClosestPlantInSight(
    std::vector<std::vector<std::vector<Entity *>>> &grid,
    double grid_cell_size) const {
    return (GetClosestFoodInSight(grid, grid_cell_size, Food::type::plant));
}

/*!
 * @brief Finds the closest food entity (meat or plant) within the VisionSystem's line of sight.
 *
 * @details Performs a breadth-first search (BFS) on the grid cells within the VisionSystem's
 * field of view to locate the closest food entity. It utilizes IsGridCellPotentiallyInsideCone
 * functions to efficiently narrow down the search area to relevant cells. The function iterates over
 * entities within these cells, dynamically casting them to Food* to check if they are edible.
 * It then determines if they are within the VisionSystem's field of view and closer than any previously found food.
 * The search continues until the closest food is found or a predefined number of cells have been processed.
 *
 * @param grid A 3-dimensional vector representing the environmental grid where each cell contains entities.
 * @param grid_cell_size The size of each square cell in the grid.
 *
 * @return A pointer to the closest food entity within the line of sight; nullptr if no food is found.
 */
Food *VisionSystem::GetClosestFoodInSight(
    std::vector<std::vector<std::vector<Entity *>>> &grid,
    double grid_cell_size, Food::type food_type) const {
  int grid_width = grid.size();
  int grid_height = grid[0].size();

  int x_grid = static_cast<int>(x_coord_ / grid_cell_size);
  int y_grid = static_cast<int>(y_coord_ / grid_cell_size);

  //temporary fix multiply by 4
  int max_cells_to_find_food = M_PI * pow(vision_radius_ + 2 * sqrt(2) * grid_cell_size + SETTINGS.environment.max_food_size, 2) / (grid_cell_size * grid_cell_size);

  auto cone_center = Point(x_coord_, y_coord_);
  auto cone_orientation = GetOrientation();
  auto cone_left_boundary = OrientedAngle(cone_orientation - vision_angle_ / 2);
  auto cone_right_boundary =
      OrientedAngle(cone_orientation + vision_angle_ / 2);

  Food *closest_food = nullptr;
  double smallest_distance_food = std::numeric_limits<double>::max();

  std::queue<std::pair<int, int>> cells_queue;
  std::set<std::pair<int, int>> visited_cells;

  cells_queue.push({x_grid, y_grid});
  visited_cells.insert({x_grid, y_grid});

  size_t processed_cells = 0;

  while (!cells_queue.empty()) {
    auto [x, y] = cells_queue.front();
    cells_queue.pop();
    ++processed_cells;

    for (Entity *entity : grid[x][y]) {
      Food *food = dynamic_cast<Food *>(entity);

      if (food && food->GetType()==food_type) {
        auto food_point = Point(entity->GetCoordinates());

        auto food_direction = OrientedAngle(cone_center, food_point);

        double distance = food_point.dist(cone_center);

        bool is_in_field_of_view = (food_direction.IsInsideCone(
            cone_left_boundary, cone_right_boundary));

        bool is_on_edge = (food_direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary) <= M_PI/2) && (distance * sin(food_direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary)) <= food->GetSize() + SETTINGS.engine.eps);

        if (is_in_field_of_view) {
          bool is_within_vision_radius =
              distance <= vision_radius_ + food->GetSize() + SETTINGS.engine.eps;
          if (is_within_vision_radius && distance < smallest_distance_food) {
            smallest_distance_food = distance;
            closest_food = food;
            break;
          }
        }

        if (is_on_edge) {
          bool is_within_vision_radius =
              (distance * cos(food_direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary)) <= vision_radius_ + SETTINGS.engine.eps);
          if (is_within_vision_radius && distance < smallest_distance_food) {
            smallest_distance_food = distance;
            closest_food = food;
            break;
          }
        }
      }
    }
    if (closest_food) {
      break;
    }

    //assert(processed_cells <= max_cells_to_find_food && "processed_cells exceeded max_cells_to_find_food in GetClosestFoodInSight");
    if (processed_cells > max_cells_to_find_food){
        break;
    }

    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        if (dx * dx + dy * dy != 1) continue;
        int nx = x + dx, ny = y + dy;
        if (0 <= nx && nx < grid_width && 0 <= ny && ny < grid_height &&
            !visited_cells.count({nx, ny})) {
          if (IsGridCellPotentiallyInsideCone(
                  Point(nx * grid_cell_size, ny * grid_cell_size),
                  grid_cell_size, cone_center, vision_radius_,
                  cone_left_boundary, cone_right_boundary)) {
            visited_cells.insert({nx, ny});
            cells_queue.push({nx, ny});
          }
        }
      }
    }
  }
  return closest_food;
}

bool VisionSystem::IsFoodInSight(Food *food)
{
  auto cone_center = Point(x_coord_, y_coord_);
  auto cone_orientation = GetOrientation();
  auto cone_left_boundary = OrientedAngle(cone_orientation - vision_angle_ / 2);
  auto cone_right_boundary =
      OrientedAngle(cone_orientation + vision_angle_ / 2);
  auto food_point = Point(food->GetCoordinates());
  auto food_direction = OrientedAngle(cone_center, food_point);

  return food_direction.IsInsideCone(cone_left_boundary, cone_right_boundary);
}


/*!
 * @brief Processes the VisionSystem's vision to locate food.
 *
 * @details Determines the closest food entity based on the VisionSystem's position
 * and updates the VisionSystem's orientation and distance metrics towards the
 * located food. If there is no food in sight, it assumes the distance to food is
 * the vision radius (so far away), and the orientation is something random
 * in its field of view.
 *
 * @param grid The environmental grid.
 * @param GridCellSize Size of each cell in the grid.
 */
void VisionSystem::ProcessVisionFood(
    std::vector<std::vector<std::vector<Entity *>>> &grid,
    double GridCellSize, double width, double height) {
  Food *closePlant = GetClosestPlantInSight(grid, GridCellSize);
  Food *closeMeat = GetClosestMeatInSight(grid, GridCellSize);

  if (closePlant){
        distance_plant_ = this->GetDistance(*closePlant, width, height) - (*closePlant).GetSize();
        orientation_plant_ = this->GetRelativeOrientation(*closePlant);
        closest_plant_id_ = closePlant->GetID();
        plant_size_ = closePlant->GetSize();
  }
  else {
      distance_plant_ = vision_radius_;
      orientation_plant_ = remainder(GetRandomFloat(orientation_- vision_angle_/2, orientation_+ vision_angle_/2), 2*M_PI);
      closest_plant_id_ = 0;
      plant_size_ = -1;
  }

  if (closeMeat){
        distance_meat_ = this->GetDistance(*closeMeat, width, height) - (*closeMeat).GetSize();
        orientation_meat_ = this->GetRelativeOrientation(*closeMeat);
        closest_meat_id_ = closeMeat->GetID();
        meat_size_ = closeMeat->GetSize();
  }
  else {
      distance_meat_ = vision_radius_;
      orientation_meat_ = remainder(GetRandomFloat(orientation_- vision_angle_/2, orientation_+ vision_angle_/2), 2*M_PI);
      closest_meat_id_ = 0;
      meat_size_ = -1;
  }
}

/*!
 * @brief Retrieves the id of the closest food.
 *
 * @return The id of the closest food or -1 if there is no food in vision.
 */
int VisionSystem::GetFoodID() const {
  if (distance_plant_ <= distance_meat_) {return closest_plant_id_;};
  return closest_meat_id_;
}

/*!
 * @brief Generates a random floating-point number within a given range.
 *
 * @details This function uses a uniform distribution to ensure an even spread
 * of values.
 *
 * @param max_value The upper limit of the random number range.
 *
 * @return A random floating-point number between 0 and max_value.
 */
double GetRandomFloat(double min_value, double max_value) {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<double> dis(min_value, max_value);
  return dis(gen);
}
