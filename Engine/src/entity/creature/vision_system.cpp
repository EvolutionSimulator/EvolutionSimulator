#include "entity/creature/vision_system.h"
#include "entity/creature/pheromone.h"
#include "core/settings.h"
#include <queue>
#include <set>

VisionSystem::VisionSystem(neat::Genome genome, Mutable mutables)
    : AliveEntity(genome, mutables),
      vision_radius_(mutables.GetVisionFactor()),
      vision_angle_(SETTINGS.physical_constraints.vision_ar_ratio
                    / mutables.GetVisionFactor()),
      distance_entity_(mutables.GetVisionFactor()),
      entity_compatibility_(0),
      orientation_entity_(0),
      entity_color_(0),
      entity_size_(0),
      closest_entity_(nullptr)
{
  number_entities_to_return_ = 1;
  for(BrainModule module : genome.GetModules()){
    if (module.GetModuleId() == 3){
      number_entities_to_return_++;
    }
  }
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

double VisionSystem::GetEntityCompatibility() const { return entity_compatibility_; }


std::vector<std::shared_ptr<Entity>> VisionSystem::GetClosestEntitiesInSight(std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
                                              double grid_cell_size, double map_width, double map_heigth) const
{
    int grid_width = grid.size();
    int grid_height = grid[0].size();

    int x_grid = static_cast<int>(x_coord_ / grid_cell_size);
    int y_grid = static_cast<int>(y_coord_ / grid_cell_size);

    int max_cells_to_find_food = M_PI * pow(vision_radius_ + 2 * sqrt(2) * grid_cell_size + SETTINGS.environment.max_food_size, 2) / (grid_cell_size * grid_cell_size);

    auto cone_center = Point(x_coord_, y_coord_);
    auto cone_orientation = GetOrientation();
    auto cone_left_boundary = OrientedAngle(cone_orientation - vision_angle_ / 2);
    auto cone_right_boundary = OrientedAngle(cone_orientation + vision_angle_ / 2);

    std::vector<std::shared_ptr<Entity>> found_entities;
    found_entities.reserve(number_entities_to_return_);

    std::queue<std::pair<int, int>> cells_queue;
    std::set<std::pair<int, int>> visited_cells;

    cells_queue.push({x_grid, y_grid});
    visited_cells.insert({x_grid, y_grid});

    size_t processed_cells = 0;

    while (!cells_queue.empty()) {
      auto [x, y] = cells_queue.front();
      cells_queue.pop();
      ++processed_cells;

      for (auto entity : grid[x][y]) {
        if (entity &&  entity.get() != this && IsInVisionCone(entity, map_width, map_heigth)) {
          std::shared_ptr<Pheromone> pheromone_entity = std::dynamic_pointer_cast<Pheromone>(entity);

            if (!pheromone_entity) {
            found_entities.push_back(entity);
            if (found_entities.size() == number_entities_to_return_) {
              break;
            }
          }
        }
      }
      if (found_entities.size() == number_entities_to_return_ || processed_cells > max_cells_to_find_food) {
        break;
      }

      for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
          if (dx * dx + dy * dy != 1) continue;
          int nx = x + dx, ny = y + dy;
          if (0 <= nx && nx < grid_width && 0 <= ny && ny < grid_height && !visited_cells.count({nx, ny})) {
            if (IsGridCellPotentiallyInsideCone(
                    Point(nx * grid_cell_size, ny * grid_cell_size),
                    grid_cell_size, cone_center, vision_radius_,
                    cone_left_boundary, cone_right_boundary,
                    map_width, map_heigth)) {
              visited_cells.insert({nx, ny});
              cells_queue.push({nx, ny});
            }
          }
        }
      }
    }
    found_entities.resize(number_entities_to_return_, nullptr);
    return found_entities;
}


bool VisionSystem::IsInRightDirection(std::shared_ptr<Entity> entity, double map_width, double map_heigth)
{
  auto cone_center = Point(x_coord_, y_coord_);
  auto cone_orientation = GetOrientation();
  auto cone_left_boundary = OrientedAngle(cone_orientation - vision_angle_ / 2);
  auto cone_right_boundary =
      OrientedAngle(cone_orientation + vision_angle_ / 2);
  auto entity_point = Point(entity->GetCoordinates());
  auto entity_direction = OrientedAngle(cone_center, entity_point, map_width, map_heigth);

  return entity_direction.IsInsideCone(cone_left_boundary, cone_right_boundary);
}

bool VisionSystem::IsInVisionCone(std::shared_ptr<Entity> entity, double map_width, double map_heigth) const
{
  auto cone_center = Point(x_coord_, y_coord_);
  auto cone_orientation = GetOrientation();
  auto cone_left_boundary = OrientedAngle(cone_orientation - vision_angle_ / 2);
  auto cone_right_boundary = OrientedAngle(cone_orientation + vision_angle_ / 2);

  auto entity_point = Point(entity->GetCoordinates());

  auto food_direction = OrientedAngle(cone_center, entity_point, map_width, map_heigth);

  double distance = entity_point.dist(cone_center, map_width, map_heigth);

  bool is_in_field_of_view = (food_direction.IsInsideCone(cone_left_boundary, cone_right_boundary));

  bool is_on_edge = (food_direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary) <= M_PI/2) && (distance * sin(food_direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary)) <= entity->GetSize() + SETTINGS.engine.eps);

  if (is_in_field_of_view) {
    bool is_within_vision_radius =
        distance <= vision_radius_ + entity->GetSize() + SETTINGS.engine.eps;
    if (is_within_vision_radius) { return true; }
  }

  if (is_on_edge) {
    bool is_within_vision_radius =
        (distance * cos(food_direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary)) <= vision_radius_ + SETTINGS.engine.eps);
    if (is_within_vision_radius) { return true;}
  }

  return false;
}





/*!
 * @brief Retrieves the id of the closest food.
 *
 * @return The id of the closest food or -1 if there is no food in vision.
 */
std::shared_ptr<Entity> VisionSystem::GetFoodID() const {
  return closest_entity_;
}
