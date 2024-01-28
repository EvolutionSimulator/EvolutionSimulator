#include "creature.h"

#include <algorithm>
#include <cassert>
#include <queue>
#include <set>

#include "collision_functions.h"
#include "random.h"
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
      GetSize() * deltaTime * SETTINGS.environment.movement_energy;
  double heat_loss = mutable_.GetEnergyLoss() * pow(size_, 1) * deltaTime
          * SETTINGS.environment.heat_energy;

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
  mating_desire_ = Random::Double(0, 1) < probability;
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
  return brain_distance + mutable_distance <
             SETTINGS.compatibility.compatibility_threshold;
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
  this->Rotate(deltaTime);
  this->Think(grid, GridCellSize, deltaTime, kMapWidth, kMapHeight);
  this->Digest(deltaTime);
  this->Grow(energy_/(1 + max_energy_) * deltaTime / 100);
  this->AddAcid((energy_ + 10) * deltaTime );
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

void Creature::OnCollision(std::shared_ptr<Entity>other_entity, double kMapWidth, double kMapHeight) {
  if (other_entity->GetState() != Entity::Alive || !IsInRightDirection(other_entity, kMapWidth, kMapHeight) || eating_cooldown_ != 0.0){
      MovableEntity::OnCollision(other_entity, kMapWidth, kMapHeight);
      return;
  }
  SetEnergy(GetEnergy() - bite_strength_ * SETTINGS.physical_constraints.d_bite_energy_consumption_ratio);

  if (std::shared_ptr<Food> food_entity = std::dynamic_pointer_cast<Food>(other_entity)) {
      DigestiveSystem::Bite(food_entity);
  }

  if (std::shared_ptr<Creature>creature_entity = std::dynamic_pointer_cast<Creature>(other_entity)) {
      if (attack_) Bite(creature_entity);
  }
  MovableEntity::OnCollision(other_entity, kMapWidth, kMapHeight);

  // if (other_entity->GetState() == Entity::Alive && grabbing_ && IsInSight(other_entity) && !(this->GetGrabbedEntity())){
  //   //checking if the creature wants to grab, has the entity in sight and if he is not already grabbing something
  //     Grab(other_entity);
  // }
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
  ProcessPheromoneDetection(grid, GridCellSize);

  std::vector<std::shared_ptr<Entity>> closeEntities = GetClosestEntitiesInSight(grid, GridCellSize, width, height);
  if(closeEntities[0]) closest_entity_ = closeEntities[0];

  if (neuron_data_.size() == 0) return;
  neuron_data_.at(0) = 1;
  neuron_data_.at(1) = energy_;
  neuron_data_.at(2) = health_;
  neuron_data_.at(3) = GetVelocity();
  neuron_data_.at(4) = GetVelocityAngle();
  neuron_data_.at(5) = GetRotationalVelocity();
  neuron_data_.at(6) = GetEmptinessPercent();
  ProcessVision(closeEntities[0], 7);

  int entity_counter = 1;
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

    if (module.GetModuleId() == 3){ //Vision Module
        int i = module.GetFirstInputIndex();
        ProcessVision(closeEntities[entity_counter], i);
        entity_counter++;
    }
  }

  std::vector<double> output = brain_.Activate(neuron_data_);

  SetAcceleration(std::tanh(output.at(0)) * mutable_.GetMaxForce());
  SetAccelerationAngle(std::tanh(output.at(1)) * M_PI);
  SetRotationalAcceleration(std::tanh(output.at(2))*mutable_.GetMaxForce());
  attack_ = std::tanh(output.at(3)) > 0 ? 1 : 0;

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
void Creature::ProcessVision(std::shared_ptr<Entity> entity, int start) {
  if (entity){
    neuron_data_.at(start) = this->GetDistance(entity) - entity->GetSize();
    neuron_data_.at(start + 1)= this->GetRelativeOrientation(entity);
    neuron_data_.at(start + 2) = entity->GetSize();
    neuron_data_.at(start + 3) = entity->GetColor();

    std::shared_ptr<Creature> otherCreature = std::dynamic_pointer_cast<Creature>(entity);

    if (otherCreature && Compatible(otherCreature)) { neuron_data_.at(start + 4) = 1;}
    else{ neuron_data_.at(start + 4) = 0; }

  }
  else {
    neuron_data_.at(start) =  vision_radius_;
    neuron_data_.at(start + 1) = remainder(Random::Double(orientation_- vision_angle_/2, orientation_+ vision_angle_/2), 2*M_PI);
    neuron_data_.at(start + 2) = -1;
    neuron_data_.at(start + 3) = 0;
    neuron_data_.at(start + 4) = 0;
  }
}
