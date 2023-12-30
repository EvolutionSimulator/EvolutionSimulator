#include "entities/creature.h"

#include <algorithm>
#include <cassert>
#include <queue>
#include <set>

#include "core/collision_functions.h"
#include "core/settings.h"

/*!
 * @brief Construct a new Creature object.
 *
 * @details Initializes a Creature instance with a given NEAT genome. The
 * constructor sets up various creature attributes like health, energy, brain
 * (neural network), genome, neuron data, reproduction cooldown, and age. It
 * also initializes the creature as a movable entity.
 *
 * @param genome A `neat::Genome` object that represents the genetic makeup of
 * the creature, which is used to initialize the creature's neural network
 * (`brain_`).
 *
 * Default properties:
 * - Health: 100
 * - Energy: 100
 * - Brain (Neural Network): Constructed from the provided genome.
 * - Neuron Data: Initialized with zeros (size equal to
 * `settings::environment::kInputNeurons`).
 * - Reproduction Cooldown: Set to
 * `settings::environment::kReproductionCooldown`.
 * - Age: 0
 */
Creature::Creature(neat::Genome genome, Mutable mutables)
    : MovableEntity(), mutable_(mutables), brain_(neat::NeuralNetwork(genome)),
      genome_(genome), neuron_data_(SETTINGS.environment.input_neurons, 0),
      vision_radius_(mutables.GetVisionFactor()),
      vision_angle_(SETTINGS.physical_constraints.vision_ARratio /
                    mutables.GetVisionFactor()),
      age_(0), reproduction_cooldown_(mutables.GetMaturityAge()) {
  size_ = mutables.GetBabySize();
  health_ = mutables.GetIntegrity() * pow(size_, 2);
  energy_ = mutables.GetEnergyDensity() * pow(size_, 2);
}

/*!
 * @brief Retrieves the generation of the creature.
 *
 * @details This function returns the generation number of the creature
 * instance. The generation is stored as a private member variable `generation_`
 * within the Creature class. This function is a constant member function,
 * implying it does not modify any member variables.
 *
 * @return int The generation number of the creature.
 */
int Creature::GetGeneration() const { return generation_; }

/*!
 * @brief Sets the generation number for the creature.
 *
 * @details This method assigns a new generation number to the creature.
 * The generation number typically represents the creature's place in a
 * generational sequence.
 *
 * @param generation The generation number to be set for the creature.
 */
void Creature::SetGeneration(int generation) { generation_ = generation; }

/*!
 * @brief Retrieves the current energy level of the creature.
 *
 * @details This method returns the creature's current energy, which is a key
 * factor in its survival and activities.
 *
 * @return double The current energy level of the creature.
 */
double Creature::GetEnergy() const { return energy_; }

/*!
 * @brief Balances the creature's health and energy levels based on various
 * conditions.
 *
 * @details This method adjusts the creature's health and energy depending on
 * their current values and predefined thresholds.
 */
void Creature::BalanceHealthEnergy() {
  if (GetEnergy() < 1) {
    SetHealth(GetHealth() + GetEnergy() - 1);
    SetEnergy(1);
    if (GetHealth() >= (GetEnergy() - 1)) {
      SetHealth(GetHealth() + (GetEnergy() - 1));
      SetEnergy(1);
    } else {
      SetHealth((GetHealth() + GetEnergy()) / 2);
      SetEnergy(GetHealth());
    }
  } else if (GetHealth() < 0) {
    if (GetEnergy() >= (GetHealth() - 0.1)) {
      SetEnergy(GetEnergy() - GetHealth() - 0.1);
      SetHealth(0.1);
    } else {
      SetHealth((GetHealth() + GetEnergy()) / 2);
      SetEnergy(GetHealth());
    }
  } else if (GetEnergy() > max_energy_) {
    SetHealth(GetHealth() + (GetEnergy() - max_energy_));
    SetEnergy(max_energy_);
  } else if (GetHealth() > GetEnergy() && GetEnergy() <= 0) {
    SetEnergy(GetEnergy() + 0.1);
    SetHealth(GetHealth() - 0.1);
  } else if (GetHealth() < GetEnergy() && GetEnergy() >= 0.1 * max_energy_) {
    SetEnergy(GetEnergy() - 0.1);
    SetHealth(GetHealth() + 0.1);
  }
}

/*!
 * @brief Retrieves the current health level of the creature.
 *
 * @details This method returns the creature's current health, reflecting its
 * overall well-being.
 *
 * @return double The current health level of the creature.
 */
double Creature::GetHealth() const { return health_; }

