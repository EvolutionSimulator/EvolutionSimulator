#pragma once

#include "entity_grid.h"
#include "environment.h"
#include "simulationdata.h"

class CreatureManager {
 public:
  CreatureManager();

  void InitializeCreatures(SimulationData& data, Environment& environment);

  void ModifyAllCreatures(SimulationData& data, double delta_x, double delta_y);
  void UpdateAllCreatures(SimulationData& data, Environment& environment,
                          EntityGrid& entity_grid, double deltaTime);

  void HatchEggs(SimulationData& data, Environment& environment);
  void ReproduceCreatures(SimulationData& data, Environment& environment);

 private:
  void ReproduceTwoCreatures(SimulationData& data,
                             std::shared_ptr<Creature> creature1,
                             std::shared_ptr<Creature> creature2);
};
