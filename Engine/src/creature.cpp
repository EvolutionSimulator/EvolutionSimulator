#include "creature.h"

#include <algorithm>
#include <cassert>

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
Creature::Creature(neat::Genome genome)
    : MovableEntity(),
      health_(100),
      energy_(100),
      brain_(neat::NeuralNetwork(genome)),
      genome_(genome),
      neuron_data_(settings::environment::kInputNeurons, 0),
      reproduction_cooldown_(settings::environment::kReproductionCooldown),
      age_(0) {}

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
    SetHealth(GetHealth() + GetEnergy() - 5);
    SetEnergy(5);
    if (GetHealth() >= (GetEnergy() - 5)) {
      SetHealth(GetHealth() + (GetEnergy() - 5));
      SetEnergy(5);
    } else {
      SetHealth((GetHealth() + GetEnergy()) / 2);
      SetEnergy(GetHealth());
    }
  } else if (GetHealth() < 0) {
    if (GetEnergy() >= (GetHealth() - 5)) {
      SetEnergy(GetEnergy() - GetHealth() - 5);
      SetHealth(5);
    } else {
      SetHealth((GetHealth() + GetEnergy()) / 2);
      SetEnergy(GetHealth());
    }
  } else if (GetEnergy() > GetMaxEnergy()) {
    SetHealth(GetHealth() + (GetEnergy() - GetMaxEnergy()));
    SetEnergy(GetMaxEnergy());
  } else if (GetHealth() > GetEnergy() &&
             GetEnergy() <= settings::environment::kHealthToEnergy) {
    SetEnergy(GetEnergy() + 5);
    SetHealth(GetHealth() - 5);
  } else if (GetHealth() < GetEnergy() &&
             GetEnergy() >= settings::environment::kEnergyToHealth) {
    SetEnergy(GetEnergy() - 5);
    SetHealth(GetHealth() + 5);
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
  if (health > 100) {
    health_ = 100;
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
  SetEnergy(GetEnergy() -
            (fabs(GetAcceleration()) + fabs(GetRotationalAcceleration()) + 50) *
                GetSize() * deltaTime / 100);

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
      reproduction_cooldown_ == 0.0) {
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
  SetEnergy(GetEnergy() - 0.75 * max_energy_);
  reproduction_cooldown_ = settings::environment::kReproductionCooldown;
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
 * @brief Sets the maximum energy level of the creature.
 *
 * @details This method ensures the maximum energy level is not set below a
 * minimum threshold, which is calculated as twice the creature's size. If the
 * provided max_energy is less than this threshold, the maximum energy is set to
 * the threshold value instead.
 *
 * @param max_energy The desired maximum energy level.
 */
void Creature::SetMaxEnergy(double max_energy) {
  if (max_energy < GetSize() * 2) {
    max_energy_ = GetSize() * 2;
  } else {
    max_energy_ = max_energy;
  }
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
 * @details Adjusts the maximum energy based on the difference in age and then
 * sets the new age. The maximum energy is reduced proportionally to the
 * decrease in age.
 *
 * @param age The new age to be set.
 */
void Creature::SetAge(double age) {
  SetMaxEnergy(GetMaxEnergy() - (GetAge() - age));

  age_ = age;
}

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
  this->UpdateEnergy(deltaTime);
  this->UpdateVelocities(deltaTime);
  this->Move(deltaTime, kMapWidth, kMapHeight);
  this->Rotate(deltaTime);
  this->Think(grid, GridCellSize);
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
      Eats(food->GetNutritionalValue() * food->GetSize());
      food->Eat();
    }
  } else {
    MovableEntity::OnCollision(other_entity, kMapWidth, kMapHeight);
  }
}

/*!
 * @brief Sets the growth factor of the creature.
 *
 * @details The growth factor determines how much the creature grows in size
 * relative to the energy consumed.
 *
 * @param growth_factor The new growth factor.
 */
void Creature::SetGrowthFactor(double growth_factor) {
  growth_factor_ = growth_factor;
}

