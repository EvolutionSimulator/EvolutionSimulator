#pragma once

#include "simulationdata.h"
#include "environment.h"
#include "entity_grid.h"

class CreatureManager {
 public:
  CreatureManager();

  void InitializeCreatures(SimulationData& data, Environment& environment);

  void ModifyAllCreatures(SimulationData &data, double delta_x, double delta_y);
  void UpdateAllCreatures(SimulationData& data, Environment& environment, EntityGrid& entity_grid, double deltaTime);

  void ReproduceCreatures(SimulationData& data, Environment& environment);

 private:
  void ReproduceTwoCreatures(SimulationData& data, Creature& creature1, Creature& creature2);
};