/*!
 * @brief Sets the health level of the creature.
 *
 * @param health The new health level to be set, capped at 100.
 */
void Creature::SetHealth(double health) {
  if (health > mutable_.GetIntegrity() * pow(size_, 2)) {
    health_ = mutable_.GetIntegrity() * pow(size_, 2);
  } else {
    health_ = health;
  }
}

/*!
 * @brief Triggers the death of the creature.
 *
 * @details This method changes the state of the creature to 'Dead', indicating
 * its demise.
 */
void Creature::Dies() { SetState(Dead); }

/*!
 * @brief Sets the energy level of the creature.
 *
 * @param energy The new energy level to be set, limited by the creature's
 * maximum energy.
 */
void Creature::SetEnergy(double energy) {
  if (energy > max_energy_) {
    energy_ = max_energy_;
  } else {
    energy_ = energy;
  }
}

/*!
 * @brief Updates the creature's energy level based on various factors.
 *
 * @details This method modifies the energy level considering the creature's
 * movement, rotation, and size over a time delta.
 *
 * @param deltaTime The time interval over which the energy update is
 * calculated.
 */
void Creature::UpdateEnergy(double deltaTime) {
  double movement_energy =
      (fabs(GetAcceleration()) + fabs(GetRotationalAcceleration())) *
      GetSize() * deltaTime / 200;
  double heat_loss = mutable_.GetEnergyLoss() * pow(size_, 1) * deltaTime / 100;

  SetEnergy(GetEnergy() - movement_energy - heat_loss);
  BalanceHealthEnergy();

  if (GetHealth() <= 0) {
    Dies();
  }
}

/*!
 * @brief Checks if the creature is fit for reproduction.
 *
 * @details This method determines if the creature meets the energy requirements
 * and cooldown period for reproduction.
 *
 * @return true if the creature is fit for reproduction, false otherwise.
 */
bool Creature::Fit() {
  if (energy_ > SETTINGS.environment.reproduction_threshold * max_energy_ &&
      reproduction_cooldown_ == 0.0 && age_ < 700) {
    return true;
  }
  return false;
}

/*!
 * @brief Handles the creature's reproduction process.
 *
 * @details This method deducts the energy cost of reproduction and initiates
 * the cooldown period.
 */
void Creature::Reproduced() {
  SetEnergy(GetEnergy() - 0.75 * mutable_.GetEnergyDensity() * pow(size_, 2));
  reproduction_cooldown_ = mutable_.GetReproductionCooldown();
}

/*!
 * @brief Retrieves the maximum energy level of the creature.
 *
 * @details This method returns the upper limit of the creature's energy
 * capacity.
 *
 * @return double The maximum energy level of the creature.
 */
double Creature::GetMaxEnergy() { return max_energy_; }

/*!
 * @brief Updates the maximum energy level of the creature.
 *
 * @details This method takes into account the changes in the energy
 *          level due to both the increased size and the age.
 *
 * @param max_energy The desired maximum energy level.
 */
void Creature::UpdateMaxEnergy() {
  max_energy_ = mutable_.GetEnergyDensity() * pow(size_, 2) * (1 - age_ / 1000);
}

/*!
 * @brief Retrieves the age of the creature.
 *
 * @details This method returns the current age of the creature, which is stored
 * in the age_ member variable.
 *
 * @return The current age of the creature.
 */
double Creature::GetAge() const { return age_; }

/*!
 * @brief Sets the age of the creature.
 *
 * @param age The new age to be set.
 */
void Creature::SetAge(double age) { age_ = age; }

/*!
 * @brief Handles the creature's consumption of food.
 *
 * @details Increases the creature's energy based on the nutritional value of
 * the food consumed. If the resulting energy exceeds the maximum energy limit,
 * it triggers a health balance routine.
 *
 * @param nutritional_value The nutritional value of the consumed food.
 */
void Creature::Eats(double nutritional_value) {
  velocity_ = 0;
  SetEnergy(GetEnergy() + nutritional_value);
  if (GetEnergy() > max_energy_) {
    BalanceHealthEnergy();
  }
}

/*!
 * @brief Updates the creature's state over a given time interval.
 *
 * @details This comprehensive method updates various aspects of the creature's
 * life, including energy, movement, rotation, thinking, and aging, over a
 * specified time interval. Additionally, it handles the creature's reproduction
 * cooldown.
 *
 * @param deltaTime Time elapsed since the last update.
 * @param kMapWidth Width of the map.
 * @param kMapHeight Height of the map.
 * @param grid The environment grid containing entities.
 * @param GridCellSize Size of each cell in the grid.
 */