/*!
 * @brief Retrieves the growth factor of the creature.
 *
 * @details Returns the current growth factor, which affects the creature's size
 * increase relative to energy intake.
 *
 * @return The current growth factor of the creature.
 */
double Creature::GetGrowthFactor() { return growth_factor_; }

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
                     double GridCellSize) {
  // Not pretty but we'll figure out a better way in the future
  ProcessVisionFood(grid, GridCellSize);
  neuron_data_.at(0) = energy_;
  neuron_data_.at(1) = GetVelocity();
  neuron_data_.at(2) = GetVelocityAngle();
  neuron_data_.at(3) = GetRotationalVelocity();
  neuron_data_.at(4) = orientation_food_;
  neuron_data_.at(5) = distance_food_;
  std::vector<double> output = brain_.Activate(neuron_data_);
  SetAcceleration(std::tanh(output.at(0))*max_force_);
  SetAccelerationAngle(output.at(1));
  SetRotationalAcceleration(output.at(2));
}

/*!
 * @brief Processes the creature's vision to locate food.
 *
 * @details Determines the closest food entity based on the creature's position
 * and updates the creature's orientation and distance metrics towards the
 * located food.
 *
 * @param grid The environmental grid.
 * @param GridCellSize Size of each cell in the grid.
 */
void Creature::ProcessVisionFood(
    std::vector<std::vector<std::vector<Entity *>>> &grid,
    double GridCellSize) {
  Food *food = this->GetClosestFood(grid, GridCellSize);
  if (food == nullptr) { //temporary fix that shouldn't be necessary after the new vision system is implemented
      distance_food_ = 1000;
      orientation_food_ = 0;
      return;
  }
  distance_food_ = this->GetDistance(*food, settings::environment::kMapWidth,
                                     settings::environment::kMapHeight);
  orientation_food_ = this->GetRelativeOrientation(*food);
}

/*!
 * @brief Retrieves the maximum size the creature can grow to.
 *
 * @details Returns the maximum size limit for the creature, beyond which it
 * cannot grow regardless of energy intake.
 *
 * @return The maximum size of the creature.
 */
double Creature::GetMaxSize() { return max_size_; }

/*!
 * @brief Sets the maximum size the creature can grow to.
 *
 * @details This method establishes an upper limit for the creature's size. If
 * the creature grows beyond this limit, its size is capped at this maximum
 * value.
 *
 * @param max_size The new maximum size limit for the creature.
 */
void Creature::SetMaxSize(double max_size) { max_size_ = max_size; }

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
  double size = GetSize() + energy * GetGrowthFactor();
  (size > GetMaxSize()) ? SetSize(GetMaxSize()) : SetSize(size);
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
Food *Creature::GetClosestFood(
    std::vector<std::vector<std::vector<Entity *>>> &grid,
    double GridCellSize) const {
  if (grid.empty()) return nullptr;
  std::pair<double, double> coordinates_creature = GetCoordinates();
  int i_creature = (int)coordinates_creature.first / (int)GridCellSize;
  int j_creature = (int)coordinates_creature.second /
                   (int)GridCellSize;  // position of the creature on the grid
  std::vector<Food *> closest_food_entities = get_food_at_distance(
      grid, i_creature, j_creature,
      0);  // here we place the candidates for the closest food
  int grid_distance = 1;
  int boundary = std::max(grid.size(), grid[0].size());

  while (closest_food_entities.size() == 0 && grid_distance <= boundary) {
    // std::cout << "checking at distance " << grid_distance << std::endl;
    closest_food_entities =
        get_food_at_distance(grid, i_creature, j_creature, grid_distance);
    grid_distance++;
  }
  // assert(!closest_food_entities.empty());
  if (closest_food_entities.empty()) return nullptr;

  Food *closest_food = closest_food_entities.front();
  double smallest_distance =
      GetDistance(*closest_food, settings::environment::kMapWidth,
                  settings::environment::kMapHeight);
  for (Food *&food : closest_food_entities) {
    double distance = GetDistance(*food, settings::environment::kMapWidth,
                                  settings::environment::kMapHeight);
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
