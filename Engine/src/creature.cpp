#include "creature.h"

#include <algorithm>
#include <cassert>
#include <queue>
#include <set>

#include "collision_functions.h"
#include "mathlib.h"
#include "settings.h"
#include "grabbing_entity.h"

/*!
 * @brief Construct a new Creature object.
 *
 * @details Initializes a Creature instance with a given NEAT genome. The
 * constructor sets up various creature attributes like health, energy, brain
 * (neural network), genome, neuron data, reproduction cooldown, and age. It
 * also initializes the creature as a movable entity.
 *
 * @param genome A `neat::Genome´object used to initialize the brain of the
 * AliveEntity.
 * @param mutables A Mutable object used to initialize the properties of the
 * AliveEntity.
 *
 * * Default properties:
 * - Health: 100
 * - Energy: 100
 * - Brain (Neural Network): Constructed from the provided genome.
 * - Neuron Data: Initialized with zeros (size equal to
 * `settings::environment::kInputNeurons`).
 * - Reproduction Cooldown: Set to
 * `settings::environment::kReproductionCooldown`.
 * - Age: 0
 * - Stomach capacity: area of creature *
 * settings::environment::kDStomachCapacityFactor
 */

Creature::Creature(neat::Genome genome, Mutable mutables)
    : Entity(),
      MovableEntity(),
      AliveEntity(genome, mutables),
      VisionSystem(genome, mutables),
      DigestiveSystem(genome, mutables),
      ReproductiveSystem(genome, mutables),
      MaleReproductiveSystem(genome, mutables),
      FemaleReproductiveSystem(genome, mutables),
      PheromoneSystem(genome, mutables),
      mating_desire_(false)
      {
    think_count_ = this->GetID();
    color_hue_ = mutables.GetColor();
    distance_enemy_ = mutables.GetVisionFactor();
    orientation_enemy_ = 0;
    enemy_size_ = 0;
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

  SetEnergy(GetEnergy() - movement_energy*pregnancy_hardship_ - heat_loss);
  BalanceHealthEnergy();

  if (GetHealth() <= 0) {
    Dies();
  }
}

/*!
 * @brief Updates the creature's desire to mate
 *
 * @details This method modifies the boolean desire of creatures to mate.
 * For females, can mate in the age range kMinReproducingAge to
 * kMaxReproducingAge, with a probability that is inversely proportional to age,
 * while for males can mate past kMinProducingAge with falling probability.
 */
void Creature::UpdateMatingDesire() {
  if (!this->MaleReproductiveSystem::ReadyToProcreate() &&
      !(this->FemaleReproductiveSystem::ReadyToProcreate())) {
    mating_desire_ = false;
    return;
  }

  if (this->GetAge() >= SETTINGS.physical_constraints.max_reproducing_age) {
    mating_desire_ = false;
    return;
  }

  double min_reproducing_age =
      std::min(this->MaleReproductiveSystem::GetMaturityAge(),
               this->FemaleReproductiveSystem::GetMaturityAge());
  if (energy_ < std::max(mutable_.GetEnergyDensity() * pow(size_, 2) * 0.25, max_energy_ * 0.6)) {
      mating_desire_ = false; //To avoid creatures that just reproduce
      return;
  }
  double probability =
      1 - (this->GetAge() - min_reproducing_age) /
              (SETTINGS.physical_constraints.max_reproducing_age -
               min_reproducing_age) *
              SETTINGS.physical_constraints.mating_desire_factor;
  mating_desire_ = mathlib::RandomDouble(0, 1) < probability;
}




/*!
 * @brief Determines if the current creature is compatible with another
 * creature.
 *
 * @details This function calculates the compatibility between the current
 * creature and another creature based on their genomes and mutable
 * characteristics. It sums the brain distance, derived from genome
 * compatibility, with the mutable distance, derived from mutable
 * characteristics compatibility. The creatures are considered compatible if the
 * sum is less than a predefined compatibility threshold.
 *
 * @param other_creature A reference to another `Creature` object for
 * compatibility comparison.
 * @return bool Returns `true` if the sum of brain and mutable distances is less
 * than the compatibility threshold, indicating compatibility; otherwise returns
 * `false`.
 */
