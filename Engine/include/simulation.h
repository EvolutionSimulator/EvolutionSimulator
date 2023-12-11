#pragma once

#include <functional>
#include <mutex>

#include "environment.h"  // Include the Environment header
#include "simulationdata.h"

class Simulation {
 public:
  explicit Simulation(Environment& environment);  // New constructor accepting
                                                  // Environment reference
  ~Simulation();
  SimulationData* GetSimulationData();

  void Start();
  void Update(double deltaTime);
  void FixedUpdate(double deltaTime);
  void Stop();  // Gives us the possibility to stop the simulation
  void ProcessData(std::function<void(SimulationData*)> processFunc);

 private:
  SimulationData* data_;
  std::mutex data_mutex_;
  bool is_running_;  // New boolean flag to control simulation state
};