void Creature::Update(double deltaTime, double const kMapWidth,
                      double const kMapHeight,
                      std::vector<std::vector<std::vector<Entity *>>> &grid,
                      double GridCellSize, double frictional_coefficient) {
  this->frictional_coefficient_ = frictional_coefficient;
  this->UpdateMaxEnergy();
  this->UpdateEnergy(deltaTime);
  this->UpdateVelocities(deltaTime);
  this->Move(deltaTime, kMapWidth, kMapHeight);
  this->Rotate(deltaTime);
  this->Think(grid, GridCellSize, deltaTime);
  age_ += 0.05;

  if (reproduction_cooldown_ <= 0) {
    reproduction_cooldown_ = 0.0;
  } else {
    reproduction_cooldown_ -= deltaTime;
  }
}

/*!
 * @brief Retrieves the creature's genetic genome.
 *
 * @details Returns the genome of the creature, which is a representation of its
 * genetic makeup.
 *
 * @return The genome of the creature.
 */
neat::Genome Creature::GetGenome() { return genome_; }

/*!
 * @brief Retrieves the creature's mutables
 *
 * @details Return the mutables of the creature which is a representation of its
 * characteristics
 *
 * @return The mutables of the creature.
 */
Mutable Creature::GetMutable() { return mutable_; }

/*!
 * @brief Handles the collision of the creature with another entity.
 *
 * @details Processes the interaction when the creature collides with another
 * entity. If the entity is food, the creature consumes it. Otherwise, standard
 * collision handling is performed.
 *
 * @param other_entity The entity the creature collides with.
 * @param kMapWidth Width of the map.
 * @param kMapHeight Height of the map.
 */
void Creature::OnCollision(Entity &other_entity, double const kMapWidth,
                           double const kMapHeight) {
  if (Food *food = dynamic_cast<Food *>(&other_entity)) {
    if (food->GetState() == Entity::Alive) {
      double max_nutrition = food->GetNutritionalValue() * food->GetSize();
      double hunger = GetMaxEnergy() - GetEnergy();
      if (max_nutrition > hunger) {
        Eats(hunger);
        food->SetSize(((max_nutrition - hunger) / max_nutrition) *
                      food->GetSize());
      } else {
        Eats(max_nutrition);
        food->Eat();
      }
    }
  } else {
    MovableEntity::OnCollision(other_entity, kMapWidth, kMapHeight);
  }
}

/*!
 * @brief Processes the creature's thinking logic.
 *
 * @details This method involves processing vision, updating neuron data based
 * on environmental stimuli, and determining movement and rotation based on the
 * outputs from the creature's neural network.
 *
 * @param grid The environmental grid.
 * @param GridCellSize Size of each cell in the grid.
 */
