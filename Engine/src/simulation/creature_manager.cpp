#include "simulation/creature_manager.h"

CreatureManager::CreatureManager()
{

}

/*!
 * @brief Initializes creatures randomly on the map, mutating their genome
 * multiple times.
 */
void CreatureManager::InitializeCreatures(SimulationData& data, Environment& environment) {
  // Retrieve information from the environment
  double world_width = SETTINGS.environment.map_width;
  double world_height = SETTINGS.environment.map_height;
  double creature_density = environment.GetCreatureDensity();
  double max_creature_size = SETTINGS.environment.max_creature_size;
  double min_creature_size = SETTINGS.environment.min_creature_size;

  data.creatures_.clear();

  for (double x = 0; x < world_width; x += 2.0) {
    for (double y = 0; y < world_height; y += 2.0) {
      if (std::rand() / (RAND_MAX + 1.0) < creature_density) {
        neat::Genome genome(SETTINGS.environment.input_neurons,
                            SETTINGS.environment.output_neurons);
        Mutable mutables;
        for (int i = 0; i < 40; i++) {
          genome.Mutate();
          mutables.Mutate();
        }

        Creature new_creature(genome, mutables);
        new_creature.RandomInitialization(world_width, world_height,
                                          max_creature_size, min_creature_size);
        data.creatures_.emplace_back(new_creature);
      }
    }
  }
}

/*!
 * @brief Handles the reproduction process of creatures in the simulation.
 *
 * @details Pairs creatures from the reproduction queue and creates offspring
 * with crossed-over genomes.
 */
void CreatureManager::ReproduceCreatures(SimulationData &data, Environment &environment) {
  double world_width = SETTINGS.environment.map_width;
  double world_height = SETTINGS.environment.map_height;
  double max_creature_size = SETTINGS.environment.max_creature_size;
  double min_creature_size = SETTINGS.environment.min_creature_size;

  while (data.reproduce_.size() >= 2) {
    Creature creature1 = data.reproduce_.front();
    data.reproduce_.pop();
    Creature creature2 = data.reproduce_.front();
    data.reproduce_.pop();
    double energy1 = creature1.GetEnergy();
    double energy2 = creature2.GetEnergy();
    if (energy1 > energy2) {
      neat::Genome new_genome =
          neat::Crossover(creature1.GetGenome(), creature2.GetGenome());
      new_genome.Mutate();
      new_genome.Mutate();
      Mutable new_mutable =
          MutableCrossover(creature1.GetMutable(), creature2.GetMutable());
      new_mutable.Mutate();
      new_mutable.Mutate();
      Creature new_creature(new_genome, new_mutable);
      new_creature.RandomInitialization(world_width, world_height);
      new_creature.SetGeneration(creature1.GetGeneration() + 1);
      data.creatures_.emplace_back(new_creature);
    } else {
      neat::Genome new_genome =
          neat::Crossover(creature2.GetGenome(), creature1.GetGenome());
      new_genome.Mutate();
      new_genome.Mutate();
      Mutable new_mutable =
          MutableCrossover(creature2.GetMutable(), creature1.GetMutable());
      new_mutable.Mutate();
      new_mutable.Mutate();
      Creature new_creature(new_genome, new_mutable);
      new_creature.RandomInitialization(world_width, world_height);
      new_creature.SetGeneration(creature2.GetGeneration() + 1);
      data.creatures_.emplace_back(new_creature);
    }
  }
}



/*!
 * @brief Modifies the positions of all creatures in the simulation.
 *
 * @param delta_x The change in x-coordinate for each creature.
 * @param delta_y The change in y-coordinate for each creature.
 */

void CreatureManager::ModifyAllCreatures(SimulationData &data, double delta_x, double delta_y) {
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

void CreatureManager::UpdateAllCreatures(SimulationData &data, Environment &environment, EntityGrid &entity_grid, double deltaTime) {
  auto grid = entity_grid.GetGrid();

  for (Creature& creature : data.creatures_) {
    creature.Update(deltaTime, SETTINGS.environment.map_width,
                    SETTINGS.environment.map_height, grid,
                    SETTINGS.environment.grid_cell_size, environment.GetFrictionalCoefficient());
    if (creature.Fit()) {
      data.reproduce_.push(creature);
      creature.Reproduced();
    }
  }
}
