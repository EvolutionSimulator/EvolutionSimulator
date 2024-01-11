#pragma once

#include <functional>
#include <mutex>

#include "collision_manager.h"
#include "simulation/creature_manager.h"
#include "core/data_accessor.h"
#include "simulation/entity_grid.h"
#include "simulation/environment.h"
#include "simulation/food_manager.h"
#include "simulation/simulation_data.h"
#include "core/synchronization_primitives.h"

class Simulation {
 public:
  Simulation();
  explicit Simulation(Environment& environment);
  ~Simulation();

  DataAccessor<SimulationData> GetSimulationData();
  DataAccessor<Environment> GetEnvironment();

  void Start();
  void Update(double deltaTime);
  void FixedUpdate(double deltaTime);
  void Stop();
  void ProcessData(std::function<void(SimulationData*)> processFunc);

 private:
  SimulationData* data_;
  Environment environment_;

  FoodManager food_manager_;
  EntityGrid entity_grid_;
  CreatureManager creature_manager_;
  CollisionManager collision_manager_;

  SynchronizationPrimitives data_sync_;
  SynchronizationPrimitives environment_sync_;
  bool is_running_;
};