void Creature::Think(std::vector<std::vector<std::vector<Entity *>>> &grid,
                     double GridCellSize, double deltaTime) {
  // Not pretty but we'll figure out a better way in the future
  ProcessVisionFood(grid, GridCellSize);
  neuron_data_.at(0) = energy_;
  neuron_data_.at(1) = GetVelocity();
  neuron_data_.at(2) = GetVelocityAngle();
  neuron_data_.at(3) = GetRotationalVelocity();
  neuron_data_.at(4) = orientation_food_;
  neuron_data_.at(5) = distance_food_;
  std::vector<double> output = brain_.Activate(neuron_data_);
  SetAcceleration(std::tanh(output.at(0)) * mutable_.GetMaxForce());
  SetAccelerationAngle(std::tanh(output.at(1)) * M_PI);
  SetRotationalAcceleration(std::tanh(output.at(2)) * mutable_.GetMaxForce());
  Grow(std::max(std::tanh(output.at(3)) * deltaTime, 0.0));
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

/*!
 * @brief Processes the creature's vision to locate food.
 *
 * @details Determines the closest food entity based on the creature's position
 * and updates the creature's orientation and distance metrics towards the
 * located food. If there is no food in sight, it assumes the distance to food
 * is the vision radius (so far away), and the orientation is something random
 * in its field of view.
 *
 * @param grid The environmental grid.
 * @param GridCellSize Size of each cell in the grid.
 */
void Creature::ProcessVisionFood(
    std::vector<std::vector<std::vector<Entity *>>> &grid,
    double GridCellSize) {
  Food *food = GetClosestFoodInSight(grid, GridCellSize);
  if (food) {
    distance_food_ = this->GetDistance(*food, SETTINGS.environment.map_width,
                                       SETTINGS.environment.map_height) -
                     (*food).GetSize();
    orientation_food_ = this->GetRelativeOrientation(*food);
  } else {
    distance_food_ = vision_radius_;
    orientation_food_ =
        remainder(GetRandomFloat(orientation_ - vision_angle_ / 2,
                                 orientation_ + vision_angle_ / 2),
                  2 * M_PI);
  }
}

/*!
 * @brief Manages the creature's growth based on energy consumption.
 *
 * @details This function calculates the creature's growth as a function of the
 * energy consumed and the growth factor. If the calculated size exceeds the
 * maximum size, it is capped at the maximum. It also deducts the used energy.
 *
 * @param energy The amount of energy consumed for growth.
 */
void Creature::Grow(double energy) {
  double size = GetSize() + energy * mutable_.GetGrowthFactor();
  (size > mutable_.GetMaxSize()) ? SetSize(mutable_.GetMaxSize())
                                 : SetSize(size);
  SetEnergy(GetEnergy() - energy);
}

/*!
 * @brief Finds the closest food entity in the vicinity of the creature.
 *
 * @details Scans the nearby environment, represented by a grid, to locate the
 * nearest food entity. Returns a pointer to the closest food, or nullptr if no
 * food is within reach.
 *
 * @param grid The environmental grid.
 * @param GridCellSize Size of each cell in the grid.
 *
 * @return A pointer to the closest food entity or nullptr if none is found.
 */
Food *
Creature::GetClosestFood(std::vector<std::vector<std::vector<Entity *>>> &grid,
                         double GridCellSize) const {
  if (grid.empty())
    return nullptr;
  std::pair<double, double> coordinates_creature = GetCoordinates();
  int i_creature = (int)coordinates_creature.first / (int)GridCellSize;
  int j_creature = (int)coordinates_creature.second /
                   (int)GridCellSize; // position of the creature on the grid
  std::vector<Food *> closest_food_entities = get_food_at_distance(
      grid, i_creature, j_creature,
      0); // here we place the candidates for the closest food
  int grid_distance = 1;
  int boundary = std::max(grid.size(), grid[0].size());

  while (closest_food_entities.size() == 0 && grid_distance <= boundary) {
    // std::cout << "checking at distance " << grid_distance << std::endl;
    closest_food_entities =
        get_food_at_distance(grid, i_creature, j_creature, grid_distance);
    grid_distance++;
  }
  // assert(!closest_food_entities.empty());
  if (closest_food_entities.empty())
    return nullptr;

  Food *closest_food = closest_food_entities.front();
  double smallest_distance =
      GetDistance(*closest_food, SETTINGS.environment.map_width,
                  SETTINGS.environment.map_height);
  for (Food *&food : closest_food_entities) {
    double distance = GetDistance(*food, SETTINGS.environment.map_width,
                                  SETTINGS.environment.map_height);
    if (distance < smallest_distance) {
      closest_food = food;
      smallest_distance = distance;
    }
    assert(!closest_food_entities.empty());
    return closest_food;
  }
  return nullptr;
}

/*!
 * @brief Identifies food entities within a specified grid distance.
 *
 * @details Searches the grid within a specified distance from the creature's
 * location to find food entities. Returns a vector containing pointers to all
 * food entities found within this range.
 *
 * @param grid The environmental grid.
 * @param i_creature The i-coordinate of the creature on the grid.
 * @param j_creature The j-coordinate of the creature on the grid.
 * @param grid_distance The distance to search for food on the grid.
 *
 * @return A vector of pointers to food entities within the specified distance.
 */
std::vector<Food *>
get_food_at_distance(std::vector<std::vector<std::vector<Entity *>>> &grid,
                     int i_creature, int j_creature, int grid_distance) {
  std::vector<Food *> food;
  if (grid.empty())
    return food;
  int grid_width = grid.size();
  int grid_height = grid[0].size();

  if (i_creature + grid_distance < grid_width) {
    for (int j = std::max(0, j_creature - grid_distance);
         j < std::min(grid_height, j_creature + grid_distance + 1); j++) {
      // std:: cout << "checking " << i_creature + grid_distance << ", " << j <<
      // std::endl;
      for (std::vector<Entity *>::iterator it =
               grid[i_creature + grid_distance][j].begin();
           it != grid[i_creature + grid_distance][j].end(); it++) {
        Entity *entity_ptr = *it;
        Food *food_ptr = dynamic_cast<Food *>(entity_ptr);
        if (food_ptr != nullptr) {
          food.push_back(food_ptr);
        }
      }
    }
  }
  if (i_creature - grid_distance >= 0) {
    for (int j = std::max(0, j_creature - grid_distance);
         j < std::min(grid_height, j_creature + grid_distance + 1); j++) {
      for (std::vector<Entity *>::iterator it =
               grid[i_creature - grid_distance][j].begin();
           it != grid[i_creature - grid_distance][j].end(); it++) {
        Entity *entity_ptr = *it;
        Food *food_ptr = dynamic_cast<Food *>(entity_ptr);
        if (food_ptr != nullptr) {
          food.push_back(food_ptr);
        }
      }
    }
  }
  if (j_creature + grid_distance < grid_height) {
    for (int i = std::max(0, i_creature - grid_distance + 1);
         i < std::min(grid_width, i_creature + grid_distance); i++) {
      for (std::vector<Entity *>::iterator it =
               grid[i][j_creature + grid_distance].begin();
           it != grid[i][j_creature + grid_distance].end(); it++) {
        Entity *entity_ptr = *it;
        Food *food_ptr = dynamic_cast<Food *>(entity_ptr);
        if (food_ptr != nullptr) {
          food.push_back(food_ptr);
        }
      }
    }
  }
  if (j_creature - grid_distance >= 0) {
    for (int i = std::max(0, i_creature - grid_distance + 1);
         i < std::min(grid_width, i_creature + grid_distance); i++) {
      for (std::vector<Entity *>::iterator it =
               grid[i][j_creature - grid_distance].begin();
           it != grid[i][j_creature - grid_distance].end(); it++) {
        Entity *entity_ptr = *it;
        Food *food_ptr = dynamic_cast<Food *>(entity_ptr);
        if (food_ptr != nullptr) {
          food.push_back(food_ptr);
        }
      }
    }
  }
  return food;
}

/*!
 * @brief Sets the vision parameters for the creature.
 *
 * @details This function sets the creature's vision radius and angle.
 * The vision radius determines how far the creature can see,
 * while the vision angle specifies the total angle at which it sees,
 * which will be centered at its orientation (so at most angle/2 to the
 * left and angle/2 to the right of its orientation).
 *
 * @param radius The radius of the vision, representing how far the creature can
 * see.
 * @param angle The angle of the vision, representing the full angle of the
 * visual field.
 */
void Creature::SetVision(double radius, double angle) {
  vision_radius_ = radius;
  vision_angle_ = angle;
}

double Creature::GetVisionRadius() const { return vision_radius_; }

double Creature::GetVisionAngle() const { return vision_angle_; }

/*!
 * @brief Finds the closest food entity (meat or plant) within the creature's
 * line of sight.
 *
 * @details Performs a breadth-first search (BFS) on the grid cells within the
 * creature's field of view to locate the closest food entity. It utilizes
 * IsGridCellPotentiallyInsideCone functions to efficiently narrow down the
 * search area to relevant cells. The function iterates over entities within
 * these cells, dynamically casting them to Food* to check if they are edible.
 * It then determines if they are within the creature's field of view and closer
 * than any previously found food. The search continues until the closest food
 * is found or a predefined number of cells have been processed.
 *
 * @param grid A 3-dimensional vector representing the environmental grid where
 * each cell contains entities.
 * @param grid_cell_size The size of each square cell in the grid.
 *
 * @return A pointer to the closest food entity within the line of sight;
 * nullptr if no food is found.
 */
Food *Creature::GetClosestFoodInSight(
    std::vector<std::vector<std::vector<Entity *>>> &grid,
    double grid_cell_size) const {
  int grid_width = grid.size();
  int grid_height = grid[0].size();

  int x_grid = static_cast<int>(x_coord_ / grid_cell_size);
  int y_grid = static_cast<int>(y_coord_ / grid_cell_size);
  // multiplied by 4 as a temporal fix
  int max_cells_to_find_food = 4 * M_PI * (vision_radius_ + grid_cell_size) *
                               (vision_radius_ + grid_cell_size) /
                               (grid_cell_size * grid_cell_size);

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

      if (food) {
        auto food_point = Point(entity->GetCoordinates());

        auto food_direction = OrientedAngle(cone_center, food_point);

        bool is_in_field_of_view = food_direction.IsInsideCone(
            cone_left_boundary, cone_right_boundary);

        double distance = food_point.dist(cone_center);

        bool is_within_vision_radius =
            distance <= vision_radius_ + SETTINGS.engine.EPS;

        if (is_in_field_of_view && is_within_vision_radius) {
          if (distance < smallest_distance_food) {
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

    assert(processed_cells <= max_cells_to_find_food &&
           "processed_cells exceeded max_cells_to_find_food in "
           "GetClosestFoodInSight");

    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        if (dx * dx + dy * dy != 1)
          continue;
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
