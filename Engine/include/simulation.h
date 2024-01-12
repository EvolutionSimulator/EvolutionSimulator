#pragma once

#include <functional>
#include <mutex>

#include "environment.h"  // Include the Environment header
#include "simulationdata.h"
#include "data_accessor.h"
#include "synchronization_primitives.h"

class Simulation {
 public:
  explicit Simulation(
      myEnvironment::Environment&
          environment);  // New constructor accepting Environment reference
  ~Simulation();
  DataAccessor<SimulationData> GetSimulationData();
  void Start();
  void Update(double deltaTime);
  void FixedUpdate(double deltaTime);
  void Stop();  // Gives us the possibility to stop the simulation

 private:
  SimulationData* data_;
  SynchronizationPrimitives data_sync_;
  bool is_running_;  // New boolean flag to control simulation state
};
