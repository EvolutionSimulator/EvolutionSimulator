#include "simulation/creature_manager.h"

#include "core/settings.h"

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
  #pragma omp parallel for
  for (auto& egg : data.eggs_) {
    egg->Update(deltaTime);
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
  for (auto& egg : data.eggs_) {
      if (egg->GetAge() >= egg->GetIncubationTime()){
          std::shared_ptr<Creature> new_creature = egg->Hatch();
          std::cerr << "Hatched creature" << std::endl;
          data.creatures_.push_back(new_creature);
          egg->SetState(Entity::Dead);
      }
  }
  data.eggs_.erase(std::remove_if(data.eggs_.begin(), data.eggs_.end(), [](auto& egg) {
      return egg->GetState() == Entity::Dead;}), data.eggs_.end());
}

/*!
 * @brief Handles the reproduction process of creatures in the simulation.
 *
 * @details Pairs creatures by compatibility from the reproduction queue and
 * creates offspring with crossed-over genomes.
 */
void CreatureManager::ReproduceCreatures(SimulationData& data,
                                         Environment& environment) {
    std::queue<std::shared_ptr<Creature>> not_reproduced;

    while (!data.reproduce_.empty()) {
      auto creature1 = data.reproduce_.front();
      data.reproduce_.pop();
      bool paired = false;
      bool readded = false;  // Flag to track if creature1 is re-added to the queue

      std::queue<std::shared_ptr<Creature>> next_round_queue;
      while (!data.new_reproduce_.empty() && !paired) {
        auto creature2 = data.new_reproduce_.front();
        data.new_reproduce_.pop();

        if (creature1->Compatible(creature2) &&
            creature1->MaleReproductiveSystem::ReadyToProcreate() &&
            creature2->FemaleReproductiveSystem::ReadyToProcreate()) {
          if (creature1->GetDistance(creature2) < SETTINGS.compatibility.compatibility_distance) {
            std::cerr << "Reproducing creatures" << std::endl;
            ReproduceTwoCreatures(data, creature1, creature2);
            paired = true;
          } else {
            // Compatible but not close enough, add both back for next round
            next_round_queue.push(creature1);
            next_round_queue.push(creature2);
            readded = true;  // Mark creature1 as re-added
            break; // Break out of the loop to prevent re-pairing of creature1
          }
        } else {
          next_round_queue.push(creature2);  // Save for next round
        }
      }

      // If creature1 wasn't paired and not re-added, add it to not_reproduced
      if (!paired && !readded) {
        not_reproduced.push(creature1);
      }

      // Refill new_reproduce_ with unpaired creatures for the next attempt
      while (!next_round_queue.empty()) {
        data.new_reproduce_.push(next_round_queue.front());
        next_round_queue.pop();
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
                                            std::shared_ptr<Creature> father,
                                            std::shared_ptr<Creature> mother) {
  father->MaleReproductiveSystem::MateWithFemale();
  mother->FemaleReproductiveSystem::MateWithMale(father, mother);
  father->SetWaitingToReproduce(false);
  mother->SetWaitingToReproduce(false);
  father->MaleAfterMate();
  mother->FemaleAfterMate();
  father->IncreaseOffspringNumber();
  mother->IncreaseOffspringNumber();
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
  int creatures_genome_ = 0;
  neat::Genome genome(SETTINGS.environment.input_neurons,
                      SETTINGS.environment.output_neurons);
  for (double x = 0; x < world_width; x += 2.0) {
    for (double y = 0; y < world_height; y += 2.0) {
      if (std::rand() / (RAND_MAX + 1.0) < creature_density) {
        if(creatures_genome_ % 3 == 0){
          genome = neat::Genome(SETTINGS.environment.input_neurons,
                                SETTINGS.environment.output_neurons);
          for(int i = 0; i < 10; i++){
            genome.Mutate();
          }
        }
        creatures_genome_++;
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
