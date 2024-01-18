#include "creature_manager.h"

#include "settings.h"

#include <omp.h>

CreatureManager::CreatureManager() {}

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

<<<<<<< HEAD
  #pragma omp parallel for
  for (auto& egg : data.eggs_) {
    egg->Update(deltaTime);
=======
  for (Creature &creature : data.creatures_) {
    creature.Update(deltaTime, SETTINGS.environment.map_width,
                    SETTINGS.environment.map_height, grid,
                    SETTINGS.environment.grid_cell_size,
                    environment.GetFrictionalCoefficient());
    if (creature.Fit()) {
      data.new_reproduce_.push(creature);
      creature.Reproduced();
    }
    std::vector<Pheromone> emissions = creature.EmitPheromones(deltaTime);
    data.pheromones_.insert(data.pheromones_.end(), emissions.begin(), emissions.end());
>>>>>>> da22f30 (Fix to reproduction)
  }
  // Vector to store thread-local reproduce lists
  std::vector<std::vector<std::shared_ptr<Creature>>> local_reproduce_lists(omp_get_max_threads());
  std::vector<std::vector<std::shared_ptr<Pheromone>>> local_pheromone_lists(omp_get_max_threads());

  #pragma omp parallel for
  for (int i = 0; i < data.creatures_.size(); ++i) {
    auto& creature = data.creatures_[i];
    creature->Update(deltaTime, SETTINGS.environment.map_width,
                     SETTINGS.environment.map_height, grid,
                     SETTINGS.environment.grid_cell_size,
                     environment.GetFrictionalCoefficient());

    if (creature->GetMatingDesire() && !creature->WaitingToReproduce()) {
      int thread_id = omp_get_thread_num();
      local_reproduce_lists[thread_id].push_back(creature);
      creature->SetWaitingToReproduce(true);
    }

    if (creature->FemaleReproductiveSystem::CanBirth()) {
      std::cerr << "Creature is ready to give birth" << std::endl;
      data.eggs_.push_back(creature->FemaleReproductiveSystem::GiveBirth(
          creature->GetCoordinates()));
    }
    std::vector<std::shared_ptr<Pheromone>> emissions = creature->EmitPheromones(deltaTime);
        local_pheromone_lists[omp_get_thread_num()].insert(
            local_pheromone_lists[omp_get_thread_num()].end(),
            emissions.begin(), emissions.end());
  }

  // Merge thread-local lists into the global reproduce list
  for (auto &list : local_reproduce_lists) {
    for (auto &creature : list) {
      data.new_reproduce_.push(creature);
    }
  }

  for (auto &list : local_pheromone_lists) {
      data.pheromones_.insert(data.pheromones_.end(), list.begin(), list.end());
  }
}

void CreatureManager::HatchEggs(SimulationData& data, Environment& environment) {
  data.eggs_.erase(std::remove_if(data.eggs_.begin(), data.eggs_.end(), [this, &data](std::shared_ptr<Egg>& egg) {
    if (egg->GetAge() >= egg->GetIncubationTime()) {
      std::shared_ptr<Creature> new_creature = egg->Hatch();
      std::cerr << "Hatched creature" << std::endl;
      data.creatures_.push_back(new_creature);
      return true;
    }
    return false;
  }), data.eggs_.end());
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
  double max_creature_size = SETTINGS.environment.max_creature_size;
  double min_creature_size = SETTINGS.environment.min_creature_size;

  std::queue<std::shared_ptr<Creature>> not_reproduced;
  std::queue<std::shared_ptr<Creature>> temp_queue;

  while (!data.reproduce_.empty()) {
    auto creature1 = data.reproduce_.front();
    data.reproduce_.pop();
    bool paired = false;

    // Attempt to pair creature1 with a compatible new creature
    while (!data.new_reproduce_.empty() && !paired) {
      auto creature2 = data.new_reproduce_.front();
      data.new_reproduce_.pop();
      //If these two creatures are compatible reproduce them otherwise
      //save the creature in a temporary queue for the next pairing round
      if (creature1->Compatible(creature2) and
          creature1->MaleReproductiveSystem::ReadyToProcreate() and
          creature2->FemaleReproductiveSystem::ReadyToProcreate()) {
        std::cerr << "Reproducing creatures" << std::endl;
        ReproduceTwoCreatures(data, *creature1, *creature2);
        paired = true;
      } else {
        temp_queue.push(creature2);  // Save for next round
      }
    }

    // If the creature wasn't paired add it to not_reproduced
    if (!paired) {
      not_reproduced.push(creature1);
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
    data.new_reproduce_.pop();
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
  father.SetWaitingToReproduce(false);
  mother.SetWaitingToReproduce(false);
  father.MaleAfterMate();
  mother.FemaleAfterMate();
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
        std::shared_ptr<Creature> new_creature = std::make_shared<Creature>(genome, mutables);
        new_creature->RandomInitialization(world_width, world_height);
        data.creatures_.push_back(new_creature);
      }
    }
  }
}
