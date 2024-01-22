#include "../include/creature_manager.h"

#include "settings.h"

CreatureManager::CreatureManager() {}

/*!
 * @brief Modifies the positions of all creatures in the simulation.
 *
 * @param delta_x The change in x-coordinate for each creature.
 * @param delta_y The change in y-coordinate for each creature.
 */

void CreatureManager::ModifyAllCreatures(SimulationData& data, double delta_x,
                                         double delta_y) {
  std::pair<double, double> coordinates;
  for (Creature& creature : data.creatures_) {
    coordinates = creature.GetCoordinates();
    coordinates.first = coordinates.first + delta_x;
    coordinates.second = coordinates.second + delta_y;
    creature.SetCoordinates(coordinates.first, coordinates.second,
                            SETTINGS.environment.map_width,
                            SETTINGS.environment.map_height);
  }
}

/*!
 * @brief Updates the state of all creatures for a given time interval.
 *
 * @param deltaTime The time interval for which the creatures' states are
 * updated.
 */

void CreatureManager::UpdateAllCreatures(SimulationData& data,
                                         Environment& environment,
                                         EntityGrid& entity_grid,
                                         double deltaTime) {
  auto grid = entity_grid.GetGrid();
  for (Egg& egg : data.eggs_) {
    egg.Update(deltaTime);
  }

  for (int creature_index = 0; creature_index < data.creatures_.size();
       creature_index++) {
    Creature& creature = data.creatures_[creature_index];
    creature.Update(deltaTime, SETTINGS.environment.map_width,
                    SETTINGS.environment.map_height, grid,
                    SETTINGS.environment.grid_cell_size,
                    environment.GetFrictionalCoefficient());
    if (creature.GetMatingDesire()) {
      data.new_reproduce_.push(creature_index);
    }
    if (creature.FemaleReproductiveSystem::CanBirth()) {
      data.eggs_.push_back(creature.FemaleReproductiveSystem::GiveBirth(
          creature.GetCoordinates()));
      creature.SetVelocity(
          creature.GetVelocity() *
          SETTINGS.physical_constraints.after_birth_velocity_factor);
    }
  }
}

void CreatureManager::HatchEggs(SimulationData& data) {
  std::remove_if(data.eggs_.begin(), data.eggs_.end(), [this](Egg& egg) {
    if (egg.GetAge() >= egg.GetIncubationTime()) {
      Creature new_creature = egg.Hatch();
      new_creature.SetCoordinates(
          egg.GetCoordinates().first, egg.GetCoordinates().second,
          environment_.GetMapWidth(), environment_.GetMapHeight());
      data.creatures_.push_back(new_creature);
      return true;
    }
    return false;
  });
}

/*!
 * @brief Handles the reproduction process of creatures in the simulation.
 *
 * @details Pairs creatures by compatibility from the reproduction queue and
 * creates offspring with crossed-over genomes.
 */
void CreatureManager::ReproduceCreatures(SimulationData& data,
                                         Environment& environment) {
  double world_width = environment.GetMapWidth();
  double world_height = environment.GetMapHeight();
  double max_creature_size = settings::environment::kMaxCreatureSize;
  double min_creature_size = settings::environment::kMinCreatureSize;

  std::queue<int> not_reproduced;
  std::queue<int> temp_queue;

  while (!data.reproduce_.empty()) {
    int creature1_index = data.reproduce_.front();
    Creature& creature1 = data.creatures_[creature1_index];
    data.reproduce_.pop();
    bool paired = false;

    // Attempt to pair creature1 with a compatible new creature
    while (!data.new_reproduce_.empty() && !paired) {
      int creature2_index = data.new_reproduce_.front();
      Creature& creature2 = data.creatures_[creature2_index];
      data.new_reproduce_.pop();
      // If these two creatures are compatible reproduce them otherwise
      // save the creature in a temporary queue for the next pairing round
      if (creature1.Compatible(creature2) and
          creature1.MaleReproductiveSystem::ReadyToProcreate() and
          creature2.FemaleReproductiveSystem::ReadyToProcreate()) {
        ReproduceTwoCreatures(data, creature1, creature2);
        paired = true;
      } else {
        temp_queue.push(creature2_index);  // Save for next round
      }
    }

    // If the creature wasn't paired add it to not_reproduced
    if (!paired) {
      not_reproduced.push(creature1_index);
    }

    // Refill newCreatures with unpaired creatures for next attempt
    while (!temp_queue.empty()) {
      data.new_reproduce_.push(temp_queue.front());
      temp_queue.pop();
    }
  }

  // Refill reproduce_ with creatures for the next iteration
  while (!data.new_reproduce_.empty()) {
    data.reproduce_.push(data.new_reproduce_.front());
    new_reproduce_.pop();
  }

  // Refill reproduce_ with the remaining creatures
  while (!not_reproduced.empty()) {
    data.reproduce_.push(not_reproduced.front());
    not_reproduced.pop();
  }
}

/*!
 *  @brief Reproduces two creatures and adds a descendant to the simulation
 *
 *  @details Takes two creatures and uses the crossover functions for genomes
 * and mutables to create a child creature out of the previous two. We take as
 * the dominant creature for the algorithms that that has the highest energy at
 * the moment of reproduction
 */
void CreatureManager::ReproduceTwoCreatures(SimulationData& data,
                                            Creature& father,
                                            Creature& mother) {
  father.MaleReproductiveSystem::MateWithFemale();
  mother.FemaleReproductiveSystem::MateWithMale(&father, &mother);
  father.AfterMate();
  mother.AfterMate();
}

/*!
 * @brief Initializes creatures randomly on the map, mutating their genome
 * multiple times.
 */
void CreatureManager::InitializeCreatures(SimulationData& data,
                                          Environment& environment) {
  // Retrieve information from the environment
  double world_width = SETTINGS.environment.map_width;
  double world_height = SETTINGS.environment.map_height;
  double creature_density = environment.GetCreatureDensity();
  double max_creature_size = SETTINGS.environment.max_creature_size;
  double min_creature_size = SETTINGS.environment.min_creature_size;

  data.creatures_.clear();
  neat::Genome genome = neat::minimallyViableGenome();
  for (double x = 0; x < world_width; x += 2.0) {
    for (double y = 0; y < world_height; y += 2.0) {
      if (std::rand() / (RAND_MAX + 1.0) < creature_density) {
        // neat::Genome genome(SETTINGS.environment.input_neurons,
        //                     SETTINGS.environment.output_neurons);
        Mutable mutables;
        for (int i = 0; i < 40; i++) {
          mutables.Mutate();
        }
        Creature new_creature(genome, mutables);
        new_creature.RandomInitialization(world_width, world_height);
        data.creatures_.emplace_back(new_creature);
      }
    }
  }
}
