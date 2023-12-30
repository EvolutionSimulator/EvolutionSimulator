#pragma once

#include "entities/creature.h"
#include "simulation/entity_grid.h"
#include "simulation/simulation_data.h"

class CreatureManager {
public:
  CreatureManager();

  void InitializeCreatures(SimulationData& data, Environment& environment);

  void ModifyAllCreatures(SimulationData &data, double delta_x, double delta_y);
  void UpdateAllCreatures(SimulationData& data, Environment &environment, EntityGrid& entity_grid, double deltaTime);

  void ReproduceCreatures(SimulationData& data, Environment& environment);
};
