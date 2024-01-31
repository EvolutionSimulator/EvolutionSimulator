#pragma once

#include <functional>
#include <mutex>

#include "simulation/environment.h"  // Include the Environment header
#include "simulation/simulation_data.h"
#include "core/data_accessor.h"
#include "core/synchronization_primitives.h"

#include "simulation/food_manager.h"
#include "simulation/entity_grid.h"
#include "simulation/collision_manager.h"
#include "simulation/creature_manager.h"

class Simulation {
 public:
  explicit Simulation(
      Environment&
          environment);  // New constructor accepting Environment reference
  ~Simulation();
  DataAccessor<SimulationData> GetSimulationData();
  void Start();
  void Update(double deltaTime);
  void FixedUpdate(double deltaTime);
  void Stop();  // Gives us the possibility to stop the simulation

 private:
  FoodManager food_manager_;
  EntityGrid entity_grid_;
  CollisionManager collision_manager_;
  CreatureManager creature_manager_;

  SimulationData* data_;
  SynchronizationPrimitives data_sync_;

  bool is_running_;  // New boolean flag to control simulation state
};
