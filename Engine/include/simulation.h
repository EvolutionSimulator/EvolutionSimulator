#pragma once

#include <functional>
#include <mutex>

#include "collision_manager.h"
#include "creature_manager.h"
#include "data_accessor.h"
#include "entity_grid.h"
#include "environment.h"
#include "food_manager.h"
#include "simulation_data.h"

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

  std::recursive_mutex data_mutex_;
  std::recursive_mutex environment_mutex_;
  bool is_running_;
};