bool Creature::Compatible(const std::shared_ptr<Creature>other_creature) {
  if (this->GetID() == other_creature->GetID()) return false;
  double brain_distance =
      this->GetGenome().CompatibilityBetweenGenomes(other_creature->GetGenome());
  double mutable_distance = this->GetMutable().CompatibilityBetweenMutables(
      other_creature->GetMutable());
  double physical_distance =
      this->GetDistance(other_creature, SETTINGS.environment.map_width,
                        SETTINGS.environment.map_height);
  bool flag = brain_distance + mutable_distance <
                  SETTINGS.compatibility.compatibility_threshold &&
              physical_distance < SETTINGS.compatibility.compatibility_distance;
  return brain_distance + mutable_distance <
             SETTINGS.compatibility.compatibility_threshold &&
         physical_distance < SETTINGS.compatibility.compatibility_distance;
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
                      std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
                      double GridCellSize, double frictional_coefficient) {
  if (state_ == Dead) return;
  this->frictional_coefficient_ = frictional_coefficient;
  this->UpdateMaxEnergy();
  // this->SetAffectedByGrabbedEnttityAll(false);
  // this->SetGrabValues();
  this->UpdateVelocities(deltaTime);
  this->Move(deltaTime, kMapWidth, kMapHeight);
  this->Rotate(deltaTime);  //, kMapWidth, kMapHeight);
  this->Think(grid, GridCellSize, deltaTime, kMapWidth, kMapHeight);
  this->Digest(deltaTime);
  this->Grow(energy_/(1 + max_energy_) * deltaTime / 100);
  this->UpdateMatingDesire();
  this->FemaleReproductiveSystem::Update(deltaTime);
  this->MaleReproductiveSystem::Update(deltaTime);
  this->UpdateAge(deltaTime);
  this->UpdateEnergy(deltaTime);


  if (eating_cooldown_ <= 0) {
    eating_cooldown_ = 0.0;
  } else {
    eating_cooldown_ -= deltaTime;
  }
}

/*!
 * @brief Handles the collision of the creature with another entity.
 *
 * @details Processes the interaction when the creature collides with another
 * entity. If the creature is hungry, the creature consumes it, if the creature
 * wants to grab it, it does so. Otherwise, standard collision handling is
 * performed.
 *
 * @param other_entity The entity the creature collides with.
 * @param kMapWidth Width of the map.
 * @param kMapHeight Height of the map.
 */
