#include "creature.h"
#include "grabbing_entity.h"

#include <algorithm>
#include <cassert>
#include <queue>
#include <set>

#include "collision_functions.h"
#include "settings.h"

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
 */
Creature::Creature(neat::Genome genome, Mutable mutables)
    : GrabbingEntity(),
      AliveEntity(genome, mutables),
      VisionSystem(genome, mutables),
      DigestiveSystem(genome, mutables),
      ReproductiveSystem(genome,mutables)
      {
    size_ = mutables.GetBabySize();
    health_ = mutables.GetIntegrity() * pow(size_, 2);
    energy_ = mutables.GetEnergyDensity() * pow(size_, 2);
    int neural_inputs = SETTINGS.environment.input_neurons;
    for (BrainModule module : genome.GetModules()){
        neural_inputs += module.GetInputNeuronIds().size();
    }
    neuron_data_ = std::vector<double>(neural_inputs, 0.0);
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
  return brain_distance + mutable_distance < SETTINGS.compatibility.compatibility_threshold;
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
  age_ += deltaTime;

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
 * @brief Handles the collision of the creature with another entity.
 *
 * @details Processes the interaction when the creature collides with another
 * entity. If the creature is hungry, the creature consumes it, if the creature wants to grab it, it does so. Otherwise, standard
 * collision handling is performed.
 *
 * @param other_entity The entity the creature collides with.
 * @param kMapWidth Width of the map.
 * @param kMapHeight Height of the map.
 */

void Creature::OnCollision(Entity &other_entity, double const kMapWidth, double const kMapHeight) {
  if (other_entity.GetState() == Entity::Alive && eating_cooldown_ == 0.0 && biting_ == 1 && IsInSight(&other_entity)) {

    SetEnergy(GetEnergy() - bite_strength_ * SETTINGS.physical_constraints.d_bite_energy_consumption_ratio);

    if (Food* food_entity = dynamic_cast<Food*>(&other_entity)) {
      DigestiveSystem::Bite(food_entity);
    } else if (Creature* creature_entity = dynamic_cast<Creature*>(&other_entity)) {
      Bite(creature_entity);
    }
  }
  if (grabbing_ && IsInSight(&other_entity) && !(this->GetGrabbedEntity())){ //checking if the creature wants to bite, has the entity in sight and if he is not already grabbing something
      Grab(&other_entity);
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

  think_count++;
  if (think_count % 5 != 0){
      return;
  }
  think_count = 0;
  // To allow creatures to use a module it should be included below
  ProcessVisionFood(grid, GridCellSize, width, height);
  ProcessVisionEnemies(grid, GridCellSize, width, height);

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


  for (BrainModule module : GetGenome().GetModules()){
    if (module.GetModuleId() == 1){ //Geolocation Module
        int i  = module.GetFirstInputIndex();
        neuron_data_.at(i) = x_coord_;
        neuron_data_.at(i + 1) = y_coord_;
        neuron_data_.at(i + 2) = orientation_;
    }
  }

  std::vector<double> output = brain_.Activate(neuron_data_);
  SetAcceleration(std::tanh(output.at(0))*mutable_.GetMaxForce());
  SetAccelerationAngle(std::tanh(output.at(1)) * M_PI);
  SetRotationalAcceleration(std::tanh(output.at(2))*mutable_.GetMaxForce());
  Grow(std::max(std::tanh(output.at(3)) * deltaTime, 0.0));
  AddAcid(std::max(std::tanh(output.at(4)) * 10.0, 0.0));
  biting_ = std::tanh(output.at(5)) > 0 ? 0 : 1;
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
//  std::cout << "Size difference: "<< size - GetSize() << "Energy: " << energy << "Growth Factor: " << mutable_.GetGrowthFactor() << std::endl;
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
//       GetDistance(*closest_food, SETTINGS.environment.map_width,
//                   SETTINGS.environment.map_height);
//   for (Food *&food : closest_food_entities) {
//     double distance = GetDistance(*food, SETTINGS.environment.map_width,
//                                   SETTINGS.environment.map_height);
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
 * @brief Finds the closest enemy creature within the creature's line of sight.
 *
 * @details Performs a breadth-first search (BFS) on the grid cells within the creature's
 * field of view to locate the closest enemy creature. A creature is considered an enemy if it
 * is not compatible with it.
 *
 * @param grid A 3-dimensional vector representing the environmental grid where each cell contains entities.
 * @param grid_cell_size The size of each square cell in the grid.
 *
 * @return A pointer to the closest food entity within the line of sight; nullptr if no food is found.
 */
Creature *Creature::GetClosestEnemyInSight(
    std::vector<std::vector<std::vector<Entity *>>> &grid,
    double grid_cell_size){
  int grid_width = grid.size();
  int grid_height = grid[0].size();

  int x_grid = static_cast<int>(x_coord_ / grid_cell_size);
  int y_grid = static_cast<int>(y_coord_ / grid_cell_size);

         //temporary fix multiply by 4
  int max_cells_to_find_food = 4 * M_PI * pow(vision_radius_ + 2 * sqrt(2) * grid_cell_size + settings::environment::kMaxFoodSize, 2) / (grid_cell_size * grid_cell_size);

  auto cone_center = Point(x_coord_, y_coord_);
  auto cone_orientation = GetOrientation();
  auto cone_left_boundary = OrientedAngle(cone_orientation - vision_angle_ / 2);
  auto cone_right_boundary =
      OrientedAngle(cone_orientation + vision_angle_ / 2);

  Creature *closest_enemy = nullptr;
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

    for (Entity *entity : grid[x][y]) {
      Creature *creature = dynamic_cast<Creature *>(entity);

      if (creature && creature->GetState() == Entity::states::Alive && Compatible(*creature) == 0) {
        auto point = Point(entity->GetCoordinates());

        auto direction = OrientedAngle(cone_center, point);

        double distance = point.dist(cone_center);

        bool is_in_field_of_view = (direction.IsInsideCone(
            cone_left_boundary, cone_right_boundary));

        bool is_on_edge = (direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary) <= M_PI/2) && (distance * sin(direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary)) <= creature->GetSize() + settings::engine::EPS);

        if (is_in_field_of_view) {
          bool is_within_vision_radius =
              distance <= vision_radius_ + creature->GetSize() + settings::engine::EPS;
          if (is_within_vision_radius && distance < smallest_distance_enemy) {
            smallest_distance_enemy = distance;
            closest_enemy = creature;
            break;
          }
        }

        if (is_on_edge) {
          bool is_within_vision_radius =
              (distance * cos(direction.AngleDistanceToCone(cone_left_boundary, cone_right_boundary)) <= vision_radius_ + settings::engine::EPS);
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

void Creature::ProcessVisionEnemies(std::vector<std::vector<std::vector<Entity *>>> &grid,
                       double grid_cell_size, double width, double height)
{
  Creature *closeEnemy = GetClosestEnemyInSight(grid, grid_cell_size);

  if (closeEnemy){
    distance_enemy_ = this->GetDistance(*closeEnemy, width, height) - (*closeEnemy).GetSize();
    orientation_enemy_ = this->GetRelativeOrientation(*closeEnemy);
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
 * @details Adds the food it bites to the stomach (increasing fulness and potential
 * energy). Decreases food size/deletes food that gets bitten.
 *
 * @param food The food the creature bites into.
 */
void Creature::Bite(Creature* creature)
{
  //Reset eating cooldown, makes creature stop to bite
  eating_cooldown_ = mutable_.GetEatingSpeed();

  //Bite logic - inflict damage, add energy
  const double damage = M_PI*pow(bite_strength_,2)*SETTINGS.physical_constraints.d_bite_damage_ratio;
  creature->SetHealth(creature->GetHealth()-damage);
  SetEnergy(GetEnergy()+bite_strength_*SETTINGS.physical_constraints.d_bite_energy_consumption_ratio/10);
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
void Creature::Grab(Entity* entity){
    this->SetGrabbedEntity(dynamic_cast<MovableEntity*>(entity));
    dynamic_cast<GrabbingEntity*>(entity)->AddToGrabbingEntities(this);
    SetEnergy(GetEnergy() - entity->GetSize());
    this->UpdateEntityVelocities();
}



