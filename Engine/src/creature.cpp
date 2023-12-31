#include "creature.h"

#include <algorithm>
#include <cassert>
#include <queue>
#include <set>

#include "collisions.h"
#include "config.h"

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
 * - Stomach capacity: area of creature * settings::environment::kDStomachCapacityFactor
 */
Creature::Creature(neat::Genome genome, Mutable mutables)
    : MovableEntity(),
      mutable_(mutables),
      brain_(neat::NeuralNetwork(genome)),
      genome_(genome),
      neuron_data_(settings::environment::kInputNeurons, 0),
      vision_radius_(mutables.GetVisionFactor()),
      vision_angle_(settings::physical_constraints::kVisionARratio
                    / mutables.GetVisionFactor()),
      age_(0),
      reproduction_cooldown_ (mutables.GetMaturityAge()),
      eating_cooldown_ (mutables.GetEatingSpeed()),
      stomach_acid_ (0.0),
      potential_energy_in_stomach_(0.0){
    size_ = mutables.GetBabySize();
    health_ = mutables.GetIntegrity() * pow(size_, 2);
    energy_ = mutables.GetEnergyDensity() * pow(size_, 2);
    stomach_capacity_ = mutables.GetStomachCapacityFactor() * pow(size_, 2);
    bite_strength_ = mutables.GetGeneticStrength() * size_;
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
  if (GetEnergy() < 0) {
    SetHealth(GetHealth() + GetEnergy() - 0.1);
    SetEnergy(0.1);
  } else if (GetHealth() < GetEnergy() &&
             GetEnergy() >= 0.1*max_energy_) {
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
  double movement_energy = (fabs(GetAcceleration()) + fabs(GetRotationalAcceleration())) * GetSize() * deltaTime/200;
  double heat_loss = mutable_.GetEnergyLoss() * pow(size_, 1) * deltaTime/100;

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
  if (energy_ > settings::environment::kReproductionThreshold * max_energy_ &&
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
  SetEnergy(GetEnergy() - 0.7 * max_energy_);
  reproduction_cooldown_ = mutable_.GetReproductionCooldown();
}

/*!
 * @brief Determines if the current creature is compatible with another creature.
 *
 * @details This function calculates the compatibility between the current creature and another creature based on their genomes and mutable characteristics. It sums the brain distance, derived from genome compatibility, with the mutable distance, derived from mutable characteristics compatibility. The creatures are considered compatible if the sum is less than a predefined compatibility threshold.
 *
 * @param other_creature A reference to another `Creature` object for compatibility comparison.
 * @return bool Returns `true` if the sum of brain and mutable distances is less than the compatibility threshold, indicating compatibility; otherwise returns `false`.
 */
bool Creature::Compatible(const Creature& other_creature){
  double brain_distance = this->GetGenome().CompatibilityBetweenGenomes(other_creature.GetGenome());
  double mutable_distance = this->GetMutable().CompatibilityBetweenMutables(other_creature.GetMutable());
  return brain_distance + mutable_distance < settings::compatibility::kCompatibilityThreshold;
}

/*!
 * @brief Retrieves the maximum energy level of the creature.
 *
 * @details This method returns the upper limit of the creature's energy
 * capacity.
 *
 * @return double The maximum energy level of the creature.
 */
double Creature::GetMaxEnergy() const { return max_energy_; }


/*!
 * @brief Sets  the maximum energy level of the creature.
 *
 * @details This method sets the upper limit of the creature's energy
 * capacity.
 */
void Creature::SetMaxEnergy(double max_energy) { max_energy_ = max_energy; };


/*!
 * @brief Updates the maximum energy level of the creature.
 *
 * @details This method takes into account the changes in the energy
 *          level due to both the increased size and the age.
 *
 * @param max_energy The desired maximum energy level.
 */
void Creature::UpdateMaxEnergy() {
  max_energy_ = mutable_.GetEnergyDensity() * pow(size_, 2) * (1 - age_/1000);
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
void Creature::SetAge(double age) {
  age_ = age;
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
  this->Think(grid, GridCellSize, deltaTime, kMapWidth, kMapHeight);
  this->Digest(deltaTime);
  age_ += 0.05;

  if (reproduction_cooldown_ <= 0) {
    reproduction_cooldown_ = 0.0;
  } else {
    reproduction_cooldown_ -= deltaTime;
  }

  if (eating_cooldown_ <= 0) {
    eating_cooldown_ = 0.0;
  } else {
    eating_cooldown_ -= deltaTime;
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
neat::Genome Creature::GetGenome() const { return genome_; }

/*!
 * @brief Retrieves the creature's mutables
 *
 * @details Return the mutables of the creature which is a representation of its
 * characteristics
 *
 * @return The mutables of the creature.
 */
Mutable Creature::GetMutable() const {return mutable_;}

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
    if (food->GetState() == Entity::Alive && eating_cooldown_ == 0.0 && biting_ == 1) {
      {
        if (IsFoodInSight(food))
        {
          Bite(food);
        }
      }
    }
  }
  MovableEntity::OnCollision(other_entity, kMapWidth, kMapHeight);
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
                     double GridCellSize, double deltaTime, double width, double height) {
  // Not pretty but we'll figure out a better way in the future
  ProcessVisionFood(grid, GridCellSize, width, height);
  neuron_data_.at(0) = 1;
  neuron_data_.at(1) = energy_;
  neuron_data_.at(2) = GetVelocity();
  neuron_data_.at(3) = GetVelocityAngle();
  neuron_data_.at(4) = GetRotationalVelocity();
  neuron_data_.at(5) = GetEmptinessPercent();
  neuron_data_.at(6) = orientation_plant_;
  neuron_data_.at(7) = distance_plant_;
  neuron_data_.at(8) = plant_size_;
  neuron_data_.at(9) = orientation_meat_;
  neuron_data_.at(10) = distance_meat_;
  neuron_data_.at(11) = meat_size_;
  std::vector<double> output = brain_.Activate(neuron_data_);
  SetAcceleration(std::tanh(output.at(0))*mutable_.GetMaxForce());
  SetAccelerationAngle(std::tanh(output.at(1)) * M_PI);
  SetRotationalAcceleration(std::tanh(output.at(2))*mutable_.GetMaxForce());
  Grow(std::max(std::tanh(output.at(3)) * deltaTime, 0.0));
  AddAcid(std::max(std::tanh(output.at(4)) * 10.0, 0.0));
  biting_ = std::tanh(output.at(5)) > 0 ? 0 : 1;
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
 * located food. If there is no food in sight, it assumes the distance to food is
 * the vision radius (so far away), and the orientation is something random
 * in its field of view.
 *
 * @param grid The environmental grid.
 * @param GridCellSize Size of each cell in the grid.
 */
void Creature::ProcessVisionFood(
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
int Creature::GetFoodID() const {
  if (distance_plant_ <= distance_meat_) {return closest_plant_id_;};
  return closest_meat_id_;
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
  (size > mutable_.GetMaxSize()) ? SetSize(mutable_.GetMaxSize()) : SetSize(size);
  SetEnergy(GetEnergy() - energy);
  stomach_capacity_ = mutable_.GetStomachCapacityFactor() * pow(size_, 2);
  bite_strength_ = mutable_.GetGeneticStrength() * size_;
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
// Food *Creature::GetClosestFood(
//     std::vector<std::vector<std::vector<Entity *>>> &grid,
//     double GridCellSize) const {
//   if (grid.empty()) return nullptr;
//   std::pair<double, double> coordinates_creature = GetCoordinates();
//   int i_creature = (int)coordinates_creature.first / (int)GridCellSize;
//   int j_creature = (int)coordinates_creature.second /
//                    (int)GridCellSize;  // position of the creature on the grid
//   std::vector<Food *> closest_food_entities = get_food_at_distance(
//       grid, i_creature, j_creature,
//       0);  // here we place the candidates for the closest food
//   int grid_distance = 1;
//   int boundary = std::max(grid.size(), grid[0].size());

//   while (closest_food_entities.size() == 0 && grid_distance <= boundary) {
//     // std::cout << "checking at distance " << grid_distance << std::endl;
//     closest_food_entities =
//         get_food_at_distance(grid, i_creature, j_creature, grid_distance);
//     grid_distance++;
//   }
//   // assert(!closest_food_entities.empty());
//   if (closest_food_entities.empty()) return nullptr;

//   Food *closest_food = closest_food_entities.front();
//   double smallest_distance =
//       GetDistance(*closest_food, settings::environment::kMapWidth,
//                   settings::environment::kMapHeight);
//   for (Food *&food : closest_food_entities) {
//     double distance = GetDistance(*food, settings::environment::kMapWidth,
//                                   settings::environment::kMapHeight);
//     if (distance < smallest_distance) {
//       closest_food = food;
//       smallest_distance = distance;
//     }
//     assert(!closest_food_entities.empty());
//     return closest_food;
//   }
//   return nullptr;
// }

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
std::vector<Food *> get_food_at_distance(
    std::vector<std::vector<std::vector<Entity *>>> &grid, int i_creature,
    int j_creature, int grid_distance) {
  std::vector<Food *> food;
  if (grid.empty()) return food;
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
 * @param radius The radius of the vision, representing how far the creature can see.
 * @param angle The angle of the vision, representing the full angle of the visual field.
 */
void Creature::SetVision(double radius, double angle) {
  vision_radius_ = radius;
  vision_angle_ = angle;
}

double Creature::GetVisionRadius() const { return vision_radius_; }

double Creature::GetVisionAngle() const { return vision_angle_; }



Food* Creature::GetClosestMeatInSight(
    std::vector<std::vector<std::vector<Entity *>>> &grid,
    double grid_cell_size) const {
    return (GetClosestFoodInSight(grid, grid_cell_size, Food::type::meat));
}

// Function to get the closest plant in sight
Food* Creature::GetClosestPlantInSight(
    std::vector<std::vector<std::vector<Entity *>>> &grid,
    double grid_cell_size) const {
    return (GetClosestFoodInSight(grid, grid_cell_size, Food::type::plant));
}

/*!
 * @brief Finds the closest food entity (meat or plant) within the creature's line of sight.
 *
 * @details Performs a breadth-first search (BFS) on the grid cells within the creature's
 * field of view to locate the closest food entity. It utilizes IsGridCellPotentiallyInsideCone
 * functions to efficiently narrow down the search area to relevant cells. The function iterates over
 * entities within these cells, dynamically casting them to Food* to check if they are edible.
 * It then determines if they are within the creature's field of view and closer than any previously found food.
 * The search continues until the closest food is found or a predefined number of cells have been processed.
 *
 * @param grid A 3-dimensional vector representing the environmental grid where each cell contains entities.
 * @param grid_cell_size The size of each square cell in the grid.
 *
 * @return A pointer to the closest food entity within the line of sight; nullptr if no food is found.
 */
Food *Creature::GetClosestFoodInSight(
    std::vector<std::vector<std::vector<Entity *>>> &grid,
    double grid_cell_size, Food::type food_type) const {
  int grid_width = grid.size();
  int grid_height = grid[0].size();

  int x_grid = static_cast<int>(x_coord_ / grid_cell_size);
  int y_grid = static_cast<int>(y_coord_ / grid_cell_size);

  //temporary fix multiply by 4
  int max_cells_to_find_food = M_PI * pow(vision_radius_ + 2 * sqrt(2) * grid_cell_size + settings::environment::kMaxFoodSize, 2) / (grid_cell_size * grid_cell_size);

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

        bool is_on_edge = (food_direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary) <= M_PI/2) && (distance * sin(food_direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary)) <= food->GetSize() + settings::engine::EPS);

        if (is_in_field_of_view) {
          bool is_within_vision_radius =
              distance <= vision_radius_ + food->GetSize() + settings::engine::EPS;
          if (is_within_vision_radius && distance < smallest_distance_food) {
            smallest_distance_food = distance;
            closest_food = food;
            break;
          }
        }

        if (is_on_edge) {
          bool is_within_vision_radius =
              (distance * cos(food_direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary)) <= vision_radius_ + settings::engine::EPS);
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

double Creature::GetStomachCapacity() const {return stomach_capacity_;};
double Creature::GetStomachFullness() const {return stomach_fullness_;};
double Creature::GetEmptinessPercent() const {return 100 * (1 - stomach_fullness_/stomach_capacity_);};
double Creature::GetEnergyInStomach() const {return potential_energy_in_stomach_;};

/*!
 * @brief Handles the creature's digestion of food.
 *
 * @details Increases the creature's energy based on the nutritional value of
 * the food digested. If the resulting energy exceeds the maximum energy limit,
 * it triggers a health balance routine. Then empties out stomach.
 *
 * @param nutritional_value The nutritional value of the food to be digested.
 */
void Creature::Digest(double deltaTime)
{

  double quantity = std::min(deltaTime * settings::physical_constraints::KDDigestionRate, stomach_acid_);
  quantity = std::min(quantity,  stomach_fullness_);

  if (quantity < settings::engine::EPS || stomach_fullness_ < settings::engine::EPS) { return; };
  double avg_nutritional_value = potential_energy_in_stomach_ / stomach_fullness_;

  // Digests the food, increasing energy
  SetEnergy(GetEnergy() + quantity * avg_nutritional_value);
  if (GetEnergy() > max_energy_) {
    BalanceHealthEnergy();
  }

  // Empties out the stomach space
  stomach_acid_ -= quantity;
  potential_energy_in_stomach_ -= quantity * avg_nutritional_value;
  stomach_fullness_ -= quantity;
}

/*!
 * @brief Handles the biting of the food.
 *
 * @details Adds the food it bites to the stomach (increasing fulness and potential
 * energy). Decreases food size/deletes food that gets bitten.
 *
 * @param food The food the creature bites into.
 */
void Creature::Bite(Food* food)
{
  //Reset eating cooldown, makes creature stop to bite
  eating_cooldown_ = mutable_.GetEatingSpeed();

  //Bite logic
  double max_nutrition = 0;

  //Check how much food the creature can eat, depending on bite strength and fullness of stomach
  double available_space = std::max(stomach_capacity_ - stomach_fullness_, 0.0);
  double food_to_eat = std::sqrt(std::min(pow(bite_strength_,2), available_space));

  // Check if creature eats the whole food or a part of it
  if (food_to_eat >= food->GetSize())
  {
    max_nutrition = food->GetNutritionalValue() * food->GetSize();
    stomach_fullness_ += pow(food->GetSize(), 2);
    food->Eat();
  }
  else
  {
    double initial_food_size = food->GetSize();
    double new_radius = std::sqrt(pow(initial_food_size,2) - pow(food_to_eat,2));
    food->SetSize(new_radius);
    stomach_fullness_ += pow(food_to_eat, 2);
    max_nutrition =  food->GetNutritionalValue() * food_to_eat;
  }

  // Herbivore/carnivore multiplier
  if (Plant *plant = dynamic_cast<Plant *>(food)) {
    max_nutrition = max_nutrition * 2 * (1 - mutable_.GetDiet());
  }
  else if (Meat *meat = dynamic_cast<Meat *>(food)) {
    max_nutrition = max_nutrition * 2 * mutable_.GetDiet();
  }

  //Add nutrition to stomach, make sure capacity is not surpassed
  potential_energy_in_stomach_ += max_nutrition;
  if (stomach_fullness_ > stomach_capacity_)
  {
    stomach_fullness_ = stomach_capacity_;
  }
}

void Creature::AddAcid(double quantity)
{
  double initial_acid = stomach_acid_;
  stomach_acid_ = std::min(stomach_capacity_, stomach_acid_ + quantity);
  SetEnergy(GetEnergy() - (stomach_acid_ - initial_acid)/settings::physical_constraints::KDAcidToEnergy);
}

double Creature::GetAcid() const {return stomach_acid_; };

bool Creature::IsFoodInSight(Food *food)
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