void Creature::OnCollision(std::shared_ptr<Entity>other_entity, double const kMapWidth, double const kMapHeight) {
  if (std::dynamic_pointer_cast<Pheromone>(other_entity) != nullptr) return;
  if (other_entity->GetState() == Entity::Alive && eating_cooldown_ == 0.0 && biting_ == 1 && IsInSight(other_entity)) {
    SetEnergy(GetEnergy() - bite_strength_ * SETTINGS.physical_constraints.d_bite_energy_consumption_ratio);
    if (std::shared_ptr<Food> food_entity = std::dynamic_pointer_cast<Food>(other_entity)) {
      DigestiveSystem::Bite(food_entity);
    } else if (std::shared_ptr<Creature>creature_entity = std::dynamic_pointer_cast<Creature>(other_entity)) {
            Bite(creature_entity);
    }
  }

  // if (other_entity->GetState() == Entity::Alive && grabbing_ && IsInSight(other_entity) && !(this->GetGrabbedEntity())){
  //   //checking if the creature wants to grab, has the entity in sight and if he is not already grabbing something
  //     Grab(other_entity);
  // }
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
void Creature::Think(std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
                     double GridCellSize, double deltaTime, double width, double height) {
  // Not pretty but we'll figure out a better way in the future

  think_count_++;
  if (think_count_ % 5 != 0){
      return;
  }
  think_count_ = 0;
  // To allow creatures to use a module it should be included below
  ProcessVisionFood(grid, GridCellSize, width, height);
  ProcessVisionEnemies(grid, GridCellSize, width, height);
  ProcessPheromoneDetection(grid, GridCellSize);

  if (neuron_data_.size() == 0) return;
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
  neuron_data_.at(12) = orientation_enemy_;
  neuron_data_.at(13) = distance_enemy_;
  neuron_data_.at(14) = enemy_size_;

  for (BrainModule module : GetGenome().GetModules()) {
    if (module.GetModuleId() == 1) {  // Geolocation Module
      int i = module.GetFirstInputIndex();
      neuron_data_.at(i) = x_coord_;
      neuron_data_.at(i + 1) = y_coord_;
      neuron_data_.at(i + 2) = orientation_;
    }

    if (module.GetModuleId() == 2){ //Pheromone Module
        int i = module.GetFirstInputIndex();
        int type = module.GetType();
        neuron_data_.at(i) = pheromone_densities_.at(type);
    }
  }

  std::vector<double> output = brain_.Activate(neuron_data_);
  SetAcceleration(std::tanh(output.at(0)) * mutable_.GetMaxForce());
  SetAccelerationAngle(std::tanh(output.at(1)) * M_PI);
  SetRotationalAcceleration(std::tanh(output.at(2))*mutable_.GetMaxForce());
  // Grow(std::max(std::tanh(output.at(3)) * deltaTime, 0.0));
  AddAcid(std::max(std::tanh(output.at(4)) * 10.0, 0.0));
  biting_ = std::tanh(output.at(5)) > 0 ? 1 : 0;



  for (BrainModule& module : GetGenome().GetModules()){
      if (module.GetModuleId() == 2){
          int i = module.GetFirstOutputIndex();
          int type = module.GetType();
          pheromone_emissions_.at(type) = output.at(i);
      }
  }

  // grabbing_ = std::tanh(output.at(6)) > 0 ? 0 : 1;
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
  //  std::cout << "Size difference: "<< size - GetSize() << "Energy: " <<
  //  energy << "Growth Factor: " << mutable_.GetGrowthFactor() << std::endl;
  (size > mutable_.GetMaxSize()) ? SetSize(mutable_.GetMaxSize())
                                 : SetSize(size);
  SetEnergy(GetEnergy() - energy);
  stomach_capacity_ = mutable_.GetStomachCapacityFactor() * pow(size_, 2);
  bite_strength_ = mutable_.GetGeneticStrength() * size_;
}

bool Creature::GetMatingDesire() const { return mating_desire_; }

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
 * @brief Finds the closest enemy creature within the creature's line of sight.
 *
 * @details Performs a breadth-first search (BFS) on the grid cells within the
 * creature's field of view to locate the closest enemy creature. A creature is
 * considered an enemy if it is not compatible with it.
 *
 * @param grid A 3-dimensional vector representing the environmental grid where
 * each cell contains entities.
 * @param grid_cell_size The size of each square cell in the grid.
 *
 * @return A pointer to the closest food entity within the line of sight;
 * nullptr if no food is found.
 */
std::shared_ptr<Creature> Creature::GetClosestEnemyInSight(
    std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
    double grid_cell_size){
  int grid_width = grid.size();
  int grid_height = grid[0].size();

  int x_grid = static_cast<int>(x_coord_ / grid_cell_size);
  int y_grid = static_cast<int>(y_coord_ / grid_cell_size);

         //temporary fix multiply by 4
  int max_cells_to_find_food = 4 * M_PI * pow(vision_radius_ + 2 * sqrt(2) * grid_cell_size + SETTINGS.environment.max_food_size, 2) / (grid_cell_size * grid_cell_size);

  auto cone_center = Point(x_coord_, y_coord_);
  auto cone_orientation = GetOrientation();
  auto cone_left_boundary = OrientedAngle(cone_orientation - vision_angle_ / 2);
  auto cone_right_boundary =
      OrientedAngle(cone_orientation + vision_angle_ / 2);

  std::shared_ptr<Creature> closest_enemy = nullptr;
  double smallest_distance_enemy = std::numeric_limits<double>::max();

  std::queue<std::pair<int, int>> cells_queue;
  std::set<std::pair<int, int>> visited_cells;

  cells_queue.push({x_grid, y_grid});
  visited_cells.insert({x_grid, y_grid});

  size_t processed_cells = 0;

  while (!cells_queue.empty()) {
    auto [x, y] = cells_queue.front();
    cells_queue.pop();
    ++processed_cells;

    for (std::shared_ptr<Entity> entity : grid[x][y]) {
      std::shared_ptr<Creature> creature = std::dynamic_pointer_cast<Creature>(entity);
      if (creature && creature->GetState() == Entity::states::Alive && Compatible(creature) == 0) {
        auto point = Point(entity->GetCoordinates());

        auto direction = OrientedAngle(cone_center, point);

        double distance = point.dist(cone_center);

        bool is_in_field_of_view =
            (direction.IsInsideCone(cone_left_boundary, cone_right_boundary));
        bool is_on_edge = (direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary) <= M_PI/2) && (distance * sin(direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary)) <= creature->GetSize() + SETTINGS.engine.eps);

        if (is_in_field_of_view) {
          bool is_within_vision_radius =
              distance <= vision_radius_ + creature->GetSize() + SETTINGS.engine.eps;
          if (is_within_vision_radius && distance < smallest_distance_enemy) {
            smallest_distance_enemy = distance;
            closest_enemy = creature;
            break;
          }
        }

        if (is_on_edge) {
          bool is_within_vision_radius =
              (distance * cos(direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary)) <= vision_radius_ + SETTINGS.engine.eps);
          if (is_within_vision_radius && distance < smallest_distance_enemy) {
            smallest_distance_enemy = distance;
            closest_enemy = creature;
            break;
          }
        }
      }
    }
    if (closest_enemy) {
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
  return closest_enemy;
}


void Creature::ProcessVisionEnemies(std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
                       double grid_cell_size, double width, double height)
{
  std::shared_ptr<Creature> closeEnemy = GetClosestEnemyInSight(grid, grid_cell_size);

  if (closeEnemy){
    distance_enemy_ = this->GetDistance(closeEnemy, width, height) - closeEnemy->GetSize();
    orientation_enemy_ = this->GetRelativeOrientation(closeEnemy);
    enemy_size_ = closeEnemy->GetSize();
  }
  else {
    distance_enemy_ = vision_radius_;
    orientation_enemy_ = remainder(GetRandomFloat(orientation_- vision_angle_/2, orientation_+ vision_angle_/2), 2*M_PI);
    enemy_size_ = -1;
  }

}

/*!
 * @brief Handles the biting of the creature.
 *
 * @details Adds the food it bites to the stomach (increasing fulness and
 * potential energy). Decreases food size/deletes food that gets bitten.
 *
 * @param food The food the creature bites into.
 */
void Creature::Bite(std::shared_ptr<Creature> creature)
{
  eating_cooldown_ = mutable_.GetEatingSpeed();

  //Bite logic - inflict damage, add energy
  const double damage = M_PI*pow(bite_strength_,2)*SETTINGS.physical_constraints.d_bite_damage_ratio;
  creature->SetHealth(creature->GetHealth()-damage);
\
  const double nutrition = SETTINGS.physical_constraints.d_bite_nutritional_value * M_PI*pow(bite_strength_,2) * 2 * mutable_.GetDiet();
  SetEnergy(GetEnergy()+ nutrition);
}

/*!
 * @brief Handles the grabbing of another creature.
 *
 * @details Sets the entity the creature grabs as the grabbed entity.
 * Adds the creature to the set of entities that grabs the grabbed entity.
 * Decreases energy (depending on grabbed entity size).
 * Updates the velocity.
 *
 * @param entity The entity the creature bites into.
 */
/*
void Creature::Grab(std::shared_ptr<Entity> entity){
    if(std::dynamic_pointer_cast<MovableEntity>(entity)){
        this->SetGrabbedEntity(std::dynamic_pointer_cast<MovableEntity>(entity));
        std::dynamic_pointer_cast<GrabbingEntity>(entity)->AddToGrabbedBy(std::make_shared<GrabbingEntity>(*this));
        SetEnergy(GetEnergy() - entity->GetSize());
        // this->SetGrabValues();
        // this->UpdateEntityVelocities();
    }
}
*/
